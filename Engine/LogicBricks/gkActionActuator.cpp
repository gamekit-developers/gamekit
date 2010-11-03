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
#include "gkActionActuator.h"
#include "gkAction.h"
#include "gkActionManager.h"
#include "gkEngine.h"
#include "gkLogger.h"
#include "gkUserDefs.h"
#include "gkGameObject.h"
#include "gkSkeleton.h"
#include "gkEntity.h"




gkActionActuator::gkActionActuator(gkGameObject* object, gkLogicLink* link, const gkString& name)
	:   gkLogicActuator(object, link, name),
	    m_start(1),
	    m_end(1),
	    m_blend(0),
	    m_curTick(0.f),
	    m_mode(0),
	    m_prio(0),
	    m_startAct(""),
	    m_startProp(""),
	    m_reset(true),
	    m_isInit(false),
	    m_action(0)
{

}



gkActionActuator::~gkActionActuator()
{
}



gkLogicBrick* gkActionActuator::clone(gkLogicLink* link, gkGameObject* dest)
{
	gkActionActuator* act = new gkActionActuator(*this);
	act->cloneImpl(link, dest);
	act->m_isInit = false;
	act->m_action = 0;
	return act;
}



void gkActionActuator::doInit(void)
{
	m_action = m_object->getActionPlayer(m_startAct);
	
	if(!m_action)
	{
		gkAction* res = static_cast<gkAction*>(gkActionManager::getSingleton().getByName(m_startAct));
		if(res)
			m_action = m_object->addAction(res);
	}
	
	if (m_action)
	{
		m_action->setTimePosition(m_start);
	}
	
	m_curTick = m_start;
	
}



void gkActionActuator::notifyActiveStatus(void)
{
	if (!m_isActive)
	{
		if (m_reset)
			m_curTick = m_start;
			if (m_action != 0)
				m_action->setTimePosition(m_start);
	}
}


void gkActionActuator::execute(void)
{
	if (m_action != 0)
		m_curTick = m_action->getTimePosition();

	if (isPulseOff())
	{
		if (m_reset)
		{
			m_curTick = m_start;
			if (m_action != 0)
				m_action->setTimePosition(m_start);
		}

		if (m_curTick < m_end && (m_mode == AA_PLAY || m_mode == AA_LOOP_END))
			setPulse(BM_ON);
		else
			return;
	}

	if (!m_isInit)
	{
		m_isInit = true;
		doInit();
	}

	if (m_action != 0)
	{
		switch (m_mode)
		{
		case AA_LOOP_STOP:
		case AA_LOOP_END:
			// loop anim
			if (m_curTick >= m_end)
				m_action->setTimePosition(m_start);
		case AA_PLAY:
		default:
			m_object->playAction(m_action, m_blend);
			break;
		}
		
		
	}
}
