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


gkLogicSensor::gkLogicSensor(gkGameObject *object, gkLogicLink *link, const gkString &name)
:       gkLogicBrick(object, link, name), m_freq(0), m_tick(0), m_pulse(PULSE_NONE), m_tap(-1),
        m_invert(false), m_positive(false), m_suspend(false), m_sorted(false), m_isDetector(false),
        m_inverted(false), m_oldState(-1), m_firstTap(-1)
{
}



void gkLogicSensor::tick(void)
{
    if (m_suspend) return;

    // seed
    if (m_oldState == -1)
        m_oldState = m_link->getState();

    bool doQuery = false;
    if (++m_tick > m_freq || (m_firstTap == -1)) {
        doQuery = true;
        m_tick = 0;
        if (m_firstTap == -1)
            m_firstTap = -2;
    }

    if (doQuery) {

        // test
        m_positive = query();

        // invert
        if (m_invert || (m_pulse & PULSE_NEGATIVE))
            m_positive = !m_positive;

        if (m_tap) {
            if (m_firstTap != (int)m_positive)
                m_firstTap = (int)m_positive;
            else return;
        }

        if (m_isDetector) {
            if (m_oldState != m_link->getState()) {
                m_oldState = m_link->getState();
                m_positive = true;
            }
            else return;
        }

        // dispatch results
        if (m_positive || m_inverted)
            gkLogicManager::getSingleton().push(this);
    }
}


typedef gkLogicController* T;
bool gkLogicSensor_gSort(const T &a, const T &b)
{
    return a->getPriority() > b->getPriority();
}


void gkLogicSensor::dispatch(void)
{
    if (!m_controllers.empty()) {
        // send signal to all controllers

        if (!m_sorted) {
            m_sorted = true;
            m_controllers.sort(gkLogicSensor_gSort);
        }

        utListIterator<Controllers> it(m_controllers);
        while (it.hasMoreElements())
            it.getNext()->relay();
    }

}
