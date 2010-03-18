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
#include "gkStateNode.h"
#include "gkLogger.h"

gkStateNode::gkStateNode(gkLogicTree *parent, size_t id) 
: gkLogicNode(parent, id),
m_currentState(-1)
{
	ADD_ISOCK(*getUpdate(), this, gkLogicSocket::ST_BOOL);
	ADD_OSOCK(*getCurrentState(), this, gkLogicSocket::ST_INT);
	ADD_OSOCK(*getCurrentName(), this, gkLogicSocket::ST_STRING);
}

gkStateNode::~gkStateNode()
{
	size_t n = m_events.size();

	for(size_t i=0; i<n; i++)
	{
		gkLogicSocket* pSocket = m_events[i];

		delete pSocket;
	}
}

bool gkStateNode::evaluate(gkScalar tick)
{
	return getUpdate()->getValueBool();
}

void gkStateNode::update(gkScalar tick)
{
	if(m_currentState != getCurrentState()->getValueInt())
	{
		m_timer.reset();
		
		m_currentState = getCurrentState()->getValueInt();
	}

	size_t pos = m_transitions.find(m_currentState);

    if (pos != GK_NPOS)
    {
		const Data* pData = 0;

		REACTION_ITERATOR it(m_transitions.at(pos));
		while (it.hasMoreElements()) 
		{
			const void* p = it.peekNextKey().key();

			const gkLogicSocket* pSocket = static_cast<const gkLogicSocket*>(p);

			if(!pSocket->isConnected() || pSocket->getValueBool())
			{
				pData = &(it.peekNextValue());

				if(pSocket->isConnected()) 
					break;
			}

			it.next();
		}

		if(pData && pData->m_ms < m_timer.getTimeMilliseconds())
		{
			m_currentState = pData->m_state;

			getCurrentState()->setValue(m_currentState);

			size_t pos = m_translation.find(m_currentState);

			GK_ASSERT(pos != GK_NPOS);

			gkString name = m_translation.at(pos);

			getCurrentName()->setValue(name);

			gkLogMessage(name);

			m_timer.reset();
		}
	}
}

gkLogicSocket* gkStateNode::addTransition(int from, int to, unsigned long ms)
{
	gkLogicSocket* pSocket = new gkLogicSocket();

	ADD_ISOCK(*pSocket, this, gkLogicSocket::ST_BOOL);

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

void gkStateNode::addTranslation(int state, const gkString& name)
{
	m_translation.insert(state, name);
}
