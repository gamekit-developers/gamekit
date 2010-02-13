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
#ifndef _OgreFaceBufferObject_h_
#define _OgreFaceBufferObject_h_

#include "OgreMathUtils.h"
#include "OgreMeshUtils.h"
#include "OgreVertexBufferObject.h"


namespace Ogre {


//-----------------------------------------------------------------------------
class IndexBufferItem
{
public:
	Index v1, v2, v3;
public:

	IndexBufferItem();
	IndexBufferItem(const IndexBufferItem& o);
	IndexBufferItem& operator= (const IndexBufferItem& o);
};


//-----------------------------------------------------------------------------
class FaceBufferObject
{
public:

	VertexBufferObject v0;
	VertexBufferObject v1;
	VertexBufferObject v2;

public:

	FaceBufferObject();
	FaceBufferObject(const FaceBufferObject& o);
	FaceBufferObject& operator= (const FaceBufferObject& o);

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
inline FaceBufferObject::FaceBufferObject()
{
}


//-----------------------------------------------------------------------------
inline FaceBufferObject::FaceBufferObject(const FaceBufferObject& o)
{
	*this= o;
}


//-----------------------------------------------------------------------------
inline FaceBufferObject& FaceBufferObject::operator= (const FaceBufferObject& o)
{
	v0= o.v0;
	v1= o.v1;
	v2= o.v2;
	return *this;
}


//-----------------------------------------------------------------------------
inline void FaceBufferObject::calcNormal()
{
	Vector3 vr0= Vector3(v0.vertex);
	Vector3 vr1= Vector3(v1.vertex);
	Vector3 vr2= Vector3(v2.vertex);

	Vector3 n= (vr1 - vr2).crossProduct(vr2 - vr0);
	if (n.length() > 0)
		n.normalise();

	/// set for the face
	VEC3CPY(v0.normal, n);
	VEC3CPY(v1.normal, n);
	VEC3CPY(v2.normal, n);
}


//-----------------------------------------------------------------------------
inline void FaceBufferObject::addVertexBoneAssignment(short index, int vba)
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
inline void FaceBufferObject::addVertex(short index, float* v)
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
inline void FaceBufferObject::addNormal(short index, short *v)
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
inline void FaceBufferObject::addColour(short index, unsigned int col)
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
inline void FaceBufferObject::addUvCord(short index, int layer, float* uv)
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
inline void FaceBufferObject::addIndex(short index, Index real)
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
inline void FaceBufferObject::setMaxLayer(int lay)
{
	v0.layer_max= v1.layer_max= v2.layer_max= lay;
}


//-----------------------------------------------------------------------------
inline IndexBufferItem::IndexBufferItem()
{
	v1= v2= v3= 0;
}


//-----------------------------------------------------------------------------
inline IndexBufferItem::IndexBufferItem(const IndexBufferItem& o)
{
	*this= o;
}

//-----------------------------------------------------------------------------
inline IndexBufferItem& IndexBufferItem::operator= (const IndexBufferItem& o)
{
	v1= o.v1;
	v2= o.v2;
	v3= o.v3;
	return *this;
}



}
#endif//_OgreFaceBufferObject_h_
