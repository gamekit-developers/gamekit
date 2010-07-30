/*
-------------------------------------------------------------------------------
--                                                                           --
--  This file is part of GameKit.                                            --
--  http://gamekit.googlecode.com/                                           --
--                                                                           --
--                                                                           --
--  Released under the zlib license.                                         --
--  http://www.gzip.org/zlib/zlib_license.html                               --
--                                                                           --
--                                                                           --
-------------------------------------------------------------------------------
-- OgreKit Physics Bindings
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