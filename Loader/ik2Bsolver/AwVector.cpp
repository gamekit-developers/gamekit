//-
// ==========================================================================
// Copyright 1995,2006,2008 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+
//
//	CLASS:    AwVector
//
// *****************************************************************************
//
//	CLASS DESCRIPTION (AwVector)
//
//	A vector is a mathematical entity used to express the notion of
//	direction and magnitude (not to be confused with a point that expresses
//	location).  The direction is defined by the line from the origin
//	to the vector's x, y, z coordinate.  Vectors can be used to represent
//	translations, in the way that quaternions are used to represent
//	rotations.  When vectors are used to represent a direction, multiplying
//	it by a transformation matrix results in the vector being properly
//	re-oriented by the transformation (eg. a pure translation doesn't affect
//	the direction something is pointing, so a vector remains unchanged under
//	a pure translation).
//
// *****************************************************************************

#include <math.h>
#include <iostream>
#include <AwMath.h>
#include <AwPoint.h>
#include <AwVector.h>
#include <AwMatrix.h>
#include <AwQuaternion.h>

// Initialize constants
//
const AwVector AwVector::zero;
const AwVector AwVector::xAxis(1.0, 0.0, 0.0);
const AwVector AwVector::yAxis(0.0, 1.0, 0.0);
const AwVector AwVector::zAxis(0.0, 0.0, 1.0);

AwVector::AwVector(const AwPoint &pt)
//
//	Description:
//		Convenience method to construct a vector from a point.
//
:	x(pt.x), y(pt.y), z(pt.z)
{}

AwVector AwVector::operator*(const AwMatrix &right) const
{ 
	AwVector tmp; 
	double *a = (double *) (right.matrix);
	tmp.x = x * *a     + y * *(a+4) + z * *(a+8);
	tmp.y = x * *(a+1) + y * *(a+5) + z * *(a+9);
	tmp.z = x * *(a+2) + y * *(a+6) + z * *(a+10);

	return tmp; 
}

AwVector& AwVector::operator=(const AwMatrix &tm)
//
//	Description:
//		Extract the translation components out of a 4X4 homogeneous
//		transformation matrix into this vector.
//
{
	x = tm(3, 0);
	y = tm(3, 1);
	z = tm(3, 2);
	return *this;
}

AwVector &AwVector::normalize()
{
	double n = norm();
	if (n > kDoubleEpsilonSqr && !::equivalent(n, 1.0, 2.0*kDoubleEpsilon)) {
		double factor = 1.0 / sqrt(n);
		x *= factor; y *= factor; z *= factor;
	}
	return *this;
}

AwVector AwVector::rotateBy(const AwQuaternion &q) const
//
//	Description:
//		Returns the vector that represents the rotation of this
//		vector by the given quaternion.
//
//		Simply carry out the quaternion multiplications:
//		result = q.conjugate() * (*this) * q
//
{
	double rw = - q.x * x - q.y * y - q.z * z;
	double rx = q.w * x + q.y * z - q.z * y;
	double ry = q.w * y + q.z * x - q.x * z;
	double rz = q.w * z + q.x * y - q.y * x;
	return AwVector(- rw * q.x +  rx * q.w - ry * q.z + rz * q.y,
					- rw * q.y +  ry * q.w - rz * q.x + rx * q.z,
					- rw * q.z +  rz * q.w - rx * q.y + ry * q.x);
}

bool AwVector::isParallel(const AwVector &otherVector, double tolerance) const
//
//	Description:
//		Returns TRUE if the vectors are parallel, that is, pointing in
//		the same or opposite directions, but not necessarily of the same
//		magnitude.
//
{
	AwVector v1, v2;
	v1 = this->normal();
	v2 = otherVector.normal();
	double dotPrd = v1.dotProduct(v2);
	return (::equivalent(fabs(dotPrd), (double) 1.0, tolerance));
}

AwVector::Axis AwVector::dominantAxis() const
//
// Description:
//		Returns the axis along which this vector is dominant
//
//
{
	double xx, yy;

	if ((xx = fabs(x)) > (yy = fabs(y))) {
		if (xx > fabs(z)) {
			return kXaxis;
		} else {
			return kZaxis;
		}
	} else {
		if (yy > fabs(z)) {
			return kYaxis;
		} else {
			return kZaxis;
		}
	}
}

double AwVector::angle(const AwVector &vec) const
{ 
	double cosine = normal().dotProduct(vec.normal());
	double angle;
	if (cosine >= 1.0)
		angle = 0.0;
	else if (cosine <= -1.0)
		angle = kPi;
	else
		angle = acos(cosine);
	return angle;
}

AwVector::operator AwMatrix() const
//
//	Description:
//		This casts a AwVector to a translation Matrix.
//
{
	AwMatrix m;
	m(3, 0) = x;
	m(3, 1) = y;
	m(3, 2) = z;
	return m;
}

/*friend*/ ostream &operator<<(ostream &os, const AwVector &v)
//
//	Description:
//		Stream output.
//
{
	os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
	return os;
}
