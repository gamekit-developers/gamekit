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

#include "OgreRoot.h"
#include "gkCameraNode.h"
#include "gkEngine.h"
#include "gkGameObject.h"
#include "gkUtils.h"
#include "gkSweptTest.h"
#include "gkPhysicsController.h"
#include "gkLogger.h"
#include "gkCamera.h"
#include "btBulletDynamicsCommon.h"
#include <limits>

gkCameraNode::gkCameraNode(gkLogicTree* parent, size_t id)
	: gkLogicNode(parent, id),
	  m_center(gkVector3::ZERO),
	  m_oldCenter(gkVector3::ZERO),
	  m_target(0),
	  m_centerObj(0),
	  m_rollNode(gkQuaternion::IDENTITY),
	  m_pitchNode(gkQuaternion::IDENTITY),
	  m_idealRadius(0),
	  m_radiusIdealIsSet(false),
	  m_oldRadius(0),
	  m_oldRadiusIsSet(false)
{
	ADD_ISOCK(UPDATE, true);
	ADD_ISOCK(CENTER_OBJ, 0);
	ADD_ISOCK(CENTER_POSITION, gkVector3::ZERO);
	ADD_ISOCK(INITIAL_ROLL, 0);
	ADD_ISOCK(INITIAL_PITCH, 45);
	ADD_ISOCK(REL_X, 0);
	ADD_ISOCK(REL_Y, 0);
	ADD_ISOCK(REL_Z, 0);
	ADD_ISOCK(TARGET, 0);
	ADD_ISOCK(MIN_PITCH, 0);
	ADD_ISOCK(MAX_PITCH, 80);
	ADD_ISOCK(MIN_ROLL, -180);
	ADD_ISOCK(MAX_ROLL, 180);
	ADD_ISOCK(KEEP_DISTANCE, true);
	ADD_ISOCK(MIN_Z, 0);
	ADD_ISOCK(MAX_Z, std::numeric_limits<gkScalar>::infinity());
	ADD_ISOCK(AVOID_BLOCKING, false);
	ADD_ISOCK(BLOCKING_RADIUS, 0.3f);
	ADD_ISOCK(STIFNESS, 0.8f);
	ADD_ISOCK(DAMPING, 0.3f);
	ADD_OSOCK(CURRENT_ROLL, gkQuaternion::IDENTITY);
	ADD_OSOCK(CURRENT_PITCH, gkQuaternion::IDENTITY)
}

gkCameraNode::~gkCameraNode()
{
}

bool gkCameraNode::evaluate(gkScalar tick)
{
	m_centerObj = GET_SOCKET_VALUE(CENTER_OBJ);

	if (m_target != GET_SOCKET_VALUE(TARGET))
	{
		m_radiusIdealIsSet = false;

		m_oldRadiusIsSet = false;

		m_target = GET_SOCKET_VALUE(TARGET);

		m_rollNode = gkQuaternion(gkDegree(GET_SOCKET_VALUE(INITIAL_ROLL)), gkVector3::UNIT_Z);

		m_pitchNode = gkQuaternion(gkDegree(GET_SOCKET_VALUE(INITIAL_PITCH)), gkVector3::UNIT_X);

		m_oldCenter = m_center = GET_SOCKET_VALUE(CENTER_POSITION);
	}

	bool update = GET_SOCKET_VALUE(UPDATE);

	return update && m_centerObj && m_target && m_centerObj->isInstanced() && m_target->isInstanced();
}

