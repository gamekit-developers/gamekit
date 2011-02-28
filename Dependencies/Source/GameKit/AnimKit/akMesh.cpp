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

#include "akMesh.h"
#include "akMathUtils.h"


akSubMesh::akSubMesh(UTuint32 numVertices) : m_boneIndices(0), m_boneWeights(0), m_positions2(0)
{
	m_vertexBuffer.setVerticesNumber(numVertices);
	
	m_positions = new akVector3[numVertices];
	m_vertexBuffer.addElement(akVertexBuffer::VB_DU_POSITION, akVertexBuffer::VB_DT_3FLOAT32, sizeof(akVector3), m_positions);
	
	m_normals = new akVector3[numVertices];
	m_vertexBuffer.addElement(akVertexBuffer::VB_DU_NORMAL, akVertexBuffer::VB_DT_3FLOAT32, sizeof(akVector3), m_normals);

}


akSubMesh::~akSubMesh()
{
	delete[] m_positions;
	delete[] m_normals;
	
	if(m_boneIndices)
		delete[] m_boneIndices;
	
	if(m_boneWeights)
		delete[] m_boneWeights;
	
	if(m_positions2)
		delete[] m_positions2;
}


void akSubMesh::addSkinningDataBuffer(void)
{
	if(!m_boneIndices && !m_boneWeights)
	{
		m_boneIndices = new UTuint8[4 * m_vertexBuffer.getVerticesNumber()];
		m_vertexBuffer.addElement(akVertexBuffer::VB_DU_BONE_IDX, akVertexBuffer::VB_DT_4UINT8, 4*sizeof(UTuint8), m_boneIndices);
		
		m_boneWeights = new float[4 * m_vertexBuffer.getVerticesNumber()];
		m_vertexBuffer.addElement(akVertexBuffer::VB_DU_BONE_WEIGHT, akVertexBuffer::VB_DT_4FLOAT32, 4*sizeof(float), m_boneWeights);
	}
}

void akSubMesh::addSecondPositionBuffer(void)
{
	if(!m_positions2)
	{
		m_positions2 = new akVector3[m_vertexBuffer.getVerticesNumber()];
		m_vertexBuffer.addElement(akVertexBuffer::VB_DU_POSITION, akVertexBuffer::VB_DT_3FLOAT32, sizeof(akVector3), m_positions2);
	}
}

akMesh::akMesh()
{

}

akMesh::~akMesh()
{
	int i;
	for(i=0; i<m_submeshes.size(); i++)
	{
		delete m_submeshes[i];
	}
}

void akMesh::addSubMesh(akSubMesh* smesh)
{
	m_submeshes.push_back(smesh);
}

