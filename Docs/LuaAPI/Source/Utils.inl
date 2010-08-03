/*! \page Utils Utils
\LuaSectionUp{index}


\beginmenu{Classes}
\LuaClassMenu{Array}
\LuaClassMenu{ArrayIterator}
\endmenu

\endpage



<!-- ============================================ Array ============================================ -->
\LuaClass{Array}
\LuaSectionUp{Utils}

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
\LuaClassUp{Array}


Creates an empty array.


\code
function Array:constructor()
\endcode


\endpage


<!-- ======================================== -->
\LuaMethod{Array,clear}
\LuaClassUp{Array}

Removes all elements from the array.

\code
function Array:clear()
\endcode


\endpage


<!-- ======================================== -->
\LuaMethod{Array,at}
\LuaClassUp{Array}

Returns the element at the array index specified.

\code
function Array:at(number)
\endcode

\param number Array index equal or greater than zero. 
\returns T or nil if the array index is greater than it's size

\endpage


<!-- ======================================== -->
\LuaMethod{Array,empty}
\LuaClassUp{Array}

True if the array has no elements.

\code
function Array:empty()
\endcode

\returns bool


\endpage


<!-- ======================================== -->
\LuaMethod{Array,size}
\LuaClassUp{Array}

Returns the number of elements in this array.

\code
function Array:size()
\endcode

\returns number


\endpage


<!-- ======================================== -->
\LuaMethod{Array,push}
\LuaClassUp{Array}

Adds an element to the back of the array.

\code
function Array:push(T)
\endcode


\param T Generic type depending on the type contained in the array.

\endpage




<!-- ======================================== -->
\LuaMethod{Array,erase}
\LuaClassUp{Array}

Removes an element from the array.


\code
function Array:erase(T)
\endcode


\param T Generic type depending on the type contained in the array.
\returns bool True if the object was removed, false if T is not in the array.

\endpage



<!-- ======================================== -->
\LuaMethod{Array,iterator}
\LuaClassUp{Array}

Returns an iterator for all elements in this array.


\code
function Array:iterator()
\endcode

\returns \LuaClassRef{ArrayIterator}


\endpage



<!-- ======================================== -->
\LuaMethod{Array,__index}
\LuaClassUp{Array}

Index meta method.

\code
function Array:__index(number)
\endcode

\param number Array index equal or greater than zero. 
\returns T or nil if the array index is greater than it's size

\endpage



<!-- ============================================ ArrayIterator ============================================ -->
\LuaClass{ArrayIterator}
\LuaSectionUp{Utils}

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
\LuaClassUp{ArrayIterator}


Creates an empty iterator.

\code
function ArrayIterator:constructor()
\endcode


\endpage


<!-- ======================================== -->
\LuaMethod{ArrayIterator,hasMoreElements}
\LuaClassUp{ArrayIterator}

Returns true if more iterations are present.

\code
function ArrayIterator:hasMoreElements()
\endcode

\returns bool

\endpage


<!-- ======================================== -->
\LuaMethod{ArrayIterator,moveNext}
\LuaClassUp{ArrayIterator}

Advances to the next iteration.

\code
function ArrayIterator:moveNext()
\endcode

\endpage


<!-- ======================================== -->
\LuaMethod{ArrayIterator,peekNext}
\LuaClassUp{ArrayIterator}

Returns the next iteration, without advancing.

\code
function ArrayIterator:peekNext()
\endcode

\returns T The data type contained in the array. 

\endpage


<!-- ======================================== -->
\LuaMethod{ArrayIterator,getNext}
\LuaClassUp{ArrayIterator}

Returns the current iteration, and advances to the next.

\code
function ArrayIterator:getNext()
\endcode

\returns T The data type contained in the array. 


\endpage

*/
