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
#include "gkLogicSensor.h"
#include "gkLogicController.h"
#include "gkLogicManager.h"
#include "gkLogicLink.h"
#include "gkLogicDispatcher.h"



enum TapMode
{
	TAP_OUT = -2,
	TAP_IN,
};


gkLogicSensor::gkLogicSensor(gkGameObject* object, gkLogicLink* link, const gkString& name)
	:       gkLogicBrick(object, link, name),
	        m_freq(0), m_tick(0), m_pulse(PM_IDLE),
	        m_invert(false), m_positive(false), m_suspend(false), m_tap(false), m_firstExec(true),
	        m_sorted(false), m_isDetector(false),
	        m_oldState(-1),
	        m_firstTap(TAP_IN), m_lastTap(TAP_OUT),
	        m_dispatchType(-1)
{
}


gkLogicSensor::~gkLogicSensor()
{
	disconnect();
}



void gkLogicSensor::cloneImpl(gkLogicLink* link, gkGameObject* dest)
{
	gkLogicBrick::cloneImpl(link, dest);
	m_controllers.clear();
	reset();
	connect();
}


void gkLogicSensor::reset(void)
{
	m_oldState = -1;
	m_firstTap = TAP_IN;
	m_firstExec = true;
	m_positive  = false;
}


void gkLogicSensor::connect(void)
{
	if (m_dispatchType != -1)
		gkLogicManager::getSingleton().getDispatcher(m_dispatchType).connect(this);
}


void gkLogicSensor::disconnect(void)
{
	if (m_dispatchType != -1)
		gkLogicManager::getSingleton().getDispatcher(m_dispatchType).disconnect(this);
}


void gkLogicSensor::link(gkLogicController* cont)
{
	UT_ASSERT(cont && m_controllers.find(cont) == UT_NPOS);

	m_controllers.push_back(cont);
	cont->link(this);
}


bool gkLogicController_cSort(gkLogicController* const& a, gkLogicController* const& b)
{
	return a->getPriority() < b->getPriority();
}



void gkLogicSensor::sort(void)
{
	m_controllers.sort(gkLogicController_cSort);

	gkControllerIterator it(m_controllers);
	while (it.hasMoreElements())
		it.getNext()->sort();
}


bool gkLogicSensor::isPositive(void) const
{
	bool result = m_positive;

	if (m_invert)
	{
		if (!(m_tap && !(m_pulse & PM_TRUE)))
			result = !result;
	}

	return result;
}



void gkLogicSensor::execute(void)
{
	if (!inActiveState())
	{
		if (m_oldState != m_link->getState())
		{
			m_oldState = m_link->getState();
			m_firstTap = TAP_IN;
			m_firstExec = true;
			m_positive  = false;
		}
		return;
	}

	if (m_suspend || m_controllers.empty())
		return;

	bool doDispatch = false, detDispatch = false;
	if (m_oldState != m_link->getState())
	{
		m_firstExec = true;
		m_positive  = false;
		m_firstTap = TAP_IN;

		m_oldState = m_link->getState();
		if (m_isDetector)
			doDispatch = true;
	}

	bool doQuery = false;
	if (m_firstExec || (++m_tick > m_freq) || m_pulse == PM_IDLE)
	{
		doQuery = true;
		m_tick = 0;
	}


	if (doQuery)
	{
		// Sensor detection.
		bool lp = m_positive;

		if (m_listener)
		{
			if (m_listener->m_mode == gkLogicBrick::Listener::OVERIDE)
				m_positive = m_listener->executeEvent(this);
			else
				m_positive = m_listener->executeEvent(this) && query();
		}
		else
			m_positive = query();

		// Sensor Pulse.
		if (m_pulse == PM_IDLE)
			doDispatch = lp != m_positive;
		else
		{
			if (m_pulse & PM_TRUE)
			{
				if (!m_invert)
					doDispatch = (lp != m_positive) || m_positive;
				else
					doDispatch = (lp != m_positive) || !m_positive;
			}
			if (m_pulse & PM_FALSE)
			{
				if (!m_invert)
					doDispatch = (lp != m_positive) || !m_positive;
				else
					doDispatch = (lp != m_positive) || m_positive;
			}
		}

		// Tap mode (Switch On->Switch Off)
		if (m_tap && !(m_pulse & PM_TRUE))
		{
			doQuery = m_positive;
			if (m_invert)
				doQuery = !doQuery;

			doDispatch = false;
			m_pulseState = BM_OFF;

			if (m_firstTap == TAP_IN && doQuery)
			{
				doDispatch = true;
				m_positive = true;
				m_pulseState = BM_ON;
				m_firstTap = TAP_OUT;
				m_lastTap = TAP_IN;
			}
			else if (m_lastTap == TAP_IN)
			{
				m_positive = false;
				doDispatch = true;
				m_lastTap = TAP_OUT;
			}
			else
			{
				m_positive = false;
				if (!doQuery)
					m_firstTap  = TAP_IN;
			}
		}
		else m_pulseState = isPositive() ? BM_ON : BM_OFF;

		if (m_firstExec)
		{
			m_firstExec = false;
			if (m_invert && !doDispatch)
				doDispatch = true;
		}
		if (!doDispatch)
			doDispatch = detDispatch;

		// Dispatch results
		if (doDispatch) dispatch();
	}
}

void gkLogicSensor::dispatch(void)
{
	if (!m_controllers.empty())
	{
		bool value = isPositive();

		gkLogicManager& mgr = gkLogicManager::getSingleton();

		gkControllerIterator it(m_controllers);
		while (it.hasMoreElements())
		{
			gkLogicController* cont = it.getNext();
			// fire pulse
			mgr.push(this, cont, value);
		}
	}
}
