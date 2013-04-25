/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Charlie C.

    Contributor(s): Thomas Trocha(dertom)
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
#ifndef _gsMath_h_
#define _gsMath_h_

#include "gsCommon.h"

/** \addtogroup Math
	@{
*/


class gsVector3
#ifndef SWIG
	: public gkVector3
#endif
{
public:


#ifdef SWIG
	/**
		\LuaMethod{Vector3,x}

		x axis component.

		\code
		number Vector3.x		
		\endcode		 
	*/
	float x;
	/**
		\LuaMethod{Vector3,y}

		y axis component.

		\code
		number Vector3.y	
		\endcode		 
	*/
	float y;
	/**
		\LuaMethod{Vector3,z}

		z axis component.

		\code
		number Vector3.z
		\endcode		 
	*/
	float z;
#else
	gsVector3(const gkVector3& rhs) : gkVector3(rhs.x, rhs.y, rhs.z) {}
#endif
	/**

	*/
	gsVector3() : gkVector3(0, 0, 0) {}
	/**
		\LuaMethod{Vector3,constructor}

		Constructs a new vector.

		\code
		function Vector3:constructor(x, y, z)
		\endcode

		\param x x component.
		\param y y component.
		\param z z component.
	*/
	gsVector3(float _x, float _y, float _z) : gkVector3(_x, _y, _z) {}
	/**
		\sectionseperator{Overload:}

		Constructs a new copy.

		\code
		function Vector3:constructor(vec)
		\endcode

		\param vec The \LuaClassRef{Vector3} to copy.	
	*/
	gsVector3(const gsVector3& rhs) : gkVector3(rhs.x, rhs.y, rhs.z) {}

	/**
		\LuaMethod{Vector3,__tostring}

		Vector3 string meta method.

		\code
		function Vector3:__tostring()
		\endcode

		\returns string x, y, z
	*/
	const char* __str__(void)
	{
		static char buffer[72];
		sprintf(buffer, "%f, %f, %f", x, y, z);
		return buffer;
	}
	/**
		\LuaMethod{Vector3,normalize}

		Normalizes the vector.

		\code
		function Vector3:normalize()
		\endcode

		\returns number The previous length.
	*/
	float       normalize(void)                    { return normalise(); }
	/**
		\LuaMethod{Vector3,dot}

		Returns the dot product.

		\code
		function Vector3:dot(vec)
		\endcode

		\param vec The other vector.
		\returns number
	*/
	float       dot(const gsVector3& v)            { return dotProduct(v); }
	/**

	*/
	gsVector3   cross(const gsVector3& v)          { return gsVector3(crossProduct(v));}
	/**
		\LuaMethod{Vector3,length}

		Returns sqrt(length).

		\code
		function Vector3:length(vec)
		\endcode

		\returns number
	*/
	float       length(void)                       { return gkVector3::length(); }
	/**
		\LuaMethod{Vector3,length2}

		Returns the squared length.

		\code
		function Vector3:length2(vec)
		\endcode

		\returns number
	*/
	float       length2(void)                      { return gkVector3::squaredLength(); }
	/**
		\LuaMethod{Vector3,distance}

		Returns sqrt(distance2).

		\code
		function Vector3:distance(vec)
		\endcode

		\returns number
	*/
	float       distance(const gsVector3& v)       { return gkVector3::distance(v); }
	/**
		\LuaMethod{Vector3,distance2}

		Returns the squared distance.

		\code
		function Vector3:distance2(vec)
		\endcode

		\returns number
	*/
	float       distance2(const gsVector3& v)      { return gkVector3::squaredDistance(v); }

	/**
		\LuaMethod{Vector3,__add}

		Vector3 addition meta method.

		\code
		function Vector3:__add(a, b)
		\endcode

		\param a \LuaClassRef{Vector3}
		\param b \LuaClassRef{Vector3}
		\returns \LuaClassRef{Vector3} a + b		
	*/
	gsVector3 operator- (void)                { return -(gkVector3) * this; }
	/**
		\LuaMethod{Vector3,__add}

		Vector3 addition meta method.

		\code
		function Vector3:__add(a, b)
		\endcode

		\param a \LuaClassRef{Vector3}
		\param b \LuaClassRef{Vector3}
		\returns \LuaClassRef{Vector3} a + b
	*/
	gsVector3 operator+ (const gsVector3& v)  { return ((const gkVector3&) * this).operator + (v); }
	/**
		\LuaMethod{Vector3,__sub}

		Vector3 subtraction meta method.

		\code
		function Vector3:__sub(a, b)
		\endcode

		\param a \LuaClassRef{Vector3}
		\param b \LuaClassRef{Vector3}
		\returns \LuaClassRef{Vector3} a - b 
	*/
	gsVector3 operator- (const gsVector3& v)  { return ((const gkVector3&) * this).operator - (v); }
	/**
		\LuaMethod{Vector3,__mul}

		Vector3 multiplication meta method.

		\code
		function Vector3:__mul(a, b)
		\endcode

		\param a \LuaClassRef{Vector3}
		\param b \LuaClassRef{Vector3}
		\returns \LuaClassRef{Vector3} a * b
	*/
	gsVector3 operator* (const gsVector3& v)  { return ((const gkVector3&) * this).operator * (v); }
	/**
		\sectionseperator{Overload:}

		\code
		function Vector3:__mul(a, b)
		\endcode

		\param a \LuaClassRef{Vector3}
		\param b number
		\returns \LuaClassRef{Vector3} a * b
	*/
	gsVector3 operator* (float v)             { return ((const gkVector3&) * this).operator * (v); }
	/**
		\LuaMethod{Vector3,__div}

		Vector3 division meta method.

		\code
		function Vector3:__div(a, b)
		\endcode

		\param a \LuaClassRef{Vector3}
		\param b \LuaClassRef{Vector3}
		\returns \LuaClassRef{Vector3} a / b
	*/
	gsVector3 operator/ (const gsVector3& v)  { return ((const gkVector3&) * this).operator / (v); }
	/**
		\sectionseperator{Overload:}

		\code
		function Vector3:__div(a, b)
		\endcode

		\param a \LuaClassRef{Vector3}
		\param b number
		\returns \LuaClassRef{Vector3} a / b
	*/
	gsVector3 operator/ (float v)             { return ((const gkVector3&) * this).operator / (v); }
};

