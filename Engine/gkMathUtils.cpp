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
#include "gkMathUtils.h"


gkMatrix3 gkMathUtils::getMat3FromEuler(const gkVector3& eul, bool isDegree)
{
	gkMatrix3 rx, ry, rz;

	gkRadian Rx, Ry, Rz;

	if (gkFuzzyVec(eul))
		return gkMatrix3::IDENTITY;

	if (isDegree)
	{
		if (!gkFuzzy(eul.x)) Rx = gkRadian(gkDegree(eul.x));
		if (!gkFuzzy(eul.y)) Ry = gkRadian(gkDegree(eul.y));
		if (!gkFuzzy(eul.z)) Rz = gkRadian(gkDegree(eul.z));
	}

	else
	{
		if (!gkFuzzy(eul.x)) Rx = gkRadian(eul.x);
		if (!gkFuzzy(eul.y)) Ry = gkRadian(eul.y);
		if (!gkFuzzy(eul.z)) Rz = gkRadian(eul.z);
	}

	if (!gkFuzzy(eul.x)) rx.FromAxisAngle(gkVector3::UNIT_X, Rx);
	else rx = gkMatrix3::IDENTITY;

	if (!gkFuzzy(eul.y)) ry.FromAxisAngle(gkVector3::UNIT_Y, Ry);
	else ry = gkMatrix3::IDENTITY;

	if (!gkFuzzy(eul.z)) rz.FromAxisAngle(gkVector3::UNIT_Z, Rz);
	else rz = gkMatrix3::IDENTITY;

	return gkMatrix3(rz * ry * rx);
}


gkQuaternion gkMathUtils::getQuatFromEuler(const gkVector3& eul, bool isDegree)
{
	if (gkFuzzyVec(eul))
		return gkQuaternion::IDENTITY;

	return gkQuaternion(getMat3FromEuler(eul, isDegree));
}



gkVector3 gkMathUtils::getEulerFromQuat(const gkQuaternion& q, bool toDegree)
{
	gkMatrix3 matr;
	q.ToRotationMatrix(matr);

	gkScalar x, y, z; gkRadian rx, ry, rz;
	matr.ToEulerAnglesZYX(rz, ry, rx);

	if (toDegree)
	{
		x = rx.valueDegrees();
		y = ry.valueDegrees();
		z = rz.valueDegrees();
	}
	else
	{
		x = rx.valueRadians();
		y = ry.valueRadians();
		z = rz.valueRadians();
	}

	if (gkFuzzy(x)) x = 0.0;
	if (gkFuzzy(y)) y = 0.0;
	if (gkFuzzy(z)) z = 0.0;

	return gkVector3(x, y, z);
}


gkMatrix4 gkMathUtils::getFromFloat(const float m[][4])
{
	// convert a Blender4x4 matrix to Ogre4x4
	return gkMatrix4(
	           m[0][0], m[1][0], m[2][0], m[3][0],
	           m[0][1], m[1][1], m[2][1], m[3][1],
	           m[0][2], m[1][2], m[2][2], m[3][2],
	           m[0][3], m[1][3], m[2][3], m[3][3]
	       );
}


gkMatrix4 gkMathUtils::getFromFloat3(const float m[][3])
{
	// convert a Blender3x3 matrix to Ogre4x4
	return gkMatrix4(
	           m[0][0], m[1][0], m[2][0], 0.0,
	           m[0][1], m[1][1], m[2][1], 0.0,
	           m[0][2], m[1][2], m[2][2], 0.0,
	           0.0,     0.0,     0.0,     1.0
	       );
}


gkMatrix4 gkMathUtils::getFromFloatNorm(const float m[][4])
{
	gkVector3 l, s;
	gkQuaternion r;
	extractMatrix(m, l, r, s);

	gkMatrix4 ret;
	ret.makeTransform(l, s, r);
	return ret;
}


void gkMathUtils::extractLocEulScale(const gkMatrix4& m, gkVector3& loc, gkVector3& eul, gkVector3& scale, bool inDegrees)
{
	// extract translation
	loc = m.getTrans();

	// extract rotation
	gkMatrix3 matr;
	m.extract3x3Matrix(matr);

	gkQuaternion rot(matr);
	rot.normalise();
	eul = getEulerFromQuat(rot, inDegrees);

	// extract scale
	gkMatrix4 invRot(rot.Inverse());
	gkMatrix4 t = invRot * m;
	scale = gkVector3(t[0][0], t[1][1], t[2][2]);
}



