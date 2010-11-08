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
#include "gkWindowSystem.h"
#include "gkWindowSystemPrivate.h"

using namespace Ogre;

// Internal interface
gkWindowSystemPrivate::gkWindowSystemPrivate() :
	m_sys(0),
	m_input(0), 
	m_keyboard(0), 
	m_mouse(0)
{
}

gkWindowSystemPrivate::~gkWindowSystemPrivate()
{

	if (m_input)
	{
		// free input

		if (m_keyboard)
			m_input->destroyInputObject(m_keyboard);

		if (m_mouse)
			m_input->destroyInputObject(m_mouse);

		UTsize i;
		for (i = 0; i < m_joysticks.size(); ++i)
			m_input->destroyInputObject(m_joysticks[i]);


		OIS::InputManager::destroyInputSystem(m_input);

		m_input = 0; m_keyboard = 0; m_mouse = 0;
	}
}


bool gkWindowSystemPrivate::setup(gkWindowSystem* sys, const gkUserDefs& prefs)
{
	if (!sys || m_sys) 
		return false;

	m_sys = sys;

	// OIS
	try
	{
		size_t handle;
		m_sys->m_window->getCustomAttribute("WINDOW", &handle);


		OIS::ParamList params;

		if (!prefs.grabInput)
		{

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			params.insert(std::make_pair(std::string("w32_mouse"),		std::string("DISCL_FOREGROUND")));
			params.insert(std::make_pair(std::string("w32_mouse"),		std::string("DISCL_NONEXCLUSIVE")));
			params.insert(std::make_pair(std::string("w32_keyboard"),	std::string("DISCL_FOREGROUND")));
			params.insert(std::make_pair(std::string("w32_keyboard"),	std::string("DISCL_NONEXCLUSIVE")));

			if (m_sys->m_useExternalWindow)
				params.insert(std::make_pair(std::string("w32_pass_event"), std::string(""))); //pass event to old window proc

#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE
			params.insert(std::make_pair(std::string("MacAutoRepeatOn"), std::string("true")));
#else
			params.insert(std::make_pair(std::string("x11_mouse_grab"),		std::string("false")));
			params.insert(std::make_pair(std::string("x11_mouse_hide"),		std::string("false")));
			params.insert(std::make_pair(std::string("x11_keyboard_grab"),  std::string("false")));
			params.insert(std::make_pair(std::string("XAutoRepeatOn"),		std::string("true")));
#endif
		}

		params.insert(std::make_pair("WINDOW", StringConverter::toString(handle)));
		m_input = OIS::InputManager::createInputSystem(params);
		m_input->enableAddOnFactory(OIS::InputManager::AddOn_All);


		m_keyboard = (OIS::Keyboard*) m_input->createInputObject(OIS::OISKeyboard, true);
		m_keyboard->setEventCallback(this);

		m_mouse = (OIS::Mouse*)m_input->createInputObject(OIS::OISMouse, true);
		m_mouse->setEventCallback(this);


		for (int i = 0; i < m_input->getNumberOfDevices(OIS::OISJoyStick); i++)
		{
			OIS::JoyStick* oisjs = (OIS::JoyStick*)m_input->createInputObject(OIS::OISJoyStick, true);
			oisjs->setEventCallback(this);


			gkJoystick* gkjs = new gkJoystick(oisjs->getNumberOfComponents(OIS::OIS_Button), oisjs->getNumberOfComponents(OIS::OIS_Axis));

			m_joysticks.push_back(oisjs);
			m_sys->m_joysticks.push_back(gkjs);
		}

		const OIS::MouseState& st = m_mouse->getMouseState();
		st.width  = m_sys->m_mouse.winsize.x;
		st.height = m_sys->m_mouse.winsize.y;

	}
	catch (OIS::Exception& e)
	{
		gkPrintf("%s", e.what());
		return false;
	}

	return true;
}


void gkWindowSystemPrivate::dispatch(void)
{
	GK_ASSERT(m_mouse && m_keyboard);

	m_sys->m_mouse.moved = false;
	m_sys->m_mouse.wheelDelta = 0.f;
	m_sys->m_mouse.relitave.x = 0.f;
	m_sys->m_mouse.relitave.y = 0.f;

	m_mouse->capture();
	m_keyboard->capture();

	utArrayIterator<utArray<OIS::JoyStick*> > iter(m_joysticks);
	while (iter.hasMoreElements())
	{
		iter.peekNext()->capture();
		iter.getNext();
	}
}

void gkWindowSystemPrivate::process(void)
{
	if (!m_sys->m_useExternalWindow)
		WindowEventUtilities::messagePump();
}

