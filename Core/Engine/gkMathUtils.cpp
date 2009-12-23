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
#include "gkMathUtils.h"

#define OGRE_ANGULAR_MOTION_THRESHOLD Real(0.5)*Real(6.283185307179586232 * 0.25)


using namespace Ogre;


// ----------------------------------------------------------------------------
Matrix3 gkMathUtils::getMat3FromEuler(const Vector3& eul, bool isDegree)
{
	Matrix3 rx, ry, rz;

	Radian Rx, Ry, Rz;

	if (gkFuzzyVec(eul))
		return Matrix3::IDENTITY;

	if (isDegree)
	{
		if (!gkFuzzy(eul.x)) Rx= Radian(Degree(eul.x));
		if (!gkFuzzy(eul.y)) Ry= Radian(Degree(eul.y));
		if (!gkFuzzy(eul.z)) Rz= Radian(Degree(eul.z));
	}

	else
	{
		if (!gkFuzzy(eul.x)) Rx= Radian(eul.x);
		if (!gkFuzzy(eul.y)) Ry= Radian(eul.y);
		if (!gkFuzzy(eul.z)) Rz= Radian(eul.z);
	}

	if (!gkFuzzy(eul.x)) rx.FromAxisAngle(Vector3::UNIT_X, Rx);
	else rx= Matrix3::IDENTITY;

	if (!gkFuzzy(eul.y)) ry.FromAxisAngle(Vector3::UNIT_Y, Ry);
	else ry= Matrix3::IDENTITY;

	if (!gkFuzzy(eul.z)) rz.FromAxisAngle(Vector3::UNIT_Z, Rz);
	else rz= Matrix3::IDENTITY;

	return Matrix3(rz*ry*rx);
}

// ----------------------------------------------------------------------------
Quaternion gkMathUtils::getQuatFromEuler(const Vector3 &eul, bool isDegree)
{
	if (gkFuzzyVec(eul))
		return Quaternion::IDENTITY;

	return Quaternion(getMat3FromEuler(eul, isDegree));
}


// ----------------------------------------------------------------------------
Vector3 gkMathUtils::getEulerFromQuat(const Quaternion& q, bool toDegree)
{
	Matrix3 matr;
	q.ToRotationMatrix(matr);

	Real x, y, z; Radian rx, ry, rz;
	matr.ToEulerAnglesZYX(rz, ry, rx);

	if (toDegree)
	{
		x= rx.valueDegrees();
		y= ry.valueDegrees();
		z= rz.valueDegrees();
	}
	else
	{
		x= rx.valueRadians();
		y= ry.valueRadians();
		z= rz.valueRadians();
	}

	if (gkFuzzy(x)) x= 0.0;
	if (gkFuzzy(y)) y= 0.0;
	if (gkFuzzy(z)) z= 0.0;

	return Vector3(x, y, z);
}

// ----------------------------------------------------------------------------
Matrix4 gkMathUtils::getFromFloat(const float m[][4])
{
	/// convert a Blender4x4 matrix to Ogre4x4
	return Matrix4(
			   m[0][0], m[1][0], m[2][0], m[3][0],
			   m[0][1], m[1][1], m[2][1], m[3][1],
			   m[0][2], m[1][2], m[2][2], m[3][2],
			   m[0][3], m[1][3], m[2][3], m[3][3]
		   );
}

// ----------------------------------------------------------------------------
Matrix4 gkMathUtils::getFromFloat3(const float m[][3])
{
	/// convert a Blender3x3 matrix to Ogre4x4
	return Matrix4(
			   m[0][0], m[1][0], m[2][0], 0.0,
			   m[0][1], m[1][1], m[2][1], 0.0,
			   m[0][2], m[1][2], m[2][2], 0.0,
			   0.0,	 0.0,	 0.0,	 1.0
		   );
}

// ----------------------------------------------------------------------------
void gkMathUtils::extractLocEulScale(const Matrix4 &m, Vector3 &loc, Vector3 &eul, Vector3 &scale, bool inDegrees)
{
	// extract translation
	loc= m.getTrans();

	// extract rotation
	Matrix3 matr;
	m.extract3x3Matrix(matr);

	Quaternion rot(matr);
	rot.normalise();
	eul= getEulerFromQuat(rot, inDegrees);

	// extract scale
	Matrix4 invRot(rot.Inverse());
	Matrix4 t= invRot * m;
	scale= Vector3(t[0][0], t[1][1], t[2][2]);
}


// ----------------------------------------------------------------------------
void gkMathUtils::makeLocEulScale(Matrix4 &dest, const Vector3& loc, const Vector3& eul, const Vector3 &scale, bool inDegrees)
{
	dest.makeTransform(loc, scale, getQuatFromEuler(eul, inDegrees));
}

// ----------------------------------------------------------------------------
void gkMathUtils::makeLocQuatScale(Matrix4 &dest, const Vector3& loc, const Quaternion& q, const Vector3 &scale)
{
	dest.makeTransform(loc, scale, q);
}

// ----------------------------------------------------------------------------
Quaternion gkMathUtils::interp(const Quaternion &a, const Quaternion &b, Real t, bool fast)
{
	return fast ?	Quaternion::nlerp(t, a, b, a.Dot(b) < 0.0) :
					Quaternion::Slerp(t, a, b, a.Dot(b) < 0.0);
}

// ----------------------------------------------------------------------------
Vector3 gkMathUtils::interp(const Vector3 &a, const Vector3 &b, Real t)
{
	Real s= 1.0 - t;
	Vector3 ret;
	ret.x= s * a.x + t * b.x;
	ret.y= s * a.y + t * b.y;
	ret.z= s * a.z + t * b.z;
	return ret;
}

