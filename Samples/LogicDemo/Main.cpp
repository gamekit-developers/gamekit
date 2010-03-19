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
		IDLE_CAPOEIRA
	};
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

namespace object
{
	gkString PLAYER = "Player";
	gkString MESH_MOMO = "MeshMomo";
	gkString CAMERA = "View";
	gkString PLANE = "Plane";
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
		m_wKeyNode(0), m_sKeyNode(0), m_mouseNode(0), m_leftMouseNode(0), m_rightMouseNode(0),
		m_playerSetter(0), m_meshMomoSetter(0), m_cameraSetter(0), m_animNode(0), m_momoCameraArcBall(0),
		m_momoGrab(0), m_stateMachine(0)
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

		CreateDebugLogic();
		
		m_tree->solveOrder();

		gkGameObject* pCamera = pScene->getMainCamera();

		GK_ASSERT(pCamera);

		pCamera->attachLogic(m_tree);
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

		m_mouseNode = m_tree->createNode<gkMouseNode>();

		m_leftMouseNode = m_tree->createNode<gkMouseButtonNode>();
		
		m_rightMouseNode = m_tree->createNode<gkMouseButtonNode>();
		m_rightMouseNode->setButton(gkMouse::Right);

		gkPulseNode* pulse =  m_tree->createNode<gkPulseNode>();
		pulse->getUpdate()->setValue(true);

		m_playerSetter = m_tree->createNode<gkObjectSetterNode>();
		m_playerSetter->getUpdate()->link(pulse->getOutput());
		m_playerSetter->getInput()->setValue(object::PLAYER);

		gkDisableDeactivationNode* disableDeactivationForMomo = m_tree->createNode<gkDisableDeactivationNode>();
		disableDeactivationForMomo->getUpdate()->link(pulse->getOutput());
		disableDeactivationForMomo->getTarget()->link(m_playerSetter->getOutput());

		m_meshMomoSetter = m_tree->createNode<gkObjectSetterNode>();
		m_meshMomoSetter->getUpdate()->link(pulse->getOutput());
		m_meshMomoSetter->getInput()->setValue(object::MESH_MOMO);

		m_cameraSetter = m_tree->createNode<gkObjectSetterNode>();
		m_cameraSetter->getUpdate()->link(pulse->getOutput());
		m_cameraSetter->getInput()->setValue(object::CAMERA);

		m_animNode = m_tree->createNode<gkAnimationNode>();

		CreateCommomMomoCameraArcBallLogic();

		CreateCommonMomoGrabLogic();

