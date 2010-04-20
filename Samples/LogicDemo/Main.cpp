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

#include "OgreKit.h"

namespace momoState
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
		FALL_UP
	};
}

namespace momoVelocity
{
	gkScalar WALK = 1;
	gkScalar WALK_BACK = -1;
	gkScalar RUN = 2.5f;
	gkScalar RUN_FASTER = 4;
}

namespace momoAnimation
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

namespace ratState
{
	enum STATES
	{
		IDLE,
		WALK,
		RUN
	};
}

namespace ratVelocity
{
	gkScalar WALK = 0.5f;
	gkScalar RUN = 1.5f;
}

namespace ratAnimation
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


namespace object
{
	gkString PLAYER = "Player";
	gkString CAMERA = "View";
	gkString PLANE = "Plane";
	gkString RAT_PLAYER = "RatPlayer";
}

namespace material
{
	gkString ARROW_CURSOR = "ArrowCursor";
}

namespace particle
{
	gkString DUST_RUN = "DustRun";
	gkString DUST_WALK = "DustWalk";
}

class OgreKit : public gkCoreApplication
{
public:

    OgreKit(const gkString &blend) 
		: m_blend(blend), m_tree(0), m_ctrlKeyNode(0), m_shiftKeyNode(0),
		m_wKeyNode(0), m_sKeyNode(0), m_cKeyNode(0), m_xKeyNode(0), m_mouseNode(0), m_leftMouseNode(0), m_rightMouseNode(0),
		m_momoPlayer(0), m_ratPlayer(0), m_cameraPlayer(0), m_animNode(0), m_animRatNode(0), m_momoCameraArcBall(0),
		m_momoGrab(0), m_stateMachine(0), m_stateRatMachine(0), m_momoFollowPathNode(0)
	{
        gkPath path = "./data/OgreKitStartup.cfg";

		GK_ASSERT(path.isFile());
        
		m_prefs.load(path.getPath());
    }

	void tick(gkScalar rate){}

    bool load()
    {
        gkBlendFile* pBlendFile = m_engine->loadBlendFile(m_blend);

		GK_ASSERT(pBlendFile);

        if (m_prefs.userWindow)
            m_engine->initializeWindow();

        gkSceneIterator scit = pBlendFile->getSceneIterator();

		GK_ASSERT(scit.hasMoreElements());

        gkScene* pScene = scit.peekNext();

		pScene->load();

		CreateLogicNodes(pScene);

        return true;
    }

	void CreateLogicNodes(gkScene* pScene)
	{
		m_tree = gkNodeManager::getSingleton().create();

		CreateCommonLogic();

		CreateExitLogic();

		CreateCameraLogic();

		CreateMomoLogic();

		CreateRatLogic();

		CreateDebugLogic();

		m_tree->solveOrder();

		gkGameObject* pCamera = pScene->getMainCamera();

		GK_ASSERT(pCamera);

		pCamera->attachLogic(m_tree);
	}

