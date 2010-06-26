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

#include "RatLogic.h"
#include "SceneLogic.h"
#include "MomoLogic.h"
#include "Common.h"

namespace
{
	enum STATES
	{
		IDLE,
		WALK,
		RUN,
		DEATH
	};

	namespace velocity
	{
		gkScalar WALK = 0.7f;
		gkScalar RUN = 2.5f;
	}

	namespace animation
	{
		gkString BEING_HIT = "rat_beinghit";
		gkString BLINK = "rat_blink";
		gkString DEATH = "rat_death";
		gkString HIT = "rat_hit";
		gkString IDLE = "rat_idle";
		gkString RUN = "rat_run";
		gkString STOP = "rat_stop";
		gkString WALK = "rat_walk";
	}
}

RatLogic::RatLogic(gkGameObject* obj, SceneLogic* scene, PMOMO momo)
: m_obj(obj),
m_scene(scene),
m_tree(scene->m_tree),
m_momo(momo),
m_characterNode(0),
m_steeringObject(0),
m_steeringCapture(0),
m_steeringFollowing(0)
{
	m_steeringObject = m_steeringCapture = new gkSteeringCapture(
		obj, 
		velocity::RUN, 
		gkVector3(0, 1, 0), 
		gkVector3(0, 0, 1), 
		gkVector3(1, 0, 0), 
		m_momo->m_obj,
		0.5f,
		5
	);

	m_steeringCapture->setMaxForce(10);

	m_steeringFollowing = new gkSteeringPathFollowing(
			obj, 
			velocity::RUN, 
			gkVector3(0, 1, 0), 
			gkVector3(0, 0, 1), 
			gkVector3(1, 0, 0), 
			gkVector3(2, 4, 2), 
			256,
			0.003f
		);

	m_steeringFollowing->setGoalRadius(m_steeringFollowing->radius());
	m_steeringFollowing->setGoalPosition(m_momo->m_obj->getPosition());
	m_steeringFollowing->setMaxForce(10);

	m_characterNode = m_tree->createNode<gkCharacterNode>();
	m_characterNode->setObj(m_obj);
	
	m_characterNode->setForward(m_steeringObject->forward());
	m_characterNode->setListener(this);

	
	gkRayTestNode* hasHit = m_tree->createNode<gkRayTestNode>();
	hasHit->getTARGET()->setValue(m_obj);
	hasHit->getRAY_DIRECTION()->setValue(gkVector3(0, 0.5f, 0));

	// Initial state
	m_characterNode->getCURRENT_STATE()->setValue(IDLE); 

	//IDLE TRANSITION
	m_characterNode->addTransition(DEATH, IDLE, 15000);
	m_characterNode->addTransition(IDLE, IDLE);
	m_characterNode->addTransition(WALK, IDLE);
	m_characterNode->addTransition(RUN, IDLE);

	INT_EQUAL_NODE_TYPE* wantToWalk = INT_EQUAL_NODE(m_characterNode->getAI_STATE(), WALK); 

	// WALK TRANSITION
	IS_TRUE(wantToWalk)->link(m_characterNode->addTransition(IDLE, WALK));
	IS_TRUE(wantToWalk)->link(m_characterNode->addTransition(WALK, WALK));
	IS_TRUE(wantToWalk)->link(m_characterNode->addTransition(RUN, WALK));

	INT_EQUAL_NODE_TYPE* wantToRun = INT_EQUAL_NODE(m_characterNode->getAI_STATE(), RUN); 

	// RUN TRANSITION
	IS_TRUE(wantToRun)->link(m_characterNode->addTransition(IDLE, RUN));
	IS_TRUE(wantToRun)->link(m_characterNode->addTransition(WALK, RUN));
	IS_TRUE(wantToRun)->link(m_characterNode->addTransition(RUN, RUN));

	//DEATH TRANSITION
	{
		PGAMEOBJ_EQUAL_NODE_TYPE* ifNode = PGAMEOBJ_EQUAL_NODE(m_momo->m_kickTestNode->getHIT_OBJ(), m_obj);
		IS_TRUE(ifNode)->link(m_characterNode->addTransition(IDLE, DEATH));
		IS_TRUE(ifNode)->link(m_characterNode->addTransition(RUN, DEATH));
		IS_TRUE(ifNode)->link(m_characterNode->addTransition(WALK, DEATH));
		m_characterNode->getANIM_NOT_HAS_REACHED_END()->link(m_characterNode->addTransition(DEATH, DEATH));
	}

	gkCharacterNode::MAP map;
	typedef gkCharacterNode::StateData StateData;
	map[IDLE] = StateData(IDLE, animation::IDLE, true, 0);
	map[WALK] = StateData(WALK, animation::WALK, true, velocity::WALK);
	map[RUN] = StateData(RUN, animation::RUN, true, velocity::RUN);
	map[DEATH] = StateData(DEATH, animation::DEATH, false, 0);

	m_characterNode->setMapping(map);
}

RatLogic::~RatLogic()
{
	delete m_steeringFollowing;
	delete m_steeringCapture;
}

gkCharacterNode::STATE RatLogic::updateAI(gkCharacterNode* obj, gkScalar tick)
{
	gkCharacterNode::STATE newState = gkCharacterNode::NULL_STATE;

	if(obj->isCurrentStatus(DEATH)->getIS_FALSE()->getValue())
	{
		getBehaviour();

		//gkCharacterNode::STATE oldState = m_steeringObject->getState();

		if(m_steeringObject->update(tick))
		{
			gkScalar speed = m_steeringObject->speed();

			if(speed == 0)
				newState = IDLE;
			else if(speed < velocity::RUN)
				newState = WALK;
			else
				newState = RUN;

			//if(m_steeringObject->getState() != oldState)
			//	gkLogMessage(m_steeringObject->getDebugStringState());
		}
		else 
		{
			m_steeringObject->setGoalPosition(m_momo->m_obj->getPosition());
		}

	}

	return newState;
}

void RatLogic::getBehaviour()
{
	if(abs(m_steeringObject->position().z - m_momo->m_obj->getPosition().z) > m_steeringObject->radius())
	{
		if(m_steeringObject == m_steeringFollowing && m_steeringObject->getState() == gkSteeringObject::STUCK)
		{
			m_steeringObject = m_steeringCapture;
		}
		else if(m_steeringObject->getState() != gkSteeringObject::STUCK && m_steeringObject == m_steeringCapture && m_characterNode->getNOT_FALLING()->getValue() && m_momo->m_characterNode->getNOT_FALLING()->getValue())
		{
			if(gkMath::RangeRandom(0, 10000) < 10)
			{
				m_steeringObject = m_steeringFollowing;
				m_steeringFollowing->reset();
				m_steeringFollowing->setGoalPosition(m_momo->m_obj->getPosition());
			}
		}
	}
	else
	{
		if(m_steeringObject == m_steeringFollowing)
		{
			if(m_steeringFollowing->getState() == gkSteeringObject::IN_GOAL )
			{
				m_steeringObject = m_steeringCapture;
				m_steeringCapture->reset();
			}
		}
		
	}
}
