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
#ifndef _gkLogicController_h_
#define _gkLogicController_h_

#include "gkLogicBrick.h"


class gkLogicController : public gkLogicBrick
{
protected:
	gkSensors       m_sensors;
	gkActuators     m_actuators;
	bool            m_activeState;

	void            cloneImpl(gkLogicLink* link, gkGameObject* dest);

public:

	gkLogicController(gkGameObject* object, gkLogicLink* link, const gkString& name);
	virtual ~gkLogicController() {}

	void _execute(void);

	void sort(void);
	void link(gkLogicSensor* v);
	void link(gkLogicActuator* v);

	gkSensors&      getSensors(void)        {return m_sensors;}
	gkActuators&    getActuators(void)      {return m_actuators;}

};


#endif//_gkLogicController_h_
