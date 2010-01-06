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

#pragma once

#ifdef WIN32
	#ifndef NT_APP
		#define NT_APP
	#endif
	
	#pragma warning(disable:4786)
#endif

#include <maya/MStatus.h>
#include <maya/MString.h>
#include <maya/MTypes.h>
#include <maya/MFnMesh.h>
#include <maya/MDistance.h>
#include <maya/MItMeshVertex.h>
#include <maya/MPointArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MFnSkinCluster.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MDagPathArray.h>
#include <maya/MDagPath.h>
#include <maya/MItGeometry.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFileIO.h>
#include <maya/MLibrary.h>
#include <maya/MItDag.h>
#include <maya/MGlobal.h>
#include <maya/MPlug.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
using namespace std;

#include "PolyList.h"
#include "Material.h"
#include "BlendShape.h"
#include "SkeletonWriter.h"

#include "../SharedStructures.h"


//Used to store UV set information
struct UVSet {
	MFloatArray	uArray;
	MFloatArray	vArray;
	MString		name;
	UVSet*		next;
};


class MeshWriter
{
public:
	MeshWriter(MFnMesh& mesh);
	~MeshWriter(void);

	// Process the poly mesh
	MStatus extractPolyMesh();

	void extractMeshData( SMesh* smesh );

	void setBlendShape(const BlendShape* blendShape);
	void setSkeletonWriter(SkeletonWriter* skeleton);

private:
	// Mesh data
	MDagPath			m_DagPath;
	MFnMesh&			m_Mesh;

	// Mesh name
	string				m_MeshName;

	//for storing UV information
	UVSet*				m_HeadUVSet;

	//the current UV set's name
	MString				m_CurrentUVSetName;

	//for storing general mesh information
	MPointArray			m_VertexArray;
	//MColorArray		m_ColorArray;
	MFloatVectorArray	m_NormalArray;
	//MFloatVectorArray	m_TangentArray;
	//MFloatVectorArray	m_BinormalArray;

	//for storing skin cluster
	//MFnSkinCluster		*m_SkinCluster;
	string				m_SkinName;
	int					m_InfluenceCount;
	MDagPathArray		m_InfluenceObjects;
	vector<uint>		m_InfHashes;
	int*				m_JointIndices;
	double*				m_JointWeights;

	// For Index list and Poly list
	VertIndex*			m_VertIndexList;
	int					m_NumVertIndex;
	PolyList*			m_PolyList;

	const BlendShape*	m_BlendShape;
	SkeletonWriter*		m_Skeleton;
	// Methods

	// Extract UVSet data
	MStatus extractUVSet();

	// Extract skin cluster connected to the mesh
	MStatus extractSkinCluster();

	// process a skin cluster and store joint indices and weights into arrays
	void extractSkinClusterData(MDagPath skinPath, MFnSkinCluster skinCluster);

	// Extract mesh data
	MStatus extractMeshData();

	// Build index list and poly list
	MStatus buildIndexList();

	void extractSkin(SMesh* smesh);
	void extractVertices(SMesh* smesh);
	void extractTriangles(SMesh* smesh);
};
