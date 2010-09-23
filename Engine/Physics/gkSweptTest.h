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
#ifndef _gkSweptTest_h_
#define _gkSweptTest_h_

#include "gkCommon.h"
#include "gkMathUtils.h"

class btCollisionObject;

class gkSweptTest
{
public:
	typedef std::set<btCollisionObject*> AVOID_LIST;

	gkSweptTest(const AVOID_LIST& avoidList);

	~gkSweptTest();

	bool collides(const Ogre::Ray& ray, gkScalar rayRadius);

	const gkVector3& getHitPoint() const { return m_hitPointWorld; }

	const gkVector3& getHitNormal() const { return m_hitNormalWorld; }

	const gkVector3& getReflection() const { return m_reflection; }

	const gkVector3& getSliding() const { return m_sliding; }

	btCollisionObject* getCollisionObject() const { return m_collisionObject; }

	gkGameObject* getObject() const;

private:

	const AVOID_LIST& m_avoidList;

	gkVector3 m_hitPointWorld;

	btCollisionObject* m_collisionObject;

	gkVector3 m_hitNormalWorld;

	// Reflection after hit
	gkVector3 m_reflection;

	// sliding vector along the plane(m_hitNormalWorld)
	// using the remaining component (after hit) of the ray direction component
	gkVector3 m_sliding;
};


#endif//_gkSweptTest_h_
