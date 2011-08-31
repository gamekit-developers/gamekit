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
#include "gkLogicLink.h"
#include "gkGameObject.h"
#include "gkLogicSensor.h"
#include "gkLogicController.h"
#include "gkLogicActuator.h"
#include "gkLogicManager.h"


gkLogicLink::gkLogicLink() : m_state(0), m_debug(0), m_object(0), m_externalOwner(false)
{
}



gkLogicLink::~gkLogicLink()
{
	if (!m_sensors.empty())
	{
		utListIterator<BrickList> it(m_sensors);
		while (it.hasMoreElements())
			delete it.getNext();
	}
	if (!m_controllers.empty())
	{
		utListIterator<BrickList> it(m_controllers);
		while (it.hasMoreElements())
			delete it.getNext();
	}
	if (!m_actuators.empty())
	{
		utListIterator<BrickList> it(m_actuators);
		while (it.hasMoreElements())
			delete it.getNext();
	}
}



gkLogicLink* gkLogicLink::clone(gkGameObject* dest)
{
	gkLogicLink* link = gkLogicManager::getSingleton().createLink();
	link->m_object = dest;
	link->m_state = m_state;

	if (!m_actuators.empty())
	{
		utListIterator<BrickList> it(m_actuators);
		while (it.hasMoreElements())
		{
			gkLogicActuator* oact = (gkLogicActuator*)it.getNext();
			gkLogicActuator* nact = (gkLogicActuator*)oact->clone(link, dest);

			link->push(nact, oact);
		}
	}

	if (!m_controllers.empty())
	{

		utListIterator<BrickList> it(m_controllers);
		while (it.hasMoreElements())
		{
			gkLogicController* ocont = (gkLogicController*)it.getNext();
			gkLogicController* ncont = (gkLogicController*)ocont->clone(link, dest);

			gkActuators& acts = ocont->getActuators();
			if (!acts.empty())
			{
				for (UTsize i = 0; i < acts.size(); ++i)
				{
					gkLogicActuator* lnact = link->findActuator(acts.at(i));
					if (lnact)
						ncont->link(lnact);
				}
			}
			link->push(ncont, ocont);
		}
	}

	if (!m_sensors.empty())
	{
		utListIterator<BrickList> it(m_sensors);
		while (it.hasMoreElements())
		{
			gkLogicSensor*   osens = (gkLogicSensor*)it.getNext();
			gkLogicSensor*   nsens = (gkLogicSensor*)osens->clone(link, dest);

			gkControllers& conts = osens->getControllers();
			if (!conts.empty())
			{
				for (UTsize i = 0; i < conts.size(); ++i)
				{
					gkLogicController* lncont = link->findController(conts.at(i));
					if (lncont)
						nsens->link(lncont);
				}
			}
			link->push(nsens);
		}
	}
	return link;
}


void gkLogicLink::destroyInstance(void)
{
	gkLogicManager& mgr = gkLogicManager::getSingleton();
	mgr.notifyLinkInstanceDestroyed(this);

	for (UTsize i = 0; i < m_others.size(); ++i)
		mgr.notifyLinkInstanceDestroyed(m_others[i]);
}


void gkLogicLink::push(gkLogicSensor* v, void* user)
{
	GK_ASSERT(v);
	m_sensors.push_back(v);

	if (user)
		m_sfind.insert(user, v);
}


void gkLogicLink::push(gkLogicController* v, void* user)
{
	GK_ASSERT(v);
	m_controllers.push_back(v);

	if (user)
		m_cfind.insert(user, v);
}


void gkLogicLink::push(gkLogicActuator* v, void* user)
{
	GK_ASSERT(v);
	m_actuators.push_back(v);
	if (user)
		m_afind.insert(user, v);
}



gkLogicSensor* gkLogicLink::findSensor(const gkString& name)
{
	if (!m_sensors.empty())
	{
		utListIterator<BrickList> it(m_sensors);
		while (it.hasMoreElements())
		{
			gkLogicBrick* sc = it.getNext();
			if (sc->getName() == name)
				return static_cast<gkLogicSensor*>(sc);
		}
	}
	return 0;
}


gkLogicActuator* gkLogicLink::findActuator(const gkString& name)
{
	if (!m_actuators.empty())
	{

		utListIterator<BrickList> it(m_actuators);
		while (it.hasMoreElements())
		{

			gkLogicBrick* ac = it.getNext();
			if (ac->getName() == name)
				return static_cast<gkLogicActuator*>(ac);
		}
	}
	return 0;
}


gkLogicController* gkLogicLink::findController(const gkString& name)
{
	if (!m_controllers.empty())
	{

		utListIterator<BrickList> it(m_controllers);
		while (it.hasMoreElements())
		{
			gkLogicBrick* co = it.getNext();
			if (co->getName() == name) return static_cast<gkLogicController*>(co);
		}
	}
	return 0;
}



gkLogicSensor* gkLogicLink::findSensor(void* user)
{
	UTsize pos = m_sfind.find(user);
	if (pos != UT_NPOS)
		return static_cast<gkLogicSensor*>(m_sfind.at(pos));
	return 0;
}




gkLogicActuator* gkLogicLink::findActuator(void* user)
{
	UTsize pos = m_afind.find(user);
	if (pos != UT_NPOS)
		return static_cast<gkLogicActuator*>(m_afind.at(pos));
	return 0;
}



gkLogicController*  gkLogicLink::findController(void* user)
{
	UTsize pos = m_cfind.find(user);
	if (pos != UT_NPOS)
		return static_cast<gkLogicController*>(m_cfind.at(pos));
	return 0;
}




void gkLogicLink::notifyLink(gkLogicLink* link)
{
	if (this != link && m_others.find(link) == UT_NPOS)
		m_others.push_back(link);
}



bool gkLogicLink::hasLink(gkLogicLink* link)
{
	return link == this || m_others.find(link) != UT_NPOS;
}


void gkLogicLink::notifyState(void)
{
	if (!m_others.empty())
	{
		for (UTsize i = 0; i < m_others.size(); ++i)
			m_others[i]->m_state = m_state;
	}
}
