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

#ifndef XMESH__H_
#define XMESH__H_

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#endif

#include <vector>
#include <fstream>

#include "I16Aligned.h"
#include "XMaterial.h"
#include "XMaterialGroup.h"
#include "XSkeleton.h"
#include "XOptions.h"
#include "XTransformation.h"

#include "../SharedStructures.h"

//////////////////////////////////////////////////////////////////////////
enum BlendShapeBinding
{
	NONE,
	BASE,
	SKINED_BASE/*,
	TARGET*/
};

class XMaterial;
//
/// a list of material types
typedef std::vector< XMaterial* > XMaterialList;

/// an iterator type for the material list
typedef XMaterialList::iterator XItMaterial;


class XMesh : public I16Aligned
{
	friend std::istream &operator>>(std::istream&,XMesh&);

public:

	XMesh();
	~XMesh();
	
	void gl(XMaterialList& mats);
	
	const char* Name() const {
		return m_MeshName.c_str();
	}

	// blendshape
	void allocateBlendWeights();

	// skinning
	void allocateSkinningMatrices();

	void calculateInvBindMats();

	//void init(const Matrix4 &MBind);
	void init(XSkeleton* skeleton);

	void allocateFullOutVerts();

	void applyBlendshape();
	void applySkinning();

	// return true if any of material in this mesh is transparent
	bool sortTransparentMaterials( XMaterialList *allMats );

	// blend shape binding to this mesh
	BlendShapeBinding m_BlendShapeBinding;

	std::string		m_MeshName;
	unsigned int	m_ID;
	unsigned int	m_ParentHash;
	uint			m_NumBlendWeights;
	uint			m_IsSkinned;

	char			m_SkinName[256];
	uint			m_NumSkinInfs;
	uint*			m_SkinInfHashes;
	
	uint			m_Stride;
	uint			m_NumVerts;
	float*			m_verts;      // if skinned, position is in bind pose
	uint			m_vertsVBO;

	uint					m_NumMaterials;
	vector<XMaterialGroup>	m_MG_list;

	float*			m_BlendWeights; // runtime

	XSkeleton*		m_Skeleton;		// runtime
	Matrix4*		m_InvBindM4;	// runtime
	Matrix4*		m_CombinedMats;	// runtime
	Matrix3*		m_CombinedRots; // runtime

	uint			m_NumFloats;			// runtime
	uint			m_UVoffset;				// runtime
	uint			m_JointIndexOffset;		// runtime 
	uint			m_JointWeightsOffset;	// runtime
	int*			m_JointIndices;			// runtime to avoid costly float -> int conversion


	bool			m_Blendshaped;	// runtime
	float*			m_FullOutVerts; // runtime to store the final output vertices

	bool			m_Visibility;	// runtime
	Matrix4			m_AnimatedM;	// runtime
};


#endif

