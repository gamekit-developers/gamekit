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
#ifndef _gkFaceBufferObject_h_
#define _gkFaceBufferObject_h_

#include "gkMathUtils.h"
#include "gkMeshUtils.h"
#include "gkVertexBufferObject.h"




//-----------------------------------------------------------------------------
class gkIndexBufferItem
{
public:
	Index v1, v2, v3;
public:

	gkIndexBufferItem();
	gkIndexBufferItem(const gkIndexBufferItem& o);
	gkIndexBufferItem& operator= (const gkIndexBufferItem& o);
};


//-----------------------------------------------------------------------------
class gkFaceBufferObject
{
public:

	gkVertexBufferObject v0;
	gkVertexBufferObject v1;
	gkVertexBufferObject v2;

public:

	gkFaceBufferObject();
	gkFaceBufferObject(const gkFaceBufferObject& o);
	gkFaceBufferObject& operator= (const gkFaceBufferObject& o);

	void addVertexBoneAssignment(short index, int vba);
	void addVertex(short index, float *v);
	void addNormal(short index, short *v);
	void addColour(short index, unsigned int col);
	void addUvCord(short index, int layer, float *uv);
	void addIndex(short index, Index real);
	void setMaxLayer(int lay);

	void calcNormal();
};


//-----------------------------------------------------------------------------
GK_INLINE gkFaceBufferObject::gkFaceBufferObject()
{
}


//-----------------------------------------------------------------------------
GK_INLINE gkFaceBufferObject::gkFaceBufferObject(const gkFaceBufferObject& o)
{
	*this= o;
}


//-----------------------------------------------------------------------------
GK_INLINE gkFaceBufferObject& gkFaceBufferObject::operator= (const gkFaceBufferObject& o)
{
	v0= o.v0;
	v1= o.v1;
	v2= o.v2;
	return *this;
}


//-----------------------------------------------------------------------------
GK_INLINE void gkFaceBufferObject::calcNormal()
{
	Ogre::Vector3 vr0= Ogre::Vector3(v0.vertex);
	Ogre::Vector3 vr1= Ogre::Vector3(v1.vertex);
	Ogre::Vector3 vr2= Ogre::Vector3(v2.vertex);

	Ogre::Vector3 n= (vr1 - vr2).crossProduct(vr2 - vr0);
	if (n.length() > 0)
		n.normalise();

	/// set for the face
	VEC3CPY(v0.normal, n);
	VEC3CPY(v1.normal, n);
	VEC3CPY(v2.normal, n);
}


//-----------------------------------------------------------------------------
GK_INLINE void gkFaceBufferObject::addVertexBoneAssignment(short index, int vba)
{
	switch (index)
	{
	case 0:
		v0.vba_index= vba;
		break;
	case 1:
		v1.vba_index= vba;
		break;
	case 2:
		v2.vba_index= vba;
		break;
	}
}

//-----------------------------------------------------------------------------
GK_INLINE void gkFaceBufferObject::addVertex(short index, float* v)
{
	switch (index)
	{
	case 0:
		VEC3CPY(v0.vertex, v);
		break;
	case 1:
		VEC3CPY(v1.vertex, v);
		break;
	case 2:
		VEC3CPY(v2.vertex, v);
		break;
	}
}

//-----------------------------------------------------------------------------
GK_INLINE void gkFaceBufferObject::addNormal(short index, short *v)
{
	switch (index)
	{
	case 0:
		VEC3CPN(v0.normal, v);
		break;
	case 1:
		VEC3CPN(v1.normal, v);
		break;
	case 2:
		VEC3CPN(v2.normal, v);
		break;
	}
}

//-----------------------------------------------------------------------------
GK_INLINE void gkFaceBufferObject::addColour(short index, unsigned int col)
{
	switch (index)
	{
	case 0:
		v0.colour= col;
		break;
	case 1:
		v1.colour= col;
		break;
	case 2:
		v2.colour= col;
		break;
	}
}

//-----------------------------------------------------------------------------
GK_INLINE void gkFaceBufferObject::addUvCord(short index, int layer, float* uv)
{
	if (layer >= 0 && layer < 8)
	{
		switch (index)
		{
		case 0:
			VEC2CPY(v0.uv[layer], uv);
			break;
		case 1:
			VEC2CPY(v1.uv[layer], uv);
			break;
		case 2:
			VEC2CPY(v2.uv[layer], uv);
			break;
		}
	}
}

//-----------------------------------------------------------------------------
GK_INLINE void gkFaceBufferObject::addIndex(short index, Index real)
{
	switch (index)
	{
	case 0:
		v0.index= real;
		break;
	case 1:
		v1.index= real;
		break;
	case 2:
		v2.index= real;
		break;
	}
}

//-----------------------------------------------------------------------------
GK_INLINE void gkFaceBufferObject::setMaxLayer(int lay)
{
	v0.layer_max= v1.layer_max= v2.layer_max= lay;
}


//-----------------------------------------------------------------------------
GK_INLINE gkIndexBufferItem::gkIndexBufferItem()
{
	v1= v2= v3= 0;
}


//-----------------------------------------------------------------------------
GK_INLINE gkIndexBufferItem::gkIndexBufferItem(const gkIndexBufferItem& o)
{
	*this= o;
}

//-----------------------------------------------------------------------------
GK_INLINE gkIndexBufferItem& gkIndexBufferItem::operator= (const gkIndexBufferItem& o)
{
	v1= o.v1;
	v2= o.v2;
	v3= o.v3;
	return *this;
}




#endif//_gkFaceBufferObject_h_
