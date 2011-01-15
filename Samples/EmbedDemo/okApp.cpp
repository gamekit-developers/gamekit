/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 harkon.kr

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

#include "StdAfx.h"
#include "okApp.h"
#include "okUtils.h"
#include "Animation/gkAnimationManager.h"

#define DEFAULT_CONFIG_FILE "OgreKitStartup.cfg"
#define DEFAULT_LOG			"oklog.log"

#define DEFAULT_SCENE_NAME	"DefaultScene"
#define DEFAULT_CAMERA_NAME	"DefaultCamera"

UT_IMPLEMENT_SINGLETON(okApp)

okApp::okApp() 
	:	m_showPhysicsDebug(false), 
		m_inited(false),		
		m_winCount(0)
{
	gkLogger::enable(DEFAULT_LOG, true);
}

okApp::~okApp()
{
	if (m_inited)
		uninit();

	gkLogger::disable();
}

void  okApp::tick (gkScalar rate)
{
	gkCoreApplication::tick(rate);
}


gkScene* okApp::createEmptyScene()
{
	if (hasScene())
		return NULL;

	gkScene* scene = gkSceneManager::getSingleton().createEmptyScene();

	if (scene)
	{		
		scene->createInstance();
		GK_ASSERT(scene->getMainCamera() && scene->getMainCamera()->getCamera());
	
		m_scenes.push_back(scene);	
	}

	return scene;
}

bool okApp::setup(void)
{	
	if (!createEmptyScene())
	{
		gkPrintf("Can't create empty scene.");
		return false;
	}

    gkWindowSystem::getSingleton().addListener(this);

	gkPrintf("done\n");
    return true;
}


gkWindow* okApp::createWindow(const gkString& windowHandle, int winSizeX, int winSizeY)
{
	if (!isInited()) return false;

	gkUserDefs  prefs	= m_prefs;

	prefs.winsize.x     = winSizeX;
	prefs.winsize.y     = winSizeY;
	prefs.wintitle      = utStringFormat("okwin_%d", m_winCount++); 	
	prefs.extWinhandle	= windowHandle;

	gkWindow* window = gkWindowSystem::getSingleton().createWindow(prefs);

	return window;
}

void okApp::destroyWindow(gkWindow* win)
{
	if (!isInited()) return;

	gkWindowSystem::getSingleton().destroyWindow(win);
}

bool okApp::init(const gkString& cfg, const gkString& windowHandle, int winSizeX, int winSizeY)
{
	m_cfg = cfg;

	m_prefs.winsize.x        = winSizeX; 
	m_prefs.winsize.y        = winSizeY;
	m_prefs.wintitle         = utStringFormat("okwin_%d", m_winCount++); 
	m_prefs.verbose          = false; 
	m_prefs.grabInput        = false;
	m_prefs.debugPhysics     = false;
	m_prefs.debugPhysicsAabb = false;

	if (!m_cfg.empty())
	{
		gkPath path = m_cfg;

		// overide settings if found
		if (path.isFileInBundle()) 
		{
			m_prefs.load(path.getPath());
			gkPrintf("Load config file: %s", path.getPath().c_str());
		}
	}

	m_prefs.extWinhandle = windowHandle;

	if (!initialize()) //create engine & call this->setup
		return false;

	if (!m_engine->initializeStepLoop())
		return false;

	m_inited = true;

	return true;
}

void okApp::uninit()
{
	if (!m_inited) return;

	if (m_engine) 
	{
		m_engine->finalizeStepLoop();

		delete m_engine;
		m_engine = NULL;
	}

	m_scenes.clear();

	m_inited = false;
}

gkWindow* okApp::getMainWindow()
{
	return gkWindowSystem::getSingleton().getMainWindow();
}

void okApp::unloadAllScenes()
{	
	gkBlendLoader::getSingleton().unloadAll();

	m_scenes.clear();
}