class gsVector4
#ifndef SWIG
	: public gkVector4
#endif
{
public:


#ifdef SWIG
	float x;
	float y;
	float z;
	float w;
#else
	gsVector4(const gkVector4& rhs) : gkVector4(rhs.x, rhs.y, rhs.z, rhs.w) {}
#endif
	gsVector4() : gkVector4(0, 0, 0,0) {}
	gsVector4(float _x, float _y, float _z) : gkVector4(_x, _y, _z, w) {}
	gsVector4(const gsVector4& rhs) : gkVector4(rhs.x, rhs.y, rhs.z, rhs.w) {}

	const char* __str__(void)
	{
		static char buffer[72];
		sprintf(buffer, "%f, %f, %f, %f", x, y, z , w);
		return buffer;
	}

	float       dot(const gsVector4& v)            { return dotProduct(v); }

	gsVector4 operator- (void)                { return -(gkVector4) * this; }
	gsVector4 operator+ (const gsVector4& v)  { return ((const gkVector4&) * this).operator + (v); }
	gsVector4 operator- (const gsVector4& v)  { return ((const gkVector4&) * this).operator - (v); }
	gsVector4 operator* (const gsVector4& v)  { return ((const gkVector4&) * this).operator * (v); }
	gsVector4 operator* (float v)             { return ((const gkVector4&) * this).operator * (v); }
	gsVector4 operator/ (const gsVector4& v)  { return ((const gkVector4&) * this).operator / (v); }
	gsVector4 operator/ (float v)             { return ((const gkVector4&) * this).operator / (v); }
};


extern gsVector3 lerp(const gsVector3& vec1,const gsVector3& vec2, float t);
extern gkVector3 getTriNormal(const gkVertex& v0,const gkVertex& v1,const gkVertex& v2);

class gsQuaternion
#ifndef SWIG
	: public gkQuaternion
