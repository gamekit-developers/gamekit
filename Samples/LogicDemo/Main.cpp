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
#include "gkWindowSystem.h"
#include "gkScene.h"
#include "gkCamera.h"
#include "gkLogger.h"
#include "gkUtils.h"
#include "gkPath.h"
#include "gkBlendFile.h"
#include "gkMemoryTest.h"

// node tests 
#include "gkNodeManager.h"
#include "gkKeyNode.h"
#include "gkIfNode.h"
#include "gkMouseNode.h"
#include "gkLogicTree.h"
#include "gkMouseButtonNode.h"
#include "gkArcBallNode.h"
#include "gkCursorNode.h"
#include "gkPickNode.h"
#include "gkTrackNode.h"
#include "gkMotionNode.h"

class OgreKit : public gkCoreApplication, public gkWindowSystem::Listener
{
public:

    OgreKit(const gkString &blend) 
		: m_blend(blend)
	{
        gkPath path = "./data/OgreKitStartup.cfg";

		GK_ASSERT(path.isFile());
        
		m_prefs.load(path.getPath());
    }

    virtual ~OgreKit()
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

        gkScene* pScene = scit.peekNext();

		m_prefs.blendermat = true;
        
		pScene->load();

		CreateCameraLogic(pScene);
		
		CreateMomoMeshLogic(pScene);

        // add input hooks
        gkWindowSystem::getSingleton().addListener(this);

        return true;
    }

    void keyReleased(const gkKeyboard& key, const gkScanCode& sc)
    {
        if (sc == KC_ESCKEY) m_engine->requestExit();
    }

	void CreateMomoMeshLogic(gkScene* pScene)
	{
        gkGameObject *ob = pScene->getObject("MeshMomo");

		GK_ASSERT(ob);

        gkLogicTree* tree = gkNodeManager::getSingleton().create();

		gkKeyNode* ctrlKey = tree->createNode<gkKeyNode>();

		ctrlKey->setKey(KC_LEFTCTRLKEY);

		gkMouseNode* mouse = tree->createNode<gkMouseNode>();

		CreateMomoMeshMotionLogic(tree, mouse, ctrlKey);

        tree->solveOrder();

		ob->attachLogic(tree);
	}

	void CreateMomoMeshMotionLogic(gkLogicTree* tree, gkMouseNode* mouse, gkKeyNode* ctrlKey)
	{
		gkIfNode* ifNode = tree->createNode<gkIfNode>();
		ifNode->setStatement(CMP_NOT);
		ifNode->getA()->link(ctrlKey->getIsDown());

		gkMotionNode* motion = tree->createNode<gkMotionNode>();

		motion->getUpdate()->link(ifNode->getTrue());
		motion->getX()->setValue(0);
		motion->getY()->setValue(0);
		motion->getZ()->link(mouse->getRelX());

/*		gkKeyNode* wKey = tree->createNode<gkKeyNode>();
		wKey->setKey(KC_WKEY);

		gkIfNode* wIfNode = tree->createNode<gkIfNode>();
		ifNode->setStatement(CMP_AND);
		ifNode->getA()->link(ifNode->getTrue());
		ifNode->getB()->link(wKey->getIsDown());

		gkAnimationNode* runAnim = tree->createNode<gkAnimationNode>();
		runAnim->setAnim("Momo_Run");
		runAnim->getBlend()->setValue(
		runAnim->getUpdate()->link(wIfNode->getTrue());*/


	}

	void CreateCameraLogic(gkScene* pScene)
	{
        gkGameObject *ob = pScene->getMainCamera();

		GK_ASSERT(ob);

        gkLogicTree* tree = gkNodeManager::getSingleton().create();

		gkKeyNode* ctrlKey = tree->createNode<gkKeyNode>();

		ctrlKey->setKey(KC_LEFTCTRLKEY);

		gkMouseNode* mouse = tree->createNode<gkMouseNode>();

        ArcBallLogic(tree, mouse, ctrlKey);

		CursorLogic(tree, mouse, ctrlKey);

		PickLogic(tree, mouse, ctrlKey);

		TrackLogic(pScene->getObject("MeshMomo"), tree, ctrlKey);

        tree->solveOrder();

		ob->attachLogic(tree);
	}

	void CursorLogic(gkLogicTree* tree, gkMouseNode* mouse, gkKeyNode* ctrlKey)
	{
		gkCursorNode* cursor = tree->createNode<gkCursorNode>();

		cursor->getEnable()->link(ctrlKey->getIsDown());
		cursor->getUpdate()->link(mouse->getMotion());
		cursor->getX()->link(mouse->getAbsX());
		cursor->getY()->link(mouse->getAbsY());
		cursor->getMaterialName()->setValue(gkString("ArrowCursor"));
		cursor->getWidth()->setValue(32);
		cursor->getHeight()->setValue(32);

		cursor->update(0);
	}

	void PickLogic(gkLogicTree* tree, gkMouseNode* mouse, gkKeyNode* ctrlKey)
	{
		gkMouseButtonNode* right = tree->createNode<gkMouseButtonNode>();

		right->setButton(gkMouse::Right);

		gkPickNode* pick = tree->createNode<gkPickNode>();

		pick->getEnable()->link(ctrlKey->getIsDown());
		pick->getCreatePick()->link(right->getPress());
		pick->getReleasePick()->link(right->getRelease());
		pick->getUpdate()->link(mouse->getMotion());
		pick->getX()->link(mouse->getAbsX());
		pick->getY()->link(mouse->getAbsY());
	}

	void ArcBallLogic(gkLogicTree* tree, gkMouseNode* mouse, gkKeyNode* ctrlKey)
	{
		gkArcBallNode* arcBall = tree->createNode<gkArcBallNode>();

		gkMouseButtonNode* left = tree->createNode<gkMouseButtonNode>();

		gkIfNode* ifAndNode = tree->createNode<gkIfNode>();
		ifAndNode->setStatement(CMP_AND);

		ifAndNode->getA()->link(left->getIsDown());
		ifAndNode->getB()->link(mouse->getMotion());

		gkIfNode* ifOrNode = tree->createNode<gkIfNode>();
		ifOrNode->setStatement(CMP_OR);

		ifOrNode->getA()->link(mouse->getWheel());
		ifOrNode->getB()->link(ifAndNode->getTrue());

		arcBall->getEnable()->link(ctrlKey->getIsDown());
		arcBall->getUpdateCenter()->link(left->getPress());
		arcBall->getUpdatePosition()->link(ifOrNode->getTrue());
		arcBall->getX()->link(mouse->getAbsX());
		arcBall->getY()->link(mouse->getAbsY());
		arcBall->getRelX()->link(mouse->getRelX());
		arcBall->getRelY()->link(mouse->getRelY());
		arcBall->getRelZ()->link(mouse->getWheel());
	}

	void TrackLogic(gkGameObject* pTarget, gkLogicTree* tree, gkKeyNode* ctrlKey)
	{
		gkTrackNode* track = tree->createNode<gkTrackNode>();

		gkIfNode* ifNode = tree->createNode<gkIfNode>();
		ifNode->setStatement(CMP_NOT);
		ifNode->getA()->link(ctrlKey->getIsDown());

		track->getEnable()->link(ifNode->getTrue());
		track->SetTarget(pTarget);
		track->getOffset()->setValue(gkVector3(0, -1.5, -0.2));
	}

	void tick(gkScalar tr)
    {
    }

private:

   gkString m_blend;
};


int main(int argc, char **argv)
{
    TestMemory;
    OgreKit okit(gkUtils::getFile("./data/momo_ogre_plus.blend"));
    okit.run();
    return 0;
}

