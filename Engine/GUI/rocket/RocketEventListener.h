/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 harkon.kr

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

#ifndef ROCKETEVENTLISTENER_H
#define ROCKETEVENTLISTENER_H

#include <Rocket/Core/Context.h>
#include "OgreKit.h"

class RocketRenderListener;

class RocketEventListener : public gkWindowSystem::Listener
{
public:
	RocketEventListener(gkWindow* window, Rocket::Core::Context* context);
	virtual ~RocketEventListener();

	// MouseListener interface.
	virtual void mouseMoved(const gkMouse& mouse);
	virtual void mousePressed(const gkMouse& mouse);
	virtual void mouseReleased(const gkMouse& mouse);

	// KeyListener interface.
	virtual void keyPressed(const gkKeyboard &key, const gkScanCode &sc);
	virtual void keyReleased(const gkKeyboard &key, const gkScanCode &sc);


private:
	int getKeyCode(int kc);
	int getKeyModifierState(int mod);

	Rocket::Core::Context* m_context;
	gkWindow* m_window;
};

#endif //ROCKETEVENTLISTENER_H
