/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Nestor Silveira.

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
#include "gkFSM.h"
#include "gkLogger.h"

gkFSM::gkFSM()
	: m_started(false),
	  m_currentState(-1)
{
}

gkFSM::~gkFSM()
{
	size_t n = m_events.size();

	for (size_t i = 0; i < n; i++)
	{
		Event* pEvent = m_events[i];

		if (pEvent->m_e)
			delete pEvent->m_e;

		delete pEvent;
	}
}

void gkFSM::update()
{
	size_t pos = m_transitions.find(m_currentState);

	if (pos != GK_NPOS)
	{
		if (!m_started)
		{
			m_started = true;

			execute_start_trigger(-1, m_currentState);
		}

		const Data* pData = 0;

		REACTION_ITERATOR it(m_transitions.at(pos));

		while (it.hasMoreElements())
		{
			const void* p = it.peekNextKey().key();

			const Event* pEvent = static_cast<const Event*>(p);

			if (!pEvent->m_e || pEvent->m_e->evaluate())
			{
				const Data* pTmpData  = &(it.peekNextValue());

				if (pEvent->m_e)
				{
					pData = pTmpData;

					if (pTmpData->m_state != m_currentState)
					{
						break;
					}
				}
				else if (!pData)
				{
					pData = pTmpData;
				}
			}

			it.next();
		}

		if (pData && pData->m_state != m_currentState && pData->m_ms < m_timer.getTimeMilliseconds())
		{
			if (pData->m_trigger.get())
				pData->m_trigger->execute(m_currentState, pData->m_state);

			execute_end_trigger(m_currentState, pData->m_state);

			setState(pData->m_state);

			execute_start_trigger(m_currentState, pData->m_state);
		}
	}
}

void gkFSM::addStartTrigger(int state, ITrigger* trigger)
{
	size_t pos = m_startTriggers.find(state);

	GK_ASSERT(pos == GK_NPOS);

	m_startTriggers.insert(state, gkPtrRef<ITrigger>(trigger));
}

void gkFSM::addEndTrigger(int state, ITrigger* trigger)
{
	size_t pos = m_endTriggers.find(state);

	GK_ASSERT(pos == GK_NPOS);

	m_endTriggers.insert(state, gkPtrRef<ITrigger>(trigger));
}

void gkFSM::execute_start_trigger(int from, int to)
{
	size_t pos = m_startTriggers.find(to);

	if (pos != GK_NPOS)
		m_startTriggers.at(pos)->execute(from, to);
}

void gkFSM::execute_end_trigger(int from, int to)
{
	size_t pos = m_endTriggers.find(from);

	if (pos != GK_NPOS)
		m_endTriggers.at(pos)->execute(from, to);
}

gkFSM::Event* gkFSM::addTransition(int from, int to, unsigned long ms, ITrigger* trigger)
{
	Event* pEvent = new Event;

	Data data(ms, to, trigger);

	size_t pos = m_transitions.find(from);

	if (pos != GK_NPOS)
	{
		REACTION& reaction = m_transitions.at(pos);

		reaction.insert(pEvent, data);
	}
	else
	{
		REACTION reaction;
		reaction.insert(pEvent, data);
		m_transitions.insert(from, reaction);
	}

	m_events.push_back(pEvent);

	return pEvent;
}


void gkFSM::setState(int state)
{
	m_currentState = state;

	m_timer.reset();

	notifyState(state);
}
