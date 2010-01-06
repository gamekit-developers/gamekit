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
//	CLASS:    AwQuaternion
//
// *****************************************************************************
//
//	CLASS DESCRIPTION (AwQuaternion)
//
//	Quaternions can be used to specify orientations and rotations of 3-D
//	objects relative to a starting reference, similar to the way that cartesian
//	vectors can be used to specify positions and translations of 3-D objects
//	relative to an origin.  Quaternions represent orientations as a single
//	rotation, just as rectangular co-ordinates represent position as a single
//	vector.
//
// *****************************************************************************

#include <math.h>
#include <assert.h>
#include <iostream>
#include <AwMath.h>
#include <AwVector.h>
#include <AwMatrix.h>
#include <AwQuaternion.h>

// Initialize constants
//
const AwQuaternion AwQuaternion::identity;

AwQuaternion::AwQuaternion(const AwVector &a, const AwVector &b)
//
//	Description:
//		This constructor creates a new quaternion that will rotate vector
//		a into vector b about their mutually perpendicular axis. (if one
//		exists)
//
: w(1.0), x(0.0), y(0.0), z(0.0)
{
	double factor = a.length() * b.length();
	
	if (fabs(factor) > kFloatEpsilon) {
		// Vectors have length > 0
		AwVector pivotVector;
		double dot = a.dotProduct(b) / factor;
		double theta = acos(clamp(dot, -1.0, 1.0));
		
		pivotVector = a^b;
        if (dot < 0.0 && pivotVector.length() < kFloatEpsilon) {
			// Vectors parallel and opposite direction, therefore a rotation
			// of 180 degrees about any vector perpendicular to this vector
			// will rotate vector a onto vector b.
			//
			// The following guarantees the dot-product will be 0.0.
			//
		    size_t dominantIndex = a.dominantAxis();
			pivotVector[dominantIndex] = -a[(dominantIndex+1)%3];
			pivotVector[(dominantIndex+1)%3] = a[dominantIndex];
			pivotVector[(dominantIndex+2)%3] = 0.0;
		}		
		setAxisAngle(pivotVector, theta);
	}
}

AwQuaternion & AwQuaternion::operator=(const AwMatrix &tm)
//
//	Description:
//		Convert the given 4X4 homogeneous rotation matrix to a
//		quaternion of unit length.
//
//	Notes:
//		This methods always returns a quaternion of unit length IF it
//		is given a proper orthogonal matrix.  A proper othogonal
//		matrix is one such that the determinant of the matrix is one.
//		(If the determinant were -1, this would imply that the
//		orthogonal matrix is also producing a reflection, in addition
//		to a rotation.)
//
{
	double trace, s;
	int i, j, k;
	
	// vectors are multiplied on the left (pre-multipy).
	//

	trace = tm.matrix[0][0] + tm.matrix[1][1] + tm.matrix[2][2];
	if (trace > 0.0) {
		s = sqrt(trace + 1.0);
		w = s * 0.5;
		assert(s > kExtendedEpsilon);
		s = 0.5/s;
		x = (tm.matrix[1][2] - tm.matrix[2][1]) * s;
		y = (tm.matrix[2][0] - tm.matrix[0][2]) * s;
		z = (tm.matrix[0][1] - tm.matrix[1][0]) * s;
	} else {		
		i = 0;
		if (tm.matrix[1][1] > tm.matrix[0][0])
			i = 1;
		if (tm.matrix[2][2] > tm.matrix[i][i])
			i = 2;
		j = (i+1)%3;
		k = (j+1)%3;
		s = sqrt(tm.matrix[i][i] - (tm.matrix[j][j] + tm.matrix[k][k]) + 1.0);
		(*this)[i] = s * 0.5;
		assert(s > kExtendedEpsilon);
		s = 0.5 / s;
		w = (tm.matrix[j][k] - tm.matrix[k][j]) * s;
		(*this)[j] = (tm.matrix[j][i] + tm.matrix[i][j]) * s;
		(*this)[k] = (tm.matrix[k][i] + tm.matrix[i][k]) * s;
	}
	
	return *this;
}

AwQuaternion AwQuaternion::operator*(const AwQuaternion &rhs) const
//
//	Description:
//		This operator returns the product of two quaternions.
//		"This" quaternion is the left quaternion in the product.
//
//	Note:
//		In general, if p and q are quaternions, pq != qp, i.e.,
//		multiplication does not commute!
//
//		Quaternions in Maya multiply on the right (post-multiply)
//		the same as matrices. Many popular quaternion papers (Shoemake)
//		use pre-multiplication where quaternions pre-multiply on the left
//		so you must be aware of this when using quaternions.
//
{
	AwQuaternion result;	
	result.w = rhs.w * w - (rhs.x * x + rhs.y * y + rhs.z * z);
	result.x = rhs.w * x +  rhs.x * w + rhs.y * z - rhs.z * y;
	result.y = rhs.w * y +  rhs.y * w + rhs.z * x - rhs.x * z;
	result.z = rhs.w * z +  rhs.z * w + rhs.x * y - rhs.y * x;
	return result;
}

