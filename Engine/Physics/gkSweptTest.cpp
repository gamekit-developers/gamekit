/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) Nestor Silveira.

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
#include "gkSweptTest.h"
#include "gkEngine.h"
#include "gkScene.h"
#include "gkDynamicsWorld.h"
#include "gkPhysicsController.h"
#include "btBulletDynamicsCommon.h"

class SweptTestResultCallback : public btCollisionWorld::ClosestConvexResultCallback
{
	const gkSweptTest::AVOID_LIST& m_avoidList;
	btScalar m_allowedPenetration;
	btOverlappingPairCache* m_pairCache;
	btDispatcher* m_dispatcher;


public:
	SweptTestResultCallback(const gkSweptTest::AVOID_LIST& avoidList, const btVector3& fromA, const btVector3& toA, btOverlappingPairCache* pairCache, btDispatcher* dispatcher) :
		btCollisionWorld::ClosestConvexResultCallback(fromA, toA),
		m_avoidList(avoidList),
		m_allowedPenetration(0.0f),
		m_pairCache(pairCache),
		m_dispatcher(dispatcher)
	{
	}

	btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (m_avoidList.end() != m_avoidList.find(convexResult.m_hitCollisionObject))
			return 1.0f;

		//ignore result if there is no contact response
		if (!convexResult.m_hitCollisionObject->hasContactResponse())
			return 1.0f;

		btVector3 linVelA, linVelB;
		linVelA = m_convexToWorld - m_convexFromWorld;
		linVelB = btVector3(0, 0, 0);

		btVector3 relativeVelocity = (linVelA - linVelB);
		//don't report time of impact for motion away from the contact normal (or causes minor penetration)
		if (convexResult.m_hitNormalLocal.dot(relativeVelocity) >= -m_allowedPenetration)
			return 1.f;

		return ClosestConvexResultCallback::addSingleResult (convexResult, normalInWorldSpace);
	}

	bool needsCollision(btBroadphaseProxy* proxy0) const
	{
		//don't collide with itself
		if (m_avoidList.end() != m_avoidList.find(static_cast<btCollisionObject*>(proxy0->m_clientObject)))
			return false;

		///don't do CCD when the collision filters are not matching
		if (!ClosestConvexResultCallback::needsCollision(proxy0))
			return false;

		return true;
	}
};

/////////////////////////////////////////////////////////////////////////////

gkSweptTest::gkSweptTest(const AVOID_LIST& avoidList)
	: m_hitPointWorld(gkVector3::ZERO),
	  m_collisionObject(0),
	  m_avoidList(avoidList)
{
}

gkSweptTest::~gkSweptTest()
{
}

bool gkSweptTest::collides(const Ogre::Ray& ray, gkScalar rayRadius)
{
	gkVector3 from = ray.getOrigin();
	gkVector3 to = ray.getOrigin() + ray.getDirection();

	btVector3 rayFrom(from.x, from.y, from.z);
	btVector3 rayTo(to.x, to.y, to.z);

	btTransform start, end;
	start.setIdentity();
	end.setIdentity();

	start.setOrigin(rayFrom);
	end.setOrigin(rayTo);

	gkScene* pScene = gkEngine::getSingleton().getActiveScene();

	GK_ASSERT(pScene);

	btDynamicsWorld* pWorld = pScene->getDynamicsWorld()->getBulletWorld();

	GK_ASSERT(pWorld);

	SweptTestResultCallback rayCallback(m_avoidList, rayFrom, rayTo, pWorld->getBroadphase()->getOverlappingPairCache(), pWorld->getDispatcher());
	rayCallback.m_collisionFilterGroup = btBroadphaseProxy::AllFilter;
	rayCallback.m_collisionFilterMask = btBroadphaseProxy::AllFilter;

	btSphereShape tmpSphere(rayRadius);
	pWorld->convexSweepTest(&tmpSphere, start, end, rayCallback);

	if (rayCallback.hasHit())
	{
		m_hitPointWorld = gkVector3(rayCallback.m_hitPointWorld);

		m_collisionObject = rayCallback.m_hitCollisionObject;

		m_hitNormalWorld = gkVector3(rayCallback.m_hitNormalWorld);

		m_reflection = ray.getDirection().reflect(m_hitNormalWorld);

		gkVector3 parallelComponent = m_reflection.dotProduct(m_hitNormalWorld) * m_hitNormalWorld;

		m_sliding = m_reflection - parallelComponent;

		return true;
	}

	return false;
}

gkGameObject* gkSweptTest::getObject() const
{
	return static_cast<gkPhysicsController*>(m_collisionObject->getUserPointer())->getObject();
}
