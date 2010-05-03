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
		gkScalar WALK = 0.5f;
		gkScalar RUN = 1.5f;
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

RatLogic::RatLogic(const gkString& name, SceneLogic* scene, PMOMO momo)
: m_name(name),
m_scene(scene),
m_momo(momo),
m_animNode(0),
m_pathFindingNode(0),
m_stateMachineNode(0),
m_followPathNode(0),
m_playerNode(0),
m_hasHit(0)
{
	CreateNodes();
	CreatePlayer();
	CreateAnimation();
	CreatePathfinding();
	CreateStateMachine();	
}

RatLogic::~RatLogic()
{
}

void RatLogic::CreateNodes()
{
	m_animNode = m_scene->m_tree->createNode<gkAnimationNode>();
	m_pathFindingNode = m_scene->m_tree->createNode<gkFindPathNode>();
	m_stateMachineNode = m_scene->m_tree->createNode<gkStateMachineNode>();
	m_followPathNode = m_scene->m_tree->createNode<gkFollowPathNode>();
	m_playerNode = m_scene->m_tree->createNode<gkObjNode>();
}

void RatLogic::CreatePlayer()
{
	m_playerNode->getUPDATE_OBJ()->link(m_scene->m_pulseNode->getOUTPUT());
	m_playerNode->getOBJ_NAME()->setValue(m_name);
}

void RatLogic::CreateAnimation()
{
	typedef gkMapNode<int, gkString> MAP_VEL_NODE;
	
	MAP_VEL_NODE* mapVelNode = m_scene->m_tree->createNode< MAP_VEL_NODE >();
	mapVelNode->getINPUT()->link(m_stateMachineNode->getCURRENT_STATE());
	
	MAP_VEL_NODE::MAP mapVel;
	mapVel[IDLE] = animation::IDLE;
	mapVel[WALK] = animation::WALK;
	mapVel[RUN] = animation::RUN;
	mapVel[DEATH] = animation::DEATH;

	mapVelNode->getMAPPING()->setValue(mapVel);

	m_animNode->getTARGET()->link(m_playerNode->getMESH_OBJ());
	m_animNode->getANIM_NAME()->link(mapVelNode->getOUTPUT());

	typedef gkMapNode<int, bool> MAP_LOOP_NODE;
	
	MAP_LOOP_NODE* mapLoopNode = m_scene->m_tree->createNode< MAP_LOOP_NODE >();
	mapLoopNode->getINPUT()->link(m_stateMachineNode->getCURRENT_STATE());
	
	MAP_LOOP_NODE::MAP mapLoop;
	mapLoop[IDLE] = true;
	mapLoop[WALK] = true;
	mapLoop[RUN] = true;
	mapLoop[DEATH] = false;

	mapLoopNode->getMAPPING()->setValue(mapLoop);

	m_animNode->getLOOP()->link(mapLoopNode->getOUTPUT());
}

void RatLogic::CreatePathfinding()
{
	m_pathFindingNode->getUPDATE()->link(MyCurrentStatusIs(DEATH)->getIS_FALSE());

	m_pathFindingNode->getSTART_POS()->link(m_playerNode->getPOSITION());
	m_pathFindingNode->getEND_POS()->link(m_momo->m_playerNode->getPOSITION());

	m_pathFindingNode->getREDO_PATH_IF_FOLLOWING()->link(HasHit()->getHIT());
	
	m_followPathNode->getUPDATE()->setValue(true);
	m_followPathNode->getTARGET()->link(m_playerNode->getOBJ());
	m_followPathNode->getSOURCE()->link(m_momo->m_playerNode->getOBJ());
	m_followPathNode->getPATH()->link(m_pathFindingNode->getPATH());
	m_followPathNode->setWalkVelocity(velocity::WALK);
	m_followPathNode->setRunVelocity(velocity::RUN);
	m_followPathNode->getSHOW_PATH_OFFSET()->setValue(gkVector3(0, 0, 0.3f));
}

gkRayTestNode* RatLogic::HasHit()
{
	if(!m_hasHit)
	{
		m_hasHit = m_scene->m_tree->createNode<gkRayTestNode>();
		m_hasHit->getENABLE()->setValue(true);
		m_hasHit->getTARGET()->link(m_playerNode->getOBJ());
		m_hasHit->getRAY_DIRECTION()->setValue(gkVector3(0, 0.5f, 0));
	}

	return m_hasHit;
}

RatLogic::PNODE RatLogic::MyCurrentStatusIs(int status)
{
	std::pair<MAP::iterator, bool> result = 
		m_statuses.insert(MAP::value_type(status, m_scene->m_tree->createNode<NODE >()));

	if(result.second)
	{
		result.first->second->getA()->setValue(status);
		result.first->second->getB()->link(m_stateMachineNode->getCURRENT_STATE());
	}

	return result.first->second;
}

void RatLogic::CreateStateMachine()
{
	// Initial state
	m_stateMachineNode->getCURRENT_STATE()->setValue(IDLE); 

	//IDLE TRANSITION
	m_stateMachineNode->addTransition(DEATH, IDLE, 15000);
	m_stateMachineNode->addTransition(IDLE, IDLE);
	m_stateMachineNode->addTransition(WALK, IDLE);
	m_stateMachineNode->addTransition(RUN, IDLE);

	// WALK TRANSITION
	m_followPathNode->getWALK()->link(m_stateMachineNode->addTransition(IDLE, WALK));
	m_followPathNode->getWALK()->link(m_stateMachineNode->addTransition(WALK, WALK));
	m_followPathNode->getWALK()->link(m_stateMachineNode->addTransition(RUN, WALK));

	// RUN TRANSITION
	m_followPathNode->getRUN()->link(m_stateMachineNode->addTransition(IDLE, RUN));
	m_followPathNode->getRUN()->link(m_stateMachineNode->addTransition(WALK, RUN));
	m_followPathNode->getRUN()->link(m_stateMachineNode->addTransition(RUN, RUN));

	//DEATH TRANSITION
	{
		gkIfNode<gkGameObject*, CMP_EQUALS>* ifNode = m_scene->m_tree->createNode<gkIfNode<gkGameObject*, CMP_EQUALS> >();
		ifNode->getA()->link(m_momo->m_kickTestNode->getHIT_OBJ());
		ifNode->getB()->link(m_playerNode->getOBJ());

		ifNode->getIS_TRUE()->link(m_stateMachineNode->addTransition(IDLE, DEATH));
		ifNode->getIS_TRUE()->link(m_stateMachineNode->addTransition(RUN, DEATH));
		ifNode->getIS_TRUE()->link(m_stateMachineNode->addTransition(WALK, DEATH));
		m_animNode->getNOT_HAS_REACHED_END()->link(m_stateMachineNode->addTransition(DEATH, DEATH));
	}
}

