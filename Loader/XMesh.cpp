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
#include <string>

#include "XMesh.h"
#include "GLExtensions.h"

#ifdef WIN32
#include "XCgFX.h"
extern XCgFX cgfx;
#else
#include "mm_malloc.h"
#endif

using namespace std;

extern XOptions options;

XMesh::XMesh() 
{
	m_verts = NULL;
	m_FullOutVerts = NULL;
	m_BlendShapeBinding = NONE;
	m_Blendshaped = false;

	// blendshape
	m_BlendWeights = NULL;

	// skinning
	m_SkinInfHashes = NULL;
	m_InvBindM4 = NULL;
	m_CombinedMats = NULL;
	m_CombinedRots = NULL;

	m_AnimatedM = Matrix4::identity();
	m_Visibility = true;
};

XMesh::~XMesh() 
{
	if ( m_verts )
		_mm_free( m_verts );
	if( m_FullOutVerts )
		_mm_free( m_FullOutVerts );

	// blendshape
	if (m_BlendWeights)
		_mm_free( m_BlendWeights );

	// skinning
	if (m_SkinInfHashes)
		free(m_SkinInfHashes);
	if (m_InvBindM4)
		_mm_free( m_InvBindM4 );
	if (m_CombinedMats)
		_mm_free( m_CombinedMats );
	if (m_CombinedRots)
		_mm_free( m_CombinedRots );

};

void XMesh::allocateBlendWeights()
{
	m_BlendWeights = (float*)_mm_malloc( sizeof(float) * m_NumBlendWeights, 16 );
	memset( m_BlendWeights, 0, sizeof(float) * m_NumBlendWeights );
}

void XMesh::allocateFullOutVerts()
{
	if( !m_FullOutVerts )
	{
		m_FullOutVerts = (float*)_mm_malloc( sizeof( float ) * m_NumFloats, 16 );
		memcpy( m_FullOutVerts, m_verts, sizeof(float)*m_NumFloats );
	}
}

void XMesh::allocateSkinningMatrices()
{
	m_InvBindM4 = (Matrix4*)_mm_malloc( sizeof( Matrix4 ) * m_NumSkinInfs, 16 );
	m_CombinedMats = (Matrix4*)_mm_malloc( sizeof( Matrix4 ) * m_NumSkinInfs, 16 );
	m_CombinedRots = (Matrix3*)_mm_malloc( sizeof( Matrix3 ) * m_NumSkinInfs, 16 );
}

