/*
-------------------------------------------------------------------------------
	This file is part of OgreKit.
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
#ifndef _OgreMathUtils_h_
#define _OgreMathUtils_h_


#include "OgreIntTypes.h"
#include "OgreVector3.h"
#include "OgreVector2.h"
#include "OgreMatrix3.h"
#include "OgreMatrix4.h"
#include "OgreMath.h"
#include "OgreQuaternion.h"
#include "OgreColourValue.h"
#include "OgreRectangle.h"

#include <float.h>


#if OGRE_DOUBLE_PRECISION == 1
# define OGRE_EPSILON   DBL_EPSILON
# define OGRE_INFINITY  DBL_MAX
#else
# define OGRE_EPSILON   FLT_EPSILON
# define OGRE_INFINITY  FLT_MAX
#endif


#define OgreMax( lhs, rhs )     ((lhs) < (rhs) ? (rhs) : (lhs))
#define OgreMin( lhs, rhs )     ((rhs) < (lhs) ? (rhs) : (lhs))
#define OgreClamp( x, mi, ma )  ((x) <  (mi) ?  (mi)  : (x) > (ma) ? (ma) : (x))
#define OgreAbs( x )            ((x)  < ( 0 ) ? -(x) : (x))
#define OgreFuzzy( x )          (OgreAbs( x ) < OGRE_EPSILON)
#define OgreFuzzyT( x, t)       (OgreAbs( x ) < t)
#define OgreDegrees( x )        (Real(x) * Ogre::Real(57.295779513082320876))
#define OgreRadians( x )        (Real(x) * Ogre::Real(0.017453292519943295))
#define OgreIsInf( x )          (OgreAbs( x ) >= OGRE_INFINITY)
#define OgreFuzzyEq(x, y)       (OgreFuzzy((x)- (y)))
#define OgreFuzzyEqT(x, y, t)   (OgreFuzzyT( (x)- (y), (t)))
#define OgreFuzzyVec(v)         (OgreFuzzy(v.squaredLength()))


namespace Ogre {

    

class MathUtils
{
public:

    static Matrix3 getMat3FromEuler(const Vector3& eul, bool isDegree= true);
    static Quaternion getQuatFromEuler(const Vector3 &eul, bool isDegree= true);
    static Vector3 getEulerFromQuat(const Quaternion& q, bool toDegree= true);

    /// for blender matrices (transpose)
    static Matrix4 getFromFloat(const float m[][4]);
	static Matrix4 getFromFloatNorm(const float m[][4]);
    static Matrix4 getFromFloat3(const float m[][3]);


    static void extractLocEulScale(const Matrix4 &m, Vector3 &loc, Vector3 &eul, Vector3 &scale, bool inDegrees= true);
    static void makeLocEulScale(Matrix4 &dest, const Vector3& loc, const Vector3& eul, const Vector3 &scale, bool inDegrees= true);
    static void makeLocQuatScale(Matrix4 &dest, const Vector3& loc, const Quaternion& q, const Vector3 &scale);

    static Quaternion interp(const Quaternion &a, const Quaternion &b, Real t, bool fast=true);
    static Vector2 interp(const Vector2 &a, const Vector2 &b, Real t);
    static Vector3 interp(const Vector3 &a, const Vector3 &b, Real t);
    static Real interp(const Real &a, const Real &b, Real t);

    static void extractTransform(const Matrix4& m, Vector3 &loc, Quaternion &quat, Vector3 &scale);
    static void extractTransformFast(const Matrix4& m, Vector3 &loc, Quaternion &quat, Vector3 &scale, bool getScale= true);
	static void blendMatrixFast(Matrix4& dest, const Matrix4 &A, const Matrix4 &B, Real fact, bool fastRot=true, bool getScale=true);

	static void extractMatrix(const float m[][4], Vector3 &loc, Quaternion &quat, Vector3 &scale);
};



}


#endif//_OgreMathUtils_h_
