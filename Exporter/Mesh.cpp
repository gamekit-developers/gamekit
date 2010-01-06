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

#ifdef WIN32
	#ifndef NT_APP
		#define NT_APP
	#endif
	
	#pragma warning(disable:4786)
#endif

#include <vector>
#include <string>
#include <map>
#include <assert.h>

#include <maya/MFloatMatrix.h>
#include <maya/MFloatVector.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MMatrix.h>
#include <maya/MPlugArray.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MFnMesh.h>
#include <maya/MFnTransform.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshVertex.h>

#include "Mesh.h"
#include "Material.h"
#include "MeshWriter.h"

#include "BlendShape.h"

/// this is a global variable that is used to generate unique object id numbers. 
unsigned int MeshRefID = 0;

/// all of the object references are stored within a map
typedef std::map<std::string, unsigned int> MeshMap;

/// the iterator type
typedef MeshMap::iterator MeshIT;

/// a map of all useful transforms
MeshMap g_Meshes;


unsigned int GetMeshID(const std::string name)
{
	// find the mesh
	MeshIT it = g_Meshes.find(name);

	// if not found
	if (it == g_Meshes.end()) 
	{
		// insert entry into mesh map
		std::pair<MeshIT,bool> pair_ = g_Meshes.insert( std::make_pair( name , ++MeshRefID  )  );
		
		return pair_.first->second;
	}
	return it->second;
}

void ClearMeshes(SModel* model)
{
	g_Meshes.clear(); MeshRefID = 0;

	for ( uint i = 0; i < model->numMeshes; i++ )
	{
		SMesh* mesh = &model->meshes[ i ];
		free( mesh->verts );
		for (uint i = 0; i < mesh->numMats; i++)
		{
			SMatGrp* matGrp = &mesh->matGrps[i];
			free( matGrp->triIndices );
		}
		free( mesh->matGrps );
	}

	Skeleton* s = &model->skeleton;
	free( s->m_parentIndices );
	free( s->m_basePose );
	free( s->m_jointNameHashes );
}

/////////////////////////////////////////////////////////////////////////////////////////
bool shouldBeExported(const MObject &obj, const BlendShapeList& blendShapeList)
{
	MFnMesh fnMesh( obj );
	std::string meshName( fnMesh.name().asChar() );

	// determine blendshape binding
	BlendShape* blendShape = NULL;
	BlendShapeBinding binding = blendShapeList.getBlendShape( meshName, &blendShape );
	if ( binding == TARGET )
		return false;  // no need to export target objects

	// ignore all 'intermediate objects'
	if ( fnMesh.isIntermediateObject() ) 
		return false;

	return true;
}

void ExtractMesh(const MObject &obj, const BlendShapeList& blendShapeList, SMesh* mesh, SkeletonWriter* skeleton )
{
	MFnMesh fnMesh( obj );
	strcpy_s( mesh->name, fnMesh.name().asChar() );

	// determine blendshape binding
	BlendShape* blendShape = NULL;
	BlendShapeBinding binding = blendShapeList.getBlendShape( mesh->name, &blendShape );

	mesh->meshID =  GetMeshID(mesh->name);

	std::cout << "extracting mesh " << mesh->name << std::endl;

	MeshWriter meshWriter( fnMesh );	
	meshWriter.setSkeletonWriter( skeleton );
	meshWriter.extractPolyMesh();
	if ( binding == BASE )
	{
		assert( blendShape );
		meshWriter.setBlendShape( blendShape );
	}
	meshWriter.extractMeshData( mesh );

	// extract parent hash
	MObject& parentObj = fnMesh.parent(0);
	assert(parentObj.hasFn(MFn::kTransform)); 
	MFnTransform fnX(parentObj);	
	const char* parentName = fnX.name().asChar();
	int parentIndex = skeleton->findJoint( parentName );
	if ( parentIndex != -1 )
		mesh->parentHash = skeleton->m_jointNameHashList[ parentIndex ];
	else
		mesh->parentHash = 0;
}

