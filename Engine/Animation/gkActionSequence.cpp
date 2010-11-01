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
#include "gkActionSequence.h"
#include "gkEngine.h"


gkActionStrip::gkActionStrip()
	:    m_scaledRange(-1.f, -1.f),
	     m_scaleDelta(GK_INFINITY),
	     m_curBlend(0.f),
	     m_parentWeight(1.f),
	     m_action(0),
	     m_blend(0.f, 0.f),
	     m_init(false),
	     m_time(0.f),
	     m_prevDelta(0.f)
{
}


void gkActionStrip::setBlend(const gkVector2& io)
{
	m_blend = io;
	m_init = false;
}


void gkActionStrip::setDeltaRange(const gkVector2& v)
{
	m_scaledRange = v;
}


void gkActionStrip::evaluate(const gkScalar& delta)
{
	if (m_action)
	{
		m_action->enable(true);

		if (!m_init)
		{
			// Set the default seed
			m_init = true;
			m_parentWeight = 1.f;

			m_curBlend = 0.f;
			m_time = 0.f;


			m_action->setTimePosition(0.f);

			if (gkFuzzy(m_blend.x))
				m_curBlend = 1.f;

			m_action->setWeight(m_curBlend);

			m_prevDelta = GK_INFINITY;
		}

		if (delta != m_prevDelta)
		{
			m_prevDelta = delta;

			gkScalar olen = m_action->getLength();
			gkScalar nlen = getLength();
			gkScalar dlen = 0.f;

			if (nlen < olen)
				dlen = olen - nlen;
			else
				dlen = nlen - olen;

			m_scaleDelta = m_prevDelta + (dlen / gkEngine::getTickRate());
		}

		gkScalar bi, bo;
		bi = m_blend.x;
		bo = m_blend.y;

		m_time += (m_scaleDelta);
		m_action->setTimePosition(m_time);

		if (!gkFuzzy(bi))
		{
			if (m_curBlend < 1.f)
			{
				m_curBlend += 1.f / bi;
			}
		}


		if (!gkFuzzy(bo) && (m_time + bo) > getLength())
		{
			m_curBlend -= 1.f / bo;
		}
		else if (m_time >= bo)
		{
			m_curBlend = 1.f;
		}

		m_action->setWeight(gkClampf(m_curBlend, 0.f, m_parentWeight));

		if (!m_action->isDone())
			m_action->evaluate(0.f);
	}
}




void gkActionStrip::reset(void)
{
	m_init = false;
	if (m_action)
		m_action->reset();
}



void gkActionStrip::enable(bool v)
{
	if (m_action)
		m_action->enable(v);
}



void gkActionStrip::setParentWeight(gkScalar w)
{
	m_parentWeight = w;
}






gkActionSequence::gkActionSequence()
	:    m_timeDirty(true),
	     m_range(FLT_MAX, -FLT_MAX),
	     m_evalTime(0.f),
	     m_default(0),
	     m_weight(1.f),
	     m_defTime(0.f),
	     m_enabled(true),
	     m_active(0)
{
}


gkActionSequence::~gkActionSequence()
{
	Sequences::Iterator it = m_seq.iterator();
	while (it.hasMoreElements())
		delete it.getNext();
}



void gkActionSequence::addItem(gkAction* act, const gkVector2& timeRange, const gkVector2& blendIO)
{
	if (!act)
		return;


	// start and end time

	if (m_range.x > timeRange.x)
		m_range.x = timeRange.x;
	if (m_range.y < timeRange.y)
		m_range.y = timeRange.y;


	gkActionStrip* seq = new gkActionStrip();
	seq->setAction(act);


	seq->setBlend(blendIO);
	seq->setDeltaRange(timeRange);
	seq->enable(true);
	m_seq.push_back(seq);

	clearWeights();

}


void gkActionSequence::clearWeights(void)
{
	UTsize i, s;
	Sequences::Pointer p;

	i = 0;
	s = m_seq.size();
	p = m_seq.ptr();


	while (i < s)
	{
		gkActionStrip* seq = p[i];
		seq->reset();
		++i;
	}
}



void gkActionSequence::reset(void)
{
	m_enabled = false;
	m_evalTime = 0.f;
	m_defTime = 0.f;
	clearWeights();


	if (m_default)
	{
		m_default->enable(false);
		m_default->setWeight(1.f);
		m_default->setTimePosition(0.f);
	}
}



void gkActionSequence::setWeight(gkScalar w)
{
	if (m_enabled)
	{
		if (m_weight != w)
		{
			m_weight = w;
			UTsize i, s;
			Sequences::Pointer p;

			i = 0;
			s = m_seq.size();
			p = m_seq.ptr();

			while (i < s) p[i++]->setParentWeight(m_weight);
		}
	}
}

void gkActionSequence::setTimePosition(const gkScalar& v)
{
	if (m_enabled)
	{
		m_evalTime = v;
		m_timeDirty = true;
	}

}

void gkActionSequence::evaluate(const gkScalar& delta)
{
	if (!m_enabled)
		return;

	UTsize i, s;
	Sequences::Pointer p;

	i = 0;
	s = m_seq.size();
	p = m_seq.ptr();


	if (m_timeDirty)
		m_timeDirty = false;
	else
		m_evalTime += delta;

	if (m_default)
	{
		// Loop in background

		m_defTime += delta;
		if (m_defTime >= m_default->getLength())
			m_defTime = 0.f;

	}

	while (i < s)
	{
		gkActionStrip* seq = p[i++];
		const gkScalar st  = seq->getStart();
		const gkScalar end = st + seq->getEnd();


		if (m_evalTime < st || m_evalTime > end)
		{
			// out of current range
			continue;
		}


		if (m_evalTime <= m_range.x)
		{
			// out of initial starting range
			continue;
		}


		m_active = seq->getAction();
		seq->evaluate(delta);
	}
}
