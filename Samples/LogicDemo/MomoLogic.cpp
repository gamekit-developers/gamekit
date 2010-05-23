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

#include "MomoLogic.h"
#include "SceneLogic.h"
#include "Common.h"

namespace
{
	enum STATES
	{
		CARRY,
		CATCH,
		CATCH_ENDED,
		THROW_WITH,
		WALK,
		RUN,
		RUN_FASTER,
		WALK_BACK,
		IDLE_NASTY,
		IDLE_CAPOEIRA,
		FALL_UP,
		KICK
	};

	namespace velocity
	{
		gkScalar WALK = 1;
		gkScalar WALK_BACK = -1;
		gkScalar RUN = 2.5f;
		gkScalar RUN_FASTER = 4;
		gkScalar NONE = 0;
	}

	namespace animation
	{
		gkString CARRY = "Momo_Carry";
		gkString CATCH = "Momo_Catch";
		gkString DEATH = "Momo_Death";
		gkString DIE_LAVA = "Momo_DieLava";
		gkString DJ = "Momo_dj";
		gkString DROWNING = "Momo_Drowning";
		gkString EDGE_CLIMB = "Momo_EdgeClimb";
		gkString EDGE_IDLE = "Momo_EdgeIdle";
		gkString FALL = "Momo_Fall";
		gkString FALL_UP = "Momo_FallUp";
		gkString GLIDE = "Momo_Glide";
		gkString HIT_LIGHTLY = "Momo_Hit_Lightly";
		gkString HIT_CARRY = "Momo_HitCarry";
		gkString IDLE1 = "Momo_Idle1";
		gkString IDLE_CAPOEIRA = "Momo_IdleCapoeira";
		gkString IDLE_NASTY = "Momo_IdleNasty";
		gkString JUMP = "Momo_Jump";
		gkString KICK = "Momo_Kick";
		gkString LAND = "Momo_Land";
		gkString PICK = "Momo_Pick";
		gkString REVIVE = "Momo_Revive";
		gkString RUN = "Momo_Run";
		gkString RUN_FASTER = "Momo_RunFaster";
		gkString SHIMMY_L = "Momo_ShimmyL";
		gkString SHIMMY_R = "Momo_ShimmyR";
		gkString TAIL_WHIP = "Momo_TailWhip";
		gkString THROW_1 = "Momo_Throw1";
		gkString THROW_SHEEP = "Momo_ThrowSheep";
		gkString THROW_WITH = "Momo_ThrowWith";
		gkString THROW_WITHOUT = "Momo_ThrowWithout";
		gkString TURN_L = "Momo_Turn.L";
		gkString TURN_R = "Momo_Turn.R";
		gkString WALK = "Momo_Walk";
		gkString WALK_BACK = "Momo_WalkBack";
		gkString WALK_FAST = "Momo_WalkFast";
		gkString WALK_HAND = "Momo_WalkHand";
		gkString WALK_SLOW = "Momo_WalkSlow";
		gkString WALL_FLIP = "Momo_WallFlip";
	}
}

namespace particle
{
	gkString DUST_RUN = "DustRun";
	gkString DUST_WALK = "DustWalk";
}

