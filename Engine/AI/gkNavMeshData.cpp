/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Nestor Silveira.

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
#include "btBulletDynamicsCommon.h"

#include "gkEngine.h"
#include "gkScene.h"
#include "gkDynamicsWorld.h"
#include "gkRigidBody.h"

#include "gkNavMeshData.h"
#include "gkGameObject.h"
#include "gkLogger.h"

typedef gkGameObject::NavMeshData NavMeshData;

gkNavMeshData::gkNavMeshData(gkScene* scene)
	: m_object(0), m_scene(scene), m_hasChanged(false)
{
	GK_ASSERT(m_scene);
}

gkNavMeshData::~gkNavMeshData()
{
	destroyInstances();
}

void gkNavMeshData::destroyInstance(gkGameObject* pObj)
{
	if (!isValid(pObj)) return;

	{
		gkCriticalSection::Lock guard(m_cs);

		if (m_data.tris.empty()) return;
	}

	GK_ASSERT(!pObj->getNavData().isEmpty());

	int indexBase = pObj->getNavData().triangleBaseIndex;
	size_t vertexBaseIndex = indexBase / 2;
	size_t normalBaseIndex = indexBase / 6;

	int nIndex = pObj->getNavData().nIndex;
	size_t nVertex = nIndex / 2;
	size_t nNormal = nIndex / 3;

	int indexValue = nIndex / 2;

	{
		gkCriticalSection::Lock guard(m_cs);

		gkMeshData::TRIANGLES::iterator it = m_data.tris.erase(m_data.tris.begin() + indexBase, m_data.tris.begin() + indexBase + nIndex);
		m_data.verts.erase(m_data.verts.begin() + vertexBaseIndex, m_data.verts.begin() + vertexBaseIndex + nVertex);
		m_data.normals.erase(m_data.normals.begin() + normalBaseIndex, m_data.normals.begin() + normalBaseIndex + nNormal);

		while (it != m_data.tris.end())
		{
			if (*it > indexBase)
			{
				*it -= indexValue;
			}

			++it;
		}
	}

	pObj->resetNavData();

	gkGameObjectSet& objs = m_scene->getInstancedObjects();
	gkGameObjectSet::Iterator it = objs.iterator();
	while (it.hasMoreElements())
	{
		gkGameObject* p = it.getNext();
		p->recalcNavData(indexBase, nIndex);
	}

	m_hasChanged = true;
}

void gkNavMeshData::updateOrCreate(gkGameObject* pObj)
{
	if (!isValid(pObj)) return;

	m_object = pObj;

	if (m_object && m_object->isInstanced())
	{
		AddCollisionObj();
	}

	m_hasChanged = true;
}

void gkNavMeshData::destroyInstances()
{
	gkGameObjectSet& objs = m_scene->getInstancedObjects();
	gkGameObjectSet::Iterator it = objs.iterator();
	while (it.hasMoreElements())
	{
		gkGameObject* pObj = it.getNext();
		pObj->resetNavData();
	}

	gkCriticalSection::Lock guard(m_cs);

	m_data.verts.clear();
	m_data.tris.clear();
	m_data.normals.clear();

	m_hasChanged = true;
}

void gkNavMeshData::createInstances()
{
	gkGameObjectSet& objs = m_scene->getInstancedObjects();
	gkGameObjectSet::Iterator it = objs.iterator();
	while (it.hasMoreElements())
	{
		gkGameObject* pObj = it.getNext();
		updateOrCreate(pObj);
	}

	m_hasChanged = true;
}

void gkNavMeshData::processTriangle(btVector3* triangle, int partId, int triangleIndex)
{
	btVector3 v1 = m * triangle[0];
	btVector3 v2 = m * triangle[1];
	btVector3 v3 = m * triangle[2];

	gkVector3 vv1(v1.x(), v1.z(), v1.y());
	gkVector3 vv2(v2.x(), v2.z(), v2.y());
	gkVector3 vv3(v3.x(), v3.z(), v3.y());

	addTriangle(vv1, vv2, vv3, triangleIndex);

//    m_debug->drawLine(v1, v2, btVector3(1,1,0));
//    m_debug->drawLine(v2, v3, btVector3(1,1,0));
//    m_debug->drawLine(v3, v1, btVector3(1,1,0));
}

void gkNavMeshData::addTriangle(const gkVector3& v1, const gkVector3& v2, const gkVector3& v3, int triangleIndex)
{
	if (m_object->getNavData().isEmpty())
	{
		size_t n = m_data.verts.size();

		int a = n;
		int b = n + 1;
		int c = n + 2;

		m_data.verts.push_back(v1);
		m_data.verts.push_back(v2);
		m_data.verts.push_back(v3);

		m_data.tris.push_back(a);
		m_data.tris.push_back(b);
		m_data.tris.push_back(c);

		gkVector3 normal((v3 - v1).crossProduct(v2 - v1));
		normal.normalise();

		m_data.normals.push_back(normal);

		m_data.tris.push_back(c);
		m_data.tris.push_back(b);
		m_data.tris.push_back(a);

		m_data.normals.push_back(-normal);
		/*
		    os << "v " << v1.x << " " << v1.y << " " << v1.z << "\n";
		    os << "v " << v2.x << " " << v2.y << " " << v2.z << "\n";
		    os << "v " << v3.x << " " << v3.y << " " << v3.z << "\n";

		    a++; b++; c++;
		    os << "f " << a  << " " << b << " " << c << "\n";
		    os << "f " << c  << " " << b << " " << a << "\n";
		*/
	}
	else
	{
		size_t tBaseIndex = m_object->getNavData().triangleBaseIndex + triangleIndex;
		size_t vBaseIndex = tBaseIndex / 2;
		size_t nBaseIndex = tBaseIndex / 6;

		gkVector3& ov1 = m_data.verts[vBaseIndex];
		gkVector3& ov2 = m_data.verts[vBaseIndex+1];
		gkVector3& ov3 = m_data.verts[vBaseIndex+2];

		ov1 = v1;
		ov2 = v2;
		ov3 = v3;

		gkVector3 normal((v3 - v1).crossProduct(v2 - v1));
		normal.normalise();

		m_data.normals[nBaseIndex] = normal;
		m_data.normals[nBaseIndex+1] = -normal;
	}
}

