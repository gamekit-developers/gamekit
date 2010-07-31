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
#include "OgreKitPhysics.h"
#include "OgreKitCore.h"


namespace OgreKit
{
// ----------------------------------------------------------------------------
RayTest::RayTest()
{
	m_ray = new gkRayTest();
}

// ----------------------------------------------------------------------------
RayTest::~RayTest()
{
	delete m_ray;
	m_ray = 0;
}

// ----------------------------------------------------------------------------
bool RayTest::cast(const Vector3 &from, const Vector3 &to)
{
	return m_ray->collides(Ogre::Ray(from, to));
}


// ----------------------------------------------------------------------------
Vector3 RayTest::getHitPoint(void)
{
	return m_ray->getHitPoint();
}
// ----------------------------------------------------------------------------
Vector3 RayTest::getHitNormal(void)
{
	return m_ray->getHitNormal();
}

// ----------------------------------------------------------------------------
Pointer<GameObject> RayTest::getObject(void)
{
	return Pointer<GameObject>(new GameObject(m_ray->getObject()));
}

// ----------------------------------------------------------------------------
float RayTest::getHitFraction(void)
{
	return m_ray->getHitFraction();
}


// ----------------------------------------------------------------------------
SweptTest::SweptTest(const Pointer<GameObject> &avoid)
{
	if (!avoid.isNull())
	{
		btCollisionObject *obj = avoid->cast<gkGameObject>()->getCollisionObject();
		if (obj)
			empty_fixme.insert(obj);
	}

	m_test = new gkSweptTest(empty_fixme);
}


// ----------------------------------------------------------------------------
SweptTest::~SweptTest()
{
	delete m_test;
}

// ----------------------------------------------------------------------------
bool SweptTest::collides(const Ray &ray, float rayRadius)
{
	return m_test->collides(ray, rayRadius);
}

// ----------------------------------------------------------------------------
Vector3 SweptTest::getHitPoint(void)
{
	return m_test->getHitPoint();
}

// ----------------------------------------------------------------------------
Vector3 SweptTest::getHitNormal(void)
{
	return m_test->getHitNormal();
}


// ----------------------------------------------------------------------------
Vector3 SweptTest::getReflection(void)
{
	return m_test->getReflection();
}

// ----------------------------------------------------------------------------
Vector3 SweptTest::getSliding(void)
{
	return m_test->getSliding();
}

// ----------------------------------------------------------------------------
Pointer<GameObject> SweptTest::getObject(void)
{
	return Pointer<GameObject>(new GameObject(m_test->getObject()));
}


}
