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
#ifndef _gkGameController_h_
#define _gkGameController_h_

class gkGamePlayer;
class gkJoystick;
class gkMouse;
class gkKeyboard;

class gkGameController
{
public:

	enum InputCode
	{
		IC_RUN,
		IC_WALK,
		IC_JUMP,
		IC_STOP,
		IC_BUTTON_0,
		IC_BUTTON_1,
		IC_BUTTON_2,
		IC_MAX,
	};


public:

	gkGameController() {}
	virtual ~gkGameController() {}

	virtual bool hasInput(const InputCode& ic) = 0;

	virtual void moveCamera(void) = 0;
	virtual void movePlayer(void) = 0;

	virtual void updateInputState(void) = 0;
};

#endif//_gkGameController_h_
