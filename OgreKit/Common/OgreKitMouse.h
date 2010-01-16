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
#ifndef _OgreMouse_h_
#define _OgreMouse_h_

#include "OgreRenderWindow.h"
#include "OgreUtilsCommon.h"
#include "OgreMathUtils.h"


// ----------------------------------------------------------------------------
class OgreKitMouse
{
public:

	enum State
	{
		NullState= 0,
		Pressed,
		Released,
	};

	enum Buttons
	{
		Left= 0,
		Right,
		Middle,
	};

	typedef int ButtonState[3];


	Ogre::Vector2 getPosition(void) const {return position;}
	Ogre::Vector2 getRelitave(void) const {return relitave;}

	OGRE_INLINE bool isButtonDown(int button) const
	{
		if (button >= 0 && button < 3)
			return (buttons[button] == Pressed);
		return false;
	}

	OGRE_INLINE bool mouseMoved(void) const { return moved; }

public:

	OgreKitMouse() :
		position(0, 0),
		relitave(0, 0),
		moved(false),
		wheelDelta(0.0)
	{
		buttons[0]= NullState;
		buttons[1]= NullState;
		buttons[2]= NullState;
	}

	Ogre::Vector2			position;	// absolite position 
	Ogre::Vector2			relitave;	// relitave position
	ButtonState				buttons;	// button pressed state
	bool					moved;		// mouse moved 
	Ogre::Real				wheelDelta;	// current wheel 
};


#endif//_OgreMouse_h_
