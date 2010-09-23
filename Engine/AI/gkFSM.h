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
#ifndef _gkFSM_h_
#define _gkFSM_h_

#include "gkCommon.h"
#include "gkMathUtils.h"
#include "Thread/gkPtrRef.h"
#include "LinearMath/btQuickprof.h"

class gkFSM
{
public:

	gkFSM();
	~gkFSM();

	void update();

	class IEvent
	{
	public:
		virtual bool evaluate() = 0;
		virtual ~IEvent() {}
	};

	template < typename OBJ, bool has_parameter = false, typename PARAM = int >
	class LogicEvent : public IEvent
	{
	public:
		typedef bool (OBJ::*FUNCTION_0)();
		typedef bool (OBJ::*FUNCTION_1)(PARAM);

		LogicEvent(OBJ* obj, FUNCTION_0 func) : m_obj(obj), m_func0(func) { GK_ASSERT(m_obj && m_func0); }
		LogicEvent(OBJ* obj, FUNCTION_1 func, PARAM param) : m_obj(obj), m_func1(func), m_param(param) { GK_ASSERT(m_obj && m_func1); }
		~LogicEvent() {}
		bool evaluate() { return evaluate(Int2Type<has_parameter>()); }
		bool evaluate(Int2Type<false>) { return (m_obj->*m_func0)(); }
		bool evaluate(Int2Type<true>) { return (m_obj->*m_func1)(m_param); }

	private:

		OBJ* m_obj;
		FUNCTION_0 m_func0;
		FUNCTION_1 m_func1;
		PARAM m_param;
	};

	class Event
	{
	public:
		Event() : m_e(0) {};
		~Event() {};
		void when(IEvent* e) { m_e = e;}

	private:
		IEvent* m_e;

		friend class gkFSM;
	};

	class ITrigger : public gkReferences
	{
	public:
		virtual void execute(int fromState, int toState) = 0;
		virtual ~ITrigger() {}
	};

	template < typename OBJ, bool has_parameter = false, typename PARAM = int >
	class LogicTrigger : public ITrigger
	{
	public:
		typedef void (OBJ::*FUNCTION_0)(int, int);
		typedef void (OBJ::*FUNCTION_1)(PARAM, int, int);

		LogicTrigger(OBJ* obj, FUNCTION_0 func) : m_obj(obj), m_func0(func) { GK_ASSERT(m_obj && m_func0); }
		LogicTrigger(OBJ* obj, FUNCTION_1 func, PARAM param) : m_obj(obj), m_func1(func), m_param(param) { GK_ASSERT(m_obj && m_func1); }
		~LogicTrigger() {}
		void execute(int fromState, int toState) { execute(Int2Type<has_parameter>(), fromState, toState); }
		void execute(Int2Type<false>, int fromState, int toState) { (m_obj->*m_func0)(fromState, toState); }
		void execute(Int2Type<true>, int fromState, int toState) { (m_obj->*m_func1)(m_param, fromState, toState); }

	private:

		OBJ* m_obj;
		FUNCTION_0 m_func0;
		FUNCTION_1 m_func1;
		PARAM m_param;
	};

	void addStartTrigger(int state, ITrigger* trigger);

	void addEndTrigger(int state, ITrigger* trigger);

	Event* addTransition(int from, int to, unsigned long ms = 0, ITrigger* trigger = 0);

	int getState() const { return m_currentState; }

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

	typedef utArray<Event*> EVENTS;

	EVENTS m_events;

	TRIGGERS m_startTriggers;

	TRIGGERS m_endTriggers;
};


#endif//_gkFSM_h_
