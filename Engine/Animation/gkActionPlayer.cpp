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

#include "gkActionPlayer.h"
#include "gkActionSequence.h"


gkActionPlayer::gkActionPlayer()
	:	m_action(0),
	    m_evalTime(1),
	     m_weight(1.0),
	     m_enabled(true),
	     m_mode(GK_ACT_LOOP),
	     m_object(0)
{
}

gkActionPlayer::gkActionPlayer(gkAction* resource, gkGameObject* object)
	:	m_action(resource),
	    m_evalTime(1),
	     m_weight(1.0),
	     m_enabled(true),
	     m_mode(GK_ACT_LOOP),
	     m_object(object)
{
}

void gkActionPlayer::setTimePosition(gkScalar v)
{
	if (m_enabled && m_action)
	{
		m_evalTime = gkClampf(v, m_action->getStart(), m_action->getEnd());
	}
}



void gkActionPlayer::setWeight(gkScalar w)
{
	if (m_enabled)
	{
		m_weight = gkClampf(w, 0, 1);
	}
}



void gkActionPlayer::evaluate(gkScalar time)
{
	if (!m_enabled || !m_action)
		return;
	
	gkScalar start = m_action->getStart();
	gkScalar end = m_action->getEnd();
	
	if (m_mode & GK_ACT_LOOP)
	{
		if (m_evalTime <= start)
			m_evalTime = start;
		if (m_evalTime >= end)
			m_evalTime = start;
	}
	else
	{
		if (m_evalTime <= start)
			m_evalTime = start;

		if (m_evalTime + time >= end)
			m_evalTime = end - time;
	}



	m_evalTime += time;

	gkScalar tick = m_evalTime;
	if (m_mode & GK_ACT_INVERSE)
		tick = (end - start) - m_evalTime;

	gkScalar delta = (tick - start) / (end - start);

	m_action->evaluate(tick, delta, m_weight, m_object);
	
	
}


void gkActionPlayer::reset(void)
{
	m_evalTime = 0.f;
	m_weight = 1.f;
	m_enabled = false;
}
