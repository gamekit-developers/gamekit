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

using namespace Ogre;

#define gkWindowSystemPrivate gkWindowSystem::Private

#ifdef OGREKIT_BUILD_IPHONE
#import <UIKit/UIKit.h>


@interface gkGestureView : UIView
{
}

@end

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


#endif
// Internal interface
class gkWindowSystemPrivate :
	public OIS::MouseListener,
	public OIS::KeyListener,
	public OIS::JoyStickListener,
#ifdef OGREKIT_BUILD_IPHONE
	public OIS::MultiTouchListener,
#endif
	public WindowEventListener

{
public:
	Private();
	virtual ~Private();

	bool mouseMoved(const OIS::MouseEvent& arg);
	bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
	bool keyPressed(const OIS::KeyEvent& arg);
	bool keyReleased(const OIS::KeyEvent& arg);

#ifdef OGREKIT_BUILD_IPHONE
	bool touchPressed(const OIS::MultiTouchEvent& arg);
	bool touchReleased(const OIS::MultiTouchEvent& arg);
	bool touchMoved(const OIS::MultiTouchEvent& arg);
	bool touchCancelled(const OIS::MultiTouchEvent& arg);

	void transformInputState(OIS::MultiTouchState& state);
#endif

	void windowResized(RenderWindow* rw);
	void windowClosed(RenderWindow* rw);
	bool buttonPressed(const OIS::JoyStickEvent& arg, int i);
	bool buttonReleased(const OIS::JoyStickEvent& arg, int i);
	bool axisMoved(const OIS::JoyStickEvent& arg, int i);

	int getCode(int kc);

	gkWindowSystem*         m_sys;
	OIS::InputManager*      m_input;
	OIS::Keyboard*          m_keyboard;
	OIS::Mouse*             m_mouse;
	utArray<OIS::JoyStick*> m_joysticks;

#ifdef OGREKIT_BUILD_IPHONE
	OIS::MultiTouch*        m_touch;
	gkGestureView*          m_gestureView;
#endif
};


