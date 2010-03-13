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
#include "OgreRoot.h"
#include "OgreConfigFile.h"
#include "OgreRenderSystem.h"
#include "OgreStringConverter.h"
#include "OgreFrameListener.h"

#include "gkEngine.h"
#include "gkWindowSystem.h"
#include "gkScene.h"
#include "gkSceneManager.h"
#include "gkLogger.h"
#include "gkUtils.h"
#include "gkLuaManager.h"
#include "gkSoundManager.h"
#include "gkLogicManager.h"
#include "gkBlendFile.h"
#include "gkBlendLoader.h"
#include "gkRenderFactory.h"
#include "gkUserDefs.h"
#include "gkTextManager.h"
#include "gkNodeManager.h"


#include "LinearMath/btQuickprof.h"


using namespace Ogre;

struct TickState
{
    unsigned long ticks, rate;
    unsigned long skip, loop;
    unsigned long cur, next, prev;
    gkScalar blend, fixed, invt;
    btClock *T;
    bool lock, init;
};


// shorthand
#define gkEnginePrivate gkEngine::Private
#define ENGINE_TICKS_PER_SECOND     gkScalar(60)
#define ENGINE_TIME_SCALE           gkScalar(0.001)
#define GET_TICK(t)                 ((unsigned long)(t)->getTimeMilliseconds())

// tick states
gkScalar gkEngine::m_tickRate = ENGINE_TICKS_PER_SECOND;
gkScalar gkEngine::m_animRate = 25;



class gkEnginePrivate : public FrameListener
{
public:
    Private(gkEngine *par)
    :       engine(par),
            windowsystem(0),
            scene(0)
    {
        memset(&state, 0, sizeof(TickState));
        plugin_factory = new gkRenderFactoryPrivate();
    }

    ~Private()
    {
        delete plugin_factory;
    }


    // one full update
    void tick(gkScalar delta);
    bool frameRenderingQueued(const FrameEvent& evt);

    gkEngine*                   engine;
    gkWindowSystem*             windowsystem;       // current window system
    gkScene*                    scene;              // current scene
    gkRenderFactoryPrivate*     plugin_factory;     // static plugin loading
    TickState                   state;
    Ogre::Root*                 root;
};



gkEngine::gkEngine(gkUserDefs *oth)
:       m_root(0),
        m_window(0),
        m_initialized(false),
        m_ownsDefs(oth != 0),
        m_listener(0)
{
    m_private = new gkEnginePrivate(this);
    if (oth != 0)
        m_defs = oth;
    else
        m_defs = new gkUserDefs();
}

gkEngine::~gkEngine()
{
    if (m_initialized) finalize();


    // persistent throughout
    gkLogger::disable();

    if (!m_ownsDefs)
    {
        delete m_defs;
        m_defs = 0;
    }
}


void gkEngine::initialize(bool autoCreateWindow)
{
    if (m_initialized) return;


    gkUserDefs &defs = getUserDefs();
    gkLogger::enable(defs.log, defs.verbose);

    if (defs.rendersystem == OGRE_RS_UNKNOWN)
    {
        gkPrintf("Unknown rendersystem!\n");
        return;
    }

    m_root = new Root("", "");
    m_private->plugin_factory->createRenderSystem(m_root, defs.rendersystem);
	m_private->plugin_factory->createParticleSystem(m_root);

    const RenderSystemList &renderers = m_root->getAvailableRenderers();
    if (renderers.empty())
    {
        gkPrintf("No rendersystems present\n");
        return;
    }
    m_root->setRenderSystem(renderers[0]);

    m_root->initialise(false);

    gkWindowSystem *sys = m_private->windowsystem = new gkWindowSystem();


    // gk Managers
    new gkSceneManager();
    new gkLogicManager();
    new gkNodeManager();
    new gkBlendLoader();
    new gkTextManager();
    new gkLuaManager();


    if (autoCreateWindow) initializeWindow();


    m_animRate = defs.animspeed;
    m_tickRate = defs.tickrate;
    m_tickRate = gkClampf(m_tickRate, 25, 90);
    m_initialized = true;
}


void gkEngine::initializeWindow(void)
{
    if (m_private->windowsystem && !m_window)
    {
        gkWindowSystem *sys = m_private->windowsystem;
        gkUserDefs &defs = getUserDefs();

        m_window = sys->createMainWindow(defs);
        if (!defs.resources.empty())
            loadResources(defs.resources);
    }
}


void gkEngine::finalize()
{
    if (!m_initialized) return;


    delete gkSceneManager::getSingletonPtr();
    delete gkTextManager::getSingletonPtr();
    delete gkBlendLoader::getSingletonPtr();
    delete gkLuaManager::getSingletonPtr();
    delete gkLogicManager::getSingletonPtr();
    delete gkNodeManager::getSingletonPtr();
    delete gkWindowSystem::getSingletonPtr();

    delete m_root;
    delete m_private;

    m_root = 0;
    m_initialized = false;
}

gkUserDefs& gkEngine::getUserDefs(void)
{
    GK_ASSERT(m_defs);
    return *m_defs;
}

void gkEngine::requestExit(void)
{
    gkWindowSystem::getSingleton().exit(true);
}


