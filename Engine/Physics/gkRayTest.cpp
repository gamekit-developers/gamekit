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
#include "gkRayTest.h"
#include "gkEngine.h"
#include "gkScene.h"
#include "gkDynamicsWorld.h"
#include "gkPhysicsController.h"
#include "btBulletDynamicsCommon.h"


gkRayTest::gkRayTest()
	: m_hitPointWorld(gkVector3::ZERO),
	  m_hitNormalWorld(gkVector3::ZERO),
	  m_collisionObject(0)
{
}

gkRayTest::~gkRayTest()
{
}

bool gkRayTest::collides(const Ogre::Ray& ray)
{
	gkVector3 from = ray.getOrigin();
	gkVector3 to = ray.getOrigin() + ray.getDirection();

	btVector3 rayFrom(from.x, from.y, from.z);
	btVector3 rayTo(to.x, to.y, to.z);

	btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom, rayTo);
	rayCallback.m_collisionFilterGroup = btBroadphaseProxy::AllFilter;
	rayCallback.m_collisionFilterMask = btBroadphaseProxy::AllFilter;

	gkScene* pScene = gkEngine::getSingleton().getActiveScene();

	GK_ASSERT(pScene);

	btDynamicsWorld* pWorld = pScene->getDynamicsWorld()->getBulletWorld();

	GK_ASSERT(pWorld);

	pWorld->rayTest(rayFrom, rayTo, rayCallback);

	if (rayCallback.hasHit())
	{
		m_hitPointWorld = gkVector3(rayCallback.m_hitPointWorld);

		m_hitNormalWorld = gkVector3(rayCallback.m_hitNormalWorld);

		m_collisionObject = rayCallback.m_collisionObject;

		m_hitFraction = rayCallback.m_closestHitFraction;

		return true;
	}

	return false;
}

gkGameObject* gkRayTest::getObject() const
{
	return static_cast<gkPhysicsController*>(m_collisionObject->getUserPointer())->getObject();
}
