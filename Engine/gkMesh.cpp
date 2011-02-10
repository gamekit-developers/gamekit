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
#include "gkCommon.h"
#include "gkMesh.h"
#include "gkResourceManager.h"
#include "BulletCollision/CollisionShapes/btTriangleMesh.h"



#if GK_EXTERNAL_RENDER == GK_EXTERNAL_RENDER_OGREKIT
#include "External/Ogre/gkOgreMeshLoader.h"
#endif



gkVertex::gkVertex()
{
	co = no = (0, 0, 0);
	vcol = 0xFFFFFFFF;
	vba  = -1;
	for (int _i = 0; _i < GK_UV_MAX; _i++)
		uv[_i] = gkVector2(0, 0);
}



gkVertex::gkVertex(const gkVertex& o)
{
	*this = o;
}



gkVertex& gkVertex::operator = (const gkVertex& o)
{
	co      = o.co;
	no      = o.no;
	vcol    = o.vcol;
	vba     = o.vba;
	for (int _i = 0; _i < GK_UV_MAX; _i++)
		uv[_i] = o.uv[_i];
	return *this;
}



class gkSubMeshIndexer
{

public:

	typedef utHashTable<utIntHashKey, unsigned int> IndexMap;
	IndexMap m_indexMap;

	gkSubMeshIndexer() {}



	unsigned int getVertexIndex(gkSubMesh* sub, unsigned int index, const gkVertex& ref)
	{
		UTsize i = m_indexMap.find((int)index), fnd = UT_NPOS, size = sub->m_verts.size();

		if (i != UT_NPOS)
		{
			UTsize sp = m_indexMap.at(i);
			if (sp < size && vertEq(sub, sub->m_verts.at(sp), ref))
				fnd = sp;
		}

		if (fnd == UT_NPOS)
		{
			sub->m_bounds.merge(ref.co);
			sub->m_verts.push_back(ref);
			m_indexMap.insert((int)index, size);
			return (unsigned int)size;
		}

		return (unsigned int)fnd;
	}


	bool vertEq(gkSubMesh* sub, const gkVertex& a, const gkVertex& b)
	{
		if (!gkFuzzyT((a.co - b.co).squaredLength(), 1e-10))
			return false;
		if (!gkFuzzyT((a.no - b.no).squaredLength(), 1e-10))
			return false;

		if (sub->hasVertexColors())
		{
			if (a.vcol != b.vcol)
				return false;
		}

		bool result = true;
		for (int i = 0; i < sub->getUvLayerCount(); i++)
		{
			result = gkFuzzyT((a.uv[i] - b.uv[i]).squaredLength(), 1e-10);
			if (!result)
				break;
		}
		return result;

	}
};


gkSubMesh::gkSubMesh()
	:   m_uvlayers(0),
	    m_sort(new gkSubMeshIndexer()),
	    m_bounds(gkBoundingBox::BOX_NULL),
	    m_boundsInit(false),
	    m_hasVertexColors(false)
{
	m_material = new gkMaterialProperties();
}


gkSubMesh::~gkSubMesh()
{
	delete m_material;
	delete m_sort;
}


gkSubMesh* gkSubMesh::clone(void)
{
	gkSubMesh* nme = new gkSubMesh();
	nme->m_tris             = m_tris;
	nme->m_verts            = m_verts;
	nme->m_uvlayers         = m_uvlayers;
	nme->m_material         = m_material;
	nme->m_hasVertexColors  = m_hasVertexColors;
	nme->m_boundsInit       = false;
	nme->m_defverts         = m_defverts;
	*nme->m_material        = *m_material;
	nme->getBoundingBox();
	return nme;
}



void gkSubMesh::addTriangle(const gkVertex& v0,
                            unsigned int i0,
                            const gkVertex& v1,
                            unsigned int i1,
                            const gkVertex& v2,
                            unsigned int i2, int flag
                           )
{
	m_boundsInit = true;
	gkTriangle tri;
	tri.flag = flag;

	tri.i0 = m_sort->getVertexIndex(this, i0, v0);
	tri.i1 = m_sort->getVertexIndex(this, i1, v1);
	tri.i2 = m_sort->getVertexIndex(this, i2, v2);
	m_tris.push_back(tri);
}


void gkSubMesh::addDeformVert(const gkDeformVertex& dv)
{
	m_defverts.push_back(dv);
}


gkBoundingBox& gkSubMesh::getBoundingBox(void)
{
	if (!m_boundsInit)
	{
		m_boundsInit = true;
		UTsize i = 0, s = m_verts.size();
		Verticies::Pointer ptr = m_verts.ptr();

		while (i < s)
		{
			const gkVertex& vtx = ptr[i++];
			m_bounds.merge(vtx.co);
		}

	}
	return m_bounds;
}




gkMesh::gkMesh(gkResourceManager* creator, const gkResourceName& name, const gkResourceHandle& handle)
	:   gkResource(creator, name, handle),
	    m_bounds(gkBoundingBox::BOX_NULL),
	    m_boundsInit(false),
	    m_triMesh(0),
	    m_skeleton(0),
		m_vertexCount(0),
		m_triFaceCount(0)
{
	m_meshLoader = new gkMeshLoader(this);
}