void okApp::unloadScene(gkScene* scene)
{
	if (!scene)
		return;

	gkBlendFile* blend = scene->getLoadBlendFile();
	gkString groupName = scene->getGroupName();
		
	scene->destroyInstance();
	gkSceneManager::getSingleton().destroy(scene);

	if (blend)
		gkBlendLoader::getSingleton().unloadFile(blend);
	else
		gkResourceGroupManager::getSingleton().destroyResourceGroup(groupName); //destroy empty scene group

	m_scenes.erase(m_scenes.find(scene));		
}

gkScene* okApp::loadScene(gkWindow* window, const gkString& blend, const gkString& sceneName, bool ignoreCache)
{
	if (!m_inited) return NULL;

	GK_ASSERT(window);

	int options = gkBlendLoader::LO_ONLY_ACTIVE_SCENE | gkBlendLoader::LO_CREATE_UNIQUE_GROUP;
	if (ignoreCache) options |= gkBlendLoader::LO_IGNORE_CACHE_FILE;
	gkBlendFile* file = gkBlendLoader::getSingleton().loadFile(blend, options, sceneName);
	if (!file) 
	{
		gkPrintf("Can't open the blend file: %s", blend.c_str());
		return false;
	}

	gkScene* scene = NULL;
	
	if (!sceneName.empty())
		scene = file->getSceneByName(sceneName);

	if (!scene)	
		scene = file->getMainScene();

	if (!scene)
	{
		gkPrintf("Can't create the scene.");
		return false;
	}

	scene->setDisplayWindow(window);

	if (m_showPhysicsDebug)
		scene->getDynamicsWorld()->enableDebugPhysics(true, true);

	scene->createInstance();

	m_scenes.push_back(scene);

	return scene;
}

gkScene* okApp::mergeScene(gkScene* dstScene, const gkString& blend, const gkString& sceneName, bool ignoreCache)
{
	if (!m_inited) return NULL;

	GK_ASSERT(dstScene);

	int options = gkBlendLoader::LO_ONLY_ACTIVE_SCENE | gkBlendLoader::LO_CREATE_UNIQUE_GROUP;
	if (ignoreCache) options |= gkBlendLoader::LO_IGNORE_CACHE_FILE;
	gkBlendFile* file = gkBlendLoader::getSingleton().loadFile(blend, options, sceneName);
	if (!file) 
	{
		gkPrintf("Can't open the blend file: %s", blend.c_str());
		return false;
	}

	gkScene* scene = NULL;
	
	if (!sceneName.empty())
		scene = file->getSceneByName(sceneName);

	if (!scene)	
		scene = file->getMainScene();

	if (!scene)
	{
		gkPrintf("Can't create the scene.");
		return false;
	}

	gkSceneManager::getSingleton().copyObjects(scene, dstScene);
	
	//m_scenes.push_back(scene);

	return scene;
}


gkString okApp::getFirstSceneName()
{
	gkScene* scene = getFirstScene();
	return scene ? scene->getName() : "";
}

gkBlendFile* okApp::getFirstBlendFile()
{
	return m_scenes.empty() ? NULL : m_scenes[0]->getLoadBlendFile();
}

gkBlendFile* okApp::getBlendFile(const gkString& fname)
{
	return gkBlendLoader::getSingleton().getFileByName(fname);
}

bool okApp::changeScene(gkScene* scene, const gkString& sceneName)
{
	if (!m_inited || !scene) return false;

	if (scene->getName() == sceneName) return true;


	gkBlendFile* blend = scene->getLoadBlendFile();
	if (!blend) return false;

	gkScene* newScene = blend->getSceneByName(sceneName);
	if (!newScene) return false;

	if (scene)
	{
		if (scene == newScene)
			return false;

		scene->destroyInstance();
	}

	if (!newScene->isInstanced())
		newScene->createInstance();

	return true;
}


bool okApp::step()
{
	if (!m_inited) return false;

	return m_engine->stepOneFrame();	
}



void okApp::setShowPhysicsDebug(bool show)
{
	m_showPhysicsDebug = show;
	for (UTsize i = 0; i < m_scenes.size(); i++)
		m_scenes[i]->getDynamicsWorld()->enableDebugPhysics(show, true);	
}
