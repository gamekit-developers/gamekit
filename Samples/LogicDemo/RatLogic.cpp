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

RatLogic::RatLogic(gkGameObject* obj, SceneLogic* scene, PMOMO momo)
: m_obj(obj),
m_scene(scene),
m_tree(scene->m_tree),
m_momo(momo),
m_characterNode(0)
{
	gkCharacterNode* characterNode = m_tree->createNode<gkCharacterNode>();
	m_characterNode = characterNode;
	characterNode->setObj(m_obj);

	characterNode->getAI_ENABLE()->link(characterNode->isCurrentStatus(DEATH)->getIS_FALSE());
	m_characterNode->getAI_LOGIC()->setValue(gkCharacterNode::SEEKER);
	characterNode->setSeekerTarget(m_momo->m_obj);

	gkRayTestNode* hasHit = m_tree->createNode<gkRayTestNode>();
	hasHit->getTARGET()->setValue(m_obj);
	hasHit->getRAY_DIRECTION()->setValue(gkVector3(0, 0.5f, 0));

	// Initial state
	characterNode->getCURRENT_STATE()->setValue(IDLE); 

	//IDLE TRANSITION
	characterNode->addTransition(DEATH, IDLE, 15000);
	characterNode->addTransition(IDLE, IDLE);
	characterNode->addTransition(WALK, IDLE);
	characterNode->addTransition(RUN, IDLE);

	INT_EQUAL_NODE_TYPE* wantToWalk = INT_EQUAL_NODE(characterNode->getAI_WANTED_STATE(), WALK); 

	// WALK TRANSITION
	IS_TRUE(wantToWalk)->link(characterNode->addTransition(IDLE, WALK));
	IS_TRUE(wantToWalk)->link(characterNode->addTransition(WALK, WALK));
	IS_TRUE(wantToWalk)->link(characterNode->addTransition(RUN, WALK));

	INT_EQUAL_NODE_TYPE* wantToRun = INT_EQUAL_NODE(characterNode->getAI_WANTED_STATE(), RUN); 

	// RUN TRANSITION
	IS_TRUE(wantToRun)->link(characterNode->addTransition(IDLE, RUN));
	IS_TRUE(wantToRun)->link(characterNode->addTransition(WALK, RUN));
	IS_TRUE(wantToRun)->link(characterNode->addTransition(RUN, RUN));

	//DEATH TRANSITION
	{
		PGAMEOBJ_EQUAL_NODE_TYPE* ifNode = PGAMEOBJ_EQUAL_NODE(m_momo->m_kickTestNode->getHIT_OBJ(), m_obj);
		IS_TRUE(ifNode)->link(characterNode->addTransition(IDLE, DEATH));
		IS_TRUE(ifNode)->link(characterNode->addTransition(RUN, DEATH));
		IS_TRUE(ifNode)->link(characterNode->addTransition(WALK, DEATH));
		characterNode->getANIM_NOT_HAS_REACHED_END()->link(characterNode->addTransition(DEATH, DEATH));
	}

	gkCharacterNode::MAP map;
	typedef gkCharacterNode::StateData StateData;
	map[IDLE] = StateData(IDLE, animation::IDLE, true, 0);
	map[WALK] = StateData(WALK, animation::WALK, true, velocity::WALK);
	map[RUN] = StateData(RUN, animation::RUN, true, velocity::RUN);
	map[DEATH] = StateData(DEATH, animation::DEATH, false, 0);

	characterNode->setMapping(IDLE, map);
}

RatLogic::~RatLogic()
{
}

