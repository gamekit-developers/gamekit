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
#include "OgreAnimation.h"
#include "OgreEntity.h"
#include "gkCharacterNode.h"
#include "gkGameObject.h"
#include "gkEntity.h"
#include "gkEngine.h"
#include "gkScene.h"
#include "gkDynamicsWorld.h"
#include "gkPhysicsDebug.h"
#include "gkAction.h"
#include "gkLogger.h"
#include "Recast.h"
#include "RecastLog.h"
#include "RecastTimer.h"
#include "DetourNavMeshBuilder.h"
#include "DetourNavMesh.h"


gkCharacterNode::gkCharacterNode(gkLogicTree *parent, size_t id) 
: gkStateMachineNode(parent, id),
m_obj(0),
m_ent(0),
m_dir(0, 1, 0),
m_up(0, 0, 1),
m_upMask(1, 1, 0),
m_polyPickExt(2, 4, 2),
m_maxPathPolys(256),
m_foundThreshold(1.3f),
m_currentStateData(0),
m_scene(0),
m_debug(0),
m_ai_wanted_state(-1),
m_ai_wanted_velocity(0),
m_ai_wanted_rotation(gkQuaternion::IDENTITY)
{
	ADD_ISOCK(ANIM_BLEND_FRAMES, 10);
	ADD_ISOCK(ENABLE_GOTO, false);
	ADD_ISOCK(GOTO_POSITION, gkVector3::ZERO);
	ADD_ISOCK(REDO_PATH, false);
	ADD_ISOCK(ENABLE_ROTATION, false);
	ADD_ISOCK(ROTATION_VALUE, gkQuaternion::IDENTITY);

	
	ADD_OSOCK(ANIM_HAS_REACHED_END, false);
	ADD_OSOCK(ANIM_NOT_HAS_REACHED_END, true);
	ADD_OSOCK(ANIM_TIME_POSITION, 0);
	ADD_OSOCK(POSITION, gkVector3::ZERO);
	ADD_OSOCK(ROTATION, gkQuaternion::IDENTITY);
	ADD_OSOCK(AI_WANTED_STATE, -1);
}

gkCharacterNode::~gkCharacterNode()
{
}

bool gkCharacterNode::evaluate(gkScalar tick)
{
	GK_ASSERT(m_obj);

	gkGameObject* childEntity = m_obj->getChildEntity();

	GK_ASSERT(childEntity);

	m_ent = childEntity->getEntity();

	GK_ASSERT(m_ent);

	return m_ent->isLoaded() && gkStateMachineNode::evaluate(tick);
}

void gkCharacterNode::update(gkScalar tick)
{
	if(!m_scene)
	{
		m_scene = gkEngine::getSingleton().getActiveScene();
	}

	showPath();

	STATE oldState = m_currentStateData->m_state;

	update_state(tick);

	update_animation(oldState);
}

void gkCharacterNode::update_state(gkScalar tick)
{
	m_ai_wanted_state = -1;

	if(GET_SOCKET_VALUE(ENABLE_GOTO))
	{
		goTo(tick);
	}

	SET_SOCKET_VALUE(AI_WANTED_STATE, m_ai_wanted_state);
	
	gkStateMachineNode::update(tick);

	if(m_currentStateData->m_state == m_ai_wanted_state)
	{
		m_obj->rotate(m_ai_wanted_rotation, TRANSFORM_LOCAL);

		m_obj->setLinearVelocity(m_dir * m_ai_wanted_velocity, TRANSFORM_LOCAL);
	}
	else
	{
		if(GET_SOCKET_VALUE(ENABLE_ROTATION) && m_currentStateData->m_allow_rotation)
		{
			m_obj->setOrientation(GET_SOCKET_VALUE(ROTATION_VALUE));
		}

		m_obj->setLinearVelocity(m_dir * m_currentStateData->m_velocity, TRANSFORM_LOCAL);
	}

	SET_SOCKET_VALUE(POSITION, m_obj->getPosition());
	SET_SOCKET_VALUE(ROTATION, m_obj->getOrientation());
}

void gkCharacterNode::update_animation(STATE oldState)
{
	if(m_currentStateData->m_state != oldState)
	{
		SET_SOCKET_VALUE(ANIM_HAS_REACHED_END, false);
		SET_SOCKET_VALUE(ANIM_NOT_HAS_REACHED_END, true);
		SET_SOCKET_VALUE(ANIM_TIME_POSITION, 0);
		gkLogMessage(m_currentStateData->m_animName);
	}

	if(!GET_SOCKET_VALUE(ANIM_HAS_REACHED_END))
	{
		m_ent->playAction(m_currentStateData->m_animName, GET_SOCKET_VALUE(ANIM_BLEND_FRAMES));

		gkAction* pAct = m_ent->getActiveAction();

		GK_ASSERT(pAct);
		
		gkScalar time = pAct->getTimePosition();

		SET_SOCKET_VALUE(ANIM_TIME_POSITION, time);

		if(time >= pAct->getEnd())
		{
			SET_SOCKET_VALUE(ANIM_HAS_REACHED_END, true);
			SET_SOCKET_VALUE(ANIM_NOT_HAS_REACHED_END, false);
		}
	}
	else if(m_currentStateData->m_loop)
	{
		m_ent->playAction(m_currentStateData->m_animName, GET_SOCKET_VALUE(ANIM_BLEND_FRAMES));
	}
}

void gkCharacterNode::notifyState(int state)
{
	m_currentStateData = getStateData(state);
}

