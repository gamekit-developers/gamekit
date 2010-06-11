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
*/
#include "gkGameObject.h"
#include "gkLuaGameObject.h"
#include "gkLuaRayTest.h"
#include "gkRayTest.h"
#include "OgreRay.h"

// ----------------------------------------------------------------------------

RayTest::RayTest()
{
    m_ray = new gkRayTest();
}

RayTest::~RayTest()
{
    delete m_ray;
    m_ray = 0;
}


bool RayTest::cast(const Vector3 &from, const Vector3 &to)
{
    return m_ray->collides(Ogre::Ray(from, to));
}


Vector3 RayTest::getHitPoint(void)
{
    return m_ray->getHitPoint();
}

GameObjectPtr RayTest::getObject(void)
{
    return GameObjectPtr(new GameObject(m_ray->getObject()));
}

float RayTest::getHitFraction(void)
{
    return m_ray->getHitFraction();
}

// ----------------------------------------------------------------------------
RayTestPtr newRayTest(void)
{
    return RayTestPtr(new RayTest());
}