std::istream &operator>>(std::istream& ifs,XMesh& m)
{
	char buffer[256];

	// read mesh name
	ifs >> buffer;
	m.m_MeshName = buffer;

	ifs >> buffer; assert( strcmp( buffer, "meshID=" ) == 0 );
	ifs >> m.m_ID;

	ifs >> buffer; assert( strcmp( buffer, "ParentHash=" ) == 0 );
	ifs >> m.m_ParentHash;

	string data;

	while( data != string("NumBlendshapeWeights") )
		ifs >> data;
	ifs >> m.m_NumBlendWeights;

	while( data != string("IsSkinned") )
		ifs >> data;
	ifs >> m.m_IsSkinned;

	if ( m.m_IsSkinned )
	{
		ifs >> buffer; assert( strcmp( buffer, "SKIN" ) == 0 );
		ifs >> m.m_SkinName;
		ifs >> buffer; assert( strcmp( buffer, "NumInfluences" ) == 0 );
		ifs >> m.m_NumSkinInfs;

		m.m_SkinInfHashes = (uint*)calloc( m.m_NumSkinInfs, sizeof(uint) );
		for ( uint i = 0; i < m.m_NumSkinInfs; i++ )
			ifs >> m.m_SkinInfHashes[i];
	}

	while( data != string("Stride") )
		ifs >> data;
	ifs >> m.m_Stride;

	while( data != string("NumVerts") )
		ifs >> data;
	ifs >> m.m_NumVerts;

	m.m_NumFloats = m.m_NumVerts * m.m_Stride;
	m.m_UVoffset = 6 + m.m_NumBlendWeights * 6;
	m.m_JointIndexOffset =  m.m_UVoffset + 2;
	m.m_JointWeightsOffset = m.m_JointIndexOffset + 4;

	m.m_verts = (float*)_mm_malloc( sizeof( float ) * m.m_NumFloats, 16 );
	for ( GLuint j = 0; j < m.m_NumFloats; j++ )
		ifs >> m.m_verts[j];

	if ( m.m_IsSkinned )
	{
		// convert joint indices to separate int array
		m.m_JointIndices = new int[ m.m_NumVerts * 4 ];
		for (GLuint i = 0; i < m.m_NumVerts; i++)
			for ( int j = 0; j < 4; j++)
				m.m_JointIndices[i*4+j] = (int)m.m_verts[ i * m.m_Stride + m.m_JointIndexOffset + j ];
	}

	// read material groups
	while( data != string("MaterialCount=") )
		ifs >> data;
	ifs >> m.m_NumMaterials;

	m.m_MG_list.resize( m.m_NumMaterials );
	for ( GLuint j = 0; j < m.m_NumMaterials; j++ )
		ifs >> m.m_MG_list[j];

	// create VBOs
	glGenBuffers( 1, (GLuint*)&m.m_vertsVBO );
	glBindBuffer(GL_ARRAY_BUFFER, m.m_vertsVBO);
	glBufferData(GL_ARRAY_BUFFER, m.m_NumFloats * sizeof(GLfloat), m.m_verts, GL_STATIC_DRAW);

	// unbind VBOs
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return ifs;
}

