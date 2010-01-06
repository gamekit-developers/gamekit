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

#include <iostream>

#include "XTransformation.h"

XTransformation::~XTransformation(void)
{
}

std::istream &operator >> (std::istream &ifs, Quat &v) 
{
	float x,y,z,w;
	ifs >> x >> y >> z >> w;
	//v = Quat( x, y, z, w );
	v = Quat( x, y, z, w );
	return ifs;
}

std::istream &operator >> (std::istream &ifs,Matrix4 &m)
{
	//ifs >> m.m_00 >> m.m_01 >> m.m_02 >> m.m_03;
	//ifs	>> m.m_10 >> m.m_11 >> m.m_12 >> m.m_13;
	//ifs	>> m.m_20 >> m.m_21 >> m.m_22 >> m.m_23;
	//ifs	>> m.m_30 >> m.m_31 >> m.m_32 >> m.m_33;		
	float f;
	for ( int i = 0; i < 4; i++ )
	{
		for ( int j = 0; j < 4; j++ )
		{
			ifs >> f;
			m.setElem(i, j, f);
		}
	}
	return ifs;
}

std::istream &operator >> (std::istream &ifs, Vector3 &v) 
{
	float x,y,z;
	ifs >> x >> y >> z;
	v = Vector3( x, y, z );
	return ifs;
}

/// stream extraction operator
std::istream& operator >> (std::istream& ifs, XTransformation &xf)
{
	unsigned int vis;
	ifs >> vis;
	xf.m_Visibility = vis ? true : false;
	// read rotation translation and scale
	ifs >> xf.m_Rotation;
	ifs >> xf.m_Translation;
	ifs >> xf.m_Scale;
	return ifs;
}


XTransformation LERP(const float t, const XTransformation &t1, const XTransformation &t2)
{
	XTransformation out;
	out.m_Rotation =	slerp( t, t1.m_Rotation,	t2.m_Rotation );
	out.m_Translation = lerp(  t, t1.m_Translation, t2.m_Translation );
	out.m_Scale =		lerp(  t, t1.m_Scale,		t2.m_Scale );
	return out;
}

