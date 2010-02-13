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
#include "OgreMathUtils.h"


using namespace Ogre;



Matrix3 MathUtils::getMat3FromEuler(const Vector3& eul, bool isDegree)
{
	Matrix3 rx, ry, rz;

	Radian Rx, Ry, Rz;

	if (OgreFuzzyVec(eul))
		return Matrix3::IDENTITY;

    if (isDegree)
	{
		if (!OgreFuzzy(eul.x)) Rx= Radian(Degree(eul.x));
		if (!OgreFuzzy(eul.y)) Ry= Radian(Degree(eul.y));
		if (!OgreFuzzy(eul.z)) Rz= Radian(Degree(eul.z));
	}

	else
	{
		if (!OgreFuzzy(eul.x)) Rx= Radian(eul.x);
		if (!OgreFuzzy(eul.y)) Ry= Radian(eul.y);
		if (!OgreFuzzy(eul.z)) Rz= Radian(eul.z);
	}

	if (!OgreFuzzy(eul.x)) rx.FromAxisAngle(Vector3::UNIT_X, Rx);
	else rx= Matrix3::IDENTITY;

	if (!OgreFuzzy(eul.y)) ry.FromAxisAngle(Vector3::UNIT_Y, Ry);
	else ry= Matrix3::IDENTITY;

	if (!OgreFuzzy(eul.z)) rz.FromAxisAngle(Vector3::UNIT_Z, Rz);
	else rz= Matrix3::IDENTITY;

	return Matrix3(rz*ry*rx);
}


Quaternion MathUtils::getQuatFromEuler(const Vector3 &eul, bool isDegree)
{
	if (OgreFuzzyVec(eul))
		return Quaternion::IDENTITY;

	return Quaternion(getMat3FromEuler(eul, isDegree));
}



Vector3 MathUtils::getEulerFromQuat(const Quaternion& q, bool toDegree)
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

	if (OgreFuzzy(x)) x= 0.0;
	if (OgreFuzzy(y)) y= 0.0;
	if (OgreFuzzy(z)) z= 0.0;

	return Vector3(x, y, z);
}


Matrix4 MathUtils::getFromFloat(const float m[][4])
{
	/// convert a Blender4x4 matrix to Ogre4x4
	return Matrix4(
			   m[0][0], m[1][0], m[2][0], m[3][0],
			   m[0][1], m[1][1], m[2][1], m[3][1],
			   m[0][2], m[1][2], m[2][2], m[3][2],
			   m[0][3], m[1][3], m[2][3], m[3][3]
		   );
}


Matrix4 MathUtils::getFromFloat3(const float m[][3])
{
	/// convert a Blender3x3 matrix to Ogre4x4
	return Matrix4(
			   m[0][0], m[1][0], m[2][0], 0.0,
			   m[0][1], m[1][1], m[2][1], 0.0,
			   m[0][2], m[1][2], m[2][2], 0.0,
			   0.0,	 0.0,	 0.0,	 1.0
		   );
}


Matrix4 MathUtils::getFromFloatNorm(const float m[][4])
{
	Vector3 l, s;
	Quaternion r;
	extractMatrix(m, l, r, s);

	Matrix4 ret;
	ret.makeTransform(l, s, r);
	return ret;
}


void MathUtils::extractLocEulScale(const Matrix4 &m, Vector3 &loc, Vector3 &eul, Vector3 &scale, bool inDegrees)
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



void MathUtils::makeLocEulScale(Matrix4 &dest, const Vector3& loc, const Vector3& eul, const Vector3 &scale, bool inDegrees)
{
	dest.makeTransform(loc, scale, getQuatFromEuler(eul, inDegrees));
}


void MathUtils::makeLocQuatScale(Matrix4 &dest, const Vector3& loc, const Quaternion& q, const Vector3 &scale)
{
	dest.makeTransform(loc, scale, q);
}


Quaternion MathUtils::interp(const Quaternion &a, const Quaternion &b, Real t, bool fast)
{
	return fast ?	Quaternion::nlerp(t, a, b, a.Dot(b) < 0.0) :
					Quaternion::Slerp(t, a, b, a.Dot(b) < 0.0);
}


Vector3 MathUtils::interp(const Vector3 &a, const Vector3 &b, Real t)
{
	Real s= 1.f - t;
	Vector3 ret;
	ret.x= s * a.x + t * b.x;
	ret.y= s * a.y + t * b.y;
	ret.z= s * a.z + t * b.z;
	return ret;
}


Vector2 MathUtils::interp(const Vector2 &a, const Vector2 &b, Real t)
{
	Real s= 1.f - t;
	Vector2 ret;
	ret.x= s * a.x + t * b.x;
	ret.y= s * a.y + t * b.y;
	return ret;
}


Real MathUtils::interp(const Real &a, const Real &b, Real t)
{
	Real s= 1.f - t;
	return s * a + t * b;
}


void MathUtils::extractTransform(const Matrix4& m, Vector3 &loc, Quaternion &quat, Vector3 &scale)
{
	// extract translation
	loc= m.getTrans();

	// extract rotation
	Matrix3 matr;
	m.extract3x3Matrix(matr);
	matr.Orthonormalize();

	Quaternion rot(matr);
	if (quat.Norm() == 0.0)
		rot = Quaternion::IDENTITY;
	quat= rot;

	// extract scale
	Matrix4 invRot(rot.Inverse());
	Matrix4 T= invRot * m;
	scale= Vector3(T[0][0], T[1][1], T[2][2]);
}



void MathUtils::extractTransformFast(const Matrix4& m, Vector3 &loc, Quaternion &quat, Vector3 &scale, bool getScale)
{
	// extract translation
	loc = m.getTrans();
	quat = m.extractQuaternion();

	if (getScale)
	{
		// extract scale
		Matrix4 invRot(quat.Inverse());
		Matrix4 T= invRot * m;
		scale= Vector3(T[0][0], T[1][1], T[2][2]);
	}
	else scale = Vector3::UNIT_SCALE;
}



void MathUtils::blendMatrixFast(Matrix4& dest, const Matrix4 &A, const Matrix4 &B, Real fact, bool fastRot, bool getScale)
{
	if (fact <= 0)
	{
		dest = A;
		return;
	}
	if (fact >= 1)
	{
		dest = B;
		return;
	}


	/// Blending between matrices
	Vector3 aloc, ascale, bloc, bscale;
	Quaternion aquat, bquat;

	extractTransformFast(A, aloc, aquat, ascale, getScale);
	extractTransformFast(B, bloc, bquat, bscale, getScale);

	Vector3 dloc=bloc, dscale=bscale;
	Quaternion drot=bquat;

	if (aloc != bloc)
		dloc= interp(aloc, bloc, fact);

	if (aquat != bquat)
		drot= interp(aquat, bquat, fact, fastRot);

	if (ascale != bscale)
		dscale= interp(ascale, bscale, fact);

	dest.makeTransform(dloc, dscale, drot);
}


void MathUtils::extractMatrix(const float m[][4], Vector3 &loc, Quaternion &quat, Vector3 &scale)
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
