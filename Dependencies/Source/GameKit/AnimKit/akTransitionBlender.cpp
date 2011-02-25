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
#include "akTransitionBlender.h"
#include "akAnimationPlayer.h"


akTransitionBlend::akTransitionBlend()
	:    m_priority(0),
	     m_way(0),
	     m_mode(akAnimationPlayer::AK_ACT_LOOP),
	     m_blend(0),
	     m_frames(0),
	     m_time(0.f),
	     m_base(0),
	     m_enabled(true)
{
}


akScalar akTransitionBlend::getLength(void) const
{
	if (m_base)
		return m_base->getLength();
	return 0.f;
}


void akTransitionBlend::setBlendFrames(akScalar frames)
{
	if (frames <= 0.001f)
		frames = 1;
	m_frames = 1.f / frames;
}



void akTransitionBlend::enable(bool v)
{
	m_enabled = v;

	if (m_base) m_base->setEnabled(m_enabled);
}


bool akTransitionBlend::evaluate(akScalar delta)
{
	if (!m_enabled || !m_base)
		return true;

	if (m_mode == akAnimationPlayer::AK_ACT_LOOP)
	{
		if (m_time >= getLength())
			m_time = 0.f;
		m_time += delta;
	}
	else
	{
		m_time += delta;
		if (m_time >= getLength())
			m_time = getLength();
	}
	
	
	if (m_way != AB_NONE)
	{
		m_blend = akClampf(m_blend + m_frames, 0.f, 1.f);
		m_base->setWeight(m_way == akTransitionBlend::AB_IN ? m_blend : 1.f - m_blend);
	}
	else
		m_base->setWeight(1.f);
	
	// Apply objects
	m_base->setTimePosition(m_time);
	//m_base->evaluate();
	
	
	return m_way == akTransitionBlend::AB_OUT ? (1.f - m_blend) <= 0.f : false;
}




void akTransitionBlend::reset(void)
{
	m_blend = 1.f;
	m_time  = 0.f;
	enable(false);
}





akTransitionBlender::akTransitionBlender()
	:    m_max(2)
{
}


akTransitionBlender::~akTransitionBlender()
{
}


bool akTransitionBlendSort(const akTransitionBlend& a, const akTransitionBlend& b)
{
	return a.getPriority() > b.getPriority();
}


void akTransitionBlender::push(akAnimationPlayer* action, const akScalar& frames, int mode, int priority)
{
	akTransitionBlend act;
	act.setAnimationPlayer(action);

	if (m_stack.find(act) == UT_NPOS)
	{
		act.enable(true);
		act.setBlendFrames(frames);
		act.setMode(mode);
		act.setPriority(priority);
		pushStack(act);
	}
}



void akTransitionBlender::pushStack(akTransitionBlend& blend)
{
	if (m_stack.size() >= m_max)
	{
		m_stack.back().reset();
		m_stack.pop_back();
	}
	m_stack.push_back(blend);

	if (blend.getPriority() > 0 && m_stack.size() > 1)
		m_stack.sort(akTransitionBlendSort);
}


void akTransitionBlender::evaluate(akScalar delta)
{
	if (!m_stack.empty())
	{
		UTsize i, s;
		Stack::Pointer p;

		i = 0;
		s = m_stack.size();
		p = m_stack.ptr();


		Stack done;

		while (i < s)
		{
			akTransitionBlend& ab = p[i];
			if (s == 1)
			{
				ab.setMode(akAnimationPlayer::AK_ACT_LOOP);
				ab.setDirection(akTransitionBlend::AB_NONE);
			}
			else
			{
				ab.setMode(akAnimationPlayer::AK_ACT_END);
				if (i == 0)
					ab.setDirection(akTransitionBlend::AB_OUT);
				else
					ab.setDirection(akTransitionBlend::AB_IN);
			}
			if (ab.evaluate(delta))
				done.push_back(ab);

			++i;
		}

		i = 0;
		s = done.size();
		p = done.ptr();

		while (i < s)
		{
			if (m_stack.size() > 1)
			{
				p[i].reset();
				m_stack.erase(p[i]);
			}

			++i;
		}



		if (s > 0)
		{
			done.clear();
			if (m_stack.size() > 1)
				m_stack.sort(akTransitionBlendSort);
		}

	}
}