bool gkWindowSystemPrivate::mouseMoved(const OIS::MouseEvent& arg)
{
	gkMouse& data = m_sys->m_mouse;

	data.position.x = (Real)arg.state.X.abs;
	data.position.y = (Real)arg.state.Y.abs;
	data.relitave.x = (Real)arg.state.X.rel;
	data.relitave.y = (Real)arg.state.Y.rel;
	data.moved = true;

	if (arg.state.Z.rel != 0)
		data.wheelDelta = arg.state.Z.rel > 0 ? 1.f : -1.f;
	else data.wheelDelta = 0;

	if (!m_sys->m_listeners.empty())
	{
		gkWindowSystem::Listener* node = m_sys->m_listeners.begin();
		while (node)
		{
			node->mouseMoved(data);
			node = node->getNext();
		}
	}

	return true;
}

bool gkWindowSystemPrivate::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	gkMouse& data = m_sys->m_mouse;

	switch (id)
	{
	case OIS::MB_Left:
		data.buttons[gkMouse::Left] = GK_Pressed;
		break;
	case OIS::MB_Middle:
		data.buttons[gkMouse::Middle] = GK_Pressed;
		break;
	case OIS::MB_Right:
		data.buttons[gkMouse::Right] = GK_Pressed;
		break;
	}

	if (!m_sys->m_listeners.empty())
	{
		gkWindowSystem::Listener* node = m_sys->m_listeners.begin();
		while (node)
		{
			node->mousePressed(data);
			node = node->getNext();
		}
	}

	return true;
}

bool gkWindowSystemPrivate::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	gkMouse& data = m_sys->m_mouse;

	switch (id)
	{
	case OIS::MB_Left:
		data.buttons[gkMouse::Left] = GK_Released;
		break;
	case OIS::MB_Middle:
		data.buttons[gkMouse::Middle] = GK_Released;
		break;
	case OIS::MB_Right:
		data.buttons[gkMouse::Right] = GK_Released;
		break;
	}

	if (!m_sys->m_listeners.empty())
	{
		gkWindowSystem::Listener* node = m_sys->m_listeners.begin();
		while (node)
		{
			node->mouseReleased(data);
			node = node->getNext();
		}
	}

	return true;
}

bool gkWindowSystemPrivate::keyPressed(const OIS::KeyEvent& arg)
{
	gkKeyboard& key = m_sys->m_keyboard;

	int kc = getCode(arg.key);
	key.keys[kc] = GK_Pressed;
	key.key_count += 1;


	if (!m_sys->m_listeners.empty())
	{
		gkWindowSystem::Listener* node = m_sys->m_listeners.begin();
		while (node)
		{
			node->keyPressed(key, (gkScanCode)kc);
			node = node->getNext();
		}
	}

	return true;
}

bool gkWindowSystemPrivate::keyReleased(const OIS::KeyEvent& arg)
{
	gkKeyboard& key = m_sys->m_keyboard;

	int kc = getCode(arg.key);
	key.keys[kc] = GK_Released;
	key.key_count -= 1;

	if (!m_sys->m_listeners.empty())
	{
		gkWindowSystem::Listener* node = m_sys->m_listeners.begin();
		while (node)
		{
			node->keyReleased(key, (gkScanCode)kc);
			node = node->getNext();
		}
	}


	return true;
}

bool gkWindowSystemPrivate::buttonPressed(const OIS::JoyStickEvent& arg, int button)
{
	// Hum we assume coresponding gk and OIs joystick have the same place in their array
	gkJoystick& js = *m_sys->m_joysticks[m_joysticks.find((OIS::JoyStick*)arg.device)];

	js.buttons[button] = GK_Pressed;
	js.buttonCount += 1;

	if (!m_sys->m_listeners.empty())
	{
		gkWindowSystem::Listener* node = m_sys->m_listeners.begin();
		while (node)
		{
			node->joystickPressed(js, button);
			node = node->getNext();
		}
	}

	return true;
}

bool gkWindowSystemPrivate::buttonReleased(const OIS::JoyStickEvent& arg, int button)
{
	// Hum we assume coresponding gk and OIs joystick have the same place in their array
	gkJoystick& js = *m_sys->m_joysticks[m_joysticks.find((OIS::JoyStick*)arg.device)];

	js.buttons[button] = GK_Released;
	js.buttonCount -= 1;

	if (!m_sys->m_listeners.empty())
	{
		gkWindowSystem::Listener* node = m_sys->m_listeners.begin();
		while (node)
		{
			node->joystickReleased(js, button);
			node = node->getNext();
		}
	}

	return true;
}