	void CreateNavigationNodes()
	{
		gkDetectMotionNode* detectMotion = m_tree->createNode<gkDetectMotionNode>();
		detectMotion->getUPDATE()->setValue(true);

		gkPulseNode* pulse = m_tree->createNode<gkPulseNode>();
		detectMotion->getNOT_DETECTED()->link(pulse->getUPDATE());

		gkNavMeshNode* staticNavMesh = m_tree->createNode<gkNavMeshNode>();
		staticNavMesh->getUPDATE()->link(pulse->getOUTPUT());

		gkKeyNode* fKeyNode = m_tree->createNode<gkKeyNode>();
		fKeyNode->setKey(KC_ZKEY);

		gkIfNode<bool, CMP_AND>* ifNode = m_tree->createNode<gkIfNode<bool, CMP_AND> >();
		ifNode->getA()->link(m_ctrlKeyNode->getIS_DOWN());
		ifNode->getB()->link(fKeyNode->getPRESS());

		gkObjNode* targetNode = m_tree->createNode<gkObjNode>();
		targetNode->setType(gkObjNode::SCREEN_XY);
		targetNode->getUPDATE_OBJ()->link(ifNode->getIS_TRUE());
		targetNode->getX()->link(m_mouseNode->getABS_X());
		targetNode->getY()->link(m_mouseNode->getABS_Y());

		gkFindPathNode* findPathNode = m_tree->createNode<gkFindPathNode>();

		{
			gkIfNode<bool, CMP_AND>* ifANode = m_tree->createNode<gkIfNode<bool, CMP_AND> >();
			ifANode->getA()->link(ifNode->getIS_TRUE());
			ifANode->getB()->link(targetNode->getHAS_OBJ());

			findPathNode->getUPDATE()->link(ifANode->getIS_TRUE());
		}
		findPathNode->getNAV_MESH()->link(staticNavMesh->getOUTPUT());
		findPathNode->getSTART_POS()->link(m_momoPlayer->getPOSITION());
		findPathNode->getEND_POS()->link(targetNode->getHIT_POINT());
		findPathNode->getSHOW_PATH()->link(m_xKeyNode->getIS_DOWN());
		findPathNode->getSHOW_PATH_OFFSET()->setValue(gkVector3(0, 0, 0.3f));

		m_momoFollowPathNode = m_tree->createNode<gkFollowPathNode>();
		m_momoFollowPathNode->getUPDATE()->link(findPathNode->getPATH_FOUND());
		m_momoFollowPathNode->getTARGET()->link(m_momoPlayer->getOBJ());
		m_momoFollowPathNode->getPATH()->link(findPathNode->getPATH());
		m_momoFollowPathNode->getCURRENT_STATE()->link(m_stateMachine->getFORCE_STATUS());
		m_momoFollowPathNode->setIdleState(momoState::IDLE_CAPOEIRA);
		m_momoFollowPathNode->setWalkState(momoState::WALK, momoVelocity::WALK);
		m_momoFollowPathNode->setRunState(momoState::RUN, momoVelocity::RUN);

		{
			// Pahfinding for Rat

			gkFindPathNode* ratFindPathNode = m_tree->createNode<gkFindPathNode>();

			{
				gkIfNode<bool, CMP_OR>* ifNode = m_tree->createNode<gkIfNode<bool, CMP_OR> >();
				ifNode->getA()->link(m_momoPlayer->getMOTION());
				ifNode->getB()->link(pulse->getOUTPUT());

				ratFindPathNode->getUPDATE()->link(ifNode->getIS_TRUE());
			}

			ratFindPathNode->getNAV_MESH()->link(staticNavMesh->getOUTPUT());
			ratFindPathNode->getSTART_POS()->link(m_ratPlayer->getPOSITION());
			ratFindPathNode->getEND_POS()->link(m_momoPlayer->getPOSITION());
			ratFindPathNode->getSHOW_PATH()->link(m_xKeyNode->getIS_DOWN());
			ratFindPathNode->getSHOW_PATH_OFFSET()->setValue(gkVector3(0, 0, 0.3f));

			gkFollowPathNode* ratFollowPathNode = m_tree->createNode<gkFollowPathNode>();
			ratFollowPathNode->getUPDATE()->link(ratFindPathNode->getPATH_FOUND());
			ratFollowPathNode->getTARGET()->link(m_ratPlayer->getOBJ());
			ratFollowPathNode->getSOURCE()->link(m_momoPlayer->getOBJ());
			ratFollowPathNode->getPATH()->link(ratFindPathNode->getPATH());
			ratFollowPathNode->getCURRENT_STATE()->link(m_stateRatMachine->getFORCE_STATUS());
			ratFollowPathNode->setIdleState(ratState::IDLE);
			ratFollowPathNode->setWalkState(ratState::WALK, ratVelocity::WALK);
			ratFollowPathNode->setRunState(ratState::RUN, ratVelocity::RUN);

		}
	}

