/*
-------------------------------------------------------------------------------
	This file is part of the Ogre GameKit port.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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

#include "OgreVector3.h"
#include "OgreVector2.h"
#include "OgreMatrix3.h"
#include "OgreMatrix4.h"
#include "OgreMath.h"
#include "OgreQuaternion.h"
#include "OgreColourValue.h"
#include "OgreRectangle.h"
#include "gkRect.h"

#include <float.h>

#if OGRE_DOUBLE_PRECISION == 1
# define GK_EPSILON   DBL_EPSILON
# define GK_INFINITY  DBL_MAX
#else
# define GK_EPSILON   FLT_EPSILON
# define GK_INFINITY  FLT_MAX
#endif

// ----------------------------------------------------------------------------
// Rotation in degrees true, else false
#define GK_ROTUNIT true

#if OGRE_COMPILER == OGRE_COMPILER_MSVC
# define gkNan(n)	_isnan((n))
# define gkFinite(n) _finite((n))
#else
# define gkNan(n)	isnan((n))
# define gkFinite(n) finite((n))
#endif

#define gkPi  Ogre::Real(3.141592653589793238)
#define gkPi2 Ogre::Real(6.283185307179586476)
#define gkPih Ogre::Real(1.570796326794896619)
#define gkRPD Ogre::Real(0.017453292519943295)
#define gkDPR Ogre::Real(57.295779513082320876)

#define gkMax( lhs, rhs )		((lhs) < (rhs) ? (rhs) : (lhs))
#define gkMin( lhs, rhs )		((rhs) < (lhs) ? (rhs) : (lhs))
#define gkClamp( x, mi, ma )	((x) <  (mi) ?  (mi)  : (x) > (ma) ? (ma) : (x))
#define gkAbs( x )				((x)  < ( 0 ) ? -(x) : (x))
#define gkFuzzy( x )			(gkAbs( x ) < GK_EPSILON)
#define gkFuzzyT( x, t)			(gkAbs( x ) < t)
#define gkDegrees( x )			(Ogre::Real(x) * gkDPR)
#define gkRadians( x )			(Ogre::Real(x) * gkRPD)
#define gkIsInf( x )			(gkAbs( x ) >= GK_INFINITY)
#define gkFuzzyEq(x, y)			(gkFuzzy((x)- (y)))
#define gkFuzzyEqT(x, y, t)		(gkFuzzyT( (x)- (y), (t)))
#define gkFuzzyVec(v)			(gkFuzzy(v.squaredLength()))



// ----------------------------------------------------------------------------
class gkMathUtils
{
public:

	static Ogre::Matrix3 getMat3FromEuler(const Ogre::Vector3& eul, bool isDegree= GK_ROTUNIT);
	static Ogre::Quaternion getQuatFromEuler(const Ogre::Vector3 &eul, bool isDegree= GK_ROTUNIT);
	static Ogre::Vector3 getEulerFromQuat(const Ogre::Quaternion& q, bool toDegree= GK_ROTUNIT);

	/// for blender matrices (transpose)
	static Ogre::Matrix4 getFromFloat(const float m[][4]);
	static Ogre::Matrix4 getFromFloat3(const float m[][3]);


	static void extractLocEulScale(const Ogre::Matrix4 &m, Ogre::Vector3 &loc, Ogre::Vector3 &eul, Ogre::Vector3 &scale, bool inDegrees= GK_ROTUNIT);
	static void makeLocEulScale(Ogre::Matrix4 &dest, const Ogre::Vector3& loc, const Ogre::Vector3& eul, const Ogre::Vector3 &scale, bool inDegrees= GK_ROTUNIT);
	static void makeLocQuatScale(Ogre::Matrix4 &dest, const Ogre::Vector3& loc, const Ogre::Quaternion& q, const Ogre::Vector3 &scale);

	static Ogre::Quaternion interp(const Ogre::Quaternion &a, const Ogre::Quaternion &b, Ogre::Real t);
	static Ogre::Vector2 interp(const Ogre::Vector2 &a, const Ogre::Vector2 &b, Ogre::Real t);
	static Ogre::Vector3 interp(const Ogre::Vector3 &a, const Ogre::Vector3 &b, Ogre::Real t);
	static Ogre::Real interp(const Ogre::Real &a, const Ogre::Real &b, Ogre::Real t);


	static Ogre::Quaternion getDiffQuat(const Ogre::Quaternion &from, const Ogre::Quaternion &to);

	static void extractTransform2(const Ogre::Matrix4& m, Ogre::Vector3 &loc, Ogre::Quaternion &quat, Ogre::Vector3 &scale);
	static void extractTransform(const Ogre::Matrix4& m, Ogre::Vector3 &loc, Ogre::Quaternion &quat, Ogre::Vector3 &scale);
	static void getBlendMatrix(Ogre::Matrix4& dest, const Ogre::Matrix4 &A, const Ogre::Matrix4 &B, Ogre::Real fact);
	static void extractMatrix(const float m[][4], Ogre::Vector3 &loc, Ogre::Quaternion &quat, Ogre::Vector3 &scale);

	// adapted from bullet
	static Ogre::Vector3 calculateLinearVelocity(const Ogre::Vector3& from, const Ogre::Vector3& to, Ogre::Real timeStep);
	static Ogre::Vector3 calculateAngularVelocity(const Ogre::Quaternion& from, const Ogre::Quaternion& to, Ogre::Real timeStep);
	static Ogre::Quaternion calculateRotation(const Ogre::Quaternion& cur, const Ogre::Vector3& angular, Ogre::Real timeStep);
};





#endif//_gkMathUtils_h_
