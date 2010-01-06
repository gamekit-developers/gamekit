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

#pragma once

#include <assert.h>
#include <vector>
#include <iostream>
#include "../SharedStructures.h"

inline float CLAMP(const float &f) 
{
	if ( f > 1.0f ) 
		return 1.0f;
	if ( f < 0.0f ) 
		return 0.0f;
	return f;
}
	
class XMatAttrib
{
public:
	char		m_name[256];
	float		m_color[4];
	uint		m_TextureObject;

	XMatAttrib() : m_TextureObject(0) 
	{
		m_color[0] = 0.0f;
		m_color[1] = 0.0f;
		m_color[2] = 0.0f;
		m_color[3] = 1.0f;
	}

	friend std::istream& operator >> (std::istream& ifs, XMatAttrib& attrib)
	{
		char buffer[256];

		ifs >> attrib.m_name;
		ifs	>> attrib.m_color[0] >> attrib.m_color[1] >> attrib.m_color[2];

		ifs	>> buffer; assert( strcmp( buffer, "tex=" ) == 0 );
		ifs	>> attrib.m_TextureObject;
		
		return ifs;
	}

	inline void SetAlpha(const float a) 
	{
		m_color[3] = 1.0f - CLAMP(a);
	}

};

class XMaterial
{
public:
	uint			m_MaterialType;
	std::string		m_MaterialName;
	int				m_MaterialID;

	XMatAttrib		m_Ambient;
	XMatAttrib		m_Diffuse;
	XMatAttrib		m_Specular;
	XMatAttrib		m_Emission;
	XMatAttrib		m_Transparency;

	unsigned int	m_NormalMapObject;

	float			m_Shininess;

	bool			m_IsTransparent;	// runtime


	XMaterial(void);
	~XMaterial(void);

	friend std::istream& operator >> (std::istream& ifs, XMaterial& mat);

	inline void SetTransparency() 
	{
		// check if this material has transparency texture
		unsigned int transpTexID = m_Transparency.m_TextureObject;
		if ( transpTexID > 0 )
		{
			m_IsTransparent = true;
		}
		// check if this material has transparency color
		if ( 
			m_Transparency.m_color[0] > 0.1 ||
			m_Transparency.m_color[1] > 0.1 ||
			m_Transparency.m_color[2] > 0.1 )
		{
			m_IsTransparent = true;
			float r = m_Transparency.m_color[0];
			float g = m_Transparency.m_color[1];
			float b = m_Transparency.m_color[2];
			float transp = ( r + g + b ) / 3.0f; // just average for now
			m_Ambient.SetAlpha( transp );
			m_Diffuse.SetAlpha( transp );
		}

	}

	/// a function to set the texture object for the material to use
	inline void SetTextureObjects(unsigned int *texObjs) 
	{
		if(m_Ambient.m_TextureObject != 0 ) 
			m_Ambient.m_TextureObject = texObjs[ m_Ambient.m_TextureObject-1 ];

		if(m_Diffuse.m_TextureObject != 0 ) 
			m_Diffuse.m_TextureObject = texObjs[ m_Diffuse.m_TextureObject-1 ];

		if(m_Specular.m_TextureObject != 0 ) 
			m_Specular.m_TextureObject = texObjs[ m_Specular.m_TextureObject-1 ];

		if(m_Emission.m_TextureObject != 0 ) 
			m_Emission.m_TextureObject = texObjs[ m_Emission.m_TextureObject-1 ];

		if(m_Transparency.m_TextureObject != 0 ) 
			m_Transparency.m_TextureObject = texObjs[ m_Transparency.m_TextureObject-1 ];

		if (m_NormalMapObject != 0)
			m_NormalMapObject = texObjs[ m_NormalMapObject-1 ];
	}

};
