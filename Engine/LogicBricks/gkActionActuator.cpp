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
#include "gkEngine.h"
#include "gkLogger.h"
#include "gkUserDefs.h"
#include "gkGameObject.h"
#include "gkSkeleton.h"
#include "gkEntity.h"
#include "gkAnimationManager.h"
#include "gkLogicManager.h"
#include "gkLogicLink.h"

//#define DEBUG_PRINT printf
#define DEBUG_PRINT(...)

gkActionActuator::gkActionActuator(gkGameObject* object, gkLogicLink* link, const gkString& name)
	:   gkLogicActuator(object, link, name),
	    m_start(1),
	    m_end(1),
	    m_blend(0),
	    m_mode(0),
	    m_prio(0),
	    m_startAct(""),
	    m_startProp(""),
	    m_reset(true),
	    m_isInit(false),
	    m_action(0),
		m_startTime(0),
		m_isPlaying(false),
		m_ignorePulseOn(false),
		m_state(0)
{	
}



gkActionActuator::~gkActionActuator()
{
	if (m_isPlaying)
		gkLogicManager::getSingleton().removeUpdate(this);
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
	m_action = m_object->getAnimationPlayer(m_startAct);
	
	if (!m_action)
	{
		gkAnimation* res = gkAnimationManager::getSingleton().getAnimation(gkResourceName(m_startAct, getObjectGroupName()));
		if(res)
			m_action = m_object->addAnimation(res, m_startAct);
	}
	
	if (m_action)
	{
		if (m_start > m_end)
		{
			m_start = 0;
			m_end = m_action->getLength();
		}

		//if (m_mode == AA_PLAY)
			m_action->setMode(AK_ACT_END);
		//else
		//	m_action->setMode(AK_ACT_LOOP);
	}
	
	resetAction();
}



void gkActionActuator::resetTimer(void)
{	
	m_startTime = gkEngine::getSingleton().getCurTime();
}

gkScalar gkActionActuator::getElapsedTime(void)
{
	return (gkEngine::getSingleton().getCurTime() - m_startTime) / 1000.f;
}

void gkActionActuator::resetAction(bool exceptTimer)
{
	if (!exceptTimer)
		resetTimer();
	
	if (m_action)
		m_action->setTimePosition(m_start);	

	DEBUG_PRINT("reset: %s\n", m_startAct.c_str());
}

void gkActionActuator::playAction(void)
{
	GK_ASSERT(m_action);

	DEBUG_PRINT("play: %s %d %d\n", m_startAct.c_str(), m_link->getState(), m_ignorePulseOn);

	if (m_mode != AA_LOOP_STOP)
		resetAction();

	m_isPlaying = true;
	m_object->playAnimation(m_action, m_blend);
	m_state = m_link->getState();

	if (isPulseOn() && m_mode != AA_LOOP_STOP) //ignore on until next off
		m_ignorePulseOn = true;

	gkLogicManager::getSingleton().requestUpdate(this);
}

void gkActionActuator::stopAction(void)
{
	m_isPlaying = false;
	m_object->stopAnimation(m_action);	

	DEBUG_PRINT("stop: %s %d\n", m_startAct.c_str(), m_ignorePulseOn);

	gkLogicManager::getSingleton().removeUpdate(this);
}

bool gkActionActuator::isActionEnd(void)
{
	return m_action ? m_action->getTimePosition() >= m_action->getLength() : false;
}


void gkActionActuator::notifyActiveStatus(void)
{
	if (!m_isActive)
	{
		DEBUG_PRINT("deactive : %s\n", m_startAct.c_str());	
		m_ignorePulseOn = false;
	}
}

void gkActionActuator::stateChanged(void)
{
	DEBUG_PRINT("state %s changed %d -> %d\n", m_startAct.c_str(), m_state, m_link->getState());
	stopAction();
		
	if (m_reset)
		resetAction();

	m_ignorePulseOn = false;	
}


void gkActionActuator::update(void)
{	
	if (!m_isPlaying) return;

	if (m_state != m_link->getState())
	{
		stateChanged();
		return;
	}

	gkScalar t = getElapsedTime() + m_start;
	bool end = t >= m_end;
	bool off = isPulseOff();

	if (off)
		m_ignorePulseOn = false;

	if (m_mode == AA_LOOP_END)
	{
		if (end)
		{
			stopAction();
			if (!off)
				playAction();
		}
	}
	else if (m_mode == AA_LOOP_STOP)
	{
		if (off)
			stopAction();
		else if (end)
			resetAction(false);
	}
	else //if (m_mode == AA_PLAY)
	{
		if (end)
			stopAction();
	}
}

void gkActionActuator::execute(void)
{
	if (!m_isInit)
	{
		m_isInit = true;
		doInit();
	}

	if (!m_action) 
		return;

	if (isPulseOff())
	{
		m_ignorePulseOn = false;
	}
	else
	{		
		if (m_mode == AA_LOOP_STOP)
		{
			if (!m_isPlaying)
				playAction();
		}
		else //if (m_mode == AA_PLAY || m_mode == AA_LOOP_END)
		{
			if (!m_ignorePulseOn)
			{
				if (!m_isPlaying || m_reset)
					playAction();
			}		
		}
	}


	if (m_isPlaying)
		m_object->playAnimation(m_action, m_blend);

}