void XMesh::init(XSkeleton* skeleton)
{
	m_Skeleton = skeleton;

	Matrix4 BindMat = Matrix4::identity();
	if ( m_ParentHash )
		BindMat = m_Skeleton->getJointWorldMatrix( m_ParentHash );

	allocateBlendWeights();

	allocateSkinningMatrices();

	// allocate space for deformation
	allocateFullOutVerts();

	// update verts and position in vbo data to the bind pose
	for (GLuint i = 0; i < m_NumVerts; i++)
	{
		Vector4 V( 
			m_verts[ i * m_Stride + 0 ],
			m_verts[ i * m_Stride + 1 ],
			m_verts[ i * m_Stride + 2 ],
			1.0f);

		// calculate the world space position of the vertex when it was actually bound.
		Vector4 WSV = BindMat*V;
		
		m_verts[ i * m_Stride + 0 ] = WSV.getX();
		m_verts[ i * m_Stride + 1 ] = WSV.getY();
		m_verts[ i * m_Stride + 2 ] = WSV.getZ();

		m_FullOutVerts[ i * m_Stride + 0 ] = WSV.getX();
		m_FullOutVerts[ i * m_Stride + 1 ] = WSV.getY();
		m_FullOutVerts[ i * m_Stride + 2 ] = WSV.getZ();
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_vertsVBO);
	glBufferData(GL_ARRAY_BUFFER, m_NumFloats * sizeof(GLfloat), m_FullOutVerts, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void XMesh::calculateInvBindMats() 
{
	for (size_t i = 0; i < m_NumSkinInfs; i++ )
	{
		uint infHash = m_SkinInfHashes[i];
		Matrix4 world = m_Skeleton->getJointWorldMatrix( infHash );
		float d = determinant( world ); 
		if ( fabs(d) < 0.01f )
		{
			std::cerr << "COULD NOT INVERT MATRIX!!!" << std::endl;
			assert(0);
			exit(1);
		}
		Matrix4 Inv = inverse( world );
		m_InvBindM4[i] = Inv;
	}
}

bool XMesh::sortTransparentMaterials( XMaterialList *allMats )
{
	for ( GLuint i = 0; i < m_NumMaterials; i++ )
	{
		const XMaterialGroup &mg = m_MG_list[i];
		int matID = mg.m_MaterialID;
		
		if ( matID > 0 )
		{
			XMaterial *mat = (*allMats)[ matID-1 ];

			if ( mat->m_IsTransparent )
				return true;
		}
	}
	return false;
}

void XMesh::applyBlendshape()
{
	// update mesh using current weights
	for (GLuint i = 0; i < m_NumVerts; i++)
	{
		// weighted sum of targets

		// position
		Vector4 baseV(
			m_verts[ i * m_Stride + 0 ],
			m_verts[ i * m_Stride + 1 ],
			m_verts[ i * m_Stride + 2 ],
			0);
		//Matrix4 targetM(0.0f);
		for ( unsigned int j = 0; j < m_NumBlendWeights; j++ )
		{
			Vector4 targetV(
				m_verts[ i * m_Stride + 6 + j*6 + 0 ],
				m_verts[ i * m_Stride + 6 + j*6 + 1 ],
				m_verts[ i * m_Stride + 6 + j*6 + 2 ],
				0);
			//targetM.setCol( j, targetV );
			baseV += targetV * m_BlendWeights[j];
		}
		//baseV += targetM * currentWeights;
		m_FullOutVerts[ i * m_Stride + 0 ] = baseV.getX();
		m_FullOutVerts[ i * m_Stride + 1 ] = baseV.getY();
		m_FullOutVerts[ i * m_Stride + 2 ] = baseV.getZ();

		// normal
		baseV = Vector4(
			m_verts[ i * m_Stride + 3 + 0 ],
			m_verts[ i * m_Stride + 3 + 1 ],
			m_verts[ i * m_Stride + 3 + 2 ],
			0);
		//targetM = Matrix4(0.0f);
		for ( unsigned int j = 0; j < m_NumBlendWeights; j++ )
		{
			Vector4 targetV(
				m_verts[ i * m_Stride + 6 + j*6 + 3 + 0 ],
				m_verts[ i * m_Stride + 6 + j*6 + 3 + 1 ],
				m_verts[ i * m_Stride + 6 + j*6 + 3 + 2 ],
				0);
			//targetM.setCol( j, targetV );
			baseV += targetV * m_BlendWeights[j];
		}
		//baseV += targetM * currentWeights;
		m_FullOutVerts[ i * m_Stride + 3 + 0 ] = baseV.getX();
		m_FullOutVerts[ i * m_Stride + 3 + 1 ] = baseV.getY();
		m_FullOutVerts[ i * m_Stride + 3 + 2 ] = baseV.getZ();
	}

	if ( m_BlendShapeBinding == SKINED_BASE )
	{
		m_Blendshaped = true;
	}
	else if (  m_BlendShapeBinding == BASE )
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertsVBO);
		glBufferData(GL_ARRAY_BUFFER, m_NumFloats * sizeof(GLfloat), m_FullOutVerts, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	else
	{
		assert(0); // no blenshape binding
	}
}

void XMesh::applySkinning()
{
	for (size_t i = 0; i < m_NumSkinInfs; i++ )
	{
		Matrix4 W4 = m_Skeleton->getJointWorldMatrix( m_SkinInfHashes[i] );

		Matrix4 Inv4 = m_InvBindM4[i];
		m_CombinedMats[i] = W4 * Inv4;
		m_CombinedRots[i] = m_CombinedMats[i].getUpper3x3();
	}

	// update the position in vbo data
	for (GLuint i = 0; i < m_NumVerts; i++)
	{
		Vector4 sum( 0.0f, 0.0f, 0.0f, 0.0f );
		// position
		if (m_Blendshaped)
		{
			Vector4 V(
				m_FullOutVerts[ i * m_Stride + 0 ],
				m_FullOutVerts[ i * m_Stride + 1 ],
				m_FullOutVerts[ i * m_Stride + 2 ],
				1.0f);
			for ( int j = 0; j < 4; j++)
			{
				int idx = m_JointIndices[ i*4 + j ];
				float weight = m_verts[ i * m_Stride + m_JointWeightsOffset + j];
				sum += m_CombinedMats[idx] * V * weight;
			}
		}
		else // no blendshape
		{
			Vector4 V(
				m_verts[ i * m_Stride + 0 ],
				m_verts[ i * m_Stride + 1 ],
				m_verts[ i * m_Stride + 2 ],
				1.0f);
			for ( int j = 0; j < 4; j++)
			{
				int idx = m_JointIndices[ i*4 + j ];
				float weight = m_verts[ i * m_Stride + m_JointWeightsOffset + j];
				sum += m_CombinedMats[idx] * V * weight;
			}
		}
		// normal
		Vector3 sumNormal( 0.0f );
		if (m_Blendshaped)
		{
			Vector3 V(
				m_FullOutVerts[ i * m_Stride + 3 + 0 ],
				m_FullOutVerts[ i * m_Stride + 3 + 1 ],
				m_FullOutVerts[ i * m_Stride + 3 + 2 ]);
			for ( int j = 0; j < 4; j++)
			{
				int idx = m_JointIndices[ i*4 + j ];
				float weight = m_verts[ i * m_Stride + m_JointWeightsOffset + j];
				sumNormal += m_CombinedRots[idx] * V * weight;
			}
		}
		else // no blendshape
		{
			Vector3 V(
				m_verts[ i * m_Stride + 3 + 0 ],
				m_verts[ i * m_Stride + 3 + 1 ],
				m_verts[ i * m_Stride + 3 + 2 ]);
			for ( int j = 0; j < 4; j++)
			{
				//int idx = m_verts[ i * m_Stride + 8 + j];
				int idx = m_JointIndices[ i*4 + j ];
				float weight = m_verts[ i * m_Stride + m_JointWeightsOffset + j];
				sumNormal += m_CombinedRots[idx] * V * weight;
			}
		}
		sumNormal = normalize( sumNormal );

		m_FullOutVerts[ i * m_Stride + 0 ] = sum.getX();
		m_FullOutVerts[ i * m_Stride + 1 ] = sum.getY();
		m_FullOutVerts[ i * m_Stride + 2 ] = sum.getZ();
		m_FullOutVerts[ i * m_Stride + 3 + 0 ] = sumNormal.getX();
		m_FullOutVerts[ i * m_Stride + 3 + 1 ] = sumNormal.getY();
		m_FullOutVerts[ i * m_Stride + 3 + 2 ] = sumNormal.getZ();
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vertsVBO);
	glBufferData(GL_ARRAY_BUFFER, m_NumFloats * sizeof(GLfloat), m_FullOutVerts, GL_STATIC_DRAW);
	//glBufferSubData(GL_ARRAY_BUFFER, 0, m_NumFloats * sizeof(GLfloat), m_FullOutVerts);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_Blendshaped = false;
}

void XMesh::gl(XMaterialList& mats) 
{
	
	if( !options.m_bDrawMeshes )
		return;

	if ( m_NumBlendWeights > 0 )
		applyBlendshape();

	if ( m_IsSkinned )
		applySkinning();
	else // not skinned
	{
		m_AnimatedM = Matrix4::identity();
		if ( m_ParentHash )
		{
			m_AnimatedM = m_Skeleton->getJointWorldMatrix( m_ParentHash );
			bool visibility = m_Skeleton->getJointVisibility( m_ParentHash );
			if ( !visibility )
				return;
		}
		glMultMatrixf( (float*)&m_AnimatedM );
	}

	for ( GLuint i = 0; i < m_NumMaterials; i++ )
	{
		const XMaterialGroup &mg = m_MG_list[i];
		int matID = mg.m_MaterialID;
		
		// check if this material group has material or not
		//assert( matID > 0 ); 
		if ( matID == 0 )
			continue;

		XMaterial *mat = mats[ matID-1 ];

		// set material
#ifdef WIN32
		cgfx.bindMaterial( mat );
#else 
		// fixed function fallback 
		glEnable( GL_LIGHTING );
		glEnable( GL_LIGHT0 );
		glEnable( GL_COLOR_MATERIAL );
		glColor4fv( mat->m_Diffuse.m_color );
		uint diffuseTex = mat->m_Diffuse.m_TextureObject;
		if ( diffuseTex )
		{
			glEnable( GL_TEXTURE_2D );
			glBindTexture( GL_TEXTURE_2D, diffuseTex );
			//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
			float white[4] = {1,1,1,1};
			glColor4fv( white );
		}
		if ( mat->m_IsTransparent )
		{
//			glEnable( GL_BLEND );
//			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR );
			glEnable( GL_ALPHA_TEST );
			glAlphaFunc( GL_GREATER, 0.1f );
		}
		else
		{
//			glDisable( GL_BLEND );
			glDisable( GL_ALPHA_TEST );
		}
		// flip y for DXT texture 
		glMatrixMode( GL_TEXTURE );
		glLoadIdentity();
		glScalef( 1, -1, 1 );
		//glTranslatef( 0, 1, 0 );
		glMatrixMode(GL_MODELVIEW);
#endif
			// Enable Pointers
			glEnableClientState( GL_VERTEX_ARRAY );	
			glEnableClientState( GL_NORMAL_ARRAY );	
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );

			// Bind VBOs
			glBindBuffer( GL_ARRAY_BUFFER_ARB, m_vertsVBO );
			glVertexPointer(  3, GL_FLOAT, m_Stride*sizeof(float), (GLvoid*)(0)			   );					
			glNormalPointer( 	 GL_FLOAT, m_Stride*sizeof(float), (GLvoid*)(3*sizeof(float)) );
			glTexCoordPointer(2, GL_FLOAT, m_Stride*sizeof(float), (GLvoid*)(m_UVoffset*sizeof(float)) );
			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mg.m_indicesVBO );

			//glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			glDrawElements( GL_TRIANGLES, mg.m_numIndices, GL_UNSIGNED_INT, 0 );

			// Unbind VBOs
			glBindBuffer( GL_ARRAY_BUFFER_ARB, 0 );
			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

			// Disable Pointers
			glDisableClientState( GL_VERTEX_ARRAY );			
			glDisableClientState( GL_NORMAL_ARRAY );			
			glDisableClientState( GL_TEXTURE_COORD_ARRAY );

#ifdef WIN32
		cgfx.unbindMaterial( mat );
#endif

	}

	if ( options.m_bNormals )
	{
		float *normalLines = new float[ 3 * 2 * m_NumVerts ];
		for ( unsigned int i = 0; i < m_NumVerts; i++ )
		{
			if (m_FullOutVerts)
				memcpy( &normalLines[i*6], &m_FullOutVerts[i*m_Stride], sizeof(float)*3*2 );
			else
				memcpy( &normalLines[i*6], &m_verts[i*m_Stride], sizeof(float)*3*2 );

			for ( int j = 0; j < 3; j++ )
			{
				normalLines[i*6 + 3 + j] += normalLines[i*6 + j];
			}
		}
		// Enable Pointers
		glEnableClientState( GL_VERTEX_ARRAY );	

		// Bind VBOs
		glVertexPointer( 3, GL_FLOAT, 0, (GLvoid*)normalLines );

		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glColor3f(1,0,0); // red
		glDrawArrays( GL_LINES, 0, m_NumVerts*2 );

		// Unbind VBOs
		glBindBuffer( GL_ARRAY_BUFFER_ARB, 0 );

		// Disable Pointers
		glDisableClientState( GL_VERTEX_ARRAY );			

		delete [] normalLines;
	}
}

