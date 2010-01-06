#ifndef _AwMatrix
#define _AwMatrix
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
//	CLASS:    AwMatrix
//
// *****************************************************************************
//
//	CLASS DESCRIPTION (AwMatrix)
//
//	This class is a standard 4x4 double matrix.  Simple operations
//	such as multiplication, inversion and transposition are supported.
//	An indexing function is supplied, but if required, users can
//	access the matrix elements directly.
//
// *****************************************************************************

#include <AwMath.h>

#define kMatrixEquivalentTolerance kDoubleEpsilon

class AwPoint;
class AwVector;

class AwMatrix {
public:
	AwMatrix();
	AwMatrix(const AwMatrix &src);
//#ifndef COMPILE_OUTSIDE_MAYA
//	AwMatrix(const MMatrix &src);
//#endif
	~AwMatrix();

	AwMatrix &operator=(const AwMatrix &rhs);

	double *operator[](unsigned i);
	const double *operator[](unsigned i) const;
	double &operator()(short i, short j);
	const double &operator()(short i, short j) const;

	AwMatrix operator*(const AwMatrix &right) const;
	AwMatrix operator*(double) const;

	bool operator==(const AwMatrix &otherM) const;
	bool operator!=(const AwMatrix &otherM) const;
	bool isEquivalent(const AwMatrix &otherM,
					  double tolerance = kMatrixEquivalentTolerance) const;

	AwMatrix &setToIdentity();
	AwMatrix inverse() const;
	AwMatrix &invertIt();
	AwMatrix transpose() const;
	AwMatrix &transposeIt();
	double det3x3() const;
	double det4x4() const;
	bool isOrthogonal() const;

    friend ostream &operator<<(ostream &os, const AwMatrix &);

	static const AwMatrix identity;  // The multiplicative identity.
	
	double matrix[4][4];  // [row][column]
};

///////////////////////////////////////////////////////////////////////////
//	Inline methods
///////////////////////////////////////////////////////////////////////////

inline AwMatrix::AwMatrix()
{
	matrix[0][1] = matrix[0][2] = matrix[0][3] =
	matrix[1][0] = matrix[1][2] = matrix[1][3] =
	matrix[2][0] = matrix[2][1] = matrix[2][3] =
	matrix[3][0] = matrix[3][1] = matrix[3][2] = 0.0;
	matrix[0][0] = matrix[1][1] = matrix[2][2] = matrix[3][3] = 1.0;
}

inline AwMatrix &AwMatrix::operator=(const AwMatrix &rhs)
{
	matrix[0][0] = rhs.matrix[0][0]; matrix[0][1] = rhs.matrix[0][1];
	matrix[0][2] = rhs.matrix[0][2]; matrix[0][3] = rhs.matrix[0][3];
	matrix[1][0] = rhs.matrix[1][0]; matrix[1][1] = rhs.matrix[1][1];
	matrix[1][2] = rhs.matrix[1][2]; matrix[1][3] = rhs.matrix[1][3];
	matrix[2][0] = rhs.matrix[2][0]; matrix[2][1] = rhs.matrix[2][1];
	matrix[2][2] = rhs.matrix[2][2]; matrix[2][3] = rhs.matrix[2][3];
	matrix[3][0] = rhs.matrix[3][0]; matrix[3][1] = rhs.matrix[3][1];
	matrix[3][2] = rhs.matrix[3][2]; matrix[3][3] = rhs.matrix[3][3];
	return *this;
}

inline AwMatrix::AwMatrix(const AwMatrix &src)
{ *this = src; }

//#ifndef COMPILE_OUTSIDE_MAYA
//inline AwMatrix::AwMatrix(const MMatrix &src)
//{
//	matrix[0][0] = src.matrix[0][0]; matrix[0][1] = src.matrix[0][1];
//	matrix[0][2] = src.matrix[0][2]; matrix[0][3] = src.matrix[0][3];
//	matrix[1][0] = src.matrix[1][0]; matrix[1][1] = src.matrix[1][1];
//	matrix[1][2] = src.matrix[1][2]; matrix[1][3] = src.matrix[1][3];
//	matrix[2][0] = src.matrix[2][0]; matrix[2][1] = src.matrix[2][1];
//	matrix[2][2] = src.matrix[2][2]; matrix[2][3] = src.matrix[2][3];
//	matrix[3][0] = src.matrix[3][0]; matrix[3][1] = src.matrix[3][1];
//	matrix[3][2] = src.matrix[3][2]; matrix[3][3] = src.matrix[3][3];
//}
//#endif

inline AwMatrix::~AwMatrix()
{}

inline double *AwMatrix::operator[](unsigned i)
{ return &matrix[i][0]; }

inline const double *AwMatrix::operator[](unsigned i) const
{ return &matrix[i][0]; }

inline double & AwMatrix::operator()(short i, short j)
{ return matrix[i][j]; }

inline const double& AwMatrix::operator()(short i, short j) const
{ return matrix[i][j]; }

inline AwMatrix AwMatrix::inverse() const
{ AwMatrix temp(*this); temp.invertIt(); return temp; }

inline AwMatrix AwMatrix::transpose() const
{ AwMatrix temp(*this); temp.transposeIt(); return temp; }

inline bool AwMatrix::operator!=(const AwMatrix &otherM) const
{ return !( (*this) == otherM ); }

#endif /* _AwMatrix */