void gkCameraNode::update(gkScalar tick)
{
	gkQuaternion rollNode = m_rollNode * gkQuaternion(Ogre::Angle(-GET_SOCKET_VALUE(REL_X)), gkVector3::UNIT_Z);

	gkScalar rollDegrees = rollNode.getRoll().valueDegrees();

	if (rollDegrees >= GET_SOCKET_VALUE(MIN_ROLL) && rollDegrees <= GET_SOCKET_VALUE(MAX_ROLL))
	{
		m_rollNode = rollNode;
	}

	gkQuaternion pitchNode = m_pitchNode * gkQuaternion(Ogre::Angle(-GET_SOCKET_VALUE(REL_Y)), gkVector3::UNIT_X);

	gkScalar pitchDegrees = pitchNode.getPitch().valueDegrees();

	if (pitchDegrees >= GET_SOCKET_VALUE(MIN_PITCH) && pitchDegrees <= GET_SOCKET_VALUE(MAX_PITCH))
	{
		m_pitchNode = pitchNode;
	}

	m_target->setOrientation(m_rollNode * m_pitchNode);

	if (m_center != GET_SOCKET_VALUE(CENTER_POSITION))
	{
		m_oldCenter = m_center;

		m_center = GET_SOCKET_VALUE(CENTER_POSITION);
	}

	gkVector3 currentPosition = m_target->getPosition();

	Ogre::Vector3 dir;

	{
		gkVector3 newZPosition = currentPosition;

		if (GET_SOCKET_VALUE(REL_Z))
		{
			newZPosition.z += newZPosition.z * GET_SOCKET_VALUE(REL_Z) * 0.5;

			m_radiusIdealIsSet = false;
		}

		if (GET_SOCKET_VALUE(KEEP_DISTANCE))
		{
			dir = m_oldCenter - newZPosition;

			m_oldCenter = m_center;
		}
		else
		{
			dir = m_center - newZPosition;
		}
	}

	gkScalar radius = dir.length();

	if (!m_radiusIdealIsSet)
	{
		m_idealRadius = radius;

		m_radiusIdealIsSet = true;
	}

	if (!m_oldRadiusIsSet)
	{
		m_oldRadius = radius;

		m_oldRadiusIsSet = true;
	}

	gkScalar stretch = (radius - m_idealRadius) * GET_SOCKET_VALUE(STIFNESS);
	gkScalar damp = (radius - m_oldRadius) * GET_SOCKET_VALUE(DAMPING);

	radius += -stretch * tick - damp;

	gkScalar minZ = GET_SOCKET_VALUE(MIN_Z);
	gkScalar maxZ = GET_SOCKET_VALUE(MAX_Z);

	if (radius < minZ)
	{
		radius = minZ;
	}
	else if (radius > maxZ)
	{
		radius = maxZ;
	}

	m_oldRadius = radius;

	calculateNewPosition(currentPosition, radius, tick);

	SET_SOCKET_VALUE(CURRENT_ROLL, m_rollNode);
	SET_SOCKET_VALUE(CURRENT_PITCH, m_pitchNode);
}

void gkCameraNode::calculateNewPosition(const gkVector3& currentPosition, gkScalar rayLength, gkScalar tick)
{
	gkVector3 oDir = gkVector3::NEGATIVE_UNIT_Z * rayLength;

	gkVector3 tmpPosition = m_center - m_target->getOrientation() * oDir;

	bool newPosSet = false;

	if (GET_SOCKET_VALUE(AVOID_BLOCKING))
	{
		gkVector3 direction = tmpPosition - m_center;

		Ogre::Ray ray(m_center, direction);

		gkSweptTest::AVOID_LIST avoidList;
		avoidList.insert(m_centerObj->getPhysicsController()->getCollisionObject());

		gkSweptTest sweptTest(avoidList);

		gkScalar blokingRadius = GET_SOCKET_VALUE(BLOCKING_RADIUS);

		if (sweptTest.collides(ray, blokingRadius))
		{
			gkVector3 displacement = (sweptTest.getHitPoint() - currentPosition) * 0.9f;

			m_target->setPosition(currentPosition + (displacement + sweptTest.getSliding()) * tick);

			newPosSet = true;
		}
	}

	if (!newPosSet)
	{
		m_target->setPosition(tmpPosition);
	}
}