bool gkWindowSystemPrivate::axisMoved(const OIS::JoyStickEvent& arg, int axis)
{
	// Hum we assume coresponding gk and OIs joystick have the same place in their array
	gkJoystick& js = *m_sys->m_joysticks[m_joysticks.find((OIS::JoyStick*)arg.device)];

	js.axes[axis] = arg.state.mAxes[axis].abs;
	if (!m_sys->m_listeners.empty())
	{
		gkWindowSystem::Listener* node = m_sys->m_listeners.begin();
		while (node)
		{
			node->joystickMoved(js, axis);
			node = node->getNext();
		}
	}
	return true;
}

void gkWindowSystemPrivate::windowResized(RenderWindow* rw)
{
	unsigned short sz = rw->getNumViewports();
	for (unsigned short i = 0; i < sz; ++i)
	{
		Viewport* vp = rw->getViewport(i);

		// We assume all viewports are "main" vieport
		m_sys->setMainViewportDimension(vp);

		Camera* cam = vp->getCamera();
		cam->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));

		const OIS::MouseState& state = m_mouse->getMouseState();

		state.width = gkMax<int>(state.width, vp->getActualWidth());
		state.height = gkMax<int>(state.height, vp->getActualHeight());

		m_sys->m_mouse.winsize.x = (Real)state.width;
		m_sys->m_mouse.winsize.y = (Real)state.height;
	}

	// Ogre keep Y field of view constant, we want to keep X fov constant
	gkScene* scene = gkEngine::getSingleton().getActiveScene();
	if (scene)
	{
		gkCamera* cam = scene->getMainCamera();
		if (cam)
			cam->setFov(gkDegree(cam->getFov()));
	}
}


void gkWindowSystemPrivate::windowClosed(RenderWindow* rw)
{
	GK_ASSERT(m_sys);
	m_sys->m_exit = true;
}