#endif
{
public:

#ifdef SWIG
	/**
		\LuaMethod{Quaternion,w}

		w component.

		\code
		number Quaternion.w
		\endcode
	*/
	float w;
	/**
		\LuaMethod{Quaternion,x}

		x component.

		\code		
		number Quaternion.x
		\endcode
	*/
	float x;
	/**
		\LuaMethod{Quaternion,y}

		y component.

		\code		
		number Quaternion.y
		\endcode
	*/
	float y;
	/**
		\LuaMethod{Quaternion,z}

		z component.

		\code		
		number Quaternion.z
		\endcode
	*/
	float z;
#else
	gsQuaternion(const gkQuaternion& rhs) : gkQuaternion(rhs.w, rhs.x, rhs.y, rhs.z) {}
#endif
	/**
	
	*/
	gsQuaternion() : gkQuaternion(1, 0, 0, 0) {}
	/**
		\LuaMethod{Quaternion,constructor}

		Constructs a new quaternion.

		\code
		function Quaternion:constructor(w, x, y, z)
		\endcode

		\param w w component.
		\param x x component.
		\param y y component.
		\param z z component.
	*/
	gsQuaternion(float _w, float _x, float _y, float _z) : gkQuaternion(_w, _x, _y, _z) {}
	/**
		\sectionseperator{Overload:}

		Constructs a new copy.

		\code
		function Quaternion:constructor(quat)
		\endcode

		\param quat The \LuaClassRef{Quaternion} to copy.	
	*/
	gsQuaternion(const gsQuaternion& rhs) : gkQuaternion(rhs.w, rhs.x, rhs.y, rhs.z) {}

	/**
		\LuaMethod{Quaternion,__tostring}

		Quaternion string meta method.

		\code
		function Quaternion:__tostring()
		\endcode

		\returns string w, x, y, z	
	*/
	const char* __str__(void)
	{
		static char buffer[72];
		sprintf(buffer, "%f, %f, %f, %f", w, x, y, z);
		return buffer;
	}
	/**
		\LuaMethod{Quaternion,normalize}

		Normalizes the quaternion

		\code
		function Quaternion:normalize()
		\endcode

		\returns number The previous length.	
	*/
	float       normalize(void)                 { return normalise(); }
	/**
		\LuaMethod{Quaternion,length2}

		Returns the squared length.

		\code
		function Quaternion:length2()
		\endcode

		\returns number The length.		
	*/
	float       length2(void)                   { return Norm(); }
	/**
		\LuaMethod{Quaternion,dot}

		Returns the dot product.

		\code
		function Quaternion:dot(quat)
		\endcode

		\param quat The other quaternion.
		\returns number The restlt 
	*/
	float       dot(const gsQuaternion& v)        { return Dot(v); }
	/**
		\LuaMethod{Quaternion,inverse}

		Returns the inverse of the quaternion

		\code
		function Quaternion:inverse()
		\endcode

		\returns \LuaClassRef{Quaternion}.	
	*/
	gsQuaternion  inverse(void)                   { return Inverse(); }
	/**
		\LuaMethod{Quaternion,__unm}

		Unary minus meta method. 

		\code
		function Quaternion:__unm()
		\endcode

		\returns -\LuaClassRef{Quaternion}.	
	*/
	gsQuaternion operator- (void)                 { return -(gkQuaternion) * this; }
	/**
		\LuaMethod{Quaternion,__add}

		Quaternion addition meta method.

		\code
		function Quaternion:__add(a, b)
		\endcode

		\param a \LuaClassRef{Quaternion}
		\param b \LuaClassRef{Quaternion}
		\returns \LuaClassRef{Quaternion} a + b
	*/
	gsQuaternion operator+ (const gsQuaternion& v)  { return ((const gkQuaternion&) * this).operator + (v); }
	/**
		\LuaMethod{Quaternion,__sub}

		Quaternion subtraction meta method.

		\code
		function Quaternion:__sub(a, b)
		\endcode

		\param a \LuaClassRef{Quaternion}
		\param b \LuaClassRef{Quaternion}
		\returns \LuaClassRef{Quaternion} a - b	
	*/
	gsQuaternion operator- (const gsQuaternion& v)  { return ((const gkQuaternion&) * this).operator - (v); }
	/**
		\LuaMethod{Quaternion,__mul}

		Quaternion multiplication meta method.

		\code
		function Quaternion:__mul(a, b)
		\endcode

		\param a \LuaClassRef{Quaternion}
		\param b \LuaClassRef{Quaternion}
		\returns \LuaClassRef{Quaternion} a * b	
	*/
	gsQuaternion operator* (const gsQuaternion& v)  { return ((const gkQuaternion&) * this).operator * (v); }
	/**
		\sectionseperator{Overload 1:}

		\code
		function Quaternion:__mul(a, b)
		\endcode

		\param a \LuaClassRef{Quaternion}
		\param b number
		\returns \LuaClassRef{Quaternion} a * b	
	*/
	gsQuaternion operator* (float v)              { return ((const gkQuaternion&) * this).operator * (v); }
	/**
		\sectionseperator{Overload 2:}

		Vector3 multiplication

		\code
		function Quaternion:__mul(a, b)
		\endcode

		\param a \LuaClassRef{Vector3}
		\param b \LuaClassRef{Quaternion}
		\returns \LuaClassRef{Vector3} a * b	
	*/
	gsVector3    operator* (const gsVector3& v)     { return ((const gkQuaternion&) * this).operator * (v); }
};


