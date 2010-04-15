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
#include "gkFollowPathNode.h"
#include "gkLogger.h"
#include "gkGameObject.h"

gkFollowPathNode::gkFollowPathNode(gkLogicTree *parent, size_t id) 
: gkLogicNode(parent, id),
m_path(0),
m_target(0),
m_dir(gkVector3::ZERO),
m_up(gkVector3::ZERO),
m_upMask(gkVector3::ZERO),
m_foundThreshold(0),
m_idleState(-1),
m_walkState(-1),
m_runState(-1),
m_walkVelocity(0),
m_runVelocity(0)
{
	ADD_ISOCK(UPDATE, false);
	ADD_ISOCK(TARGET, 0);
	ADD_ISOCK(TARGET_UP_DIRECTION, gkVector3::UNIT_Z);
	ADD_ISOCK(ORIGINAL_TARGET_DIRECTION, gkVector3::UNIT_Y);
	ADD_ISOCK(FOUND_THRESHOLD, 0.8f);
	ADD_ISOCK(PATH, 0);
	ADD_OSOCK(HAS_REACHED_END, true);
	ADD_OSOCK(NOT_HAS_REACHED_END, false);
	ADD_OSOCK(CURRENT_STATE, -1);
}

bool gkFollowPathNode::evaluate(gkScalar tick)
{
	m_path = GET_SOCKET_VALUE(PATH);

	if(m_path && GET_SOCKET_VALUE(UPDATE) && GET_SOCKET_VALUE(TARGET) && animationHasBeenSet())
	{
		if(!m_path->following && !m_path->path.empty())
		{
			m_target = GET_SOCKET_VALUE(TARGET);
			m_dir = GET_SOCKET_VALUE(ORIGINAL_TARGET_DIRECTION);
			m_up = GET_SOCKET_VALUE(TARGET_UP_DIRECTION);
			m_upMask = (gkVector3::UNIT_SCALE - m_up);
			m_foundThreshold = GET_SOCKET_VALUE(FOUND_THRESHOLD);

			SET_SOCKET_VALUE(HAS_REACHED_END, false);
			SET_SOCKET_VALUE(NOT_HAS_REACHED_END, true);
			SET_SOCKET_VALUE(CURRENT_STATE, -1);

			m_path->following = true;
		}
	}

	return m_path && m_path->following;
}

void gkFollowPathNode::update(gkScalar tick)
{
	gkVector3 current_pos = m_target->getPosition();

	current_pos *= m_upMask;

	gkVector3 pos = m_path->path.front();

	pos *= m_upMask;

	gkVector3 dir = (pos - current_pos);

	gkScalar d = dir.length();

	if(d > m_foundThreshold)
	{
		gkVector3 current_dir = m_target->getOrientation() * m_dir;

		gkQuaternion q;

		gkRadian angle = GetRotationAngleForAxis(current_dir, dir, m_up);

		q.FromAngleAxis(angle, m_up);

		m_target->rotate(q, TRANSFORM_LOCAL);

		setVelocity(d, tick);
	}
	else
	{
		m_path->path.pop_front();

		if(m_path->path.empty())
		{
			m_path->following = false;

			if(!m_path->retry)
			{
				m_path->retry = 5;
			}
			else
			{
				--m_path->retry;
			}

			SET_SOCKET_VALUE(HAS_REACHED_END, true);
			SET_SOCKET_VALUE(NOT_HAS_REACHED_END, false);
			SET_SOCKET_VALUE(CURRENT_STATE, m_idleState);
		}
	}
}

void gkFollowPathNode::setVelocity(gkScalar d, gkScalar tick)
{
	if(d >= m_runVelocity*tick)
	{
		SET_SOCKET_VALUE(CURRENT_STATE, m_runState);
		m_target->setLinearVelocity(m_dir * m_runVelocity);
	}
	else if(d >= m_walkVelocity*tick)
	{
		SET_SOCKET_VALUE(CURRENT_STATE, m_walkState);
		m_target->setLinearVelocity(m_dir * m_walkVelocity);
	}
	else
	{
		SET_SOCKET_VALUE(CURRENT_STATE, m_walkState);
		m_target->setLinearVelocity(m_dir * d/tick);
	}
}

gkRadian gkFollowPathNode::GetRotationAngleForAxis(const gkVector3& from, const gkVector3& to, const gkVector3& axis)
{
	Ogre::Vector3 from1 = GetProjectionOnPlane(from, axis);

	Ogre::Vector3 to1 = GetProjectionOnPlane(to, axis);

	Ogre::Quaternion q = from1.getRotationTo(to1);

	Ogre::Radian angle;
	Ogre::Vector3 rAxis;
	
	q.ToAngleAxis(angle, rAxis);

	rAxis = angle.valueRadians() * (rAxis * axis);

	angle = rAxis.x + rAxis.y + rAxis.z;

	return angle;
}

gkVector3 gkFollowPathNode::GetProjectionOnPlane(const gkVector3& V, const gkVector3& N)
{
	//U = V - (V dot N)N 

	return  V - V.dotProduct(N) * N;
}




