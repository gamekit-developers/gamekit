/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Charlie C.

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
#include "gkWindowSystem.h"
#include "gkWindow.h"
#include "gkViewport.h"

#include "OgreRenderWindow.h"
#include "OgreRoot.h"
#include "OgreWindowEventUtilities.h"
#include "OIS.h"

#ifdef OGREKIT_USE_RTSHADER_SYSTEM
#include "OgreRTShaderSystem.h"

#ifndef RTSHADER_SYSTEM_BUILD_EXT_SHADERS
#error "rtss ext undefined. try re-run cmake."
#endif

#endif


//using namespace Ogre;

gkWindow::gkWindow() 
	:	m_sys(0),
		m_input(0), 
		m_ikeyboard(0), 
		m_imouse(0),
		m_rwindow(0),
		m_requestedWidth(0), 
		m_requestedHeight(0),
		m_framingType(0),
		m_useExternalWindow(false),
		m_scene(0)
#ifdef OGREKIT_COMPILE_LIBROCKET
		, m_gui(0)
#endif
{
}

gkWindow::~gkWindow()
{
#ifdef OGREKIT_COMPILE_LIBROCKET
	if (m_gui)
		delete m_gui;
#endif
	
	if (m_rwindow)
	{				
		Ogre::WindowEventUtilities::removeWindowEventListener(m_rwindow, this);
	}

	UTsize i;
	for (i = 0; i < m_joysticks.size(); i++)
		delete m_joysticks[i];

	m_joysticks.clear();

	for (i = 0; i < m_viewports.size(); i++)
		delete m_viewports[i];

	m_viewports.clear();

	if (m_input)
	{
		// free input

		if (m_ikeyboard)
			m_input->destroyInputObject(m_ikeyboard);

		if (m_imouse)
			m_input->destroyInputObject(m_imouse);
		
		for (i = 0; i < m_ijoysticks.size(); i++)
			m_input->destroyInputObject(m_ijoysticks[i]);


		OIS::InputManager::destroyInputSystem(m_input);

		m_input = 0; m_ikeyboard = 0; m_imouse = 0;
	}
}

size_t gkWindow::getWindowHandle()
{
	size_t handle = 0;

	if (m_rwindow)
		m_rwindow->getCustomAttribute("WINDOW", &handle);
	
	gkPrintf("WINDOW handle: 0x%x", handle);

	return handle;
}

bool gkWindow::setupInput(const gkUserDefs& prefs)
{
	// OIS
	try
	{		
		size_t handle = getWindowHandle();
		if (handle == 0) return false;


		OIS::ParamList params;

		if (!prefs.grabInput)
		{

#if GK_PLATFORM == GK_PLATFORM_WIN32
			size_t parent_handle = (size_t)GetParent((HWND)handle);
			while(parent_handle){
				handle = parent_handle;
				parent_handle = (size_t)GetParent((HWND)handle);
			}
			params.insert(std::make_pair(std::string("w32_mouse"),			std::string("DISCL_FOREGROUND")));
			params.insert(std::make_pair(std::string("w32_mouse"),			std::string("DISCL_NONEXCLUSIVE")));
			params.insert(std::make_pair(std::string("w32_keyboard"),		std::string("DISCL_FOREGROUND")));
			params.insert(std::make_pair(std::string("w32_keyboard"),		std::string("DISCL_NONEXCLUSIVE")));

			if (m_useExternalWindow)
				params.insert(std::make_pair(std::string("w32_pass_event"), std::string(""))); //pass event to old window proc

#elif GK_PLATFORM == GK_PLATFORM_APPLE
			params.insert(std::make_pair(std::string("MacAutoRepeatOn"),	std::string("true")));
#else
			params.insert(std::make_pair(std::string("x11_mouse_grab"),		std::string("false")));
			params.insert(std::make_pair(std::string("x11_mouse_hide"),		std::string("false")));
			params.insert(std::make_pair(std::string("x11_keyboard_grab"),  std::string("false")));
			params.insert(std::make_pair(std::string("XAutoRepeatOn"),		std::string("true")));
#endif
		}

		std::ostringstream wnd;
		wnd << handle;
		params.insert(std::make_pair("WINDOW", wnd.str()));
		m_input = OIS::InputManager::createInputSystem(params);
		m_input->enableAddOnFactory(OIS::InputManager::AddOn_All);


		m_ikeyboard = (OIS::Keyboard*) m_input->createInputObject(OIS::OISKeyboard, true);
		m_ikeyboard->setEventCallback(this);

		m_imouse = (OIS::Mouse*)m_input->createInputObject(OIS::OISMouse, true);
		m_imouse->setEventCallback(this);

		int i;
		for (i = 0; i < m_input->getNumberOfDevices(OIS::OISJoyStick); i++)
		{
			OIS::JoyStick* oisjs = (OIS::JoyStick*)m_input->createInputObject(OIS::OISJoyStick, true);
			oisjs->setEventCallback(this);


			gkJoystick* gkjs = new gkJoystick(oisjs->getNumberOfComponents(OIS::OIS_Button), oisjs->getNumberOfComponents(OIS::OIS_Axis));

			m_ijoysticks.push_back(oisjs);
			m_joysticks.push_back(gkjs);
		}

		const OIS::MouseState& st = m_imouse->getMouseState();
		st.width  = getWidth(); 
		st.height = getHeight();

	}
	catch (OIS::Exception& e)
	{
		gkPrintf("%s", e.what());
		return false;
	}

	return true;
}