gkWindowSystem::gkWindowSystem() 
:	m_internal(0),
	m_window(0), 
	m_exit(false), 
	m_requestedWidth(0), 
	m_requestedHeight(0),
	m_framingType(0),
	m_useExternalWindow(false)
{
	m_internal = new gkWindowSystemPrivate();
	m_internal->m_sys = this;

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


	// OIS
	try
	{
		size_t handle;
		m_window->getCustomAttribute("WINDOW", &handle);


		OIS::ParamList params;

		if (!prefs.grabInput)
		{

#ifndef OGREKIT_BUILD_IPHONE
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			params.insert(std::make_pair(std::string("w32_mouse"),   std::string("DISCL_FOREGROUND")));
			params.insert(std::make_pair(std::string("w32_mouse"),   std::string("DISCL_NONEXCLUSIVE")));
			params.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
			params.insert(std::make_pair(std::string("w32_keyboard"),  std::string("DISCL_NONEXCLUSIVE")));

			if (m_useExternalWindow)
				params.insert(std::make_pair(std::string("w32_pass_event"), std::string(""))); //pass event to old window proc

#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE
			params.insert(std::make_pair(std::string("MacAutoRepeatOn"), std::string("true")));
#else

			params.insert(std::make_pair(std::string("x11_mouse_grab"),  std::string("false")));
			params.insert(std::make_pair(std::string("x11_mouse_hide"),  std::string("false")));
			params.insert(std::make_pair(std::string("x11_keyboard_grab"),  std::string("false")));
			params.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#endif
#endif
		}

		params.insert(std::make_pair("WINDOW", StringConverter::toString(handle)));
		m_internal->m_input = OIS::InputManager::createInputSystem(params);
		m_internal->m_input->enableAddOnFactory(OIS::InputManager::AddOn_All);

#ifndef OGREKIT_BUILD_IPHONE

		m_internal->m_keyboard = (OIS::Keyboard*) m_internal->m_input->createInputObject(OIS::OISKeyboard, true);
		m_internal->m_keyboard->setEventCallback(m_internal);

		m_internal->m_mouse = (OIS::Mouse*)m_internal->m_input->createInputObject(OIS::OISMouse, true);
		m_internal->m_mouse->setEventCallback(m_internal);

#else

		m_internal->m_gestureView = [[gkGestureView alloc] init];

		[[[UIApplication sharedApplication] keyWindow] addSubview: m_internal->m_gestureView];

		[m_internal->m_gestureView becomeFirstResponder];

		m_internal->m_touch = (OIS::MultiTouch*) m_internal->m_input->createInputObject(OIS::OISMultiTouch, true);
		GK_ASSERT(m_internal->m_touch);
		m_internal->m_touch->setEventCallback(m_internal);
#endif

		for (int i = 0; i < m_internal->m_input->getNumberOfDevices(OIS::OISJoyStick); i++)
		{
			OIS::JoyStick* oisjs = (OIS::JoyStick*)m_internal->m_input->createInputObject(OIS::OISJoyStick, true);
			oisjs->setEventCallback(m_internal);


			gkJoystick* gkjs = new gkJoystick(oisjs->getNumberOfComponents(OIS::OIS_Button), oisjs->getNumberOfComponents(OIS::OIS_Axis));

			m_internal->m_joysticks.push_back(oisjs);
			m_joysticks.push_back(gkjs);
		}

#ifndef OGREKIT_BUILD_IPHONE
		const OIS::MouseState& st = m_internal->m_mouse->getMouseState();
		st.width  = winsizex;
		st.height = winsizey;
#endif

	}
	catch (OIS::Exception& e)
	{
		gkPrintf("%s", e.what());
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
#ifdef OGREKIT_BUILD_IPHONE
	[m_internal->m_gestureView becomeFirstResponder];
#endif
	if (!m_useExternalWindow)
		WindowEventUtilities::messagePump();
}

// Handle platform messages
void gkWindowSystem::dispatch(void)
{
#ifndef OGREKIT_BUILD_IPHONE
	GK_ASSERT(m_internal && m_internal->m_mouse && m_internal->m_keyboard);

	m_mouse.moved = false;
	m_mouse.wheelDelta = 0.f;
	m_mouse.relitave.x = 0.f;
	m_mouse.relitave.y = 0.f;

	m_internal->m_mouse->capture();
	m_internal->m_keyboard->capture();

	utArrayIterator<utArray<OIS::JoyStick*> > iter(m_internal->m_joysticks);
	while (iter.hasMoreElements())
	{
		iter.peekNext()->capture();
		iter.getNext();
	}

#else
	GK_ASSERT(m_internal && m_internal->m_touch);

	m_internal->m_touch->capture();    //OIS don't thing, currently. so instead use a previous saved touch event

	if (m_mouse.buttons[gkMouse::Left] != GK_Pressed)
		m_mouse.moved = false;
#endif


}


void gkWindowSystem::clearStates(void)
{
	m_mouse.clear();
	m_keyboard.clear();

	UTsize i;
	for (i = 0; i < m_joysticks.size(); ++i)
		m_joysticks[i]->clear();
}



// Internal interface
gkWindowSystemPrivate::Private() :
	m_input(0), m_keyboard(0), m_mouse(0)
{
#ifdef OGREKIT_BUILD_IPHONE
	m_touch = NULL;
	m_gestureView = NULL;
#endif
}

gkWindowSystemPrivate::~Private()
{
#ifdef OGREKIT_BUILD_IPHONE
	[m_gestureView release];
#endif

	if (m_input)
	{
		// free input

#ifndef OGREKIT_BUILD_IPHONE
		if (m_keyboard)
			m_input->destroyInputObject(m_keyboard);

		if (m_mouse)
			m_input->destroyInputObject(m_mouse);

		UTsize i;
		for (i = 0; i < m_joysticks.size(); ++i)
			m_input->destroyInputObject(m_joysticks[i]);

#else
		if (m_touch)
			m_input->destroyInputObject(m_touch);
#endif


		OIS::InputManager::destroyInputSystem(m_input);

		m_input = 0; m_keyboard = 0; m_mouse = 0;
	}
}




#ifdef OGREKIT_BUILD_IPHONE

//copy from ogre3d samplebrowser
void gkWindowSystemPrivate::transformInputState(OIS::MultiTouchState& state)
{
	GK_ASSERT(m_sys->m_window && m_sys->m_window->getViewport(0));

	Ogre::Viewport* viewport = m_sys->m_window->getViewport(0);

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

bool gkWindowSystemPrivate::touchPressed(const OIS::MultiTouchEvent& arg)
{
	gkMouse& data = m_sys->m_mouse;

	data.buttons[gkMouse::Left] = GK_Pressed;


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

bool gkWindowSystemPrivate::touchReleased(const OIS::MultiTouchEvent& arg)
{
	gkMouse& data = m_sys->m_mouse;

	data.buttons[gkMouse::Left] = GK_Released;

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

bool gkWindowSystemPrivate::touchMoved(const OIS::MultiTouchEvent& arg)
{
	gkMouse& data = m_sys->m_mouse;
	OIS::MultiTouchState state = arg.state;;

	transformInputState(state);

	data.position.x = (Real)state.X.abs;
	data.position.y = (Real)state.Y.abs;
	data.relitave.x = (Real)state.X.rel;
	data.relitave.y = (Real)state.Y.rel;
	data.moved = true;

	data.wheelDelta = 0;

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

bool gkWindowSystemPrivate::touchCancelled(const OIS::MultiTouchEvent& arg)
{
	return true;
}
#endif //OGREKIT_BUILD_IPHONE


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


GK_IMPLEMENT_SINGLETON(gkWindowSystem);
