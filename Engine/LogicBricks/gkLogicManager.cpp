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
#include "gkLogicManager.h"
#include "gkLogicLink.h"
#include "gkLogicDispatcher.h"
#include "gkLogicActuator.h"
#include "gkLogicController.h"
#include "gkMouseSensor.h"
#include "gkKeyboardSensor.h"
#include "gkCollisionSensor.h"
#include "gkJoystickSensor.h"
#include "gkLogger.h"
#include "gkDebugScreen.h"




gkLogicManager::gkLogicManager()
{
	m_sort = true;
	m_dispatchers = new gkAbstractDispatcherPtr[DIS_MAX];
	m_dispatchers[DIS_CONSTANT]     = new gkConstantDispatch;
	m_dispatchers[DIS_KEY]          = new gkKeyDispatch;
	m_dispatchers[DIS_MOUSE]        = new gkMouseDispatch;
	m_dispatchers[DIS_COLLISION]    = new gkCollisionDispatch;
	m_dispatchers[DIS_JOY]          = new gkJoyDispatch;
}


gkLogicManager::~gkLogicManager()
{
	clear();

	for (int i = 0; i < DIS_MAX; ++ i)
		delete m_dispatchers[i];

	delete []m_dispatchers;
	m_dispatchers = 0;
}



void gkLogicManager::clear(void)
{
	m_sort = true;
	if (m_dispatchers)
	{
		for (int i = 0; i < DIS_MAX; ++ i)
			m_dispatchers[i]->clear();
	}

	if (!m_links.empty())
	{
		gkLogicLink* node = m_links.begin();
		while (node)
		{
			gkLogicLink* tmp = node;
			node = node->getNext();
			delete tmp;
		}
		m_links.clear();
	}
	m_cin.clear();
	m_ain.clear();
	m_aout.clear();

	m_updateBricks.clear();
}


void gkLogicManager::clearActive(gkLogicLink* link)
{
	// remove from the active/running actuator list

	if (link)
	{
		UTsize fnd;

		utListIterator<gkLogicLink::BrickList> iter(link->getControllers());
		while (iter.hasMoreElements())
		{
			gkLogicController* cont = (gkLogicController*)iter.getNext();

			// switch off controllers
			cont->setActive(false);
			cont->setPulse(BM_OFF);
			cont->notifyLinkDestroyed();


			if ((fnd = m_cin.find(cont)) != UT_NPOS)
				m_cin.erase(fnd);
		}

		iter = utListIterator<gkLogicLink::BrickList>(link->getActuators());
		while (iter.hasMoreElements())
		{
			gkLogicActuator* act = (gkLogicActuator*)iter.getNext();

			// switch off actuators
			act->setActive(false);
			act->setPulse(BM_OFF);
			act->notifyLinkDestroyed();

			if ((fnd = m_ain.find(act)) != UT_NPOS)
				m_ain.erase(fnd);
			if (( fnd = m_aout.find(act)) != UT_NPOS)
				m_aout.erase(fnd);
		}
	}
}



void gkLogicManager::destroy(gkLogicLink* link)
{
	if (!m_links.empty())
	{

		if (m_links.find(link))
		{
			m_links.erase(link);
			clearActive(link);
			delete link;
		}
	}

}


gkLogicLink* gkLogicManager::createLink(void)
{
	gkLogicLink* link = new gkLogicLink();
	m_links.push_back(link);
	return link;
}

#define GK_DEBUG_EXEC 1


void gkLogicManager::push(gkLogicSensor* a, gkLogicController* b, bool stateValue)
{
	if (b->inActiveState())
	{
#ifdef GK_DEBUG_EXEC
		if (b->wantsDebug())
			dsPrintf("Push: Sensor %s to Controller %s: %s\n", a->getName().c_str(), b->getName().c_str(), (stateValue ? "On" : "Off"));
#endif
		push(b, a, m_cin, stateValue);
	}
}



