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
#include "gkLogger.h"
#include "gkSteering.h"
#include "gkRayTest.h"
//#include "gkSteeringCapture.h"
//#include "gkSteeringPathFollowing.h"
#include "gkCharacter.h"
#include "gkRigidBody.h"
#include "btBulletDynamicsCommon.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"

gkCharacterNode::gkCharacterNode(gkLogicTree* parent, size_t id)
	: gkStateMachineNode(parent, id),
	  m_obj(0),
	  m_ent(0),
	  m_currentStateData(0),
	  m_scene(0),
	  m_forward(gkVector3::ZERO),
	  m_falling(false)
{
	ADD_ISOCK(ANIM_BLEND_FRAMES, 10);
	ADD_ISOCK(ENABLE_ROTATION, false);
	ADD_ISOCK(ROTATION_VALUE, gkQuaternion::IDENTITY);
	ADD_ISOCK(INPUT_AI_STATE, NULL_STATE);
	ADD_ISOCK(JUMP, false);
	ADD_ISOCK(GRAVITY, 9.81f);
	ADD_ISOCK(JUMP_SPEED, 10);

	ADD_OSOCK(ANIM_HAS_REACHED_END, false);
	ADD_OSOCK(ANIM_NOT_HAS_REACHED_END, true);
	ADD_OSOCK(ANIM_TIME_POSITION, 0);
	ADD_OSOCK(POSITION, gkVector3::ZERO);
	ADD_OSOCK(ROTATION, gkQuaternion::IDENTITY);
	ADD_OSOCK(FALLING, false);
	ADD_OSOCK(NOT_FALLING, true);
	ADD_OSOCK(OUTPUT_AI_STATE, NULL_STATE);
}

gkCharacterNode::~gkCharacterNode()
{
}

void gkCharacterNode::initialize()
{
	GK_ASSERT(m_forward != gkVector3::ZERO);

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
	return m_ent->isInstanced() && gkStateMachineNode::evaluate(tick);
}

void gkCharacterNode::update(gkScalar tick)
{
	STATE previousTickState = m_currentStateData->m_state;

	update_state(tick);

	update_animation(previousTickState);
}

gkCharacterNode::STATE gkCharacterNode::getState(int previousIdx) const
{
	const StateData* state = m_currentStateData;

	while (previousIdx-- && state)
		state = state->m_previous;

	if (state)
	{
		return state->m_state;
	}

	return NULL_STATE;
}

void gkCharacterNode::update_state(gkScalar tick)
{
	STATE aiState = GET_SOCKET_VALUE(INPUT_AI_STATE);

	SET_SOCKET_VALUE(OUTPUT_AI_STATE, aiState);

	gkStateMachineNode::update(tick);

	if (aiState == NULL_STATE)
	{
		if (GET_SOCKET_VALUE(ENABLE_ROTATION) && m_currentStateData->m_allow_rotation)
		{
			m_obj->setOrientation(GET_SOCKET_VALUE(ROTATION_VALUE));
		}

		if (GET_SOCKET_VALUE(JUMP))
		{
			gkScalar jumpSpeed = GET_SOCKET_VALUE(JUMP_SPEED);

			const gkGameObjectProperties& props = m_obj->getProperties();
			const gkPhysicsProperties& phy = props.m_physics;

			if (m_obj->getAttachedCharacter())
			{
				m_obj->getAttachedCharacter()->getCharacterController()->setJumpSpeed(phy.m_mass * jumpSpeed);
				m_obj->getAttachedCharacter()->getCharacterController()->jump();
			}
			else
			{
				m_obj->applyForce(gkVector3::UNIT_Z * phy.m_mass * jumpSpeed);
			}
		}

		StateData* state = m_currentStateData;

		while (state->m_usePreviousVelocity && state->m_previous)
			state = state->m_previous;

		m_obj->setLinearVelocity(m_forward * state->m_velocity, TRANSFORM_LOCAL);
	}

	SET_SOCKET_VALUE(POSITION, m_obj->getPosition());
	SET_SOCKET_VALUE(ROTATION, m_obj->getOrientation());

	{
		// FALL TEST

		m_falling = false;

		if (m_obj->getAttachedCharacter())
		{
			m_falling = !m_obj->getAttachedCharacter()->getCharacterController()->onGround();
		}
		else
		{
			const gkVector3& velocity = m_obj->getLinearVelocity();

			m_falling = velocity.z < 0;
		}

		SET_SOCKET_VALUE(FALLING, m_falling);
		SET_SOCKET_VALUE(NOT_FALLING, !m_falling);
	}


	{
		// GRAVITY

		gkScalar gravity = GET_SOCKET_VALUE(GRAVITY);

		if (m_obj->getAttachedCharacter())
		{
			m_obj->getAttachedCharacter()->getCharacterController()->setGravity(gravity);
		}
		else
		{
			m_obj->getAttachedBody()->getBody()->setGravity(btVector3(0, 0, -gravity));
		}
	}
}

void gkCharacterNode::update_animation(STATE previousTickState)
{
	if (m_currentStateData->m_state != previousTickState)
	{
		SET_SOCKET_VALUE(ANIM_HAS_REACHED_END, false);
		SET_SOCKET_VALUE(ANIM_NOT_HAS_REACHED_END, true);
		SET_SOCKET_VALUE(ANIM_TIME_POSITION, 0);
		//gkLogMessage(m_currentStateData->m_state << ":" << m_currentStateData->m_animName);
	}

	if (!GET_SOCKET_VALUE(ANIM_HAS_REACHED_END))
	{
		m_ent->playAnimation(m_currentStateData->m_animName, GET_SOCKET_VALUE(ANIM_BLEND_FRAMES));

		gkAnimationPlayer* pAct = m_ent->getAnimationPlayer(m_currentStateData->m_animName);

		GK_ASSERT(pAct);

		gkScalar time = pAct->getTimePosition();

		SET_SOCKET_VALUE(ANIM_TIME_POSITION, time);

		if (time >= pAct->getLength())
		{
			SET_SOCKET_VALUE(ANIM_HAS_REACHED_END, true);
			SET_SOCKET_VALUE(ANIM_NOT_HAS_REACHED_END, false);
		}
	}
	else if (m_currentStateData->m_loop)
	{
		m_ent->playAnimation(m_currentStateData->m_animName, GET_SOCKET_VALUE(ANIM_BLEND_FRAMES));
	}
}

void gkCharacterNode::notifyState(int state)
{
	StateData* previous = m_currentStateData;

	m_currentStateData = getStateData(state);

	m_currentStateData->m_previous = previous;

	GK_ASSERT(m_currentStateData->m_previous != m_currentStateData);
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
}
