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

#include "gkArcBallNode.h"
#include "gkEngine.h"
#include "gkGameObject.h"
#include "gkUtils.h"
#include "gkLogger.h"

#include <limits>

gkArcBallNode::gkArcBallNode(gkLogicTree *parent, size_t id)
: gkLogicNode(parent, id),
m_currentPosition(gkVector3::ZERO),
m_center(gkVector3::ZERO),
m_oldCenter(gkVector3::ZERO),
m_target(0),
m_centerObj(0),
m_rollNode(gkQuaternion::IDENTITY),
m_pitchNode(gkQuaternion::IDENTITY)
{
	ADD_ISOCK(UPDATE, false);
	ADD_ISOCK(CENTER_OBJ, 0);
	ADD_ISOCK(CENTER_POSITION, gkVector3::ZERO);
	ADD_ISOCK(RELX, 0);
	ADD_ISOCK(RELY, 0);
	ADD_ISOCK(RELZ, 0);
	ADD_ISOCK(TARGET, 0);
	ADD_ISOCK(MIN_PITCH, 0);
	ADD_ISOCK(MAX_PITCH, 0);
	ADD_ISOCK(MIN_ROLL, 0);
	ADD_ISOCK(MAX_ROLL, 0);
	ADD_ISOCK(KEEP_DISTANCE, false);
	ADD_OSOCK(CURRENT_ROLL, gkQuaternion::IDENTITY);
	ADD_OSOCK(CURRENT_PITCH, gkQuaternion::IDENTITY)
	ADD_ISOCK(MIN_Z, 0);
	ADD_ISOCK(MAX_Z, std::numeric_limits<gkScalar>::infinity());
}

gkArcBallNode::~gkArcBallNode()
{
}

bool gkArcBallNode::evaluate(gkScalar tick)
{
	m_centerObj = GET_SOCKET_VALUE(CENTER_OBJ);

	if(m_target != GET_SOCKET_VALUE(TARGET))
	{
		m_target = GET_SOCKET_VALUE(TARGET);

		m_currentPosition = m_target ? m_target->getPosition() : gkVector3::ZERO;

		gkQuaternion q(m_target->getOrientation());

		m_rollNode = gkQuaternion(q.getRoll(), gkVector3::UNIT_Z);
		
		m_pitchNode = gkQuaternion(q.getPitch(), gkVector3::UNIT_X);

		m_oldCenter = m_center = GET_SOCKET_VALUE(CENTER_POSITION);
	}

	bool update = GET_SOCKET_VALUE(UPDATE);

	return m_centerObj && m_target && m_centerObj->isLoaded() && m_target->isLoaded() && update;
}

void gkArcBallNode::update(gkScalar tick)
{
	gkQuaternion rollNode = m_rollNode * gkQuaternion(Ogre::Angle(-GET_SOCKET_VALUE(RELX)), gkVector3::UNIT_Z);

	gkScalar rollDegrees = rollNode.getRoll().valueDegrees();

	if(rollDegrees >= GET_SOCKET_VALUE(MIN_ROLL) && rollDegrees <= GET_SOCKET_VALUE(MAX_ROLL))
	{
		m_rollNode = rollNode;
	}

	gkQuaternion pitchNode = m_pitchNode * gkQuaternion(Ogre::Angle(GET_SOCKET_VALUE(RELY)), gkVector3::UNIT_X);

	gkScalar pitchDegrees = pitchNode.getPitch().valueDegrees();

	if(pitchDegrees >= GET_SOCKET_VALUE(MIN_PITCH) && pitchDegrees <= GET_SOCKET_VALUE(MAX_PITCH))
	{
		m_pitchNode = pitchNode;
	}

	m_target->setOrientation(m_rollNode * m_pitchNode);

	gkVector3 currentPosition = m_currentPosition;

	currentPosition.z += currentPosition.z * GET_SOCKET_VALUE(RELZ) * 0.5;

	if(m_center != GET_SOCKET_VALUE(CENTER_POSITION))
	{
		m_oldCenter = m_center;

		m_center = GET_SOCKET_VALUE(CENTER_POSITION);
	}

	Ogre::Vector3 dir;

	if(GET_SOCKET_VALUE(KEEP_DISTANCE))
	{
		dir = m_oldCenter - currentPosition;
		
		m_oldCenter = m_center;
	}
	else
	{
		dir = m_center - currentPosition;
	}

	gkScalar radius = dir.length();
	gkScalar minZ = GET_SOCKET_VALUE(MIN_Z);
	gkScalar maxZ = GET_SOCKET_VALUE(MAX_Z);

	if(radius >= minZ && radius <= maxZ)
	{
		gkVector3 oDir = Ogre::Vector3::NEGATIVE_UNIT_Z * radius;

		m_currentPosition = m_center - m_target->getOrientation() * oDir;

		m_target->setPosition(m_currentPosition);
	}

	SET_SOCKET_VALUE(CURRENT_ROLL, m_rollNode);
	SET_SOCKET_VALUE(CURRENT_PITCH, m_pitchNode);
}
