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
#include "gkCommon.h"
#include "gkWindowSystem.h"
#include "gkLogger.h"
#include "gkUserDefs.h"
#include "gkCamera.h"
#include "gkEngine.h"
#include "gkScene.h"
#include "gkWindow.h"

#include "OgreRenderWindow.h"
#include "OgreRoot.h"
#include "OgreWindowEventUtilities.h"
#include "OIS.h"

#ifdef OGREKIT_BUILD_IPHONE
#include "gkWindowIOS.h"
#endif

#ifdef OGREKIT_BUILD_ANDROID
#include "gkWindowAndroid.h"
#endif


//using namespace Ogre;

UT_IMPLEMENT_SINGLETON(gkWindowSystem);

gkWindowSystem::gkWindowSystem() 
	:	m_exit(false)
{

}


gkWindowSystem::~gkWindowSystem()
{
	UTsize i;
	for (i = 0; i < m_windows.size(); i++)
		delete m_windows[i];

	m_windows.clear();
}


// Creates the render window, and sets up the OIS input system
gkWindow* gkWindowSystem::createWindow(const gkUserDefs& prefs)
{
#ifdef OGREKIT_BUILD_IPHONE
	gkWindow* window = new gkWindowIOS();
#elif defined(OGREKIT_BUILD_ANDROID)
	gkWindow* window = new gkWindowAndroid();
#else
	gkWindow* window = new gkWindow();
#endif

	if (!window->createWindow(this, prefs))
	{
		delete window;
		gkPrintf("Unable setup gkWindow object.");
		return 0;
	}

	m_windows.push_back(window);

	return window;
}

void gkWindowSystem::destroyWindow(gkWindow* window)
{
	if (!window) return;

	delete window;

	m_windows.erase(m_windows.find(window));
}


gkWindow* gkWindowSystem::getMainWindow(void)
{
	return m_windows.empty() ? NULL : m_windows[0];
}

Ogre::RenderWindow* gkWindowSystem::getMainRenderWindow(void)
{
	gkWindow* window = getMainWindow(); GK_ASSERT(window);
	return window ? window->getRenderWindow() : NULL;
}

void gkWindowSystem::addListener(Listener* l)
{
	gkWindow* window = getMainWindow(); GK_ASSERT(window);	
	if (window) window->addListener(l);
}

void gkWindowSystem::removeListener(Listener* l)
{
	gkWindow* window = getMainWindow(); GK_ASSERT(window);	
	if (window) window->removeListener(l);
}

// Call platform event loop
void gkWindowSystem::process(void)
{
	UTsize i;
	for (i = 0; i < m_windows.size(); i++)
		m_windows[i]->process();
}

// Handle platform messages
void gkWindowSystem::dispatch(void)
{
	UTsize i;
	for (i = 0; i < m_windows.size(); i++)
		m_windows[i]->dispatch();
}


void gkWindowSystem::clearStates(void)
{
	UTsize i;
	for (i = 0; i < m_windows.size(); i++)
		m_windows[i]->clearStates();
}

gkKeyboard* gkWindowSystem::getKeyboard(void)      
{
	GK_ASSERT(getMainWindow());
	return getMainWindow()->getKeyboard();
}

gkMouse* gkWindowSystem::getMouse(void)            
{
	GK_ASSERT(getMainWindow());
	return getMainWindow()->getMouse();
}

unsigned int gkWindowSystem::getNumJoysticks(void) 
{
	GK_ASSERT(getMainWindow());
	return getMainWindow()->getNumJoysticks();
}

gkJoystick* gkWindowSystem::getJoystick(int index) 
{
	GK_ASSERT(getMainWindow());
	return getMainWindow()->getJoystick(index);
}
