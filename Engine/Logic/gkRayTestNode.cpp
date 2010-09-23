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
#include "gkRayTestNode.h"
#include "gkLogger.h"
#include "gkUtils.h"
#include "gkRayTest.h"
#include "gkGameObject.h"
#include "gkPhysicsController.h"
#include "gkCam2ViewportRay.h"
#include "Ogre.h"
#include "btBulletDynamicsCommon.h"

gkRayTestNode::gkRayTestNode(gkLogicTree* parent, size_t id)
	: gkLogicNode(parent, id)
{
	ADD_ISOCK(ENABLE, true);
	ADD_ISOCK(TARGET, 0);
	ADD_ISOCK(RAY_ORIGIN_OFFSET, gkVector3::ZERO);
	ADD_ISOCK(RAY_DIRECTION, gkVector3::UNIT_Y);
	ADD_OSOCK(HIT, false);
	ADD_OSOCK(NOT_HIT, false);
	ADD_OSOCK(HIT_OBJ, 0);
	ADD_OSOCK(HIT_NAME, "");
	ADD_OSOCK(HIT_POSITION, gkVector3::ZERO);
}

bool gkRayTestNode::evaluate(gkScalar tick)
{
	m_object = GET_SOCKET_VALUE(TARGET);

	if (m_object && !m_object->isInstanced())
	{
		m_object = 0;
	}

	return m_object != 0;
}

void gkRayTestNode::update(gkScalar tick)
{
	SET_SOCKET_VALUE(HIT, false);
	SET_SOCKET_VALUE(NOT_HIT, true);
	SET_SOCKET_VALUE(HIT_OBJ, 0);
	SET_SOCKET_VALUE(HIT_NAME, "");
	SET_SOCKET_VALUE(HIT_POSITION, gkVector3::ZERO);

	if (GET_SOCKET_VALUE(ENABLE))
	{
		gkVector3 origin = m_object->getPosition() + GET_SOCKET_VALUE(RAY_ORIGIN_OFFSET);

		gkVector3 dir = m_object->getOrientation() * GET_SOCKET_VALUE(RAY_DIRECTION);

		Ogre::Ray ray(origin, dir);

		gkRayTest rayTest;

		if (rayTest.collides(ray))
		{
			btCollisionObject* pCol = rayTest.getCollisionObject();

			gkGameObject* pObj = gkPhysicsController::castObject(pCol);

			if (pObj != m_object)
			{
				SET_SOCKET_VALUE(HIT_POSITION, rayTest.getHitPoint());
				SET_SOCKET_VALUE(HIT_OBJ, pObj);
				SET_SOCKET_VALUE(HIT_NAME, pObj->getName());
				SET_SOCKET_VALUE(HIT, true);
				SET_SOCKET_VALUE(NOT_HIT, false);
			}
		}
	}
}

///////////////////////////////////////////////////

gkScreenRayTestNode::gkScreenRayTestNode(gkLogicTree* parent, size_t id)
	: gkLogicNode(parent, id)
{
	ADD_ISOCK(ENABLE, true);
	ADD_ISOCK(SCREEN_X, 0);
	ADD_ISOCK(SCREEN_Y, 0);
	ADD_OSOCK(HIT, false);
	ADD_OSOCK(NOT_HIT, false);
	ADD_OSOCK(HIT_OBJ, 0);
	ADD_OSOCK(HIT_NAME, "");
	ADD_OSOCK(HIT_POSITION, gkVector3::ZERO);
}

bool gkScreenRayTestNode::evaluate(gkScalar tick)
{

	if (GET_SOCKET_VALUE(ENABLE))
	{
		gkCam2ViewportRay ray(GET_SOCKET_VALUE(SCREEN_X), GET_SOCKET_VALUE(SCREEN_Y));

		gkRayTest rayTest;

		if (rayTest.collides(ray))
		{
			btCollisionObject* pCol = rayTest.getCollisionObject();

			gkGameObject* pObj = gkPhysicsController::castObject(pCol);

			SET_SOCKET_VALUE(HIT_POSITION, rayTest.getHitPoint());
			SET_SOCKET_VALUE(HIT_OBJ, pObj);
			SET_SOCKET_VALUE(HIT_NAME, pObj->getName());
			SET_SOCKET_VALUE(HIT, true);
			SET_SOCKET_VALUE(NOT_HIT, false);
		}
		else
		{
			SET_SOCKET_VALUE(HIT, false);
			SET_SOCKET_VALUE(NOT_HIT, true);
			SET_SOCKET_VALUE(HIT_OBJ, 0);
			SET_SOCKET_VALUE(HIT_NAME, "");
			SET_SOCKET_VALUE(HIT_POSITION, gkVector3::ZERO);
		}

	}

	return false;
}
