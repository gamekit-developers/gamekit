/*
Maya Character Exporter
Copyright (c) 2009 Jun Murakawa

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. 
   If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include <assert.h>
#include <list>
#include "MeshWriter.h"

#define CheckError(status,msg)				\
	if ( MS::kSuccess != status ) {			\
		MGlobal::displayError(msg);			\
		continue;							\
	}

MeshWriter::MeshWriter(MFnMesh& mesh)
: m_BlendShape(NULL),
m_Mesh( mesh )
{
	MStatus status = MStatus::kSuccess;

	m_Mesh.getPath ( m_DagPath );

	m_JointIndices = NULL;
	m_JointWeights = NULL;
	m_VertIndexList = NULL;
	m_PolyList =	NULL;

	m_BlendShape = NULL;
}

MeshWriter::~MeshWriter(void)
{
	if (m_VertIndexList)
		delete [] m_VertIndexList;
	
	if (m_PolyList)
		delete m_PolyList;

//	delete m_Mesh;

	if (m_JointIndices)
		delete [] m_JointIndices;
	
	if (m_JointWeights)
		delete [] m_JointWeights;
}

void MeshWriter::setSkeletonWriter(SkeletonWriter* skeleton)
{
	m_Skeleton = skeleton;
}

MStatus MeshWriter::extractPolyMesh()
{
	MStatus status = MStatus::kSuccess;

	MString meshName = m_DagPath.partialPathName(&status);
	m_MeshName = string(meshName.asChar());

	status = extractUVSet();

	status = extractMeshData();

	unsigned int polyCount = m_Mesh.numPolygons();
	m_VertIndexList = new VertIndex[polyCount * Poly::MAX_POLY_VERTS]; // At most polyCount number of sets
	m_PolyList = new PolyList(polyCount);

	status = buildIndexList();

	status = extractSkinCluster();

	return status;
}

MStatus MeshWriter::extractUVSet()
{
	MStringArray uvSetNames;
	if (MStatus::kFailure == m_Mesh.getUVSetNames(uvSetNames)) {
		MGlobal::displayError("MFnMesh::getUVSetNames"); 
		return MStatus::kFailure;
	}

	unsigned int uvSetCount = uvSetNames.length();
	unsigned int i;

	UVSet* currUVSet = NULL;

	for (i = 0; i < uvSetCount; i++ ) // for each uv set
	{ 
		if (0 == i) {
			currUVSet = new UVSet;
			m_HeadUVSet = currUVSet;
		} else {
			currUVSet->next = new UVSet;
			currUVSet = currUVSet->next;
		}

		currUVSet->name = uvSetNames[i];
		currUVSet->next = NULL;

		std::cout << "   extracting UVSet: " << currUVSet->name.asChar() << std::endl;
		// Retrieve the UV values
		if (MStatus::kFailure == m_Mesh.getUVs(currUVSet->uArray, currUVSet->vArray, &currUVSet->name)) {
			return MStatus::kFailure;
		}
	}

	if ( uvSetCount != 1 )
		std::cout << "   Warning: Multiple UVSet found but the first UVSet: " << currUVSet->name.asChar() << " will be exported " << std::endl;

	return MStatus::kSuccess;
}

MStatus MeshWriter::extractSkinCluster()
{
	// extract skin cluster
	MItDependencyNodes itSkins( MFn::kSkinClusterFilter );
	for ( ; !itSkins.isDone(); itSkins.next() )
	{
		MObject object = itSkins.thisNode();

		// For each skinCluster node, get the list of influence objects
		MFnSkinCluster skinCluster(object);
		MStatus status = MStatus::kSuccess;
		MDagPathArray infs;
		unsigned int numInfs = skinCluster.influenceObjects(infs, &status);
		CheckError(status,"Error getting influence objects.");
		if (0 == numInfs) 
		{
			status = MS::kFailure;
			CheckError(status,"Error: No influence objects found.");
		}

		// loop through the geometries affected by this cluster
		unsigned int nGeoms = skinCluster.numOutputConnections();
		for (unsigned int ii = 0; ii < nGeoms; ++ii) 
		{
			unsigned int index = skinCluster.indexForOutputConnection(ii,&status);
			CheckError(status,"Error getting geometry index.");

			// get the dag path of the ii'th geometry
			MDagPath skinPath;
			status = skinCluster.getPathAtIndex(index,skinPath);
			CheckError(status,"Error getting geometry path.");

			if (skinPath == m_DagPath)
			{
				if (NULL == m_JointIndices)
				{
					MGlobal::displayInfo("For mesh: " + m_DagPath.fullPathName() + 
						", affecting skin cluster: " + skinCluster.name() + " is found.");
					extractSkinClusterData(skinPath, skinCluster);
				}
				else
				{
					MGlobal::displayError("Multiple skin cluster found for a mesh"); 
					return MStatus::kFailure;
				}
			}
		}
	}

	return MStatus::kSuccess;
}

struct JointWeightPair
{
	int index;
	double weight;
	bool operator< ( const JointWeightPair& rhs )
	{
		return this->weight > rhs.weight; // Intensionally opposite for decending order
	}
};

void MeshWriter::extractSkinClusterData(MDagPath skinPath, MFnSkinCluster skinCluster)
{
	MStatus status = MStatus::kSuccess;

	m_InfluenceCount = skinCluster.influenceObjects(m_InfluenceObjects, &status);

	m_SkinName = skinCluster.name().asChar();

	// build joint ref
	for (int i = 0; i < m_InfluenceCount; i++)
	{
		m_InfHashes.push_back( m_Skeleton->addJoint( m_InfluenceObjects[i].node() ) );
	}
	
	// initialize geometry iterator
	MItGeometry gIter(skinPath);	

	// initialize joint indices and bone weights to 0
	m_JointIndices = new int[gIter.count() * 4];
	m_JointWeights = new double[gIter.count() * 4];
	for ( int i = 0; i < gIter.count() * 4; i++ )
	{
		m_JointIndices[i] = 0;
		m_JointWeights[i] = 0;
	}

	// iterate through the components of this geometry
	for ( /* nothing */ ; !gIter.isDone(); gIter.next() ) 
	{
		int index = gIter.index();
		MObject comp = gIter.component(&status);
		CheckError(status,"Error getting component.");

		// Get the weights for this vertex (one per influence object)
		MDoubleArray wts;
		unsigned int infCount;
		status = skinCluster.getWeights(skinPath,comp,wts,infCount);
		CheckError(status,"Error getting weights.");
		if (0 == infCount) {
			status = MS::kFailure;
			CheckError(status,"Error: 0 influence objects.");
		}

		//// Store the weight data for this vertex
		list<JointWeightPair> weightList;
		for (unsigned int i = 0; i < infCount ; ++i )
		{
			if( 0.001 <= wts[i] )
			{
				JointWeightPair pair;
				pair.index = i;
				pair.weight = wts[i];
				weightList.push_back( pair );
			}
		}

		weightList.sort();

		list<JointWeightPair>::iterator pairIt = weightList.begin();
		unsigned int count = 0;
		double sum = 0;
		for ( ; pairIt != weightList.end(); pairIt++ )
		{
			if ( count < 4 )
			{
				m_JointIndices[index*4 + count] = (*pairIt).index;
				m_JointWeights[index*4 + count] = (*pairIt).weight;
				sum += (*pairIt).weight;
			}
			count++;
		}

		if ( count > 4 )
			printf( "WARNING: %d weights are non-zero. 4 significant ones will be exported.\n", count );

		if ( sum < 1.0 )
		{
			m_JointWeights[index*4] += 1.0 - sum;
		}
	}
}