MomoLogic::MomoLogic(gkGameObject* obj, SceneLogic* scene)
: m_characterNode(0),
m_kickTestNode(0),
m_obj(obj),
m_scene(scene),
m_tree(scene->m_tree),
m_momoGrab(0),
m_cameraNode(0)
{
	m_momoGrab = m_tree->createNode<gkGrabNode>();
	m_kickTestNode = m_tree->createNode<gkRayTestNode>();
	m_cameraNode = m_tree->createNode<gkCameraNode>();

	m_characterNode = m_tree->createNode<gkCharacterNode>();
	m_characterNode->setObj(m_obj);

	BOOL_AND_NODE_TYPE* ifNode = BOOL_AND_NODE(m_scene->m_shiftKeyNode->getIS_DOWN(), 
		m_scene->m_leftMouseNode->getPRESS());

	gkObjNode* targetNode = m_tree->createNode<gkObjNode>();
	targetNode->setType(gkObjNode::SCREEN_XY);
	targetNode->getUPDATE_OBJ()->link(ifNode->getIS_TRUE());
	targetNode->getX()->link(m_scene->m_mouseNode->getABS_X());
	targetNode->getY()->link(m_scene->m_mouseNode->getABS_Y());

	m_characterNode->getENABLE_GOTO()->link(IS_TRUE(BOOL_AND_NODE(ifNode->getIS_TRUE(), targetNode->getHAS_OBJ())));
	m_characterNode->getGOTO_POSITION()->link(targetNode->getHIT_POINT());

	CreateStateMachine();

	gkCharacterNode::MAP map;
	typedef gkCharacterNode::StateData StateData;
	map[CARRY] = StateData(CARRY, animation::CARRY, true, velocity::NONE);
	map[CATCH] = StateData(CATCH, animation::CATCH, true, velocity::NONE);
	map[CATCH_ENDED] = StateData(CATCH, animation::CATCH, true, velocity::NONE);
	map[THROW_WITH] = StateData(THROW_WITH, animation::THROW_WITH, true, velocity::NONE);
	map[WALK] = StateData(WALK, animation::WALK, true, velocity::WALK);
	map[RUN] = StateData(RUN, animation::RUN, true, velocity::RUN);
	map[RUN_FASTER] = StateData(RUN_FASTER, animation::RUN_FASTER, true, velocity::RUN_FASTER);
	map[WALK_BACK] = StateData(WALK_BACK, animation::WALK_BACK, true, velocity::WALK_BACK);
	map[IDLE_NASTY] = StateData(IDLE_NASTY, animation::IDLE_NASTY, true, velocity::NONE, false);
	map[IDLE_CAPOEIRA] = StateData(IDLE_CAPOEIRA, animation::IDLE_CAPOEIRA, true, velocity::NONE, false);
	map[FALL_UP] = StateData(FALL_UP, animation::FALL_UP, true, velocity::NONE);
	map[KICK] = StateData(KICK, animation::KICK, true, velocity::NONE, false);

	m_characterNode->setMapping(map);

	m_characterNode->getENABLE_ROTATION()->setValue(true);
	m_characterNode->getROTATION_VALUE()->link(m_cameraNode->getCURRENT_ROLL());

	CreateKick();
	CreateGrab();
	CreateDustTrail();
	CreateCamera();
}

MomoLogic::~MomoLogic()
{
}

void MomoLogic::CreateKick()
{
	m_kickTestNode->getENABLE()->link(m_characterNode->isCurrentStatus(KICK)->getIS_TRUE());
	m_kickTestNode->getTARGET()->setValue(m_obj);
	m_kickTestNode->getRAY_ORIGIN_OFFSET()->setValue(gkVector3(0, 0, -0.1f));
	m_kickTestNode->getRAY_DIRECTION()->setValue(gkVector3(0, 0.5f, 0));
}

void MomoLogic::CreateGrab()
{
	m_momoGrab->getUPDATE()->setValue(true);
	m_momoGrab->getTARGET()->setValue(m_obj);
	m_momoGrab->getGRAB_DIRECTION()->setValue(gkVector3(0, 0.6f, 0));
	m_momoGrab->getTHROW_VEL()->setValue(gkVector3(0, 10, 0));
	m_momoGrab->getRELATED_OFFSET_POSITION()->setValue(gkVector3(0, -0.6f, 0.5f));

	m_momoGrab->getCREATE_PICK()->link(
		IS_TRUE(BOOL_AND_NODE(m_scene->m_rightMouseNode->getPRESS(), m_scene->m_ctrlKeyNode->getNOT_IS_DOWN()))
	);

	m_momoGrab->getRELEASE_PICK()->link(
		IS_TRUE(BOOL_AND_NODE(m_scene->m_rightMouseNode->getRELEASE(), m_scene->m_ctrlKeyNode->getNOT_IS_DOWN()))
	);

	m_momoGrab->getTHROW_OBJECT()->link(
		IS_TRUE(
			BOOL_AND_NODE(
				m_characterNode->isCurrentStatus(THROW_WITH)->getIS_TRUE(), 
				IS_TRUE(REAL_GREATER_NODE(m_characterNode->getANIM_TIME_POSITION(), 20.13f))
			)
		)
	);
}

