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
#include <fstream>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <GL/gl.h>
#define STRCPY strcpy_s
#define STRCAT strcat_s
#else 
#include <GLUT/glut.h>
#define STRCPY strcpy
#define STRCAT strcat
#endif


#include "XModel.h"
#include "XMesh.h"
#include "XOptions.h"
#include "XTextureFile.h"



extern XOptions options;

XModel::XModel(void) : m_NumTextures(0)
{
	m_TextureObjects = NULL;
	m_CurrentAnimID = 0;
}

XModel::~XModel(void)
{
	if ( m_TextureObjects )
	{
		// delete tesxtures
		glDeleteTextures(m_NumTextures, (GLuint*)m_TextureObjects);
		delete [] m_TextureObjects;
	}

	// delete all meshes
	{
		XItMesh it = m_Meshes.begin();
		for( ; it != m_Meshes.end(); ++it )
		{
			delete *it;
		}
	}
}

bool XModel::LoadModel(const char* const filename)
{
	char buffer[512];
	std::ifstream ifs(filename);//, std::ifstream::binary);

	if (ifs) 
	{
		ifs >> buffer; assert( strcmp( buffer, "TEXTURES" ) == 0 );
		ifs >> buffer; assert( strcmp( buffer, "NumTextures=" ) == 0 );
		ifs >> m_NumTextures;
		if ( m_NumTextures > 0 )
		{
			m_TexNames = (char**)calloc( m_NumTextures, sizeof(char*) );
			for ( uint i = 0; i < m_NumTextures; i++ )
			{
				m_TexNames[ i ] = (char*)calloc( 256, sizeof( char ) );
				ifs >> m_TexNames[i];
			}

			// init textures
			m_TextureObjects = new unsigned int[ m_NumTextures ];
			glGenTextures(m_NumTextures, (GLuint*)m_TextureObjects);
			for ( uint i = 0; i < m_NumTextures; i++ )
			{
				// check if texture is dds or not
				if ( strstr( m_TexNames[i], ".dds" ) == NULL )
				{
					// try a texture file name with .dds extension
					char tmp[256];
					STRCPY( tmp, m_TexNames[i] );
					char* p = strrchr( tmp, '.' );
					//strcpy_s( p, 4, ".dds" );
					*p = '\0';
					STRCAT( tmp, ".dds" );
					loadCompressedTexture( m_TextureObjects[i], tmp );
				}
				else
				{
					loadCompressedTexture( m_TextureObjects[i], m_TexNames[i] );
				}
			}
		}

		ifs >> buffer; assert( strcmp( buffer, "METERIALS" ) == 0 );
		ifs >> buffer; assert( strcmp( buffer, "NumMaterials=" ) == 0 );
		ifs >> m_NumMaterials;
		for ( uint i = 0; i < m_NumMaterials; i++ )
		{
			XMaterial* mat = new XMaterial;
			assert(mat);
			ifs >> *mat;
			if (m_TextureObjects != NULL) 
			{
				mat->SetTextureObjects( m_TextureObjects );
			}
			m_Materials.push_back(mat);
		}
		
		ifs >> buffer; assert( strcmp( buffer, "MESHES" ) == 0 );
		ifs >> buffer; assert( strcmp( buffer, "NumMeshes=" ) == 0 );
		ifs >> m_NumMeshes;
		for ( uint i = 0; i < m_NumMeshes; i++ )
		{
			ifs >> buffer; assert( strcmp( buffer, "MESH" ) == 0 );
			XMesh* pm = new XMesh;
			assert (pm);
			ifs >> *pm;
			m_Meshes.push_back(pm);
		}
		
		ifs >> buffer; assert( strcmp( buffer, "SKELETON" ) == 0 );
		ifs >> m_Skeleton;

		ifs >> buffer;
		if ( strcmp( "ANIMATION", buffer ) == 0 )
		{
			XAnimation* anim = new XAnimation;
			assert (anim);
			ifs >> *anim;
			//m_CurrentAnim = anim;
			m_Anims.push_back(anim);
			ifs >> buffer;
		}

		assert( ifs.eof() );
		ifs.close();
	}
	else
	{
		std::cerr << "[ERROR] Could not open file " 
				  << filename << std::endl;
		return false;
	}
	return true;
}

bool XModel::LoadAnim(const char* const filename)
{
	char buffer[512];
	std::ifstream ifs(filename);//, std::ifstream::binary);

	if (ifs) 
	{
		ifs >> buffer;
		if ( strcmp( "ANIMATION", buffer ) == 0 )
		{
			XAnimation* anim = new XAnimation;
			assert (anim);
			ifs >> *anim;
			//m_CurrentAnim = anim;
			m_Anims.push_back(anim);
			ifs >> buffer;
		}

		assert( ifs.eof() );
		ifs.close();
	}
	else
	{
		std::cerr << "[ERROR] Could not open file " 
				  << filename << std::endl;
		return false;
	}
	return true;
}