int gkWindowSystemPrivate::getCode(int kc)
{
#define CASE(ret, c) case (c): return ret

	switch (kc)
	{
		CASE(KC_ACCENTGRAVEKEY   ,    OIS::KC_GRAVE);
		CASE(KC_ZEROKEY          ,    OIS::KC_0);
		CASE(KC_ONEKEY           ,    OIS::KC_1);
		CASE(KC_TWOKEY           ,    OIS::KC_2);
		CASE(KC_THREEKEY         ,    OIS::KC_3);
		CASE(KC_FOURKEY          ,    OIS::KC_4);
		CASE(KC_FIVEKEY          ,    OIS::KC_5);
		CASE(KC_SIXKEY           ,    OIS::KC_6);
		CASE(KC_SEVENKEY         ,    OIS::KC_7);
		CASE(KC_EIGHTKEY         ,    OIS::KC_8);
		CASE(KC_NINEKEY          ,    OIS::KC_9);
		CASE(KC_PAD0             ,    OIS::KC_NUMPAD0);
		CASE(KC_PAD1             ,    OIS::KC_NUMPAD1);
		CASE(KC_PAD2             ,    OIS::KC_NUMPAD2);
		CASE(KC_PAD3             ,    OIS::KC_NUMPAD3);
		CASE(KC_PAD4             ,    OIS::KC_NUMPAD4);
		CASE(KC_PAD5             ,    OIS::KC_NUMPAD5);
		CASE(KC_PAD6             ,    OIS::KC_NUMPAD6);
		CASE(KC_PAD7             ,    OIS::KC_NUMPAD7);
		CASE(KC_PAD8             ,    OIS::KC_NUMPAD8);
		CASE(KC_PAD9             ,    OIS::KC_NUMPAD9);
		CASE(KC_PADSLASHKEY      ,    OIS::KC_DIVIDE);
		CASE(KC_PADENTER         ,    OIS::KC_NUMPADENTER);
		CASE(KC_PADEQUALS        ,    OIS::KC_NUMPADEQUALS);
		CASE(KC_PADPERIOD        ,    OIS::KC_DECIMAL);
		CASE(KC_PADASTERKEY      ,    OIS::KC_MULTIPLY);
		CASE(KC_PADPLUSKEY       ,    OIS::KC_ADD);
		CASE(KC_AKEY             ,    OIS::KC_A);
		CASE(KC_BKEY             ,    OIS::KC_B);
		CASE(KC_CKEY             ,    OIS::KC_C);
		CASE(KC_DKEY             ,    OIS::KC_D);
		CASE(KC_EKEY             ,    OIS::KC_E);
		CASE(KC_FKEY             ,    OIS::KC_F);
		CASE(KC_GKEY             ,    OIS::KC_G);
		CASE(KC_HKEY             ,    OIS::KC_H);
		CASE(KC_IKEY             ,    OIS::KC_I);
		CASE(KC_JKEY             ,    OIS::KC_J);
		CASE(KC_KKEY             ,    OIS::KC_K);
		CASE(KC_LKEY             ,    OIS::KC_L);
		CASE(KC_MKEY             ,    OIS::KC_M);
		CASE(KC_NKEY             ,    OIS::KC_N);
		CASE(KC_OKEY             ,    OIS::KC_O);
		CASE(KC_PKEY             ,    OIS::KC_P);
		CASE(KC_QKEY             ,    OIS::KC_Q);
		CASE(KC_RKEY             ,    OIS::KC_R);
		CASE(KC_SKEY             ,    OIS::KC_S);
		CASE(KC_TKEY             ,    OIS::KC_T);
		CASE(KC_UKEY             ,    OIS::KC_U);
		CASE(KC_VKEY             ,    OIS::KC_V);
		CASE(KC_WKEY             ,    OIS::KC_W);
		CASE(KC_XKEY             ,    OIS::KC_X);
		CASE(KC_YKEY             ,    OIS::KC_Y);
		CASE(KC_ZKEY             ,    OIS::KC_Z);
		CASE(KC_CAPSLOCKKEY      ,    OIS::KC_CAPITAL);
		CASE(KC_LEFTCTRLKEY      ,    OIS::KC_LCONTROL);
		CASE(KC_LEFTALTKEY       ,    OIS::KC_LMENU);
		CASE(KC_RIGHTALTKEY      ,    OIS::KC_RMENU);
		CASE(KC_RIGHTCTRLKEY     ,    OIS::KC_RCONTROL);
		CASE(KC_RIGHTSHIFTKEY    ,    OIS::KC_RSHIFT);
		CASE(KC_LEFTSHIFTKEY     ,    OIS::KC_LSHIFT);
		CASE(KC_ESCKEY           ,    OIS::KC_ESCAPE);
		CASE(KC_TABKEY           ,    OIS::KC_TAB);
		CASE(KC_RETKEY           ,    OIS::KC_RETURN);
		CASE(KC_SPACEKEY         ,    OIS::KC_SPACE);
		CASE(KC_BACKSPACEKEY     ,    OIS::KC_BACK);
		CASE(KC_DELKEY           ,    OIS::KC_DELETE);
		CASE(KC_SEMICOLONKEY     ,    OIS::KC_SEMICOLON);
		CASE(KC_PERIODKEY        ,    OIS::KC_PERIOD);
		CASE(KC_COMMAKEY         ,    OIS::KC_COMMA);
		CASE(KC_QUOTEKEY         ,    OIS::KC_APOSTROPHE);
		CASE(KC_MINUSKEY         ,    OIS::KC_MINUS);
		CASE(KC_SLASHKEY         ,    OIS::KC_SLASH);
		CASE(KC_BACKSLASHKEY     ,    OIS::KC_BACKSLASH);
		CASE(KC_EQUALKEY         ,    OIS::KC_EQUALS);
		CASE(KC_LEFTARROWKEY     ,    OIS::KC_LEFT);
		CASE(KC_DOWNARROWKEY     ,    OIS::KC_DOWN);
		CASE(KC_RIGHTARROWKEY    ,    OIS::KC_RIGHT);
		CASE(KC_UPARROWKEY       ,    OIS::KC_UP);
		CASE(KC_F1KEY            ,    OIS::KC_F1);
		CASE(KC_F2KEY            ,    OIS::KC_F2);
		CASE(KC_F3KEY            ,    OIS::KC_F3);
		CASE(KC_F4KEY            ,    OIS::KC_F4);
		CASE(KC_F5KEY            ,    OIS::KC_F5);
		CASE(KC_F6KEY            ,    OIS::KC_F6);
		CASE(KC_F7KEY            ,    OIS::KC_F7);
		CASE(KC_F8KEY            ,    OIS::KC_F8);
		CASE(KC_F9KEY            ,    OIS::KC_F9);
		CASE(KC_F10KEY           ,    OIS::KC_F10);
		CASE(KC_F11KEY           ,    OIS::KC_F11);
		CASE(KC_F12KEY           ,    OIS::KC_F12);
		CASE(KC_HOMEKEY          ,    OIS::KC_HOME);
		CASE(KC_PAGEUPKEY        ,    OIS::KC_PGUP);
		CASE(KC_PAGEDOWNKEY      ,    OIS::KC_PGDOWN);
		CASE(KC_ENDKEY           ,    OIS::KC_END);
		CASE(KC_INSERTKEY        ,    OIS::KC_INSERT);
		CASE(KC_PAUSEKEY         ,    OIS::KC_PAUSE);
		CASE(KC_UNKNOWNKEY       ,    OIS::KC_AT);
		CASE(KC_UNKNOWNKEY       ,    OIS::KC_NUMLOCK);
		CASE(KC_UNKNOWNKEY       ,    OIS::KC_RWIN);
		CASE(KC_UNKNOWNKEY       ,    OIS::KC_SCROLL);
	}
	return KC_NONE;

#undef CASE
}
