#ifndef _AwPoint
#define _AwPoint
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
//	acheive certain numerical effects!  The methods cartesianizeIt()
//	and cartesian() make the division necessary to return a point that
//	has w == 1.0.
//
//	(Above paraphrased from Foley and Van Dam, Vol 1. p 250)
//
// *****************************************************************************

#include <AwMath.h>
#include <AwVector.h>

class AwPoint {
public:
	AwPoint(); 
	AwPoint(const AwPoint &); 
	AwPoint(const AwVector &); 
	AwPoint(double xx, double yy, double zz = 0.0, double ww = 1.0); 
//#ifndef COMPILE_OUTSIDE_MAYA
//	AwPoint(const MPoint &); 
//#endif
	~AwPoint(); 

	AwPoint &operator=(const AwPoint &); 
	AwPoint &set(double xx, double yy, double zz = 0.0, double ww = 1.0); 

	double &operator[](size_t i); 
	double operator[](size_t i) const; 

	AwPoint operator+(const AwVector &) const; 
	AwPoint operator-(const AwVector &) const; 
	AwVector operator-(const AwPoint &) const; 
	AwPoint operator*(const AwMatrix &right) const; 

	bool operator==(const AwPoint &) const; 
	bool operator!=(const AwPoint &) const; 

	AwPoint cartesian() const;
	AwPoint &cartesianize();

	AwPoint &setToProduct(const AwPoint &left, const AwMatrix &right); 

    friend ostream &operator<<(ostream &os, const AwPoint &); 

	static const AwPoint origin;  // The point (0, 0, 0).

	double x, y, z, w;

private:
	AwVector cartesianSub(const AwPoint &) const;
	AwPoint	cartesianAdd(const AwVector &) const;
	AwPoint	cartesianSub(const AwVector &) const;
};

///////////////////////////////////////////////////////////////////////////
//	Inline methods
///////////////////////////////////////////////////////////////////////////

inline AwPoint::AwPoint()
: x(0.0), y(0.0), z(0.0), w(1.0)
{}

inline AwPoint::AwPoint(const AwPoint &pt)
: x(pt.x), y(pt.y), z(pt.z), w(pt.w)
{}

inline AwPoint::AwPoint(const AwVector &v)
: x(v.x), y(v.y), z(v.z), w(1.0)
{}

inline AwPoint::AwPoint(double xx, double yy, double zz, double ww)
: x(xx), y(yy), z(zz), w(ww)
{}

//#ifndef COMPILE_OUTSIDE_MAYA
//inline AwPoint::AwPoint(const MPoint &pt)
//: x(pt.x), y(pt.y), z(pt.z), w(pt.w)
//{}
//#endif

inline AwPoint::~AwPoint()
{}

inline AwPoint &AwPoint::set(double xx, double yy, double zz, double ww)
{ x = xx; y = yy; z = zz; w = ww; return *this; }

inline AwPoint &AwPoint::operator=(const AwPoint &src)
{ x = src.x; y = src.y; z = src.z; w = src.w; return *this; }

inline double &AwPoint::operator[](size_t i)
{ return (&x)[i]; }

inline double AwPoint::operator[](size_t i) const
{ return (&x)[i]; }

inline AwPoint AwPoint::operator+(const AwVector &v) const
{ if (w == 1.0) return AwPoint(x+v.x, y+v.y, z+v.z);
  else return cartesianAdd(v); }

inline AwPoint AwPoint::operator-(const AwVector &v) const
{ if (w == 1.0) return AwPoint(x-v.x, y-v.y, z-v.z);
  else return cartesianSub(v); }

inline AwVector AwPoint::operator-(const AwPoint &o) const
{ if (w == 1.0 && o.w == 1.0) return AwVector(x-o.x, y-o.y, z-o.z);
  else return cartesianSub(o); }

inline bool AwPoint::operator==(const AwPoint &p) const
{ return( x == p.x && y == p.y && z == p.z && w == p.w); }

inline bool AwPoint::operator!=(const AwPoint &otherPoint) const
{ return !(*this == otherPoint); }

inline AwPoint AwPoint::cartesian() const
{ AwPoint temp(*this); return temp.cartesianize(); }

#endif /* _AwPoint */
