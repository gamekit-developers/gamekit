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

#include "XMaterialGroup.h"

#include "GLExtensions.h"

XMaterialGroup::XMaterialGroup(void)
{
	m_indices = NULL;
}

XMaterialGroup::~XMaterialGroup(void)
{
	if( m_indices )
		delete [] m_indices;
}

/// \brief	stream insertion operator for the XMaterialGroup class
/// 
std::istream &operator>>(std::istream& ifs,XMaterialGroup& m)
{
	string data;

	while( data != string("MaterialID=") )
		ifs >> data;
	ifs >> m.m_MaterialID;

	while( data != string("TriCount") )
		ifs >> data;
	ifs >> m.m_TriCount;

	m.m_numIndices = m.m_TriCount * 3;

	m.m_indices = new uint[ m.m_numIndices ];
	for( GLuint j = 0; j < m.m_numIndices; j++ )
		ifs >> m.m_indices[j];

	// create index vbo
	glGenBuffers( 1, (GLuint*)&m.m_indicesVBO );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.m_indicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m.m_numIndices * sizeof(uint), m.m_indices, GL_STATIC_DRAW);

	// unbind VBOs
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return ifs;
}
