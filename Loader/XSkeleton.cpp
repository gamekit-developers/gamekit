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
#include <stdlib.h>

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#include <GL/gl.h>
#else
#include <GLUT/GLUT.h>
#include "mm_malloc.h"
#endif

#include "XSkeleton.h"

XSkeleton::XSkeleton(void)
{
	m_jointNameHashes = NULL;
	m_parentIndices = NULL;

	m_bindXforms = NULL;
	m_animXforms = NULL;
	m_animVisibility = NULL;

	m_worldMatrices = NULL;
}

XSkeleton::~XSkeleton(void)
{
	if ( m_jointNameHashes )
		free( m_jointNameHashes );
	if ( m_parentIndices )
		free( m_parentIndices );

	if ( m_bindXforms )
		_mm_free( m_bindXforms );
	if ( m_animXforms )
		_mm_free( m_animXforms );
	if ( m_animVisibility )
		delete m_animVisibility;

	if ( m_worldMatrices )
		_mm_free( m_worldMatrices );
}

std::istream& operator >> (std::istream& ifs, XSkeleton &s)
{
	char buffer[256];
	unsigned int temp;
	int iTemp;

	ifs	>> buffer; assert( strcmp( buffer, "numJoints" ) == 0 );
	ifs >> s.m_numJoints;

	if ( s.m_numJoints > 0 )
	{
		s.m_jointNameHashes = (uint*)calloc( s.m_numJoints, sizeof( uint ) );
		s.m_parentIndices = (int*)calloc( s.m_numJoints, sizeof( int ) );

		s.m_bindXforms = (XTransformation*)_mm_malloc( sizeof(XTransformation)*s.m_numJoints, 16 );
		s.m_animXforms = (XTransformation*)_mm_malloc( sizeof(XTransformation)*s.m_numJoints, 16 );
		s.m_animVisibility = new bool[ s.m_numJoints ];

		s.m_worldMatrices = (Matrix4*)_mm_malloc( sizeof(Matrix4)*s.m_numJoints, 16 );

		for ( unsigned int i = 0; i < s.m_numJoints; i++ )
		{
			ifs >> buffer; assert( strcmp( buffer, "JOINT" ) == 0 );
			ifs >> buffer;
			s.m_jointNames.push_back( std::string( buffer ) );

			ifs >> buffer; assert( strcmp( buffer, "nameHash" ) == 0 );
			ifs >> temp;
			//s.m_jointNameHashes.push_back( temp );
			s.m_jointNameHashes[i] = temp;

			ifs >> buffer; assert( strcmp( buffer, "parentIndex" ) == 0 );
			ifs >> iTemp;
			//s.m_parentIndices.push_back( iTemp );
			s.m_parentIndices[i] = iTemp;

			// read transformations
			ifs >> buffer; assert( strcmp( buffer, "Transformation" ) == 0 );
			ifs >> s.m_bindXforms[i];

			s.m_animXforms[i] = s.m_bindXforms[i];
			s.m_animVisibility[i] = s.m_bindXforms[i].m_Visibility;
		}
	}

	return ifs;
}

void XSkeleton::updateWorldMatrices(const XTransformation& rootXform, bool rootVisibility)
{
	Matrix4 parentMatrix = rootXform.asMatrix();
	Matrix4 localMatrix = m_animXforms[0].asMatrix();
	m_worldMatrices[0] = parentMatrix * localMatrix;
	m_animVisibility[0] = rootVisibility && m_animXforms[0].m_Visibility;
	for ( uint i = 1; i < m_numJoints; i++ )
	{
		int parentIndex = m_parentIndices[i];
		parentMatrix = m_worldMatrices[ parentIndex ]; 
		localMatrix = m_animXforms[i].asMatrix();
		m_worldMatrices[i] = parentMatrix * localMatrix;

		//printf("============================================\n");
		//std::cout << m_jointNames[i].c_str() << std::endl;
		//printf("============================================\n");
		//printf("m_worldMatrices\n");
		//print( m_worldMatrices[i] );
		//printf("============================================\n");
		
		m_animVisibility[i] = m_animVisibility[ parentIndex ] && m_animXforms[i].m_Visibility;
	}
}

Matrix4 XSkeleton::getJointWorldMatrix(uint jointNameHash)
{
	for ( uint i = 0; i < m_numJoints; i++ )
	{
		if ( m_jointNameHashes[i] == jointNameHash )
			return m_worldMatrices[i];
	}
	cerr << "Error! Joint with hash " << jointNameHash << " was not found." << endl;
	assert(0); exit(1);

	return Matrix4();
}

bool XSkeleton::getJointVisibility(uint jointNameHash)
{
	for ( uint i = 0; i < m_numJoints; i++ )
	{
		if ( m_jointNameHashes[i] == jointNameHash )
			return m_animVisibility[i];
	}
	cerr << "Error! Joint with hash " << jointNameHash << " was not found." << endl;
	assert(0); exit(1);

	return false;
}

void XSkeleton::draw(const Matrix4& rootMatrix)
{
	glPushMatrix();
	glMultMatrixf( (float*)&rootMatrix );
	for ( uint i = 1; i < m_numJoints; i++ )
	{
		int parentIndex = m_parentIndices[i];
		Matrix4 parentMatrix = m_worldMatrices[ parentIndex ]; 
		XTransformation* localXform = &m_animXforms[i];

		glPushMatrix();
			glMultMatrixf( (float*)&parentMatrix );
			glDisable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);

			glColor3f(0.4f,0.8f,0.8f);
			//glColor3fv( colors[ (m_ID-1) % 6 ] );

			glBegin(GL_LINES);
				glVertex3f(0.001f,0.001f,0.001f);		
				glVertex3fv( (float*)&localXform->m_Translation );
			glEnd();

			//glutWireSphere(1,4,4);

		glPopMatrix();
	}
	glPopMatrix();
}

void XSkeleton::updateXform(uint jointNameHash, const XTransformation& transformation)
{
	for ( uint i = 0; i < m_numJoints; i++ )
	{
		if ( m_jointNameHashes[i] == jointNameHash )
		{
			m_animXforms[i] = transformation;
			return;
		}
	}
	cerr << "Error! Joint with hash " << jointNameHash << " was not found." << endl;
	assert(0); exit(1);
}
