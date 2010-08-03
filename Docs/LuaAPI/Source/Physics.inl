/*! \page Physics Physics
\LuaSectionUp{index}

\beginmenu{Classes}
\LuaClassMenu{RayTest}
\LuaClassMenu{SweptTest}
\endmenu

\endpage


<!-- ============================================ RayTest ============================================ -->
\LuaClass{RayTest}
\LuaSectionUp{Physics}


\beginmenu{Methods}
\LuaMethodMenu{RayTest,constructor}
\LuaMethodMenu{RayTest,cast}
\LuaMethodMenu{RayTest,getHitPoint}
\LuaMethodMenu{RayTest,getHitNormal}
\LuaMethodMenu{RayTest,getHitFraction}
\LuaMethodMenu{RayTest,getObject}
\endmenu

\endpage


<!-- ======================================== -->
\LuaMethod{RayTest,constructor}
\LuaClassUp{RayTest}

Construct a new test.

\code
function RayTest:constructor()
\endcode


\endpage

<!-- ======================================== -->
\LuaMethod{RayTest,cast}
\LuaClassUp{RayTest}

Do the test.

\code
function RayTest:cast(ray)
\endcode

\param ray \LuaClassRef{Ray} as the starting position and heading.
\returns bool True if the ray has hit.


\endpage


<!-- ======================================== -->
\LuaMethod{RayTest,getHitPoint}
\LuaClassUp{RayTest}

Returns the hit point.

\code
function RayTest:getHitPoint()
\endcode

\returns \LuaClassRef{Vector3}

\endpage

<!-- ======================================== -->
\LuaMethod{RayTest,getHitNormal}
\LuaClassUp{RayTest}

Returns the hit normal.

\code
function RayTest:getHitNormal()
\endcode

\returns \LuaClassRef{Vector3}

\endpage

<!-- ======================================== -->
\LuaMethod{RayTest,getHitFraction}
\LuaClassUp{RayTest}

Returns the hit fraction.

\code
function RayTest:getHitFraction()
\endcode

\returns number

\endpage


<!-- ======================================== -->
\LuaMethod{RayTest,getObject}
\LuaClassUp{RayTest}

Returns the hit object.

\code
function RayTest:getObject()
\endcode

\returns \LuaClassRef{GameObject}


\endpage


<!-- ============================================ SweptTest ============================================ -->
\LuaClass{SweptTest}
\LuaSectionUp{Physics}


\beginmenu{Methods}
\LuaMethodMenu{SweptTest,constructor}
\LuaMethodMenu{SweptTest,collides}
\LuaMethodMenu{SweptTest,getHitPoint}
\LuaMethodMenu{SweptTest,getHitNormal}
\LuaMethodMenu{SweptTest,getReflection}
\LuaMethodMenu{SweptTest,getSliding}
\LuaMethodMenu{SweptTest,getObject}
\endmenu



<!-- ======================================== -->
\LuaMethod{SweptTest,constructor}
\LuaClassUp{SweptTest}

Construct a new test.

\code
function SweptTest:constructor()
\endcode


\n\n

<!-- ================= -->
\sectionseperator{Overload:}

Construct a new test with a list of objects to avoid.

\code
function SweptTest:constructor(avoid)
\endcode

\param avoid \LuaClassRef{ObjectList}


\endpage

<!-- ======================================== -->
\LuaMethod{SweptTest,collides}
\LuaClassUp{SweptTest}

Do the test.

\code
function SweptTest:collides(ray, radius)
\endcode

\param ray \LuaClassRef{Ray} as the starting position and heading.
\param radius Radius of the collision sphere.

\returns bool True if the test has hit.



\endpage


<!-- ======================================== -->
\LuaMethod{SweptTest,getHitPoint}
\LuaClassUp{SweptTest}

Returns the hit point.

\code
function SweptTest:getHitPoint()
\endcode

\returns \LuaClassRef{Vector3}

\endpage

<!-- ======================================== -->
\LuaMethod{SweptTest,getHitNormal}
\LuaClassUp{SweptTest}

Returns the hit normal.

\code
function SweptTest:getHitNormal()
\endcode

\returns \LuaClassRef{Vector3}

\endpage



<!-- ======================================== -->
\LuaMethod{SweptTest,getReflection}
\LuaClassUp{SweptTest}

Returns the reflection after hit.

\code
function SweptTest:getReflection()
\endcode

\returns \LuaClassRef{Vector3}

\endpage


<!-- ======================================== -->
\LuaMethod{SweptTest,getSliding}
\LuaClassUp{SweptTest}

Returns sliding vector along the plane normal.


\code
function SweptTest:getSliding()
\endcode

\returns \LuaClassRef{Vector3}

\endpage


<!-- ======================================== -->
\LuaMethod{SweptTest,getObject}
\LuaClassUp{SweptTest}

Returns the hit object.

\code
function SweptTest:getObject()
\endcode

\returns \LuaClassRef{GameObject}


\endpage

*/
