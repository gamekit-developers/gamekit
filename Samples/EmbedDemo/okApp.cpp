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


#define DEFAULT_BLEND_FILE	"momo_ogre.blend"
#define DEFAULT_CONFIG_FILE "OgreKitStartup.cfg"
#define DEFAULT_LOG			"oklog.log"

#define DEFAULT_RES_GROUP	"okapp_res"
#define DEFAULT_SCENE_NAME	"DefaultScene"
#define DEFAULT_CAMERA_NAME	"DefaultCamera"

okApp::okApp() 
	: m_scene(NULL), m_showDebug(false), m_inited(false)
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


bool okApp::setup(void)
{
    gkBlendFile *blend = gkBlendLoader::getSingleton().loadFile(gkUtils::getFile(m_blend), gkBlendLoader::LO_ALL_SCENES, DEFAULT_RES_GROUP);
    if (!blend)
    {
        gkPrintf("File loading failed. %s\n", m_blend.c_str());
		gkPrintf("Create Default Empty Scene. %s\n", DEFAULT_SCENE_NAME);

		m_scene = (gkScene*)gkSceneManager::getSingleton().create(DEFAULT_SCENE_NAME);

		if (m_scene)
		{
			m_scene->createCamera(DEFAULT_CAMERA_NAME);
			m_engine->setActiveScene(m_scene);
		}
    }
	else
	{
		m_scene = blend->getMainScene();	
	}

	
	if (!m_scene)
	{
		gkPrintf("No usable scenes found in blend. %s\n", m_blend.c_str());
		return false;
	}

	m_scene->createInstance();

    gkWindowSystem::getSingleton().addListener(this);

	gkPrintf("done\n");
    return true;
}


bool okApp::init(const gkString& blend, const gkString& cfg, const gkString& windowHandle, int winSizeX, int winSizeY)
{
	if (!blend.empty())
		m_blend = blend;

	m_cfg = cfg;

	if (m_blend.empty())
		m_blend = DEFAULT_BLEND_FILE; 
	if (m_cfg.empty())
		m_cfg = DEFAULT_CONFIG_FILE;


	m_prefs.winsize.x        = winSizeX;
	m_prefs.winsize.y        = winSizeY;
	m_prefs.wintitle         = ""; 
	m_prefs.verbose          = false; 
	m_prefs.grabInput        = false;
	m_prefs.debugPhysics     = false;
	m_prefs.debugPhysicsAabb = false;

	gkPath path = m_cfg;

	// overide settings if found
	if (path.isFileInBundle()) 
	{
		m_prefs.load(path.getPath());
		gkPrintf("Load config file: %s", path.getPath().c_str());
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

	m_inited = false;
}

void okApp::unload()
{
	gkBlendLoader::getSingleton().clearResourceGroup(DEFAULT_RES_GROUP);	
	//m_engine->clearAllScenes();
	//m_engine->unloadAllResources();

	gkSceneManager::getSingleton().destroyAllInstances();
	gkSceneManager::getSingleton().destroyAll();
	gkGameObjectManager::getSingleton().destroyAll();

	gkGroupManager::getSingleton().destroyAll();
	gkTextManager::getSingleton().destroyAll();
	gkSkeletonManager::getSingleton().destroyAll();
	gkMeshManager::getSingleton().destroyAll();
	gkBlendLoader::getSingleton().unloadAll();	
}

bool okApp::load(const gkString& blend)
{
	if (!m_inited) return false;

	GK_ASSERT(m_scene);

	unload();

	m_scene = NULL;
	
	gkBlendFile* file = gkBlendLoader::getSingleton().loadFile(blend, gkBlendLoader::LO_ALL_SCENES, DEFAULT_RES_GROUP);
	if (!file) 
	{
		gkPrintf("Can't open the blend file: %s", blend.c_str());
		return false;
	}

	m_scene = file->getMainScene();
	if (!m_scene)
	{
		gkPrintf("Can't create the scene.");
		return false;
	}

	m_scene->createInstance();
	m_blend = blend;

	return true;
}

bool okApp::step()
{
	if (!m_inited) return false;

	return m_engine->stepOneFrame();	
}


void okApp::keyReleased(const gkKeyboard& key, const gkScanCode& sc)
{
    if (sc == KC_ESCKEY) m_engine->requestExit();

	if (sc == KC_CKEY) 
	{
		m_showDebug = !m_showDebug;
		m_scene->getDynamicsWorld()->enableDebugPhysics(m_showDebug, true);
	}
}
