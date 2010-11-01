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
#include "gsAI.h"



class gsTrigger : public gkFSM::ITrigger
{
private:
	gkLuaEvent* m_event;

	void execute(int fromState, int toState);

public:

	gsTrigger(gsSelf lself, gsFunction lexecute);
	virtual ~gsTrigger();
};



class gsEvent : public gkFSM::IEvent
{
private:
	friend class gsFSM;
	gkLuaEvent* m_event;

	// call
	bool evaluate();

public:

	// setup for the lua callback
	gsEvent(gsSelf lself, gsFunction lexecute);
	virtual ~gsEvent();
};




class gsUpdateEvent
{
private:
	friend class gsFSM;
	gkLuaEvent* m_event;

	virtual ~gsUpdateEvent();

	// call
	void update();

public:

	// setup for the lua callback
	gsUpdateEvent(gsSelf lself, gsFunction lexecute);

};




void gsTrigger::execute(int fromState, int toState)
{
	if (m_event)
	{
		m_event->beginCall();
		m_event->addArgument(fromState);
		m_event->addArgument(toState);
		m_event->call();
	}
}



gsTrigger::~gsTrigger()
{
	delete m_event;
}




gsTrigger::gsTrigger(gsSelf lself, gsFunction lexecute)
	:   m_event(new gkLuaEvent(lself, lexecute))
{
}







bool gsEvent::evaluate()
{
	bool result = false;

	if (m_event)
	{
		m_event->beginCall();
		m_event->call(result);
	}

	return result;
}




gsEvent::~gsEvent()
{
	delete m_event;
}




gsEvent::gsEvent(gsSelf lself, gsFunction lexecute)
	:   m_event(new gkLuaEvent(lself, lexecute))
{
}




void gsUpdateEvent::update()
{

	if (m_event)
	{
		m_event->beginCall();
		m_event->call();
	}
}



gsUpdateEvent::~gsUpdateEvent()
{
	delete m_event;
}




gsUpdateEvent::gsUpdateEvent(gsSelf lself, gsFunction lexecute)
	:   m_event(new gkLuaEvent(lself, lexecute))
{
}




gsWhenEvent::gsWhenEvent(gkFSM::Event* evt) : m_event(evt)
{
}



gsWhenEvent::~gsWhenEvent()
{
}



void gsWhenEvent::when(gsSelf self, gsFunction trigger)
{
	if (m_event)
		m_event->when(new gsEvent(self, trigger));
}



gsFSM::gsFSM() 
	:	m_fsm(new gkFSM()), m_curState(0)
{
}



gsFSM::~gsFSM()
{
	delete m_fsm;

	utHashTableIterator<EVENT> iter(m_events);
	while (iter.hasMoreElements())
		delete iter.getNext().second;
}



void gsFSM::update()
{
	if (m_fsm)
		m_fsm->update();

	UTsize pos = m_events.find(m_fsm->getState());
	if (pos != UT_NPOS)
		m_events.at(pos)->update();
}



void gsFSM::setState(int state)
{
	if (m_fsm)
		m_fsm->setState(state);
}



int gsFSM::getState()
{
	if (m_fsm)
		return m_fsm->getState();
	return -1;
}


void gsFSM::addStartTrigger(int state, gsSelf self, gsFunction trigger)
{
	if (m_fsm)
		m_fsm->addStartTrigger(state, new gsTrigger(self, trigger));
}



void gsFSM::addEndTrigger(int state, gsSelf self, gsFunction trigger)
{
	if (m_fsm)
		m_fsm->addEndTrigger(state, new gsTrigger(self, trigger));
}



void gsFSM::addEvent(int state, gsSelf self, gsFunction update)
{
	UTsize pos = m_events.find(state);
	if (pos == UT_NPOS)
		m_events.insert(state, new gsUpdateEvent(self, update));
}



gsWhenEvent* gsFSM::addTransition(int from, int to)
{
	if (m_fsm)
		return (new gsWhenEvent(m_fsm->addTransition(from, to)));
	return 0;
}


gsWhenEvent* gsFSM::addTransition(int from, int to, unsigned long ms)
{
	if (m_fsm)
		return (new gsWhenEvent(m_fsm->addTransition(from, to, ms)));
	return 0;
}




gsWhenEvent* gsFSM::addTransition(int from, int to, unsigned long ms, gsSelf self, gsFunction trigger)
{
	if (m_fsm)
		return (new gsWhenEvent(m_fsm->addTransition(from, to, ms, new gsTrigger(self, trigger))));
	return 0;
}
