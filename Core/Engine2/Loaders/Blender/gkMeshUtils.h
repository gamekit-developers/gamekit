/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): none yet.
-------------------------------------------------------------------------------
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
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
#ifndef _gkMeshUtils_h_
#define _gkMeshUtils_h_

#include "OgreSubMesh.h"
#include "OgreVertexBoneAssignment.h"
#include "gkMathUtils.h"
#include "gkMeshUtils.h"
#include "gkCommon.h"
#include "gkMeshUtils.h"
#include "blender.h"


// may not be nessary
// but add some limit
#define LIMIT 1000000


#define VEC3SUB(a,b,d) {d[0]= a[0] - b[0]; d[1]= a[1] - b[1]; d[2]= a[2] - b[2];}
#define VEC2SUB(a,b,d) {d[0]= a[0] - b[0]; d[1]= a[1] - b[1];}
#define VEC3SUB2(a,b,d) {d[0]= a.x - b.x; d[1]= a.y - b.y; d[2]= a.z - b.z;}
#define VEC2SUB2(a,b,d) {d[0]= a.x - b.x; d[1]= a.y - b.y;}

#define VEC3DOT(a)      (a[0] * a[0] + a[1] * a[1] + a[2] * a[2])
#define VEC2DOT(a)      (a[0] * a[0] + a[1] * a[1])

#define VEC3ZRO(a)      {a[0]= 0.f; a[1]=0.f; a[2]=0.f;}
#define VEC2ZRO(a)      {a[0]= 0.f; a[1]= 0.f;}


// copy
#define VEC3CPY(a, b) {a[0]= b[0]; a[1]= b[1]; a[2]= b[2];}
#define VEC2CPY(a, b) {a[0]= b[0]; a[1]= b[1];}

#define VEC3CPY2(a, b) {a.x= b.x; a.y= b.y; a.z= b.z;}
#define VEC2CPY2(a, b) {a.x= b.x; a.y= b.y;}

// float normal from short
#define VEC3CPN(a, b) {a[0]= (b[0]/32767.f); a[1]= (b[1]/32767.f); a[2]= (b[2]/32767.f);}

// equal tests
#define TESTBEQ(val) (val == b.val)
#define TESTNEQ(val) (val != b.val)
#define TESTBLE(val) (val < b.val)
#define SETTEST(val) (val= b.val)


#define MINTEST(a,b) (((a)<(b))?(a):(b))
#define MAXTEST(a,b) (((a)>(b))?(a):(b))

// Test to enable only (real) materials
#define SORT_BY_MATERIAL 1

typedef size_t Index;
#define TOL 1e-10

#define VEC2F(v) ((float*)&(v))
#define VEC2D(v) ((double*)&(v))






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


GK_INLINE gkVertexBufferObject::gkVertexBufferObject()
{
}


GK_INLINE gkVertexBufferObject::gkVertexBufferObject(const gkVertexBufferObject& o)
{
    *this = o;
}