bool gkWindow::createWindow(gkWindowSystem* sys, const gkUserDefs& prefs)
{
	try
	{
		if (!sys || m_rwindow) 
			return false;

		m_sys = sys;

		int winsizex = 0, winsizey = 0;

		m_requestedWidth = (int)(prefs.winsize.x + 0.5f);
		m_requestedHeight = (int)(prefs.winsize.y + 0.5f);
		m_framingType = prefs.framingType;

		Ogre::NameValuePairList params;

		if (prefs.fsaa)
		{
			params["FSAA"] = Ogre::StringConverter::toString(prefs.fsaaSamples);
		}

		if (prefs.vsync)
		{
			params["vsync"] = Ogre::StringConverter::toString(prefs.vsync);

			if (prefs.vsyncRate > 0)
			{
				params["vsyncInterval"] = Ogre::StringConverter::toString(prefs.vsyncRate);
			}
		}

		if (!prefs.extWinhandle.empty())
		{
			params["externalWindowHandle"] = prefs.extWinhandle;
			m_useExternalWindow = true;
		}

		if (prefs.fullscreen)
		{
			Ogre::RenderSystem* rsys = Ogre::Root::getSingleton().getRenderSystem();
			Ogre::ConfigOptionMap options = rsys->getConfigOptions();
			Ogre::ConfigOption modeOption = options["Video Mode"];
			bool found = false;

			gkPrintf("Available video modes:");
			size_t i;
			for (i = 0; i < modeOption.possibleValues.size(); i++)
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
#ifdef __ANDROID__
		params["externalWindowHandle"] = prefs.extWinhandle;
		params["androidConfig"] = prefs.androidConfig;
#endif
		m_rwindow = Ogre::Root::getSingleton().createRenderWindow(prefs.wintitle,
				   winsizex, winsizey, prefs.fullscreen, &params);
		m_rwindow->setActive(true);

		// copy window size (used later for hit testing)
		m_mouse.winsize.x = winsizex;
		m_mouse.winsize.y = winsizey;	

		if (!setupInput(prefs))
		{
			gkPrintf("Unable setup gkWindow input objects.");
			return false;
		}

		Ogre::WindowEventUtilities::addWindowEventListener(m_rwindow, this);
	}
	catch (OIS::Exception& e)
	{
		gkPrintf("gkWindow: %s", e.what());
		return false;
	}
	
	return true;
}

void gkWindow::addListener(gkWindowSystem::Listener* l)
{
	m_listeners.push_back(l);
}

void gkWindow::removeListener(gkWindowSystem::Listener* l)
{
	m_listeners.erase(l);
}

bool _sortViewports(gkViewport* const& a, gkViewport* const& b){
	return a->getZOrder() > b->getZOrder();
}

gkViewport* gkWindow::addViewport(gkCamera* cam, int zorder)
{	
	if (m_rwindow)
	{
		Ogre::Viewport* vp =  m_rwindow->addViewport(cam->getCamera(), zorder); GK_ASSERT(vp);

		gkViewport* viewport = new gkViewport(this, vp);
		viewport->setDimension(m_framingType);
		
		m_viewports.push_back(viewport);
		
		// Clear only depth in all added viewports
		vp->setClearEveryFrame(true, Ogre::FBT_DEPTH);
		
		// Except the first one
		m_viewports[0]->getViewport()->setClearEveryFrame(true, Ogre::FBT_DEPTH);
		m_viewports.sort(_sortViewports);
		m_viewports[0]->getViewport()->setClearEveryFrame(true);

#ifdef OGREKIT_USE_RTSHADER_SYSTEM

		viewport->getViewport()->setMaterialScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);

		if (gkEngine::getSingleton().getUserDefs().rtss)
		{
			Ogre::RTShader::ShaderGenerator* sg = Ogre::RTShader::ShaderGenerator::getSingletonPtr();

			//enable perpixel render
			Ogre::RTShader::RenderState* schemRenderState = sg->getRenderState(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
			Ogre::RTShader::SubRenderState* perPixelLightModel = sg->createSubRenderState(Ogre::RTShader::PerPixelLighting::Type);
			schemRenderState->addTemplateSubRenderState(perPixelLightModel);
			sg->invalidateScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
		}
#endif

		return viewport;
	}
	return 0;
}

void gkWindow::removeViewport(gkViewport* viewport)
{
	if (viewport)
	{		
		m_viewports.erase(m_viewports.find(viewport));
		delete viewport;
		if (m_viewports.size()>0)
			m_viewports[0]->getViewport()->setClearEveryFrame(true);
	}
}



#ifdef OGREKIT_COMPILE_LIBROCKET

gkGUI* gkWindow::getGUI()
{
	if (!m_gui)
		m_gui = new gkGUI(this);
	return m_gui;
}

void gkWindow::destroyGUI()
{
	if (m_gui)
		delete m_gui;
	m_gui = 0;
}

#endif


void gkWindow::dispatch(void)
{
	m_mouse.moved = false;
	m_mouse.wheelDelta = 0.f;
	m_mouse.relative.x = 0.f;
	m_mouse.relative.y = 0.f;

	m_imouse->capture();
	m_ikeyboard->capture();

	utArrayIterator<utArray<OIS::JoyStick*> > iter(m_ijoysticks);
	while (iter.hasMoreElements())
	{
		iter.peekNext()->capture();
		iter.getNext();
	}
}

void gkWindow::process(void)
{
	if (!m_useExternalWindow)
		Ogre::WindowEventUtilities::messagePump();
}

void gkWindow::clearStates(void)
{	
	m_mouse.clear();
	m_keyboard.clear();
	
	UTsize i;
	for (i = 0; i < m_joysticks.size(); i++)
		m_joysticks[i]->clear();
}

bool gkWindow::mouseMoved(const OIS::MouseEvent& arg)
{
	gkMouse& data = m_mouse;

	data.position.x = (gkScalar)arg.state.X.abs;
	data.position.y = (gkScalar)arg.state.Y.abs;
	data.relative.x = (gkScalar)arg.state.X.rel;
	data.relative.y = (gkScalar)arg.state.Y.rel;
	data.moved = true;

	if (arg.state.Z.rel != 0)
		data.wheelDelta = arg.state.Z.rel > 0 ? 1.f : -1.f;
	else 
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

bool gkWindow::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	gkMouse& data = m_mouse;

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

bool gkWindow::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	gkMouse& data = m_mouse;

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

	if (!m_listeners.empty())
	{
		gkWindowSystem::Listener* node = m_listeners.begin();
		while (node)
		{
			node->mouseReleased(data);
			node = node->getNext();
		}
	}

	return true;
}

bool gkWindow::keyPressed(const OIS::KeyEvent& arg)
{
	gkKeyboard& key = m_keyboard;

	int kc = getKeyCode(arg.key);
	key.keys[kc] = GK_Pressed;
	key.key_count += 1;
	key.text = arg.text;
	key.key_mod = getKeyModifier();

	if (!m_listeners.empty())
	{
		gkWindowSystem::Listener* node = m_listeners.begin();
		while (node)
		{
			node->keyPressed(key, (gkScanCode)kc);
			node = node->getNext();
		}
	}

	return true;
}

bool gkWindow::keyReleased(const OIS::KeyEvent& arg)
{
	gkKeyboard& key = m_keyboard;

	int kc = getKeyCode(arg.key);
	key.keys[kc] = GK_Released;
	key.key_count -= 1;
	key.text = arg.text;
	key.key_mod = getKeyModifier();

	if (!m_listeners.empty())
	{
		gkWindowSystem::Listener* node = m_listeners.begin();
		while (node)
		{
			node->keyReleased(key, (gkScanCode)kc);
			node = node->getNext();
		}
	}


	return true;
}

bool gkWindow::buttonPressed(const OIS::JoyStickEvent& arg, int button)
{
	// Hum we assume coresponding gk and OIs joystick have the same place in their array
	gkJoystick& js = *m_joysticks[m_ijoysticks.find((OIS::JoyStick*)arg.device)];

	js.buttons[button] = GK_Pressed;
	js.buttonCount += 1;

	if (!m_listeners.empty())
	{
		gkWindowSystem::Listener* node = m_listeners.begin();
		while (node)
		{
			node->joystickPressed(js, button);
			node = node->getNext();
		}
	}

	return true;
}

bool gkWindow::buttonReleased(const OIS::JoyStickEvent& arg, int button)
{
	// Hum we assume coresponding gk and OIs joystick have the same place in their array
	gkJoystick& js = *m_joysticks[m_ijoysticks.find((OIS::JoyStick*)arg.device)];

	js.buttons[button] = GK_Released;
	js.buttonCount -= 1;

	if (!m_listeners.empty())
	{
		gkWindowSystem::Listener* node = m_listeners.begin();
		while (node)
		{
			node->joystickReleased(js, button);
			node = node->getNext();
		}
	}

	return true;
}

bool gkWindow::axisMoved(const OIS::JoyStickEvent& arg, int axis)
{
	// Hum we assume coresponding gk and OIS joystick have the same place in their array
	gkJoystick& js = *m_joysticks[m_ijoysticks.find((OIS::JoyStick*)arg.device)];

	js.axes[axis] = arg.state.mAxes[axis].abs;
	if (!m_listeners.empty())
	{
		gkWindowSystem::Listener* node = m_listeners.begin();
		while (node)
		{
			node->joystickMoved(js, axis);
			node = node->getNext();
		}
	}
	return true;
}

void gkWindow::windowMovedOrResized()
{
	if (!m_rwindow)
		return;

	m_rwindow->windowMovedOrResized();
	windowMoved(m_rwindow);
	windowResized(m_rwindow);
}

bool gkWindow::isActive()
{
	return m_rwindow && m_rwindow->isActive();
}

Ogre::RenderWindow* gkWindow::getOgreRenderWindow()
{
	return m_rwindow;
}

void gkWindow::windowResized(Ogre::RenderWindow* rw)
{
	UTsize i;
	gkScalar scale, ratio;

	for (i = 0; i < m_viewports.size(); ++i)
	{
		m_viewports[i]->setDimension(m_viewports[i]->getFraming());
		Ogre::Viewport* vp = m_viewports[i]->getViewport();

		// We assume all viewports are "main" vieport
		Ogre::Camera* cam = vp->getCamera();
		
		ratio = gkScalar(vp->getActualWidth()) / gkScalar(vp->getActualHeight());
		
		if (cam->getProjectionType() == Ogre::PT_ORTHOGRAPHIC)
		{
			scale = cam->getOrthoWindowWidth();
			cam->setAspectRatio(ratio);
			cam->setOrthoWindowWidth(scale);
		}
		else
			cam->setAspectRatio(ratio);

		const OIS::MouseState& state = m_imouse->getMouseState();

		state.width = gkMax<int>(state.width, vp->getActualWidth());
		state.height = gkMax<int>(state.height, vp->getActualHeight());

		m_mouse.winsize.x = (gkScalar)state.width;
		m_mouse.winsize.y = (gkScalar)state.height;
	}

	// Ogre keep Y field of view constant, we want to keep X fov constant
	gkScene* scene = getRenderScene();
	if (scene)
	{
		gkCamera* cam = scene->getMainCamera();
		if (cam)
			cam->setFov(gkDegree(cam->getFov()));
	}
	
#ifdef OGREKIT_COMPILE_LIBROCKET
	if (m_gui)
		m_gui->getContext()->SetDimensions(Rocket::Core::Vector2i(getWidth(), getHeight()));
#endif
}


void gkWindow::windowClosed(Ogre::RenderWindow* rw)
{
	GK_ASSERT(m_sys);
	if (m_sys->getMainWindow() == this)
		m_sys->exit(true);
}

void gkWindow::setRenderScene(gkScene* scene)
{
	m_scene = scene;
}

gkScene* gkWindow::getRenderScene(void)			
{ 
	return m_scene ? m_scene : gkEngine::getSingleton().getActiveScene(); 
}

gkString gkWindow::writeContentsToTimestampedFile(const gkString& filenamePrefix, const gkString& filenameSuffix)
{
	gkString suffix;
	if (filenameSuffix.find(".") != gkString::npos)
		suffix = filenameSuffix;
	else
		suffix = gkString(".") + filenameSuffix;
		
	try
	{
		return m_rwindow->writeContentsToTimestampedFile(filenamePrefix, suffix);
	}
	catch (Ogre::Exception& e)
	{
		gkLogMessage("Screenshot: " << e.what());
		return "";
	}
}

int gkWindow::getKeyModifier()
{
	if (!m_ikeyboard) return 0;

	int modifier = 0;

	if (m_ikeyboard->isModifierDown(OIS::Keyboard::Ctrl))
		modifier |= gkKeyboard::KM_CTRL;
	if (m_ikeyboard->isModifierDown(OIS::Keyboard::Shift))
		modifier |= gkKeyboard::KM_SHIFT;
	if (m_ikeyboard->isModifierDown(OIS::Keyboard::Alt))
		modifier |= gkKeyboard::KM_ALT;

	return modifier;
}


int gkWindow::getKeyCode(int kc)
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
