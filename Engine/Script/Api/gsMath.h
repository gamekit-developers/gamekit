/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

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
#ifndef _gsMath_h_
#define _gsMath_h_

#include "gsCommon.h"



class gsVector3
#ifndef SWIG
	: public gkVector3
#endif
{
public:


#ifdef SWIG
	float x, y, z;
#else
	gsVector3(const gkVector3& rhs) : gkVector3(rhs.x, rhs.y, rhs.z) {}
#endif
	gsVector3() : gkVector3(0, 0, 0) {}
	gsVector3(float _x, float _y, float _z) : gkVector3(_x, _y, _z) {}
	gsVector3(const gsVector3& rhs) : gkVector3(rhs.x, rhs.y, rhs.z) {}

	const char* __str__(void)
	{
		static char buffer[72];
		sprintf(buffer, "%f, %f, %f", x, y, z);
		return buffer;
	}

	float       normalize(void)                    { return normalise(); }
	float       dot(const gsVector3& v)            { return dotProduct(v); }
	gsVector3   cross(const gsVector3& v)          { return gsVector3(crossProduct(v));}
	float       length(void)                       { return gkVector3::length(); }
	float       length2(void)                      { return gkVector3::squaredLength(); }
	float       distance(const gsVector3& v)       { return gkVector3::distance(v); }
	float       distance2(const gsVector3& v)      { return gkVector3::squaredDistance(v); }

	gsVector3 operator- (void)                { return -(gkVector3) * this; }
	gsVector3 operator+ (const gsVector3& v)  { return ((const gkVector3&) * this).operator + (v); }
	gsVector3 operator- (const gsVector3& v)  { return ((const gkVector3&) * this).operator - (v); }
	gsVector3 operator* (const gsVector3& v)  { return ((const gkVector3&) * this).operator * (v); }
	gsVector3 operator* (float v)             { return ((const gkVector3&) * this).operator * (v); }
	gsVector3 operator/ (const gsVector3& v)  { return ((const gkVector3&) * this).operator / (v); }
	gsVector3 operator/ (float v)             { return ((const gkVector3&) * this).operator / (v); }
};


class gsQuaternion
#ifndef SWIG
	: public gkQuaternion
#endif
{
public:

#ifdef SWIG
	float w, x, y, z;
#else
	gsQuaternion(const gkQuaternion& rhs) : gkQuaternion(rhs.w, rhs.x, rhs.y, rhs.z) {}
#endif
	gsQuaternion() : gkQuaternion(1, 0, 0, 0) {}
	gsQuaternion(float _w, float _x, float _y, float _z) : gkQuaternion(_w, _x, _y, _z) {}
	gsQuaternion(const gsQuaternion& rhs) : gkQuaternion(rhs.w, rhs.x, rhs.y, rhs.z) {}


	const char* __str__(void)
	{
		static char buffer[72];
		sprintf(buffer, "%f, %f, %f, %f", w, x, y, z);
		return buffer;
	}

	float       normalize(void)                 { return normalise(); }
	float       length2(void)                   { return Norm(); }
	float       dot(const gsQuaternion& v)        { return Dot(v); }
	gsQuaternion  inverse(void)                   { return Inverse(); }
	gsQuaternion operator- (void)                 { return -(gkQuaternion) * this; }
	gsQuaternion operator+ (const gsQuaternion& v)  { return ((const gkQuaternion&) * this).operator + (v); }
	gsQuaternion operator- (const gsQuaternion& v)  { return ((const gkQuaternion&) * this).operator - (v); }
	gsQuaternion operator* (const gsQuaternion& v)  { return ((const gkQuaternion&) * this).operator * (v); }
	gsQuaternion operator* (float v)              { return ((const gkQuaternion&) * this).operator * (v); }
	gsVector3    operator* (const gsVector3& v)     { return ((const gkQuaternion&) * this).operator * (v); }
};


class gsRay
#ifndef SWIG
	: public Ogre::Ray
#endif
{
public:
	gsRay() {}
	gsRay(const gsVector3& origin, const gsVector3& direction) : Ogre::Ray(origin, direction) {}

	void setOrigin(const gsVector3& origin)       {Ogre::Ray::setOrigin(origin);}
	void setDirection(const gsVector3& origin)    {Ogre::Ray::setDirection(origin);}

	gsVector3 getOrigin()       {return Ogre::Ray::getOrigin();}
	gsVector3 getDirection()    {return Ogre::Ray::getDirection();}
};




inline gsVector3 gsVec3Negate(const gsVector3& in)
{
	return -(gkVector3)in;
}



inline gsVector3 gsVec3AddVec3(const gsVector3& a, const gsVector3& b)
{
	return (gkVector3)a + (gkVector3)b;
}



inline gsVector3 gsVec3SubVec3(const gsVector3& a, const gsVector3& b)
{
	return (gkVector3)a - (gkVector3)b;
}



inline gsVector3 gsVec3MulVec3(const gsVector3& a, const gsVector3& b)
{
	return (gkVector3)a / (gkVector3)b;
}



inline gsVector3 gsVec3DivVec3(const gsVector3& a, const gsVector3& b)
{
	return (gkVector3)a / (gkVector3)b;
}



inline gsVector3 gsVec3Mulf(const gsVector3& a, const float& b)
{
	return (gkVector3)a / b;
}



inline gsVector3 gsVec3Divf(const gsVector3& a, const float& b)
{
	return (gkVector3)a / b;
}



inline gsQuaternion gsQuatNegate(const gsQuaternion& in)
{
	return -(gkQuaternion)in;
}



inline gsQuaternion gsQuatAddQuat(const gsQuaternion& a, const gsQuaternion& b)
{
	return (gkQuaternion)a + (gkQuaternion)b;
}



inline gsQuaternion gsQuatSubQuat(const gsQuaternion& a, const gsQuaternion& b)
{
	return (gkQuaternion)a - (gkQuaternion)b;
}



inline gsQuaternion gsQuatMulQuat(const gsQuaternion& a, const gsQuaternion& b)
{
	return (gkQuaternion)a * (gkQuaternion)b;
}



inline gsQuaternion gsQuatMulf(const gsQuaternion& a, const float& b)
{
	return (gkQuaternion)a * b;
}



inline gsVector3 gsQuatMulVec3(const gsQuaternion& a, const gsVector3& b)
{
	return (gkQuaternion)a * (gkVector3)b;
}

inline gsQuaternion gsVec3RotationTo(const gsVector3& a, const gsVector3& b) 
{ 
	return ((gkVector3)a).getRotationTo(b); 
}


#endif//_gsMath_h_
