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
#include "gkFallTestNode.h"
#include "gkLogger.h"
#include "gkUtils.h"
#include "gkSweptTest.h"
#include "gkGameObject.h"
#include "Ogre.h"
#include "btBulletDynamicsCommon.h"

gkFallTestNode::gkFallTestNode(gkLogicTree *parent, size_t id) 
: gkLogicNode(parent, id)
{
	ADD_ISOCK(ENABLE, false);
	ADD_ISOCK(TARGET, 0);
	ADD_OSOCK(FALLING, false);
	ADD_OSOCK(NOT_FALLING, false);
	ADD_OSOCK(COLLIDED_OBJ, 0);
	ADD_OSOCK(CONTACT_POSITION, gkVector3::ZERO);
}

bool gkFallTestNode::evaluate(gkScalar tick)
{
	m_object = GET_SOCKET_VALUE(TARGET);

	if(m_object && !m_object->isLoaded())
	{
		m_object = 0;
	}

	return m_object != 0;
}

void gkFallTestNode::update(gkScalar tick)
{
	if(GET_SOCKET_VALUE(ENABLE))
	{
		gkVector3 origin = m_object->getPosition();

		btVector3 center;
		btScalar radius;

		m_object->getCollisionObject()->getCollisionShape()->getBoundingSphere(center, radius);

		gkScalar minZ = 1.07f * radius;

		gkSweptTest::AVOID_LIST avoidList;
		avoidList.insert(m_object->getCollisionObject());
		
		gkSweptTest sweptTest(avoidList);

		Ogre::Ray ray(origin, gkVector3(0, 0, -minZ));

		if(sweptTest.collides(ray, radius))
		{
			SET_SOCKET_VALUE(CONTACT_POSITION, sweptTest.getHitPoint());
			SET_SOCKET_VALUE(COLLIDED_OBJ, sweptTest.getObject());
			SET_SOCKET_VALUE(FALLING, false);
			SET_SOCKET_VALUE(NOT_FALLING, true);
		}
		else
		{
			SET_SOCKET_VALUE(FALLING, true);
			SET_SOCKET_VALUE(NOT_FALLING, false);
		}
	}
	else
	{
		SET_SOCKET_VALUE(FALLING, false);
		SET_SOCKET_VALUE(NOT_FALLING, false);
	}
}
