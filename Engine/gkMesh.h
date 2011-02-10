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
#ifndef _gkMesh_h_
#define _gkMesh_h_

#define GK_UV_MAX 8

#include "gkMathUtils.h"
#include "gkSerialize.h"
#include "gkResource.h"


class btTriangleMesh;
class gkMeshLoader;



class gkDeformVertex
{
public:
	int         group;
	gkScalar    weight;
	int         vertexId;
};



class gkVertex
{
public:
	gkVertex();
	gkVertex(const gkVertex& o);
	gkVertex& operator = (const gkVertex& o);

	gkVector3       co;                 // vertex coordinates
	gkVector3       no;                 // normals
	unsigned int    vcol;               // vertex color
	gkVector2       uv[GK_UV_MAX];      // texture coordinates < GK_UV_MAX
	int             vba;                // Vertex bone assignment
};



struct gkTriangle
{
	enum TriFlag
	{
		TRI_INVISIBLE    = (1 << 0),
		TRI_COLLIDER     = (1 << 1),
	};

	unsigned int i0, i1, i2;
	int flag;
};

struct gkTriFace
{
	gkVector3 p[3];
	unsigned int i[3];

	gkTriFace() { i[0] = i[1] = i[2] = 0; }

	GK_INLINE gkVector3 normal()
	{
		return (p[1]-p[0]).crossProduct(p[2]-p[1]).normalisedCopy();
	}
};


class gkSubMeshIndexer;


class gkSubMesh
{
public:
	typedef utArray<gkTriangle>         Triangles;
	typedef utArray<gkVertex>           Verticies;
	typedef utArray<gkDeformVertex>     DeformVerts;

private:

	Triangles           m_tris;
	Verticies           m_verts;
	int                 m_uvlayers;
	gkBoundingBox       m_bounds;
	bool                m_boundsInit;

	DeformVerts         m_defverts;
	bool                m_hasVertexColors;

	friend class gkSubMeshIndexer;
	gkSubMeshIndexer* m_sort;

	gkMaterialProperties*    m_material;

public:

	gkSubMesh();
	~gkSubMesh();

	Verticies&          getVertexBuffer(void)               {return m_verts;}
	Triangles&          getIndexBuffer(void)                {return m_tris;}
	DeformVerts&        getDeformVertexBuffer(void)         {return m_defverts;}
	gkString            getMaterialName(void)               {return m_material->m_name.c_str();}
	void                setMaterialName(const gkString& v)  {m_material->m_name = v.c_str();}
	void                setTotalLayers(int v)               {m_uvlayers = v;}
	int                 getUvLayerCount(void)               {return m_uvlayers;}
	void                setVertexColors(bool v)             {m_hasVertexColors = v;}
	bool                hasVertexColors(void)               {return m_hasVertexColors; }


	gkBoundingBox&       getBoundingBox(void);

	gkMaterialProperties& getMaterial(void)                 {return *m_material;}


	gkSubMesh* clone(void);
	void       addTriangle(const gkVertex& v0,
	                       unsigned int i0,
	                       const gkVertex& v1,
	                       unsigned int i1,
	                       const gkVertex& v2,
	                       unsigned int i2, int flag);

	void        addDeformVert(const gkDeformVertex& dv);
};





class gkVertexGroup
{
public:

	gkVertexGroup(const gkString& name, int index) : m_name(name), m_index(index) {}

	const int       getIndex(void)              { return m_index; }
	const gkString& getName(void)               { return m_name; }

private:
	gkString    m_name;
	int         m_index;
};


class gkMesh : public gkResource
{
public:

	typedef utArray<gkSubMesh*>             SubMeshArray;
	typedef utArrayIterator<SubMeshArray>   SubMeshIterator;
	typedef utArray<gkVertexGroup*>         VertexGroups;

private:

	SubMeshArray         m_submeshes;
	gkBoundingBox        m_bounds;
	bool                 m_boundsInit;
	VertexGroups         m_groups;
	btTriangleMesh*      m_triMesh;
	gkSkeletonResource*  m_skeleton;
	gkMeshLoader*        m_meshLoader;

	UTsize               m_vertexCount;
	UTsize               m_triFaceCount;

public:

	gkMesh(gkResourceManager* creator, const gkResourceName& name, const gkResourceHandle& handle);
	virtual ~gkMesh();

	void addSubMesh(gkSubMesh* me);
	void _setSkeleton(gkSkeletonResource* res) {m_skeleton = res;}
	gkSkeletonResource* getSkeleton(void)      {return m_skeleton;}


	gkBoundingBox& getBoundingBox(void);

	btTriangleMesh*          getTriMesh(void);
	gkMaterialProperties&    getFirstMaterial(void);


	gkVertexGroup*   createVertexGroup(const gkString& name);
	VertexGroups&    getGroups(void) {return m_groups;}
	gkVertexGroup*   findVertexGroup(const gkString& name);
	gkVertexGroup*   findVertexGroup(int i);

	SubMeshIterator getSubMeshIterator(void) {return SubMeshIterator(m_submeshes);}


	gkMeshLoader* getLoader(void) {return m_meshLoader;}

	UTsize getMeshVertexCount(void);
	const gkVertex& getMeshVertex(UTsize n);

	UTsize getMeshTriFaceCount(void);
	gkTriFace getMeshTriFace(UTsize n); 
};

#endif//_gkMesh_h_