void ExtractAllMeshes( const BlendShapeList& blendShapeList, SModel* model, SkeletonWriter* skeleton )
{
	MItDependencyNodes itDep(MFn::kMesh);

	// count meshes
	model->numMeshes = 0;
	while (!itDep.isDone()) 
	{
		if ( shouldBeExported( itDep.thisNode(), blendShapeList ) )
			model->numMeshes++;
		itDep.next();
	}

	if ( model->numMeshes > 0 )
	{
		// allocate meshes
		model->meshes = (SMesh*)calloc(model->numMeshes, sizeof(SMesh));
		unsigned int count = 0;
		itDep.reset(MFn::kMesh);
		while (!itDep.isDone()) 
		{
			if ( shouldBeExported( itDep.thisNode(), blendShapeList ) )
			{
				SMesh* mesh = &model->meshes[ count ];
				//OutputMesh(ofs, itDep.thisNode(), blendShapeList, mesh);
				ExtractMesh(itDep.thisNode(), blendShapeList, mesh, skeleton);
				count++;
			}
			itDep.next();
		}
	}

	// copy skeleton writer data to skeleton in the model
	Skeleton* s = &model->skeleton;
	s->m_numJoints = skeleton->m_numJoints;
	if ( s->m_numJoints > 0 )
	{
		s->m_parentIndices = (int*)calloc(s->m_numJoints, sizeof(int));
		s->m_basePose = (Joint*)calloc(s->m_numJoints, sizeof(Joint));
		s->m_jointNameHashes = (uint*)calloc(s->m_numJoints, sizeof(uint));

		for( unsigned int i = 0; i < s->m_numJoints; i++ )
		{
			//strcpy_s( s->m_basePose[i].name, skeleton->m_jointNames[i].c_str() );
			s->m_parentIndices[i] = skeleton->m_parentIndexList[i];
			s->m_basePose[i] = skeleton->m_jointWriters[i];
			s->m_jointNameHashes[i] = skeleton->m_jointNameHashList[i];
		}
	}

}

void OutputMesh(std::ostream& ofs, SMesh* mesh )
{
	std::cout << "outputting mesh " << mesh->name << std::endl;

	ofs << "\t" << "MESH " << mesh->name << "\n";
	ofs << "\t\t" << "meshID= " << mesh->meshID << "\n";
	ofs << "\t\t" << "ParentHash= " << mesh->parentHash << std::endl;

	// output mesh info
	ofs << "\t\t" << "NumBlendshapeWeights " << mesh->numBlends << endl;
	ofs << "\t\t" << "IsSkinned " << mesh->isSkinned << endl;
	if ( mesh->isSkinned )
	{
		ofs << "\t\t\t" << "SKIN " << mesh->skinName << std::endl;
		ofs	<< "\t\t\t" << "NumInfluences " << mesh->numSkinInfHashes << std::endl;

		// output skin inf hashes
		if ( mesh->numSkinInfHashes > 0 )
		{
			ofs	<< "\t\t\t\t";
			for ( uint infCount = 0; infCount < mesh->numSkinInfHashes; infCount++ )
			{
				ofs << mesh->skinInfHashes[ infCount ] << " ";
			}
		}
		ofs << std::endl;
	}
	ofs << "\t\t" << "Stride " << mesh->stride << endl;
	ofs << "\t\t" << "// Vertices format:  ";
	ofs << "Position.xyz | Normal.xyz";
	for ( unsigned int i = 0; i < mesh->numBlends; i++ )
		ofs << " | BlendshapePos" << i << ".dxdydz | BlendShapeNormals" << i << ".dxdydz";
	ofs << " | UV";
	if ( mesh->isSkinned )
		ofs << " | JointIndices.0123 | JointWeights.0123";
	ofs << "\n";
	ofs << "\t\t" << "NumVerts " << mesh->numVerts << std::endl;

	unsigned int numFloat = mesh->numVerts * mesh->stride;

	// output verts
	if ( numFloat > 0 )
	{
		// set float format
		streamsize oldPrec = ofs.precision(5);
		ofs.setf(std::ios::fixed,std::ios::floatfield);	// floatfield set to fixed
		ofs.setf(std::ios::showpos);					// write + for positive 

		// output verts
		uint count = 0;
		for (uint i = 0; i < mesh->numVerts; i++)
		{
			ofs << "\t\t" << "\t";
			// position and normal
			ofs << mesh->verts[ count++ ] << " ";
			ofs << mesh->verts[ count++ ] << " "; 
			ofs << mesh->verts[ count++ ] << "  "; 
			ofs << mesh->verts[ count++ ] << " ";
			ofs << mesh->verts[ count++ ] << " ";
			ofs << mesh->verts[ count++ ] << "  ";
			// blendshape target postions and normals
			for ( unsigned int j = 0; j < mesh->numBlends; j++ )
			{
				ofs << mesh->verts[ count++ ] << " ";
				ofs << mesh->verts[ count++ ] << " ";
				ofs << mesh->verts[ count++ ] << "  ";
				ofs << mesh->verts[ count++ ] << " ";
				ofs << mesh->verts[ count++ ] << " ";
				ofs << mesh->verts[ count++ ] << "  ";
			}
			// uv
			ofs << mesh->verts[ count++ ] << " ";
			ofs << mesh->verts[ count++ ] << "  ";
			// skinning
			if ( mesh->isSkinned )
			{
				ofs << mesh->verts[ count++ ] << " "; 
				ofs << mesh->verts[ count++ ] << " "; 
				ofs << mesh->verts[ count++ ] << " "; 
				ofs << mesh->verts[ count++ ] << "  "; 
				ofs << mesh->verts[ count++ ] << " "; 
				ofs << mesh->verts[ count++ ] << " "; 
				ofs << mesh->verts[ count++ ] << " "; 
				ofs << mesh->verts[ count++ ];
			}
			ofs << endl;
		}
		assert( count == numFloat );

		// unset float format
		ofs.precision(oldPrec);
		ofs.unsetf(std::ios::floatfield);
		ofs.unsetf(std::ios::showpos);
	}

	ofs << "\t\t" << "//" << endl;

	// output mesh groups and triangles
	ofs << "\t\t" << "// Material Groups:\n";
	ofs << "\t\t" << "MaterialCount= " << mesh->numMats << std::endl;
	for (uint i = 0; i < mesh->numMats; i++)
	{
		SMatGrp* matGrp = &mesh->matGrps[i];
		ofs << "\t\t" << "MaterialID= " << matGrp->materialID << endl;
		// My triangles out
		ofs << "\t\t" << "// Triangles:\n";
		ofs << "\t\t" << "TriCount " << matGrp->triCount << std::endl;
		for (unsigned int i = 0; i < matGrp->triCount; i++)
		{
			ofs << "\t\t" << "\t";
			for (int j = 0; j < 3; j++)
			{
				int index = matGrp->triIndices[ i*3 + j ];
				ofs << index << " ";
			}
			ofs << std::endl;
		}
	}
}

