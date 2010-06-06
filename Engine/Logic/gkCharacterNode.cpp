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
#include "Ogre.h"
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
#include "gkSteering.h"
#include "gkRayTest.h"

gkCharacterNode::gkCharacterNode(gkLogicTree *parent, size_t id) 
: gkStateMachineNode(parent, id),
m_obj(0),
m_ent(0),
m_dir(0, 1, 0),
m_up(0, 0, 1),
m_polyPickExt(2, 4, 2),
m_maxPathPolys(256),
m_currentStateData(0),
m_scene(0),
m_createdNavMesh(false)
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
	if(m_createdNavMesh)
	{
		delete gkNavMeshData::getSingletonPtr();
	}
}

void gkCharacterNode::initialize()
{
	if(!gkNavMeshData::getSingletonPtr())
	{
		new gkNavMeshData(gkEngine::getSingleton().getActiveScene());
		
		m_createdNavMesh = true;

		gkNavMeshData::getSingletonPtr()->loadAll();
	}
		
	GK_ASSERT(m_obj);
		
	gkGameObject* childEntity = m_obj->getChildEntity();
		
	GK_ASSERT(childEntity);
		
	m_ent = childEntity->getEntity();
		
	GK_ASSERT(m_ent);
		
	m_scene = gkEngine::getSingleton().getActiveScene();
		
	GK_ASSERT(m_scene);
}

bool gkCharacterNode::evaluate(gkScalar tick)
{
	return m_ent->isLoaded() && gkStateMachineNode::evaluate(tick);
}

void gkCharacterNode::update(gkScalar tick)
{
	m_navPath.showPath(m_scene, m_obj->getPosition());

	STATE oldState = m_currentStateData->m_state;

	update_state(tick);

	update_animation(oldState);
}

void gkCharacterNode::update_state(gkScalar tick)
{
	if(GET_SOCKET_VALUE(ENABLE_GOTO) && gkNavMeshData::getSingletonPtr() && (m_navPath.empty() || GET_SOCKET_VALUE(REDO_PATH)))
	{
		m_navPath.create(gkNavMeshData::getSingleton(), m_obj->getPosition(), GET_SOCKET_VALUE(GOTO_POSITION), m_polyPickExt, m_maxPathPolys); 
	}

	gkVector3 dir;
	
	gkVector3 current_position = m_obj->getPosition();
	
	gkScalar d = m_navPath.getDirection(current_position, 0.7f, dir); 

	if(d)
	{
		gkVector3 current_dir = m_obj->getOrientation() * m_dir;
		
		//Ogre::Ray ray(current_position, current_dir*0.5f);
		
		//gkRayTest rayTest;
		
		//if(!rayTest.collides(ray))
		{
		//	current_dir.x+=0.8f; 
		}
			gkSteering steer(m_up);

			m_obj->rotate(steer.getRotation(current_dir, dir), TRANSFORM_LOCAL);
		
			STATE newState = -1;

			m_obj->setLinearVelocity(m_dir * getVelocityForDistance(d, tick, newState), TRANSFORM_LOCAL);

			GK_ASSERT(newState != -1);

			SET_SOCKET_VALUE(AI_WANTED_STATE, newState);
		
			gkStateMachineNode::update(tick);
		//}
		//else
	//	{
			//m_navPath.clear();
	//	}

	}
	else if(m_navPath.empty())
	{
		SET_SOCKET_VALUE(AI_WANTED_STATE, -1);
		
		gkStateMachineNode::update(tick);

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
		//gkLogMessage(m_currentStateData->m_animName);
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

gkScalar gkCharacterNode::getVelocityForDistance(gkScalar d, gkScalar tick, STATE& state) const
{
	GK_ASSERT(d > 0);

	STATES_DATA::const_iterator it = std::upper_bound(m_statesData.begin(), m_statesData.end(), StateData(-1, "", false, d));

	GK_ASSERT(it != m_statesData.begin());

	--it;
	
	if(it->m_velocity == 0)
	{
		++it;

		state = it->m_state;

		return d/tick;
	}

	state = it->m_state;

	return it->m_velocity;
}
