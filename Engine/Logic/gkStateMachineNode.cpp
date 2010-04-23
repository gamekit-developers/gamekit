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
#include "gkStateMachineNode.h"
#include "gkLogger.h"

gkStateMachineNode::gkStateMachineNode(gkLogicTree *parent, size_t id) 
: gkLogicNode(parent, id),
m_currentState(-1)
{
	ADD_ISOCK(UPDATE, true);
	ADD_ISOCK(FORCE_STATUS, -1);
	ADD_OSOCK(CURRENT_STATE, 0);
}

gkStateMachineNode::~gkStateMachineNode()
{
	size_t n = m_events.size();

	for(size_t i=0; i<n; i++)
	{
		gkILogicSocket* pSocket = m_events[i];

		delete pSocket;
	}
}

bool gkStateMachineNode::evaluate(gkScalar tick)
{
	return GET_SOCKET_VALUE(UPDATE);
}

void gkStateMachineNode::update(gkScalar tick)
{
	if(m_currentState != GET_SOCKET_VALUE(CURRENT_STATE))
	{
		m_timer.reset();
		
		m_currentState = GET_SOCKET_VALUE(CURRENT_STATE);
	}

	int forceStatus = GET_SOCKET_VALUE(FORCE_STATUS);

	if(forceStatus != -1)
	{
		if(m_currentState != forceStatus)
		{
			m_currentState = forceStatus;

			SET_SOCKET_VALUE(CURRENT_STATE, m_currentState);
			SET_SOCKET_VALUE(FORCE_STATUS, -1);
		}
	}
	else
	{
		size_t pos = m_transitions.find(m_currentState);

		if (pos != GK_NPOS)
		{
			const Data* pData = 0;

			REACTION_ITERATOR it(m_transitions.at(pos));

			while (it.hasMoreElements()) 
			{
				const void* p = it.peekNextKey().key();

				const gkILogicSocket* pSocket = static_cast<const gkILogicSocket*>(p);

				if(!pSocket->isConnected() || static_cast<const gkLogicSocket<bool>*>(pSocket)->getValue())
				{
					const Data* pTmpData  = &(it.peekNextValue());

					if(pSocket->isConnected())
					{
						pData = pTmpData;

						if(pTmpData->m_state != m_currentState) 
						{
							break;
						}
					}
					else if(!pData)
					{
						pData = pTmpData;
					}
				}

				it.next();
			}

			if(pData && pData->m_state != m_currentState && pData->m_ms < m_timer.getTimeMilliseconds())
			{
				m_currentState = pData->m_state;

				SET_SOCKET_VALUE(CURRENT_STATE, m_currentState);

				m_timer.reset();
			}
		}
	}
}

gkLogicSocket<bool>* gkStateMachineNode::addTransition(int from, int to, unsigned long ms)
{
	gkLogicSocket<bool>* pSocket = new gkLogicSocket<bool>(this, true, false);

	Data data(ms, to);

	size_t pos = m_transitions.find(from);
	
	if (pos != GK_NPOS)
	{
		REACTION& reaction = m_transitions.at(pos);

		reaction.insert(pSocket, data);
	}
	else
	{
		REACTION reaction;
		reaction.insert(pSocket, data);
		m_transitions.insert(from, reaction);
	}

	m_events.push_back(pSocket);

	return pSocket;
}

