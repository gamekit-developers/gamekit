/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

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
#ifndef _gkWindowSystem_h_
#define _gkWindowSystem_h_

#include "utSingleton.h"
#include "gkCommon.h"
#include "gkInput.h"

class gkWindowSystemPrivate;
class gkWindowSystemPrivateIOS;

class gkWindowSystem : public utSingleton<gkWindowSystem>
{
public:
	enum
	{
		FRAMING_EXTEND,
		FRAMING_CROP,
		FRAMING_LETTERBOX,
	};

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
	friend class gkWindowSystemPrivate;
	friend class gkWindowSystemPrivateIOS;
	
	gkKeyboard				m_keyboard;

	gkMouse					m_mouse;

	utArray<gkJoystick*>	m_joysticks;

	// Internal interface implementation
	gkWindowSystemPrivate*	m_internal;

	Ogre::RenderWindow*		m_window;
	bool					m_exit;

	ListenerList			m_listeners;

	int m_requestedWidth;
	int m_requestedHeight;
	int m_framingType;

	bool m_useExternalWindow;
public:
	gkWindowSystem();
	virtual ~gkWindowSystem();

	Ogre::RenderWindow* createMainWindow(const gkUserDefs& prefs);
	Ogre::RenderWindow* getMainWindow(void);
	Ogre::Viewport* addMainViewport(gkCamera* cam);
	void setMainViewportDimension(Ogre::Viewport* viewport);

	void addListener(Listener* l);
	void removeListener(Listener* l);

	void process(void);
	void dispatch(void);

	void clearStates(void);

	GK_INLINE void exit(bool v)                  {m_exit = v;}
	GK_INLINE bool exitRequest(void)             {return m_exit;}
	GK_INLINE gkKeyboard* getKeyboard(void)      {return &m_keyboard;}
	GK_INLINE gkMouse* getMouse(void)            {return &m_mouse;}
	GK_INLINE unsigned int getNumJoysticks(void) {return m_joysticks.size();}
	GK_INLINE gkJoystick* getJoystick(int index) {return (index >= (int)m_joysticks.size() || index < 0) ? 0 : m_joysticks[index];}

	UT_DECLARE_SINGLETON(gkWindowSystem);
};

#endif//_gkWindowSystem_h_
