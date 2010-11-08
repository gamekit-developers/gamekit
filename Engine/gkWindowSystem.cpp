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
#include "OgreRenderWindow.h"
#include "OgreRoot.h"
#include "OgreWindowEventUtilities.h"
#include "OIS.h"
#include "gkWindowSystem.h"
#include "gkLogger.h"
#include "gkUserDefs.h"
#include "gkCamera.h"
#include "gkEngine.h"
#include "gkScene.h"
#include "gkWindowSystemPrivate.h"

#ifdef OGREKIT_BUILD_IPHONE
#include "gkWindowSystemPrivateIOS.h"
#endif


using namespace Ogre;

UT_IMPLEMENT_SINGLETON(gkWindowSystem);

gkWindowSystem::gkWindowSystem() 
:	m_internal(0),
	m_window(0), 
	m_exit(false), 
	m_requestedWidth(0), 
	m_requestedHeight(0),
	m_framingType(0),
	m_useExternalWindow(false)
{
#ifdef OGREKIT_BUILD_IPHONE
	m_internal = new gkWindowSystemPrivateIOS();
#else
	m_internal = new gkWindowSystemPrivate();
#endif
}


gkWindowSystem::~gkWindowSystem()
{
	if (m_window)
		WindowEventUtilities::removeWindowEventListener(m_window, m_internal);


	UTsize i;
	for (i = 0; i < m_joysticks.size(); ++i)
		delete m_joysticks[i];

	m_joysticks.clear();

	delete m_internal;
}


// Creates the main Ogre window, and sets up the OIS input system
RenderWindow* gkWindowSystem::createMainWindow(const gkUserDefs& prefs)
{
	int winsizex, winsizey;

	// one window for now
	if (m_window)
		return 0;

	m_requestedWidth = (int)(prefs.winsize.x + 0.5f);
	m_requestedHeight = (int)(prefs.winsize.y + 0.5f);
	m_framingType = prefs.framingType;

	Ogre::NameValuePairList params;

	if (prefs.fsaa)
	{
		params["FSAA"] = Ogre::StringConverter::toString(prefs.fsaaSamples);
	}

	if (!prefs.extWinhandle.empty())
	{
		params["externalWindowHandle"] = prefs.extWinhandle;
		m_useExternalWindow = true;
	}

	if (prefs.fullscreen)
	{
		Ogre::RenderSystem* rsys = Root::getSingleton().getRenderSystem();
		Ogre::ConfigOptionMap options = rsys->getConfigOptions();
		Ogre::ConfigOption modeOption = options["Video Mode"];
		bool found = false;

		gkPrintf("Available video modes:");
		for (size_t i = 0; i < modeOption.possibleValues.size(); i++)
		{
			gkString modeStr = modeOption.possibleValues[i];
			gkPrintf("%s\n", modeStr.c_str());

			if (!found)
			{
				int modex, modey;
				modex = Ogre::StringConverter::parseInt( modeStr.substr(0, 4));
				modey = Ogre::StringConverter::parseInt( modeStr.substr(7, 4));

				if (modex >= m_requestedWidth && modey >= m_requestedHeight)
				{
					found = true;
					winsizex = modex;
					winsizey = modey;
				}
			}
		}
		if (found)
		{
			gkPrintf("Best video mode found: %i x %i, request was %i x %i\n", winsizex, winsizey, (int)prefs.winsize.x, (int)prefs.winsize.y);
		}
		else
		{
			gkPrintf("Unable to find a video mode with request minimun resolution: %i x %i\n", (int)prefs.winsize.x, (int)prefs.winsize.y);
			return 0;
		}
	}
	else
	{
		winsizex = m_requestedWidth;
		winsizey = m_requestedHeight;
	}

	m_window = Root::getSingleton().createRenderWindow(prefs.wintitle,
	           winsizex, winsizey, prefs.fullscreen, &params);
	m_window->setActive(true);

	// copy window size (used later for hit testing)
	m_mouse.winsize.x = winsizex;
	m_mouse.winsize.y = winsizey;


	if (!m_internal->setup(this, prefs))
	{
		gkPrintf("Unable setup gkWindowSystem internal object.");
		return 0;
	}

	WindowEventUtilities::addWindowEventListener(m_window, m_internal);
	return m_window;
}


RenderWindow* gkWindowSystem::getMainWindow(void)
{
	return m_window;
}

Viewport* gkWindowSystem::addMainViewport(gkCamera* cam)
{
	if (m_window)
	{
		Viewport* vp =  m_window->addViewport(cam->getCamera());
		setMainViewportDimension(vp);
		return vp;
	}
	return 0;
}

void gkWindowSystem::setMainViewportDimension(Viewport* viewport)
{
	float l = 0.0;
	float r = 1.0;
	float t = 0.0;
	float b = 1.0;
	int w = m_window->getWidth();
	int h = m_window->getHeight();

	if (w != m_requestedWidth || h != m_requestedHeight)
	{
		switch (m_framingType)
		{
		case FRAMING_CROP:
			{
				l = (w - m_requestedWidth) / (2.0f * w);
				r = (w + m_requestedWidth) / (2.0f * w);
				t = (h - m_requestedHeight) / (2.0f * h);
				b = (h + m_requestedHeight) / (2.0f * h);
				break;
			}
		case FRAMING_LETTERBOX:
			{
				float hratio = (float)m_requestedWidth / (float)w;
				float vratio = (float)m_requestedHeight / (float)h;

				if (hratio > vratio)
				{
					l = 0;
					r = 1;
					t = (1 - (vratio / hratio)) / 2.0f;
					b = t + (vratio / hratio);
				}
				else
				{

					t = 0;
					b = 1;
					l = (1 - (hratio / vratio)) / 2.0f;
					r = l + (hratio / vratio);
				}
				break;
			}
		}

	}

	viewport->setDimensions(l, t, r - l, b - t);
}

void gkWindowSystem::addListener(Listener* l)
{
	m_listeners.push_back(l);
}

void gkWindowSystem::removeListener(Listener* l)
{
	m_listeners.erase(l);
}

// Call platform event loop
void gkWindowSystem::process(void)
{
	m_internal->process();
}

// Handle platform messages
void gkWindowSystem::dispatch(void)
{
	m_internal->dispatch();
}


void gkWindowSystem::clearStates(void)
{
	m_mouse.clear();
	m_keyboard.clear();

	UTsize i;
	for (i = 0; i < m_joysticks.size(); ++i)
		m_joysticks[i]->clear();
}