gkMesh::~gkMesh()
{
	delete m_meshLoader;
	m_meshLoader = 0;


	if (m_triMesh)
		delete m_triMesh;
	m_triMesh = 0;

	SubMeshIterator iter = getSubMeshIterator();
	while (iter.hasMoreElements())
		delete iter.getNext();

	for (UTsize i = 0; i < m_groups.size(); ++i)
		delete m_groups[i];
	m_groups.clear();
}


void gkMesh::addSubMesh(gkSubMesh* me)
{
	m_submeshes.push_back(me);
}


gkBoundingBox& gkMesh::getBoundingBox(void)
{
	if (!m_boundsInit)
	{
		m_boundsInit = true;
		SubMeshIterator iter = getSubMeshIterator();
		while (iter.hasMoreElements())
			m_bounds.merge(iter.getNext()->getBoundingBox());
	}

	return m_bounds;
}


gkMaterialProperties& gkMesh::getFirstMaterial(void)
{
	if (!m_submeshes.empty())
		return m_submeshes.at(0)->getMaterial();

	static gkMaterialProperties props;
	return props;
}


btTriangleMesh* gkMesh::getTriMesh(void)
{
	if (m_triMesh)
		return m_triMesh;


	m_triMesh = new btTriangleMesh();
	SubMeshIterator iter = getSubMeshIterator();
	while (iter.hasMoreElements())
	{
		gkSubMesh* sub = iter.getNext();

		gkSubMesh::Triangles& ibuf = sub->getIndexBuffer();
		UTsize totface = ibuf.size(), i;
		gkSubMesh::Triangles::Pointer ibufPtr = ibuf.ptr();
		gkSubMesh::Verticies::Pointer vbuf = sub->getVertexBuffer().ptr();


		for (i = 0; i < totface; ++i)
		{
			const gkTriangle& tri = ibufPtr[i];

			if (tri.flag & gkTriangle::TRI_COLLIDER)
			{
				m_triMesh->addTriangle(
				    btVector3(vbuf[tri.i0].co.x, vbuf[tri.i0].co.y, vbuf[tri.i0].co.z),
				    btVector3(vbuf[tri.i1].co.x, vbuf[tri.i1].co.y, vbuf[tri.i1].co.z),
				    btVector3(vbuf[tri.i2].co.x, vbuf[tri.i2].co.y, vbuf[tri.i2].co.z)
				);
			}
		}
	}

	return m_triMesh;
}


gkVertexGroup* gkMesh::createVertexGroup(const gkString& name)
{
	gkVertexGroup* group = new gkVertexGroup(name, m_groups.size());
	m_groups.push_back(group);
	return group;
}



gkVertexGroup* gkMesh::findVertexGroup(int i)
{
	if (i < (int)m_groups.size() && i >= 0 )
		return m_groups.at(i);
	return 0;
}


gkVertexGroup* gkMesh::findVertexGroup(const gkString& name)
{
	UTsize i = 0, s = m_groups.size();
	while (i < s)
	{
		gkVertexGroup* grp = m_groups.at(i++);
		if (grp->getName() == name)
			return grp;
	}
	return 0;
}

UTsize gkMesh::getMeshVertexCount()
{
	if (m_vertexCount > 0) return m_vertexCount;

	m_vertexCount = 0;
	UTsize i = 0;
	for (i = 0; i < m_submeshes.size(); i++)
		m_vertexCount += m_submeshes[i]->getVertexBuffer().size();

	return m_vertexCount;
}

const gkVertex& gkMesh::getMeshVertex(UTsize n)
{
	static const gkVertex v;

	UTsize vertexCount = getMeshVertexCount();
	if (n >= vertexCount) return v;

	UTsize i = 0;
	for (i = 0; i < m_submeshes.size(); i++)
	{
		UTsize scount = m_submeshes[i]->getVertexBuffer().size();
		if (n >= scount)
			n -= scount;
		else
			return m_submeshes[i]->getVertexBuffer()[n];
	}

	return v;
}


UTsize gkMesh::getMeshTriFaceCount()
{
	if (m_triFaceCount > 0) return m_triFaceCount;

	m_triFaceCount = 0;
	UTsize i = 0;
	for (i = 0; i < m_submeshes.size(); i++)
		m_triFaceCount += m_submeshes[i]->getIndexBuffer().size();

	return m_triFaceCount;
}

gkTriFace gkMesh::getMeshTriFace(UTsize n)
{
	gkTriFace f;

	UTsize faceCount = getMeshVertexCount();
	if (n >= faceCount) return f;

	UTsize i = 0, tot = 0;
	for (i = 0; i < m_submeshes.size(); i++)
	{
		const gkSubMesh::Triangles& tris = m_submeshes[i]->getIndexBuffer();
		UTsize scount = tris.size();
		if (n >= scount)
		{
			n -= scount;
			tot += scount;
		}
		else
		{
			const gkSubMesh::Verticies& verts = m_submeshes[i]->getVertexBuffer();
			const gkTriangle& t = tris[n];

			f.p[0] = verts[t.i0].co;
			f.p[1] = verts[t.i1].co;
			f.p[2] = verts[t.i2].co;

			f.i[0] = t.i0 + tot;
			f.i[1] = t.i1 + tot;
			f.i[2] = t.i0 + tot;

			break;
		}
	}

	return f;
}
