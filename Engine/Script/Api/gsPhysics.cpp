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
#include "gsPhysics.h"
#include "gsCore.h"

gsDynamicsWorld::gsDynamicsWorld(gkDynamicsWorld* dynamicsWorld)
{
	m_world = dynamicsWorld;
}

gsDynamicsWorld::~gsDynamicsWorld()
{
}

void gsDynamicsWorld::exportBullet(const gkString& fileName)
{
	m_world->exportBullet(fileName);
}

gsRayTest::gsRayTest()
{
	m_ray = new gkRayTest();
}


gsRayTest::~gsRayTest()
{
	delete m_ray;
	m_ray = 0;
}


bool gsRayTest::cast(const gsRay& ray)
{
	return m_ray->collides(ray);
}



gsVector3 gsRayTest::getHitPoint(void)
{
	return m_ray->getHitPoint();
}

gsVector3 gsRayTest::getHitNormal(void)
{
	return m_ray->getHitNormal();
}


gkGameObject* gsRayTest::getObject(void)
{
	return m_ray->getObject();
}


float gsRayTest::getHitFraction(void)
{
	return m_ray->getHitFraction();
}



gsSweptTest::gsSweptTest()
{
	m_test = new gkSweptTest(m_avoid);
}



gsSweptTest::gsSweptTest(gsArray<gsGameObject, gkGameObject> &avoid)
{
	if (!avoid.empty())
	{
		int size = avoid.size(), i;
		for (i = 0; i < size; ++i)
		{
			gkGameObject* ob = avoid.iat(i);
			btCollisionObject* col = 0;
			if ((col = ob->getCollisionObject()) != 0)
				m_avoid.insert(col);
		}
	}

	m_test = new gkSweptTest(m_avoid);
}


gsSweptTest::~gsSweptTest()
{
	delete m_test;
}


bool gsSweptTest::collides(const gsRay& ray, float rayRadius)
{
	return m_test->collides(ray, rayRadius);
}


gsVector3 gsSweptTest::getHitPoint(void)
{
	return m_test->getHitPoint();
}


gsVector3 gsSweptTest::getHitNormal(void)
{
	return m_test->getHitNormal();
}



gsVector3 gsSweptTest::getReflection(void)
{
	return m_test->getReflection();
}


gsVector3 gsSweptTest::getSliding(void)
{
	return m_test->getSliding();
}


gkGameObject* gsSweptTest::getObject(void)
{
	return m_test->getObject();
}


gsCharacter::gsCharacter(gkCharacter* object)
{
	m_character = object;
	GK_ASSERT(m_character);
} 

gsCharacter::~gsCharacter(void)
{
} 

gkGameObject* gsCharacter::getObject(void)
{
	return m_character->getObject();
} 

void gsCharacter::setLinearVelocity(float forward, float backward, float rightward, float leftward, float timeInterval)
{
	m_character->setLinearVelocity(forward, backward, rightward, leftward, timeInterval); 
} 

void gsCharacter::setGravity(float gravity)
{
	m_character->setGravity(btScalar(gravity));
}

void gsCharacter::setRotation(const gsVector3& axis, float scalar)
{
	m_character->setRotation(axis, scalar);	
}

void gsCharacter::setJumpSpeed(float scalar)
{
	m_character->setJumpSpeed(scalar);
}

void gsCharacter::jump(void)
{
	m_character->jump();
}

bool gsCharacter::isOnGround(void)
{
	return m_character->isOnGround();
}

