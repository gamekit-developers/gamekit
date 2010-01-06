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
//	CLASS:    AwPoint
//
// *****************************************************************************
//
//	CLASS DESCRIPTION (AwPoint)
//
//	By expressing a point (a location in 3-space) in homogeneous
//	coordinates we can express the three transformations (translate,
//	rotate and scale) that can be applied to it as matrix
//	multiplications.  In homogeneous coordinates, the cartesian point
//	P(x, y, z) is represented as P(W*x, W*y, W*z, W) for any scale
//	factor W != 0.  Therefor given a homogeneous-coordinate
//	representation for a point P(X, Y, Z, W), we can find the 3D
//	cartesian coordinate representation for the point as x = X/W, y =
//	Y/W and z = Z/W.  In general W == 1.0, so the division won't be
//	required.  However, some NURBS code sets W to be non-unit to
//	acheive certain numerical effects!  The methods cartesianize()
//	and cartesian() make the division necessary to return a point that
//	has w == 1.0.
//
//	(Above paraphrased from Foley and Van Dam, Vol 1. p 250)
//
// *****************************************************************************

#include <math.h>
#include <assert.h>
#include <iostream>

#include <AwPoint.h>
#include <AwMatrix.h>

// Initialize constant
//
const AwPoint AwPoint::origin;

AwVector AwPoint::cartesianSub(const AwPoint &otherPt) const
{
	AwPoint ptA = cartesian();
	AwPoint ptB = otherPt.cartesian();
	return AwVector(ptA.x - ptB.x, ptA.y - ptB.y, ptA.z - ptB.z);
}

AwPoint AwPoint::cartesianAdd(const AwVector &v) const
{
	AwPoint pt = cartesian();
	return AwPoint(pt.x + v.x, pt.y + v.y, pt.z + v.z);
}

AwPoint AwPoint::cartesianSub(const AwVector &v) const
{
	AwPoint pt = cartesian();
	return AwPoint(pt.x - v.x, pt.y - v.y, pt.z - v.z);
}

AwPoint& AwPoint::cartesianize()
//
//	Description:
//		If 'this' point is P(W*x, W*y, W*z, W), for some scale factor W != 0,
//		then it is reset to be P(x, y, z, 1).
//
//	Note:
//		This will only work correctly if the point is in homogenous form or
//		cartesian form.  If the point is in rational form, the results are
//		not defined.
//
{
	if (w != 1.0) {
		assert(w != 0.0);
		double wInv = 1.0/w;
		x *= wInv; y *= wInv; z *= wInv; w = 1.0;
	}
	return *this;
}

inline AwPoint AwPoint::operator*(const AwMatrix &right) const
{ 
	AwPoint tmp;

	const double *a = (const double *) (right.matrix);
	tmp.x = x * a[0] + y * a[4] + z * a[8]  + w * a[12];
	tmp.y = x * a[1] + y * a[5] + z * a[9]  + w * a[13];
	tmp.z = x * a[2] + y * a[6] + z * a[10] + w * a[14];
	tmp.w = x * a[3] + y * a[7] + z * a[11] + w * a[15];

	return tmp;
}

/*friend*/ ostream &operator<<(ostream &os, const AwPoint &p)
//
//	Description:
//		Stream output.
//
{
	os << "[" << p.x << ", " << p.y << ", " << p.z << ", " << p.w << "]";
	return os;
}