	void CreateCommonLogic()
	{
		m_ctrlKeyNode = m_tree->createNode<gkKeyNode>();
		m_ctrlKeyNode->setKey(KC_LEFTCTRLKEY);

		m_shiftKeyNode = m_tree->createNode<gkKeyNode>();
		m_shiftKeyNode->setKey(KC_LEFTSHIFTKEY);

		m_wKeyNode = m_tree->createNode<gkKeyNode>();
		m_wKeyNode->setKey(KC_WKEY);

		m_sKeyNode = m_tree->createNode<gkKeyNode>();
		m_sKeyNode->setKey(KC_SKEY);

		m_cKeyNode = m_tree->createNode<gkKeyNode>();
		m_cKeyNode->setKey(KC_CKEY);

		m_xKeyNode = m_tree->createNode<gkKeyNode>();
		m_xKeyNode->setKey(KC_XKEY);

		m_mouseNode = m_tree->createNode<gkMouseNode>();

		m_leftMouseNode = m_tree->createNode<gkMouseButtonNode>();
		
		m_rightMouseNode = m_tree->createNode<gkMouseButtonNode>();
		m_rightMouseNode->setButton(gkMouse::Right);

		gkPulseNode* pulse = m_tree->createNode<gkPulseNode>();
		pulse->getUPDATE()->setValue(true);

		m_momoPlayer = m_tree->createNode<gkObjNode>();
		m_momoPlayer->getUPDATE_OBJ()->link(pulse->getOUTPUT());
		m_momoPlayer->getNAME()->setValue(object::PLAYER);

		m_ratPlayer = m_tree->createNode<gkObjNode>();
		m_ratPlayer->getUPDATE_OBJ()->link(pulse->getOUTPUT());
		m_ratPlayer->getNAME()->setValue(object::RAT_PLAYER);

		m_cameraPlayer = m_tree->createNode<gkObjNode>();
		m_cameraPlayer->getUPDATE_OBJ()->link(pulse->getOUTPUT());
		m_cameraPlayer->getNAME()->setValue(object::CAMERA);

		m_animNode = m_tree->createNode<gkAnimationNode>();

		m_animRatNode = m_tree->createNode<gkAnimationNode>();

		CreateCommomMomoCameraArcBallLogic();

		m_stateMachine = m_tree->createNode<gkStateMachineNode>();

		CreateCommonMomoGrabLogic();

		CreateMomoStateMachine();

		m_stateRatMachine = m_tree->createNode<gkStateMachineNode>();

		CreateRatStateMachine();

		CreateNavigationNodes();
	}

	void CreateCommomMomoCameraArcBallLogic()
	{
		m_momoCameraArcBall = m_tree->createNode<gkArcBallNode>();
		m_momoCameraArcBall->getCENTER_OBJ()->link(m_momoPlayer->getOBJ());
		m_momoCameraArcBall->getAVOID_BLOCKING()->setValue(true);

		m_momoCameraArcBall->getCENTER_POSITION()->link(m_momoPlayer->getPOSITION());
		m_momoCameraArcBall->getTARGET()->link(m_cameraPlayer->getOBJ());

		m_momoCameraArcBall->getREL_X()->link(m_mouseNode->getREL_X());
		m_momoCameraArcBall->getREL_Y()->link(m_mouseNode->getREL_Y());
		m_momoCameraArcBall->getREL_Z()->link(m_mouseNode->getWHEEL());

		m_momoCameraArcBall->getMIN_PITCH()->setValue(0);
		m_momoCameraArcBall->getMAX_PITCH()->setValue(90);

		m_momoCameraArcBall->getMIN_ROLL()->setValue(-180);
		m_momoCameraArcBall->getMAX_ROLL()->setValue(180);

		m_momoCameraArcBall->getMIN_Z()->setValue(0.5f);
		m_momoCameraArcBall->getMAX_Z()->setValue(10);

		m_momoCameraArcBall->getKEEP_DISTANCE()->setValue(true);

		m_momoCameraArcBall->getUPDATE()->link(m_ctrlKeyNode->getNOT_IS_DOWN());
	}