void gkLogicManager::push(gkLogicController* ctrl, gkLogicActuator* act, bool stateValue)
{
	if (ctrl->inActiveState())
	{
#ifdef GK_DEBUG_EXEC
		if (act->wantsDebug())
			dsPrintf("Push: Controller %s to Actuator %s: %s\n", ctrl->getName().c_str(), act->getName().c_str(), (stateValue ? "On" : "Off"));
#endif


		if (!m_tickActuators.find(act))
		{
			m_tickActuators.push_back(act);
			act->setPulse(stateValue ? BM_ON : BM_OFF);
		}
		else if (stateValue)
		{
			act->setPulse(true);
		}

		if (!act->isActive())
		{
			act->setActive(true);
			m_ain.push_back(act);
		}

	}
}


void gkLogicManager::push(gkLogicBrick* a, gkLogicBrick* b, Bricks& in, bool stateValue)
{
	a->setPulse(stateValue ? BM_ON : BM_OFF);

	if (!a->isActive())
	{
		a->setActive(true);
		in.push_back(a);
	}
}


void gkLogicManager::notifyState(unsigned int state, gkLogicLink* link)
{
	if (!m_ain.empty())
	{
		UTsize i, s, f = 0;
		Bricks::Pointer b;

		i = 0; s = m_ain.size();
		b = m_ain.ptr();
		while (i < s)
		{
			if (!b[i]->getLink()->hasLink(link))
			{
				++i;
				continue;
			}

#ifdef GK_DEBUG_EXEC
			if (f == 0 && b[i]->wantsDebug())
			{
				dsPrintf("===== State Change %i =====\n", state);
				f = 1;
			}
#endif

			if (!(b[i]->getMask() & state))
			{
				m_aout.push_back(b[i]);
#ifdef GK_DEBUG_EXEC
				if (b[i]->wantsDebug())
					dsPrintf("Pop:  State %s\n", b[i]->getName().c_str());
#endif
			}
			++i;
		}
	}
}



void gkLogicManager::notifySceneInstanceDestroyed(void)
{
	if (m_dispatchers)
	{
		for (int i = 0; i < DIS_MAX; ++ i)
			m_dispatchers[i]->reset();
	}

	if (!m_links.empty())
	{
		gkLogicLink* node = m_links.begin();
		while (node)
		{
			clearActive(node);
			node = node->getNext();
		}
	}
}


void gkLogicManager::notifyLinkInstanceDestroyed(gkLogicLink* link)
{
	clearActive(link);
}


void gkLogicManager::notifySort(void)
{
	m_sort = true;
}



void gkLogicManager::clearActuators(void)
{
	if (!m_aout.empty())
	{
		UTsize i, s;
		Bricks::Pointer b;
		i = 0; s = m_aout.size();
		b = m_aout.ptr();
		while (i < s)
		{
#ifdef GK_DEBUG_EXEC
			if (b[i]->wantsDebug())
				dsPrintf("Pop:  Actuator %s\n", b[i]->getName().c_str());
#endif
			b[i]->setActive(false);
			m_ain.erase(b[i]);
			++i;
		}
		m_aout.clear(true);

		if (m_ain.empty())
			m_ain.clear(true);
	}
	m_tickActuators.clear();
}

void gkLogicManager::sort(void)
{
	if (m_dispatchers)
	{
		UTsize i = 0;
		while (i < DIS_MAX)
			m_dispatchers[i++]->sort();
	}
}



void gkLogicManager::update(gkScalar delta)
{

	UTsize i, s;
	Bricks::Pointer b;

	if (m_sort)
	{
		sort();
		m_sort = false;
	}

	i = 0;
	while (i < DIS_MAX)
		m_dispatchers[i++]->dispatch();

	if (!m_cin.empty())
	{
		i = 0; s = m_cin.size();
		b = m_cin.ptr();
		while (i < s)
		{
			static_cast<gkLogicController*>(b[i])->_execute();
			b[i]->setActive(false);
			++i;
		}
		m_cin.clear(true);
	}

	if (!m_ain.empty())
	{
		i = 0; s = m_ain.size();
		b = m_ain.ptr();
		while (i < s)
		{
			static_cast<gkLogicActuator*>(b[i])->_execute();
			if (b[i]->isPulseOff())
				m_aout.push_back(b[i]);
			++i;
		}
	}

	clearActuators();

	BrickSet::Iterator iter(m_updateBricks);
	while (iter.hasMoreElements())
		iter.getNext()->update();
}


UT_IMPLEMENT_SINGLETON(gkLogicManager);