AwQuaternion::operator AwMatrix() const
//
//	Description: 
//		This casts a AwQuaternion to an AwMatrix by making use of the
//		convertToMatrix method.
//
{
	AwMatrix m;
	convertToMatrix(m);
	return m;
}

void AwQuaternion::convertToMatrix(AwMatrix &tm) const
//
//	Description:
//		Convert quaternion to a 4X4 homogeneous rotation matrix and pass
//		result back in 'tm'.  The construction of the matrix assumes that the
//		vectors are going to be multiplied on the left side of the matrix.
//		If the quaternion's length has degenerated, this method will still
//		produce a well behaved matrix.
//
//	Notes:
//		For an explanation of the following implementation see
//		Shoemake, K.,  "Quaternion Calculus and Fast Animation".
//
{
	// Common subexpressions
	//
	double ww = w*w, xx = x*x, yy = y*y, zz = z*z;
	double s = 2.0/(ww + xx + yy + zz);
	double xy = x*y, xz = x*z, yz = y*z, wx = w*x, wy = w*y, wz = w*z;
	
	// vectors are multiplied on the left (pre-multipy).
	//
	tm.matrix[0][0] = 1.0 - s * (yy + zz);
	tm.matrix[1][0] = s * (xy - wz);
	tm.matrix[2][0] = s * (xz + wy);
	tm.matrix[3][0] = 0.0;
	tm.matrix[0][1] = s * (xy + wz);
	tm.matrix[1][1] = 1.0 - s * (xx + zz);
	tm.matrix[2][1] = s * (yz - wx);
	tm.matrix[3][1] = 0.0;
	tm.matrix[0][2] = s * (xz - wy);
	tm.matrix[1][2] = s * (yz + wx);
	tm.matrix[2][2] = 1.0 - s * (xx + yy);
	tm.matrix[3][2] = 0.0;
	tm.matrix[0][3] = 0.0;
	tm.matrix[1][3] = 0.0;
	tm.matrix[2][3] = 0.0;
	tm.matrix[3][3] = 1.0;
}

AwQuaternion &AwQuaternion::setAxisAngle(const AwVector &axis, double theta)
//
//	Description:
//		This function sets this quaternion to be the rotation as expressed by a
//		pivot axis and a rotation theta (in radians) about that axis.  If the
//		axis is too small the quaternion returned will be the identity
//		quaternion.
//
{
    double sumOfSquares = 
		(double) axis.x * axis.x +
		(double) axis.y * axis.y +
		(double) axis.z * axis.z;
		
    if (sumOfSquares <= kDoubleEpsilon) {
		// Axis too small.
		*this = identity;
	} else {
		theta *= 0.5;
		w = cos(theta);
		double commonFactor = sin(theta);
		if (!::equivalent(sumOfSquares, 1.0)) 
			commonFactor /= sqrt(sumOfSquares);
		x = commonFactor * (double) axis.x;
		y = commonFactor * (double) axis.y;
		z = commonFactor * (double) axis.z;
	}
	return *this;
}

bool AwQuaternion::getAxisAngle(AwVector &axis, double &theta) const
//
//	Description:
//		This function converts this quaternion into a user understandable
//		representation.  That is, the quaternion is represented as a pivot
//		vector 'axis' and a rotation 'theta' (in radians) about that pivot
//		vector.
//
//	Returns:
//		true	- angle != 0
//		false	- angle == 0	(uses arbitrary axis, if given axis not valid)
//
//	Notes:
//		If the identity unit quaternion is attempted to be converted to the
//		pivot axis and angle representation it will be set to a zero degree
//		rotation about the axis that was passed in.  (Note that any axis will
//		do, since an infinity of axis with rotation of zero satisfy the identity
//		rotation.)  If the axis is zero length, then an arbitrary axis will be
//		chosen (z-axis).
//
{
	bool result;
	double inverseOfSinThetaByTwo, thetaExtended;
	
	if (::equivalent(w, (double) 1.0)) {
		theta = 0.0;
		if (axis.length() < kDoubleEpsilon) {
			//  Passed in axis invalid, choose an arbitrary axis.
			axis.set(0.0,0.0,1.0);
		}
		result = false;
	}
	else {
		thetaExtended = acos(clamp(w,-1.0,1.0));
		theta = thetaExtended * 2.0;
		
		inverseOfSinThetaByTwo = 1.0 / sin(thetaExtended);
		axis.x = x * inverseOfSinThetaByTwo;
		axis.y = y * inverseOfSinThetaByTwo;
		axis.z = z * inverseOfSinThetaByTwo;

		result = true;
	}

	return result;
}

/*friend*/ ostream &operator<<(ostream &os, const AwQuaternion &q)
//
//	Description:
//		Stream output.
//
{
	os << "[" 
	   << "x: " << q.x << ", " 
	   << "y: " << q.y << ", " 
	   << "z: " << q.z << ", " 
	   << "w: " << q.w << "]";
	
	return os;
}
