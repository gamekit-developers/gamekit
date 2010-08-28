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
#include "gkActionManager.h"
#include "gkAction.h"
#include "gkActionChannel.h"


// ----------------------------------------------------------------------------
gkActionManager::gkActionManager() 
	:	m_active(0), m_blend(0), m_blendTime(0.0)
{
}


// ----------------------------------------------------------------------------
gkActionManager::~gkActionManager()
{
}


// ----------------------------------------------------------------------------
void gkActionManager::setAction(gkAction *act)
{
	if (act)
	{
		if (!m_active)
			m_active = act;
		else
		{
			m_blend = act;
			m_blend->setWeight(0.0);
			m_blend->setTimePosition(0.0);
		}
	}
}


// ----------------------------------------------------------------------------
void gkActionManager::update(gkScalar delta, gkScalar blendDelta)
{
	if (m_blend && m_active)
	{

		gkScalar blendFrames = m_blend->getBlendFrames();
		if (gkFuzzy(blendFrames))
			blendFrames = 1;

		blendFrames = 1.f / blendFrames;

		m_blendTime += blendFrames;

		if (m_blendTime < 1.0)
		{
			m_blend->setWeight(m_blendTime);
			m_active->setWeight(1.f - m_blendTime);
			m_blend->evaluate(blendDelta);
		}
		else
		{
			m_blendTime = 0.f;
			m_active->setWeight(0.f);
			m_active->setTimePosition(0);

			m_blend->setWeight(1.f);
			m_active = m_blend;
			m_blend = 0;
		}
	}

	if (m_active)
		m_active->evaluate(delta);
}


// ----------------------------------------------------------------------------
void gkActionManager::update(gkScalar delta)
{
	update(delta, delta);
}
