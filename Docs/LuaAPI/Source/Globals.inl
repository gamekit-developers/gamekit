/*! \addtogroup Globals
	@{


<!-- ============================================ printf ============================================ -->
\LuaClass{printf}

Prints a formatted message to the console.

\code
function printf(...)
\endcode

\param ... Arbitrary number of parameters in printf style format (fmt, ...)

\endpage

<!-- ============================================ dPrintf ============================================ -->
\LuaClass{dPrintf}

Prints a formatted message to the render window.

\code
function dPrintf(...)
\endcode

\param ... Arbitrary number of parameters in printf style format (fmt, ...)

\endpage


<!-- ============================================ Class ============================================ -->
\LuaClass{Class}

Constructs a new class object definition. 


\code
function Class()
\endcode

\returns table


\endpage



<!-- ============================================ BaseClass ============================================ -->
\LuaClass{BaseClass}


Constructs a new class object definition that inherits methods  from a parent class definition.


\code
function BaseClass(extends)
\endcode

\param extends \LuaClassRef{Class}
\returns table

\endpage

  @}
*/
