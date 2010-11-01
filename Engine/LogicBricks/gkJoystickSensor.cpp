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

#include "gkJoystickSensor.h"
#include "gkLogicManager.h"
#include "gkScene.h"


gkJoyDispatch::gkJoyDispatch()
{
}



gkJoyDispatch::~gkJoyDispatch()
{
}



void gkJoyDispatch::dispatch(void)
{
	// TODO need to sort.
	doDispatch(m_sensors);
}


gkJoystickSensor::gkJoystickSensor(gkGameObject* object, gkLogicLink* link, const gkString& name)
	:   gkLogicSensor(object, link, name),
		m_joystickIndex(0),m_elementIndex(0),
		m_axisThreshold(0), m_allEvents(false),
		m_eventType(0), m_axisDirection(0)
{
	m_dispatchType = DIS_JOY;
	connect();
}


gkLogicBrick* gkJoystickSensor::clone(gkLogicLink* link, gkGameObject* dest)
{
	gkJoystickSensor* sens = new gkJoystickSensor(*this);
	sens->cloneImpl(link, dest);
	return sens;
}


bool gkJoystickSensor::query(void)
{
	gkJoystick* js = gkWindowSystem::getSingleton().getJoystick(m_joystickIndex);
	if (js == 0)
		return false;

	switch (m_eventType)
	{
	case JT_AXIS:
		return (gkAbs(js->getAxisValue(m_elementIndex)) > m_axisThreshold) ? true : false;
	case JT_AXIS_PAIR:
		{
			int axis1 = js->getAxisValue(m_elementIndex * 2);
			int axis2 = js->getAxisValue(1 + m_elementIndex * 2);
			bool ret = false;
			if (m_allEvents)
			{
				ret = (gkAbs(axis1) > m_axisThreshold) ? true : false;
				ret |= (gkAbs(axis2) > m_axisThreshold) ? true : false;
			}
			else
			{
				switch (m_axisDirection)
				{
				case AD_LEFT:   ret = (axis1 < -1*(int)m_axisThreshold) ? true : false; break;
				case AD_RIGHT:  ret = (axis1 > (int)m_axisThreshold) ? true : false; break;
				case AD_BOTTOM: ret = (axis2 > (int)m_axisThreshold) ? true : false; break;
				case AD_TOP:    ret = (axis2 < -1*(int)m_axisThreshold) ? true : false; break;
				}
			}
			return ret;
		}
	case JT_BUTTON:
		if (m_allEvents)
		{
			for (int i = 0; i < (int)js->buttons.size(); i++)
				if (js->isButtonDown(i)) return true;
			return false;
		}
		else
			return js->isButtonDown(m_elementIndex);
	case JT_HAT:
	default:
		printf("todo!\n");
		break;

	}
	return false;
}
