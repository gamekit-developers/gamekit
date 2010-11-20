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

using namespace AsyncCalls;

namespace
{
const gkVector3 FORWARD(0, 1, 0);
const gkVector3 UP(0, 0, 1);
const gkVector3 SIDE(1, 0, 0);

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
	FALL,
	FALL_UP,
	KICK,
	JUMP,
	GLIDE,
	FLY,
	LAND
};

namespace velocity
{
gkScalar WALK = 1.5;
gkScalar WALK_BACK = -1;
gkScalar RUN = 3.5f;
gkScalar RUN_FASTER = 5;
gkScalar FLY = 6;
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
	  m_cameraNode(0),
	  m_steeringObject(0),
	  m_ifSelectNode(0),
	  m_following(false),
	  m_hasImpactGround(false)
{
	m_steeringObject = new gkSteeringPathFollowing(
	    obj,
	    velocity::RUN_FASTER + 1,
	    FORWARD,
	    UP,
	    SIDE,
	    gkVector3(2, 4, 2),
	    256,
	    0.00005f
	);

	m_steeringObject->setGoalRadius(m_steeringObject->radius());
	m_steeringObject->setMaxForce(10);

	m_momoGrab = m_tree->createNode<gkGrabNode>();
	m_kickTestNode = m_tree->createNode<gkRayTestNode>();
	m_cameraNode = m_tree->createNode<gkCameraNode>();

	m_obj->getChildEntity()->addAnimation(animation::CARRY);
	m_obj->getChildEntity()->addAnimation(animation::CATCH);
	m_obj->getChildEntity()->addAnimation(animation::THROW_WITH);
	m_obj->getChildEntity()->addAnimation(animation::WALK);
	m_obj->getChildEntity()->addAnimation(animation::RUN);
	m_obj->getChildEntity()->addAnimation(animation::RUN_FASTER);
	m_obj->getChildEntity()->addAnimation(animation::WALK_BACK);
	m_obj->getChildEntity()->addAnimation(animation::IDLE_NASTY);
	m_obj->getChildEntity()->addAnimation(animation::IDLE_CAPOEIRA);
	m_obj->getChildEntity()->addAnimation(animation::FALL);
	m_obj->getChildEntity()->addAnimation(animation::FALL_UP);
	m_obj->getChildEntity()->addAnimation(animation::KICK);
	m_obj->getChildEntity()->addAnimation(animation::JUMP);
	m_obj->getChildEntity()->addAnimation(animation::GLIDE);
	m_obj->getChildEntity()->addAnimation(animation::LAND);

	m_characterNode = m_tree->createNode<gkCharacterNode>();
	m_characterNode->setObj(m_obj);

	m_ifSelectNode = BOOL_AND_NODE(m_scene->m_shiftKeyNode->getIS_DOWN(),
	                               m_scene->m_leftMouseNode->getPRESS());

	m_screenTargetNode = m_tree->createNode<gkScreenRayTestNode>();
	m_screenTargetNode->getENABLE()->link(m_ifSelectNode->getIS_TRUE());
	m_screenTargetNode->getSCREEN_X()->link(m_scene->m_mouseNode->getABS_X());
	m_screenTargetNode->getSCREEN_Y()->link(m_scene->m_mouseNode->getABS_Y());

	m_characterNode->setForward(m_steeringObject->forward());
	m_characterNode->getJUMP_SPEED()->setValue(15);
	m_characterNode->getJUMP()->link(m_scene->m_spcKeyNode->getPRESS());

	{
		gkFunctionNode<MomoLogic, gkCharacterNode::STATE, FUNCTION_NODE_PARAM_ONE>* updateAINode = m_tree->createNode<gkFunctionNode<MomoLogic, gkCharacterNode::STATE, FUNCTION_NODE_PARAM_ONE> >();
		updateAINode->getOBJECT()->setValue(this);
		updateAINode->getFUNCTION_1()->setValue(&MomoLogic::updateAI);
		m_characterNode->getINPUT_AI_STATE()->link(updateAINode->getRESULT());
	}

	{
		gkFunctionNode<MomoLogic, gkScalar>* gravityNode = m_tree->createNode<gkFunctionNode<MomoLogic, gkScalar> >();
		gravityNode->getOBJECT()->setValue(this);
		gravityNode->getFUNCTION_0()->setValue(&MomoLogic::getGravity);

		m_characterNode->getGRAVITY()->link(gravityNode->getRESULT());
	}

	CreateStateMachine();

	gkCharacterNode::MAP map;
	typedef gkCharacterNode::StateData StateData;
	map[CARRY] = StateData(CARRY, animation::CARRY, true, false, velocity::NONE, true);
	map[CATCH] = StateData(CATCH, animation::CATCH, true, false, velocity::NONE, true);
	map[CATCH_ENDED] = StateData(CATCH, animation::CATCH, true, false, velocity::NONE, true);
	map[THROW_WITH] = StateData(THROW_WITH, animation::THROW_WITH, true, false, velocity::NONE, true);
	map[WALK] = StateData(WALK, animation::WALK, true, false, velocity::WALK, true);
	map[RUN] = StateData(RUN, animation::RUN, true, false, velocity::RUN, true);
	map[RUN_FASTER] = StateData(RUN_FASTER, animation::RUN_FASTER, true, false, velocity::RUN_FASTER, true);
	map[WALK_BACK] = StateData(WALK_BACK, animation::WALK_BACK, true, false, velocity::WALK_BACK, true);
	map[IDLE_NASTY] = StateData(IDLE_NASTY, animation::IDLE_NASTY, true, false, velocity::NONE, false);
	map[IDLE_CAPOEIRA] = StateData(IDLE_CAPOEIRA, animation::IDLE_CAPOEIRA, true, false, velocity::NONE, false);
	map[FALL] = StateData(FALL, animation::FALL, false, true, velocity::NONE, true);
	map[FALL_UP] = StateData(FALL_UP, animation::FALL_UP, true, true, velocity::NONE, true);
	map[KICK] = StateData(KICK, animation::KICK, true, false, velocity::NONE, false);
	map[JUMP] = StateData(JUMP, animation::JUMP, false, true, velocity::NONE, true);
	map[GLIDE] = StateData(GLIDE, animation::GLIDE, true, true, velocity::NONE, true);
	map[FLY] = StateData(FLY, animation::GLIDE, true, false, velocity::FLY, true);
	map[LAND] = StateData(LAND, animation::LAND, false, false, velocity::NONE, false);

	m_characterNode->setMapping(map);

	m_characterNode->addStartTrigger(LAND, new gkFSM::LogicTrigger<MomoLogic>(this, &MomoLogic::StartLand));
	m_characterNode->addEndTrigger(LAND, new gkFSM::LogicTrigger<MomoLogic>(this, &MomoLogic::EndLand));


	m_characterNode->getENABLE_ROTATION()->setValue(true);
	m_characterNode->getROTATION_VALUE()->link(m_cameraNode->getCURRENT_ROLL());

	CreateKick();
	CreateGrab();
	CreateDustTrail();
	CreateImpactGroundFX();
	CreateCamera();
}

MomoLogic::~MomoLogic()
{
	delete m_steeringObject;
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

void MomoLogic::CreateImpactGroundFX()
{
	gkParticleNode* particle = m_tree->createNode<gkParticleNode>();
	particle->getPARTICLE_SYSTEM_NAME()->setValue(particle::DUST_WALK);
	particle->getPOSITION()->link(m_characterNode->getPOSITION());

	gkFunctionNode<MomoLogic, bool>* hasImpactGroundNode = m_tree->createNode<gkFunctionNode<MomoLogic, bool> >();
	hasImpactGroundNode->getOBJECT()->setValue(this);
	hasImpactGroundNode->getFUNCTION_0()->setValue(&MomoLogic::hasImpactGround);

	particle->getCREATE()->link(hasImpactGroundNode->getRESULT());
}

void MomoLogic::CreateStateMachine()
{
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
	m_characterNode->addTransition(JUMP, IDLE_NASTY);

	gkFSM::LogicTrigger<MomoLogic>* land2idleTrigger = new gkFSM::LogicTrigger<MomoLogic>(this, &MomoLogic::handleLand2IdleTranstion);

	m_characterNode->addTransition(LAND, IDLE_NASTY, 0, land2idleTrigger);

	// IDLE_CAPOEIRA TRANSITIONS
	m_characterNode->addTransition(IDLE_NASTY, IDLE_CAPOEIRA, 70000);

	//WALK TRANSITIONS

	INT_EQUAL_NODE_TYPE* wantToWalk = INT_EQUAL_NODE(m_characterNode->getOUTPUT_AI_STATE(), WALK);
	BOOL_OR_NODE_TYPE* walkConditionNode = BOOL_OR_NODE(m_scene->m_wKeyNode->getIS_DOWN(), IS_TRUE(wantToWalk));

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
	m_characterNode->getFALLING()->link(m_characterNode->addTransition(WALK, FALL_UP));
	m_characterNode->getFALLING()->link(m_characterNode->addTransition(RUN, FALL_UP));
	m_characterNode->getFALLING()->link(m_characterNode->addTransition(FALL_UP, FALL_UP));
	m_characterNode->getFALLING()->link(m_characterNode->addTransition(RUN_FASTER, FALL_UP));
	m_characterNode->getFALLING()->link(m_characterNode->addTransition(WALK_BACK, FALL_UP));
	m_characterNode->getFALLING()->link(m_characterNode->addTransition(IDLE_NASTY, FALL_UP));

	BOOL_AND_NODE_TYPE* ifFallingAfterJump = BOOL_AND_NODE(
	            m_characterNode->getFALLING(),
	            m_characterNode->getANIM_HAS_REACHED_END());

	IS_TRUE(ifFallingAfterJump)->link(m_characterNode->addTransition(FALL, FALL_UP));

	// RUN TRANSITIONS

	INT_EQUAL_NODE_TYPE* wantToRun = INT_EQUAL_NODE(m_characterNode->getOUTPUT_AI_STATE(), RUN);

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

		BOOL_OR_NODE_TYPE* wantToRunFaster = BOOL_OR_NODE(
		        IS_TRUE(INT_EQUAL_NODE(m_characterNode->getOUTPUT_AI_STATE(), RUN_FASTER)),
		        IS_TRUE(ifNode));

		IS_TRUE(wantToRunFaster)->link(m_characterNode->addTransition(RUN, RUN_FASTER));
		IS_TRUE(wantToRunFaster)->link(m_characterNode->addTransition(RUN_FASTER, RUN_FASTER));
		IS_TRUE(wantToRunFaster)->link(m_characterNode->addTransition(WALK, RUN_FASTER));
		IS_TRUE(wantToRunFaster)->link(m_characterNode->addTransition(IDLE_NASTY, RUN_FASTER));
		IS_TRUE(wantToRunFaster)->link(m_characterNode->addTransition(IDLE_CAPOEIRA, RUN_FASTER));

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

	// JUMP TRANSITIONS

	m_scene->m_spcKeyNode->getPRESS()->link(m_characterNode->addTransition(IDLE_NASTY, JUMP));
	m_scene->m_spcKeyNode->getPRESS()->link(m_characterNode->addTransition(IDLE_CAPOEIRA, JUMP));
	m_scene->m_spcKeyNode->getPRESS()->link(m_characterNode->addTransition(WALK, JUMP));
	m_scene->m_spcKeyNode->getPRESS()->link(m_characterNode->addTransition(WALK_BACK, JUMP));
	m_scene->m_spcKeyNode->getPRESS()->link(m_characterNode->addTransition(KICK, JUMP));
	m_scene->m_spcKeyNode->getPRESS()->link(m_characterNode->addTransition(RUN, JUMP));
	m_scene->m_spcKeyNode->getPRESS()->link(m_characterNode->addTransition(RUN_FASTER, JUMP));

	BOOL_AND_NODE_TYPE* ifJumpContinueNode = BOOL_AND_NODE(
	            m_characterNode->getANIM_NOT_HAS_REACHED_END(),
	            m_characterNode->getFALLING());

	IS_TRUE(ifJumpContinueNode)->link(m_characterNode->addTransition(JUMP, JUMP));

	// FALL TRANSITION

	IS_TRUE(ifFallingAfterJump)->link(m_characterNode->addTransition(JUMP, FALL));

	BOOL_AND_NODE_TYPE* ifFallContinueNode = BOOL_AND_NODE(
	            m_characterNode->getANIM_NOT_HAS_REACHED_END(),
	            m_characterNode->getFALLING());

	IS_TRUE(ifFallContinueNode)->link(m_characterNode->addTransition(FALL, FALL));

	// GLIDE TRANSITION

	m_scene->m_spcKeyNode->getIS_DOWN()->link(m_characterNode->addTransition(JUMP, GLIDE, 1000));

	BOOL_AND_NODE_TYPE* ifGlideContinueNode = BOOL_AND_NODE(
	            BOOL_AND_NODE(
	                m_scene->m_wKeyNode->getNOT_IS_DOWN(),
	                m_scene->m_spcKeyNode->getIS_DOWN())->getIS_TRUE(),
	            m_characterNode->getFALLING());

	IS_TRUE(ifGlideContinueNode)->link(m_characterNode->addTransition(GLIDE, GLIDE));
	IS_TRUE(ifGlideContinueNode)->link(m_characterNode->addTransition(FLY, GLIDE));

	// FLY TRANSITION

	m_scene->m_wKeyNode->getIS_DOWN()->link(m_characterNode->addTransition(GLIDE, FLY));

	BOOL_AND_NODE_TYPE* ifFlyNode = BOOL_AND_NODE(
	                                    BOOL_AND_NODE(
	                                        m_scene->m_spcKeyNode->getIS_DOWN(),
	                                        m_characterNode->getFALLING())->getIS_TRUE(),
	                                    m_scene->m_wKeyNode->getIS_DOWN());


	IS_TRUE(ifFlyNode)->link(m_characterNode->addTransition(FLY, FLY));


	// LAND TRANSITION

	IS_FALSE(ifGlideContinueNode)->link(m_characterNode->addTransition(GLIDE, LAND));

	IS_TRUE(BOOL_AND_NODE(IS_FALSE(ifGlideContinueNode), IS_FALSE(ifFlyNode)))->link(m_characterNode->addTransition(FLY, LAND));

	m_characterNode->getANIM_NOT_HAS_REACHED_END()->link(m_characterNode->addTransition(LAND, LAND));

}

void MomoLogic::CreateCamera()
{
	gkScreenRayTestNode* centerObj0 = m_tree->createNode<gkScreenRayTestNode>();
	centerObj0->getSCREEN_X()->link(m_scene->m_mouseNode->getABS_X());
	centerObj0->getSCREEN_Y()->link(m_scene->m_mouseNode->getABS_Y());
	centerObj0->getENABLE()->link(IS_TRUE(BOOL_AND_NODE(
	        m_scene->m_shiftKeyNode->getNOT_IS_DOWN(),
	        IS_TRUE(BOOL_AND_NODE(
	                    m_scene->m_leftMouseNode->getPRESS(),
	                    m_scene->m_ctrlKeyNode->getIS_DOWN())))));

	gkObjectNode* centerObj1 = m_tree->createNode<gkObjectNode>();
	centerObj1->getINPUT()->link(m_scene->m_ctrlKeyNode->getNOT_IS_DOWN());
	centerObj1->setOtherObject(m_obj->getName());

	gkMultiplexerNode<gkGameObject*>* selObj0 = m_tree->createNode<gkMultiplexerNode<gkGameObject*> >();
	selObj0->getINPUT_FALSE()->setValue(0);
	selObj0->getINPUT_TRUE()->setValue(m_obj);
	selObj0->getSEL()->link(m_scene->m_ctrlKeyNode->getNOT_IS_DOWN());

	gkMultiplexerNode<gkGameObject*>* selObj = m_tree->createNode<gkMultiplexerNode<gkGameObject*> >();
	selObj->getINPUT_FALSE()->link(selObj0->getOUTPUT());
	selObj->getINPUT_TRUE()->link(centerObj0->getHIT_OBJ());
	selObj->getSEL()->link(
	    IS_TRUE(BOOL_AND_NODE(
	                m_scene->m_shiftKeyNode->getNOT_IS_DOWN(),
	                IS_TRUE(BOOL_AND_NODE(
	                            m_scene->m_leftMouseNode->getIS_DOWN(),
	                            m_scene->m_ctrlKeyNode->getIS_DOWN())))));

	gkMultiplexerNode<gkVector3>* selPos = m_tree->createNode<gkMultiplexerNode<gkVector3> >();
	selPos->getINPUT_FALSE()->link(centerObj0->getHIT_POSITION());
	selPos->getINPUT_TRUE()->link(centerObj1->getOUT_POSITION());
	selPos->getSEL()->link(m_scene->m_ctrlKeyNode->getNOT_IS_DOWN());

	m_cameraNode->getCENTER_OBJ()->link(selObj->getOUTPUT());
	m_cameraNode->getCENTER_POSITION()->link(selPos->getOUTPUT());
	m_cameraNode->getAVOID_BLOCKING()->link(m_scene->m_ctrlKeyNode->getNOT_IS_DOWN());
	m_cameraNode->getTARGET()->setValue(m_scene->m_camera);
	m_cameraNode->getREL_X()->link(m_scene->m_mouseNode->getREL_X());
	m_cameraNode->getREL_Y()->link(m_scene->m_mouseNode->getREL_Y());
	m_cameraNode->getREL_Z()->link(m_scene->m_mouseNode->getWHEEL());
	m_cameraNode->getMIN_Z()->setValue(0.5f);
	m_cameraNode->getMAX_Z()->setValue(50);
}

gkCharacterNode::STATE MomoLogic::updateAI(gkScalar tick)
{
	gkCharacterNode::STATE newState = gkCharacterNode::NULL_STATE;

	bool userSelectPos = m_ifSelectNode->getIS_TRUE()->getValue() && m_screenTargetNode->getHIT()->getValue();

	bool userMove = m_scene->m_sKeyNode->getIS_DOWN()->getValue() || m_scene->m_wKeyNode->getIS_DOWN()->getValue();

	if (userSelectPos)
	{
		m_steeringObject->reset();
		m_steeringObject->setGoalPosition(m_screenTargetNode->getHIT_POSITION()->getValue());
		m_following = true;
		m_scene->refreshNavigationMesh();
	}
	else if (userMove)
	{
		m_following = false;
	}

	if (m_following && m_steeringObject->getNavMesh().get())
	{
		if (m_steeringObject->update(tick))
		{
			gkScalar speed = m_steeringObject->speed();

			if (speed == 0)
				newState = IDLE_CAPOEIRA;
			else if (speed < velocity::RUN)
				newState = WALK;
			else if (speed < velocity::RUN_FASTER)
				newState = RUN;
			else
				newState = RUN_FASTER;
		}
		else
		{
			m_following = false;
		}
	}

	return newState;
}

gkScalar MomoLogic::getGravity()
{
	if (m_characterNode->getState() == GLIDE || m_characterNode->getState() == FLY)
		return 2.5f;
	else
		return 9.81f;
}

bool MomoLogic::hasImpactGround()
{
	if (!m_hasImpactGround)
	{
		if (m_characterNode->getState(1) == FALL_UP && !m_characterNode->isFalling())
		{
			m_hasImpactGround = true;

			return true;
		}
	}
	else if (m_characterNode->getState(1) != FALL_UP)
	{
		m_hasImpactGround = false;
	}

	return false;
}

void MomoLogic::handleLand2IdleTranstion(int fromState, int toState)
{
	GK_ASSERT(fromState == LAND && toState == IDLE_NASTY);
}

void MomoLogic::StartLand(int from, int to)
{
//    gkLogMessage("Momo Start Land");
}

void MomoLogic::EndLand(int from, int to)
{
//    gkLogMessage("Momo End Land");
}
