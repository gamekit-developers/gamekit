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
#include "gkLogger.h"
#include "gkGameObject.h"
#include "gkFollowPathNode.h"
#include "gkEngine.h"
#include "gkScene.h"
#include "gkDynamicsWorld.h"
#include "gkPhysicsDebug.h"

gkFollowPathNode::gkFollowPathNode(gkLogicTree *parent, size_t id) 
: gkLogicNode(parent, id),
m_path(0),
m_target(0),
m_dir(gkVector3::ZERO),
m_up(gkVector3::ZERO),
m_upMask(gkVector3::ZERO),
m_foundThreshold(0),
m_walkVelocity(0),
m_runVelocity(0)
{
	ADD_ISOCK(UPDATE, false);
	ADD_ISOCK(TARGET, 0);
	ADD_ISOCK(SOURCE, 0);
	ADD_ISOCK(TARGET_UP_DIRECTION, gkVector3::UNIT_Z);
	ADD_ISOCK(ORIGINAL_TARGET_DIRECTION, gkVector3::UNIT_Y);
	ADD_ISOCK(FOUND_THRESHOLD, 0.8f);
	ADD_ISOCK(PATH, 0);
	ADD_ISOCK(SHOW_PATH_OFFSET, gkVector3::ZERO);
	ADD_OSOCK(HAS_REACHED_END, true);
	ADD_OSOCK(NOT_HAS_REACHED_END, false);
	ADD_OSOCK(IDLE, false);
	ADD_OSOCK(WALK, false);
	ADD_OSOCK(RUN, false);
}

bool gkFollowPathNode::evaluate(gkScalar tick)
{
	m_path = GET_SOCKET_VALUE(PATH);
	m_target = GET_SOCKET_VALUE(TARGET);

	SET_SOCKET_VALUE(IDLE, false);
	SET_SOCKET_VALUE(WALK, false);
	SET_SOCKET_VALUE(RUN, false);

	if(GET_SOCKET_VALUE(UPDATE))
	{
		if(m_path && animationHasBeenSet())
		{
			if(!m_path->following && !m_path->path.empty())
			{
				m_dir = GET_SOCKET_VALUE(ORIGINAL_TARGET_DIRECTION);
				m_up = GET_SOCKET_VALUE(TARGET_UP_DIRECTION);
				m_upMask = (gkVector3::UNIT_SCALE - m_up);
				m_foundThreshold = GET_SOCKET_VALUE(FOUND_THRESHOLD);

				SET_SOCKET_VALUE(HAS_REACHED_END, false);
				SET_SOCKET_VALUE(NOT_HAS_REACHED_END, true);

				m_path->following = true;
			}
		}

		return m_target && m_target->isLoaded() && m_path && m_path->following;
	}

	return false;
}

void gkFollowPathNode::update(gkScalar tick)
{
	gkVector3 current_pos = m_target->getPosition();

	current_pos *= m_upMask;

	gkVector3 pos = m_path->path.front();

	pos *= m_upMask;

	gkVector3 dir = (pos - current_pos);

	gkScalar d = dir.length();

	if(d > 0.1f && !isTargetReached())
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

			gkGameObject* pSource = GET_SOCKET_VALUE(SOURCE);

			if(!m_path->retry && pSource && pSource->isLoaded())
			{
				m_path->retry = !isTargetReached();
			}
			else
			{
				m_path->retry = false;
			}

			SET_SOCKET_VALUE(HAS_REACHED_END, true);
			SET_SOCKET_VALUE(NOT_HAS_REACHED_END, false);
			SET_SOCKET_VALUE(IDLE, true);
		}
		else
		{
			showPath();
		}
	}
}

bool gkFollowPathNode::isTargetReached() 
{
	gkGameObject* pSource = GET_SOCKET_VALUE(SOURCE);

	bool targetReached = false;

	if(pSource)
	{
		const gkVector3& current_pos = m_target->getPosition();

		const gkVector3& sourcePos = pSource->getPosition();

		targetReached = current_pos.distance(sourcePos) < m_foundThreshold;
	}

	return targetReached;
}

void gkFollowPathNode::setVelocity(gkScalar d, gkScalar tick)
{
	if(d >= m_runVelocity*tick)
	{
		SET_SOCKET_VALUE(RUN, true);
		m_target->setLinearVelocity(m_dir * m_runVelocity, TRANSFORM_LOCAL);
	}
	else if(d >= m_walkVelocity*tick)
	{
		SET_SOCKET_VALUE(WALK, true);
		m_target->setLinearVelocity(m_dir * m_walkVelocity, TRANSFORM_LOCAL);
	}
	else
	{
		SET_SOCKET_VALUE(WALK, true);
		m_target->setLinearVelocity(m_dir * d/tick, TRANSFORM_LOCAL);
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

void gkFollowPathNode::showPath()
{
	gkScene* pScene = gkEngine::getSingleton().getActiveScene();
	
	gkPhysicsDebug *debug = pScene->getDynamicsWorld()->getDebug();
	
	if(debug)
	{
		unsigned int n = m_path ? m_path->path.size() : 0;

		if(n)
		{
			static const btVector3 RED_COLOR(1,0,0);

			gkVector3 offset = GET_SOCKET_VALUE(SHOW_PATH_OFFSET);

			gkVector3 oldPoint = m_path->path.at(0) + offset;

			for(unsigned int i=1; i<n; i++)
			{
				gkVector3 point = m_path->path.at(i) + offset;

				debug->drawLine(
					btVector3(oldPoint.x, oldPoint.y, oldPoint.z), 
					btVector3(point.x, point.y, point.z), 
					RED_COLOR
				);

				oldPoint = m_path->path.at(i) + offset;
			}
		}
	}
}




