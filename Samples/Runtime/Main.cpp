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
#include "OgreKit.h"


// Temporary class to test out
// various aspects of OgreKit
class Player
{
protected:
    gkScene        *m_scene;
    gkEntity       *m_mesh;
    gkGameObject    *m_zRot, *m_xRot, *m_player, *m_view;
    gkMouse        *m_mouse;
    gkKeyboard     *m_keyboard;
    bool            m_assert;


public:


    Player(gkScene *scene)
        :       m_scene(scene), m_mesh(0), m_zRot(0), m_xRot(0), m_player(0), m_assert(false)
    {
        init();
    }

    ~Player()
    {
    }

    bool valid() {return m_assert;}

    void init(void)
    {
        GK_ASSERT(m_scene);
        m_zRot = m_scene->getObject("zRot");
        m_xRot = m_scene->getObject("xRot");
        m_player = m_scene->getObject("Player");
        gkGameObject *ob = m_scene->getObject("MeshMomo");
        if (ob)
            m_mesh = ob->getEntity();
        m_mouse = gkWindowSystem::getSingleton().getMouse();
        m_keyboard = gkWindowSystem::getSingleton().getKeyboard();
        m_assert = (m_zRot && m_xRot && m_player && m_mesh);
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
    gkScene    *m_scene;
    Player     *m_player;

public:
    OgreKit(const gkString &blend)
        :       m_blend(blend), m_scene(0), m_player(0)
    {
        m_prefs.winsize.x   = 800;
        m_prefs.winsize.y   = 600;
        m_prefs.wintitle    = gkString("OgreKit Demo (Press Escape to exit)[") + m_blend + gkString("]");
        m_prefs.blendermat  = false;
        m_prefs.verbose     = false;
        m_prefs.grabInput   = true;

        m_prefs.debugPhysics = false;
        m_prefs.debugPhysicsAabb = false;

        gkPath path = "OgreKitStartup.cfg";
        // overide settings if found
        if (path.isFile()) m_prefs.load(path.getPath());

        m_prefs.userWindow  = false;
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
            if (!m_player)
            {
                m_player = new Player(m_scene);
                if (!m_player->valid())
                {
                    delete m_player;
                    m_player = 0;
                }
            }
        }
        else
        {
            m_scene->load();
            createMouseLook(m_scene, gkVector3::ZERO, gkVector3(90, 0, 0));
        }

        // add input hooks
        gkWindowSystem::getSingleton().addListener(this);
        return true;
    }


    void createMouseLook(gkScene *sc, const gkVector3 &pos, const gkVector3 &ori,
                         const float rotationScale = -0.3f,
                         const float translationScale = 0.5f
                        )
    {
        if (!sc)
            return;

        gkGameObject *x = sc->createObject("createMouseLook::x");
        gkGameObject *z = sc->createObject("createMouseLook::z");
        gkCamera *mcam = sc->getMainCamera();
        gkCamera *cam = sc->createCamera("createMouseLook::cam");

        if (mcam)
        {
            cam->getCameraProperties() = mcam->getCameraProperties();

            gkVector3 neul = gkMathUtils::getEulerFromQuat(mcam->getWorldOrientation());
            gkVector3 zeul = gkVector3(0, 0, neul.z);

            gkQuaternion &zrot= z->getProperties().orientation;
            gkQuaternion &crot= cam->getProperties().orientation;

            zrot = gkMathUtils::getQuatFromEuler(zeul);
            zrot.normalise();
            crot = gkMathUtils::getQuatFromEuler(ori);
            crot.normalise();

            z->getProperties().position = mcam->getWorldPosition();
        }
        else
        {
            z->getProperties().position = pos;
            cam->getProperties().orientation = gkMathUtils::getQuatFromEuler(ori);
        }

        x->setParent(z);
        cam->setParent(x);

        // zrotation
        gkLogicTree *ztree  = gkNodeManager::getSingleton().create();
        gkMouseNode *zmou   = ztree->createNode<gkMouseNode>();
        gkMotionNode *zmot  = ztree->createNode<gkMotionNode>();

        zmou->getScaleX()->setValue(rotationScale);

        zmot->getUpdate()->link(zmou->getMotion());
        zmot->getZ()->link(zmou->getRelX());

        z->attachLogic(ztree);


        // yrotation
        gkLogicTree *xtree= gkNodeManager::getSingleton().create();
        gkMouseNode *ymou = xtree->createNode<gkMouseNode>();
        gkMotionNode *xmot = xtree->createNode<gkMotionNode>();
        ymou->getScaleY()->setValue(rotationScale);

        xmot->getUpdate()->link(ymou->getMotion());
        xmot->getX()->link(ymou->getRelY());

        /// setup -90, 90 clamp
        xmot->setMinX(-90);
        xmot->setMaxX(90);


        x->attachLogic(xtree);

        // keyboard controlls
        gkKeyNode *fwd = ztree->createNode<gkKeyNode>();
        gkKeyNode *bak = ztree->createNode<gkKeyNode>();
        gkKeyNode *lft = ztree->createNode<gkKeyNode>();
        gkKeyNode *rgt = ztree->createNode<gkKeyNode>();

        fwd->setKey(KC_UPARROWKEY);
        bak->setKey(KC_DOWNARROWKEY);
        lft->setKey(KC_LEFTARROWKEY);
        rgt->setKey(KC_RIGHTARROWKEY);

        gkMathNode *scale1 = ztree->createNode<gkMathNode>();
        gkMathNode *scale2 = ztree->createNode<gkMathNode>();
        scale1->setFunction(MTH_MULTIPLY);
        scale2->setFunction(MTH_MULTIPLY);
        scale1->getB()->setValue(translationScale);
        scale2->getB()->setValue(translationScale);

        /// math node
        // subtract (bak - fwd) out -> link to y movement
        gkMathNode *vecfwd = ztree->createNode<gkMathNode>();

        vecfwd->setFunction(MTH_SUBTRACT);
        vecfwd->getA()->link(fwd->getIsDown());
        vecfwd->getB()->link(bak->getIsDown());
        scale1->getA()->link(vecfwd->getResult());


        gkMotionNode *fwdmot = ztree->createNode<gkMotionNode>();
        fwdmot->getY()->link(scale1->getResult());
        fwdmot->setMotionType(MT_TRANSLATION);

        // translate relitave to x GameObject
        fwdmot->setSpace(TRANSFORM_PARENT);
        fwdmot->setOtherObject("createMouseLook::x");


        /// math node
        // subtract (lft - rgt) out -> link to x movement
        gkMathNode *vecstrf = ztree->createNode<gkMathNode>();

        vecstrf->setFunction(MTH_SUBTRACT);
        vecstrf->getA()->link(rgt->getIsDown());
        vecstrf->getB()->link(lft->getIsDown());

        scale2->getA()->link(vecstrf->getResult());
        fwdmot->getX()->link(scale2->getResult());

        cam->load();
        m_scene->setMainCamera(cam);

    }

    void keyReleased(const gkKeyboard &key, const gkScanCode &sc)
    {
        if (sc == KC_ESCKEY) m_engine->requestExit();
    }

    void tick(gkScalar tr)
    {
        // update other states
        if (m_player != 0) m_player->update(tr);
    }
};


int main(int argc, char **argv)
{
    TestMemory;
    char *fname = "momo_ogre.blend";

#if GK_PLATFORM != GK_PLATFORM_APPLE
    if (argc > 1)
        fname = argv[argc-1];
#endif
    OgreKit okit(gkUtils::getFile(fname));
    okit.run();
    return 0;
}