gkCharacterNode::StateData* gkCharacterNode::getStateData(int state)
{
	MAP::iterator it = m_map.find(state);

	GK_ASSERT(it != m_map.end());

	return &(it->second);
}

void gkCharacterNode::setMapping(const MAP& map)
{
	GK_ASSERT(!map.empty());

	m_map = map;

	MAP::const_iterator it = m_map.begin();
	while(it != m_map.end())
	{
		m_statesData.push_back(it->second);
		++it;
	}

	std::sort(m_statesData.begin(), m_statesData.end());
}

void gkCharacterNode::goTo(gkScalar tick)
{
	m_goToPosition = GET_SOCKET_VALUE(GOTO_POSITION);

	if(m_path.empty() || GET_SOCKET_VALUE(REDO_PATH))
	{
		findPath();
	}

	followPath(tick);
}

void gkCharacterNode::findPath()
{
	m_navMesh = m_scene->getNavigationMesh();
	
	if(!m_navMesh.get()) return;
	
	m_path.clear();

	gkVector3 startPos = m_obj->getPosition();

	gkVector3 endPos = m_goToPosition;
	
	std::swap(startPos.y, startPos.z);
	std::swap(endPos.y, endPos.z);

	//rcTimeVal totStartTime = rcGetPerformanceTimer();

	dtQueryFilter filter;

	dtPolyRef startRef = m_navMesh->data->findNearestPoly(startPos.ptr(), m_polyPickExt.ptr(), &filter, 0);

	dtPolyRef endRef = m_navMesh->data->findNearestPoly(endPos.ptr(), m_polyPickExt.ptr(), &filter, 0);

	if(startRef && endRef)
	{
		utArray<dtPolyRef> polys;
		polys.resize(m_maxPathPolys);

		int npolys = m_navMesh->data->findPath(startRef, endRef, startPos.ptr(), endPos.ptr(), &filter, polys.ptr(), m_maxPathPolys);

		if(npolys)
		{
			utArray<gkScalar> straightPath;
			straightPath.resize(m_maxPathPolys*3);

			int nstraightPath = m_navMesh->data->findStraightPath(startPos.ptr(), endPos.ptr(), polys.ptr(), npolys, straightPath.ptr(), 0, 0, m_maxPathPolys);

			//rcTimeVal totEndTime = rcGetPerformanceTimer();

			//gkPrintf("FINDPATH TOTAL: %.1fms", rcGetDeltaTimeUsec(totStartTime, totEndTime)/1000.0f);

			std::swap(startPos.y, startPos.z);
			std::swap(endPos.y, endPos.z);
			
			gkVector3 point;

			for(int i=0; i<nstraightPath*3; i+=3)
			{
				point.x = straightPath[i];
				point.y = straightPath[i+2];
				point.z = straightPath[i+1];

				m_path.push_back(point);
			}

			m_path.push_back(endPos);
		}
	}
}

void gkCharacterNode::followPath(gkScalar tick)
{
	if(m_path.empty()) return;
	
	gkVector3 current_pos = m_obj->getPosition();

	current_pos *= m_upMask;

	gkVector3 pos = m_path.front();

	pos *= m_upMask;

	gkVector3 dir = (pos - current_pos);

	gkScalar d = dir.length();

	if(d > m_foundThreshold)
	{
		update_ai_data(dir, d, tick);
	}
	else
	{
		m_path.pop_front();

		if(!m_path.empty())
		{
			followPath(tick);
		}
	}
}

void gkCharacterNode::update_ai_data(const gkVector3& dir, gkScalar d, gkScalar tick)
{
	GK_ASSERT(d > 0);

	gkVector3 current_dir = m_obj->getOrientation() * m_dir;

	gkRadian angle(GetRotationAngleForAxis(current_dir, dir, m_up));

	m_ai_wanted_rotation.FromAngleAxis(angle, m_up);

	STATES_DATA::const_iterator it = std::upper_bound(m_statesData.begin(), m_statesData.end(), StateData(-1, "", false, d));

	GK_ASSERT(it != m_statesData.begin());

	m_ai_wanted_state = -1;

	if(it == m_statesData.end())
	{
		--it;
		m_ai_wanted_state = it->m_state;
		d = it->m_velocity;
	}
	else
	{
		m_ai_wanted_state = it->m_state;
	}

	m_ai_wanted_velocity = d;
}

gkRadian gkCharacterNode::GetRotationAngleForAxis(const gkVector3& from, const gkVector3& to, const gkVector3& axis)
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

gkVector3 gkCharacterNode::GetProjectionOnPlane(const gkVector3& V, const gkVector3& N)
{
	//U = V - (V dot N)N 

	return  V - V.dotProduct(N) * N;
}

void gkCharacterNode::showPath()
{
	m_debug = m_scene->getDynamicsWorld()->getDebug();

	if(m_debug)
	{
		unsigned int n = m_path.size();

		if(n)
		{
			static const btVector3 RED_COLOR(1,0,0);

			//gkVector3 offset = GET_SOCKET_VALUE(SHOW_PATH_OFFSET);

			gkVector3 oldPoint = m_obj->getPosition();// + offset;

			for(unsigned int i=0; i<n; i++)
			{
				gkVector3 point = m_path.at(i);// + offset;

				m_debug->drawLine(
					btVector3(oldPoint.x, oldPoint.y, oldPoint.z), 
					btVector3(point.x, point.y, point.z), 
					RED_COLOR
				);

				oldPoint = m_path.at(i);// + offset;
			}
		}
	}
}

