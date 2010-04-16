/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

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
#include "NodeUtils/nsString.h"
#include "nsMainWindow.h"
#include "nsNodeManager.h"
#include "nsOgreKitPreview.h"
#include "OgreKit.h"



// ----------------------------------------------------------------------------
class nsThreadApp : public wxThread
{
private:
    nsOgreKitPreview    *m_handler;


public:
    nsThreadApp(nsOgreKitPreview *handler) :  m_handler(handler) {}
    virtual ~nsThreadApp()  { m_handler->m_app = 0;}

    ExitCode Entry(void);
};


// ----------------------------------------------------------------------------
// Local Runtime
class OgreKit : public gkCoreApplication, public gkWindowSystem::Listener
{
private:

    gkString            m_blend;
    gkScene             *m_scene;
    nsThreadApp         *m_app;

public:

    OgreKit(nsThreadApp *app, const gkString &blend)
        :  m_blend(blend), m_scene(0), m_app(app)
    {
        m_prefs.winsize.x        = 800;
        m_prefs.winsize.y        = 600;
        m_prefs.wintitle         = gkString("OgreKit Demo (Press Escape to exit)[") + m_blend + gkString("]");
        m_prefs.blendermat       = false;
        m_prefs.verbose          = false;
        m_prefs.grabInput        = true;
        m_prefs.debugPhysics     = false;
        m_prefs.debugPhysicsAabb = false;
        m_prefs.userWindow       = false;
    }
    virtual ~OgreKit() {}

    bool load(void)
    {
        gkBlendFile *blend = m_engine->loadBlendFile(m_blend);
        if (!blend)
        {
            gkPrintf("File loading failed.\n");
            return false;
        }

        if (m_prefs.userWindow) m_engine->initializeWindow();

        gkSceneIterator scit = blend->getSceneIterator();
        if (!scit.hasMoreElements())
        {
            gkPrintf("No usable scenes found in blend.\n");
            return false;
        }

        m_scene = scit.peekNext();
        m_scene->load();

        {
            wxCRITICAL_SECTION(lock);
            loadNodes();
        }

        // add input hooks
        gkWindowSystem::getSingleton().addListener(this);
        return true;
    }


    void keyReleased(const gkKeyboard &key, const gkScanCode &sc)
    {
        if (sc == KC_ESCKEY) m_engine->requestExit();
    }


    void tick(gkScalar rate)
    {
        if (m_app->TestDestroy()) m_engine->requestExit();
    }



    gkLogicNode *createNodeType(gkLogicTree *tree, nsNode *node, nsNodeDef *type);
    void loadNodes(void);
};


// ----------------------------------------------------------------------------
gkLogicNode *OgreKit::createNodeType(gkLogicTree *tree, nsNode *node, nsNodeDef *type)
{
    gkLogicNode *lnode = 0;

    switch (type->getType())
    {
    case NT_SKELETON:
        lnode = tree->createNode<gkAnimationNode>();
        break;
    case NT_MOUSE_MOTION:
        lnode = tree->createNode<gkMouseNode>();
        break;
    case NT_OBJECT_MOTION:
        {
            gkMotionNode *motnd = tree->createNode<gkMotionNode>();
            nsMotionData *dt = static_cast<nsMotionData*>(node->getData());

            if (dt->getFlag() & 1)
            {
                motnd->setMinX(dt->getClampX().x);
                motnd->setMaxX(dt->getClampX().y);
            }
            else if (dt->getFlag() & 2)
            {
                motnd->setMinY(dt->getClampY().x);
                motnd->setMaxY(dt->getClampY().y);
            }
            else if (dt->getFlag() & 4)
            {
                motnd->setMinZ(dt->getClampZ().x);
                motnd->setMaxZ(dt->getClampZ().y);
            }

            motnd->setMotionType((gkMotionTypes)dt->getEnum());
            motnd->setOtherObject(dt->getRelitaveObject());
            motnd->setSpace(dt->getTransform());
            if (dt->getKeep())
                motnd->keepVelocity();


            lnode = motnd;

        }
        break;
    }

    if (lnode)
    {
        if (!node->getAttachedName().empty())
        {
            gkGameObject *ob = m_scene->getObject(node->getAttachedName());
            if (ob)
                lnode->attachObject(ob);
        }
    }


    return lnode;
}

#define NS_SOCKET_CAST(s, t) dynamic_cast<gkLogicSocket<t>*>(s)
typedef nsHashTable<utPointerHashKey, gkLogicNode *> nsGkNodeLookup;


