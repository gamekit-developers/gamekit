/*!

<!-- ============================================ Vector3 ============================================ -->
\LuaClass{Vector3}
\LuaSectionUp{Math}

\beginmenu{Members}
\LuaMethodMenu{Vector3,x}
\LuaMethodMenu{Vector3,y}
\LuaMethodMenu{Vector3,z}
\endmenu


\beginmenu{Methods}
\LuaMethodMenu{Vector3,constructor}
\LuaMethodMenu{Vector3,normalize}
\LuaMethodMenu{Vector3,dot}
\LuaMethodMenu{Vector3,length}
\LuaMethodMenu{Vector3,length2}
\LuaMethodMenu{Vector3,distance}
\LuaMethodMenu{Vector3,distance2}
\endmenu



\beginmenu{Operators}
\LuaMethodMenu{Vector3,__tostring}
\LuaMethodMenu{Vector3,__add}
\LuaMethodMenu{Vector3,__sub}
\LuaMethodMenu{Vector3,__mul}
\LuaMethodMenu{Vector3,__div}
\LuaMethodMenu{Vector3,__unm}
\endmenu


\endpage


<!-- ======================================== -->
\LuaMethod{Vector3,x}
\LuaClassUp{Vector3}

X axis component.

\code
number Vector3.x
\endcode
\endpage 


<!-- ======================================== -->
\LuaMethod{Vector3,y}
\LuaClassUp{Vector3}

Y axis component.

\code
number Vector3.y
\endcode
\endpage 



<!-- ======================================== -->
\LuaMethod{Vector3,z}
\LuaClassUp{Vector3}

Z axis component.

\code
number Vector3.z
\endcode
\endpage 


<!-- ======================================== -->
\LuaMethod{Vector3,constructor}
\LuaClassUp{Vector3}

Constructs a new vector.

\code
function Vector3:constructor(x, y, z)
\endcode

\param x x component.
\param y y component.
\param z z component.

<!-- ================= -->
\sectionseperator{Overload:}

Constructs a new copy.

\code
function Vector3:constructor(vec)
\endcode

\param vec The \LuaClassRef{Vector3} to copy.

\endpage


<!-- ======================================== -->
\LuaMethod{Vector3,normalize}
\LuaClassUp{Vector3}

Normalizes the vector.

\code
function Vector3:normalize()
\endcode

\returns number The previous length.

\endpage


<!-- ======================================== -->
\LuaMethod{Vector3,dot}
\LuaClassUp{Vector3}

Returns the dot product.

\code
function Vector3:dot(vec)
\endcode

\param vec The other vector.
\returns number

\endpage


<!-- ======================================== -->
\LuaMethod{Vector3,length}
\LuaClassUp{Vector3}

Returns sqrt(length).


\code
function Vector3:length(vec)
\endcode

\returns number

\endpage


<!-- ======================================== -->
\LuaMethod{Vector3,length2}
\LuaClassUp{Vector3}

Returns the squared length.


\code
function Vector3:length2(vec)
\endcode

\returns number

\endpage

<!-- ======================================== -->
\LuaMethod{Vector3,distance}
\LuaClassUp{Vector3}

Returns sqrt(distance2).


\code
function Vector3:distance(vec)
\endcode

\returns number

\endpage


<!-- ======================================== -->
\LuaMethod{Vector3,distance2}
\LuaClassUp{Vector3}

Returns the squared distance.

\code
function Vector3:distance2(vec)
\endcode

\returns number

\endpage

<!-- ======================================== -->
\LuaMethod{Vector3,__tostring}
\LuaClassUp{Vector3}

Vector3 string meta method.

\code
function Vector3:__tostring()
\endcode

\returns string x, y, z


\endpage


<!-- ======================================== -->
\LuaMethod{Vector3,__sub}
\LuaClassUp{Vector3}

Vector3 subtraction meta method.

\code
function Vector3:__sub(a, b)
\endcode

\param a \LuaClassRef{Vector3}
\param b \LuaClassRef{Vector3}
\returns \LuaClassRef{Vector3} a - b 

\endpage



<!-- ======================================== -->
\LuaMethod{Vector3,__add}
\LuaClassUp{Vector3}

Vector3 addition meta method.

\code
function Vector3:__add(a, b)
\endcode

\param a \LuaClassRef{Vector3}
\param b \LuaClassRef{Vector3}
\returns \LuaClassRef{Vector3} a + b

\endpage



<!-- ======================================== -->
\LuaMethod{Vector3,__mul}
\LuaClassUp{Vector3}

Vector3 multiplication meta method.

\code
function Vector3:__mul(a, b)
\endcode

\param a \LuaClassRef{Vector3}
\param b \LuaClassRef{Vector3}
\returns \LuaClassRef{Vector3} a * b

<!-- ================= -->
\sectionseperator{Overload:}
\code
function Vector3:__mul(a, b)
\endcode

\param a \LuaClassRef{Vector3}
\param b number
\returns \LuaClassRef{Vector3} a * b


\endpage


<!-- ======================================== -->
\LuaMethod{Vector3,__div}
\LuaClassUp{Vector3}

Vector3 division meta method.

\code
function Vector3:__div(a, b)
\endcode

\param a \LuaClassRef{Vector3}
\param b \LuaClassRef{Vector3}
\returns \LuaClassRef{Vector3} a / b

<!-- ================= -->
\sectionseperator{Overload:}
\code
function Vector3:__div(a, b)
\endcode

\param a \LuaClassRef{Vector3}
\param b number
\returns \LuaClassRef{Vector3} a / b

\endpage

<!-- ======================================== -->
\LuaMethod{Vector3,__unm}
\LuaClassUp{Vector3}

Unary minus meta method. 

\code
function Vector3:__unm()
\endcode

\returns -\LuaClassRef{Vector3}.

\endpage


<!-- ============================================ Quaternion ============================================ -->
\LuaClass{Quaternion}
\LuaSectionUp{Math}


\beginmenu{Members}
\LuaMethodMenu{Quaternion,w}
\LuaMethodMenu{Quaternion,x}
\LuaMethodMenu{Quaternion,y}
\LuaMethodMenu{Quaternion,z}
\endmenu


\beginmenu{Methods}
\LuaMethodMenu{Quaternion,constructor}
\LuaMethodMenu{Quaternion,normalize}
\LuaMethodMenu{Quaternion,length2}
\LuaMethodMenu{Quaternion,dot}
\LuaMethodMenu{Quaternion,inverse}
\endmenu



\beginmenu{Operators}
\LuaMethodMenu{Quaternion,__tostring}
\LuaMethodMenu{Quaternion,__sub}
\LuaMethodMenu{Quaternion,__add}
\LuaMethodMenu{Quaternion,__mul}
\LuaMethodMenu{Quaternion,__unm}
\endmenu

\endpage




<!-- ======================================== -->
\LuaMethod{Quaternion,w}
\LuaClassUp{Quaternion}

W component.

\code
number Quaternion.w
\endcode
\endpage 



<!-- ======================================== -->
\LuaMethod{Quaternion,x}
\LuaClassUp{Quaternion}

X axis component.

\code
number Quaternion.x
\endcode
\endpage 


<!-- ======================================== -->
\LuaMethod{Quaternion,y}
\LuaClassUp{Quaternion}

Y axis component.

\code
number Quaternion.y
\endcode
\endpage 



<!-- ======================================== -->
\LuaMethod{Quaternion,z}
\LuaClassUp{Quaternion}

Z axis component.

\code
number Quaternion.z
\endcode
\endpage 


<!-- ======================================== -->
\LuaMethod{Quaternion,constructor}
\LuaClassUp{Quaternion}

Constructs a new quaternion.

\code
function Quaternion:constructor(w, x, y, z)
\endcode

\param w w component.
\param x x component.
\param y y component.
\param z z component.


<!-- ================= -->
\sectionseperator{Overload:}

Constructs a new copy.

\code
function Quaternion:constructor(quat)
\endcode

\param quat The \LuaClassRef{Quaternion} to copy.


\endpage

<!-- ======================================== -->
\LuaMethod{Quaternion,inverse}
\LuaClassUp{Quaternion}

Returns the inverse of the quaternion

\code
function Quaternion:inverse()
\endcode

\returns \LuaClassRef{Quaternion}.


\endpage


<!-- ======================================== -->
\LuaMethod{Quaternion,normalize}
\LuaClassUp{Quaternion}

Normalizes the quaternion

\code
function Quaternion:normalize()
\endcode

\returns number The previous length.


\endpage


<!-- ======================================== -->
\LuaMethod{Quaternion,dot}
\LuaClassUp{Quaternion}

Returns the dot product.

\code
function Quaternion:dot(quat)
\endcode


\param quat The other quaternion.
\returns number The restlt 

\endpage




<!-- ======================================== -->
\LuaMethod{Quaternion,length2}
\LuaClassUp{Quaternion}

Returns the squared length.

\code
function Quaternion:length2()
\endcode

\returns number The length.

\endpage



<!-- ======================================== -->
\LuaMethod{Quaternion,__tostring}
\LuaClassUp{Quaternion}

Quaternion string meta method.

\code
function Quaternion:__tostring()
\endcode

\returns string w, x, y, z


\endpage


<!-- ======================================== -->
\LuaMethod{Quaternion,__sub}
\LuaClassUp{Quaternion}

Quaternion subtraction meta method.

\code
function Quaternion:__sub(a, b)
\endcode

\param a \LuaClassRef{Quaternion}
\param b \LuaClassRef{Quaternion}
\returns \LuaClassRef{Quaternion} a - b

\endpage



<!-- ======================================== -->
\LuaMethod{Quaternion,__add}
\LuaClassUp{Quaternion}

Quaternion addition meta method.

\code
function Quaternion:__add(a, b)
\endcode

\param a \LuaClassRef{Quaternion}
\param b \LuaClassRef{Quaternion}
\returns \LuaClassRef{Quaternion} a + b

\endpage



<!-- ======================================== -->
\LuaMethod{Quaternion,__mul}
\LuaClassUp{Quaternion}

Quaternion multiplication meta method.

\code
function Quaternion:__mul(a, b)
\endcode

\param a \LuaClassRef{Quaternion}
\param b \LuaClassRef{Quaternion}
\returns \LuaClassRef{Quaternion} a * b

<!-- ================= -->
\sectionseperator{Overload 1:}
\code
function Quaternion:__mul(a, b)
\endcode

\param a \LuaClassRef{Quaternion}
\param b number
\returns \LuaClassRef{Quaternion} a * b



<!-- ================= -->
\sectionseperator{Overload 2:}

Vector3 multiplication

\code
function Quaternion:__mul(a, b)
\endcode

\param a \LuaClassRef{Vector3}
\param b \LuaClassRef{Quaternion}
\returns \LuaClassRef{Vector3} a * b


\endpage

<!-- ======================================== -->
\LuaMethod{Quaternion,__unm}
\LuaClassUp{Quaternion}

Unary minus meta method. 

\code
function Quaternion:__unm()
\endcode

\returns -\LuaClassRef{Quaternion}.

\endpage



<!-- ============================================ Ray ============================================ -->
\LuaClass{Ray}
\LuaSectionUp{Math}

\beginmenu{Methods}
\LuaMethodMenu{Ray,constructor}
\LuaMethodMenu{Ray,setOrigin}
\LuaMethodMenu{Ray,setDirection}
\LuaMethodMenu{Ray,getOrigin}
\LuaMethodMenu{Ray,getDirection}
\endmenu


\endpage



<!-- ======================================== -->
\LuaMethod{Ray,constructor}
\LuaClassUp{Ray}

Constructs a new ray.

\code
function Ray:constructor(origin, direction)
\endcode

\param origin The starting position.
\param direction Heading.


\endpage


<!-- ======================================== -->
\LuaMethod{Ray,setOrigin}
\LuaClassUp{Ray}

Sets the position of the ray.

\code
function Ray:setOrigin(origin)
\endcode


\param origin \LuaClassRef{Vector3} as the starting position.


\endpage


<!-- ======================================== -->
\LuaMethod{Ray,setDirection}
\LuaClassUp{Ray}

Sets the heading of the ray.

\code
function Ray:setDirection(dir)
\endcode


\param dir \LuaClassRef{Vector3} as the ray heading.

\endpage



<!-- ======================================== -->
\LuaMethod{Ray,getOrigin}
\LuaClassUp{Ray}

Gets the position of the ray.

\code
function Ray:getOrigin()
\endcode

\returns \LuaClassRef{Vector3} as the starting position.

\endpage

<!-- ======================================== -->
\LuaMethod{Ray,getDirection}
\LuaClassUp{Ray}

Gets the heading of the ray.

\code
function Ray:getDirection()
\endcode


\returns \LuaClassRef{Vector3} as the ray heading.

\endpage



<!-- ============================================ Globals ============================================ -->

<!-- ======================================== -->
\LuaClass{Vec3Negate}
\LuaSectionUp{Math}

Vector3 unary minus.


\code
function OgreKit.Vec3Negate(a, b)
\endcode

\param a \LuaClassRef{Vector3}
\param b \LuaClassRef{Vector3}
\returns -\LuaClassRef{Vector3}

\endpage



<!-- ======================================== -->
\LuaClass{Vec3AddVec3}
\LuaSectionUp{Math}

Vector3 addition.


\code
function OgreKit.Vec3AddVec3(a, b)
\endcode

\param a \LuaClassRef{Vector3}
\param b \LuaClassRef{Vector3}
\returns \LuaClassRef{Vector3}

\endpage


<!-- ======================================== -->
\LuaClass{Vec3SubVec3}
\LuaSectionUp{Math}

Vector3 subtraction.


\code
function OgreKit.Vec3SubVec3(a, b)
\endcode

\param a \LuaClassRef{Vector3}
\param b \LuaClassRef{Vector3}
\returns \LuaClassRef{Vector3}

\endpage


<!-- ======================================== -->
\LuaClass{Vec3MulVec3}
\LuaSectionUp{Math}

Vector3 multiplication.


\code
function OgreKit.Vec3MulVec3(a, b)
\endcode

\param a \LuaClassRef{Vector3}
\param b \LuaClassRef{Vector3}
\returns \LuaClassRef{Vector3}

\endpage


<!-- ======================================== -->
\LuaClass{Vec3Mulf}
\LuaSectionUp{Math}

Vector3 multiplication.


\code
function OgreKit.Vec3Mulf(a, b)
\endcode

\param a \LuaClassRef{Vector3}
\param b number
\returns \LuaClassRef{Vector3}

\endpage

<!-- ======================================== -->
\LuaClass{Vec3DivVec3}
\LuaSectionUp{Math}

Vector3 division.


\code
function OgreKit.Vec3DivVec3(a, b)
\endcode

\param a \LuaClassRef{Vector3}
\param b \LuaClassRef{Vector3}
\returns \LuaClassRef{Vector3}

\endpage

<!-- ======================================== -->
\LuaClass{Vec3Divf}
\LuaSectionUp{Math}

Vector3 division.


\code
function OgreKit.Vec3Divf(a, b)
\endcode

\param a \LuaClassRef{Vector3}
\param b number
\returns \LuaClassRef{Vector3}

\endpage


<!-- ======================================== -->
\LuaClass{QuatNegate}
\LuaSectionUp{Math}


Quaternion unary minus.


\code
function OgreKit.QuatNegate(a)
\endcode

\param a \LuaClassRef{Quaternion}
\returns -\LuaClassRef{Quaternion}

\endpage



<!-- ======================================== -->
\LuaClass{QuatAddQuat}
\LuaSectionUp{Math}


Quaternion addition.


\code
function OgreKit.QuatAddQuat(a, b)
\endcode

\param a \LuaClassRef{Quaternion}
\param b \LuaClassRef{Quaternion}
\returns \LuaClassRef{Quaternion}

\endpage


<!-- ======================================== -->
\LuaClass{QuatSubQuat}
\LuaSectionUp{Math}


Quaternion subtraction.


\code
function OgreKit.QuatSubQuat(a, b)
\endcode

\param a \LuaClassRef{Quaternion}
\param b \LuaClassRef{Quaternion}
\returns \LuaClassRef{Quaternion}

\endpage

<!-- ======================================== -->
\LuaClass{QuatMulQuat}
\LuaSectionUp{Math}


Quaternion multiplication.

\code
function OgreKit.QuatMulQuat(a, b)
\endcode

\param a \LuaClassRef{Quaternion}
\param b \LuaClassRef{Quaternion}
\returns \LuaClassRef{Quaternion}

\endpage


<!-- ======================================== -->
\LuaClass{QuatMulf}
\LuaSectionUp{Math}


Quaternion multiplication.

\code
function OgreKit.QuatMulf(a, b)
\endcode

\param a \LuaClassRef{Quaternion}
\param b number
\returns \LuaClassRef{Quaternion}

\endpage

<!-- ======================================== -->
\LuaClass{QuatMulVec3}
\LuaSectionUp{Math}


Quaternion multiplication.

\code
function OgreKit.QuatMulVec3(a, b)
\endcode

\param a \LuaClassRef{Quaternion}
\param b \LuaClassRef{Vector3}
\returns \LuaClassRef{Quaternion}

\endpage

<!-- ======================================== -->
\LuaClass{Vec3RotationTo}
\LuaSectionUp{Math}


Gets the shortest arc quaternion to rotate a vector to the b vector.

\code
function OgreKit.Vec3RotationTo(a, b)
\endcode

\param a \LuaClassRef{Vector3}
\param b \LuaClassRef{Vector3}
\returns \LuaClassRef{Quaternion}

\endpage

*/