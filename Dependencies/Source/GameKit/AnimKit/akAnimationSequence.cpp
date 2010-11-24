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
#include "akAnimationSequence.h"


akAnimationStrip::akAnimationStrip()
	:    m_scaledRangeStart(-1.f),
	     m_scaledRangeEnd(-1.f),
	     m_weight(1.f),
	     m_blendin(0.f),
	     m_blendout(0.f),
	     m_curBlend(1.f)
{
}

void akAnimationStrip::setAnimation(akAnimation* act)
{
	m_anim = act;
}

void akAnimationStrip::setBlend(const akScalar& blendin, const akScalar& blendout)
{
	m_blendin = blendin;
	m_blendout = blendout;
}


void akAnimationStrip::setDeltaRange(const akScalar& start, const akScalar& end)
{
	m_scaledRangeStart = start;
	m_scaledRangeEnd = end;
}


void akAnimationStrip::evaluate(akScalar time, akScalar weight, void* object)
{
	if (object)
	{
		akScalar bi, bo;
		akScalar actualWeight;
		akScalar scaledTime = time * m_anim->getLength() / getLength();
		
		bi = m_blendin;
		bo = m_blendout;
		
		m_curBlend = 1.0f;
		
		if (!akFuzzy(bi) && time < bi)
		{
			m_curBlend *= time / bi;
		}
		
		if (!akFuzzy(bo) && (time + bo) > getLength())
		{
			m_curBlend *= (getLength() - time) / bo;
		}
		
		actualWeight = akClampf(weight * m_weight * m_curBlend, 0.f, 1.f);
		
		m_anim->evaluate(scaledTime, actualWeight, object);
	}
}



akAnimationSequence::akAnimationSequence()
	:	akAnimation()
{
}


akAnimationSequence::~akAnimationSequence()
{
	Sequences::Iterator it = m_seq.iterator();
	while (it.hasMoreElements())
		delete it.getNext();
}


void akAnimationSequence::addItem(akAnimation* act, const akScalar& start, const akScalar& end, const akScalar& blendin, const akScalar& blendout, bool fitLength)
{
	if (!act)
		return;

	if ( fitLength && m_length < end)
		m_length = end;


	akAnimationStrip* seq = new akAnimationStrip();
	seq->setAnimation(act);


	seq->setBlend(blendin , blendout);
	seq->setDeltaRange(start, end);
	m_seq.push_back(seq);
}



void akAnimationSequence::evaluate(const akScalar& time, const akScalar& weight, void* object) const
{
	UTsize i, s;
	akAnimationStrip* const* p;

	i = 0;
	s = m_seq.size();
	p = m_seq.ptr();


	while (i < s)
	{
		akAnimationStrip* seq = p[i++];
		const akScalar st  = seq->getStart();
		const akScalar end = st + seq->getEnd();


		if (time < st || time > end)
		{
			// out of current range
			continue;
		}


		if (time <= 0)
		{
			// out of initial starting range
			continue;
		}

		seq->evaluate(time-st, weight, object);
	}
}