GK_INLINE gkVertexBufferObject& gkVertexBufferObject::operator= (const gkVertexBufferObject & o)
{
    colour = o.colour;
    layer_max = o.layer_max;
    index = o.index;
    vba_index = o.vba_index;

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



GK_INLINE bool gkVertexBufferObject::operator == (const gkVertexBufferObject& o) const
{
    bool result = false;

    float d[3];
    VEC3SUB(vertex, o.vertex, d);
    result = fabs(VEC3DOT(d)) < TOL;
    if (!result)
        return false;

    VEC3SUB(normal, o.normal, d);
    result = fabs(VEC3DOT(d)) < TOL;
    if (!result)
        return false;

    result = colour == o.colour;
    if (!result)
        return false;

    float u[2];
    for (int i = 0; i < layer_max; i++)
    {
        VEC2SUB(uv[i], o.uv[i], u);
        result = fabs(VEC2DOT(u)) < TOL;
        if (!result)
            break;
    }

    return result;
}





class gkIndexBufferItem
{
public:
    Index v1, v2, v3;
public:

    gkIndexBufferItem();
    gkIndexBufferItem(const gkIndexBufferItem& o);
    gkIndexBufferItem& operator= (const gkIndexBufferItem& o);
};



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



GK_INLINE gkFaceBufferObject::gkFaceBufferObject()
{
}



GK_INLINE gkFaceBufferObject::gkFaceBufferObject(const gkFaceBufferObject& o)
{
    *this = o;
}



GK_INLINE gkFaceBufferObject& gkFaceBufferObject::operator= (const gkFaceBufferObject & o)
{
    v0 = o.v0;
    v1 = o.v1;
    v2 = o.v2;
    return *this;
}



GK_INLINE void gkFaceBufferObject::calcNormal()
{
    gkVector3 vr0 = gkVector3(v0.vertex);
    gkVector3 vr1 = gkVector3(v1.vertex);
    gkVector3 vr2 = gkVector3(v2.vertex);

    gkVector3 n = (vr1 - vr2).crossProduct(vr2 - vr0);
    if (n.length() > 0)
        n.normalise();

    // set for the face
    VEC3CPY(v0.normal, n);
    VEC3CPY(v1.normal, n);
    VEC3CPY(v2.normal, n);
}



GK_INLINE void gkFaceBufferObject::addVertexBoneAssignment(short index, int vba)
{
    switch (index)
    {
    case 0:
        v0.vba_index = vba;
        break;
    case 1:
        v1.vba_index = vba;
        break;
    case 2:
        v2.vba_index = vba;
        break;
    }
}


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


GK_INLINE void gkFaceBufferObject::addColour(short index, unsigned int col)
{
    switch (index)
    {
    case 0:
        v0.colour = col;
        break;
    case 1:
        v1.colour = col;
        break;
    case 2:
        v2.colour = col;
        break;
    }
}


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


GK_INLINE void gkFaceBufferObject::addIndex(short index, Index real)
{
    switch (index)
    {
    case 0:
        v0.index = real;
        break;
    case 1:
        v1.index = real;
        break;
    case 2:
        v2.index = real;
        break;
    }
}


GK_INLINE void gkFaceBufferObject::setMaxLayer(int lay)
{
    v0.layer_max = v1.layer_max = v2.layer_max = lay;
}



GK_INLINE gkIndexBufferItem::gkIndexBufferItem()
{
    v1 = v2 = v3 = 0;
}



GK_INLINE gkIndexBufferItem::gkIndexBufferItem(const gkIndexBufferItem& o)
{
    *this = o;
}


GK_INLINE gkIndexBufferItem& gkIndexBufferItem::operator= (const gkIndexBufferItem & o)
{
    v1 = o.v1;
    v2 = o.v2;
    v3 = o.v3;
    return *this;
}




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


GK_INLINE gkSubMeshSlot::gkSubMeshSlot()
{
    alpha = material_nr = mode = 0;
    tpage[0] = 0;
    tpage[1] = 0;
    tpage[2] = 0;
    tpage[3] = 0;
    tpage[4] = 0;
    tpage[5] = 0;
    tpage[6] = 0;
    tpage[7] = 0;
}


GK_INLINE gkSubMeshSlot::gkSubMeshSlot(const gkSubMeshSlot &oth)
{
    *this = oth;
}


//------- ----------------------------------------------------------------------
GK_INLINE bool gkSubMeshSlot::operator == (const gkSubMeshSlot& oth) const
{
    if (blender_mat)
        return material_nr == oth.material_nr;

    return (mode  == oth.mode && alpha == oth.alpha &&
            tpage[0] == oth.tpage[0] &&
            tpage[1] == oth.tpage[1] &&
            tpage[2] == oth.tpage[2] &&
            tpage[3] == oth.tpage[3] &&
            tpage[4] == oth.tpage[4] &&
            tpage[5] == oth.tpage[5] &&
            tpage[6] == oth.tpage[6] &&
            tpage[7] == oth.tpage[7]);

}


GK_INLINE const gkSubMeshSlot& gkSubMeshSlot::operator= (const gkSubMeshSlot & oth)
{
    material_nr = oth.material_nr;
    mode = oth.mode;
    alpha = oth.alpha;
    tpage[0] = oth.tpage[0];
    tpage[1] = oth.tpage[1];
    tpage[2] = oth.tpage[2];
    tpage[3] = oth.tpage[3];
    tpage[4] = oth.tpage[4];
    tpage[5] = oth.tpage[5];
    tpage[6] = oth.tpage[6];
    tpage[7] = oth.tpage[7];
    return *this;
}

typedef utArray<gkVertexBufferObject>        gkUniqueVertexBuffer;
typedef utArray<gkIndexBufferItem>            gkUniqueIndexBuffer;
typedef utHashTable<utIntHashKey, Index>    gkUniqueIndexMap;



class gkSubMeshBufferItem
{
public:
    typedef utArray<gkVertexBufferObject> VertexBuffer;
    typedef utArray<gkIndexBufferItem>    IndexBuffer;

    // The ogre submesh
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



struct gkSlotItemPair
{
    gkSubMeshSlot *slot;
    gkSubMeshBufferItem *item;
};

typedef utArray<gkSlotItemPair> SubMeshBufferLookupMap;
typedef utArrayIterator<SubMeshBufferLookupMap> SubMeshBufferLookupIterator;



GK_INLINE gkSubMeshBufferItem::gkSubMeshBufferItem():
        submesh(0)
{
}



GK_INLINE gkSubMeshBufferItem::~gkSubMeshBufferItem()
{
}



GK_INLINE Index gkSubMeshBufferItem::getVertexBufferObject(Index iter, const gkVertexBufferObject& ref)
{

    size_t found = GK_NPOS;
    const size_t &vbs = vbuf.size();

    size_t pos = indexmap.find(iter);
    if (pos != GK_NPOS)
    {
        Index second = indexmap.at(pos);
        if (second < vbs && vbuf.at(second) == ref)
            found = second;
    }

    if (found == GK_NPOS)
    {
        vbuf.push_back(ref);
        indexmap.insert(iter, vbs);
        return vbs;
    }

    return (Index)found;
}




GK_INLINE void gkSubMeshBufferItem::addTri(const gkFaceBufferObject& face)
{
    gkIndexBufferItem item;
    item.v1 = getVertexBufferObject(face.v0.index, face.v0);
    item.v2 = getVertexBufferObject(face.v1.index, face.v1);
    item.v3 = getVertexBufferObject(face.v2.index, face.v2);
    ibuf.push_back(item);
}

#endif//_gkMeshUtils_h_
