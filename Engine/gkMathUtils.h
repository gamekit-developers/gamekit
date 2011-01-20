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
#ifndef _gkMathUtils_h_
#define _gkMathUtils_h_

#include "gkCommon.h"
#include "LinearMath/btTransform.h"
#include "OgreVector3.h"
#include "OgreVector2.h"
#include "OgreMatrix3.h"
#include "OgreMatrix4.h"
#include "OgreRay.h"
#include "OgreMath.h"
#include "OgreQuaternion.h"
#include "OgreColourValue.h"
#include "OgreRectangle.h"
#include "OgreAxisAlignedBox.h"

#include <float.h>

#if OGRE_DOUBLE_PRECISION == 1
# define GK_EPSILON   DBL_EPSILON
# define GK_INFINITY  DBL_MAX
#else
# define GK_EPSILON   FLT_EPSILON
# define GK_INFINITY  FLT_MAX
# pragma warning(disable :4305)
# pragma warning(disable :4244)
#endif

// Rotation in degrees true, else false
#define GK_ROTUNIT true


// Reusing math functions for simplicity
typedef Ogre::Math              gkMath;
typedef Ogre::Real              gkScalar;
typedef Ogre::Degree            gkDegree;
typedef Ogre::Radian            gkRadian;
typedef Ogre::Vector2           gkVector2;
typedef Ogre::Vector3           gkVector3;
typedef Ogre::Vector4           gkVector4;
typedef Ogre::Quaternion        gkQuaternion;
typedef Ogre::Matrix3           gkMatrix3;
typedef Ogre::Matrix4           gkMatrix4;
typedef Ogre::ColourValue       gkColor;
typedef Ogre::AxisAlignedBox    gkBoundingBox;
typedef Ogre::Ray               gkRay;


#define gkPi        gkScalar(3.141592653589793238)
#define gkPi2       gkScalar(6.283185307179586476)
#define gkPih       gkScalar(1.570796326794896619)
#define gkRPD       gkScalar(0.017453292519943295)
#define gkDPR       gkScalar(57.295779513082320876)


GK_INLINE gkScalar  gkAbs(const gkScalar& v)                                { return v < gkScalar(0.0) ? -v : v;}
GK_INLINE bool      gkNan(const gkScalar& v)                                { return v != v;}
GK_INLINE bool      gkFinite(const gkScalar& v)                             { return gkAbs(v) < GK_INFINITY;}
GK_INLINE bool      gkFuzzy(const gkScalar& v)                              { return gkAbs(v) < GK_EPSILON;}
GK_INLINE bool      gkFuzzyT(const gkScalar& v, const gkScalar& tol)        { return gkAbs(v) < tol;}
GK_INLINE bool      gkFuzzyEq(const gkScalar& a, const gkScalar& b)         { return gkAbs(a - b) < GK_EPSILON;}
GK_INLINE bool      gkIsInf(const gkScalar& v)                              { return gkAbs(v) >= GK_INFINITY; }
GK_INLINE bool      gkFuzzyVec(const gkVector2& v)                          { return gkFuzzy(v.squaredLength());}
GK_INLINE bool      gkFuzzyVec(const gkVector3& v)                          { return gkFuzzy(v.squaredLength());}
GK_INLINE gkScalar  gkDegrees(const gkScalar& v)                            { return v * gkDPR;}
GK_INLINE gkScalar  gkRadians(const gkScalar& v)                            { return v * gkRPD;}


// Min / Max utils

template <typename T> GK_INLINE T gkMax(const T& a, const T& b)                         {return a < b ? b : a; }
template <typename T> GK_INLINE T gkMin(const T& a, const T& b)                         {return a < b ? a : b; }
template <typename T> GK_INLINE T gkClamp(const T& v, const T& a, const T& b)           {return v < a ? a : v > b ? b : v; }
GK_INLINE gkScalar gkMaxf(const gkScalar& a, const gkScalar& b)                         {return gkMax<gkScalar>(a, b); }
GK_INLINE gkScalar gkMinf(const gkScalar& a, const gkScalar& b)                         {return gkMin<gkScalar>(a, b); }
GK_INLINE gkScalar gkClampf(const gkScalar& v, const gkScalar& a, const gkScalar& b)    {return gkClamp<gkScalar>(v, a, b); }


class gkEuler
{
public:
	GK_INLINE gkEuler() {}

	GK_INLINE gkEuler(const gkDegree& dx, const gkDegree& dy, const gkDegree& dz)
		:       x(dx.valueDegrees()), y(dy.valueDegrees()), z(dz.valueDegrees())
	{
	}

	GK_INLINE gkEuler(gkScalar dx, gkScalar dy, gkScalar dz)
		:       x(gkDegree(dx)), y(gkDegree(dy)), z(gkDegree(dz))
	{
	}

