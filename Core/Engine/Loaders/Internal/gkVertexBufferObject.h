/*
-------------------------------------------------------------------------------
	This file is part of the Ogre GameKit port.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#ifndef _gkVertexBufferObject_h_
#define _gkVertexBufferObject_h_

#include "OgreMath.h"
#include "gkMeshUtils.h"
#include "OgreVertexBoneAssignment.h"



//-----------------------------------------------------------------------------
class gkVertexBufferObject
{
public:

	float vertex[3];
	float normal[3];
	unsigned int colour;
	float uv[8][2];
	Index index;
	int layer_max, vba_index;
public:

	gkVertexBufferObject();
	gkVertexBufferObject(const gkVertexBufferObject& o);

	gkVertexBufferObject& operator= (const gkVertexBufferObject&o);
	bool operator == (const gkVertexBufferObject& o) const;
};

//-----------------------------------------------------------------------------
GK_INLINE gkVertexBufferObject::gkVertexBufferObject()
{
}

//-----------------------------------------------------------------------------
GK_INLINE gkVertexBufferObject::gkVertexBufferObject(const gkVertexBufferObject& o)
{
	*this= o;
}

//-----------------------------------------------------------------------------
GK_INLINE gkVertexBufferObject& gkVertexBufferObject::operator= (const gkVertexBufferObject&o)
{
	colour= o.colour;
	layer_max= o.layer_max;
	index= o.index;
	vba_index= o.vba_index;

	VEC3CPY(vertex, o.vertex);
	VEC3CPY(normal, o.normal);

	VEC2CPY(uv[0], o.uv[0]);
	VEC2CPY(uv[1], o.uv[1]);
	VEC2CPY(uv[2], o.uv[2]);
	VEC2CPY(uv[3], o.uv[3]);
	VEC2CPY(uv[4], o.uv[4]);
	VEC2CPY(uv[5], o.uv[5]);
	VEC2CPY(uv[6], o.uv[6]);
	VEC2CPY(uv[7], o.uv[7]);

	return *this;
}


//-----------------------------------------------------------------------------
GK_INLINE bool gkVertexBufferObject::operator == (const gkVertexBufferObject& o) const
{
	bool result= false;

	float d[3];
	VEC3SUB(vertex, o.vertex, d);
	result= fabs(VEC3DOT(d)) < TOL;
	if (!result)
		return false;

	VEC3SUB(normal, o.normal, d);
	result= fabs(VEC3DOT(d)) < TOL;
	if (!result)
		return false;

	result= colour == o.colour;
	if (!result)
		return false;

	float u[2];
	for (int i= 0; i < layer_max; i++)
	{
		VEC2SUB(uv[i], o.uv[i], u);
		result= fabs(VEC2DOT(u)) < TOL;
		if (!result)
			break;
	}

	return result;
}




#endif//_gkVertexBufferObject_h_
