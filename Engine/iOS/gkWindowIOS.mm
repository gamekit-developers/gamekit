/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 harkon.kr

    Contributor(s): Jonathan.
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

#include <vector>

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
		m_iacc(0),
		m_gestureView(0)
{
}


gkWindowIOS::~gkWindowIOS()
{
	[m_gestureView release];

	if (m_input)
	{
		if (m_itouch)
		{
			m_input->destroyInputObject(m_itouch);
			m_itouch = 0;
		}
		
		if (m_iacc)
		{
			m_input->destroyInputObject(m_iacc);
			m_iacc = 0;
		}
		
		UTsize i;
		for (i=0; i<m_joysticks.size(); ++i)
			delete m_joysticks[i];
		
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
		
		m_joysticks.resize(OIS_MAX_NUM_TOUCHES+1,0);
		UTsize i;
		for (i=1; i<m_joysticks.size(); ++i)
			m_joysticks[i] = new gkJoystick(1, 2);

		try 
		{
			m_iacc = (OIS::JoyStick*)m_input->createInputObject(OIS::OISJoyStick, true);
			GK_ASSERT(m_iacc);
			m_iacc->setEventCallback(this);
		}		
		catch (OIS::Exception&) 
		{
			m_iacc = 0;
		}
		
		m_joysticks[0] = new gkJoystick(0,0);
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
	
	const std::vector<OIS::MultiTouchState>& states = m_itouch->getMultiTouchStates();

	UTsize i;
	for(i=1; i<m_joysticks.size(); ++i)
	{
		const OIS::MultiTouchState& state = states[i-1];
		gkJoystick* js = m_joysticks[i];
		
		switch(state.touchType)
		{
			case OIS::MT_Pressed:
			case OIS::MT_Moved:
				js->buttons[0] = GK_Pressed;
				break;
			
			case OIS::MT_Released:
				js->buttons[0] = GK_Released;
				break;
			
			default:
				js->buttons[0] = GK_NullState;
		}
		
		js->axes[0] = state.X.abs;
		js->axes[1] = state.Y.abs;
		
		js->relAxes[0] = state.X.rel;
		js->relAxes[1] = state.Y.rel;
		
		js->buttonCount = state.tapCount;
	}
	
	m_itouch->capture();
	
	if (m_mouse.buttons[gkMouse::Left] != GK_Pressed)
		m_mouse.moved = false;

	if(m_iacc)
		m_iacc->capture();
}

void gkWindowIOS::process(void)
{
	[m_gestureView becomeFirstResponder];

	gkWindow::process();
}

void gkWindowIOS::clearStates(void)
{
	UTsize i;
	for (i=0; i<m_joysticks.size(); ++i)
		m_joysticks[i]->clear();
	
	gkWindow::clearStates();
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
// untested patch by afterbeta
// see: http://code.google.com/p/gamekit/issues/detail?id=264
	{
		gkMouse& data = m_mouse;		
 		OIS::MultiTouchState state = arg.state;
		
		transformInputState(state);
		
		data.position.x = (gkScalar)state.X.abs;
		data.position.y = (gkScalar)state.Y.abs;
		data.relative.x = (gkScalar)state.X.rel;
		data.relative.y = (gkScalar)state.Y.rel;
        	data.buttons[gkMouse::Left] = GK_Pressed;
		data.moved = false;
		
		data.wheelDelta = 0;
	}
// on problems fall back to this:
//	{
//		m_mouse.buttons[gkMouse::Left] = GK_Pressed;
//	}
	
	int fid = arg.state.fingerID+1;
	if (fid == 0)
		return true;
	
	gkJoystick& data = *m_joysticks[fid];
		
	data.buttons[0] = GK_Pressed;
	data.buttonsPressed[0] = GK_Pressed;
	data.axes[0] = arg.state.X.abs;
	data.axes[1] = arg.state.Y.abs;
	data.relAxes[0] = 0;
	data.relAxes[1] = 0;
	data.buttonCount = arg.state.tapCount;
	
	if (!m_listeners.empty())
	{
		gkWindowSystem::Listener* node = m_listeners.begin();
		while (node)
		{
			node->mousePressed(m_mouse);
			node->joystickPressed(data,0);
			node = node->getNext();
		}
	}
	
	return true;
}

bool gkWindowIOS::touchReleased(const OIS::MultiTouchEvent& arg)
{
	{
		m_mouse.buttons[gkMouse::Left] = GK_Released;
	}
	
	int fid = arg.state.fingerID+1;
	if (fid == 0)
		return true;
	
	gkJoystick& data = *m_joysticks[fid];

	data.buttons[0] = GK_Released;
	data.buttonsPressed[0] = GK_NullState;
	data.axes[0] = arg.state.X.abs;
	data.axes[1] = arg.state.Y.abs;
	data.relAxes[0] = 0;
	data.relAxes[1] = 0;
	data.buttonCount = arg.state.tapCount;
		
	if (!m_listeners.empty())
	{
		gkWindowSystem::Listener* node = m_listeners.begin();
		while (node)
		{
			node->mousePressed(m_mouse);
			node->joystickReleased(data,0);
			node = node->getNext();
		}
	}

	return true;
}

bool gkWindowIOS::touchMoved(const OIS::MultiTouchEvent& arg)
{
	{
		gkMouse& data = m_mouse;		
 		OIS::MultiTouchState state = arg.state;
		
		transformInputState(state);
		
		data.position.x = (gkScalar)state.X.abs;
		data.position.y = (gkScalar)state.Y.abs;
		data.relative.x = (gkScalar)state.X.rel;
		data.relative.y = (gkScalar)state.Y.rel;
		data.moved = true;
		
		data.wheelDelta = 0;
		
	}
	
	int fid = arg.state.fingerID+1;
	if (fid == 0)
		return true;
	
	gkJoystick& data = *m_joysticks[fid];
	OIS::MultiTouchState state = arg.state;
		
	transformInputState(state);
		
	data.buttons[0] = GK_Pressed;
	data.buttonsPressed[0] = GK_NullState;
	data.axes[0] = state.X.abs;
	data.axes[1] = state.Y.abs;
	data.relAxes[0] = arg.state.X.rel;
	data.relAxes[1] = arg.state.Y.rel;
	data.buttonCount = arg.state.tapCount;

	if (!m_listeners.empty())
	{
		gkWindowSystem::Listener* node = m_listeners.begin();
		while (node)
		{
			node->mouseMoved(m_mouse);
			node->joystickMoved(data,0);
			node = node->getNext();
		}
	}
	
	return true;
}

bool gkWindowIOS::touchCancelled(const OIS::MultiTouchEvent& arg)
{
	return true;
}

bool gkWindowIOS::axisMoved(const OIS::JoyStickEvent& arg, int i)
{
	const OIS::Vector3& arg_accel = arg.state.mVectors[0];
	gkVector3& accel = m_joysticks[0]->accel;
	
	accel.x = arg_accel.x;
	accel.y = arg_accel.y;
	accel.z = arg_accel.z;

	if (!m_listeners.empty())
	{
		gkWindowSystem::Listener* node = m_listeners.begin();
		while (node)
		{
			node->joystickMoved(*m_joysticks[0], 0);
			node = node->getNext();
		}
	}

	return true;
}


