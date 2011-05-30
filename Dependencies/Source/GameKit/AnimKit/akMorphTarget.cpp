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

#include "akMorphTarget.h"

akMorphTarget::akMorphTarget(UTuint32 size, bool hasNormals)
{
	m_size = size;
	m_hasNormals = hasNormals;

	m_indices.resize(size);

	m_vertexOffsets.resize(size);

	if(m_hasNormals)
		m_normals.resize(size);
}

akMorphTarget::~akMorphTarget()
{
	m_indices.clear();
	m_vertexOffsets.clear();
	m_normals.clear();
}


void akMorphTarget::add(UTuint32 vertexIndex, akVector3 vertexOffset, akVector3 normal)
{
	m_indices.push_back(vertexIndex);
	m_vertexOffsets.push_back(vertexOffset);
	if(m_hasNormals)
		m_normals.push_back(normal);
	m_size++;
}

void akMorphTarget::set(UTuint32 idx, UTuint32 vertexIndex, akVector3 vertexOffset, akVector3 normal)
{
	if(idx < m_size)
	{
		m_indices[idx] = vertexIndex;
		m_vertexOffsets[idx] = vertexOffset;

		if(m_hasNormals)
			m_normals[idx] = normal;
	}
}
