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

#include "gkWindowSystem.h"
#include "gkLogger.h"
#include "gkUserDefs.h"
#include "gkCamera.h"
#include "gkEngine.h"
#include "gkScene.h"
#include "gkWindowSystem.h"
#include "gkWindow.h"
#include "gkWindowIOS.h"

#include "OgreRenderWindow.h"
#include "OgreRoot.h"
#include "OgreWindowEventUtilities.h"
#include "OIS.h"

@implementation gkGestureView

- (BOOL)canBecomeFirstResponder
{
	return YES;
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event 
{
	[self.nextResponder touchesBegan:event];
}

@end

gkWindowIOS::gkWindowIOS()
	:	m_itouch(0),
		m_gestureView(0)
{
}


gkWindowIOS::~gkWindowIOS()
{
	[m_gestureView release];

	if (m_input)
	{
		if (m_itouch)
			m_input->destroyInputObject(m_itouch);

		m_itouch = 0;
	}
}

bool gkWindowIOS::setupInput(const gkUserDefs& prefs)
{
	// OIS
	try
	{
		size_t handle = getWindowHandle();
		if (handle == 0) return false;

		OIS::ParamList params;

		params.insert(std::make_pair("WINDOW", Ogre::StringConverter::toString(handle)));
		m_input = OIS::InputManager::createInputSystem(params);
		m_input->enableAddOnFactory(OIS::InputManager::AddOn_All);

		m_gestureView = [[gkGestureView alloc] init];

		[[[UIApplication sharedApplication] keyWindow] addSubview: m_gestureView];

		[m_gestureView becomeFirstResponder];

		m_itouch = (OIS::MultiTouch*)m_input->createInputObject(OIS::OISMultiTouch, true);
		GK_ASSERT(m_itouch);
		m_itouch->setEventCallback(this);
	}
	catch (OIS::Exception& e)
	{
		gkPrintf("%s", e.what());
		return false;
	}

	return true;
}

void gkWindowIOS::dispatch(void)
{
	GK_ASSERT(m_itouch);

	m_itouch->capture();    //OIS don't thing, currently. so instead use a previous saved touch event

	if (m_mouse.buttons[gkMouse::Left] != GK_Pressed)
		m_mouse.moved = false;
}

void gkWindowIOS::process(void)
{
	[m_gestureView becomeFirstResponder];

	gkWindow::process();
}


//copied from ogre3d samplebrowser
void gkWindowIOS::transformInputState(OIS::MultiTouchState& state)
{
	GK_ASSERT(m_rwindow && m_rwindow->getViewport(0));

	Ogre::Viewport* viewport = m_rwindow->getViewport(0);

	int w = viewport->getActualWidth();
	int h = viewport->getActualHeight();
	int absX = state.X.abs;
	int absY = state.Y.abs;
	int relX = state.X.rel;
	int relY = state.Y.rel;

	switch (viewport->getOrientationMode())
	{
	case Ogre::OR_DEGREE_0:   //OR_PORTRAIT
		break;
	case Ogre::OR_DEGREE_90:  //OR_LANDSCAPERIGHT
		state.X.abs = w - absY;
		state.Y.abs = absX;
		state.X.rel = -relY;
		state.Y.rel = relX;
		break;
	case Ogre::OR_DEGREE_180:
		state.X.abs = w - absX;
		state.Y.abs = h - absY;
		state.X.rel = -relX;
		state.Y.rel = -relY;
		break;
	case Ogre::OR_DEGREE_270: //OR_LANDSCAPELEFT
		state.X.abs = absY;
		state.Y.abs = h - absX;
		state.X.rel = relY;
		state.Y.rel = -relX;
		break;
	}
}

bool gkWindowIOS::touchPressed(const OIS::MultiTouchEvent& arg)
{
	gkMouse& data = m_mouse;

	data.buttons[gkMouse::Left] = GK_Pressed;

	if (!m_listeners.empty())
	{
		gkWindowSystem::Listener* node = m_listeners.begin();
		while (node)
		{
			node->mousePressed(data);
			node = node->getNext();
		}
	}

	return true;
}

bool gkWindowIOS::touchReleased(const OIS::MultiTouchEvent& arg)
{
	gkMouse& data = m_mouse;

	data.buttons[gkMouse::Left] = GK_Released;

	if (!m_listeners.empty())
	{
		gkWindowSystem::Listener* node = m_listeners.begin();
		while (node)
		{
			node->mousePressed(data);
			node = node->getNext();
		}
	}

	return true;
}

bool gkWindowIOS::touchMoved(const OIS::MultiTouchEvent& arg)
{
	gkMouse& data = m_mouse;
	OIS::MultiTouchState state = arg.state;;

	transformInputState(state);

	data.position.x = (gkScalar)state.X.abs;
	data.position.y = (gkScalar)state.Y.abs;
	data.relative.x = (gkScalar)state.X.rel;
	data.relative.y = (gkScalar)state.Y.rel;
	data.moved = true;

	data.wheelDelta = 0;

	if (!m_listeners.empty())
	{
		gkWindowSystem::Listener* node = m_listeners.begin();
		while (node)
		{
			node->mouseMoved(data);
			node = node->getNext();
		}
	}

	return true;
}

bool gkWindowIOS::touchCancelled(const OIS::MultiTouchEvent& arg)
{
	return true;
}

