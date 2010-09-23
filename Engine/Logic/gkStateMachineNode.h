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
#include "gkFSM.h"
#include "LinearMath/btQuickprof.h"

class gkStateMachineNode : public gkLogicNode
{
	typedef gkFSM::ITrigger ITrigger;

public:

	enum
	{
		//Inputs
		UPDATE,

		//Outputs
		CURRENT_STATE,

		//////////
		MAX_SOCKETS
	};

	DECLARE_SOCKET_TYPE(UPDATE, bool);
	DECLARE_SOCKET_TYPE(CURRENT_STATE, int);

	gkStateMachineNode(gkLogicTree* parent, size_t id);

	~gkStateMachineNode();

	bool evaluate(gkScalar tick);
	void update(gkScalar tick);

	void addStartTrigger(int state, ITrigger* trigger);

	void addEndTrigger(int state, ITrigger* trigger);

	gkLogicSocket<bool>* addTransition(int from, int to, unsigned long ms = 0, ITrigger* trigger = 0);

	gkIfNode<int, CMP_EQUALS>* isCurrentStatus(int status);

protected:

	virtual void notifyState(int state) {};
	void setState(int state);

private:

	typedef utIntHashKey STATE;
	typedef utHashTable<STATE, gkPtrRef<ITrigger> > TRIGGERS;

	void execute_start_trigger(int from, int to);
	void execute_end_trigger(int from, int to);


private:

	bool m_started;
	int m_currentState;

	typedef utPointerHashKey EVENT;

	struct Data
	{
		unsigned long m_ms;
		int m_state;
		gkPtrRef<ITrigger> m_trigger;

		Data() : m_ms(0), m_state(0), m_trigger(0) {}

		Data(unsigned long ms, int state, ITrigger* trigger)
			: m_ms(ms), m_state(state), m_trigger(trigger)
		{
		}
	};

	typedef utHashTable<EVENT, Data > REACTION;
	typedef utHashTableIterator<REACTION> REACTION_ITERATOR;

	typedef utHashTable<STATE, REACTION> TRANSITIONS;

	TRANSITIONS m_transitions;

	btClock m_timer;

	typedef utArray<gkILogicSocket*> EVENTS;

	EVENTS m_events;

	typedef std::map<int, gkIfNode<int, CMP_EQUALS>* > MAP;
	MAP m_statuses;

	TRIGGERS m_startTriggers;

	TRIGGERS m_endTriggers;
};


#endif//_gkStateMachineNode_h_
