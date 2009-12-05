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
#ifndef _gkSubMeshBuffer_h_
#define _gkSubMeshBuffer_h_


#include "OgreSubMesh.h"
#include "gkMeshUtils.h"
#include "gkVertexBufferObject.h"
#include "gkFaceBufferObject.h"
#include "gkArray.h"
#include "gkHashTable.h"
#include "blender.h"



//-----------------------------------------------------------------------------
class gkSubMeshSlot
{
public:
	gkSubMeshSlot();
	gkSubMeshSlot(const gkSubMeshSlot &oth);
	const gkSubMeshSlot& operator= (const gkSubMeshSlot& oth);

	bool operator == (const gkSubMeshSlot& oth) const;

	short material_nr, mode, alpha;
	Blender::Image *tpage[8];

	static bool blender_mat;
};

//-----------------------------------------------------------------------------
GK_INLINE gkSubMeshSlot::gkSubMeshSlot()
{
	alpha = material_nr= mode= 0;
	tpage[0]= 0;
	tpage[1]= 0;
	tpage[2]= 0;
	tpage[3]= 0;
	tpage[4]= 0;
	tpage[5]= 0;
	tpage[6]= 0;
	tpage[7]= 0;
}

//-----------------------------------------------------------------------------
GK_INLINE gkSubMeshSlot::gkSubMeshSlot(const gkSubMeshSlot &oth)
{
	*this= oth;
}


//------- ----------------------------------------------------------------------
GK_INLINE bool gkSubMeshSlot::operator == (const gkSubMeshSlot& oth) const
{
	if (blender_mat)
		return material_nr == oth.material_nr;

	return ( mode  == oth.mode && alpha == oth.alpha && 
			 tpage[0] == oth.tpage[0] &&
			 tpage[1] == oth.tpage[1] &&
			 tpage[2] == oth.tpage[2] &&
			 tpage[3] == oth.tpage[3] &&
			 tpage[4] == oth.tpage[4] &&
			 tpage[5] == oth.tpage[5] &&
			 tpage[6] == oth.tpage[6] &&
			 tpage[7] == oth.tpage[7]);

}

//-----------------------------------------------------------------------------
GK_INLINE const gkSubMeshSlot& gkSubMeshSlot::operator= (const gkSubMeshSlot& oth)
{
	material_nr= oth.material_nr;
	mode= oth.mode;
	alpha = oth.alpha;
	tpage[0]= oth.tpage[0];
	tpage[1]= oth.tpage[1];
	tpage[2]= oth.tpage[2];
	tpage[3]= oth.tpage[3];
	tpage[4]= oth.tpage[4];
	tpage[5]= oth.tpage[5];
	tpage[6]= oth.tpage[6];
	tpage[7]= oth.tpage[7];
	return *this;
}

typedef gkArray<gkVertexBufferObject>	   gkUniqueVertexBuffer;
typedef gkArray<gkIndexBufferItem>		  gkUniqueIndexBuffer;
typedef gkHashTable<Index, Index>		 gkUniqueIndexMap;


//-----------------------------------------------------------------------------
class gkSubMeshBufferItem
{
public:
	typedef gkArray<gkVertexBufferObject> VertexBuffer;
	typedef gkArray<gkIndexBufferItem>	IndexBuffer;

	/// The ogre submesh
	Ogre::SubMesh* submesh;

	VertexBuffer vbuf;
	IndexBuffer  ibuf;

	gkUniqueIndexMap indexmap;


public:

	gkSubMeshBufferItem();
	~gkSubMeshBufferItem();

	Index getVertexBufferObject(Index iter, const gkVertexBufferObject& ref);
	void addTri(const gkFaceBufferObject& face);
};


//-----------------------------------------------------------------------------
struct gkSlotItemPair
{
	gkSubMeshSlot *slot;
	gkSubMeshBufferItem *item;
};

typedef gkArray<gkSlotItemPair> SubMeshBufferLookupMap;
typedef gkArrayIterator<SubMeshBufferLookupMap> SubMeshBufferLookupIterator;


//-----------------------------------------------------------------------------
GK_INLINE gkSubMeshBufferItem::gkSubMeshBufferItem():
		submesh(0)
{
}


//-----------------------------------------------------------------------------
GK_INLINE gkSubMeshBufferItem::~gkSubMeshBufferItem()
{
}


//-----------------------------------------------------------------------------
GK_INLINE Index gkSubMeshBufferItem::getVertexBufferObject(Index iter, const gkVertexBufferObject& ref)
{

	size_t found= GK_NPOS;
	const size_t &vbs= vbuf.size();

	size_t pos= indexmap.find(iter);
	if (pos != GK_NPOS)
	{
		Index second= indexmap.at(pos);
		if (second < vbs && vbuf.at(second) == ref)
			found= second;
	}

	if (found == GK_NPOS)
	{
		vbuf.push_back(ref);
		indexmap.insert(iter, vbs);
		return vbs;
	}

	return (Index)found;
}



//-----------------------------------------------------------------------------
GK_INLINE void gkSubMeshBufferItem::addTri(const gkFaceBufferObject& face)
{
	gkIndexBufferItem item;
	item.v1= getVertexBufferObject(face.v0.index, face.v0);
	item.v2= getVertexBufferObject(face.v1.index, face.v1);
	item.v3= getVertexBufferObject(face.v2.index, face.v2);
	ibuf.push_back(item);
}



#endif//_gkSubMeshBuffer_h_