void MomoLogic::CreateDustTrail()
{
	gkParticleNode* particle = m_tree->createNode<gkParticleNode>();
	particle->getPARTICLE_SYSTEM_NAME()->setValue(particle::DUST_RUN);
	particle->getCREATE()->link(IS_TRUE(INT_EQUAL_NODE(m_characterNode->getCURRENT_STATE(), RUN_FASTER)));
	particle->getPOSITION()->link(m_characterNode->getPOSITION());
	particle->getORIENTATION()->link(m_characterNode->getROTATION());
}

void MomoLogic::CreateStateMachine()
{
	INT_EQUAL_NODE_TYPE* wantToWalk = INT_EQUAL_NODE(m_characterNode->getAI_WANTED_STATE(), WALK); 

	BOOL_OR_NODE_TYPE* walkConditionNode = BOOL_OR_NODE(m_scene->m_wKeyNode->getIS_DOWN(), IS_TRUE(wantToWalk));

	// Initial state
	m_characterNode->getCURRENT_STATE()->setValue(IDLE_NASTY); 

	// Transitions

	// IDLE_NASTY TRANSITIONS
	m_characterNode->addTransition(CATCH, IDLE_NASTY);
	m_characterNode->addTransition(CATCH_ENDED, IDLE_NASTY);
	m_characterNode->addTransition(CARRY, IDLE_NASTY);
	m_characterNode->addTransition(THROW_WITH, IDLE_NASTY);
	m_characterNode->addTransition(WALK, IDLE_NASTY);
	m_characterNode->addTransition(WALK_BACK, IDLE_NASTY);
	m_characterNode->addTransition(KICK, IDLE_NASTY);
	m_characterNode->addTransition(FALL_UP, IDLE_NASTY);
	m_characterNode->addTransition(IDLE_CAPOEIRA, IDLE_NASTY, 11000);

	// IDLE_CAPOEIRA TRANSITIONS
	m_characterNode->addTransition(IDLE_NASTY, IDLE_CAPOEIRA, 70000);

	gkFallTestNode* fallTest = m_tree->createNode<gkFallTestNode>();
	fallTest->getENABLE()->setValue(true);
	fallTest->getTARGET()->setValue(m_obj);

	//WALK TRANSITIONS
	walkConditionNode->getIS_TRUE()->link(m_characterNode->addTransition(IDLE_NASTY, WALK));
	walkConditionNode->getIS_TRUE()->link(m_characterNode->addTransition(IDLE_CAPOEIRA, WALK));
	walkConditionNode->getIS_TRUE()->link(m_characterNode->addTransition(WALK, WALK));
	walkConditionNode->getIS_TRUE()->link(m_characterNode->addTransition(KICK, WALK));
	IS_TRUE(wantToWalk)->link(m_characterNode->addTransition(RUN, WALK));
	m_characterNode->addTransition(RUN, WALK);


	// WALK_BACK TRANSITIONS
	m_scene->m_sKeyNode->getIS_DOWN()->link(m_characterNode->addTransition(WALK, WALK_BACK));
	m_scene->m_sKeyNode->getIS_DOWN()->link(m_characterNode->addTransition(IDLE_NASTY, WALK_BACK));
	m_scene->m_sKeyNode->getIS_DOWN()->link(m_characterNode->addTransition(IDLE_CAPOEIRA, WALK_BACK));
	m_scene->m_sKeyNode->getIS_DOWN()->link(m_characterNode->addTransition(WALK_BACK, WALK_BACK));
	m_scene->m_sKeyNode->getIS_DOWN()->link(m_characterNode->addTransition(KICK, WALK_BACK));

	BOOL_AND_NODE_TYPE* ifKick = BOOL_AND_NODE(
		BOOL_AND_NODE(m_scene->m_leftMouseNode->getPRESS(), m_scene->m_ctrlKeyNode->getNOT_IS_DOWN())->getIS_TRUE(),
		m_scene->m_shiftKeyNode->getNOT_IS_DOWN());

	// KICK TRANSITIONS
	IS_TRUE(ifKick)->link(m_characterNode->addTransition(WALK, KICK));
	IS_TRUE(ifKick)->link(m_characterNode->addTransition(WALK_BACK, KICK));
	IS_TRUE(ifKick)->link(m_characterNode->addTransition(IDLE_NASTY, KICK));
	IS_TRUE(ifKick)->link(m_characterNode->addTransition(IDLE_CAPOEIRA, KICK));
	m_characterNode->getANIM_NOT_HAS_REACHED_END()->link(m_characterNode->addTransition(KICK, KICK));

	// FALL_UP TRANSITIONS
	fallTest->getFALLING()->link(m_characterNode->addTransition(WALK, FALL_UP));
	fallTest->getFALLING()->link(m_characterNode->addTransition(RUN, FALL_UP));
	fallTest->getFALLING()->link(m_characterNode->addTransition(FALL_UP, FALL_UP));
	fallTest->getFALLING()->link(m_characterNode->addTransition(RUN_FASTER, FALL_UP));
	fallTest->getFALLING()->link(m_characterNode->addTransition(WALK_BACK, FALL_UP));
	fallTest->getFALLING()->link(m_characterNode->addTransition(IDLE_NASTY, FALL_UP));

	INT_EQUAL_NODE_TYPE* wantToRun = INT_EQUAL_NODE(m_characterNode->getAI_WANTED_STATE(), RUN); 

	// RUN TRANSITIONS
	m_scene->m_wKeyNode->getIS_DOWN()->link(m_characterNode->addTransition(WALK, RUN, 1500));
	IS_TRUE(wantToRun)->link(m_characterNode->addTransition(WALK, RUN));
	IS_TRUE(wantToRun)->link(m_characterNode->addTransition(IDLE_NASTY, RUN));
	IS_TRUE(wantToRun)->link(m_characterNode->addTransition(IDLE_CAPOEIRA, RUN));

	m_scene->m_wKeyNode->getIS_DOWN()->link(m_characterNode->addTransition(RUN, RUN));
	IS_TRUE(wantToRun)->link(m_characterNode->addTransition(RUN, RUN));

	m_characterNode->addTransition(RUN_FASTER, RUN);


	// RUN_FASTER TRANSITIONS
	{
		BOOL_AND_NODE_TYPE* ifNode = BOOL_AND_NODE(m_scene->m_shiftKeyNode->getIS_DOWN(), m_scene->m_wKeyNode->getIS_DOWN());

		ifNode->getIS_TRUE()->link(m_characterNode->addTransition(RUN, RUN_FASTER));
		ifNode->getIS_TRUE()->link(m_characterNode->addTransition(RUN_FASTER, RUN_FASTER));
	}
	
	// CATCH TRANSITIONS

	{
		BOOL_AND_NODE_TYPE* ifNode = BOOL_AND_NODE(m_scene->m_ctrlKeyNode->getNOT_IS_DOWN(), m_scene->m_rightMouseNode->getPRESS());

		ifNode->getIS_TRUE()->link(m_characterNode->addTransition(IDLE_NASTY, CATCH));
		ifNode->getIS_TRUE()->link(m_characterNode->addTransition(IDLE_CAPOEIRA, CATCH));
		m_characterNode->getANIM_NOT_HAS_REACHED_END()->link(m_characterNode->addTransition(CATCH, CATCH));
	}

	// CATCH_ENDED TRANSITIONS
	m_characterNode->getANIM_HAS_REACHED_END()->link(m_characterNode->addTransition(CATCH, CATCH_ENDED));

	// CARRY TRANSITIONS
	m_momoGrab->getCAUGHT_TRUE()->link(m_characterNode->addTransition(CATCH_ENDED, CARRY));
	m_momoGrab->getCAUGHT_TRUE()->link(m_characterNode->addTransition(CARRY, CARRY));

	// THROW_WITH TRANSITIONS
	IS_TRUE(BOOL_AND_NODE(m_scene->m_leftMouseNode->getPRESS(), m_scene->m_ctrlKeyNode->getNOT_IS_DOWN()))->link(
			m_characterNode->addTransition(CARRY, THROW_WITH));

	m_characterNode->getANIM_NOT_HAS_REACHED_END()->link(m_characterNode->addTransition(THROW_WITH, THROW_WITH));
}

