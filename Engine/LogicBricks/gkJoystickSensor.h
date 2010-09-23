/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

    Contributor(s): nonw yet.
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

#ifndef GKJOYSTICKSENSOR_H
#define GKJOYSTICKSENSOR_H

#include "gkLogicSensor.h"
#include "gkLogicDispatcher.h"
#include "gkWindowSystem.h"


class gkJoyDispatch : public gkAbstractDispatcher
{
public:
	gkJoyDispatch();
	virtual ~gkJoyDispatch();

	void dispatch(void);
};



class gkJoystickSensor : public gkLogicSensor
{
public:
	enum JoyType
	{
		JT_NONE = 0,
		JT_AXIS,
		JT_AXIS_PAIR,
		JT_BUTTON,
		JT_HAT,
	};

	enum AxisDir
	{
		AD_LEFT,
		AD_RIGHT,
		AD_TOP,
		AD_BOTTOM,
	};

protected:
	unsigned int m_joystickIndex, m_elementIndex, m_axisThreshold;
	bool         m_allEvents;
	int          m_eventType, m_axisDirection;

public:
	gkJoystickSensor(gkGameObject* object, gkLogicLink* link, const gkString& name);
	virtual ~gkJoystickSensor() {}

	gkLogicBrick* clone(gkLogicLink* link, gkGameObject* dest);

	bool query(void);

	GK_INLINE void setJoystickIndex(unsigned int v)    {m_joystickIndex  = v;}
	GK_INLINE void setElementIndex(unsigned int v)     {m_elementIndex = v;}
	GK_INLINE void setAxisThreshold(unsigned int v)    {m_axisThreshold = v;}
	GK_INLINE void setAllElementEvents(bool v)         {m_allEvents = v;}
	GK_INLINE void setEventType(int v)                 {m_eventType = v;}
	GK_INLINE void setAxisDirection(int v)             {m_axisDirection = v;}
};

#endif // GKJOYSTICKSENSOR_H