	void CreateMomoStateMachine()
	{
		m_stateMachine->getUPDATE()->setValue(true);

		// Initial state
		m_stateMachine->getCURRENT_STATE()->setValue(momoState::IDLE_NASTY); 

		// Transitions

		m_stateMachine->addTransition(momoState::CATCH, momoState::IDLE_NASTY);
		m_stateMachine->addTransition(momoState::CATCH_ENDED, momoState::IDLE_NASTY);
		m_stateMachine->addTransition(momoState::CARRY, momoState::IDLE_NASTY);
		m_stateMachine->addTransition(momoState::THROW_WITH, momoState::IDLE_NASTY);
		m_stateMachine->addTransition(momoState::WALK, momoState::IDLE_NASTY);
		m_stateMachine->addTransition(momoState::WALK_BACK, momoState::IDLE_NASTY);

		gkFallTestNode* fallTest = m_tree->createNode<gkFallTestNode>();
		fallTest->getENABLE()->setValue(true);
		fallTest->getTARGET()->link(m_momoPlayer->getOBJ());
	
		m_wKeyNode->getIS_DOWN()->link(m_stateMachine->addTransition(momoState::IDLE_NASTY, momoState::WALK));
		m_wKeyNode->getIS_DOWN()->link(m_stateMachine->addTransition(momoState::IDLE_CAPOEIRA, momoState::WALK));
		m_wKeyNode->getIS_DOWN()->link(m_stateMachine->addTransition(momoState::WALK, momoState::WALK));
		fallTest->getFALLING()->link(m_stateMachine->addTransition(momoState::WALK, momoState::FALL_UP));
		fallTest->getFALLING()->link(m_stateMachine->addTransition(momoState::FALL_UP, momoState::FALL_UP));
		fallTest->getNOT_FALLING()->link(m_stateMachine->addTransition(momoState::FALL_UP, momoState::IDLE_NASTY));
		m_wKeyNode->getIS_DOWN()->link(m_stateMachine->addTransition(momoState::WALK, momoState::RUN, 1500));
		m_wKeyNode->getIS_DOWN()->link(m_stateMachine->addTransition(momoState::RUN, momoState::RUN));
		fallTest->getFALLING()->link(m_stateMachine->addTransition(momoState::RUN, momoState::FALL_UP));
		m_stateMachine->addTransition(momoState::RUN, momoState::WALK);

		{
			gkIfNode<bool, CMP_AND>* ifNode = m_tree->createNode<gkIfNode<bool, CMP_AND> >();
			ifNode->getA()->link(m_shiftKeyNode->getIS_DOWN());
			ifNode->getB()->link(m_wKeyNode->getIS_DOWN());
			
			ifNode->getIS_TRUE()->link(m_stateMachine->addTransition(momoState::RUN, momoState::RUN_FASTER));
			ifNode->getIS_TRUE()->link(m_stateMachine->addTransition(momoState::RUN_FASTER, momoState::RUN_FASTER));
			fallTest->getFALLING()->link(m_stateMachine->addTransition(momoState::RUN_FASTER, momoState::FALL_UP));
		}

		m_stateMachine->addTransition(momoState::RUN_FASTER, momoState::RUN);
		m_stateMachine->addTransition(momoState::RUN, momoState::WALK);
		m_sKeyNode->getIS_DOWN()->link(m_stateMachine->addTransition(momoState::RUN, momoState::WALK_BACK));


		m_sKeyNode->getIS_DOWN()->link(m_stateMachine->addTransition(momoState::IDLE_NASTY, momoState::WALK_BACK));
		m_sKeyNode->getIS_DOWN()->link(m_stateMachine->addTransition(momoState::IDLE_CAPOEIRA, momoState::WALK_BACK));
		m_sKeyNode->getIS_DOWN()->link(m_stateMachine->addTransition(momoState::WALK_BACK, momoState::WALK_BACK));
		fallTest->getFALLING()->link(m_stateMachine->addTransition(momoState::WALK_BACK, momoState::FALL_UP));


		{
			gkIfNode<bool, CMP_AND>* ifNode = m_tree->createNode<gkIfNode<bool, CMP_AND> >();
			ifNode->getA()->link(m_ctrlKeyNode->getNOT_IS_DOWN());
			ifNode->getB()->link(m_rightMouseNode->getPRESS());

			ifNode->getIS_TRUE()->link(m_stateMachine->addTransition(momoState::IDLE_NASTY, momoState::CATCH));
			ifNode->getIS_TRUE()->link(m_stateMachine->addTransition(momoState::IDLE_CAPOEIRA, momoState::CATCH));
			m_animNode->getNOT_HAS_REACHED_END()->link(m_stateMachine->addTransition(momoState::CATCH, momoState::CATCH));
			m_animNode->getHAS_REACHED_END()->link(m_stateMachine->addTransition(momoState::CATCH, momoState::CATCH_ENDED));
			m_momoGrab->getCAUGHT_TRUE()->link(m_stateMachine->addTransition(momoState::CATCH_ENDED, momoState::CARRY));
			m_momoGrab->getCAUGHT_TRUE()->link(m_stateMachine->addTransition(momoState::CARRY, momoState::CARRY));

			{
				gkIfNode<bool, CMP_AND>* ifNode = m_tree->createNode<gkIfNode<bool, CMP_AND> >();

				ifNode->getA()->link(m_leftMouseNode->getPRESS());
				ifNode->getB()->link(m_ctrlKeyNode->getNOT_IS_DOWN());

				ifNode->getIS_TRUE()->link(m_stateMachine->addTransition(momoState::CARRY, momoState::THROW_WITH));
			}
			m_animNode->getNOT_HAS_REACHED_END()->link(m_stateMachine->addTransition(momoState::THROW_WITH, momoState::THROW_WITH));
		}

		m_stateMachine->addTransition(momoState::IDLE_NASTY, momoState::IDLE_CAPOEIRA, 70000);
		m_stateMachine->addTransition(momoState::IDLE_CAPOEIRA, momoState::IDLE_NASTY, 11000);
	}

