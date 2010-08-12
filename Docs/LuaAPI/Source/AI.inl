/*!
<!-- ============================================ WhenEvent ============================================ -->
\LuaClass{WhenEvent}
\LuaSectionUp{AI}



\beginmenu{Methods}
\LuaMethodMenu{WhenEvent,when}
\endmenu

\endpage


<!-- ======================================== -->
\LuaMethod{WhenEvent,when}
\LuaClassUp{WhenEvent}


Defines and event to poll for before transition to a new state

\code
function WhenEvent:when(table, function)
\endcode


\param table    Pointer to a Lua table object, ie; self 
\param function Pointer to a Lua function, method of self. 



\LuaSee{FSM,addTransition}

\endpage



<!-- ============================================ FSM ============================================ -->

\LuaClass{FSM}
\LuaSectionUp{AI}

\beginmenu{Methods}
\LuaMethodMenu{FSM,constructor}
\LuaMethodMenu{FSM,update}
\LuaMethodMenu{FSM,setState}
\LuaMethodMenu{FSM,getState}
\LuaMethodMenu{FSM,addStartTrigger}
\LuaMethodMenu{FSM,addEndTrigger}
\LuaMethodMenu{FSM,addEvent}
\LuaMethodMenu{FSM,addTransition}
\endmenu
\endpage




<!-- ======================================== -->
\LuaMethod{FSM,constructor}
\LuaClassUp{FSM}

Creates a new finite state machine.

\code
function FSM:constructor()
\endcode

\endpage



<!-- ======================================== -->
\LuaMethod{FSM,update}
\LuaClassUp{FSM}
Updates the current state. 
\code
function FSM:update()
\endcode
\endpage


<!-- ======================================== -->
\LuaMethod{FSM,setState}
\LuaClassUp{FSM}
Apply a specific state.
\code
function FSM:setState(state)
\endcode

\param state The new state.
\endpage


<!-- ======================================== -->
\LuaMethod{FSM,getState}
\LuaClassUp{FSM}
Return the current state.
\code
function FSM:getState()
\endcode

\return number The current state.
\endpage


<!-- ======================================== -->
\LuaMethod{FSM,addStartTrigger}
\LuaClassUp{FSM}
Adds a callback to be executed at the beginning of a state transition.
\code
function FSM:addStartTrigger(state, table, function)
\endcode


\param state    Unique id representing the desired state.
\param table    Pointer to a Lua table object, ie; self 
\param function Pointer to a Lua function, method of self. 


\endpage

<!-- ======================================== -->
\LuaMethod{FSM,addEndTrigger}
\LuaClassUp{FSM}
Adds a callback to be executed at the end of a state transition.
\code
function FSM:addEndTrigger(state, table, function)
\endcode


\param state    Unique id representing the desired state.
\param table    Pointer to a Lua table object, ie; self 
\param function Pointer to a Lua function, method of self. 


\endpage

<!-- ======================================== -->
\LuaMethod{FSM,addEndTrigger}
\LuaClassUp{FSM}
Adds a callback to be executed at the end of a state transition.
\code
function FSM:addEndTrigger(state, table, function)
\endcode


\param state    Unique id representing the desired state.
\param table    Pointer to a Lua table object, ie; self 
\param function Pointer to a Lua function, method of self. 


\endpage

<!-- ======================================== -->
\LuaMethod{FSM,addEvent}
\LuaClassUp{FSM}
Adds a callback to be executed constantly while in the current state.
\code
function FSM:addEvent(state, table, function)
\endcode


\param state    Unique id representing the desired state.
\param table    Pointer to a Lua table object, ie; self 
\param function Pointer to a Lua function, method of self. 


\endpage


<!-- ======================================== -->
\LuaMethod{FSM,addTransition}
\LuaClassUp{FSM}
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


\endpage

*/