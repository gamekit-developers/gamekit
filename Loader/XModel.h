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

#include <vector>
#include <list>
#include "XAnimation.h"
#include "XMaterial.h"
#include "XTextureFile.h"
#include "XBlendShape.h"
#include "XSkeleton.h"

#include "../SharedStructures.h"


typedef std::vector< XMesh* > XMeshList;
typedef XMeshList::iterator XItMesh;

typedef std::vector< XAnimation* > XAnimList;
typedef XAnimList::iterator XItAnim;

class XModel
{

public:

	XModel();
	~XModel(void);

	bool LoadModel(const char* const filename);
	bool LoadAnim(const char* const filename);

	void init();
	void setCurrentAnimation(unsigned int id);
	void nextAnimation();	

	void Update(const float dt);
	void gl();

private:

	XMesh* GetMesh( unsigned int id );
	void bindSkins(void);
	void bindMeshBlendShapes();
	void setMaterialTransparency(void);
	void sortTransparentObjects();
	
	unsigned int*	m_TextureObjects;
	unsigned int	m_NumTextures;
	char**			m_TexNames;

	uint			m_NumMaterials;
	XMaterialList	m_Materials;

	uint			m_NumMeshes;
	XMeshList		m_Meshes;
	XMeshList		m_TranspMeshes;

	XSkeleton		m_Skeleton;
	
	XAnimList		m_Anims;
	uint			m_CurrentAnimID;
	XAnimation*		m_CurrentAnim;

};