MStatus MeshWriter::extractMeshData()
{
	MStatus status = MStatus::kSuccess;

	//if (MStatus::kFailure == m_Mesh.getPoints(m_VertexArray, MSpace::kWorld)) {
	//	MGlobal::displayError("MFnMesh::getPoints"); 
	//	status = MStatus::kFailure;
	//}
    MItMeshVertex vtxIter( m_DagPath, MObject::kNullObj, &status );
    if ( MS::kSuccess != status ) 
	{
            fprintf(stderr,"Failure in MItMeshVertex initialization.\n");
            return MS::kFailure;
    }

    for ( ; !vtxIter.isDone(); vtxIter.next() ) 
	{
            MPoint p = vtxIter.position( MSpace::kObject );
            //if (ptgroups && groups) {
            //        int compIdx = vtxIter.index();
            //    outputSetsAndGroups( mdagPath, compIdx, true, objectIdx );
            //}
            // convert from internal units to the current ui units
            p.x = MDistance::internalToUI(p.x);
            p.y = MDistance::internalToUI(p.y);
            p.z = MDistance::internalToUI(p.z);
            //printf("v %f %f %f\n",p.x,p.y,p.z);
			m_VertexArray.append( p );
    }

	//if (MStatus::kFailure == m_Mesh.getFaceVertexColors(m_ColorArray)) {
	//	MGlobal::displayError("MFnMesh::getFaceVertexColors"); 
	//	status = MStatus::kFailure;
	//}

	if (MStatus::kFailure == m_Mesh.getNormals(m_NormalArray, MSpace::kWorld)) {
		MGlobal::displayError("MFnMesh::getNormals"); 
		status = MStatus::kFailure;
	}
	if (MStatus::kFailure == m_Mesh.getCurrentUVSetName(m_CurrentUVSetName)) {
		MGlobal::displayError("MFnMesh::getCurrentUVSetName"); 
		status = MStatus::kFailure;
	}

	//if (MStatus::kFailure == m_Mesh.getTangents(m_TangentArray, MSpace::kWorld, &m_CurrentUVSetName)) {
	//	MGlobal::displayError("MFnMesh::getTangents"); 
	//	status = MStatus::kFailure;
	//}

	//if (MStatus::kFailure == m_Mesh.getBinormals(m_BinormalArray, MSpace::kWorld, &m_CurrentUVSetName)) {
	//	MGlobal::displayError("MFnMesh::getBinormals"); 
	//	status = MStatus::kFailure;
	//}

	return status;
}

