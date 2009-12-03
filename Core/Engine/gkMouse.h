/*
-------------------------------------------------------------------------------
	This file is part of the Ogre GameKit port.
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
#ifndef _gkMouse_h_
#define _gkMouse_h_

#include "OgreRenderWindow.h"
#include "gkCommon.h"
#include "gkMathUtils.h"



// ----------------------------------------------------------------------------
class gkMouseDevice
{
public:

	enum State
	{
		STATE_NULL= 0,
		STATE_PRESSED,
		STATE_RELEASED,
	};

	enum Buttons
	{
		BUTTON_LEFT= 0,
		BUTTON_RIGHT,
		BUTTON_MIDDLE,
	};

	typedef int ButtonState[3];


	Ogre::Vector2 getPosition() const;
	Ogre::Vector2 getRelitave() const;
	Ogre::Vector2 getAbsolute() const;
	bool isButtonDown(int button) const;

	bool mouseMoved() const;

	gkRectangle getWindowRect() const;
	gkRectangle getAreaRect() const;


public:
	gkMouseDevice();

	bool isButtonDown(int btn);
	Ogre::Vector2		 position;
	Ogre::Vector2		 relitave;
	Ogre::Vector2		 absolute;
	Ogre::Vector2		 last_update;
	ButtonState	 buttons;
	Ogre::RenderWindow*   window;
	gkRectangle		   area;
	bool			moved;
	Ogre::Real			wheelDelta; /*-1 -> 1*/
};

// ----------------------------------------------------------------------------
GK_INLINE gkMouseDevice::gkMouseDevice():
		position(0, 0),
		relitave(0, 0),
		absolute(0, 0),
		last_update(0, 0),
		window(0),
		moved(false),
		wheelDelta(0.0)
{
	buttons[0]= STATE_NULL;
	buttons[1]= STATE_NULL;
	buttons[2]= STATE_NULL;
}

// ----------------------------------------------------------------------------
GK_INLINE bool gkMouseDevice::isButtonDown(int btn)
{
	if (btn >= 0 && btn < 3)
		return (buttons[btn] & STATE_PRESSED) != 0;
	return false;
}

// ----------------------------------------------------------------------------
GK_INLINE bool gkMouseDevice::mouseMoved() const
{
	return moved;
}

// ----------------------------------------------------------------------------
GK_INLINE Ogre::Vector2 gkMouseDevice::getPosition() const
{
	return position;
}

// ----------------------------------------------------------------------------
GK_INLINE Ogre::Vector2 gkMouseDevice::getRelitave() const
{
	return relitave;
}

// ----------------------------------------------------------------------------
GK_INLINE Ogre::Vector2 gkMouseDevice::getAbsolute() const
{
	return absolute;
}

// ----------------------------------------------------------------------------
GK_INLINE gkRectangle gkMouseDevice::getWindowRect() const
{
	if (window)
		return gkRectangle(0, 0, window->getWidth(), window->getHeight());
	return gkRectangle(0, 0, 1, 1);
}

// ----------------------------------------------------------------------------
GK_INLINE gkRectangle gkMouseDevice::getAreaRect() const
{
	return area;
}





#endif//_gkMouse_h_
