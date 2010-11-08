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

#ifndef _gkWindowSystemPrivate_h_
#define _gkWindowSystemPrivate_h_

// Internal interface
class gkWindowSystemPrivate :
	public OIS::MouseListener,
	public OIS::KeyListener,
	public OIS::JoyStickListener,
	public Ogre::WindowEventListener
{
public:
	gkWindowSystemPrivate();
	virtual ~gkWindowSystemPrivate();

	virtual bool setup(gkWindowSystem* sys, const gkUserDefs& prefs);
	virtual void dispatch(void);
	virtual void process(void);

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

	int getCode(int kc);

	gkWindowSystem*         m_sys;
	OIS::InputManager*      m_input;
	OIS::Keyboard*          m_keyboard;
	OIS::Mouse*             m_mouse;
	utArray<OIS::JoyStick*> m_joysticks;
};

#endif //_gkWindowSystemPrivate_h_