	void CreateRatStateMachine()
	{
		m_stateRatMachine->getUPDATE()->setValue(true);

		// Initial state
		m_stateRatMachine->getCURRENT_STATE()->setValue(ratState::IDLE); 
	}

	void CreateMomoLogic()
	{
		CreateMomoAnimationLogic();

		CreateMomoMoveLogic();

		CreateMomoLoadUnloadLogic();

		CreateMomoDustTrailLogic();
	}

	void CreateRatLogic()
	{
		CreateRatAnimationLogic();
	}

	void CreateCommonMomoGrabLogic()
	{
		m_momoGrab = m_tree->createNode<gkGrabNode>();
		m_momoGrab->getUPDATE()->setValue(true);
		m_momoGrab->getTARGET()->link(m_momoPlayer->getOBJ());
		m_momoGrab->getGRAB_DIRECTION()->setValue(gkVector3(0, 0.6f, 0));
		m_momoGrab->getTHROW_VEL()->setValue(gkVector3(0, 20.5f, 0));
		m_momoGrab->getRELATED_OFFSET_POSITION()->setValue(gkVector3(0, 0, 1));

		{
			gkIfNode<bool, CMP_AND>* ifNode = m_tree->createNode<gkIfNode<bool, CMP_AND> >();

			ifNode->getA()->link(m_rightMouseNode->getPRESS());
			ifNode->getB()->link(m_ctrlKeyNode->getNOT_IS_DOWN());

			m_momoGrab->getCREATE_PICK()->link(ifNode->getIS_TRUE());
		}

		{
			gkIfNode<bool, CMP_AND>* ifNode = m_tree->createNode<gkIfNode<bool, CMP_AND> >();

			ifNode->getA()->link(m_rightMouseNode->getRELEASE());
			ifNode->getB()->link(m_ctrlKeyNode->getNOT_IS_DOWN());

			m_momoGrab->getRELEASE_PICK()->link(ifNode->getIS_TRUE());
		}

		{
			gkIfNode<int, CMP_EQUALS>* equalThrowWithNode = m_tree->createNode<gkIfNode<int, CMP_EQUALS> >();

			equalThrowWithNode->getA()->setValue(momoState::THROW_WITH);
			equalThrowWithNode->getB()->link(m_stateMachine->getCURRENT_STATE());

			gkIfNode<gkScalar, CMP_GREATER>* throwTimePositionNode = m_tree->createNode<gkIfNode<gkScalar, CMP_GREATER> >();

			throwTimePositionNode->getA()->link(m_animNode->getTIME_POSITION());
			throwTimePositionNode->getB()->setValue(20.13f);

			gkIfNode<bool, CMP_AND>* ifNode = m_tree->createNode<gkIfNode<bool, CMP_AND> >();
			ifNode->getA()->link(equalThrowWithNode->getIS_TRUE());
			ifNode->getB()->link(throwTimePositionNode->getIS_TRUE());

			m_momoGrab->getTHROW_OBJECT()->link(ifNode->getIS_TRUE());
		}
	}

