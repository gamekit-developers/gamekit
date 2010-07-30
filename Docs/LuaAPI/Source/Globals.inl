/*! \page Globals Globals


\beginmenu{Globals}
\LuaGlobalMenu{Class}
\LuaGlobalMenu{BaseClass}
\LuaGlobalMenu{printf}
\LuaGlobalMenu{dPrintf}
\endmenu


\endpage


<!-- ============================================ printf ============================================ -->
\LuaGlobal{printf}

Prints a formatted message to the console.

\code
function printf(...)
\endcode

\param ... Arbitrary number of parameters in printf style format (fmt, ...)

\endpage

<!-- ============================================ dPrintf ============================================ -->
\LuaGlobal{dPrintf}

Prints a formatted message to the render window.

\code
function dPrintf(...)
\endcode

\param ... Arbitrary number of parameters in printf style format (fmt, ...)

\endpage


<!-- ============================================ Class ============================================ -->
\LuaGlobal{Class}

Constructs a new class object definition. 


\code
function Class()
\endcode

\returns table


\sectionseperator{Usage:}
\code

NewClass = Class()

function NewClass:constructor()
    
    ...
end

function NewClass:method(param)
    ...
end

local inst = NewClass()
inst:method("Hello World")

\endcode


\endpage



<!-- ============================================ BaseClass ============================================ -->
\LuaGlobal{BaseClass}


Constructs a new class object definition that inherits methods  from a parent class definition.


\code
function BaseClass(extends)
\endcode

\returns table


\sectionseperator{Usage:}

\code

NewClass = Class()
function NewClass:constructor()
    ...
end

function NewClass:method(param)
    ...
end


NewDerrived = BaseClass(NewClass)
NewDerrived:constructor()
    ...
end


local inst = NewDerrived()
inst:method("Hello World")


\endcode

\endpage


*/