// ----------------------------------------------------------------------------
void OgreKit::loadNodes(void)
{
    nsNodeManager &nsNodes = nsNodeManager::getSingleton();
    gkNodeManager &gkNodes = gkNodeManager::getSingleton();


    nsTreeIterator it = nsNodes.getTreeIterator();
    while (it.hasMoreElements())
    {
        nsNodeTree *ntree   = it.getNext();
        gkLogicTree *ltree  = gkNodes.create();

        gkGameObject *ob = m_scene->getObject(ntree->getAttachedName());
        if (ob) ltree->attachObject(ob);


        // link map
        nsGkNodeLookup nodeLookup;


        // create initial types

        nsNodeIterator nit(ntree->getNodeIterator());
        while (nit.hasMoreElements())
        {
            nsNode *nd          = nit.getNext();
            gkLogicNode *lnode  = createNodeType(ltree, nd, nd->getType());
            if (!lnode) continue;


            nodeLookup.insert(nd, lnode);

            int sockIndex = 0;
            for (nsSocket *sock = nd->getFirstInput(); sock; sock = sock->getNext(), ++sockIndex)
            {
                gkILogicSocket *lsock = lnode->getInputSocket(sockIndex);

                const nsValue &var = sock->getValue();

                if (var.isTypeOf(typeid(bool)))
                    NS_SOCKET_CAST(lsock, bool)->setValue(var);
                else if (var.isTypeOf(typeid(int)))
                    NS_SOCKET_CAST(lsock, int)->setValue(var);
                else if (var.isTypeOf(typeid(float)))
                    NS_SOCKET_CAST(lsock, float)->setValue(var);
                else if (var.isTypeOf(typeid(nsString)))
                    NS_SOCKET_CAST(lsock, nsString)->setValue(var);
                else if (var.isTypeOf(typeid(nsObjectSocketData)))
                    NS_SOCKET_CAST(lsock, gkGameObject*)->setValue(m_scene->getObject(var.toString()));
            }
        }


        nsHashTableIterator<nsGkNodeLookup> hashIt(nodeLookup);
        while (hashIt.hasMoreElements())
        {
            nsHashTableIterator<nsGkNodeLookup>::Pair p = hashIt.getNext();
            nsNode *tnode = (nsNode *)p.first.key();
            gkLogicNode *lnode = p.second;


            for (nsSocket *sock = tnode->getFirstInput(); sock; sock = sock->getNext())
            {
                if (sock->isConnected())
                {
                    // get parent node link
                    nsSocket *tlsock        = sock->getSocketLink();
                    nsNode *pnd             = tlsock->getParent();
                    gkILogicSocket *lcsock  = lnode->getInputSocket(sock->getType()->getUid());


                    if (lcsock)
                    {
                        NSsize pos = nodeLookup.find(pnd);
                        if (pos != NS_NPOS)
                        {
                            gkLogicNode *plnode     = nodeLookup.at(pos);
                            gkILogicSocket *lsock   = plnode->getOutputSocket(tlsock->getType()->getUid());
                            if (lsock)
                                lcsock->link(lsock);
                        }
                    }
                }
            }

        }

    }

}
// ----------------------------------------------------------------------------
wxThread::ExitCode nsThreadApp::Entry(void)
{
    const wxString &preview = nsMainWindow::getSingleton().getPreviewFile();
    {
        OgreKit okt(this, wxToSTDString(preview));
        okt.run();
    }
    return (wxThread::ExitCode)0;
}


// ----------------------------------------------------------------------------
nsOgreKitPreview::nsOgreKitPreview()
    :   m_app(0)
{
}

// ----------------------------------------------------------------------------
nsOgreKitPreview::~nsOgreKitPreview()
{
    stop();
}

// ----------------------------------------------------------------------------
void nsOgreKitPreview::stop(void)
{
    if (m_app)
    {
        wxCriticalSectionLocker enter(m_appCS);
        m_app->Delete();
    }


    while (1)
    {
        wxCriticalSectionLocker enter(m_appCS);
        if (!m_app) break;
    }

    // wait for thread completion
    wxThread::This()->Sleep(1);
}

// ----------------------------------------------------------------------------
void nsOgreKitPreview::start(void)
{
    if (!m_app)
    {

        m_app = new nsThreadApp(this);
        if (m_app->Create() ==  wxTHREAD_NO_ERROR)
        {
            if (m_app->Run() != wxTHREAD_NO_ERROR)
            {
                delete m_app;
                m_app = 0;
            }
        }
        else
        {
            delete m_app;
            m_app = 0;
        }
    }
}
