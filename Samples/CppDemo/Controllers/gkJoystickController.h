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
#ifndef _gkJoystickController_h_
#define _gkJoystickController_h_

#include "gkApplication.h"
#include "gkGameController.h"


class gkJoystickController : public gkGameController
{
public:

	gkJoystickController(gkGamePlayer *player);
	virtual ~gkJoystickController();


	bool hasCameraMovement(void);
	bool hasInput(const InputCode& ic);

	void moveCamera(void);
	void movePlayer(void);
	
	void updateInputState(void);

private:
	bool isButtonDownCache(int btn, int& cache);

	gkGamePlayer *m_player;

	int m_btn1Cache, m_btn2Cache, m_btn3Cache;

	bool m_isBtn1, m_isBtn2, m_isBtn3;


	gkJoystick*  m_joystick;


	gkJoystickAxisState m_camRot, m_movement;

};

#endif//_gkJoystickController_h_