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
: m_scene(scene),
m_animNode(scene->m_tree->createNode<gkAnimationNode>()),
m_pathFindingNode(scene->m_tree->createNode<gkFindPathNode>()),
m_stateMachineNode(scene->m_tree->createNode<gkStateMachineNode>()),
m_followPathNode(scene->m_tree->createNode<gkFollowPathNode>()),
m_playerNode(scene->m_tree->createNode<gkObjNode>()),
m_rayTestNode(scene->m_tree->createNode<gkRayTestNode>())
{
	m_playerNode->getUPDATE_OBJ()->link(m_scene->m_pulseNode->getOUTPUT());
	m_playerNode->getOBJ_NAME()->setValue(name);

	// Animation
	typedef gkMapNode<int, gkString> MAP_NODE;
	
	MAP_NODE* mapNode = m_scene->m_tree->createNode< MAP_NODE >();
	mapNode->getINPUT()->link(m_stateMachineNode->getCURRENT_STATE());
	
	MAP_NODE::MAP mapping;
	mapping[IDLE] = animation::IDLE;
	mapping[WALK] = animation::WALK;
	mapping[RUN] = animation::RUN;
	mapping[DEATH] = animation::DEATH;

	mapNode->getMAPPING()->setValue(mapping);

	m_animNode->getTARGET()->link(m_playerNode->getMESH_OBJ());
	m_animNode->getANIM_NAME()->link(mapNode->getOUTPUT());

	// Pahfinding for Rat

	gkIfNode<int, CMP_NOT_EQUAL>* ifNode = m_scene->m_tree->createNode<gkIfNode<int, CMP_NOT_EQUAL> >();
	ifNode->getA()->setValue(DEATH);
	ifNode->getB()->link(m_stateMachineNode->getCURRENT_STATE());

	m_pathFindingNode->getUPDATE()->link(ifNode->getIS_TRUE());

	m_pathFindingNode->getSTART_POS()->link(m_playerNode->getPOSITION());
	m_pathFindingNode->getEND_POS()->link(momo->m_playerNode->getPOSITION());
	

	{
		gkIfNode<gkString, CMP_FIND>* ifBNode = m_scene->m_tree->createNode<gkIfNode<gkString, CMP_FIND> >();
		ifBNode->getA()->link(m_rayTestNode->getHIT_NAME());
		ifBNode->getB()->setValue(gkString("Rat"));

		gkIfNode<bool, CMP_AND>* ifNode = m_scene->m_tree->createNode<gkIfNode<bool, CMP_AND> >();
		ifNode->getA()->link(m_pathFindingNode->getPATH_FOUND());
		ifNode->getB()->link(ifBNode->getIS_FALSE());

		m_followPathNode->getUPDATE()->link(ifNode->getIS_TRUE());
		m_pathFindingNode->getREDO_PATH_IF_FOLLOWING()->link(ifBNode->getIS_TRUE());
	}
	m_followPathNode->getTARGET()->link(m_playerNode->getOBJ());
	m_followPathNode->getSOURCE()->link(momo->m_playerNode->getOBJ());
	m_followPathNode->getPATH()->link(m_pathFindingNode->getPATH());
	m_followPathNode->setWalkVelocity(velocity::WALK);
	m_followPathNode->setRunVelocity(velocity::RUN);
	m_followPathNode->getSHOW_PATH_OFFSET()->setValue(gkVector3(0, 0, 0.3f));
	
	

	// State machine

	// Initial state
	m_stateMachineNode->getCURRENT_STATE()->setValue(IDLE); 

	//IDLE TRANSITION
	m_animNode->getHAS_REACHED_END()->link(m_stateMachineNode->addTransition(DEATH, IDLE));
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
		gkIfNode<gkString, CMP_FIND>* ifNode = m_scene->m_tree->createNode<gkIfNode<gkString, CMP_FIND> >();
		ifNode->getA()->link(momo->m_kickTestNode->getHIT_NAME());
		ifNode->getB()->setValue(gkString("Rat"));

		ifNode->getIS_TRUE()->link(m_stateMachineNode->addTransition(IDLE, DEATH));
		ifNode->getIS_TRUE()->link(m_stateMachineNode->addTransition(RUN, DEATH));
		ifNode->getIS_TRUE()->link(m_stateMachineNode->addTransition(WALK, DEATH));
		m_animNode->getNOT_HAS_REACHED_END()->link(m_stateMachineNode->addTransition(DEATH, DEATH));
	}

	RayTest();
}

RatLogic::~RatLogic()
{
}

void RatLogic::RayTest()
{
	m_rayTestNode->getENABLE()->setValue(true);
	m_rayTestNode->getTARGET()->link(m_playerNode->getOBJ());
	m_rayTestNode->getRAY_DIRECTION()->setValue(gkVector3(0, 0.5f, 0));

}