class gsRay
#ifndef SWIG
	: public Ogre::Ray
#endif
{
public:
	gsRay() {}
	/**
		\LuaMethod{Ray,constructor}

		Constructs a new ray.

		\code
		function Ray:constructor(origin, direction)
		\endcode

		\param origin The starting position.
		\param direction Heading.	
	*/
	gsRay(const gsVector3& origin, const gsVector3& direction) : Ogre::Ray(origin, direction) {}
	/**
		\LuaMethod{Ray,setOrigin}

		Sets the position of the ray.

		\code
		function Ray:setOrigin(origin)
		\endcode

		\param origin \LuaClassRef{Vector3} as the starting position.	
	*/
	void setOrigin(const gsVector3& origin)       {Ogre::Ray::setOrigin(origin);}
	/**
		\LuaMethod{Ray,setDirection}

		Sets the heading of the ray.

		\code
		function Ray:setDirection(dir)
		\endcode

		\param dir \LuaClassRef{Vector3} as the ray heading.		
	*/
	void setDirection(const gsVector3& origin)    {Ogre::Ray::setDirection(origin);}

	/**
		\LuaMethod{Ray,getOrigin}

		Gets the position of the ray.

		\code
		function Ray:getOrigin()
		\endcode

		\returns \LuaClassRef{Vector3} as the starting position.			
	*/
	gsVector3 getOrigin()       {return Ogre::Ray::getOrigin();}
	/**
		\LuaMethod{Ray,getDirection}

		Gets the heading of the ray.

		\code
		function Ray:getDirection()
		\endcode

		\returns \LuaClassRef{Vector3} as the ray heading.	
	*/
	gsVector3 getDirection()    {return Ogre::Ray::getDirection();}
};



/**
	Vector3 unary minus.


	\code
	function OgreKit.Vec3Negate(a, b)
	\endcode

	\param a \LuaClassRef{Vector3}
	\param b \LuaClassRef{Vector3}
	\returns -\LuaClassRef{Vector3}
*/
inline gsVector3 gsVec3Negate(const gsVector3& in)
{
	return -(gkVector3)in;
}


/**
	Vector3 addition.

	\code
	function OgreKit.Vec3AddVec3(a, b)
	\endcode

	\param a \LuaClassRef{Vector3}
	\param b \LuaClassRef{Vector3}
	\returns \LuaClassRef{Vector3}
*/
inline gsVector3 gsVec3AddVec3(const gsVector3& a, const gsVector3& b)
{
	return (gkVector3)a + (gkVector3)b;
}


/**
	Vector3 subtraction.


	\code
	function OgreKit.Vec3SubVec3(a, b)
	\endcode

	\param a \LuaClassRef{Vector3}
	\param b \LuaClassRef{Vector3}
	\returns \LuaClassRef{Vector3}
*/
inline gsVector3 gsVec3SubVec3(const gsVector3& a, const gsVector3& b)
{
	return (gkVector3)a - (gkVector3)b;
}


/**
	Vector3 multiplication.


	\code
	function OgreKit.Vec3MulVec3(a, b)
	\endcode

	\param a \LuaClassRef{Vector3}
	\param b \LuaClassRef{Vector3}
	\returns \LuaClassRef{Vector3}	
*/
inline gsVector3 gsVec3MulVec3(const gsVector3& a, const gsVector3& b)
{
	return (gkVector3)a / (gkVector3)b;
}