// ----------------------------------------------------------------------------
Vector2 gkMathUtils::interp(const Vector2 &a, const Vector2 &b, Real t)
{
	Real s= 1.0 - t;
	Vector2 ret;
	ret.x= s * a.x + t * b.x;
	ret.y= s * a.y + t * b.y;
	return ret;
}

// ----------------------------------------------------------------------------
Real gkMathUtils::interp(const Real &a, const Real &b, Real t)
{
	Real s= 1.0 - t;
	return s * a + t * b;
}

// ----------------------------------------------------------------------------
void gkMathUtils::extractTransform(const Matrix4& m, Vector3 &loc, Quaternion &quat, Vector3 &scale)
{
	// extract translation
	loc= m.getTrans();

	// extract rotation
	Matrix3 matr;
	m.extract3x3Matrix(matr);
	matr.Orthonormalize();

	Quaternion rot(matr);

	if (rot.Norm() != 1.0)
		rot.normalise();
	quat= rot;

	// extract scale
	Matrix4 invRot(rot.Inverse());
	Matrix4 T= invRot * m;
	scale= Vector3(T[0][0], T[1][1], T[2][2]);
}

// ----------------------------------------------------------------------------
void gkMathUtils::getBlendMatrix(Matrix4& dest, const Matrix4 &A, const Matrix4 &B, Real fact)
{
	/// Blending between matrices
	Vector3 aloc, ascale, bloc, bscale;
	Quaternion aquat, bquat;
	extractTransform(A, aloc, aquat, ascale);
	extractTransform(B, bloc, bquat, bscale);

	Vector3 dloc= interp(aloc, bloc, fact);
	Quaternion drot= interp(aquat, bquat, fact);
	Vector3 dscale= interp(ascale, bscale, fact);

	makeLocQuatScale(dest, dloc, drot, dscale);
}

// ----------------------------------------------------------------------------
void gkMathUtils::extractMatrix(const float m[][4], Vector3 &loc, Quaternion &quat, Vector3 &scale)
{
	/// Used for extracting transforms from a Blender4x4 matrix
	Matrix4 btomat= getFromFloat(m);

	// extract translation
	loc= btomat.getTrans();

	// extract rotation
	Matrix3 matr;
	btomat.extract3x3Matrix(matr);
	matr.Orthonormalize();

	Quaternion rot(matr);
	if (rot.Norm() != 1.0)
		rot.normalise();
	quat= rot;

	// extract scale
	Matrix4 invRot(rot.Inverse());
	btomat= invRot * btomat;
	scale= Vector3(btomat[0][0], btomat[1][1], btomat[2][2]);
}

// ----------------------------------------------------------------------------
Vector3 gkMathUtils::calculateLinearVelocity(const Vector3& from, const Vector3& to, Real timeStep)
{
	// Adapted from bullet
	return (to - from) / timeStep;
}

// ----------------------------------------------------------------------------
Vector3 gkMathUtils::calculateAngularVelocity(const Quaternion& from, const Quaternion& to, Real timeStep)
{
	// Adapted from bullet

	Matrix3 t0, t1;
	to.ToRotationMatrix(t1);
	from.ToRotationMatrix(t0);

	Quaternion dorn= Quaternion(t1 * t0.Inverse());
	dorn.normalise();

	Real angle= Real(2.0) * acos(dorn.w);
	Vector3 axis;
	axis.x= dorn.x;
	axis.y= dorn.y;
	axis.z= dorn.z;


	Real len= axis.squaredLength();
	if (len < GK_EPSILON * GK_EPSILON)
		axis= Vector3::UNIT_X;
	else
		axis /= Math::Sqrt(len);

	return axis * angle / timeStep;
}

// ----------------------------------------------------------------------------
Quaternion gkMathUtils::calculateRotation(const Quaternion& from, const Vector3& angvel, Real timeStep)
{
	// Adapted from bullet
	Vector3 axis;
	Real fAngle= angvel.length();

	if (fAngle*timeStep > OGRE_ANGULAR_MOTION_THRESHOLD)
		fAngle= OGRE_ANGULAR_MOTION_THRESHOLD / timeStep;

	if (fAngle < Real(0.001))
		axis= angvel*(Real(0.5)*timeStep-(timeStep*timeStep*timeStep)*(Real(0.020833333333))*fAngle*fAngle);
	else
		axis= angvel*(Math::Sin(Real(0.5)*fAngle*timeStep)/fAngle);

	Quaternion dorn(Math::Cos(fAngle*timeStep*Real(0.5)), axis.x, axis.y, axis.z);
	Quaternion predictedOrn= dorn * from;
	predictedOrn.normalise();
	return predictedOrn;
}

// ----------------------------------------------------------------------------
Quaternion gkMathUtils::getDiffQuat(const Quaternion &from, const Quaternion &to)
{
	Matrix3 t0, t1;
	to.ToRotationMatrix(t1);
	from.ToRotationMatrix(t0);

	Quaternion dorn= Quaternion(t1 * t0.Inverse());
	dorn.normalise();
	return dorn;
}


// ----------------------------------------------------------------------------
void gkMathUtils::extractTransform2(const Matrix4& m, Vector3 &loc, Quaternion &quat, Vector3 &scale)
{
	loc= m.getTrans();
	quat= m.extractQuaternion();

	quat.normalise();

	Matrix4 invRot(quat.Inverse());
	Matrix4 T= invRot * m;
	scale= Vector3(T[0][0], T[1][1], T[2][2]);
}
