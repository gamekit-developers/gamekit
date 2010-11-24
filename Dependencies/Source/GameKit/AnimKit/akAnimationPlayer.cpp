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

#include "akAnimationPlayer.h"
#include "akAnimationSequence.h"


akAnimationPlayer::akAnimationPlayer()
	:	m_action(0),
	    m_evalTime(0.f),
	     m_weight(1.0),
	     m_enabled(true),
	     m_mode(AK_ACT_LOOP),
	     m_speedfactor(1.0f)
{
}

akAnimationPlayer::akAnimationPlayer(akAnimation* resource)
	:	m_action(resource),
	    m_evalTime(0.f),
	     m_weight(1.0),
	     m_enabled(true),
	     m_mode(AK_ACT_LOOP),
	     m_speedfactor(1.0f)
{
}

void akAnimationPlayer::setTimePosition(akScalar v)
{
	if (m_enabled && m_action)
	{
		m_evalTime = akClampf(v, 0.f, m_action->getLength());
	}
}



void akAnimationPlayer::setWeight(akScalar w)
{
	if (m_enabled)
	{
		m_weight = akClampf(w, 0, 1);
	}
}



void akAnimationPlayer::evaluate(akScalar tick)
{
	if (!m_enabled || !m_action)
		return;
	
	akScalar end = m_action->getLength();
	akScalar dt = m_speedfactor * tick;
	
	if (m_mode & AK_ACT_LOOP)
	{
		if (m_evalTime <= 0.f)
			m_evalTime = 0.f;
		if (m_evalTime >= end)
			m_evalTime = 0.f;
	}
	else
	{
		if (m_evalTime <= 0.f)
			m_evalTime = 0.f;

		if (m_evalTime + dt >= end)
			m_evalTime = end - dt;
	}

	m_evalTime += dt;

	akScalar time = m_evalTime;
	if (m_mode & AK_ACT_INVERSE)
		time = end - m_evalTime;

	evaluateImpl(time);
	
	
}


void akAnimationPlayer::reset(void)
{
	m_evalTime = 0.f;
//	m_weight = 1.f;
//	m_enabled = false;
}
