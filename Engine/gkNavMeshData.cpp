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

#include "gkNavMeshData.h"
#include "gkEngine.h"
#include "gkScene.h"
#include "gkDynamicsWorld.h"

#include "btBulletDynamicsCommon.h"

gkNavMeshData::gkNavMeshData() : m_object(0)//, m_debug(0), os(L"momo.obj")
{
	//gkScene* pScene = gkEngine::getSingleton().getActiveScene();

	//GK_ASSERT(pScene);

	//m_debug = new gkPhysicsDebug(pScene->getDynamicsWorld());
}

gkNavMeshData::~gkNavMeshData()
{
	//delete m_debug;
}

bool gkNavMeshData::refresh(gkGameObject* pObj)
{
	m_object = pObj;

	if(m_object)
	{
		AddCollisionObj();
	}

	return true;
}

void gkNavMeshData::processTriangle(btVector3* triangle,int partId, int triangleIndex)
{
	btVector3 v1 = m * triangle[0];
	btVector3 v2 = m * triangle[1];
	btVector3 v3 = m * triangle[2];

	gkVector3 vv1(v1.x(), v1.z(), v1.y());
	gkVector3 vv2(v2.x(), v2.z(), v2.y());
	gkVector3 vv3(v3.x(), v3.z(), v3.y());

	addTriangle(vv1, vv2, vv3, triangleIndex);

//	m_debug->drawLine(v1, v2, btVector3(1,1,0));
//	m_debug->drawLine(v2, v3, btVector3(1,1,0));
//	m_debug->drawLine(v3, v1, btVector3(1,1,0));
}

void gkNavMeshData::addTriangle(const gkVector3& v1, const gkVector3& v2, const gkVector3& v3, int triangleIndex)
{
	if(m_object->getNavIndex() == -1)
	{
		size_t n = m_verts.size();

		int a = n; 
		int b = n+1;
		int c = n+2;

		m_verts.push_back(v1);
		m_verts.push_back(v2);
		m_verts.push_back(v3);

		m_tris.push_back(a);
		m_tris.push_back(b);
		m_tris.push_back(c);

		gkVector3 normal((v3-v1).crossProduct(v2-v1)); 
		normal.normalise();

		m_normals.push_back(normal);

		m_tris.push_back(c);
		m_tris.push_back(b);
		m_tris.push_back(a);

		m_normals.push_back(-normal);
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
		size_t tBaseIndex = m_object->getNavIndex() + triangleIndex;
		size_t vBaseIndex = tBaseIndex/2;
		size_t nBaseIndex = tBaseIndex/6;

		m_verts[vBaseIndex] = v1;
		m_verts[vBaseIndex+1] = v2;
		m_verts[vBaseIndex+2] = v3;

		gkVector3 normal((v3-v1).crossProduct(v2-v1)); 
		normal.normalise();

		m_normals[nBaseIndex] = normal;
		m_normals[nBaseIndex+1] = -normal;
	}
}

void gkNavMeshData::AddCollisionObj()
{
	size_t tIndex = m_tris.size();

	gkScene* pScene = m_object->getOwner();

	GK_ASSERT(pScene);

	btVector3 aabbMin,aabbMax;
	pScene->getDynamicsWorld()->getBulletWorld()->getBroadphase()->getBroadphaseAabb(aabbMin,aabbMax);
	
	aabbMin-=btVector3(BT_LARGE_FLOAT,BT_LARGE_FLOAT,BT_LARGE_FLOAT);
	aabbMax+=btVector3(BT_LARGE_FLOAT,BT_LARGE_FLOAT,BT_LARGE_FLOAT);

	btCollisionObject* colObj = m_object->getAttachedObject()->getCollisionObject();

	const btCollisionShape* shape = colObj->getCollisionShape();

	m = colObj->getWorldTransform();

	int shapetype=shape->getShapeType();

	switch(shapetype)
	{
		case SPHERE_SHAPE_PROXYTYPE:
		{
			break;
		}

		case BOX_SHAPE_PROXYTYPE:
		{
			const btBoxShape* boxShape = static_cast<const btBoxShape*>(shape);
			btVector3 halfExtent = boxShape->getHalfExtentsWithMargin();
			
			static int indices[36] = {
				0,1,2,
				3,2,1,
				4,0,6,
				6,0,2,
				5,1,4,
				4,1,0,
				7,3,1,
				7,1,5,
				5,4,7,
				7,4,6,
				7,2,3,
				7,6,2};

			static btVector3 vertices[8]={	btVector3(1,1,1),btVector3(-1,1,1),	btVector3(1,-1,1),	btVector3(-1,-1,1),	btVector3(1,1,-1),	btVector3(-1,1,-1),	btVector3(1,-1,-1),	btVector3(-1,-1,-1)};
			int si=36;
			for (int i=0;i<si;i+=3)
			{
				btVector3 v1 = m * (vertices[indices[i]]*halfExtent);
				btVector3 v2 = m * (vertices[indices[i+1]]*halfExtent);
				btVector3 v3 = m * (vertices[indices[i+2]]*halfExtent);

				gkVector3 vv1(v1.x(), v1.z(), v1.y());
				gkVector3 vv2(v2.x(), v2.z(), v2.y());
				gkVector3 vv3(v3.x(), v3.z(), v3.y());

				addTriangle(vv1, vv2, vv3, i/3);

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

	if(m_object->getNavIndex() == -1)
	{
		m_object->setNavIndex(tIndex);
	}
}
