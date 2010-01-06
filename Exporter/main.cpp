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

	#pragma comment(lib,"Foundation.lib")
	#pragma comment(lib,"OpenMaya.lib")
	#pragma comment(lib,"Image.lib")
	#pragma comment(lib,"OpenMayaAnim.lib")
#endif

#include <vector>
#include <ostream>

#include <maya/MLibrary.h>
#include <maya/MFileIO.h>

#include <algorithm>
#include <fstream>
#include <string>
#include <map>
#include <assert.h>

#include "../SharedStructures.h"
#include "Material.h"
#include "Mesh.h"
#include "Texture.h"
#include "SkeletonWriter.h"
#include "BlendShape.h"
#include "BlendShapeList.h"
#include "Animation.h"

#define TYPE_MODEL	 0
#define TYPE_ANIM    1
#define TYPE_CAMERA  2

struct InputFile
{
	uint			type;
	std::string		inFileName;
	std::string		outFileName;
};

int main(int argc,char** argv)
{
	std::string configFileName;

	if (argc == 2 )
	{
		configFileName  = argv[1];
	}
	else {
		printf("Usage: Exporter CONFIG_FILE_NAME \n");
		return 0;
	}

	// check file extension
	std::string key (".conf");
	size_t found = configFileName.rfind(key);
	if ( found == std::string::npos )
	{
		std::cerr << "Error! Config file name has to have .conf as extension" << std::endl;
		assert(0); exit(1);
	}

	// open config file
	std::ifstream ifs( configFileName.c_str() );
	if (!ifs)
	{
		std::cerr << "Error! Could not open config file" << std::endl;
		assert(0); exit(1);
	}


	// read config file
	char buffer[256];
	std::vector<InputFile> fileList;
	while( !ifs.eof() )
	{
		ifs >> buffer;
		if ( strlen( buffer ) > 0 )
		{
			//std::cout << buffer << std::endl;
			InputFile file;
			uint extLength = 2; // for .mb or .ma
			if ( strcmp( buffer, "model" ) == 0 )
				file.type = TYPE_MODEL;
			else if ( strcmp( buffer, "anim" ) == 0 )
				file.type = TYPE_ANIM;
			else if ( strcmp( buffer, "camera" ) == 0 )
			{
				file.type = TYPE_CAMERA;
				extLength = 6;
			}
			else
			{
				std::cerr << "Error! Unrecognized file type " << buffer << " in config file." << std::endl;
				assert(0); exit(1);
			}

			// generate output file name
			ifs >> file.inFileName;
			size_t nameLength = file.inFileName.length();
			std::string name = file.inFileName.substr( 0, nameLength - extLength );
			file.outFileName = name + buffer;

			fileList.push_back( file );
		}
	}

	// output scene file
	std::string sceneFileName = configFileName.substr( 0, found );
	sceneFileName += ".scene";
	std::ofstream ofs( sceneFileName.c_str() );
	if (!ofs) 
	{
		std::cerr << "Could not open scene file " << sceneFileName.c_str() << " for output." << std::endl;
		assert(0); exit(1);				
	}	
	for ( size_t i = 0; i < fileList.size(); i++ )
	{
		InputFile file = fileList[i];
		if ( file.type == TYPE_MODEL )
		{
			ofs << "model  "; 
		}
		else if ( file.type == TYPE_ANIM )
		{
			ofs << "anim   ";
		}
		else // TYPE_CAMERA
		{
			ofs << "camera ";
		}
		ofs << file.outFileName;
		ofs << "\n";
	}		
	ofs.close();

	// initialise the maya library - This basically starts up Maya
	std::cout << "Initializing Maya library" << std::endl;
	MLibrary::initialize(argv[0]);

	// process maya files in config files
	for ( size_t i = 0; i < fileList.size(); i++ )
	{
		InputFile file = fileList[i];

		if (file.type == TYPE_CAMERA )
			continue;
	
		char fileNameStr[256];

		// replace \ to /
		_getcwd( fileNameStr, 256 );
		strcat_s( fileNameStr, "/" );
		strcat_s( fileNameStr, file.inFileName.c_str() );
		for ( uint i = 0; i < strlen( fileNameStr ); i++ )
		{
			if ( fileNameStr[i] == '\\' )
				fileNameStr[i] = '/';
		}

		std::cout << "Opening Maya file " << fileNameStr << std::endl;
		MStatus status = MFileIO::open(fileNameStr,NULL,true);
		if( MS::kSuccess != status )
		{
			std::cerr << "Could not open Maya file " << fileNameStr << " " << status.errorString().asChar() << std::endl;
			assert(0); exit(1);
		}

		// create an output file stream
		std::ofstream ofs(file.outFileName.c_str());
		if (!ofs) {
			std::cerr << "Could not open output file " << file.outFileName.c_str() << std::endl;
			MLibrary::cleanup();
			assert(0); exit(1);				
		}

		std::cout << "Extracting model data " << std::endl;

		DisableBlendShapes();

		BlendShapeList blendShapes;
		blendShapes.extractAllBlendShapes();

		SModel sModel;
		memset( &sModel, 0, sizeof( SModel ) );
		SkeletonWriter skeleton;

		ExtractTextures( &sModel );
		ExtractMaterials( &sModel );
		ExtractAllMeshes( blendShapes, &sModel, &skeleton);

		if ( file.type == TYPE_MODEL )
		{
			std::cout << "Exporting model to " << file.outFileName << std::endl;
			OutputTextures(ofs, &sModel );
			OutputMaterials(ofs, &sModel);
			OutputAllMeshes(ofs, &sModel);
			//skeleton.OutputSkeleton(ofs);
			ofs << "\n";
		}

		std::cout << "Exporting anim to " << file.outFileName << std::endl;
		SAnimation sAnim;
		memset( &sAnim, 0, sizeof( SAnimation ) );
		OutputAnimation( ofs, blendShapes, &sAnim, skeleton );
		ofs << "\n";

		EnableBlendShapes();

		ClearTextures( &sModel );
		ClearMaterials( &sModel );
		ClearMeshes( &sModel );

		// close the output file
		ofs.close();
	}

	// cleanup maya data
	MLibrary::cleanup();

	return 0;
}