void OutputAllMeshes( std::ostream& ofs, SModel* model )
{
	ofs << "MESHES" << std::endl;
	ofs << "NumMeshes= " << model->numMeshes << std::endl;

	for ( uint i = 0; i < model->numMeshes; i++ )
	{
		SMesh* mesh = &model->meshes[ i ];
		OutputMesh(ofs, mesh);
	}

	// output skeleton
	Skeleton* s = &model->skeleton;
	ofs << "SKELETON" << std::endl;
	ofs << "numJoints " << s->m_numJoints << std::endl;
	for( unsigned int i = 0; i < s->m_numJoints; i++ )
	{
		Joint* j = &s->m_basePose[i];

		ofs << "\t" << "JOINT " << j->name << std::endl;
		ofs << "\t\t" << "nameHash        " << s->m_jointNameHashes[i] << std::endl;
		ofs << "\t\t" << "parentIndex     " << s->m_parentIndices[i] << std::endl;
		ofs << "\t\t" << "Transformation  ";

		//JointWriter joint = m_jointWriters[i];
		//joint.writeJointTransformation( ofs );

		// set float format
		std::streamsize oldPrec = ofs.precision(5);
		ofs.setf(std::ios::fixed,std::ios::floatfield);	// floatfield set to fixed
		ofs.setf(std::ios::showpos);					// write + for positive 

		ofs << j->m_visibility << "    " 
			<< j->m_rotation[0] << " " << j->m_rotation[1] << " " << j->m_rotation[2] << " " << j->m_rotation[3] << "    " 
			<< j->m_translation[0] << " " << j->m_translation[1] << " " << j->m_translation[2] << "    " 
			<< j->m_scale[0] << " " << j->m_scale[1] << " " << j->m_scale[2] << std::endl;

		// unset float format
		ofs.precision(oldPrec);
		ofs.unsetf(std::ios::floatfield);
		ofs.unsetf(std::ios::showpos);
	}
}