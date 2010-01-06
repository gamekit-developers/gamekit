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
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#endif

#include <assert.h>
#include <iostream>
#include "XMaterial.h"

XMaterial::XMaterial(void)
{
	m_IsTransparent = false;
}

XMaterial::~XMaterial(void)
{
}

std::istream& operator >> (std::istream& ifs, XMaterial& mat)
{
	char buffer[256];

	ifs >> buffer; assert( strcmp( buffer, "MaterialType=" ) == 0 );
	ifs >> mat.m_MaterialType;
	ifs >> buffer; // "LAMBERT", "PHONG", "BLINN"

	if ( strcmp( buffer, SMatNames[ mat.m_MaterialType ] ) != 0 )
	{
		std::cerr << "Error: Unrecognized material type " << mat.m_MaterialName.c_str() << std::endl;
	}

	ifs >> buffer; // name
	mat.m_MaterialName = buffer;
	
	ifs >> buffer;	assert( strcmp(buffer, "materialID=") == 0 );
	ifs	>> mat.m_MaterialID;

	ifs	>> mat.m_Ambient
		>> mat.m_Diffuse
		>> mat.m_Emission  // incadescence
		>> mat.m_Transparency;

	ifs >> buffer;	assert( strcmp(buffer, "normalmap") == 0 );
	ifs >> buffer;	assert( strcmp(buffer, "tex=") == 0 );
	ifs	>> mat.m_NormalMapObject;

	switch ( mat.m_MaterialType )
	{
	case MATERIAL_LAMBERT:
		{
			// nothing
			break;
		}
	case MATERIAL_PHONG:
		{
			ifs	>> mat.m_Specular;

			ifs	>> buffer; assert( strcmp( buffer, "shininess" ) == 0 );
			ifs	>> mat.m_Shininess;
			break;
		}
	case MATERIAL_BLINN:
		{
			ifs	>> mat.m_Specular;

			ifs >> buffer; assert( strcmp( buffer, "eccentricity" ) == 0 );
			ifs	>> mat.m_Shininess;  // eccentricity

			float f;
			ifs >> buffer >> f;  // specularRollOff				
			break;
		}
	default:
		std::cerr << "Error! Unrecognized material type " << mat.m_MaterialType;
		assert(0); exit(1);
	}

	return ifs;
}