/**
	Vector3 division.

	\code
	function OgreKit.Vec3DivVec3(a, b)
	\endcode

	\param a \LuaClassRef{Vector3}
	\param b \LuaClassRef{Vector3}
	\returns \LuaClassRef{Vector3}	
*/
inline gsVector3 gsVec3DivVec3(const gsVector3& a, const gsVector3& b)
{
	return (gkVector3)a / (gkVector3)b;
}


/**
	Vector3 multiplication.

	\code
	function OgreKit.Vec3Mulf(a, b)
	\endcode

	\param a \LuaClassRef{Vector3}
	\param b number
	\returns \LuaClassRef{Vector3}	
*/
inline gsVector3 gsVec3Mulf(const gsVector3& a, const float& b)
{
	return (gkVector3)a / b;
}


/**
	Vector3 division.

	\code
	function OgreKit.Vec3Divf(a, b)
	\endcode

	\param a \LuaClassRef{Vector3}
	\param b number
	\returns \LuaClassRef{Vector3}	
*/
inline gsVector3 gsVec3Divf(const gsVector3& a, const float& b)
{
	return (gkVector3)a / b;
}


/**
	Quaternion unary minus.


	\code
	function OgreKit.QuatNegate(a)
	\endcode

	\param a \LuaClassRef{Quaternion}
	\returns -\LuaClassRef{Quaternion}	
*/
inline gsQuaternion gsQuatNegate(const gsQuaternion& in)
{
	return -(gkQuaternion)in;
}


/**
	Quaternion addition.

	\code
	function OgreKit.QuatAddQuat(a, b)
	\endcode

	\param a \LuaClassRef{Quaternion}
	\param b \LuaClassRef{Quaternion}
	\returns \LuaClassRef{Quaternion}
*/
inline gsQuaternion gsQuatAddQuat(const gsQuaternion& a, const gsQuaternion& b)
{
	return (gkQuaternion)a + (gkQuaternion)b;
}


/**
	Quaternion subtraction.

	\code
	function OgreKit.QuatSubQuat(a, b)
	\endcode

	\param a \LuaClassRef{Quaternion}
	\param b \LuaClassRef{Quaternion}
	\returns \LuaClassRef{Quaternion}	
*/
inline gsQuaternion gsQuatSubQuat(const gsQuaternion& a, const gsQuaternion& b)
{
	return (gkQuaternion)a - (gkQuaternion)b;
}


/**
	Quaternion multiplication.

	\code
	function OgreKit.QuatMulQuat(a, b)
	\endcode

	\param a \LuaClassRef{Quaternion}
	\param b \LuaClassRef{Quaternion}
	\returns \LuaClassRef{Quaternion}	
*/
inline gsQuaternion gsQuatMulQuat(const gsQuaternion& a, const gsQuaternion& b)
{
	return (gkQuaternion)a * (gkQuaternion)b;
}


/**
	Quaternion multiplication.

	\code
	function OgreKit.QuatMulf(a, b)
	\endcode

	\param a \LuaClassRef{Quaternion}
	\param b number
	\returns \LuaClassRef{Quaternion}	
*/
inline gsQuaternion gsQuatMulf(const gsQuaternion& a, const float& b)
{
	return (gkQuaternion)a * b;
}


/**
	Quaternion multiplication.

	\code
	function OgreKit.QuatMulVec3(a, b)
	\endcode

	\param a \LuaClassRef{Quaternion}
	\param b \LuaClassRef{Vector3}
	\returns \LuaClassRef{Quaternion}	
*/
inline gsVector3 gsQuatMulVec3(const gsQuaternion& a, const gsVector3& b)
{
	return (gkQuaternion)a * (gkVector3)b;
}

/**
	Gets the shortest arc quaternion to rotate a vector to the b vector.

	\code
	function OgreKit.Vec3RotationTo(a, b)
	\endcode

	\param a \LuaClassRef{Vector3}
	\param b \LuaClassRef{Vector3}
	\returns \LuaClassRef{Quaternion}
*/
inline gsQuaternion gsVec3RotationTo(const gsVector3& a, const gsVector3& b) 
{ 
	return ((gkVector3)a).getRotationTo(b); 
}

/** @}
*/

#endif//_gsMath_h_
