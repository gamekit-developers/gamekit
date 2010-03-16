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

using namespace Ogre;

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
	ADD_ISOCK(*getUpdate(), this, gkLogicSocket::ST_BOOL);

	ADD_ISOCK(*getCenterPosition(), this, gkLogicSocket::ST_VECTOR);
	ADD_ISOCK(*getCenterObj(), this, gkLogicSocket::ST_GAME_OBJECT);

	ADD_ISOCK(*getRelX(), this, gkLogicSocket::ST_REAL);
	ADD_ISOCK(*getRelY(), this, gkLogicSocket::ST_REAL);
	ADD_ISOCK(*getRelZ(), this, gkLogicSocket::ST_REAL);

	ADD_ISOCK(*getMinPitch(), this, gkLogicSocket::ST_REAL);
	ADD_ISOCK(*getMaxPitch(), this, gkLogicSocket::ST_REAL);
	ADD_ISOCK(*getMinRoll(), this, gkLogicSocket::ST_REAL);
	ADD_ISOCK(*getMaxRoll(), this, gkLogicSocket::ST_REAL);

	ADD_ISOCK(*getKeepDistance(), this, gkLogicSocket::ST_BOOL);
	
	ADD_ISOCK(*getTarget(), this, gkLogicSocket::ST_GAME_OBJECT);

	ADD_OSOCK(*getRoll(), this, gkLogicSocket::ST_QUAT);
	ADD_OSOCK(*getPitch(), this, gkLogicSocket::ST_QUAT);
}

gkArcBallNode::~gkArcBallNode()
{
}

bool gkArcBallNode::evaluate(Real tick)
{
	m_centerObj = getCenterObj()->getValueGameObject();

	if(m_target != getTarget()->getValueGameObject())
	{
		m_target = getTarget()->getValueGameObject();

		m_currentPosition = m_target ? m_target->getPosition() : gkVector3::ZERO;
	}

	bool update = getUpdate()->getValueBool();

	if(!update)
	{
		m_rollNode = gkQuaternion::IDENTITY;
		
		m_pitchNode = gkQuaternion::IDENTITY;
	}

	return m_centerObj && m_target && m_centerObj->isLoaded() && m_target->isLoaded() && update;
}

void gkArcBallNode::update(Real tick)
{
	if(m_center != getCenterPosition()->getValueVector3())
	{
		gkQuaternion q(m_target->getOrientation());

		m_rollNode = gkQuaternion(q.getRoll(), gkVector3::UNIT_Z);
		
		m_pitchNode = gkQuaternion(q.getPitch(), gkVector3::UNIT_X);

		m_oldCenter = m_center;

		m_center = getCenterPosition()->getValueVector3();
	}

	gkQuaternion rollNode = m_rollNode * gkQuaternion(Angle(-getRelX()->getValueReal()), gkVector3::UNIT_Z);

	gkScalar rollDegrees = rollNode.getRoll().valueDegrees();

	if(rollDegrees >= getMinRoll()->getValueReal() && rollDegrees <= getMaxRoll()->getValueReal())
	{
		m_rollNode = rollNode;
	}

	gkQuaternion pitchNode = m_pitchNode * gkQuaternion(Angle(getRelY()->getValueReal()), gkVector3::UNIT_X);

	gkScalar pitchDegrees = pitchNode.getPitch().valueDegrees();

	if(pitchDegrees >= getMinPitch()->getValueReal() && pitchDegrees <= getMaxPitch()->getValueReal())
	{
		m_pitchNode = pitchNode;
	}

	m_target->setOrientation(m_rollNode * m_pitchNode);

	gkVector3 currentPosition = m_currentPosition;

	currentPosition.z += currentPosition.z * getRelZ()->getValueReal() * 0.5;

	Ogre::Vector3 dir;

	if(getKeepDistance()->getValueBool())
	{
		dir = m_oldCenter - currentPosition;
	}
	else
	{
		dir = m_center - currentPosition;
	}

	Vector3 oDir = Ogre::Vector3::NEGATIVE_UNIT_Z * dir.length();

	m_currentPosition = m_center - m_target->getOrientation() * oDir;

	m_target->setPosition(m_currentPosition);
	getRoll()->setValue(m_rollNode);
	getPitch()->setValue(m_pitchNode);
}
