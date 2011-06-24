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
#include "akAnimationClip.h"


akAnimationPlayer::akAnimationPlayer()
	:	m_clip(0),
		m_evalTime(0.f),
		m_mode(AK_ACT_END),
		m_speedfactor(1.0f),
		m_enabled(false),
		m_weight(0.0),
		m_length(0.0),
		m_mask(0)
{
}

akAnimationPlayer::akAnimationPlayer(akAnimationClip* clip)
	:	m_clip(clip),
		m_evalTime(0.f),
		m_mode(AK_ACT_END),
		m_speedfactor(1.0f),
		m_enabled(false),
		m_weight(0.0),
		m_mask(0)
{
	m_length = clip->getLength();
}

void akAnimationPlayer::setAnimationClip(akAnimationClip *v)
{
	m_clip = v;
	m_length = m_clip->getLength();
}

void akAnimationPlayer::setTimePosition(akScalar v)
{
	if( v != m_evalTime && m_length > 0)
	{
		m_evalTime = v;
	
		if (m_mode & AK_ACT_LOOP)
		{
			while (m_evalTime >= m_length)
				m_evalTime -= m_length;
		}
		else
		{
			m_evalTime = akClampf(m_evalTime, 0.f, m_length);
		}
	}
}

void akAnimationPlayer::setUniformTimePosition(akScalar v)
{
	setTimePosition( v*m_length );
}

akScalar akAnimationPlayer::getUniformTimePosition(void) const
{
	return m_evalTime/m_length;
}

void akAnimationPlayer::stepTime(akScalar tick)
{
	setTimePosition(m_evalTime + tick * m_speedfactor);
}


void akAnimationPlayer::reset(void)
{
	m_evalTime = 0.f;
}

void akAnimationPlayer::evaluate(akPose *pose) const
{
	if (m_enabled && m_clip)
	{
		m_clip->evaluate(pose, m_evalTime, m_weight, getUniformTimePosition(), m_mask);
	}
}

void akAnimationPlayer::evaluate(akSkeletonPose *pose) const
{
	if (m_enabled && m_clip)
	{
		m_clip->evaluate(pose, m_evalTime, m_weight, getUniformTimePosition(), m_mask);
	}
}

void akAnimationPlayer::evaluate(akTransformState *pose) const
{
	if (m_enabled && m_clip)
	{
		m_clip->evaluate(pose, m_evalTime, m_weight, getUniformTimePosition());
	}
}