	void CreateMomoMoveLogic()
	{
		typedef gkMapNode<int, gkScalar> MAP_NODE;
		
		MAP_NODE* mapNode = m_tree->createNode< MAP_NODE >();
		mapNode->getINPUT()->link(m_stateMachine->getCURRENT_STATE());
		
		MAP_NODE::MAP mapping;
		mapping[momoState::WALK] = momoVelocity::WALK;
		mapping[momoState::RUN] = momoVelocity::RUN;
		mapping[momoState::RUN_FASTER] = momoVelocity::RUN_FASTER;
		mapping[momoState::WALK_BACK] = momoVelocity::WALK_BACK;

		mapNode->getMAPPING()->setValue(mapping);

		gkIfNode<bool, CMP_AND>* ifNode = m_tree->createNode<gkIfNode<bool, CMP_AND> >();
		ifNode->getA()->link(m_momoFollowPathNode->getHAS_REACHED_END());
		ifNode->getB()->link(mapNode->getHAS_OUTPUT());

		{
			// orient Momo
			m_momoPlayer->getSET_ROTATION()->link(ifNode->getIS_TRUE());
			m_momoPlayer->getSET_ROTATION_VALUE()->link(m_momoCameraArcBall->getCURRENT_ROLL());
		}

		{
			m_momoPlayer->getSET_LINEAR_VEL()->link(ifNode->getIS_TRUE());
			m_momoPlayer->getSET_LINEAR_VEL_VALUE_X()->setValue(0);
			m_momoPlayer->getSET_LINEAR_VEL_VALUE_Y()->link(mapNode->getOUTPUT());
			m_momoPlayer->getSET_LINEAR_VEL_VALUE_Z()->setValue(0);
		}
	}

	void CreateMomoDustTrailLogic()
	{
		{
			gkIfNode<gkString, CMP_EQUALS>* equalNode = m_tree->createNode<gkIfNode<gkString, CMP_EQUALS> >();

			equalNode->getA()->setValue(momoAnimation::RUN_FASTER);
			equalNode->getB()->link(m_animNode->getCURRENT_ANIM_NAME());

			CreateMomoDustTrailLogic(particle::DUST_RUN, equalNode->getIS_TRUE());
		}

		/*
		{
			gkStringEqualNode* equalNode = m_tree->createNode<gkStringEqualNode>();

			equalNode->getA()->setValue(momoAnimation::WALK_BACK);
			equalNode->getB()->link(m_animNode->getCurrentAnimName());

			CreateMomoDustTrailLogic(particle::DUST_WALK, equalNode->getTrue());
		}
		*/
	}

	void CreateMomoDustTrailLogic(const gkString& name, gkILogicSocket* pUpdateSocket)
	{
		gkParticleNode* particle = m_tree->createNode<gkParticleNode>();
		particle->getPARTICLE_SYSTEM_NAME()->setValue(name);
		particle->getCREATE()->link(pUpdateSocket);
		particle->getPOSITION()->link(m_momoPlayer->getPOSITION());
		particle->getORIENTATION()->link(m_momoPlayer->getROTATION());
	}

	void CreateMomoLoadUnloadLogic()
	{
		// reload

		gkKeyNode* rKeyNode = m_tree->createNode<gkKeyNode>();
		rKeyNode->setKey(KC_RKEY);

		m_momoPlayer->getRELOAD()->link(rKeyNode->getPRESS());

		// unload

		gkKeyNode* uKeyNode = m_tree->createNode<gkKeyNode>();
		uKeyNode->setKey(KC_UKEY);

		m_momoPlayer->getUNLOAD()->link(uKeyNode->getPRESS());

		// load

		gkKeyNode* lKeyNode = m_tree->createNode<gkKeyNode>();
		lKeyNode->setKey(KC_LKEY);

		m_momoPlayer->getLOAD()->link(lKeyNode->getPRESS());
	}

