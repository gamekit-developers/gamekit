/*!

<!-- ============================================ Array ============================================ -->
\LuaClass{Array}


\beginmenu{Methods}
\LuaMethodMenu{Array,constructor}
\LuaMethodMenu{Array,clear}
\LuaMethodMenu{Array,at}
\LuaMethodMenu{Array,size}
\LuaMethodMenu{Array,empty}
\LuaMethodMenu{Array,push}
\LuaMethodMenu{Array,erase}
\LuaMethodMenu{Array,iterator}
\endmenu


\beginmenu{Operators}
\LuaMethodMenu{Array,__index}
\endmenu

\endpage


<!-- ======================================== -->
\LuaMethod{Array,constructor}



Creates an empty array.


\code
function Array:constructor()
\endcode


\endpage


<!-- ======================================== -->
\LuaMethod{Array,clear}


Removes all elements from the array.

\code
function Array:clear()
\endcode


\endpage


<!-- ======================================== -->
\LuaMethod{Array,at}


Returns the element at the array index specified.

\code
function Array:at(number)
\endcode

\param number array index equal or greater than zero. 
\returns T or nil if the array index is greater than it's size

\endpage


<!-- ======================================== -->
\LuaMethod{Array,empty}


True if the array has no elements.

\code
function Array:empty()
\endcode

\returns bool


\endpage


<!-- ======================================== -->
\LuaMethod{Array,size}


Returns the number of elements in this array.

\code
function Array:size()
\endcode

\returns number


\endpage


<!-- ======================================== -->
\LuaMethod{Array,push}


Adds an element to the back of the array.

\code
function Array:push(T)
\endcode


\param T Generic type depending on the type contained in the array.

\endpage




<!-- ======================================== -->
\LuaMethod{Array,erase}


Removes an element from the array.


\code
function Array:erase(T)
\endcode


\param T Generic type depending on the type contained in the array.
\returns bool True if the object was removed, false if T is not in the array.

\endpage



<!-- ======================================== -->
\LuaMethod{Array,iterator}


Returns an iterator for all elements in this array.


\code
function Array:iterator()
\endcode

\returns \LuaClassRef{ArrayIterator}


\endpage



<!-- ======================================== -->
\LuaMethod{Array,__index}


Index meta method.

\code
function Array:__index(number)
\endcode

\param number array index equal or greater than zero. 
\returns T or nil if the array index is greater than it's size

\endpage



<!-- ============================================ ArrayIterator ============================================ -->
\LuaClass{ArrayIterator}


\beginmenu{Methods}
\LuaMethodMenu{ArrayIterator,constructor}
\LuaMethodMenu{ArrayIterator,hasMoreElements}
\LuaMethodMenu{ArrayIterator,moveNext}
\LuaMethodMenu{ArrayIterator,peekNext}
\LuaMethodMenu{ArrayIterator,getNext}
\endmenu

\endpage


<!-- ======================================== -->
\LuaMethod{ArrayIterator,constructor}



Creates an empty iterator.

\code
function ArrayIterator:constructor()
\endcode


\endpage


<!-- ======================================== -->
\LuaMethod{ArrayIterator,hasMoreElements}


Returns true if more iterations are present.

\code
function ArrayIterator:hasMoreElements()
\endcode

\returns bool

\endpage


<!-- ======================================== -->
\LuaMethod{ArrayIterator,moveNext}


Advances to the next iteration.

\code
function ArrayIterator:moveNext()
\endcode

\endpage


<!-- ======================================== -->
\LuaMethod{ArrayIterator,peekNext}


Returns the next iteration, without advancing.

\code
function ArrayIterator:peekNext()
\endcode

\returns T The data type contained in the array. 

\endpage


<!-- ======================================== -->
\LuaMethod{ArrayIterator,getNext}


Returns the current iteration, and advances to the next.

\code
function ArrayIterator:getNext()
\endcode

\returns T The data type contained in the array. 


\endpage

*/