void gkMathUtils::makeLocEulScale(gkMatrix4& dest, const gkVector3& loc, const gkVector3& eul, const gkVector3& scale, bool inDegrees)
{
	dest.makeTransform(loc, scale, getQuatFromEuler(eul, inDegrees));
}


void gkMathUtils::makeLocQuatScale(gkMatrix4& dest, const gkVector3& loc, const gkQuaternion& q, const gkVector3& scale)
{
	dest.makeTransform(loc, scale, q);
}

gkQuaternion gkMathUtils::interp(const gkQuaternion& a, const gkQuaternion& b, gkScalar t, bool fast)
{
	return fast ? gkQuaternion::nlerp(t, a, b, a.Dot(b) < 0.0) : gkQuaternion::Slerp(t, a, b, a.Dot(b) < 0.0);
}


gkVector3 gkMathUtils::interp(const gkVector3& a, const gkVector3& b, gkScalar t)
{
	gkScalar s = 1.f - t;
	gkVector3 ret;
	ret.x = s * a.x + t * b.x;
	ret.y = s * a.y + t * b.y;
	ret.z = s * a.z + t * b.z;
	return ret;
}


gkVector2 gkMathUtils::interp(const gkVector2& a, const gkVector2& b, gkScalar t)
{
	gkScalar s = 1.f - t;
	gkVector2 ret;
	ret.x = s * a.x + t * b.x;
	ret.y = s * a.y + t * b.y;
	return ret;
}


gkScalar gkMathUtils::interp(const gkScalar& a, const gkScalar& b, gkScalar t)
{
	gkScalar s = 1.f - t;
	return s * a + t * b;
}


void gkMathUtils::extractTransform(const gkMatrix4& m, gkVector3& loc, gkQuaternion& quat, gkVector3& scale)
{
	// extract translation
	loc = m.getTrans();

	// extract rotation
	gkMatrix3 matr;
	m.extract3x3Matrix(matr);
	matr.Orthonormalize();

	gkQuaternion rot(matr);
	if (quat.Norm() == 0.0)
		rot = gkQuaternion::IDENTITY;
	quat = rot;

	// extract scale
	gkMatrix4 invRot(rot.Inverse());
	gkMatrix4 T = invRot * m;
	scale = gkVector3(T[0][0], T[1][1], T[2][2]);
}



void gkMathUtils::extractTransformFast(const gkMatrix4& m, gkVector3& loc, gkQuaternion& quat, gkVector3& scale, bool getScale)
{
	// extract translation
	loc = m.getTrans();
	quat = m.extractQuaternion();

	if (getScale)
	{
		// extract scale
		gkMatrix4 invRot(quat.Inverse());
		gkMatrix4 T = invRot * m;
		scale = gkVector3(T[0][0], T[1][1], T[2][2]);
	}
	else scale = gkVector3::UNIT_SCALE;
}



void gkMathUtils::blendMatrixFast(gkMatrix4& dest, const gkMatrix4& A, const gkMatrix4& B, gkScalar fact, bool fastRot, bool getScale)
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

	gkVector3 aloc, ascale, bloc, bscale;
	gkQuaternion aquat, bquat;

	extractTransformFast(A, aloc, aquat, ascale, getScale);
	extractTransformFast(B, bloc, bquat, bscale, getScale);

	gkVector3 dloc = bloc, dscale = bscale;
	gkQuaternion drot = bquat;

	if (aloc != bloc)
		dloc = interp(aloc, bloc, fact);

	if (aquat != bquat)
		drot = interp(aquat, bquat, fact, fastRot);

	if (ascale != bscale)
		dscale = interp(ascale, bscale, fact);

	dest.makeTransform(dloc, dscale, drot);
}


void gkMathUtils::extractMatrix(const float m[][4], gkVector3& loc, gkQuaternion& quat, gkVector3& scale)
{
	// Used for extracting transforms from a Blender4x4 matrix
	gkMatrix4 btomat = getFromFloat(m);

	// extract translation
	loc = btomat.getTrans();

	// extract rotation
	gkMatrix3 matr;
	btomat.extract3x3Matrix(matr);
	matr.Orthonormalize();

	gkQuaternion rot(matr);
	if (rot.Norm() != 1.0)
		rot.normalise();
	quat = rot;

	// extract scale
	gkMatrix4 invRot(rot.Inverse());
	btomat = invRot * btomat;
	scale = gkVector3(btomat[0][0], btomat[1][1], btomat[2][2]);
}