MStatus MeshWriter::buildIndexList()
{
	// Material ID
	
	// this will hold references to the shaders used on the meshes
	MObjectArray Shaders;

	// this is used to hold indices to the materials returned in the object array
	MIntArray    FaceIndices;

	// this function builds a list of all surface shaders used on the mesh. Note
	// that the nodes returned are the shading groups, not the material nodes!
	m_Mesh.getConnectedShaders(0,Shaders,FaceIndices);

	// having recieved the shader information, we will try to process the returned
	// shaders to find their name and ultimately find the relevant material ID that
	// references the list we have built up.
	std::vector< unsigned int > ShaderIDs;

	// start the first shader as a default zero value
	ShaderIDs.push_back(0);

	// for each surface shader
	for(int k=0;k!=Shaders.length();++k)
	{
		// attachy function set...
		MFnDependencyNode fnShader(Shaders[k]);

		// get access to the surfaceShader attribute. This will be connected to
		// lambert , phong nodes etc.
		MPlug sshader = fnShader.findPlug("surfaceShader");

		MPlugArray materials;

		// get the material connected to the surface shader
		sshader.connectedTo(materials,true,true);

		// 
		for(unsigned int l=0;l<materials.length();++l)
		{
			MFnDependencyNode fnMat(materials[l].node());
			ShaderIDs.push_back( GetMaterialID(fnMat.name()) );
		}
	}

	unsigned i, j, indexCount;

	MStatus status = MStatus::kSuccess;
	MIntArray indexArray;
	MIntArray normalIndexArray;

	// Build VertIndex list and Poly list
	int counter = 0; // count how many index set have been added to the index sets

	for (i = 0; i < m_PolyList->list.size(); i++) // for each Poly
	{ 
		Poly *poly = &m_PolyList->list[i];

		// Get vertex count, list of indices of vertices (positions), 
		// and list of indices of normals

		indexCount = m_Mesh.polygonVertexCount(i, &status);
		if (MStatus::kFailure == status) {
			MGlobal::displayError("MFnMesh::polygonVertexCount");
			return MStatus::kFailure;
		}

		status = m_Mesh.getPolygonVertices (i, indexArray);
		if (MStatus::kFailure == status) {
			MGlobal::displayError("MFnMesh::getPolygonVertices");
			return MStatus::kFailure;
		}

		status == m_Mesh.getFaceNormalIds (i, normalIndexArray);
		if (MStatus::kFailure == status) {
			MGlobal::displayError("MFnMesh::getFaceNormalIds");
			return MStatus::kFailure;
		}

		// Fill out indices in my Poly list 
		poly->numVerts = indexCount;

		for (j = 0; j < indexCount; j++) 
		{
			int uvID = -1;
			status = m_Mesh.getPolygonUVid(i, j, uvID, &m_HeadUVSet->name);
			if (MStatus::kFailure == status )  //|| uvID == -1) {
			{
				MGlobal::displayError("MFnMesh::getPolygonUVid");
				return MStatus::kFailure;
			}

			VertIndex vertIndex;
			vertIndex.pos = indexArray[j];
			vertIndex.normal = normalIndexArray[j];
			vertIndex.uv = uvID;

			// Search this vertIndex in the list so far
			int index = -1;
			for (int k = 0; k < counter; k++)
			{
				if(m_VertIndexList[k].isEqual(vertIndex))
				{
					index = k;
					break;
				}
			}

			// If vertIndex is new, add it to m_VertIndexList
			if (index == -1)
			{
				m_VertIndexList[counter] = vertIndex;
				index = counter;
				counter++;
			}

			// Add index to PolyList
			poly->indices[j] = index;
		}

		// set material index
		int shaderIdx = FaceIndices[i]+1;
		poly->matID = ShaderIDs[ shaderIdx ];
	}

	m_NumVertIndex = counter; // remember counted number of unique IndexSet

	m_PolyList->triangulate();

	return status;
}