void MomoLogic::CreateCamera()
{
	gkObjNode* centerObj0 = m_tree->createNode<gkObjNode>();
	centerObj0->setType(gkObjNode::SCREEN_XY);
	centerObj0->getX()->link(m_scene->m_mouseNode->getABS_X());
	centerObj0->getY()->link(m_scene->m_mouseNode->getABS_Y());
	centerObj0->getUPDATE_OBJ()->link(IS_TRUE(BOOL_AND_NODE(
		m_scene->m_shiftKeyNode->getNOT_IS_DOWN(),
		IS_TRUE(BOOL_AND_NODE(
			m_scene->m_leftMouseNode->getPRESS(), 
			m_scene->m_ctrlKeyNode->getIS_DOWN())))));


	centerObj0->getUPDATE_STATE()->setValue(false);

	gkObjNode* centerObj1 = m_tree->createNode<gkObjNode>();
	centerObj1->setType(gkObjNode::POINTER);
	centerObj1->getOBJ_POINTER()->setValue(m_obj);
	centerObj1->getUPDATE_OBJ()->link(m_scene->m_ctrlKeyNode->getNOT_IS_DOWN());
	centerObj1->getUPDATE_STATE()->link(m_scene->m_ctrlKeyNode->getNOT_IS_DOWN());

	gkMultiplexerNode<gkGameObject*>* selObj0 = m_tree->createNode<gkMultiplexerNode<gkGameObject*> >();
	selObj0->getINPUT_FALSE()->setValue(0);
	selObj0->getINPUT_TRUE()->link(centerObj1->getOBJ());
	selObj0->getSEL()->link(m_scene->m_ctrlKeyNode->getNOT_IS_DOWN());

	gkMultiplexerNode<gkGameObject*>* selObj = m_tree->createNode<gkMultiplexerNode<gkGameObject*> >();
	selObj->getINPUT_FALSE()->link(selObj0->getOUTPUT());
	selObj->getINPUT_TRUE()->link(centerObj0->getOBJ());
	selObj->getSEL()->link(
		IS_TRUE(BOOL_AND_NODE(
			m_scene->m_shiftKeyNode->getNOT_IS_DOWN(),
			IS_TRUE(BOOL_AND_NODE(
				m_scene->m_leftMouseNode->getIS_DOWN(), 
				m_scene->m_ctrlKeyNode->getIS_DOWN())))));

	gkMultiplexerNode<gkVector3>* selPos = m_tree->createNode<gkMultiplexerNode<gkVector3> >();
	selPos->getINPUT_FALSE()->link(centerObj0->getHIT_POINT());
	selPos->getINPUT_TRUE()->link(centerObj1->getPOSITION());
	selPos->getSEL()->link(m_scene->m_ctrlKeyNode->getNOT_IS_DOWN());

	m_cameraNode->getCENTER_OBJ()->link(selObj->getOUTPUT());
	m_cameraNode->getCENTER_POSITION()->link(selPos->getOUTPUT());
	m_cameraNode->getAVOID_BLOCKING()->link(m_scene->m_ctrlKeyNode->getNOT_IS_DOWN());
	m_cameraNode->getTARGET()->link(m_scene->m_cameraPlayer->getOBJ());
	m_cameraNode->getREL_X()->link(m_scene->m_mouseNode->getREL_X());
	m_cameraNode->getREL_Y()->link(m_scene->m_mouseNode->getREL_Y());
	m_cameraNode->getREL_Z()->link(m_scene->m_mouseNode->getWHEEL());
	m_cameraNode->getMIN_Z()->setValue(0.5f);
	m_cameraNode->getMAX_Z()->setValue(10);
}
