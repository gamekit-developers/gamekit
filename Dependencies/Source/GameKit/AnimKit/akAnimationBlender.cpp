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
#include "akAnimationBlender.h"
#include "akAnimationPlayer.h"


akAnimationBlend::akAnimationBlend()
	:    m_priority(0),
	     m_way(0),
	     m_mode(AK_ACT_LOOP),
	     m_blend(0),
	     m_frames(0),
	     m_time(0.f),
	     m_base(0),
	     m_enabled(true)

{
}


akScalar akAnimationBlend::getLength(void) const
{
	if (m_base)
		return m_base->getLength();
	return 0.f;
}


void akAnimationBlend::setBlendFrames(akScalar frames)
{
	if (frames <= 0.001f)
		frames = 1;
	m_frames = 1.f / frames;
}



void akAnimationBlend::enable(bool v)
{
	m_enabled = v;

	if (m_base) m_base->enable(m_enabled);
}


bool akAnimationBlend::evaluate(akScalar delta)
{
	if (!m_enabled || !m_base)
		return true;

	if (m_mode == AK_ACT_LOOP)
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
		m_base->setWeight(m_way == akAnimationBlend::AB_IN ? m_blend : 1.f - m_blend);
	}
	else
		m_base->setWeight(1.f);
	
	// Apply objects
	//m_base->setTimePosition(m_time);
	//m_base->evaluate(0.f);
	m_base->evaluate(delta);
	
	
	return m_way == akAnimationBlend::AB_OUT ? (1.f - m_blend) <= 0.f : false;
}




void akAnimationBlend::reset(void)
{
	m_blend = 1.f;
	m_time  = 0.f;
	enable(false);
}





akAnimationBlender::akAnimationBlender()
	:    m_max(2)
{
}


akAnimationBlender::~akAnimationBlender()
{
}


bool akAnimationBlendSort(const akAnimationBlend& a, const akAnimationBlend& b)
{
	return a.getPriority() > b.getPriority();
}


void akAnimationBlender::push(akAnimationPlayer* action, const akScalar& frames, int mode, int priority)
{
	akAnimationBlend act;
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



void akAnimationBlender::pushStack(akAnimationBlend& blend)
{
	if (m_stack.size() >= m_max)
	{
		m_stack.back().reset();
		m_stack.pop_back();
	}
	m_stack.push_back(blend);

	if (blend.getPriority() > 0 && m_stack.size() > 1)
		m_stack.sort(akAnimationBlendSort);
}


void akAnimationBlender::remove(akAnimationPlayer* action)
{
	akAnimationBlend act;
	act.setAnimationPlayer(action);

	m_stack.erase(m_stack.find(act));
}

void akAnimationBlender::evaluate(akScalar delta)
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
			akAnimationBlend& ab = p[i];
			if (s == 1)
			{
				ab.setMode(AK_ACT_LOOP);
				ab.setDirection(akAnimationBlend::AB_NONE);
			}
			else
			{
				ab.setMode(AK_ACT_END);
				if (i == 0)
					ab.setDirection(akAnimationBlend::AB_OUT);
				else
					ab.setDirection(akAnimationBlend::AB_IN);
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
				m_stack.sort(akAnimationBlendSort);
		}

	}
}
