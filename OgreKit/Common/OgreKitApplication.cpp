/*
-------------------------------------------------------------------------------
	This file is part of OgreKit.
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
#include "OgreKitApplication.h"
#include "OgreKitPrefs.h"
#include "OgreRoot.h"
#include "OgreConfigFile.h"
#include "OgreResourceGroupManager.h"
#include "OgreException.h"
#include "OgreFrameListener.h"
#include "OgreWindowEventUtilities.h"
#include "OgreRenderWindow.h"
#include "OgreCamera.h"
#include "OgreStringConverter.h"
#include "OgreRenderSystem.h"
#include "OgreViewport.h"
#include "OgreMathUtils.h"

#include "OIS.h"

// ----------------------------------------------------------------------------
class OgreKitApplicationPrivate :
			public Ogre::FrameListener,
			public Ogre::WindowEventListener,
			public OIS::MouseListener,
			public OIS::KeyListener
{
public:

	OgreKitApplicationPrivate(OgreKitApplication *app);
    virtual ~OgreKitApplicationPrivate();

    void setupInput(Ogre::RenderWindow *window);
    
	/// WindowEventListener
	void windowResized(Ogre::RenderWindow* rw);	
    bool windowClosing(Ogre::RenderWindow* rw);

	// MouseListener
	bool mouseMoved(const OIS::MouseEvent &arg);
	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);


	// KeyListener
	bool keyPressed(const OIS::KeyEvent &arg);
	bool keyReleased(const OIS::KeyEvent &arg);

	// FrameListener
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    bool frameEnded(const Ogre::FrameEvent& evt);

	void endFrame(void);

	// key converter
	int getKeyCode(int kc);

	OgreKitApplication*		m_app;
    Ogre::RenderWindow*     m_window;
	OIS::InputManager*		m_input;

	OIS::Keyboard*			m_key;
	OIS::Mouse*				m_mouse;


	struct GameTick
	{
		unsigned long ticks, rate;
		unsigned long skip, loop;
		unsigned long cur, next;
		Ogre::Real blend, fixed, invt;
		Ogre::Timer T;
		bool lock, init;
	}tick;
};


// ----------------------------------------------------------------------------
OgreKitApplication::OgreKitApplication(const Ogre::String& config) :
		m_root(0),
		m_resourcePath(""),
        m_cfg(config),
		m_window(0),
        m_quit(false)
{
	m_private = new OgreKitApplicationPrivate(this);
}


// ----------------------------------------------------------------------------
OgreKitApplication::~OgreKitApplication()
{
	delete m_private;

	if (m_root)
		delete m_root;
}

// ----------------------------------------------------------------------------
void OgreKitApplication::go(void)
{
	if (!setup())
		return;

	m_private->windowResized(m_window);

	assert(m_root);
	for (;;)
	{
		Ogre::WindowEventUtilities::messagePump();

		if (!m_root->renderOneFrame())
			break;
	}
}

// ----------------------------------------------------------------------------
bool OgreKitApplication::setup(void)
{
	OgreKitPrefs pr;
    if (!m_cfg.empty()) pr.load(m_cfg);

    m_resourcePath = pr.resources;

    m_root = new Ogre::Root("", "");
	m_loader.createRenderSystem(m_root, OGRE_RS_GL);
	m_loader.loadPlugins(m_root);

	m_root->setRenderSystem(m_root->getAvailableRenderers().at(0));
	m_root->initialise(false);

	Ogre::NameValuePairList lst;
	lst["FSAA"] = Ogre::StringConverter::toString(pr.aa_level);
	lst["border"] = "fixed";

	m_window = m_root->createRenderWindow("OgreKit Demo", pr.w, pr.h, pr.fullscreen, &lst);
    m_private->setupInput(m_window);

    if (!m_resourcePath.empty())
        setupResources();

    createScene();
	m_root->addFrameListener(m_private);
	return true;
}


// ----------------------------------------------------------------------------
void OgreKitApplication::setupResources(void)
{

	Ogre::ConfigFile fp;
	fp.load(m_resourcePath);

	Ogre::ResourceGroupManager *resourceManager = Ogre::ResourceGroupManager::getSingletonPtr();
	Ogre::ConfigFile::SectionIterator cit = fp.getSectionIterator();

	while (cit.hasMoreElements())
	{
		Ogre::String name = cit.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *ptr = cit.getNext();
		for (Ogre::ConfigFile::SettingsMultiMap::iterator dit = ptr->begin(); dit != ptr->end(); ++dit)
			resourceManager->addResourceLocation(dit->second, dit->first, name);
	}

	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
// ----------------------------------------------------------------------------
OgreKitApplicationPrivate::OgreKitApplicationPrivate(OgreKitApplication *app) :
		m_app(app), m_window(0), m_input(0), m_key(0), m_mouse(0)
{
	tick.init = false;
}

// ----------------------------------------------------------------------------
OgreKitApplicationPrivate::~OgreKitApplicationPrivate()
{
	if (m_input)
	{
		if (m_key)
			m_input->destroyInputObject(m_key);
		if (m_mouse)
			m_input->destroyInputObject(m_mouse);
		OIS::InputManager::destroyInputSystem(m_input);
	}

    Ogre::WindowEventUtilities::removeWindowEventListener(m_window, this);
}



// ----------------------------------------------------------------------------
void OgreKitApplicationPrivate::setupInput(Ogre::RenderWindow *window)
{
    if (!m_window)
        m_window = window;

	size_t handle;
	window->getCustomAttribute("WINDOW", &handle);

	OIS::ParamList params;
	params.insert(std::make_pair("WINDOW", Ogre::StringConverter::toString(handle)));

	/// install OIS
	m_input = OIS::InputManager::createInputSystem(params);
    m_key = (OIS::Keyboard*)m_input->createInputObject(OIS::OISKeyboard, true);
	m_key->setEventCallback(this);

    m_mouse = (OIS::Mouse*)m_input->createInputObject(OIS::OISMouse, true);
	m_mouse->setEventCallback(this);

	Ogre::WindowEventUtilities::addWindowEventListener(window, this);
}


// ----------------------------------------------------------------------------
void OgreKitApplicationPrivate::windowResized(Ogre::RenderWindow* rw)
{
	unsigned short sz = rw->getNumViewports();

    for (unsigned short i = 0; i<sz; ++i)
	{
		Ogre::Viewport *vp = rw->getViewport(i);
		Ogre::Camera *cam = vp->getCamera();
        cam->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

		const OIS::MouseState &state = m_mouse->getMouseState();

		state.width = OgreMax(state.width, vp->getActualWidth());
		state.height = OgreMax(state.height, vp->getActualHeight());

	}

}

// ----------------------------------------------------------------------------
bool OgreKitApplicationPrivate::windowClosing(Ogre::RenderWindow* rw)
{
    m_app->m_quit = true;
    return true;
}

// ----------------------------------------------------------------------------
#define OKIT_GET_TICK(t)		((unsigned long)(t).getMilliseconds())
#define OKIT_TICKS_PER_SECOND	60
#define OKIT_TIME_SCALE			Ogre::Real(0.001)


// ----------------------------------------------------------------------------
bool OgreKitApplicationPrivate::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	tick.loop = 0;
	tick.lock = false;

	if (!tick.init)
	{
		// initialize timer states
		tick.init = true;

		tick.rate = OKIT_TICKS_PER_SECOND;
		tick.ticks = 1000 / tick.rate;
		tick.skip  = OgreMax(tick.rate / 5, 1);
		tick.invt  = 1.f / tick.ticks;
		tick.fixed = 1.f / OKIT_TICKS_PER_SECOND;
		tick.T.reset();

		tick.cur = OKIT_GET_TICK(tick.T);
		tick.next = tick.cur;
	}

	while ((tick.cur = OKIT_GET_TICK(tick.T)) > tick.next && tick.loop < tick.skip)
	{
		if (!tick.lock)
		{
			if (m_key) m_key->capture();
			if (m_mouse) m_mouse->capture();

			m_app->update(tick.fixed);
			endFrame();

			if (m_app->m_quit)
				return false;

			if (((OKIT_GET_TICK(tick.T) - tick.cur) * OKIT_TIME_SCALE) > tick.fixed)
				tick.lock = true;
		}

		tick.next += tick.ticks;
		++tick.loop;
	}

	return !m_app->m_quit;
}

// ----------------------------------------------------------------------------
bool OgreKitApplicationPrivate::frameEnded(const Ogre::FrameEvent& evt)
{
	endFrame();
    return !m_app->m_quit;
}

// ----------------------------------------------------------------------------
void OgreKitApplicationPrivate::endFrame(void)
{
	OgreKitMouse &data = m_app->m_mouse;
	data.wheelDelta = 0;

	data.moved = false;
	m_app->endFrame();
}

// ----------------------------------------------------------------------------
bool OgreKitApplicationPrivate::mouseMoved(const OIS::MouseEvent &arg)
{
	OgreKitMouse &data = m_app->m_mouse;

	data.position.x= (Ogre::Real)arg.state.X.abs;
	data.position.y= (Ogre::Real)arg.state.Y.abs;
	data.relitave.x= (Ogre::Real)arg.state.X.rel;
	data.relitave.y= (Ogre::Real)arg.state.Y.rel;
	data.moved= true;

	if (arg.state.Z.rel != 0)
		data.wheelDelta= arg.state.Z.rel > 0 ? 1.f : -1.f;

	return false;
}

// ----------------------------------------------------------------------------
bool OgreKitApplicationPrivate::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	OgreKitMouse &data = m_app->m_mouse;

	switch (id)
	{
	case OIS::MB_Left:
		data.buttons[OgreKitMouse::Left] = OgreKitMouse::Pressed;
		break;
	case OIS::MB_Middle:
		data.buttons[OgreKitMouse::Middle] = OgreKitMouse::Pressed;
		break;
	case OIS::MB_Right:
		data.buttons[OgreKitMouse::Right] = OgreKitMouse::Pressed;
		break;
	}
	return false;
}

// ----------------------------------------------------------------------------
bool OgreKitApplicationPrivate::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	OgreKitMouse &data = m_app->m_mouse;

	switch (id)
	{
	case OIS::MB_Left:
		data.buttons[OgreKitMouse::Left] = OgreKitMouse::Released;
		break;
	case OIS::MB_Middle:
		data.buttons[OgreKitMouse::Middle] = OgreKitMouse::Released;
		break;
	case OIS::MB_Right:
		data.buttons[OgreKitMouse::Right] = OgreKitMouse::Released;
		break;
	}
	return false;
}

// ----------------------------------------------------------------------------
bool OgreKitApplicationPrivate::keyPressed(const OIS::KeyEvent &arg)
{
	OgreKitKeyboard &key = m_app->m_keyboard;


	int kc= getKeyCode(arg.key);
	key.keys[kc] = OgreKitKeyboard::Pressed;
	key.key_count += 1;
	return false;
}

// ----------------------------------------------------------------------------
bool OgreKitApplicationPrivate::keyReleased(const OIS::KeyEvent &arg)
{
	OgreKitKeyboard &key = m_app->m_keyboard;

	int kc= getKeyCode(arg.key);
	key.keys[kc] = OgreKitKeyboard::Released;
	key.key_count -= 1;
	return false;
}


// ----------------------------------------------------------------------------
int OgreKitApplicationPrivate::getKeyCode(int kc)
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