XMesh* XModel::GetMesh( unsigned int id ) 
{
	XItMesh it = m_Meshes.begin();
	for (; it != m_Meshes.end(); ++it )
	{
		XMesh *mesh = (XMesh*)*it;
		if ( mesh->m_ID == id )
		{
			return mesh;
		}
	}
	return NULL;
}

void XModel::init()
{
	if ( m_Skeleton.m_numJoints > 0 )
	{
		m_Skeleton.updateWorldMatrices( XTransformation(), true );

		XItMesh itMesh = m_Meshes.begin();
		for( ; itMesh != m_Meshes.end(); ++itMesh )
		{
			XMesh *mesh = (*itMesh);
			if ( mesh->m_IsSkinned )
			{
				mesh->init( &m_Skeleton );		
				mesh->calculateInvBindMats();
			}
			else
			{
				mesh->m_Skeleton = &m_Skeleton;
			}
		}
	}

	bindMeshBlendShapes();

	setMaterialTransparency();

	sortTransparentObjects();

	if ( m_Anims.size() > 0 )
		m_CurrentAnim = m_Anims[ 0 ];
}


void XModel::bindSkins(void)
{
	// initialize skin's bind pose
	XItMesh itMesh = m_Meshes.begin();
	for( ; itMesh != m_Meshes.end(); ++itMesh )
	{
		XMesh *mesh = (*itMesh);
		if ( mesh->m_IsSkinned )
		{
			mesh->init( &m_Skeleton );		
			mesh->calculateInvBindMats();
		}
	}
}

void XModel::bindMeshBlendShapes()
{
	for ( uint i = 0; i < m_Anims.size(); i++ )
	{
		//for( XItBlendShape it = m_Anims[i]->m_BlendShapes.begin(); it != m_Anims[i]->m_BlendShapes.end() ; ++it ) 
		for ( int j = 0; j < m_Anims[i]->m_NumBlendShapes; j++ )
		{
			// bind base mesh
			XBlendShape *bs = m_Anims[i]->m_BlendShapes[j];//*it;
			bs->m_pTheMesh = (XMesh*)GetMesh( bs->baseMeshID );
			//std::cout << "BlendShape base " << bs->m_BaseName << " mesh " << bs->m_pTheMesh->m_MeshName << std::endl;
			bs->m_pTheMesh->allocateFullOutVerts();
			if ( bs->m_pTheMesh->m_IsSkinned )
				bs->m_pTheMesh->m_BlendShapeBinding = SKINED_BASE;
			else 
				bs->m_pTheMesh->m_BlendShapeBinding = BASE;

		}
	}
}

void XModel::setMaterialTransparency(void)
{
	for ( XItMaterial it = m_Materials.begin(); it != m_Materials.end(); it++)
	{
		XMaterial* mat = *it;
		mat->SetTransparency();
	}
}

void XModel::sortTransparentObjects()
{
	// move transparent objects to m_TranspMeshes
	std::vector< unsigned int > eraseList;
	for ( unsigned int i = 0; i < m_Meshes.size(); i++)
	{
		XMesh *mesh = m_Meshes[i];

		// ugh, a mesh could have both transparent and opaque material...
		// Well, if mesh has any transparent material, treat it as 
		// a transparent object
		if ( mesh->sortTransparentMaterials( &m_Materials ) )
		{
			m_TranspMeshes.push_back( mesh );
			// want to delete from the end, so insert from front
			eraseList.insert( eraseList.begin(), i );
		}

	}

	// move back transparent objects to the tail of m_Meshes
	assert( m_TranspMeshes.size() == eraseList.size() );
	for ( unsigned int i = 0; i < eraseList.size(); i++ )
	{
		m_Meshes.erase( m_Meshes.begin() + eraseList[i] );
		m_Meshes.push_back( m_TranspMeshes[i] );
	}
}

void XModel::setCurrentAnimation(unsigned int id) 
{
	assert(m_Anims.size() > id);
	m_CurrentAnim = m_Anims[id];
}

void XModel::nextAnimation()
{
	m_CurrentAnimID++;
	if ( m_Anims.size() <= m_CurrentAnimID )
		m_CurrentAnimID = 0;
	
	m_CurrentAnim = m_Anims[ m_CurrentAnimID ];
}

void XModel::Update(const float dt)
{
	if ( options.m_bAnimation )
	{
		if(m_CurrentAnim) 
		{
			m_CurrentAnim->Update( dt );
			if ( m_Skeleton.m_numJoints > 0 )
			{
				m_CurrentAnim->updateSkeleton( m_Skeleton );	
				m_Skeleton.updateWorldMatrices( XTransformation(), true);
			}
		}	
	}
}

void XModel::gl()
{

	glPushMatrix();

		if( options.m_bDrawJoints ) 
		{
			m_Skeleton.draw( Matrix4::identity() );
		}

		// draw meshes
		XItMesh itMesh = m_Meshes.begin();
		for( ; itMesh != m_Meshes.end(); ++itMesh )
		{
			XMesh *mesh = (*itMesh);
			if ( mesh->m_Visibility )
			{
				glPushMatrix();
					mesh->gl(m_Materials);
				glPopMatrix();
			}
		}

	glPopMatrix();

}

