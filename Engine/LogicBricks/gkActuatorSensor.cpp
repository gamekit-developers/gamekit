/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

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

#include "gkActuatorSensor.h"
#include "gkLogicManager.h"
#include "gkLogicDispatcher.h"
#include "gkGameObject.h"
#include "gkLogicLink.h"
#include "gkLogicActuator.h"


gkActuatorSensor::gkActuatorSensor(gkGameObject* object, gkLogicLink* link, const gkString& name)
	:    gkLogicSensor(object, link, name), m_actuatorName("")
{
	m_dispatchType = DIS_CONSTANT;
	connect();
}


gkLogicBrick* gkActuatorSensor::clone(gkLogicLink* link, gkGameObject* dest)
{
	gkActuatorSensor* sens = new gkActuatorSensor(*this);
	sens->cloneImpl(link, dest);
	return sens;
}


bool gkActuatorSensor::query(void)
{
	gkLogicActuator* act = m_object->getLogicBricks()->findActuator(m_actuatorName);
	return act->isPulseOn();
}
