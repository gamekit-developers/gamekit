/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): silveira.nestor.
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
#include "gkCamera.h"
#include "gkEntity.h"
#include "gkCoreApplication.h"
#include "gkWindowSystem.h"
#include "gkScene.h"
#include "gkLogger.h"
#include "gkUtils.h"
#include "gkPath.h"
#include "gkBlendFile.h"
#include "gkMemoryTest.h"
#include "gkDynamicsWorld.h"
#include "gkRigidBody.h"
#include "gkTextManager.h"
#include "gkLuaManager.h"

// node tests 
#include "gkVariable.h"
#include "gkNodeManager.h"
#include "gkKeyNode.h"
#include "gkMathNode.h"
#include "gkIfNode.h"
#include "gkMouseNode.h"
#include "gkSwitchNode.h"
#include "gkVariableNode.h"
#include "gkGroupNode.h"
#include "gkObjectNode.h"
#include "gkMotionNode.h"
#include "gkLogicTree.h"
#include "gkMouseButtonNode.h"
#include "gkArcBallNode.h"


// Temporary class to test out
// various aspects of OgreKit
class Player
{
protected:
    gkScene*        m_scene;
    gkEntity*       m_mesh;
    gkGameObject    *m_zRot, *m_xRot, *m_player, *m_view;
    gkMouse*        m_mouse;
    gkKeyboard*     m_keyboard;


public:


    Player(gkScene *scene)
    :       m_scene(scene), m_mesh(0), m_zRot(0), m_xRot(0), m_player(0)
    {
        init();
    }

    ~Player()
    {
    }

    void init(void)
    {
        GK_ASSERT(m_scene);
        m_zRot      = m_scene->getObject("zRot");
        m_xRot      = m_scene->getObject("xRot");
        m_player    = m_scene->getObject("Player");
        m_mesh      = m_scene->getObject("MeshMomo")->getEntity();
        m_mouse     = gkWindowSystem::getSingleton().getMouse();
        m_keyboard  = gkWindowSystem::getSingleton().getKeyboard();
    }

    void update(gkScalar tick)
    {
        if (m_mouse->moved)
        {
            if (m_mouse->relitave.x != 0) m_zRot->roll(gkRadian((-m_mouse->relitave.x * tick)*0.5f));
            if (m_mouse->relitave.y != 0) m_xRot->pitch(gkRadian((-m_mouse->relitave.y * tick)*0.5f));
        }

        if (m_player->hasMoved())
        {
            gkScalar tol = 0.05;
            gkVector3 dpos = (m_player->getPosition() - (m_zRot->getPosition() + gkVector3(0, 0, -0.2f))) * tol;
            m_zRot->translate(dpos);
        }




        if (m_keyboard->isKeyDown(KC_WKEY) || m_keyboard->isKeyDown(KC_UPARROWKEY))
        {
            m_mesh->playAction("Momo_Run", 10);
            gkVector3 v = gkVector3(0, 3.f, 0);
            v = m_zRot->getOrientation() * v;

            v.z = m_player->getLinearVelocity().z;
            m_mesh->setOrientation(m_zRot->getOrientation());
            m_player->setLinearVelocity(v);
            return;
        }
        else if (m_keyboard->isKeyDown(KC_SKEY) || m_keyboard->isKeyDown(KC_DOWNARROWKEY))
        {
            m_mesh->playAction("Momo_WalkBack", 10);
            gkVector3 v = gkVector3(0, -2.f, 0);
            v = m_zRot->getOrientation() * v;
            v.z = m_player->getLinearVelocity().z;
            m_mesh->setOrientation(m_zRot->getOrientation());
            m_player->setLinearVelocity(v);

            //v = m_mesh->getOrientation() * v;
            //v.z = m_player->getLinearVelocity().z;
            //m_player->setLinearVelocity(v);
            return;
        }

        // default
        m_mesh->playAction("Momo_IdleNasty", 10);
    }
};


