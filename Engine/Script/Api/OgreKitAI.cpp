/*
-------------------------------------------------------------------------------
--                                                                           --
--  This file is part of GameKit.                                            --
--  http://gamekit.googlecode.com/                                           --
--                                                                           --
--                                                                           --
--  Released under the zlib license.                                         --
--  http://www.gzip.org/zlib/zlib_license.html                               --
--                                                                           --
--                                                                           --
-------------------------------------------------------------------------------
-- OgreKit AI Bindings
*/
#include "OgreKitAI.h"

namespace OgreKit {


// FSM_Trigger base
class FSM_Trigger : public gkFSM::ITrigger
{
private:
    gkLuaEvent *m_event;

    void execute(int fromState, int toState);

public:

    FSM_Trigger(Self lself, Function lexecute);
    virtual ~FSM_Trigger();
};

 

// FSM_Event base
class FSM_Event : public gkFSM::IEvent
{
private:
    friend class FSM;
    gkLuaEvent *m_event;

    // call
    bool evaluate();

public:

    // setup for the lua callback
    FSM_Event(Self lself, Function lexecute);
    virtual ~FSM_Event();
};


// FSM_UpdateEvent base
class FSM_UpdateEvent
{
private:
    friend class FSM;
    gkLuaEvent *m_event;


    // internal __gc lua is not the owner
    virtual ~FSM_UpdateEvent();

    // call
    void update();


public:

    // setup for the lua callback
    FSM_UpdateEvent(Self lself, Function lexecute);

};


///////////////////////////////////////////////////////////////////////////////
// 
// OgreKit.FSM_Trigger
// 
///////////////////////////////////////////////////////////////////////////////

void FSM_Trigger::execute(int fromState, int toState)
{
    if (m_event)
    {
        m_event->beginCall();
        m_event->addArgument(fromState);
        m_event->addArgument(toState);
        m_event->call();
    }
}


FSM_Trigger::~FSM_Trigger()
{
    delete m_event;
}



FSM_Trigger::FSM_Trigger(Self lself, Function lexecute) 
    :   m_event(new gkLuaEvent(lself, lexecute))
{
}


///////////////////////////////////////////////////////////////////////////////
// 
// OgreKit.FSM_Event
// 
///////////////////////////////////////////////////////////////////////////////


bool FSM_Event::evaluate()
{
    bool result = false;
    
    if (m_event)
    {
        m_event->beginCall();
        m_event->call(result);
    }

    return result;
}


FSM_Event::~FSM_Event()
{
    delete m_event;
}



FSM_Event::FSM_Event(Self lself, Function lexecute) 
    :   m_event(new gkLuaEvent(lself, lexecute))
{
}




///////////////////////////////////////////////////////////////////////////////
// 
// OgreKit.FSM_UpdateEvent
// 
///////////////////////////////////////////////////////////////////////////////


void FSM_UpdateEvent::update()
{
    
    if (m_event)
    {
        m_event->beginCall();
        m_event->call();
    }
}


FSM_UpdateEvent::~FSM_UpdateEvent()
{
    delete m_event;
}



FSM_UpdateEvent::FSM_UpdateEvent(Self lself, Function lexecute) 
    :   m_event(new gkLuaEvent(lself, lexecute))
{
}


///////////////////////////////////////////////////////////////////////////////
// 
// OgreKit.WhenEvent
// 
///////////////////////////////////////////////////////////////////////////////
WhenEvent::WhenEvent(gkFSM::Event *evt) : m_event(evt)
{
}

WhenEvent::~WhenEvent()
{
}

void WhenEvent::when(Self self, Function trigger)
{
    if (m_event)
        m_event->when(new FSM_Event(self, trigger));
}



///////////////////////////////////////////////////////////////////////////////
// 
// OgreKit.FSM
// 
///////////////////////////////////////////////////////////////////////////////
FSM::FSM()
{
    m_fsm = new gkFSM();
}

FSM::~FSM()
{
    delete m_fsm;

    utHashTableIterator<EVENT> iter(m_events);
    while (iter.hasMoreElements())
        delete iter.getNext().second;
}

void FSM::update()
{
    if (m_fsm)
        m_fsm->update();

    UTsize pos = m_events.find(m_fsm->getState());
    if (pos != UT_NPOS)
        m_events.at(pos)->update();
}


void FSM::setState(int state)
{
    if (m_fsm)
        m_fsm->setState(state);
}

int FSM::getState()
{
    if (m_fsm)
        return m_fsm->getState();
    return -1;
}

void FSM::addStartTrigger(int state, Self self, Function trigger)
{
    if (m_fsm)
        m_fsm->addStartTrigger(state, new FSM_Trigger(self, trigger));
}

void FSM::addEndTrigger(int state, Self self, Function trigger)
{
    if (m_fsm)
        m_fsm->addEndTrigger(state, new FSM_Trigger(self, trigger));
}


void FSM::addEvent(int state, Self self, Function update)
{
    UTsize pos = m_events.find(state);
    if (pos == UT_NPOS)
        m_events.insert(state, new FSM_UpdateEvent(self, update));
}

Pointer<WhenEvent> FSM::addTransition(int from, int to)
{
    if (m_fsm) 
        return Pointer<WhenEvent>(new WhenEvent(m_fsm->addTransition(from, to)));
    return Pointer<WhenEvent>();
}

Pointer<WhenEvent> FSM::addTransition(int from, int to, unsigned long ms)
{
    if (m_fsm) 
        return Pointer<WhenEvent>(new WhenEvent(m_fsm->addTransition(from, to, ms)));
    return Pointer<WhenEvent>();
}

Pointer<WhenEvent> FSM::addTransition(int from, int to, unsigned long ms, Self self, Function trigger)
{
    if (m_fsm) 
        return Pointer<WhenEvent>(new WhenEvent(m_fsm->addTransition(from, to, ms, new FSM_Trigger(self, trigger))));
    return Pointer<WhenEvent>();
}

}



