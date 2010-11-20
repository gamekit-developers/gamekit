/*!

<!-- ============================================ LuaScript ============================================ -->
\LuaClass{LuaScript}


\beginmenu{Methods}
\LuaMethodMenu{LuaScript,execute}
\LuaMethodMenu{LuaScript,getName}
\endmenu


\endpage


<!-- ======================================== -->
\LuaMethod{LuaScript,execute}
\LuaClassUp{LuaScript}


Compile and run script.


\code
function LuaScript:execute()
\endcode

\returns bool Execution result.

\endpage


<!-- ======================================== -->
\LuaMethod{LuaScript,getName}
\LuaClassUp{LuaScript}


Get script resource name.


\code
function LuaScript:getName()
\endcode

\returns string Resource name.

\endpage


<!-- ============================================ LuaManager ============================================ -->
\LuaClass{LuaManager}


\beginmenu{Methods}
\LuaMethodMenu{LuaManager,create}
\LuaMethodMenu{LuaManager,getScript}
\endmenu


\endpage


<!-- ======================================== -->
\LuaMethod{LuaManager,create}
\LuaClassUp{LuaManager}


Create new Lua script.


\code
function LuaManager:create(name, text)
\endcode

\param name Name of the scrit.
\param text Content of the script.
\returns \LuaClassRef{LuaScript} The freshly created script.

\endpage


<!-- ======================================== -->
\LuaMethod{LuaManager,getScript}
\LuaClassUp{LuaManager}


Get existing Lua script.


\code
function LuaManager:getScript(name)
\endcode

\param name Name of the scrit.
\returns \LuaClassRef{LuaScript} The existing script with this resource name.

\endpage
*/
