/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Charlie C.

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
#ifndef _gsAI_h_
#define _gsAI_h_

#include "gsCommon.h"
#include "gsMath.h"
#include "gsUtils.h"
#include "Script/Lua/gkLuaUtils.h"



class gsWhenEvent
{
public:
	~gsWhenEvent();
	/**
		\LuaMethod{WhenEvent,when}

		Defines and event to poll for before transition to a new state

		\code
		function WhenEvent:when(table, function)
		\endcode

		\param table    Pointer to a Lua table object, ie; self 
		\param function Pointer to a Lua function, method of self. 

		\LuaSee{FSM,addTransition}
	*/
	void when(gsSelf self, gsFunction when);

	OGRE_KIT_WRAP_CLASS_COPY_CTOR(gsWhenEvent, gkFSM::Event, m_event);
};



class gsFSM
{
private:
	typedef utHashTable<utIntHashKey, class gsUpdateEvent*> EVENT;

	gkFSM*   m_fsm;
	int      m_curState;
	EVENT    m_events;

public:

	/**
		\LuaMethod{FSM,constructor}

		Creates a new finite state machine.

		\code
		function FSM:constructor()
		\endcode
	*/
	gsFSM();
	~gsFSM();

	/**
		\LuaMethod{FSM,update}	
	
		Updates the current state. 

		\code
		function FSM:update()
		\endcode	
	*/
	void  update();

	/**
		\LuaMethod{FSM,setState}
		Apply a specific state.
		\code
		function FSM:setState(state)
		\endcode

		\param state The new state.
		
	*/
	void  setState(int state);
	/**
		\LuaMethod{FSM,getState}

		Return the current state.

		\code
		function FSM:getState()
		\endcode

		\return number The current state.
		
	*/
	int   getState();


	/**
		\LuaMethod{FSM,addStartTrigger}

		Adds a callback to be executed at the beginning of a state transition.

		\code
		function FSM:addStartTrigger(state, table, function)
		\endcode

		\param state    Unique id representing the desired state.
		\param table    Pointer to a Lua table object, ie; self 
		\param function Pointer to a Lua function, method of self. 
	*/
	void  addStartTrigger(int state, gsSelf self, gsFunction trigger);
	/**
		\LuaMethod{FSM,addEndTrigger}

		Adds a callback to be executed at the end of a state transition.

		\code
		function FSM:addEndTrigger(state, table, function)
		\endcode

		\param state    Unique id representing the desired state.
		\param table    Pointer to a Lua table object, ie; self 
		\param function Pointer to a Lua function, method of self. 	
	*/
	void  addEndTrigger(int state, gsSelf self, gsFunction trigger);	

	/**
		\LuaMethod{FSM,addEvent}

		Adds a callback to be executed constantly while in the current state.

		\code
		function FSM:addEvent(state, table, function)
		\endcode

		\param state    Unique id representing the desired state.
		\param table    Pointer to a Lua table object, ie; self 
		\param function Pointer to a Lua function, method of self. 
	*/
	void  addEvent(int state, gsSelf self, gsFunction update);

	/**
		\LuaMethod{FSM,addTransition}

		Adds a state transition. 

		\code
		function FSM:addTransition(from, to, ms=nil, table=nil, trigger=nil)
		\endcode

		\param from     The starting state
		\param to       The destination state. 
		\param ms       Milliseconds to wait upon transition.
		\param table    Pointer to a Lua table object, ie; self 
		\param trigger  Pointer to a Lua function. To be called upon transition.

		\LuaMethodReturn{WhenEvent,For adding a transition test.}

		\note Parameters table and trigger are a pair to be used together.
	*/
	gsWhenEvent* addTransition(int from, int to);
	gsWhenEvent* addTransition(int from, int to, unsigned long ms);
	gsWhenEvent* addTransition(int from, int to, unsigned long ms, gsSelf self, gsFunction trigger);

};

#endif//_gsAI_h_
