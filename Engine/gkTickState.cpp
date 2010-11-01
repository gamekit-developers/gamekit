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
#include "gkTickState.h"

#define gkGetTickCount(timerPtr) ((unsigned long)(timerPtr)->getTimeMilliseconds())
#define gkMSScale                gkScalar(0.001)


gkTickState::gkTickState()
	:   m_ticks(0),
		m_rate(0),
		m_skip(0),
		m_loop(0),
		m_cur(0),
		m_next(0),
		m_fixed(0.f),
		m_invt(0),
		m_clock(0),
		m_lock(false),
		m_init(false)
{
	initialize(60);
}



gkTickState::gkTickState(int rate)
	:   m_ticks(0),
		m_rate(0),
		m_skip(0),
		m_loop(0),
		m_cur(0),
		m_next(0),
		m_fixed(0.f),
		m_invt(0),
		m_clock(0),
		m_lock(false),
		m_init(false)
{
	initialize(rate);
}



gkTickState::~gkTickState()
{
	delete m_clock;
	m_clock = 0;
}


void gkTickState::initialize(int rate)
{
	m_rate = gkMax<unsigned long>(1, rate);

	m_ticks = 1000 / m_rate;

	// time out at 1/5 rate
	m_skip  = gkMax<unsigned long>(m_rate / 5, 1);

	m_invt  = gkScalar(1.0) / (gkScalar)m_ticks;
	m_fixed = gkScalar(1.0) / (gkScalar)m_rate;

	if (m_clock)
		delete m_clock;
	m_clock = new btClock();
	m_init = false;
}


void gkTickState::reset(void)
{
	m_init = false;
}



void gkTickState::tick(void)
{
	GK_ASSERT(m_clock);

	beginTickImpl();


	m_loop = 0;
	m_lock = false;

	if (!m_init)
	{
		// initialize timer states
		m_init = true;
		m_clock->reset();
		m_cur = m_next = gkGetTickCount(m_clock);
	}



	while ((m_cur = gkGetTickCount(m_clock)) > m_next && m_loop < m_skip)
	{
		tickImpl(m_fixed);

		// test for a long tick, and stop if were over
		if ( (( gkGetTickCount(m_clock) - m_cur) * gkMSScale) > m_fixed)
		{
			m_lock = true;
			break;
		}

		m_next += m_ticks;
		++m_loop;
	}

	if (m_lock || m_cur > m_next)
	{
		// sync tick back to a usable state
		m_cur = m_next = gkGetTickCount(m_clock);
	}

	endTickImpl();
}
