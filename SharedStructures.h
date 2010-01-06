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

typedef unsigned int  uint;

// Model

#define MATERIAL_LAMBERT	0
#define MATERIAL_PHONG		1
#define MATERIAL_BLINN		2

static const char* SMatNames[3] = { "LAMBERT", "PHONG", "BLINN" };

struct SMatAttrib
{
	float	color[3];
	uint	textureID;
};

struct SMaterial
{
	uint		type;
	char		name[256];			
	uint		materialID;
	SMatAttrib	ambient;
	SMatAttrib	diffuse;
	SMatAttrib	incandescence;
	SMatAttrib	transparency;
	SMatAttrib	specular;
	float		shininess; 
	float		eccentricity;
	float		specularRollOff;
	uint		normalmapTex;
};

struct SMatGrp
{
	uint	materialID;
	uint	triCount;			uint*	triIndices;
};

struct SMesh
{
	char	name[256];			
	uint	meshID; 
	uint	parentHash;
	uint	numBlends;
	uint	isSkinned;
	char	skinName[256];
	uint	numSkinInfHashes;	uint*		skinInfHashes;
	uint	stride;
	uint	numVerts;			float*		verts;
	uint	numMats;			SMatGrp*	matGrps;
};

struct Joint
{
	char	name[256];
	uint	m_visibility;
    float	m_rotation[4];             // joint rotation (quaternion)
    float	m_translation[4];          // joint translation 
    float	m_scale[4];                // joint scale
};

struct Skeleton
{
	uint	m_numJoints;
	int*	m_parentIndices;    
	Joint*	m_basePose;
	uint*	m_jointNameHashes;
};

struct SModel
{
	char	name[256];
	uint	numTextures;		char**		texNames;
	uint	numMaterials;		SMaterial*	materials;
	uint	numMeshes;			SMesh*		meshes;
	Skeleton	skeleton;
};

// Animation

struct SBlendAnim
{
	char	name[256];
	char	baseName[256];
	uint	baseMeshID;
	uint	numWeights;			float*	weights;
};

struct SJointAnim
{
	uint	numJointAnims;		
	char**	jointNames;
	uint*	jointNameHashes;
	Joint*	jointXforms;
};

struct SAnimation
{
	char	name[256];
	uint	numFrames;
	float	startTime;		float			endTime;		
	uint	numBlendAnims;	SBlendAnim*		blendAnims;
	SJointAnim	jointAnim;
};