struct minH : public std::binary_function<gkVector3, gkVector3, bool>
{
	bool operator()(const gkVector3& a, const gkVector3& b) const  { return a.y < b.y; }
};

void gkNavMeshData::AddCollisionObj()
{
	gkCriticalSection::Lock guard(m_cs);

	size_t vIndex = m_data.verts.size();
	size_t tIndex = m_data.tris.size();

	gkScene* pScene = m_object->getOwner();

	GK_ASSERT(pScene);

	btVector3 aabbMin, aabbMax;
	pScene->getDynamicsWorld()->getBulletWorld()->getBroadphase()->getBroadphaseAabb(aabbMin, aabbMax);

	aabbMin -= btVector3(BT_LARGE_FLOAT, BT_LARGE_FLOAT, BT_LARGE_FLOAT);
	aabbMax += btVector3(BT_LARGE_FLOAT, BT_LARGE_FLOAT, BT_LARGE_FLOAT);

	btCollisionObject* colObj = m_object->getPhysicsController()->getCollisionObject();

	const btCollisionShape* shape = colObj->getCollisionShape();

	m = colObj->getWorldTransform();

	int shapetype = shape->getShapeType();

	switch (shapetype)
	{
	case SPHERE_SHAPE_PROXYTYPE:
		{
			break;
		}

	case BOX_SHAPE_PROXYTYPE:
		{
			const btBoxShape* boxShape = static_cast<const btBoxShape*>(shape);
			btVector3 halfExtent = boxShape->getHalfExtentsWithMargin();

			static int indices[36] =
			{
				0, 1, 2,
				3, 2, 1,
				4, 0, 6,
				6, 0, 2,
				5, 1, 4,
				4, 1, 0,
				7, 3, 1,
				7, 1, 5,
				5, 4, 7,
				7, 4, 6,
				7, 2, 3,
				7, 6, 2
			};

			static btVector3 vertices[8] = {    btVector3(1, 1, 1), btVector3(-1, 1, 1),    btVector3(1, -1, 1),    btVector3(-1, -1, 1),    btVector3(1, 1, -1),    btVector3(-1, 1, -1),    btVector3(1, -1, -1),    btVector3(-1, -1, -1)};
			int si = 36;
			for (int i = 0; i < si; i += 3)
			{
				btVector3 v1 = m * (vertices[indices[i]] * halfExtent);
				btVector3 v2 = m * (vertices[indices[i+1]] * halfExtent);
				btVector3 v3 = m * (vertices[indices[i+2]] * halfExtent);

				gkVector3 vv1(v1.x(), v1.z(), v1.y());
				gkVector3 vv2(v2.x(), v2.z(), v2.y());
				gkVector3 vv3(v3.x(), v3.z(), v3.y());

				addTriangle(vv1, vv2, vv3, i / 3);

				//m_debug->drawLine(v1, v2, btVector3(1,1,0));
				//m_debug->drawLine(v2, v3, btVector3(1,1,0));
				//m_debug->drawLine(v3, v1, btVector3(1,1,0));
			}

			break;
		}

	default:

		if (shape->isConcave() && !shape->isInfinite())
		{
			btConcaveShape* concaveMesh = (btConcaveShape*) shape;

			concaveMesh->processAllTriangles(this, aabbMin, aabbMax);
		}

		break;
	}

	if (m_object->getNavData().isEmpty())
	{
		size_t n = m_data.tris.size() - tIndex;

		GK_ASSERT(n >= 6); // minimum 2 triangles

		gkMeshData::VERTS::iterator it0 = m_data.verts.begin() + vIndex;
		gkMeshData::VERTS::iterator it1 = m_data.verts.end();

		float hMin = std::min_element(it0, it1, minH())->y;

		float hMax = std::max_element(it0, it1, minH())->y;

		m_object->setNavData(NavMeshData(tIndex, n, hMin, hMax));
	}
}

bool gkNavMeshData::isValid(gkGameObject* pObj)
{
	const gkPhysicsProperties& physicsState = pObj->getProperties().m_physics;

	bool active = false;

	btCollisionObject* pColObj = pObj->getCollisionObject();

	if (physicsState.isRigid())
	{
		GK_ASSERT(pColObj);

		int current_state = pColObj->getActivationState();

		int old_state = pObj->getAttachedBody()->setActivationState(current_state);

		active = current_state == WANTS_DEACTIVATION && old_state == ACTIVE_TAG;
	}
	else if (physicsState.isStatic())
	{
		// Sanity checks with parent / child physics controllers.
		GK_ASSERT(pColObj);

		active = true;
	}

	return active;
}
