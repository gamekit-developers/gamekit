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
#include "gkCoreApplication.h"
#include "gkScene.h"
#include "gkCamera.h"
#include "gkLogger.h"
#include "gkUtils.h"
#include "gkPath.h"
#include "gkBlendFile.h"
#include "gkMemoryTest.h"

// node tests 
#include "gkNodeManager.h"
#include "gkIfNode.h"
#include "gkMouseNode.h"
#include "gkLogicTree.h"
#include "gkButtonNode.h"
#include "gkArcBallNode.h"
#include "gkCursorNode.h"
#include "gkPickNode.h"
#include "gkTrackNode.h"
#include "gkNewMotionNode.h"
#include "gkAnimationNode.h"
#include "gkExitNode.h"
#include "gkSetterNode.h"

class OgreKit : public gkCoreApplication
{
public:

    OgreKit(const gkString &blend) 
		: m_blend(blend), m_scene(0), m_camera(0), m_player(0), m_meshMomo(0),
		m_tree(0), m_ctrlKeyNode(0), 
		m_wKeyNode(0), m_sKeyNode(0), m_mouseNode(0), m_leftMouseNode(0), m_rightMouseNode(0)

	{
        gkPath path = "./data/OgreKitStartup.cfg";

		GK_ASSERT(path.isFile());
        
		m_prefs.load(path.getPath());
    }

	void tick(gkScalar rate)
	{
	}

    bool load()
    {
        gkBlendFile* pBlendFile = m_engine->loadBlendFile(m_blend);

		GK_ASSERT(pBlendFile);

        if (m_prefs.userWindow)
            m_engine->initializeWindow();

        gkSceneIterator scit = pBlendFile->getSceneIterator();

		GK_ASSERT(scit.hasMoreElements());

        m_scene = scit.peekNext();

		m_prefs.blendermat = true;
        
		m_scene->load();

		m_camera = m_scene->getMainCamera();

		GK_ASSERT(m_camera);

        m_player = m_scene->getObject("Player");

		GK_ASSERT(m_player);

        m_meshMomo = m_scene->getObject("MeshMomo");

		GK_ASSERT(m_meshMomo);

		CreateCommonTree();

		ExitLogic();

		CreateCameraLogic();
		
		CreateMomoMeshLogic();

		CreateMomoPlayerLogic();

		m_tree->solveOrder();

		m_camera->attachLogic(m_tree);

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
	}

	void CreateMomoPlayerLogic()
	{
		gkNewMotionNode* motion = m_tree->createNode<gkRotateNode>();
		motion->SetTarget(m_player);

		motion->getUpdate()->link(m_ctrlKeyNode->getNotIsDown());
		motion->getX()->setValue(0);
		motion->getY()->setValue(0);
		motion->getZ()->link(m_mouseNode->getRelX());

		{
			// move forward

			gkIfNode* ifNode = m_tree->createNode<gkIfNode>();
			ifNode->setStatement(CMP_AND);
			ifNode->getA()->link(m_ctrlKeyNode->getNotIsDown());
			ifNode->getB()->link(m_wKeyNode->getIsDown());

			gkNewMotionNode* motion = m_tree->createNode<gkLinearVelNode>();
			motion->SetTarget(m_player);

			motion->getUpdate()->link(ifNode->getTrue());
			motion->getX()->setValue(0);
			motion->getY()->setValue(3);
			motion->getZ()->setValue(0);
		}

		{
			// move back

			gkIfNode* ifNode = m_tree->createNode<gkIfNode>();
			ifNode->setStatement(CMP_AND);
			ifNode->getA()->link(m_ctrlKeyNode->getNotIsDown());
			ifNode->getB()->link(m_sKeyNode->getIsDown());

			gkNewMotionNode* motion = m_tree->createNode<gkLinearVelNode>();
			motion->SetTarget(m_player);

			motion->getUpdate()->link(ifNode->getTrue());
			motion->getX()->setValue(0);
			motion->getY()->setValue(-2);
			motion->getZ()->setValue(0);
		}
	}

	void CreateMomoMeshLogic()
	{
		gkAnimationNode* anim = m_tree->createNode<gkAnimationNode>();
		anim->SetTarget(m_meshMomo);

		gkSetterNode* runName = m_tree->createNode<gkSetterNode>();
		runName->SetTarget(anim->getAnimName());
		runName->getInput()->setValue(gkString("Momo_Run"));
		runName->getUpdate()->link(m_wKeyNode->getIsDown());
			
		gkSetterNode* walkName = m_tree->createNode<gkSetterNode>();
		walkName->SetTarget(anim->getAnimName());
		walkName->getInput()->setValue(gkString("Momo_WalkBack"));
		walkName->getUpdate()->link(m_sKeyNode->getIsDown());

		gkIfNode* ifNode = m_tree->createNode<gkIfNode>();
		ifNode->setStatement(CMP_AND);
		ifNode->getA()->link(m_wKeyNode->getNotIsDown());
		ifNode->getB()->link(m_sKeyNode->getNotIsDown());

		gkSetterNode* ildeName = m_tree->createNode<gkSetterNode>();
		ildeName->SetTarget(anim->getAnimName());
		ildeName->getInput()->setValue(gkString("Momo_IdleNasty"));
		ildeName->getUpdate()->link(ifNode->getTrue());
	}

	void CreateCameraLogic()
	{
        ArcBallLogic();

		CursorLogic();

		PickLogic();

		TrackLogic();
	}

	void ExitLogic()
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
		cursor->getWidth()->setValue(32);
		cursor->getHeight()->setValue(32);

		cursor->update(0);
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

		arcBall->SetTarget(m_camera);

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
		track->SetTarget(m_camera);
		track->SetTrack(m_player);

		track->getOffset()->setValue(gkVector3(0, -1.5, -0.2));
	}

private:

	gkString m_blend;

	gkScene* m_scene;

    gkGameObject* m_camera;

	gkGameObject* m_player;

	gkGameObject* m_meshMomo;

	gkLogicTree* m_tree;

	gkKeyNode* m_ctrlKeyNode;

	gkKeyNode* m_wKeyNode; 
	
	gkKeyNode* m_sKeyNode;

	gkMouseNode* m_mouseNode;

	gkMouseButtonNode* m_leftMouseNode;

	gkMouseButtonNode* m_rightMouseNode;
};

int main(int argc, char **argv)
{
    TestMemory;
    OgreKit okit(gkUtils::getFile("./data/momo_ogre_plus.blend"));
    okit.run();
    return 0;
}

