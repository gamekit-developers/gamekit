/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) Nestor Silveira.

    Contributor(s): Alberto Torres.
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
#include "gkRayTest.h"
#include "gkEngine.h"
#include "gkScene.h"
#include "gkDynamicsWorld.h"
#include "gkPhysicsController.h"
#include "btBulletDynamicsCommon.h"


gkRayTest::gkRayTest(gkScene* scene)
	: m_hitPointWorld(gkVector3::ZERO),
	  m_hitNormalWorld(gkVector3::ZERO),
	  m_collisionObject(0),
	  m_hitFraction(0)
{
	m_scene = scene?scene:gkEngine::getSingleton().getActiveScene();
}

gkRayTest::~gkRayTest()
{
}

bool gkRayTest::collides(const Ogre::Ray& ray)
{
	gkVector3 from = ray.getOrigin();
	gkVector3 to = ray.getOrigin() + ray.getDirection();
	gkRayTestFilter test;
	return gkRayTest::collides(from, to,test);
}

bool gkRayTest::collides(const gkVector3& from, const gkVector3& to,
			  gkRayTest::gkRayTestFilter& rayCallback)
{
	btVector3 rayFrom(from.x, from.y, from.z);
	btVector3 rayTo(to.x, to.y, to.z);
	btVector3 hitPointWorld;
	
	rayCallback.m_collisionFilterGroup = btBroadphaseProxy::AllFilter;
	rayCallback.m_collisionFilterMask = btBroadphaseProxy::AllFilter;


	GK_ASSERT(m_scene);

	btDynamicsWorld* pWorld = m_scene->getDynamicsWorld()->getBulletWorld();

	GK_ASSERT(pWorld);

	pWorld->rayTest(rayFrom, rayTo, rayCallback);

	if (rayCallback.hasHit())
	{
		m_hitNormalWorld = gkVector3(rayCallback.m_hitNormalWorld);

		m_collisionObject = rayCallback.m_collisionObject;

		m_hitFraction = rayCallback.m_closestHitFraction;

		hitPointWorld.setInterpolate3(rayFrom, rayTo, m_hitFraction);
		m_hitPointWorld = gkVector3(hitPointWorld);

		return true;
	}

	return false;
}

gkGameObject* gkRayTest::getObject() const
{
	return static_cast<gkPhysicsController*>(m_collisionObject->getUserPointer())->getObject();
}


btScalar gkRayTest::gkRayTestFilter::addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace)
{
	//caller already does the filter on the m_closestHitFraction
	btAssert(rayResult.m_hitFraction <= m_closestHitFraction);
	
	m_closestHitFraction = rayResult.m_hitFraction;
	m_collisionObject = rayResult.m_collisionObject;
	if (normalInWorldSpace)
	{
		m_hitNormalWorld = rayResult.m_hitNormalLocal;
	} else
	{
		///need to transform normal into worldspace
		m_hitNormalWorld = m_collisionObject->getWorldTransform().getBasis()*rayResult.m_hitNormalLocal;
	}
	return rayResult.m_hitFraction;
}



bool notMeFilter::filterFunc(btCollisionObject* ob) const{
	gkGameObject *other = gkPhysicsController::castObject(ob);
	
	return other!=m_self;
}



bool xrayFilter::filterFunc(btCollisionObject* ob) const{
	gkGameObject *other = gkPhysicsController::castObject(ob);
	
	return other!=m_self && gkPhysicsController::sensorTest(
		other, m_prop, m_material, false);
}



bool gkRayTest::gkRayTestFilter::needsCollision(btBroadphaseProxy* proxy0) const
{
	bool collides = (proxy0->m_collisionFilterGroup & m_collisionFilterMask) != 0;
	collides = collides && (m_collisionFilterGroup & proxy0->m_collisionFilterMask);
	collides = collides && filterFunc((btCollisionObject*)proxy0->m_clientObject);
	return collides;
}

/*
bool gkRayTestCStyleFilter::needsCollision(btBroadphaseProxy* proxy0) const
{
	bool collides = (proxy0->m_collisionFilterGroup & m_collisionFilterMask) != 0;
	collides = collides && (m_collisionFilterGroup & proxy0->m_collisionFilterMask);
	if (m_filterFunc!=NULL)
		collides = collides && m_filterFunc(
			(btCollisionObject*)proxy0->m_clientObject, m_filterFuncData
		);
	return collides;
}
*/

