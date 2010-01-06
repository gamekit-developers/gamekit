#ifndef _AwVector
#define _AwVector
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

#if defined(OSMac_)
#include <stdlib.h>
#endif
#include <AwMath.h>
#include <iostream>
using namespace std;

#define kVectorEquivalentTolerance	kDoubleEpsilon

class AwPoint;
class AwMatrix;
class AwQuaternion;

class AwVector {
public:
	enum Axis {
		kXaxis,
		kYaxis,
		kZaxis
	};

	AwVector(); 
	AwVector(const AwVector &); 
	AwVector(const AwPoint &); 
	AwVector(double xx, double yy, double zz = 0.0); 
//#ifndef COMPILE_OUTSIDE_MAYA
//	AwVector(const MVector &); 
//#endif
	~AwVector(); 

	AwVector &operator=(const AwVector &); 
	AwVector &operator=(const AwMatrix &); 
	AwVector &set(double xx, double yy, double zz = 0.0); 

	double &operator[](size_t i); 
	double operator[](size_t i) const; 

	double operator*(const AwVector &right) const; 
	AwVector operator^(const AwVector &right) const; 

	double dotProduct(const AwVector &) const; 
	AwVector crossProduct(const AwVector &) const; 

	AwVector operator*(const AwMatrix &right) const; 
	AwVector &operator*=(const AwMatrix &right);
	AwVector operator*(double) const;
	AwVector operator+(const AwVector &) const; 
	AwVector operator-(const AwVector &) const; 
	AwVector operator-() const; 

	bool operator==(const AwVector &) const; 
	bool operator!=(const AwVector &) const; 

	double length() const; 
	double norm() const; 
	AwVector normal() const; 
	AwVector &normalize(); 

	AwVector rotateBy(const AwQuaternion &) const; 
	bool isParallel(const AwVector &, 
					double = kVectorEquivalentTolerance) const; 
	Axis dominantAxis() const; 
	double angle(const AwVector &) const; 

	operator AwMatrix() const; 

    friend ostream &operator<<(ostream &os, const AwVector &);

	static const AwVector zero;		// The identity transformation. 
	static const AwVector xAxis;	// The x axis 
	static const AwVector yAxis;	// The y axis 
	static const AwVector zAxis;	// The z axis 

	double x, y, z;
};

///////////////////////////////////////////////////////////////////////////
//	Inline methods
///////////////////////////////////////////////////////////////////////////

inline AwVector::AwVector ()
: x(0.0), y(0.0), z(0.0) {}

inline AwVector::AwVector(const AwVector &v)
: x(v.x), y(v.y), z(v.z) {}

inline AwVector::AwVector(double xx, double yy, double zz)
: x(xx), y(yy), z(zz) {}

//#ifndef COMPILE_OUTSIDE_MAYA
//inline AwVector::AwVector(const MVector &v)
//: x(v.x), y(v.y), z(v.z) {}
//#endif

inline AwVector::~AwVector ()
{}

inline AwVector &AwVector::operator=(const AwVector &src)
{ x = src.x; y = src.y; z = src.z; return *this; }

inline AwVector &AwVector::set(double xx, double yy, double zz)
{ x = xx; y = yy; z = zz; return *this; }

inline double & AwVector::operator[](size_t i)
{ return (&x)[i]; }

inline double AwVector::operator[](size_t i) const
{ return (&x)[i]; }

inline double AwVector::dotProduct(const AwVector &v) const
{ return (x*v.x  + y*v.y + z*v.z); }

inline AwVector AwVector::crossProduct(const AwVector &r) const
{ return AwVector(y*r.z - z*r.y, z*r.x - x*r.z, x*r.y - y*r.x); }

inline double AwVector::operator*(const AwVector &a) const
{ return dotProduct(a); }

inline AwVector AwVector::operator^(const AwVector &a) const
{ return crossProduct(a); }

inline AwVector AwVector::operator+(const AwVector &v) const
{ return AwVector(x + v.x, y + v.y, z + v.z); }

inline AwVector AwVector::operator-(const AwVector &v) const
{ return AwVector(x - v.x, y - v.y, z - v.z); }

inline AwVector &AwVector::operator*=(const AwMatrix &m)
{ *this = *this * m; return *this; }

inline AwVector AwVector::operator*(double s) const
{ return AwVector(x * s, y * s, z * s); }

inline AwVector AwVector::operator-() const
{ return AwVector(-x, -y, -z); }

inline bool AwVector::operator==(const AwVector &v) const
{ return (x == v.x && y == v.y && z == v.z); }

inline bool AwVector::operator!=(const AwVector &v) const
{ return !(*this == v); }

inline double AwVector::norm() const
{ return (x*x + y*y + z*z); }

inline double AwVector::length() const
{ return sqrt(norm()); }

inline AwVector AwVector::normal() const
{ AwVector tmp(*this); return tmp.normalize(); }

#endif // _AwVector
