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
#include "gkLogicController.h"
#include "gkLogicSensor.h"
#include "gkLogicActuator.h"
#include "gkLogicLink.h"
#include "gkGameObject.h"



gkLogicController::gkLogicController(gkGameObject* object, gkLogicLink* link, const gkString& name)
	:       gkLogicBrick(object, link, name), m_activeState(false)
{
}


void gkLogicController::cloneImpl(gkLogicLink* link, gkGameObject* dest)
{
	gkLogicBrick::cloneImpl(link, dest);
	m_sensors.clear();
	m_actuators.clear();
}



void gkLogicController::_execute(void)
{
	if (m_listener)
	{
		if (m_listener->executeEvent(this) && 
				m_listener->m_mode != gkLogicBrick::Listener::OVERIDE)
			execute();
	}
	else
		execute();
}



bool gkLogicController_sSort(gkLogicSensor* const& a, gkLogicSensor* const& b)
{
	return a->getPriority() < b->getPriority();
}


bool gkLogicController_aSort(gkLogicActuator* const& a, gkLogicActuator* const& b)
{
	return a->getPriority() < b->getPriority();
}


void gkLogicController::sort(void)
{
	m_sensors.sort(gkLogicController_sSort);
	m_actuators.sort(gkLogicController_aSort);
}


void gkLogicController::link(gkLogicSensor* v)
{
	UT_ASSERT(v && m_sensors.find(v) == UT_NPOS);
	m_sensors.push_back(v);


	gkLogicLink* olink = v->getLink();


	// append state bits

	m_object->setState(m_link->getState());
	v->setMask(v->getMask() | getMask());
	v->setDebugMask(getDebugMask());

	olink->setState(m_link->getState());

	m_link->notifyLink(olink);
	olink->notifyLink(m_link);
}



void gkLogicController::link(gkLogicActuator* v)
{
	UT_ASSERT(v && m_actuators.find(v) == UT_NPOS);
	m_actuators.push_back(v);

	v->setPriority(getPriority());

	gkLogicLink* olink = v->getLink();

	// append state bits

	m_object->setState(m_link->getState());
	v->setMask(v->getMask() | getMask());
	v->setDebugMask(getDebugMask());

	olink->setState(m_link->getState());
	m_link->notifyLink(olink);
	olink->notifyLink(m_link);
}
