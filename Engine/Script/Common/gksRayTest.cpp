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
-- Generic Script API Binding                                                --
-------------------------------------------------------------------------------
*/
#include "gkGameObject.h"
#include "gksGameObject.h"
#include "gksRayTest.h"
#include "gkRayTest.h"
#include "OgreRay.h"

// ----------------------------------------------------------------------------
gksRayTest::gksRayTest()
{
	m_ray = new gkRayTest();
}

// ----------------------------------------------------------------------------
gksRayTest::~gksRayTest()
{
	delete m_ray;
	m_ray = 0;
}

// ----------------------------------------------------------------------------
bool gksRayTest::cast(const gksVector3 &from, const gksVector3 &to)
{
	return m_ray->collides(Ogre::Ray(from, to));
}


// ----------------------------------------------------------------------------
gksVector3 gksRayTest::getHitPoint(void)
{
	return m_ray->getHitPoint();
}

// ----------------------------------------------------------------------------
gksPointer<gksGameObject> gksRayTest::getObject(void)
{
	return gksPointer<gksGameObject>(new gksGameObject(m_ray->getObject()));
}

// ----------------------------------------------------------------------------
float gksRayTest::getHitFraction(void)
{
	return m_ray->getHitFraction();
}

// ----------------------------------------------------------------------------
gksPointer<gksRayTest> newRayTest(void)
{
	return gksPointer<gksRayTest>(new gksRayTest());
}