	GK_INLINE gkEuler(const gkVector3& v)
		:       x(gkDegree(v.x)), y(gkDegree(v.y)), z(gkDegree(v.z))
	{
	}

	GK_INLINE gkEuler(const gkRadian& rx, const gkRadian& ry, const gkRadian& rz)
		:       x(rx.valueDegrees()), y(ry.valueDegrees()), z(rz.valueDegrees())
	{
	}

	GK_INLINE gkEuler(const gkQuaternion& q)
	{
		gkMatrix3 matr;
		q.ToRotationMatrix(matr);
		matr.ToEulerAnglesZYX(z, y, x);
	}

	GK_INLINE gkEuler(const gkMatrix3& v)           { v.ToEulerAnglesZYX(z, y, x); }
	GK_INLINE gkVector3 toVector3(void) const       { return gkVector3(x.valueDegrees(), y.valueDegrees(), z.valueDegrees()); }
	GK_INLINE gkQuaternion toQuaternion(void) const { return gkQuaternion(toMatrix3());}

	GK_INLINE gkMatrix3 toMatrix3(void) const
	{
		gkMatrix3 rx, ry, rz;
		rx.FromAxisAngle(gkVector3::UNIT_X, x);
		ry.FromAxisAngle(gkVector3::UNIT_Y, y);
		rz.FromAxisAngle(gkVector3::UNIT_Z, z);
		return gkMatrix3(rz * ry * rx);
	}

	gkRadian x, y, z;
};


class gkRectangle
{
public:
	gkRectangle() {}
	gkRectangle(gkScalar xv, gkScalar yv, gkScalar wv, gkScalar hv)
	{
		x = xv;
		y = yv;
		width = wv;
		height = hv;
	}
	gkRectangle(const gkRectangle& o) { *this = o; }

	gkRectangle& operator= (const gkRectangle& o)
	{
		x = o.x;
		y = o.y;
		width = o.width;
		height = o.height;
		return *this;
	}
	gkScalar x, y;
	gkScalar width, height;
};




class gkMathUtils
{
public:

	static gkMatrix3 getMat3FromEuler(const gkVector3& eul, bool isDegree = GK_ROTUNIT);
	static gkQuaternion getQuatFromEuler(const gkVector3& eul, bool isDegree = GK_ROTUNIT);
	static gkVector3 getEulerFromQuat(const gkQuaternion& q, bool toDegree = GK_ROTUNIT);

	static gkMatrix4 getFromFloat(const float m[][4]);
	static gkMatrix4 getFromFloat3(const float m[][3]);
	static gkMatrix4 getFromFloatNorm(const float m[][4]);

	static void extractLocEulScale(const gkMatrix4& m, gkVector3& loc, gkVector3& eul, gkVector3& scale, bool inDegrees = true);
	static void makeLocEulScale(gkMatrix4& dest, const gkVector3& loc, const gkVector3& eul, const gkVector3& scale, bool inDegrees = true);
	static void makeLocQuatScale(gkMatrix4& dest, const gkVector3& loc, const gkQuaternion& q, const gkVector3& scale);

	static gkQuaternion interp(const gkQuaternion& a, const gkQuaternion& b, gkScalar t, bool fast = true);
	static gkVector2 interp(const gkVector2& a, const gkVector2& b, gkScalar t);
	static gkVector3 interp(const gkVector3& a, const gkVector3& b, gkScalar t);
	static gkScalar interp(const gkScalar& a, const gkScalar& b, gkScalar t);

	static void extractTransform(const gkMatrix4& m, gkVector3& loc, gkQuaternion& quat, gkVector3& scale);
	static void extractTransformFast(const gkMatrix4& m, gkVector3& loc, gkQuaternion& quat, gkVector3& scale, bool getScale = true);
	static void blendMatrixFast(gkMatrix4& dest, const gkMatrix4& A, const gkMatrix4& B, gkScalar fact, bool fastRot = true, bool getScale = true);

	static void extractMatrix(const float m[][4], gkVector3& loc, gkQuaternion& quat, gkVector3& scale);



	// Bullet converters
	static GK_INLINE gkVector3      get(const btVector3& v)     { return gkVector3(v.x(), v.y(), v.z()); }
	static GK_INLINE btVector3      get(const gkVector3& v)     { return btVector3(v.x, v.y, v.z); }
	static GK_INLINE gkQuaternion   get(const btQuaternion& v)  { return gkQuaternion(v.w(), v.x(), v.y(), v.z()); }
	static GK_INLINE btQuaternion   get(const gkQuaternion& v)  { return btQuaternion(v.x, v.y, v.z, v.w); }
	static GK_INLINE btTransform	get(const gkMatrix3& m)		{ return btTransform(get(gkQuaternion(m))); }
	static GK_INLINE btTransform	get(const gkMatrix4& m)		{ return btTransform(get(m.extractQuaternion()), get(m.getTrans())); }
};





#endif//_gkMathUtils_h_