class OgreKit :
            public gkCoreApplication,
            public gkWindowSystem::Listener
{
public:

    gkString    m_blend;
    gkScene*    m_scene;
    Player*     m_player;

public:


    OgreKit(const gkString &blend)
    :       m_blend(blend), m_scene(0), m_player(0)
    {
        m_prefs.winsize.x   = 800;
        m_prefs.winsize.y   = 600;
        m_prefs.wintitle    = gkString("OgreKit Demo (Press Escape to exit)[") + m_blend + gkString("]");
        m_prefs.blendermat  = false;
        m_prefs.userWindow  = true;
        m_prefs.verbose     = false;
        m_prefs.grabInput   = true;

        m_prefs.debugPhysics = false;
        m_prefs.debugPhysicsAabb = false;

        gkPath path = "OgreKitStartup.cfg";
        // overide settings if found
        if (path.isFile()) m_prefs.load(path.getPath());
    }

    virtual ~OgreKit()
    {
        if (m_player)
            delete m_player;
    }

    bool load(void)
    {
        gkBlendFile *blend = m_engine->loadBlendFile(m_blend);
        if (!blend)
        {
            gkPrintf("File loading failed.\n");
            return false;
        }

        if (m_prefs.userWindow)
            m_engine->initializeWindow();


        gkSceneIterator scit = blend->getSceneIterator();
        if (!scit.hasMoreElements())
        {
            gkPrintf("No usable scenes found in blend.\n");
            return false;
        }

        m_scene = scit.peekNext();

        if (m_blend.find("momo_ogre.blend") != gkString::npos)
        {
            m_prefs.blendermat = true;
            m_scene->load();

            // special case temp game logic
            if (!m_player) m_player = new Player(m_scene);
        }
        else
        {
            m_scene->load();
            //test13(m_scene);
            test0(m_scene);
			//ArcBallTest(m_scene);
        }

        // add input hooks
        gkWindowSystem::getSingleton().addListener(this);
        return true;
    }


    void keyReleased(const gkKeyboard& key, const gkScanCode& sc)
    {
        if (sc == KC_ESCKEY) m_engine->requestExit();
    }

	void ArcBallTest(gkScene* scene)
	{
		// Make sure you disabled grabInput

        gkGameObject *ob = scene->getMainCamera();

		GK_ASSERT(ob);

        gkLogicTree* tree = gkNodeManager::getSingleton().create();

		gkArcBallNode* arcBall = tree->createNode<gkArcBallNode>();

		gkMouseButtonNode* left = tree->createNode<gkMouseButtonNode>();

		gkMouseNode* mouse = tree->createNode<gkMouseNode>();

		gkIfNode* ifAndNode = tree->createNode<gkIfNode>();
		ifAndNode->setStatement(CMP_AND);

		ifAndNode->getA()->link(left->getIsDown());
		ifAndNode->getB()->link(mouse->getMotion());

		gkIfNode* ifOrNode = tree->createNode<gkIfNode>();
		ifOrNode->setStatement(CMP_OR);

		ifOrNode->getA()->link(mouse->getWheel());
		ifOrNode->getB()->link(ifAndNode->getTrue());

		arcBall->getUpdateCenter()->link(left->getPress());
		arcBall->getUpdatePosition()->link(ifOrNode->getTrue());
		arcBall->getX()->link(mouse->getAbsX());
		arcBall->getY()->link(mouse->getAbsY());
		arcBall->getRelX()->link(mouse->getRelX());
		arcBall->getRelY()->link(mouse->getRelY());
		arcBall->getRelZ()->link(mouse->getWheel());

        tree->solveOrder();
        ob->attachLogic(tree);
	}

    void test0(gkScene* scene)
    {
        gkGameObject *ob = 0;

        if (scene->hasObject("Cube"))
            ob = scene->getObject("Cube");
        else
            ob = scene->getMainCamera();

        gkLogicTree* tree = gkNodeManager::getSingleton().create();


        // if (akey || leftmouse) {
        //   rotate ob around z by mousex
        // } else {
        //   rotate ob around x by mousex
        // }

        gkMouseNode* mouse      = tree->createNode<gkMouseNode>();
        gkMotionNode* motionZ   = tree->createNode<gkMotionNode>();
        gkMotionNode* motionT   = tree->createNode<gkMotionNode>();
        gkIfNode* ifNode        = tree->createNode<gkIfNode>();
        gkKeyNode* ctrl         = tree->createNode<gkKeyNode>();
        gkMouseButtonNode* left = tree->createNode<gkMouseButtonNode>();

        ifNode->setStatement(CMP_OR);
        ctrl->setKey(KC_AKEY);
        left->setButton(gkMouse::Left);

        ifNode->getA()->link(ctrl->getIsDown());
        ifNode->getB()->link(left->getIsDown());


        motionZ->getUpdate()->link(ifNode->getTrue());
        motionZ->getZ()->link(mouse->getRelX());

        motionT->getUpdate()->link(ifNode->getFalse());
        motionT->getX()->link(mouse->getRelX());


        tree->solveOrder();
        ob->attachLogic(tree);
    }

    void test13(gkScene* pScene)
    {
        gkGameObject *pObject = 0;

        if (pScene->hasObject("Cube"))
            pObject = pScene->getObject("Cube");
        else
            pObject = pScene->getMainCamera();


        gkLogicTree* pTree = gkNodeManager::getSingleton().create();
        gkMouseNode* pMouseNode = pTree->createNode<gkMouseNode>();
        gkMathNode* pMathNode = pTree->createNode<gkMathNode>();

        pMathNode->getA()->link(pMouseNode->getRelX());
        pMathNode->getB()->link(pMouseNode->getRelY());

        gkKeyNode* pKeyNode = pTree->createNode<gkKeyNode>();
        pKeyNode->setKey(KC_AKEY);

        gkMotionNode* pMotionNode = pTree->createNode<gkMotionNode>();

        pMotionNode->getUpdate()->link(pKeyNode->getIsDown());
        pMotionNode->getX()->link(pMouseNode->getRelX());
        pMotionNode->getZ()->link(pMouseNode->getRelY());
        pMotionNode->getDamping()->link(pMathNode->getResult());

        pTree->solveOrder();
        pObject->attachLogic(pTree);
    }


    void tick(gkScalar tr)
    {
        // update other states
        if (m_player != 0)
            m_player->update(tr);
    }

};


int main(int argc, char **argv)
{
    TestMemory;
    char *fname = "momo_ogre.blend";
	//char *fname = "SimpleState.blend";
	//char *fname = "clubsilo_packed.blend";
	
	

#if GK_PLATFORM != GK_PLATFORM_APPLE
    if (argc > 1)
        fname = argv[argc-1];
#endif

    OgreKit okit(gkUtils::getFile(fname));
    okit.run();
    return 0;
}
