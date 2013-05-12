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

#ifndef _gkWindow_h_
#define _gkWindow_h_

#include "OgreWindowEventUtilities.h" //TODO: remove this
#include "OIS.h"
#include "OgreRenderWindow.h"

#include "gkCommon.h"
#include "gkInput.h"
#include "gkWindowSystem.h"

#ifdef OGREKIT_COMPILE_LIBROCKET
#include "GUI/gkGUI.h"
#endif

class gkWindow :
	public OIS::MouseListener,
	public OIS::KeyListener,
	public OIS::JoyStickListener,
	public Ogre::WindowEventListener
{
public:
	class Listener : public utListClass<Listener>::Link
	{
	public:
		virtual ~Listener() {}

		virtual void mouseMoved(const gkMouse& mouse) {}
		virtual void mousePressed(const gkMouse& mouse) {}
		virtual void mouseReleased(const gkMouse& mouse) {}
		virtual void keyPressed(const gkKeyboard& key, const gkScanCode& sc) {}
		virtual void keyReleased(const gkKeyboard& key, const gkScanCode& sc) {}
		virtual void joystickMoved(const gkJoystick& joystick, int axis) {}
		virtual void joystickPressed(const gkJoystick& joystick, int button) {}
		virtual void joystickReleased(const gkJoystick& joystick, int button) {}
	};

	typedef utListClass<Listener> ListenerList;

protected:
	virtual bool setupInput(const gkUserDefs& prefs);

	size_t getWindowHandle();

	gkKeyboard				m_keyboard;
	gkMouse					m_mouse;
	utArray<gkJoystick*>	m_joysticks;

	gkWindowSystem*         m_sys;
	OIS::InputManager*      m_input;
	OIS::Keyboard*          m_ikeyboard;
	OIS::Mouse*             m_imouse;
	utArray<OIS::JoyStick*> m_ijoysticks;

	Ogre::RenderWindow*		m_rwindow;

	utArray<gkViewport*>	m_viewports;
	
	gkScene*				m_scene;

	int m_requestedWidth;
	int m_requestedHeight;
	int m_framingType;
	
	bool m_useExternalWindow;

	gkWindowSystem::ListenerList m_listeners;
	
#ifdef OGREKIT_COMPILE_LIBROCKET
	
private:
	gkGUI* m_gui;

public:
	gkGUI* getGUI();
	void destroyGUI();
	
#endif


public:
	gkWindow();
	virtual ~gkWindow();

	virtual bool createWindow(gkWindowSystem* sys, const gkUserDefs& prefs);
	
	virtual void dispatch(void);
	virtual void process(void);

	virtual void clearStates(void);

	void addListener(gkWindowSystem::Listener* l);
	void removeListener(gkWindowSystem::Listener* l);

	bool mouseMoved(const OIS::MouseEvent& arg);
	bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
	bool keyPressed(const OIS::KeyEvent& arg);
	bool keyReleased(const OIS::KeyEvent& arg);

	void windowResized(Ogre::RenderWindow* rw);
	void windowClosed(Ogre::RenderWindow* rw);
	bool buttonPressed(const OIS::JoyStickEvent& arg, int i);
	bool buttonReleased(const OIS::JoyStickEvent& arg, int i);
	bool axisMoved(const OIS::JoyStickEvent& arg, int i);

	void windowMovedOrResized();
	bool isActive();

	Ogre::RenderWindow* getOgreRenderWindow();

	static int getKeyCode(int kc);
	int getKeyModifier();

	GK_INLINE Ogre::RenderWindow* getRenderWindow() { return m_rwindow; }
	GK_INLINE OIS::InputManager*  getInputManager() { return m_input;   }

	gkViewport* addViewport(gkCamera* cam, int zorder=0);
	void removeViewport(gkViewport* viewport);
	GK_INLINE UTsize getViewportCount(void)			{ return m_viewports.size(); }
	GK_INLINE gkViewport* getViewport(UTsize i)		{ return m_viewports[i]; }

	GK_INLINE int getWidth(void)					{ return (int)m_rwindow->getWidth();  }
	GK_INLINE int getHeight(void)					{ return (int)m_rwindow->getHeight(); }
	GK_INLINE gkVector2 getSize(void)				{ return gkVector2(getWidth(), getHeight()); }

	GK_INLINE int getRequestedWidth(void)			{ return m_requestedWidth;  }
	GK_INLINE int getRequestedHeight(void)			{ return m_requestedHeight; }

	GK_INLINE gkMouse* getMouse(void)				{ return &m_mouse; }
	GK_INLINE gkKeyboard* getKeyboard(void)			{ return &m_keyboard; }
	GK_INLINE unsigned int getNumJoysticks(void)	{ return m_joysticks.size(); }
	GK_INLINE gkJoystick* getJoystick(int index)	{ return (index >= (int)m_joysticks.size() || index < 0) ? 0 : m_joysticks[index]; }

	gkScene* getRenderScene(void);
	void setRenderScene(gkScene* scene);

	gkString writeContentsToTimestampedFile(const gkString& filenamePrefix, const gkString& filenameSuffix);
};

#endif //_gkWindow_h_