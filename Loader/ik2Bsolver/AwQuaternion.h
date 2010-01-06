#ifndef _AwQuaternion
#define _AwQuaternion
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

#include <AwMath.h>
//#include <MIOStream.h>
#include <iostream>

class AwVector;
class AwMatrix;

class AwQuaternion {
public:
	//	Constructors
	//
	AwQuaternion(); 
	AwQuaternion(const AwQuaternion &q); 
	AwQuaternion(double x, double y, double z, double w); 
	AwQuaternion(const AwVector &, const AwVector &); 
	AwQuaternion(double angle, const AwVector &axis); 
//#ifndef COMPILE_OUTSIDE_MAYA
//	AwQuaternion(const MQuaternion &q);
//	operator MQuaternion() const;
//#endif
	~AwQuaternion(); 

	AwQuaternion &operator=(const AwQuaternion &); 
	AwQuaternion &operator=(const AwMatrix &);

	double &operator[](unsigned i); 
	double  operator[](unsigned i) const; 

	AwQuaternion operator*(const AwQuaternion &) const; 

	bool operator==(const AwQuaternion &) const; 
	bool operator!=(const AwQuaternion &) const; 

	operator AwMatrix() const;
	void convertToMatrix(AwMatrix &) const;

	AwQuaternion &setAxisAngle(const AwVector &axis, double theta); 
	bool getAxisAngle(AwVector &axis, double &theta) const; 
	
    friend ostream &operator<<(ostream &os, const AwQuaternion &);

    static const AwQuaternion identity; // The multiplicative identity. 

	double x, y, z, w; // imaginary (3) & real components
};

///////////////////////////////////////////////////////////////////////////
//	Inline methods
///////////////////////////////////////////////////////////////////////////

inline AwQuaternion::AwQuaternion()
: w(1.0), x(0.0), y(0.0), z(0.0)
{}

inline AwQuaternion::AwQuaternion(const AwQuaternion &q)
: w(q.w), x(q.x), y(q.y), z(q.z)
{}

inline AwQuaternion::AwQuaternion(double a, double b, double c, double d)
: x(a), y(b), z(c), w(d)
{}

inline AwQuaternion::AwQuaternion(double angle, const AwVector &axis)
: w(1.0), x(0.0), y(0.0), z(0.0)
{ setAxisAngle(axis, angle); }

//#ifndef COMPILE_OUTSIDE_MAYA
//inline AwQuaternion::AwQuaternion(const MQuaternion &q)
//: w(q.w), x(q.x), y(q.y), z(q.z)
//{}
//
//inline AwQuaternion::operator MQuaternion() const
//{
//	return MQuaternion(x, y, z, w);
//}
//#endif

inline AwQuaternion::~AwQuaternion ()
{}

inline AwQuaternion &AwQuaternion::operator=(const AwQuaternion &src)
{ w = src.w; x = src.x; y = src.y; z = src.z; return *this; }

inline double &AwQuaternion::operator[](unsigned i)
{ return (&x)[i]; }

inline double AwQuaternion::operator[](unsigned i) const
{ return (&x)[i]; }

inline bool AwQuaternion::operator==( const AwQuaternion &otherQ ) const
{ return w == otherQ.w && x == otherQ.x && y == otherQ.y && z == otherQ.z; }

inline bool AwQuaternion::operator!=( const AwQuaternion &otherQ ) const
{ return !( (*this) == otherQ ); }

#endif /* _AwQuaternion */
