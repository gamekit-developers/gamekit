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
#ifndef _gkStateMachineNode_h_
#define _gkStateMachineNode_h_

#include "gkLogicNode.h"
#include "gkIfNode.h"
#include "LinearMath/btQuickprof.h"

class gkStateMachineNode : public gkLogicNode
{
public:

	enum
	{
		UPDATE,
		CURRENT_STATE
	};

	DECLARE_SOCKET_TYPE(UPDATE, bool);
	DECLARE_SOCKET_TYPE(CURRENT_STATE, int);

    gkStateMachineNode(gkLogicTree *parent, size_t id);

	~gkStateMachineNode();

	bool evaluate(gkScalar tick);
	void update(gkScalar tick);

	gkLogicSocket<bool>* addTransition(int from, int to, unsigned long ms = 0);

	gkIfNode<int, CMP_EQUALS>* isCurrentStatus(int status);

private:

	typedef utPointerHashKey EVENT;
	typedef utIntHashKey STATE;

	struct Data
	{
		unsigned long m_ms;
		int m_state;

		Data() : m_ms(0), m_state(0) {}

		Data(unsigned long ms, int state) 
			: m_ms(ms), m_state(state)
		{
		}
	};

	typedef utHashTable<EVENT, Data > REACTION;
	typedef utHashTableIterator<REACTION> REACTION_ITERATOR;

	typedef utHashTable<STATE, REACTION> TRANSITIONS;

	TRANSITIONS m_transitions;

	btClock m_timer;

	int m_currentState;

	typedef utArray<gkILogicSocket*> EVENTS;

	EVENTS m_events;

	typedef std::map<int, gkIfNode<int, CMP_EQUALS>* > MAP;
	MAP m_statuses;

};


#endif//_gkStateMachineNode_h_
