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
#include "gkActionManager.h"
#include "gkEngine.h"


gkActionStrip::gkActionStrip()
	:    m_scaledRange(-1.f, -1.f),
	     m_weight(1.f),
	     m_actionPlayer(),
	     m_blend(0.f, 0.f),
	     m_curBlend(1.f)
{
	m_actionPlayer.enable(true);
}

void gkActionStrip::setAction(gkAction* act)
{
	m_actionPlayer.setAction(act);
}

void gkActionStrip::setBlend(const gkVector2& io)
{
	m_blend = io;
}


void gkActionStrip::setDeltaRange(const gkVector2& v)
{
	m_scaledRange = v;
}


void gkActionStrip::evaluate(gkScalar time, gkScalar weight, gkGameObject* object)
{
	if (object)
	{
		gkScalar scaledTime = time * m_actionPlayer.getLength() / getLength();
		
		m_actionPlayer.setTimePosition(scaledTime);


		if (!m_actionPlayer.isDone())
		{
			gkScalar bi, bo;
			bi = m_blend.x;
			bo = m_blend.y;
			
			m_curBlend = 1.0f;
			
			if (!gkFuzzy(bi) && time < bi)
			{
				m_curBlend *= time / bi;
			}
			
			if (!gkFuzzy(bo) && (time + bo) > getLength())
			{
				m_curBlend *= (getLength() - time) / bo;
			}
			
			m_actionPlayer.setWeight(gkClampf(weight * m_weight * m_curBlend, 0.f, 1.f));
			m_actionPlayer.setObject(object);
			m_actionPlayer.evaluate(0.f);
		}
	}
}




void gkActionStrip::reset(void)
{
	m_actionPlayer.reset();
}



void gkActionStrip::enable(bool v)
{
	m_actionPlayer.enable(v);
}







gkActionSequence::gkActionSequence(gkResourceManager* creator, const gkResourceName& name, const gkResourceHandle& handle)
	:	gkAction(creator, name, handle)
{
}


gkActionSequence::~gkActionSequence()
{
	Sequences::Iterator it = m_seq.iterator();
	while (it.hasMoreElements())
		delete it.getNext();
}


void gkActionSequence::addItem(const gkString& act, const gkVector2& timeRange, const gkVector2& blendIO)
{
	gkAction* action = gkActionManager::getSingleton().getByName<gkAction>(act);
	addItem(action, timeRange, blendIO);
}


void gkActionSequence::addItem(gkAction* act, const gkVector2& timeRange, const gkVector2& blendIO)
{
	if (!act)
		return;


	// start and end time

	if (m_start > timeRange.x)
		m_start = timeRange.x;
	if (m_end < timeRange.y)
		m_end = timeRange.y;


	gkActionStrip* seq = new gkActionStrip();
	seq->setAction(act);


	seq->setBlend(blendIO);
	seq->setDeltaRange(timeRange);
	seq->enable(true);
	m_seq.push_back(seq);
}



//void gkActionSequence::evaluate(const gkScalar& time, const gkScalar& delta, const gkScalar& weight, gkGameObject* object) const
//{
//	UTsize i, s;
//	gkActionStrip* const* p;

//	i = 0;
//	s = m_seq.size();
//	p = m_seq.ptr();


//	while (i < s)
//	{
//		gkActionStrip* seq = p[i++];
//		const gkScalar st  = seq->getStart();
//		const gkScalar end = st + seq->getEnd();


//		if (time < st || time > end)
//		{
//			// out of current range
//			continue;
//		}


//		if (time <= m_start)
//		{
//			// out of initial starting range
//			continue;
//		}


////		m_active = seq->getAction();
//		seq->evaluate(time-st, weight, object);
//	}
//}