void MeshWriter::extractMeshData( SMesh* smesh )
{
	extractVertices(smesh);
	extractTriangles( smesh );
}

void MeshWriter::extractSkin(SMesh* smesh)
{
	assert( m_InfluenceCount > 0 );
	strcpy_s( smesh->skinName, m_SkinName.c_str() );	

	smesh->numSkinInfHashes = m_InfluenceCount;

	// extract skin inf hashes
	if ( smesh->numSkinInfHashes > 0 )
	{
		smesh->skinInfHashes = (uint*)calloc( smesh->numSkinInfHashes, sizeof(uint) );
		uint infCount = 0;
		vector<uint>::iterator itInfHashes = m_InfHashes.begin();
		for ( ; itInfHashes != m_InfHashes.end(); ++itInfHashes )
		{
			smesh->skinInfHashes[ infCount ] = *itInfHashes;
			infCount++;
		}
		assert( infCount == smesh->numSkinInfHashes );
	}
}

void MeshWriter::extractVertices(SMesh* smesh)
{
	// get blendshape data
	unsigned int numWeights = 0;
	float **bsPos, **bsNml;
	if ( m_BlendShape )
	{
		numWeights = m_BlendShape->m_NumWeights;
		// get target positions and normals
		bsPos = new float*[numWeights];
		bsNml = new float*[numWeights];
		for ( unsigned int i = 0; i < numWeights; i++ )
		{
			BlendShapeWeight *weight = &m_BlendShape->m_WeightList[i];
			bsPos[i] = weight->m_Positions;
			bsNml[i] = weight->m_Normals;
		}
	}

	// determine stride
	unsigned int stride = 8; // default, no skinning, no blendshape
	stride += numWeights * 6; // blendshape, 3 for position, 3 for normal
	if ( m_JointIndices )
	{
		stride += 8; // skinning
	}

	smesh->numBlends = numWeights;
	smesh->isSkinned = m_JointIndices ? 1 : 0;
	if ( smesh->isSkinned )
	{
		extractSkin( smesh );
	}
	smesh->stride = stride;
	smesh->numVerts = m_NumVertIndex;

	unsigned int numFloat = smesh->numVerts * smesh->stride;

	// extract verts
	if ( numFloat > 0 )
	{
		smesh->verts = (float*)calloc( numFloat, sizeof(float) );//new float[ numFloat ];
		unsigned int count = 0;

		for (uint i = 0; i < smesh->numVerts; i++)
		{
			const VertIndex vertIndex = m_VertIndexList[i];

			int vertexIndex = vertIndex.pos;
			int normalIndex = vertIndex.normal;
			int uvIndex     = vertIndex.uv;

			float u = uvIndex == -1 ? -1.0f : m_HeadUVSet->uArray[uvIndex];
			float v = uvIndex == -1 ? -1.0f : m_HeadUVSet->vArray[uvIndex];

			// position and normal
			smesh->verts[ count++ ] = (float)m_VertexArray[vertexIndex].x;
			smesh->verts[ count++ ] = (float)m_VertexArray[vertexIndex].y; 
			smesh->verts[ count++ ] = (float)m_VertexArray[vertexIndex].z; 
			smesh->verts[ count++ ] = (float)m_NormalArray[normalIndex].x;
			smesh->verts[ count++ ] = (float)m_NormalArray[normalIndex].y;
			smesh->verts[ count++ ] = (float)m_NormalArray[normalIndex].z;
			// blendshape target postions and normals
			for ( unsigned int j = 0; j < smesh->numBlends; j++ )
			{
				smesh->verts[ count++ ] = (float)(bsPos[j][vertexIndex*3+0] - m_VertexArray[vertexIndex].x);
				smesh->verts[ count++ ] = (float)(bsPos[j][vertexIndex*3+1] - m_VertexArray[vertexIndex].y);
				smesh->verts[ count++ ] = (float)(bsPos[j][vertexIndex*3+2] - m_VertexArray[vertexIndex].z);
				smesh->verts[ count++ ] =  bsNml[j][normalIndex*3+0] - m_NormalArray[normalIndex].x;
				smesh->verts[ count++ ] =  bsNml[j][normalIndex*3+1] - m_NormalArray[normalIndex].y;
				smesh->verts[ count++ ] =  bsNml[j][normalIndex*3+2] - m_NormalArray[normalIndex].z;
			}
			// uv
			smesh->verts[ count++ ] =  u;
			smesh->verts[ count++ ] =  v;
			// skinning
			if ( m_JointIndices )
			{
				smesh->verts[ count++ ] = (float)m_JointIndices[vertexIndex*4 + 0]; 
				smesh->verts[ count++ ] = (float)m_JointIndices[vertexIndex*4 + 1]; 
				smesh->verts[ count++ ] = (float)m_JointIndices[vertexIndex*4 + 2]; 
				smesh->verts[ count++ ] = (float)m_JointIndices[vertexIndex*4 + 3]; 
				smesh->verts[ count++ ] = (float)m_JointWeights[vertexIndex*4 + 0]; 
				smesh->verts[ count++ ] = (float)m_JointWeights[vertexIndex*4 + 1]; 
				smesh->verts[ count++ ] = (float)m_JointWeights[vertexIndex*4 + 2]; 
				smesh->verts[ count++ ] = (float)m_JointWeights[vertexIndex*4 + 3];
			}
		}
		assert( count == numFloat );
	}
}

void MeshWriter::extractTriangles(SMesh* smesh)
{
	MaterialGroup mg( m_PolyList );
	mg.extract( smesh );
}

void MeshWriter::setBlendShape(const BlendShape* blendShape)
{
	m_BlendShape = blendShape;
}
