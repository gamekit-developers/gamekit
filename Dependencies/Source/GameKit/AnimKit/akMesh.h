/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

    Contributor(s): none yet.
-------------------------------------------------------------------------------
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/

#ifndef AKMESH_H
#define AKMESH_H

#include "utCommon.h"
#include "akCommon.h"
#include "akMathUtils.h"
#include "akVertexBuffer.h"



/// Contains a vertexbuffer (description of the vertex data) and howns the memory
class akSubMesh
{
private:
	akVertexBuffer m_vertexBuffer;
	Vector3*       m_positions;
	Vector3*       m_normals;
	UTuint8*       m_boneIndices;
	float*         m_boneWeights;
	Vector3*       m_positions2;

public:
	
	akSubMesh(unsigned int numVertices);
	~akSubMesh();
	
	
	/// Allocate necesseary data to store the skinning info ( bone indices and weights)
	/// and add them to the vertexbuffer
	void addSkinningDataBuffer(void);
	
	
	/// Alocate a second postion buffer to store positon of the deformed mesh
	void addSecondPositionBuffer(void);
	
	UT_INLINE akVertexBuffer* getVertexBuffer(void)
	{
		return &m_vertexBuffer;
	}
	
};

/// A triangle mesh
/// Contains several submeshes (or section or surfaces)
class akMesh
{

private:
	utArray<akSubMesh*> m_submeshes;

public:
	akMesh();
	~akMesh();
	
	void addSubMesh(akSubMesh* mesh);
	
	UT_INLINE int getNumSubMeshes(void) const
	{
		return m_submeshes.size();
	}
	
	UT_INLINE akSubMesh* getSubMesh(int i)
	{
		return m_submeshes[i];
	}

};


#endif // AKMESH_H
