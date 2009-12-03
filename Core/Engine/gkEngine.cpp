/*
-------------------------------------------------------------------------------
	This file is part of the Ogre GameKit port.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#include "gkSceneObject.h"
#include "gkSceneObjectManager.h"
#include "gkLogger.h"
#include "gkUtils.h"
#include "gkScriptManager.h"
#include "gkSoundManager.h"
#include "gkLogicManager.h"
#include "gkConsole.h"
#include "gkDebugPage.h"


#include "Loaders/gkBlendFile.h"
#include "Loaders/gkBlendLoader.h"

#include "gkRenderFactory.h"
#include "gkUserDefs.h"

using namespace Ogre;





// ----------------------------------------------------------------------------
class gkEnginePrivate : public FrameListener
{
public:
	gkEnginePrivate();
	~gkEnginePrivate();


	/// one full update
	void tick(Real elapsed, Real dt);

	/// for doing smooth ticks
	bool frameRenderingQueued(const FrameEvent& evt);

	gkWindowSystem*		   windowsystem;   // current window system
	Real					frame_interpl;  // frame blending
	gkSceneObject*			scene;		  // current scene
	gkRenderFactoryPrivate*   plugin_factory; // static/dynamic loading
};







// ----------------------------------------------------------------------------
gkEnginePrivate::gkEnginePrivate() :
		windowsystem(0),
		frame_interpl(0.0),
		scene(0)
{
	plugin_factory= new gkRenderFactoryPrivate();
}

// ----------------------------------------------------------------------------
gkEnginePrivate::~gkEnginePrivate()
{
	delete plugin_factory;
}

// ----------------------------------------------------------------------------
gkEngine::gkEngine(const String& homeDir) :
		mRoot(0),
		mInitialized(false)
{
	mPrivate= new gkEnginePrivate();
	mDefs= new gkUserDefs();
	mDefs->load(homeDir);
	gkLogger::enable(mDefs->log, mDefs->verbose);
}


// ----------------------------------------------------------------------------
gkEngine::~gkEngine()
{
	if (mInitialized)
		finalize();

	gkLogger::disable();
	delete mDefs;
	mDefs= 0;
}


// ----------------------------------------------------------------------------
void gkEngine::initialize(void)
{
	if (mInitialized)
		return;

	gkUserDefs &defs= getUserDefs();

	mRoot= new Root("", "");

	new gkSceneObjectManager();
	new gkLogicManager();
	new gkBlendLoader();

	gkScriptManager::initialize();

	mPrivate->plugin_factory->createRenderSystem(mRoot, gkRenderFactoryPrivate::OGRE_RS_GL);

	const RenderSystemList &renderers= mRoot->getAvailableRenderers();
	if (renderers.empty())
	{
		OGRE_EXCEPT(Exception::ERR_INVALID_STATE,
					"No rendersystems present",
					"gkEngine::initialize");
	}

	mRoot->setRenderSystem(renderers[0]);
	mRoot->initialise(false);

	gkWindowSystem::WindowBackend backend= gkWindowSystem::OGRE_BACKEND;

	gkWindowSystem *sys= mPrivate->windowsystem= gkWindowSystem::initialize(backend);
	if (!sys)
	{
		OGRE_EXCEPT(Exception::ERR_INVALID_STATE,
					"gkWindowSystem creation failed",
					"gkEngine::initialize");
	}

	sys->createWindow(defs.wintitle, defs.winsize.x, defs.winsize.y, false);

	mAnimRate= defs.animspeed;
	mTickRate= defs.tickrate;
	mTickRate= gkClamp(mTickRate, 25, 90);

	if (!defs.resources.empty())
		loadResources(defs.resources);
	mInitialized= true;
}

// ----------------------------------------------------------------------------
void gkEngine::finalize()
{
	if (!mInitialized) return;

	delete gkLogicManager::getSingletonPtr();
	gkWindowSystem::finalize();
	gkScriptManager::finalize();

	delete gkSceneObjectManager::getSingletonPtr();
	delete gkBlendLoader::getSingletonPtr();

	delete mRoot;
	delete mPrivate;

	mRoot= 0;
	mInitialized= false;
}

// ----------------------------------------------------------------------------
gkUserDefs& gkEngine::getUserDefs(void)
{
	GK_ASSERT(mDefs);
	return *mDefs;
}

// ----------------------------------------------------------------------------
void gkEngine::requestExit(void)
{
	gkWindowSystem::getSingleton().exit();
}

// ----------------------------------------------------------------------------
gkBlendFile* gkEngine::loadBlendFile(const String& blend, const String& inResource)
{
	return gkBlendLoader::getSingleton().loadFile(blend, inResource);
}

// ----------------------------------------------------------------------------
void gkEngine::loadResources(const String &name)
{
	if (name.empty())
		return;

	try
	{
		ConfigFile fp;
		fp.load(name);

		ResourceGroupManager *resourceManager= ResourceGroupManager::getSingletonPtr();
		ConfigFile::SectionIterator cit= fp.getSectionIterator();

		while (cit.hasMoreElements())
		{
			String name= cit.peekNextKey();
			ConfigFile::SettingsMultiMap *ptr= cit.getNext();
			for (ConfigFile::SettingsMultiMap::iterator dit= ptr->begin(); dit != ptr->end(); ++dit)
				resourceManager->addResourceLocation(dit->second, dit->first, name);
		}
		ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	}
	catch (Exception &e)
	{
		gkLogMessage("Failed to load resource file!\n" << e.getDescription());
	}
}


// ----------------------------------------------------------------------------
void gkEngine::addDebugProperty(gkVariable *prop)
{
}

// ----------------------------------------------------------------------------
void gkEngine::removeDebugProperty(gkVariable *prop)
{
}

// ----------------------------------------------------------------------------
Real gkEngine::getStepRate(void)
{
	return 1.0 / mTickRate;
}

// ----------------------------------------------------------------------------
Real gkEngine::getTickRate(void)
{
	return mTickRate;
}

// ----------------------------------------------------------------------------
Real gkEngine::getAnimRate(void)
{
	return mAnimRate;
}


// ----------------------------------------------------------------------------
#define ENGINE_TICKS_PER_SECOND Real(60)
#define GET_TICK(t) ((t).getMilliseconds())

// tick states
Real gkEngine::mTickRate= ENGINE_TICKS_PER_SECOND;
Real gkEngine::mAnimRate= 25;

// ----------------------------------------------------------------------------
// Game loop is based on the following articles:
// http://dewitters.koonsolo.com/gameloop.html
// http://gafferongames.wordpress.com/game-physics/fix-your-timestep/
// ----------------------------------------------------------------------------

void gkEngine::run(void)
{

	GK_ASSERT(mPrivate);
	if (!mPrivate->scene)
	{
		gkLogMessage("Can't run with out a registered scene. exiting");
		return;
	}

	gkWindowSystem *sys= mPrivate->windowsystem;
	if (!sys)
	{
		gkLogMessage("Can't run with out a window system. exiting");
		return;
	}

	mRoot->addFrameListener(mPrivate);

	// setup timer
	mRoot->clearEventTimes();
	mRoot->getRenderSystem()->_initRenderTargets();

	unsigned long ticks= 1000 / (unsigned long)mTickRate;
	unsigned long skip= (unsigned long(Real(mTickRate) / 5.0)), loop= 0;
	if (skip == 0) skip= 1;

	/// start time
	Timer t;
	t.reset();

	unsigned long nextTick= GET_TICK(t);
	do
	{
		/// update window messages per frame
		sys->processEvents();

		for (loop= 0; ((GET_TICK(t) > nextTick) && loop < skip); loop++, nextTick += ticks)
			mPrivate->tick(nextTick, getStepRate());

		mPrivate->frame_interpl= Real(GET_TICK(t) + ticks - nextTick) / Real(ticks);
		mRoot->renderOneFrame();
	}
	while (!sys->exitRequest());
	mRoot->removeFrameListener(mPrivate);
}


// ----------------------------------------------------------------------------
bool gkEnginePrivate::frameRenderingQueued(const FrameEvent& evt)
{
	// this will be called after the frame has been rendered
	// so it's behind one frame
	if (scene != 0)
		scene->update(frame_interpl, true); // smooth tick
	return true;
}

// ----------------------------------------------------------------------------
void gkEnginePrivate::tick(Real elapsed, Real dt)
{
	/// Proccess one full game tick
	GK_ASSERT(windowsystem);
	GK_ASSERT(scene);

	/// dispatch inputs
	windowsystem->dispatchEvents();

	/// update main scene
	scene->update(dt, false);

	/// clear per frame stuff
	windowsystem->endFrame();
}

// ----------------------------------------------------------------------------
void gkEngine::setActiveScene(gkSceneObject *sc)
{
	GK_ASSERT(mPrivate);
	mPrivate->scene= sc;
}

// ----------------------------------------------------------------------------
GK_IMPLEMENT_SINGLETON(gkEngine);