gkBlendFile* gkEngine::loadBlendFile(const gkString& blend, const gkString& inResource)
{
    gkBlendFile *file = 0;
    try
    {
        file = gkBlendLoader::getSingleton().loadFile(blend, inResource);
    }
    catch (Ogre::Exception &e)
    {
        // log any ogre exceptions
        gkPrintf("%s\n", e.getDescription().c_str());
    }
    return file;
}


void gkEngine::loadResources(const gkString &name)
{
    if (name.empty()) return;

    try
    {
        ConfigFile fp;
        fp.load(name);

        ResourceGroupManager *resourceManager = ResourceGroupManager::getSingletonPtr();
        ConfigFile::SectionIterator cit = fp.getSectionIterator();

        while (cit.hasMoreElements())
        {
            gkString name = cit.peekNextKey();
            ConfigFile::SettingsMultiMap *ptr = cit.getNext();
            for (ConfigFile::SettingsMultiMap::iterator dit = ptr->begin(); dit != ptr->end(); ++dit)
                resourceManager->addResourceLocation(dit->second, dit->first, name);
        }
        ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    }
    catch (Exception &e)
    {
        gkLogMessage("Failed to load resource file!\n" << e.getDescription());
    }
}


void gkEngine::addDebugProperty(gkVariable *prop)
{
    // Soon, Evaluate third party GUI systems
}


void gkEngine::removeDebugProperty(gkVariable *prop)
{
    // Soon, Evaluate third party GUI systems
}


gkScalar gkEngine::getStepRate(void)
{
    return gkScalar(1.0) / ENGINE_TICKS_PER_SECOND;
}

gkScalar gkEngine::getTickRate(void)
{
    return ENGINE_TICKS_PER_SECOND;
}

gkScalar gkEngine::getAnimRate(void)
{
    return m_animRate;
}

gkScene* gkEngine::getActiveScene(void)
{
    GK_ASSERT(m_private);
    return m_private->scene;
}

void gkEngine::run(void)
{
    // Start main game loop

    GK_ASSERT(m_private);
    if (!m_private->scene)
    {
        gkLogMessage("Can't run with out a registered scene. exiting\n");
        return;
    }

    gkWindowSystem *sys = m_private->windowsystem;
    if (!sys)
    {
        gkLogMessage("Can't run with out a window system. exiting\n");
        return;
    }

    // setup timer
    m_root->clearEventTimes();
    m_root->getRenderSystem()->_initRenderTargets();
    m_root->addFrameListener(m_private);

    btClock t; t.reset();

    TickState state;
    state.rate = (unsigned long)ENGINE_TICKS_PER_SECOND;
    state.ticks = 1000 / state.rate;
    state.skip  = gkMax<unsigned long>(state.rate / 5, 1);
    state.invt  = 1.0 / (gkScalar)state.ticks;
    state.fixed = 1.0 / (gkScalar)ENGINE_TICKS_PER_SECOND;
    state.T = &t;
    state.init = false;
    m_private->state = state;
    m_private->root = m_root;


    do
    {
        sys->process();

        if (!m_root->renderOneFrame())
            break;
    }
    while (!sys->exitRequest());

    m_root->removeFrameListener(m_private);
}


bool gkEnginePrivate::frameRenderingQueued(const FrameEvent& evt)
{
    state.loop = 0;
    state.lock = false;

    if (!state.init)
    {
        // initialize timer states
        state.init = true;
        state.cur = GET_TICK(state.T);
        state.next = state.prev = state.cur;
    }

    while ((state.cur = GET_TICK(state.T)) > state.next && state.loop < state.skip)
    {
        if (!state.lock)
        {
            tick(state.fixed);
            if (((GET_TICK(state.T) - state.cur) * ENGINE_TIME_SCALE) > state.fixed)
                state.lock = true;
        }
        state.next += state.ticks;
        ++state.loop;
    }

    state.blend = gkScalar(GET_TICK(state.T) + state.ticks - state.next) * state.invt;
    if (state.blend >= 0 && state.blend <= 1)
        scene->synchronizeMotion(state.blend);
    return true;
}


void gkEnginePrivate::tick(gkScalar dt)
{
    // Proccess one full game tick
    GK_ASSERT(windowsystem && scene && engine);
    // dispatch inputs
    windowsystem->dispatch();
    // update main scene
    scene->update(dt);
    // update callbacks
    if (engine->m_listener) 
        engine->m_listener->tick(dt);

    // post process 
    if (!engine->m_loadables.empty())
    {
        utHashTableIterator<LoadQueryMap> it(engine->m_loadables);
        while (it.hasMoreElements())
        {
            utHashTableIterator<LoadQueryMap>::Pair obpair = it.getNext();
            if (obpair.second == LQ_RELOAD)
                static_cast<gkObject*>(obpair.first.key())->reload();
            else if (obpair.second == LQ_LOAD)
                static_cast<gkObject*>(obpair.first.key())->load();
            else if (obpair.second == LQ_UNLOAD)
                static_cast<gkObject*>(obpair.first.key())->unload();
        }
        engine->m_loadables.clear();
    }

}

void gkEngine::setActiveScene(gkScene *sc)
{
    GK_ASSERT(m_private); m_private->scene = sc;
}


GK_IMPLEMENT_SINGLETON(gkEngine);
