/*
-------------------------------------------------------------------------------
--                                                                           --
--  This file is part of GameKit.                                            --
--  http://gamekit.googlecode.com/                                           --
--                                                                           --
--                                                                           --
--  Released under the zlib license.                                         --
--  http://www.gzip.org/zlib/zlib_license.html                               --
--                                                                           --
--                                                                           --
-- Generic Script API Binding                                                --
-------------------------------------------------------------------------------
*/
#ifndef _gksMath_h_
#define _gksMath_h_

#include "gkMathUtils.h"

// ----------------------------------------------------------------------------
class gksVector3
#ifndef SWIG
	: public gkVector3
#endif
{
public:

#ifdef SWIG
	float x, y, z;
#else
	gksVector3(const gkVector3 &rhs) : gkVector3(rhs.x, rhs.y, rhs.z) {}
#endif
	gksVector3() : gkVector3(0,0,0) {}
	gksVector3(float _x, float _y, float _z) : gkVector3(_x,_y,_z) {}
	gksVector3(const gksVector3 &rhs) : gkVector3(rhs.x, rhs.y, rhs.z) {}

	const char *__str__(void)
	{
		static char buffer[72];
		sprintf(buffer, "%f, %f, %f", x, y, z);
		return buffer;
	}

	float       normalize(void)                 { return normalise(); }
	float       dot(const gksVector3 &v)        { return dotProduct(v); }
	gksVector3  cross(const gksVector3 &v)      { return gksVector3(crossProduct(v));}
	float       length(void)                    { return gkVector3::length(); }
	float       length2(void)                   { return gkVector3::squaredLength(); }
	float       distance(const gksVector3 &v)   { return gkVector3::distance(v); }
	float       distance2(const gksVector3 &v)  { return gkVector3::squaredDistance(v); }

	gksVector3 operator- (void)                 { return -(gkVector3)*this; }
	gksVector3 operator+ (const gksVector3 &v)  { return ((const gkVector3 &)*this).operator+(v); }
	gksVector3 operator- (const gksVector3 &v)  { return ((const gkVector3 &)*this).operator-(v); }
	gksVector3 operator* (const gksVector3 &v)  { return ((const gkVector3 &)*this).operator*(v); }
	gksVector3 operator* (float v)              { return ((const gkVector3 &)*this).operator*(v); }
	gksVector3 operator/ (const gksVector3 &v)  { return ((const gkVector3 &)*this).operator/(v); }
	gksVector3 operator/ (float v)              { return ((const gkVector3 &)*this).operator/(v); }
};

// ----------------------------------------------------------------------------
class gksQuaternion
#ifndef SWIG
	: public gkQuaternion
#endif
{
public:

#ifdef SWIG
	float w, x, y, z;
#else
	gksQuaternion(const gkQuaternion &rhs) : gkQuaternion(rhs.w, rhs.x, rhs.y, rhs.z) {}
#endif
	gksQuaternion() : gkQuaternion(1, 0,0,0) {}
	gksQuaternion(float _w, float _x, float _y, float _z) : gkQuaternion(_w, _x,_y,_z) {}
	gksQuaternion(const gksQuaternion &rhs) : gkQuaternion(rhs.w, rhs.x, rhs.y, rhs.z) {}


	const char *__str__(void)
	{
		static char buffer[72];
		sprintf(buffer, "%f, %f, %f, %f", w, x, y, z);
		return buffer;
	}

	float       normalize(void)             { return normalise(); }
	float       length2(void)               { return Norm(); }
	float       dot(const gksQuaternion &v) { return Dot(v); }
	gksQuaternion  inverse(void)            { return Inverse(); }

	gksQuaternion operator- (void)                     { return -(gkQuaternion)*this; }
	gksQuaternion operator+ (const gksQuaternion &v)   { return ((const gkQuaternion &)*this).operator+(v); }
	gksQuaternion operator- (const gksQuaternion &v)   { return ((const gkQuaternion &)*this).operator-(v); }
	gksQuaternion operator* (const gksQuaternion &v)   { return ((const gkQuaternion &)*this).operator*(v); }
	gksQuaternion operator* (float v)                  { return ((const gkQuaternion &)*this).operator*(v); }
	gksVector3    operator* (const gksVector3 &v)      { return ((const gkQuaternion &)*this).operator*(v); }
};


#ifdef SWIG_V8
// no operator support in V8
inline gksVector3 Vec3Negate(const gksVector3 &in)
{
	return -(gkVector3)in;
}
inline gksVector3 Vec3AddVec3(const gksVector3 &a, const gksVector3 &b)
{
	return (gkVector3)a + (gkVector3)b;
}
inline gksVector3 Vec3SubVec3(const gksVector3 &a, const gksVector3 &b)
{
	return (gkVector3)a - (gkVector3)b;
}
inline gksVector3 Vec3MulVec3(const gksVector3 &a, const gksVector3 &b)
{
	return (gkVector3)a / (gkVector3)b;
}
inline gksVector3 Vec3DivVec3(const gksVector3 &a, const gksVector3 &b)
{
	return (gkVector3)a / (gkVector3)b;
}
inline gksVector3 Vec3Mulf(const gksVector3 &a, const float &b)
{
	return (gkVector3)a / b;
}
inline gksVector3 Vec3Divf(const gksVector3 &a, const float &b)
{
	return (gkVector3)a / b;
}
inline gksQuaternion QuatNegate(const gksQuaternion &in)
{
	return -(gkQuaternion)in;
}
inline gksQuaternion QuatAddQuat(const gksQuaternion &a, const gksQuaternion &b)
{
	return (gkQuaternion)a + (gkQuaternion)b;
}
inline gksQuaternion QuatSubQuat(const gksQuaternion &a, const gksQuaternion &b)
{
	return (gkQuaternion)a - (gkQuaternion)b;
}
inline gksQuaternion QuatMulQuat(const gksQuaternion &a, const gksQuaternion &b)
{
	return (gkQuaternion)a * (gkQuaternion)b;
}
inline gksQuaternion QuatMulf(const gksQuaternion &a, const float &b)
{
	return (gkQuaternion)a * b;
}
inline gksVector3 QuatMulVec3(const gksQuaternion &a, const gksVector3 &b)
{
	return (gkQuaternion)a * (gkVector3)b;
}
#endif


#endif//_gksMath_h_