		CreateMomoStateMachine();
	}

	void CreateCommomMomoCameraArcBallLogic()
	{
		gkPositionSetterNode* posNode = m_tree->createNode<gkPositionSetterNode>();
		posNode->getInput()->link(m_playerSetter->getOutput());
		posNode->getUpdate()->setValue(true);

		m_momoCameraArcBall = m_tree->createNode<gkArcBallNode>();
		m_momoCameraArcBall->getCenterObj()->link(m_playerSetter->getOutput());

		m_momoCameraArcBall->getCenterPosition()->link(posNode->getOutput());
		m_momoCameraArcBall->getTarget()->link(m_cameraSetter->getOutput());

		m_momoCameraArcBall->getRelX()->link(m_mouseNode->getRelX());
		m_momoCameraArcBall->getRelY()->link(m_mouseNode->getRelY());
		m_momoCameraArcBall->getRelZ()->link(m_mouseNode->getWheel());

		m_momoCameraArcBall->getMinPitch()->setValue(0);
		m_momoCameraArcBall->getMaxPitch()->setValue(90);

		m_momoCameraArcBall->getMinRoll()->setValue(-180);
		m_momoCameraArcBall->getMaxRoll()->setValue(180);

		m_momoCameraArcBall->getMinZ()->setValue(0.5f);
		m_momoCameraArcBall->getMaxZ()->setValue(10);

		m_momoCameraArcBall->getKeepDistance()->setValue(true);

		m_momoCameraArcBall->getUpdate()->link(m_ctrlKeyNode->getNotIsDown());
	}

	void CreateMomoStateMachine()
	{
		m_stateMachine = m_tree->createNode<gkStateMachineNode>();
		m_stateMachine->getUpdate()->setValue(true);

		// Initial state
		m_stateMachine->getCurrentState()->setValue(momoState::IDLE_NASTY); 

		// Transitions

		m_stateMachine->addTransition(momoState::CATCH, momoState::IDLE_NASTY);
		m_stateMachine->addTransition(momoState::CATCH_ENDED, momoState::IDLE_NASTY);
		m_stateMachine->addTransition(momoState::CARRY, momoState::IDLE_NASTY);
		m_stateMachine->addTransition(momoState::THROW_WITH, momoState::IDLE_NASTY);
		m_stateMachine->addTransition(momoState::WALK, momoState::IDLE_NASTY);
		m_stateMachine->addTransition(momoState::WALK_BACK, momoState::IDLE_NASTY);

	
		m_wKeyNode->getIsDown()->link(m_stateMachine->addTransition(momoState::IDLE_NASTY, momoState::WALK));
		m_wKeyNode->getIsDown()->link(m_stateMachine->addTransition(momoState::IDLE_CAPOEIRA, momoState::WALK));
		m_wKeyNode->getIsDown()->link(m_stateMachine->addTransition(momoState::WALK, momoState::WALK));
		m_wKeyNode->getIsDown()->link(m_stateMachine->addTransition(momoState::WALK, momoState::RUN, 1500));
		m_wKeyNode->getIsDown()->link(m_stateMachine->addTransition(momoState::RUN, momoState::RUN));
		m_stateMachine->addTransition(momoState::RUN, momoState::WALK);

		{
			gkIfNode* ifNode = m_tree->createNode<gkIfNode>();
			ifNode->setStatement(CMP_AND);
			ifNode->getA()->link(m_shiftKeyNode->getIsDown());
			ifNode->getB()->link(m_wKeyNode->getIsDown());
			
			ifNode->getTrue()->link(m_stateMachine->addTransition(momoState::RUN, momoState::RUN_FASTER));
			ifNode->getTrue()->link(m_stateMachine->addTransition(momoState::RUN_FASTER, momoState::RUN_FASTER));
		}

		m_stateMachine->addTransition(momoState::RUN_FASTER, momoState::RUN);
		m_stateMachine->addTransition(momoState::RUN, momoState::WALK);
		m_sKeyNode->getIsDown()->link(m_stateMachine->addTransition(momoState::RUN, momoState::WALK_BACK));


		m_sKeyNode->getIsDown()->link(m_stateMachine->addTransition(momoState::IDLE_NASTY, momoState::WALK_BACK));
		m_sKeyNode->getIsDown()->link(m_stateMachine->addTransition(momoState::IDLE_CAPOEIRA, momoState::WALK_BACK));
		m_sKeyNode->getIsDown()->link(m_stateMachine->addTransition(momoState::WALK_BACK, momoState::WALK_BACK));


		{
			gkIfNode* ifNode = m_tree->createNode<gkIfNode>();
			ifNode->setStatement(CMP_AND);
			ifNode->getA()->link(m_ctrlKeyNode->getNotIsDown());
			ifNode->getB()->link(m_rightMouseNode->getPress());

			ifNode->getTrue()->link(m_stateMachine->addTransition(momoState::IDLE_NASTY, momoState::CATCH));
			ifNode->getTrue()->link(m_stateMachine->addTransition(momoState::IDLE_CAPOEIRA, momoState::CATCH));
			m_animNode->getNotHasReachedEnd()->link(m_stateMachine->addTransition(momoState::CATCH, momoState::CATCH));
			m_animNode->getHasReachedEnd()->link(m_stateMachine->addTransition(momoState::CATCH, momoState::CATCH_ENDED));
			m_momoGrab->getCaughtTrue()->link(m_stateMachine->addTransition(momoState::CATCH_ENDED, momoState::CARRY));
			m_momoGrab->getCaughtTrue()->link(m_stateMachine->addTransition(momoState::CARRY, momoState::CARRY));
			m_momoGrab->getThrowed()->link(m_stateMachine->addTransition(momoState::CARRY, momoState::THROW_WITH));
			m_animNode->getNotHasReachedEnd()->link(m_stateMachine->addTransition(momoState::THROW_WITH, momoState::THROW_WITH));
		}

		m_stateMachine->addTransition(momoState::IDLE_NASTY, momoState::IDLE_CAPOEIRA, 15000);
		m_stateMachine->addTransition(momoState::IDLE_CAPOEIRA, momoState::IDLE_NASTY, 10000);
	}

	void CreateMomoLogic()
	{
		CreateMomoAnimationLogic();

		CreateMomoMoveLogic();

		CreateMomoLoadUnloadLogic();

		CreateMomoDustTrailLogic();
	}

	void CreateCommonMomoGrabLogic()
	{
		m_momoGrab = m_tree->createNode<gkGrabNode>();
		m_momoGrab->getUpdate()->setValue(true);
		m_momoGrab->getTarget()->link(m_playerSetter->getOutput());
		m_momoGrab->getGrabDirection()->setValue(gkVector3(0, 0.6f, 0));
		m_momoGrab->getThrowVelocity()->setValue(gkVector3(0, 20.5f, 0));
		m_momoGrab->getOffsetPosition()->setValue(gkVector3(0, 0, 1));

		{
			gkIfNode* ifNode = m_tree->createNode<gkIfNode>();
			ifNode->setStatement(CMP_AND);

			ifNode->getA()->link(m_rightMouseNode->getPress());
			ifNode->getB()->link(m_ctrlKeyNode->getNotIsDown());

			m_momoGrab->getCreatePick()->link(ifNode->getTrue());
		}

		{
			gkIfNode* ifNode = m_tree->createNode<gkIfNode>();
			ifNode->setStatement(CMP_AND);

			ifNode->getA()->link(m_rightMouseNode->getRelease());
			ifNode->getB()->link(m_ctrlKeyNode->getNotIsDown());

			m_momoGrab->getReleasePick()->link(ifNode->getTrue());
		}

		{
			gkIfNode* ifNode = m_tree->createNode<gkIfNode>();
			ifNode->setStatement(CMP_AND);

			ifNode->getA()->link(m_leftMouseNode->getPress());
			ifNode->getB()->link(m_ctrlKeyNode->getNotIsDown());

			m_momoGrab->getThrowObject()->link(ifNode->getTrue());
		}
	}

	void CreateMomoMoveLogic()
	{
		gkIntEqualNode* equalWalkNode = m_tree->createNode<gkIntEqualNode>();

		equalWalkNode->getA()->setValue(momoState::WALK);
		equalWalkNode->getB()->link(m_stateMachine->getCurrentState());

		gkIntEqualNode* equalRunNode = m_tree->createNode<gkIntEqualNode>();

		equalRunNode->getA()->setValue(momoState::RUN);
		equalRunNode->getB()->link(m_stateMachine->getCurrentState());

		gkIntEqualNode* equalRunFasterNode = m_tree->createNode<gkIntEqualNode>();

		equalRunFasterNode->getA()->setValue(momoState::RUN_FASTER);
		equalRunFasterNode->getB()->link(m_stateMachine->getCurrentState());

		gkIntEqualNode* equalWalkBackNode = m_tree->createNode<gkIntEqualNode>();

		equalWalkBackNode->getA()->setValue(momoState::WALK_BACK);
		equalWalkBackNode->getB()->link(m_stateMachine->getCurrentState());


		{
			// orient Momo

			gkIfNode* ifANode = m_tree->createNode<gkIfNode>();
			ifANode->setStatement(CMP_OR);
			ifANode->getA()->link(equalWalkNode->getTrue());
			ifANode->getB()->link(equalWalkBackNode->getTrue());

			gkIfNode* ifBNode = m_tree->createNode<gkIfNode>();
			ifBNode->setStatement(CMP_OR);

			ifBNode->getA()->link(equalRunNode->getTrue());
			ifBNode->getB()->link(equalRunFasterNode->getTrue());

			gkIfNode* ifNode = m_tree->createNode<gkIfNode>();
			ifNode->setStatement(CMP_OR);
			ifNode->getA()->link(ifANode->getTrue());
			ifNode->getB()->link(ifBNode->getTrue());

			gkSetOrientationNode* orientation = m_tree->createNode<gkSetOrientationNode>();

			orientation->getUpdate()->link(ifNode->getTrue());
			orientation->getInput()->link(m_momoCameraArcBall->getRoll());
			orientation->getTarget()->link(m_playerSetter->getOutput());
		}

		{
			gkLinearVelNode* motion = m_tree->createNode<gkLinearVelNode>();

			motion->getUpdate()->link(equalWalkNode->getTrue());
			motion->getX()->setValue(0);
			motion->getY()->setValue(1);
			motion->getZ()->setValue(0);
			motion->getTarget()->link(m_playerSetter->getOutput());
		}

		{
			gkLinearVelNode* motion = m_tree->createNode<gkLinearVelNode>();

			motion->getUpdate()->link(equalRunNode->getTrue());
			motion->getX()->setValue(0);
			motion->getY()->setValue(2.5f);
			motion->getZ()->setValue(0);
			motion->getTarget()->link(m_playerSetter->getOutput());
		}

		{
			gkLinearVelNode* motion = m_tree->createNode<gkLinearVelNode>();

			motion->getUpdate()->link(equalRunFasterNode->getTrue());
			motion->getX()->setValue(0);
			motion->getY()->setValue(4.0f);
			motion->getZ()->setValue(0);
			motion->getTarget()->link(m_playerSetter->getOutput());
		}

		{
			gkLinearVelNode* motion = m_tree->createNode<gkLinearVelNode>();

			motion->getUpdate()->link(equalWalkBackNode->getTrue());
			motion->getX()->setValue(0);
			motion->getY()->setValue(-1);
			motion->getZ()->setValue(0);
			motion->getTarget()->link(m_playerSetter->getOutput());
		}
	}

	void CreateMomoDustTrailLogic()
	{
		{
			gkStringEqualNode* equalNode = m_tree->createNode<gkStringEqualNode>();

			equalNode->getA()->setValue(momoAnimation::RUN_FASTER);
			equalNode->getB()->link(m_animNode->getCurrentAnimName());

			CreateMomoDustTrailLogic(particle::DUST_RUN, equalNode->getTrue());
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

	void CreateMomoDustTrailLogic(const gkString& name, gkLogicSocket* pUpdateSocket)
	{
		gkOrientationSetterNode* orientation = m_tree->createNode<gkOrientationSetterNode>();
		orientation->getInput()->link(m_playerSetter->getOutput());
		orientation->getUpdate()->link(pUpdateSocket);

		gkCollisionNode* collision = m_tree->createNode<gkCollisionNode>();
		collision->getEnable()->setValue(true);
		collision->getCollidesWith()->setValue(object::PLANE);
		collision->getTarget()->link(m_playerSetter->getOutput());

		gkIfNode* ifNode = m_tree->createNode<gkIfNode>();
		ifNode->setStatement(CMP_AND);

		collision->getHasCollided()->link(ifNode->getA());
		ifNode->getB()->link(pUpdateSocket);

		gkParticleNode* particle = m_tree->createNode<gkParticleNode>();
		particle->getParticleSystemName()->setValue(name);
		particle->getCreate()->link(ifNode->getTrue());
		particle->getPosition()->link(collision->getContactPosition());
		particle->getOrientation()->link(orientation->getOutput());
	}

	void CreateMomoLoadUnloadLogic()
	{
		{
			// reload

			gkKeyNode* rKeyNode = m_tree->createNode<gkKeyNode>();
			rKeyNode->setKey(KC_RKEY);

			gkReloadNode* reloadPlayerNode = m_tree->createNode<gkReloadNode>();
			reloadPlayerNode->getTarget()->link(m_playerSetter->getOutput());
			reloadPlayerNode->getUpdate()->link(rKeyNode->getPress());

			gkReloadNode* reloadMeshMomoNode = m_tree->createNode<gkReloadNode>();
			reloadMeshMomoNode->getTarget()->link(m_meshMomoSetter->getOutput());
			reloadMeshMomoNode->getUpdate()->link(rKeyNode->getPress());
		}

		{
			// unload

			gkKeyNode* uKeyNode = m_tree->createNode<gkKeyNode>();
			uKeyNode->setKey(KC_UKEY);

			gkUnloadNode* unloadPlayerNode = m_tree->createNode<gkUnloadNode>();
			unloadPlayerNode->getTarget()->link(m_playerSetter->getOutput());
			unloadPlayerNode->getUpdate()->link(uKeyNode->getPress());

			gkUnloadNode* unloadMeshMomoNode = m_tree->createNode<gkUnloadNode>();
			unloadMeshMomoNode->getTarget()->link(m_meshMomoSetter->getOutput());
			unloadMeshMomoNode->getUpdate()->link(uKeyNode->getPress());
		}

		{
			// load

			gkKeyNode* lKeyNode = m_tree->createNode<gkKeyNode>();
			lKeyNode->setKey(KC_LKEY);

			gkLoadNode* loadPlayerNode = m_tree->createNode<gkLoadNode>();
			loadPlayerNode->getTarget()->link(m_playerSetter->getOutput());
			loadPlayerNode->getUpdate()->link(lKeyNode->getPress());

			gkLoadNode* loadMeshMomoNode = m_tree->createNode<gkLoadNode>();
			loadMeshMomoNode->getTarget()->link(m_meshMomoSetter->getOutput());
			loadMeshMomoNode->getUpdate()->link(lKeyNode->getPress());
		}
	}

	void CreateMomoAnimationLogic()
	{
		m_stateMachine->addTranslation(momoState::CARRY, momoAnimation::CARRY);
		m_stateMachine->addTranslation(momoState::CATCH, momoAnimation::CATCH);
		m_stateMachine->addTranslation(momoState::CATCH_ENDED, momoAnimation::CATCH);
		m_stateMachine->addTranslation(momoState::THROW_WITH, momoAnimation::THROW_WITH);
		m_stateMachine->addTranslation(momoState::WALK, momoAnimation::WALK);
		m_stateMachine->addTranslation(momoState::RUN, momoAnimation::RUN);
		m_stateMachine->addTranslation(momoState::RUN_FASTER, momoAnimation::RUN_FASTER);
		m_stateMachine->addTranslation(momoState::WALK_BACK, momoAnimation::WALK_BACK);
		m_stateMachine->addTranslation(momoState::IDLE_NASTY, momoAnimation::IDLE_NASTY);
		m_stateMachine->addTranslation(momoState::IDLE_CAPOEIRA, momoAnimation::IDLE_CAPOEIRA);

		m_animNode->getTarget()->link(m_meshMomoSetter->getOutput());
		m_stateMachine->getCurrentName()->link(m_animNode->getAnimName());
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

		exit->getExit()->link(key->getPress());
	}

	void CreateCursorLogic()
	{
		gkCursorNode* cursor = m_tree->createNode<gkCursorNode>();

		cursor->getEnable()->link(m_ctrlKeyNode->getIsDown());
		cursor->getUpdate()->link(m_mouseNode->getMotion());
		cursor->getX()->link(m_mouseNode->getAbsX());
		cursor->getY()->link(m_mouseNode->getAbsY());
		cursor->getMaterialName()->setValue(material::ARROW_CURSOR);
	}

	void CreatePickLogic()
	{
		gkPickNode* pick = m_tree->createNode<gkPickNode>();

		pick->getUpdate()->link(m_ctrlKeyNode->getIsDown());
		pick->getCreatePick()->link(m_rightMouseNode->getPress());
		pick->getReleasePick()->link(m_rightMouseNode->getRelease());
		pick->getX()->link(m_mouseNode->getAbsX());
		pick->getY()->link(m_mouseNode->getAbsY());
	}

	void CreateCursorCameraArcBallLogic()
	{
		gkObjectSetterNode* centerObj = m_tree->createNode<gkObjectSetterNode>();
		centerObj->setType(gkObjectSetterNode::SCREEN_XY);
		centerObj->getX()->link(m_mouseNode->getAbsX());
		centerObj->getY()->link(m_mouseNode->getAbsY());

		{
			gkIfNode* ifNode = m_tree->createNode<gkIfNode>();
			ifNode->setStatement(CMP_AND);

			ifNode->getA()->link(m_leftMouseNode->getPress());
			ifNode->getB()->link(m_ctrlKeyNode->getIsDown());

			centerObj->getUpdate()->link(ifNode->getTrue());
		}

		gkArcBallNode* arcBall = m_tree->createNode<gkArcBallNode>();
		arcBall->getCenterObj()->link(centerObj->getOutput());
		arcBall->getCenterPosition()->link(centerObj->getHitPoint());
		arcBall->getTarget()->link(m_cameraSetter->getOutput());

		arcBall->getRelX()->link(m_mouseNode->getRelX());
		arcBall->getRelY()->link(m_mouseNode->getRelY());
		arcBall->getRelZ()->link(m_mouseNode->getWheel());

		arcBall->getMinPitch()->setValue(-90);
		arcBall->getMaxPitch()->setValue(90);

		arcBall->getMinRoll()->setValue(-180);
		arcBall->getMaxRoll()->setValue(180);


		{
			gkIfNode* ifNode = m_tree->createNode<gkIfNode>();
			ifNode->setStatement(CMP_AND);
			ifNode->getA()->link(m_ctrlKeyNode->getIsDown());
			ifNode->getB()->link(m_leftMouseNode->getIsDown());

			arcBall->getUpdate()->link(ifNode->getTrue());
		}
	}

	void CreateDebugLogic()
	{
		gkKeyNode* m_cKeyNode = m_tree->createNode<gkKeyNode>();
		m_cKeyNode->setKey(KC_CKEY);

		gkShowPhysicsNode* showPhysics = m_tree->createNode<gkShowPhysicsNode>();
		showPhysics->getEnable()->link(m_cKeyNode->getIsDown());
	}

private:

	gkString m_blend;

	gkLogicTree* m_tree;

	gkKeyNode* m_ctrlKeyNode;

	gkKeyNode* m_shiftKeyNode;

	gkKeyNode* m_wKeyNode; 
	
	gkKeyNode* m_sKeyNode;

	gkMouseNode* m_mouseNode;

	gkMouseButtonNode* m_leftMouseNode;

	gkMouseButtonNode* m_rightMouseNode;

	gkObjectSetterNode* m_playerSetter;

	gkObjectSetterNode* m_meshMomoSetter;

	gkObjectSetterNode* m_cameraSetter;

	gkArcBallNode* m_momoCameraArcBall;

	gkAnimationNode* m_animNode;

	gkGrabNode* m_momoGrab;

	gkStateMachineNode* m_stateMachine;
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

