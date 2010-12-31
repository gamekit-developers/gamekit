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

class gkWindowIOS;

class gkWindowSystem : public utSingleton<gkWindowSystem>
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
	utArray<gkWindow*>		m_windows;
	bool					m_exit;

public:
	gkWindowSystem();
	virtual ~gkWindowSystem();

	gkWindow* createWindow(const gkUserDefs& prefs);	
	void destroyWindow(gkWindow* window);

	UTsize getWindowCount(void)			{ return m_windows.size(); }
	gkWindow* getWindow(UTsize i)		{ GK_ASSERT(i < m_windows.size()); return m_windows[i]; }

	gkWindow* getMainWindow(void);
	Ogre::RenderWindow* getMainRenderWindow(void);
	

	void addListener(Listener* l);
	void removeListener(Listener* l);

	void process(void);
	void dispatch(void);

	void clearStates(void);

	GK_INLINE void exit(bool v)         { m_exit = v; }
	GK_INLINE bool exitRequest(void)	{ return m_exit; }

	gkKeyboard*		getKeyboard(void);
	gkMouse*		getMouse(void);   
	unsigned int	getNumJoysticks(void);
	gkJoystick*		getJoystick(int index);

	UT_DECLARE_SINGLETON(gkWindowSystem);
};

#endif//_gkWindowSystem_h_
