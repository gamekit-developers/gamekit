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
const gkVector3 FORWARD(0, 1, 0);
const gkVector3 UP(0, 0, 1);
const gkVector3 SIDE(1, 0, 0);

namespace velocity
{
gkScalar NONE = 0;
gkScalar WALK = 0.7f;
gkScalar RUN = 2.5f;
}

namespace animation
{
enum STATES
{
	IDLE,
	WALK,
	RUN,
	DEATH,
	STOP
};

gkString BEING_HIT_STR = "rat_beinghit";
gkString BLINK_STR = "rat_blink";
gkString DEATH_STR = "rat_death";
gkString HIT_STR = "rat_hit";
gkString IDLE_STR = "rat_idle";
gkString RUN_STR = "rat_run";
gkString STOP_STR = "rat_stop";
gkString WALK_STR = "rat_walk";
}

namespace logic
{
enum STATES
{
	CAPTURE,
	FOLLOWING_PATH,
	WANDER,
	STUCK,
};
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
	  m_steeringFollowing(0),
	  m_steeringWander(0)
{
	m_steeringObject = m_steeringCapture = new gkSteeringCapture(
	    obj,
	    velocity::RUN,
	    FORWARD,
	    UP,
	    SIDE,
	    m_momo->m_obj,
	    0.5f,
	    5
	);

	m_steeringCapture->setMaxForce(10);

	m_steeringFollowing = new gkSteeringPathFollowing(
	    obj,
	    velocity::RUN,
	    FORWARD,
	    UP,
	    SIDE,
	    gkVector3(2, 4, 2),
	    256,
	    0.003f
	);

	m_steeringFollowing->setGoalRadius(m_steeringFollowing->radius());
	m_steeringFollowing->setGoalPosition(m_momo->m_obj->getPosition());
	m_steeringFollowing->setMaxForce(10);

	m_steeringWander = new gkSteeringWander(
	    obj,
	    velocity::RUN,
	    FORWARD,
	    UP,
	    SIDE,
	    0.3f,
	    5
	);

	m_steeringWander->setMaxForce(10);

	m_obj->getChildEntity()->addAnimation(animation::IDLE_STR);
	m_obj->getChildEntity()->addAnimation(animation::WALK_STR);
	m_obj->getChildEntity()->addAnimation(animation::RUN_STR);
	m_obj->getChildEntity()->addAnimation(animation::DEATH_STR);
	m_obj->getChildEntity()->addAnimation(animation::STOP_STR);
	
	m_characterNode = m_tree->createNode<gkCharacterNode>();
	m_characterNode->setObj(m_obj);

	m_characterNode->setForward(m_steeringObject->forward());

	{
		gkFunctionNode<RatLogic, gkCharacterNode::STATE, FUNCTION_NODE_PARAM_ONE>* updateAINode = m_tree->createNode<gkFunctionNode<RatLogic, gkCharacterNode::STATE, FUNCTION_NODE_PARAM_ONE> >();
		updateAINode->getOBJECT()->setValue(this);
		updateAINode->getFUNCTION_1()->setValue(&RatLogic::updateAI);
		m_characterNode->getINPUT_AI_STATE()->link(updateAINode->getRESULT());
	}


	gkRayTestNode* hasHit = m_tree->createNode<gkRayTestNode>();
	hasHit->getTARGET()->setValue(m_obj);
	hasHit->getRAY_DIRECTION()->setValue(gkVector3(0, 0.5f, 0));

	// Initial state
	m_characterNode->getCURRENT_STATE()->setValue(animation::IDLE);

	//IDLE TRANSITION
	m_characterNode->addTransition(animation::DEATH, animation::IDLE, 15000);
	m_characterNode->addTransition(animation::IDLE, animation::IDLE);
	m_characterNode->addTransition(animation::WALK, animation::IDLE);
	m_characterNode->addTransition(animation::RUN, animation::IDLE);
	m_characterNode->addTransition(animation::STOP, animation::IDLE);

	INT_EQUAL_NODE_TYPE* wantToWalk = INT_EQUAL_NODE(m_characterNode->getOUTPUT_AI_STATE(), animation::WALK);

	// WALK TRANSITION
	IS_TRUE(wantToWalk)->link(m_characterNode->addTransition(animation::IDLE, animation::WALK));
	IS_TRUE(wantToWalk)->link(m_characterNode->addTransition(animation::WALK, animation::WALK));
	IS_TRUE(wantToWalk)->link(m_characterNode->addTransition(animation::RUN, animation::WALK));

	INT_EQUAL_NODE_TYPE* wantToRun = INT_EQUAL_NODE(m_characterNode->getOUTPUT_AI_STATE(), animation::RUN);

	// RUN TRANSITION
	IS_TRUE(wantToRun)->link(m_characterNode->addTransition(animation::IDLE, animation::RUN));
	IS_TRUE(wantToRun)->link(m_characterNode->addTransition(animation::WALK, animation::RUN));
	IS_TRUE(wantToRun)->link(m_characterNode->addTransition(animation::RUN, animation::RUN));

	//DEATH TRANSITION
	{
		PGAMEOBJ_EQUAL_NODE_TYPE* ifNode = PGAMEOBJ_EQUAL_NODE(m_momo->m_kickTestNode->getHIT_OBJ(), m_obj);
		IS_TRUE(ifNode)->link(m_characterNode->addTransition(animation::IDLE, animation::DEATH));
		IS_TRUE(ifNode)->link(m_characterNode->addTransition(animation::RUN, animation::DEATH));
		IS_TRUE(ifNode)->link(m_characterNode->addTransition(animation::WALK, animation::DEATH));
		m_characterNode->getANIM_NOT_HAS_REACHED_END()->link(m_characterNode->addTransition(animation::DEATH, animation::DEATH));
	}

	// STOP TRANSITION
	{
		gkFunctionNode<RatLogic, bool>* stuckNode = m_tree->createNode<gkFunctionNode<RatLogic, bool> >();
		stuckNode->getOBJECT()->setValue(this);
		stuckNode->getFUNCTION_0()->setValue(&RatLogic::isLogicStuck);

		stuckNode->getRESULT()->link(m_characterNode->addTransition(animation::WALK, animation::STOP));
		stuckNode->getRESULT()->link(m_characterNode->addTransition(animation::RUN, animation::STOP));
		stuckNode->getRESULT()->link(m_characterNode->addTransition(animation::STOP, animation::STOP));
	}

	gkCharacterNode::MAP map;
	typedef gkCharacterNode::StateData StateData;
	map[animation::IDLE] = StateData(animation::IDLE, animation::IDLE_STR, true, false, velocity::NONE, true);
	map[animation::WALK] = StateData(animation::WALK, animation::WALK_STR, true, false, velocity::WALK, true);
	map[animation::RUN] = StateData(animation::RUN, animation::RUN_STR, true, false, velocity::RUN, true);
	map[animation::DEATH] = StateData(animation::DEATH, animation::DEATH_STR, false, false, velocity::NONE, true);
	map[animation::STOP] = StateData(animation::STOP, animation::STOP_STR, false, false, velocity::NONE, true);

	m_characterNode->setMapping(map);

	defineLogicStates();
}

RatLogic::~RatLogic()
{
	delete m_steeringFollowing;
	delete m_steeringCapture;
	delete m_steeringWander;
}

void RatLogic::defineLogicStates()
{
	m_logicalState.setState(logic::CAPTURE);

	// FOLLOWING_PATH TRANSITION

	m_logicalState.addStartTrigger(logic::FOLLOWING_PATH, new gkFSM::LogicTrigger<RatLogic>(this, &RatLogic::StartFollowing));
	m_logicalState.addEndTrigger(logic::FOLLOWING_PATH, new gkFSM::LogicTrigger<RatLogic>(this, &RatLogic::EndFollowing));

	m_logicalState.addTransition(logic::CAPTURE, logic::FOLLOWING_PATH, 5000)->when(
	    new gkFSM::LogicEvent<RatLogic>(this, &RatLogic::AmIAtDiffLevelOfMomo));

	m_logicalState.addTransition(logic::WANDER, logic::FOLLOWING_PATH, 7000)->when(
	    new gkFSM::LogicEvent<RatLogic>(this, &RatLogic::AmIAtDiffLevelOfMomo));

	// CAPTURE TRANSITION

	m_logicalState.addTransition(logic::WANDER, logic::CAPTURE, 10000)->when(
	    new gkFSM::LogicEvent<RatLogic>(this, &RatLogic::AmINotInGoal));


	// STUCK TRANSITION

	m_logicalState.addTransition(logic::FOLLOWING_PATH, logic::STUCK, 1000)->when(
	    new gkFSM::LogicEvent<RatLogic>(this, &RatLogic::AmIStuck));

	m_logicalState.addTransition(logic::CAPTURE, logic::STUCK, 1000)->when(
	    new gkFSM::LogicEvent<RatLogic>(this, &RatLogic::AmIStuck));

	m_logicalState.addTransition(logic::WANDER, logic::STUCK, 1000)->when(
	    new gkFSM::LogicEvent<RatLogic>(this, &RatLogic::AmIStuck));


	// WANDER TRANSTION

	m_logicalState.addTransition(logic::STUCK, logic::WANDER, 3000);

	m_logicalState.addTransition(logic::FOLLOWING_PATH, logic::WANDER)->when(
	    new gkFSM::LogicEvent<RatLogic>(this, &RatLogic::AmIInGoal));

	m_logicalState.addTransition(logic::FOLLOWING_PATH, logic::WANDER, 15000);

}

gkCharacterNode::STATE RatLogic::updateAI(gkScalar tick)
{
	gkCharacterNode::STATE newState = gkCharacterNode::NULL_STATE;

	if (m_characterNode->getState() != animation::DEATH)
	{
		int oldLogicalState = m_logicalState.getState();

		m_logicalState.update();

		gkSteeringObject* steeringObject = getSteeringBehaviour();

		int newLogicalState = m_logicalState.getState();

		if (newLogicalState != oldLogicalState)
		{
			//gkLogMessage(m_obj->getName() << " " << oldLogicalState << "->" << newLogicalState);
			steeringObject->reset();
			steeringObject->setGoalPosition(m_momo->m_obj->getPosition());
			m_steeringObject = steeringObject;
		}

		//gkCharacterNode::STATE oldState = steeringObject->getState();

		if (steeringObject->update(tick))
		{
			gkScalar speed = steeringObject->speed();

			if (speed == 0)
				newState = animation::IDLE;
			else if (speed < velocity::RUN)
				newState = animation::WALK;
			else
				newState = animation::RUN;

			//if(steeringObject->getState() != oldState)
			//    gkLogMessage(steeringObject->getDebugStringState());
		}
	}

	return newState;
}

gkSteeringObject* RatLogic::getSteeringBehaviour() const
{
	switch (m_logicalState.getState())
	{
	case logic::CAPTURE:
		return m_steeringCapture;

	case logic::FOLLOWING_PATH:
		return m_steeringFollowing;

	case logic::STUCK:
		return m_steeringObject;

	case logic::WANDER:
		return m_steeringWander;

	default:
		GK_ASSERT(false);
		return 0;
	}
}

bool RatLogic::AmIAtDiffLevelOfMomo()
{
	const gkGameObjectProperties& props = m_obj->getProperties();

	const gkPhysicsProperties& phy = props.m_physics;

	return !m_characterNode->isFalling() && !m_momo->m_characterNode->isFalling() &&
	       gkAbs(m_obj->getPosition().z - m_momo->m_obj->getPosition().z) > phy.m_radius;
}

bool RatLogic::AmIInGoal()
{
	return m_steeringObject->getState() == gkSteeringObject::IN_GOAL;
}

bool RatLogic::AmINotInGoal()
{
	return m_steeringObject->getState() != gkSteeringObject::IN_GOAL;
}

bool RatLogic::AmIStuck()
{
	return m_steeringObject->getState() == gkSteeringObject::STUCK;
}

bool RatLogic::isLogicStuck()
{
	return logic::STUCK == m_logicalState.getState();
}

void RatLogic::StartFollowing(int from, int to)
{
	m_scene->refreshNavigationMesh();
}

void RatLogic::EndFollowing(int from, int to)
{
}
