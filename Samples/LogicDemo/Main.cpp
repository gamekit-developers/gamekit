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

namespace
{
	gkString MOMO_RUN_ANIM		= "Momo_Run";
	gkString MOMO_WALKBACK_ANIM = "Momo_WalkBack";
	gkString MOMO_IDLENASTY_ANIM = "Momo_IdleNasty";
	gkString DUST_RUN_PARTICLE	= "DustRun";
	gkString DUST_WALK_PARTICLE	= "DustWalk";
}

class OgreKit : public gkCoreApplication
{
public:

    OgreKit(const gkString &blend) 
		: m_blend(blend), m_tree(0), m_ctrlKeyNode(0), 
		m_wKeyNode(0), m_sKeyNode(0), m_mouseNode(0), m_leftMouseNode(0), m_rightMouseNode(0),
		m_playerSetter(0), m_meshMomoSetter(0), m_cameraSetter(0), m_animNode(0)

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

		CreateCommonTree();

		CreateExitLogic();

		CreateCameraLogic();

		CreateMomoLogic();
		
		m_tree->solveOrder();

		gkGameObject* pCamera = pScene->getMainCamera();

		GK_ASSERT(pCamera);

		pCamera->attachLogic(m_tree);

        return true;
    }

	void CreateCommonTree()
	{
		m_tree = gkNodeManager::getSingleton().create();

		m_ctrlKeyNode = m_tree->createNode<gkKeyNode>();
		m_ctrlKeyNode->setKey(KC_LEFTCTRLKEY);

		m_wKeyNode = m_tree->createNode<gkKeyNode>();
		m_wKeyNode->setKey(KC_WKEY);

		m_sKeyNode = m_tree->createNode<gkKeyNode>();
		m_sKeyNode->setKey(KC_SKEY);

		m_mouseNode = m_tree->createNode<gkMouseNode>();

		m_leftMouseNode = m_tree->createNode<gkMouseButtonNode>();
		
		m_rightMouseNode = m_tree->createNode<gkMouseButtonNode>();
		m_rightMouseNode->setButton(gkMouse::Right);

		m_playerSetter = m_tree->createNode<gkObjectSetterNode>();
		m_playerSetter->getUpdate()->setValue(true);
		m_playerSetter->getInput()->setValue(gkString("Player"));
		m_playerSetter->getJustOnce()->setValue(true);

		m_meshMomoSetter = m_tree->createNode<gkObjectSetterNode>();
		m_meshMomoSetter->getUpdate()->setValue(true);
		m_meshMomoSetter->getInput()->setValue(gkString("MeshMomo"));
		m_meshMomoSetter->getJustOnce()->setValue(true);

		m_cameraSetter = m_tree->createNode<gkObjectSetterNode>();
		m_cameraSetter->getUpdate()->setValue(true);
		m_cameraSetter->getInput()->setValue(gkString("View"));
		m_cameraSetter->getJustOnce()->setValue(true);

		m_animNode = m_tree->createNode<gkAnimationNode>();

	}

	void CreateMomoLogic()
	{
		CreateMomoAnimationLogic();

		CreateMomoMoveLogic();

		CreateMomoLoadUnloadLogic();

		CreateMomoGrabLogic();

		CreateMomoDustTrailLogic();
	}

	void CreateMomoGrabLogic()
	{
		gkGrabNode* grab1 = m_tree->createNode<gkGrabNode>();
		grab1->getTarget()->link(m_playerSetter->getOutput());
		grab1->getGrabDirection()->setValue(gkVector3(0.1f, 1.1f, 0));
		grab1->getThrowVelocity()->setValue(gkVector3(0, 20.5f, 0));
		grab1->getOffsetPosition()->setValue(gkVector3(0, 0, 0.2));

		gkGrabNode* grab2 = m_tree->createNode<gkGrabNode>();
		grab2->getTarget()->link(m_playerSetter->getOutput());
		grab2->getGrabDirection()->setValue(gkVector3(-0.1f, 1.1f, 0));
		grab2->getThrowVelocity()->setValue(gkVector3(0, 20.5f, 0));
		grab2->getOffsetPosition()->setValue(gkVector3(0, 0, 0.2));

		{
			gkIfNode* ifNode = m_tree->createNode<gkIfNode>();
			ifNode->setStatement(CMP_AND);

			ifNode->getA()->link(m_rightMouseNode->getPress());
			ifNode->getB()->link(m_ctrlKeyNode->getNotIsDown());

			grab1->getCreateGrab()->link(ifNode->getTrue());
			grab2->getCreateGrab()->link(ifNode->getTrue());
		}

		{
			gkIfNode* ifNode = m_tree->createNode<gkIfNode>();
			ifNode->setStatement(CMP_AND);

			ifNode->getA()->link(m_rightMouseNode->getRelease());
			ifNode->getB()->link(m_ctrlKeyNode->getNotIsDown());

			grab1->getReleaseGrab()->link(ifNode->getTrue());
			grab2->getReleaseGrab()->link(ifNode->getTrue());
		}

		{
			gkIfNode* ifNode = m_tree->createNode<gkIfNode>();
			ifNode->setStatement(CMP_AND);

			ifNode->getA()->link(m_leftMouseNode->getPress());
			ifNode->getB()->link(m_ctrlKeyNode->getNotIsDown());

			grab1->getThrowObject()->link(ifNode->getTrue());
			grab2->getThrowObject()->link(ifNode->getTrue());
		}
	}

	void CreateMomoMoveLogic()
	{
		{
			// Rotate Momo

			gkNewMotionNode* motion = m_tree->createNode<gkRotateNode>();

			motion->getUpdate()->link(m_ctrlKeyNode->getNotIsDown());
			motion->getX()->setValue(0);
			motion->getY()->setValue(0);
			motion->getZ()->link(m_mouseNode->getRelX());
			motion->getTarget()->link(m_playerSetter->getOutput());
		}

		{
			// move Momo forward

			gkNewMotionNode* motion = m_tree->createNode<gkLinearVelNode>();

			motion->getUpdate()->link(m_wKeyNode->getIsDown());
			motion->getX()->setValue(0);
			motion->getY()->setValue(2.5f);
			motion->getZ()->setValue(0);
			motion->getTarget()->link(m_playerSetter->getOutput());
		}

		{
			// move Momo back

			gkNewMotionNode* motion = m_tree->createNode<gkLinearVelNode>();

			motion->getUpdate()->link(m_sKeyNode->getIsDown());
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

			equalNode->getA()->setValue(MOMO_RUN_ANIM);
			equalNode->getB()->link(m_animNode->getCurrentAnimName());

			CreateMomoDustTrailLogic(DUST_RUN_PARTICLE, equalNode->getOutput());
		}

		{
			gkStringEqualNode* equalNode = m_tree->createNode<gkStringEqualNode>();

			equalNode->getA()->setValue(MOMO_WALKBACK_ANIM);
			equalNode->getB()->link(m_animNode->getCurrentAnimName());

			CreateMomoDustTrailLogic(DUST_WALK_PARTICLE, equalNode->getOutput());
		}
	}

	void CreateMomoDustTrailLogic(const gkString& name, gkLogicSocket* pUpdateSocket)
	{
		gkOrientationSetterNode* orientation = m_tree->createNode<gkOrientationSetterNode>();
		orientation->getInput()->link(m_playerSetter->getOutput());
		orientation->getUpdate()->link(pUpdateSocket);

		gkCollisionNode* collision = m_tree->createNode<gkCollisionNode>();
		collision->getEnable()->setValue(true);
		collision->getCollidesWith()->setValue(gkString("Plane"));
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

			m_playerSetter->getReload()->link(rKeyNode->getPress());
			m_meshMomoSetter->getReload()->link(rKeyNode->getPress());
		}

		{
			// unload

			gkKeyNode* uKeyNode = m_tree->createNode<gkKeyNode>();
			uKeyNode->setKey(KC_UKEY);

			m_playerSetter->getUnload()->link(uKeyNode->getPress());
			m_meshMomoSetter->getUnload()->link(uKeyNode->getPress());
		}

		{
			// load

			gkKeyNode* lKeyNode = m_tree->createNode<gkKeyNode>();
			lKeyNode->setKey(KC_LKEY);

			m_playerSetter->getLoad()->link(lKeyNode->getPress());
			m_meshMomoSetter->getLoad()->link(lKeyNode->getPress());
		}
	}

	void CreateMomoAnimationLogic()
	{
		m_animNode->getTarget()->link(m_meshMomoSetter->getOutput());

		gkStringSetterNode* runName = m_tree->createNode<gkStringSetterNode>();
		runName->getOutput()->link(m_animNode->getAnimName());
		runName->getInput()->setValue(MOMO_RUN_ANIM);
		runName->getUpdate()->link(m_wKeyNode->getIsDown());
			
		gkStringSetterNode* walkName = m_tree->createNode<gkStringSetterNode>();
		walkName->getOutput()->link(m_animNode->getAnimName());
		walkName->getInput()->setValue(MOMO_WALKBACK_ANIM);
		walkName->getUpdate()->link(m_sKeyNode->getIsDown());

		gkIfNode* ifNode = m_tree->createNode<gkIfNode>();
		ifNode->setStatement(CMP_AND);
		ifNode->getA()->link(m_wKeyNode->getNotIsDown());
		ifNode->getB()->link(m_sKeyNode->getNotIsDown());

		gkStringSetterNode* ildeName = m_tree->createNode<gkStringSetterNode>();
		ildeName->getOutput()->link(m_animNode->getAnimName());
		ildeName->getInput()->setValue(MOMO_IDLENASTY_ANIM);
		ildeName->getUpdate()->link(ifNode->getTrue());
	}

	void CreateCameraLogic()
	{
        ArcBallLogic();

		CursorLogic();

		PickLogic();

		TrackLogic();
	}

	void CreateExitLogic()
	{
		gkExitNode* exit = m_tree->createNode<gkExitNode>();

		gkKeyNode* key = m_tree->createNode<gkKeyNode>();

		key->setKey(KC_ESCKEY);

		exit->getExit()->link(key->getPress());
	}

	void CursorLogic()
	{
		gkCursorNode* cursor = m_tree->createNode<gkCursorNode>();

		cursor->getEnable()->link(m_ctrlKeyNode->getIsDown());
		cursor->getUpdate()->link(m_mouseNode->getMotion());
		cursor->getX()->link(m_mouseNode->getAbsX());
		cursor->getY()->link(m_mouseNode->getAbsY());
		cursor->getMaterialName()->setValue(gkString("ArrowCursor"));
	}

	void PickLogic()
	{
		gkPickNode* pick = m_tree->createNode<gkPickNode>();

		pick->getEnable()->link(m_ctrlKeyNode->getIsDown());
		pick->getCreatePick()->link(m_rightMouseNode->getPress());
		pick->getReleasePick()->link(m_rightMouseNode->getRelease());
		pick->getUpdate()->link(m_mouseNode->getMotion());
		pick->getX()->link(m_mouseNode->getAbsX());
		pick->getY()->link(m_mouseNode->getAbsY());
	}

	void ArcBallLogic()
	{
		gkArcBallNode* arcBall = m_tree->createNode<gkArcBallNode>();

		arcBall->getTarget()->link(m_cameraSetter->getOutput());

		gkIfNode* ifAndNode = m_tree->createNode<gkIfNode>();
		ifAndNode->setStatement(CMP_AND);

		ifAndNode->getA()->link(m_leftMouseNode->getIsDown());
		ifAndNode->getB()->link(m_mouseNode->getMotion());

		gkIfNode* ifOrNode = m_tree->createNode<gkIfNode>();
		ifOrNode->setStatement(CMP_OR);

		ifOrNode->getA()->link(m_mouseNode->getWheel());
		ifOrNode->getB()->link(ifAndNode->getTrue());

		arcBall->getEnable()->link(m_ctrlKeyNode->getIsDown());
		arcBall->getUpdateCenter()->link(m_leftMouseNode->getPress());
		arcBall->getUpdatePosition()->link(ifOrNode->getTrue());
		arcBall->getX()->link(m_mouseNode->getAbsX());
		arcBall->getY()->link(m_mouseNode->getAbsY());
		arcBall->getRelX()->link(m_mouseNode->getRelX());
		arcBall->getRelY()->link(m_mouseNode->getRelY());
		arcBall->getRelZ()->link(m_mouseNode->getWheel());
	}

	void TrackLogic()
	{
		gkTrackNode* track = m_tree->createNode<gkTrackNode>();

		track->getEnable()->link(m_ctrlKeyNode->getNotIsDown());
		track->getTarget()->link(m_cameraSetter->getOutput());
		track->getTrack()->link(m_playerSetter->getOutput());

		track->getOffset()->setValue(gkVector3(0, -2, 0.2));
	}

private:

	gkString m_blend;

	gkLogicTree* m_tree;

	gkKeyNode* m_ctrlKeyNode;

	gkKeyNode* m_wKeyNode; 
	
	gkKeyNode* m_sKeyNode;

	gkMouseNode* m_mouseNode;

	gkMouseButtonNode* m_leftMouseNode;

	gkMouseButtonNode* m_rightMouseNode;

	gkObjectSetterNode* m_playerSetter;

	gkObjectSetterNode* m_meshMomoSetter;

	gkObjectSetterNode* m_cameraSetter;

	gkAnimationNode* m_animNode;
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

