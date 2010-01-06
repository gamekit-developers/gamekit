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

#ifdef WIN32
	#ifndef NT_APP
		#define NT_APP
	#endif
	#pragma warning(disable:4786)
	#include <direct.h>
#endif

#include "Texture.h"

#include <maya/MPlug.h>

#include <maya/MFnDependencyNode.h>
#include <maya/MItDependencyNodes.h>

#include <iostream>
#include <string>
#include <map>
#include <assert.h>
#include <vector>

std::map<std::string, unsigned int> g_TextureToID_Map; 
unsigned int g_TexID_gen = 0;

unsigned int GetTextureID(MString &fnn) 
{
	std::map<std::string, unsigned int>::iterator it = g_TextureToID_Map.find(fnn.asChar());

	if (it != g_TextureToID_Map.end()) 
	{
		return it->second;
	}
	return 0;
}

void ClearTextures(SModel* model)
{
	g_TextureToID_Map.clear(); g_TexID_gen = 0;

	for( uint i = 0; i < model->numTextures; i++ )
	{
		free( model->texNames[i] );
	}
	free(model->texNames);
}

void ExtractTextures(SModel* model)
{
	MItDependencyNodes itDep(MFn::kFileTexture);

	uint texCount = 0;
	// count textures
	while (!itDep.isDone()) 
	{
		//model->numTextures++;
		texCount++;
		itDep.next();
	}

	//if ( model->numTextures > 0 )
	if ( texCount > 0 )
	{
		// string for current working directory
		char cwdStr[256];
		_getcwd( cwdStr, 256 );
		for ( uint i = 0; i < strlen( cwdStr ); i++ )
		{
			if ( cwdStr[i] == '\\' )
				cwdStr[i] = '/';
		}
		size_t cwdStrLen = strlen( cwdStr );

		//model->texNames = (char**)calloc( model->numTextures, sizeof(char*) );//256 );
		std::vector<std::string> texNames;
		itDep.reset(MFn::kFileTexture);
		texCount = 0;
		while (!itDep.isDone()) 
		{
			MFnDependencyNode fnDep(itDep.item());
			MPlug filenamePlug = fnDep.findPlug("fileTextureName");
			MString textureName;
			filenamePlug.getValue(textureName);
			std::string nameString( textureName.asChar() );
			std::cout << "Found texture file " << nameString << std::endl;
			
			// find local path from current working directory
			size_t dataPos = nameString.find( cwdStr );
			if ( dataPos == std::string::npos )
			{
				std::cerr << "Warning: Texture is found at invalid directory " << nameString << std::endl;
				//assert(0); exit(1);
				itDep.next();
				continue;
			}
			std::string localPath = nameString.substr( cwdStrLen + 1 ); // + 1 for last '/'

			// extract local path name
			//model->texNames[ count ] = (char*)calloc( 256, sizeof( char ) );
			//strcpy_s( model->texNames[ count ], 256, localPath.c_str() );

			g_TextureToID_Map.insert( std::make_pair( fnDep.name().asChar(), ++g_TexID_gen) );

			texNames.push_back( localPath );

			itDep.next();
			texCount++;
		}
		//assert( count == model->numTextures );

		// extract local path name
		model->numTextures = texCount;
		model->texNames = (char**)calloc( model->numTextures, sizeof(char*) );//256 );
		for ( uint i = 0; i < model->numTextures; i++ )
		{
			model->texNames[i] = (char*)calloc( 256, sizeof( char ) );
			strcpy_s( model->texNames[i], 256, texNames[i].c_str() );

			//g_TextureToID_Map.insert( std::make_pair( fnDep.name().asChar(), ++g_TexID_gen) );
		}


	}
}

void OutputTextures(std::ostream& ofs, SModel* model)
{
	ofs << "TEXTURES" << std::endl;
	ofs << "NumTextures= " << model->numTextures << std::endl;

	for( uint i = 0; i < model->numTextures; i++ )
	{
		ofs << "\t" << model->texNames[i] << std::endl;
	}
}