	void CreateMomoAnimationLogic()
	{
		typedef gkMapNode<int, gkString> MAP_NODE;
		
		MAP_NODE* mapNode = m_tree->createNode< MAP_NODE >();
		mapNode->getINPUT()->link(m_stateMachine->getCURRENT_STATE());
		
		MAP_NODE::MAP mapping;
		mapping[momoState::CARRY] = momoAnimation::CARRY;
		mapping[momoState::CATCH] = momoAnimation::CATCH;
		mapping[momoState::CATCH_ENDED] = momoAnimation::CATCH;
		mapping[momoState::THROW_WITH] = momoAnimation::THROW_WITH;
		mapping[momoState::WALK] = momoAnimation::WALK;
		mapping[momoState::RUN] = momoAnimation::RUN;
		mapping[momoState::RUN_FASTER] = momoAnimation::RUN_FASTER;
		mapping[momoState::WALK_BACK] = momoAnimation::WALK_BACK;
		mapping[momoState::IDLE_NASTY] = momoAnimation::IDLE_NASTY;
		mapping[momoState::IDLE_CAPOEIRA] = momoAnimation::IDLE_CAPOEIRA;
		mapping[momoState::FALL_UP] = momoAnimation::FALL_UP;

		mapNode->getMAPPING()->setValue(mapping);

		m_animNode->getTARGET()->link(m_momoPlayer->getMESH_OBJ());
		m_animNode->getANIM_NAME()->link(mapNode->getOUTPUT());
	}

	void CreateRatAnimationLogic()
	{
		typedef gkMapNode<int, gkString> MAP_NODE;
		
		MAP_NODE* mapNode = m_tree->createNode< MAP_NODE >();
		mapNode->getINPUT()->link(m_stateRatMachine->getCURRENT_STATE());
		
		MAP_NODE::MAP mapping;
		mapping[ratState::IDLE] = ratAnimation::IDLE;
		mapping[ratState::WALK] = ratAnimation::WALK;
		mapping[ratState::RUN] = ratAnimation::RUN;

		mapNode->getMAPPING()->setValue(mapping);

		m_animRatNode->getTARGET()->link(m_ratPlayer->getMESH_OBJ());
		m_animRatNode->getANIM_NAME()->link(mapNode->getOUTPUT());
	}


	void CreateCameraLogic()
	{
        CreateCursorCameraArcBallLogic();

		CreateCursorLogic();

		CreatePickLogic();
	}

	void CreateExitLogic()
	{
		gkExitNode* exit = m_tree->createNode<gkExitNode>();

		gkKeyNode* key = m_tree->createNode<gkKeyNode>();

		key->setKey(KC_ESCKEY);

		exit->getEXIT()->link(key->getPRESS());
	}

	void CreateCursorLogic()
	{
		gkCursorNode* cursor = m_tree->createNode<gkCursorNode>();

		cursor->getENABLE()->link(m_ctrlKeyNode->getIS_DOWN());
		cursor->getUPDATE()->link(m_mouseNode->getMOTION());
		cursor->getX()->link(m_mouseNode->getABS_X());
		cursor->getY()->link(m_mouseNode->getABS_Y());
		cursor->getMATERIAL_NAME()->setValue(material::ARROW_CURSOR);
	}

	void CreatePickLogic()
	{
		gkPickNode* pick = m_tree->createNode<gkPickNode>();

		pick->getUPDATE()->link(m_ctrlKeyNode->getIS_DOWN());
		pick->getCREATE_PICK()->link(m_rightMouseNode->getPRESS());
		pick->getRELEASE_PICK()->link(m_rightMouseNode->getRELEASE());
		pick->getX()->link(m_mouseNode->getABS_X());
		pick->getY()->link(m_mouseNode->getABS_Y());
	}

	void CreateCursorCameraArcBallLogic()
	{
		gkObjNode* centerObj = m_tree->createNode<gkObjNode>();
		centerObj->setType(gkObjNode::SCREEN_XY);
		centerObj->getX()->link(m_mouseNode->getABS_X());
		centerObj->getY()->link(m_mouseNode->getABS_Y());

		centerObj->getRESET()->link(m_ctrlKeyNode->getRELEASE());
		{
			gkIfNode<bool, CMP_AND>* ifNode = m_tree->createNode<gkIfNode<bool, CMP_AND> >();

			ifNode->getA()->link(m_leftMouseNode->getPRESS());
			ifNode->getB()->link(m_ctrlKeyNode->getIS_DOWN());

			centerObj->getUPDATE_OBJ()->link(ifNode->getIS_TRUE());
		}

		gkArcBallNode* arcBall = m_tree->createNode<gkArcBallNode>();
		arcBall->getCENTER_OBJ()->link(centerObj->getOBJ());
		arcBall->getCENTER_POSITION()->link(centerObj->getHIT_POINT());
		arcBall->getTARGET()->link(m_cameraPlayer->getOBJ());

		arcBall->getREL_X()->link(m_mouseNode->getREL_X());
		arcBall->getREL_Y()->link(m_mouseNode->getREL_Y());
		arcBall->getREL_Z()->link(m_mouseNode->getWHEEL());

		arcBall->getMIN_PITCH()->setValue(-90);
		arcBall->getMAX_PITCH()->setValue(90);

		arcBall->getMIN_ROLL()->setValue(-180);
		arcBall->getMAX_ROLL()->setValue(180);


		{
			gkIfNode<bool, CMP_AND>* ifANode = m_tree->createNode<gkIfNode<bool, CMP_AND> >();
			ifANode->getA()->link(m_ctrlKeyNode->getIS_DOWN());
			ifANode->getB()->link(m_leftMouseNode->getIS_DOWN());

			gkIfNode<bool, CMP_AND>* ifBNode = m_tree->createNode<gkIfNode<bool, CMP_AND> >();
			ifBNode->getA()->link(m_ctrlKeyNode->getIS_DOWN());
			ifBNode->getB()->link(m_mouseNode->getWHEEL_MOTION());

			gkIfNode<bool, CMP_OR>* ifNode = m_tree->createNode<gkIfNode<bool, CMP_OR> >();
			ifNode->getA()->link(ifANode->getIS_TRUE());
			ifNode->getB()->link(ifBNode->getIS_TRUE());

			arcBall->getUPDATE()->link(ifNode->getIS_TRUE());
		}
	}

	void CreateDebugLogic()
	{
		gkShowPhysicsNode* showPhysics = m_tree->createNode<gkShowPhysicsNode>();
		showPhysics->getENABLE()->link(m_cKeyNode->getIS_DOWN());
	}

private:

	gkString m_blend;

	gkLogicTree* m_tree;

	gkKeyNode* m_ctrlKeyNode;

	gkKeyNode* m_shiftKeyNode;

	gkKeyNode* m_wKeyNode; 
	
	gkKeyNode* m_sKeyNode;

	gkKeyNode* m_cKeyNode;

	gkKeyNode* m_xKeyNode;

	gkMouseNode* m_mouseNode;

	gkMouseButtonNode* m_leftMouseNode;

	gkMouseButtonNode* m_rightMouseNode;

	gkObjNode* m_momoPlayer;

	gkObjNode* m_ratPlayer;
	
	gkObjNode* m_cameraPlayer;

	gkArcBallNode* m_momoCameraArcBall;

	gkAnimationNode* m_animNode;

	gkAnimationNode* m_animRatNode;

	gkGrabNode* m_momoGrab;

	gkStateMachineNode* m_stateMachine;

	gkStateMachineNode* m_stateRatMachine;

	gkFollowPathNode* m_momoFollowPathNode;
};

int main(int argc, char **argv)
{
	// See ReadMe.txt for how to use this demo

    TestMemory;

	// This demo only works with momo_ogre_plus.blend file because it contains
	// (Ogre) material definitions and (Ogre) particle definitions inside blender's TX blocks.
	// Also logic is making reference to concrete objects inside momo_ogre_plus.blend.

	// If you want to avoid blender's TX blocks in order to make (Ogre) materials and particles
	// generic to all blender files then move them outside and load as standard Ogre resources...
	// (Use OgreKitResource.cfg)
    OgreKit okit(gkUtils::getFile("./data/momo_ogre_plus.blend"));
    okit.run();
    return 0;
}

