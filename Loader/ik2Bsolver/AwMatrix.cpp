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

#include <math.h>
#include <assert.h>
#include <iostream>

#include <AwMath.h>
#include <AwPoint.h>
#include <AwVector.h>
#include <AwMatrix.h>

#define DET2X2(a,b,c,d) (a*d-b*c)

#define DET3X3(a1,a2,a3,b1,b2,b3,c1,c2,c3) /* | a1 b1 c1 | */ \
		   ( a1 * DET2X2( b2, b3, c2, c3 ) /* | a2 b2 c2 | */ \
		   - b1 * DET2X2( a2, a3, c2, c3 ) /* | a3 b3 c3 | */ \
		   + c1 * DET2X2( a2, a3, b2, b3 ) )

// Initialize constant
//
const AwMatrix AwMatrix::identity;

AwMatrix &AwMatrix::setToIdentity()
//
//  Description: 
//  	Sets the matrix to the identity.
//
{
	matrix[0][0] = 1.0;
	matrix[0][1] = 0.0;
	matrix[0][2] = 0.0;
	matrix[0][3] = 0.0;
	matrix[1][0] = 0.0;
	matrix[1][1] = 1.0;
	matrix[1][2] = 0.0;
	matrix[1][3] = 0.0;
	matrix[2][0] = 0.0;
	matrix[2][1] = 0.0;
	matrix[2][2] = 1.0;
	matrix[2][3] = 0.0;
	matrix[3][0] = 0.0;
	matrix[3][1] = 0.0;
	matrix[3][2] = 0.0;
	matrix[3][3] = 1.0;
	
	return *this;
}

AwMatrix AwMatrix::operator*(const AwMatrix &right) const
//
//	Description: 
//		Returns the matrix that is the inner product of the two argument matrices.
//
//	Note:
//		This routine has been hand scheduled for MIPS R4x00 CPU's
//		Don't rearrange this code unless you know what your doing.
//
{ 
	AwMatrix result;
	double *a = (double *) result.matrix;
	double *b = (double *) this->matrix;
	double *c = (double *) right.matrix;

	register double i0, i1, i2, i3, i4, i5, i6, i7;

	if (a == c) {
		register double c0, c4, c8, c12;

		c0 = c[0]; c4 = c[4]; c8 = c[8]; c12 = c[12];
		i0 = b[0] * c0; i1 = b[1] * c4; i2 = b[2] * c8; i3 = b[3] * c12;
		i4 = b[4] * c0; i5 = b[5] * c4; i6 = b[6] * c8; i7 = b[7] * c12;
		a[0] = i0 + i1 + i2 + i3;
		a[4] = i4 + i5 + i6 + i7;

		i0 = b[8]  * c0; i1 = b[9]  * c4; i2 = b[10] * c8; i3 = b[11] * c12;
		i4 = b[12] * c0; i5 = b[13] * c4; i6 = b[14] * c8; i7 = b[15] * c12;
		a[8]  = i0 + i1 + i2 + i3;
		a[12] = i4 + i5 + i6 + i7;

		c0 = c[1]; c4 = c[5]; c8 = c[9]; c12 = c[13];
		i0 = b[0] * c0; i1 = b[1] * c4; i2 = b[2] * c8; i3 = b[3] * c12;
		i4 = b[4] * c0; i5 = b[5] * c4; i6 = b[6] * c8; i7 = b[7] * c12;
		a[1] = i0 + i1 + i2 + i3;
		a[5] = i4 + i5 + i6 + i7;

		i0 = b[8]  * c0; i1 = b[9]  * c4; i2 = b[10] * c8; i3 = b[11] * c12;
		i4 = b[12] * c0; i5 = b[13] * c4; i6 = b[14] * c8; i7 = b[15] * c12;
		a[9]  = i0 + i1 + i2 + i3;
		a[13] = i4 + i5 + i6 + i7;

		c0 = c[2]; c4 = c[6]; c8 = c[10]; c12 = c[14];
		i0 = b[0] * c0; i1 = b[1] * c4; i2 = b[2] * c8; i3 = b[3] * c12;
		i4 = b[4] * c0; i5 = b[5] * c4; i6 = b[6] * c8; i7 = b[7] * c12;
		a[2] = i0 + i1 + i2 + i3;
		a[6] = i4 + i5 + i6 + i7;

		i0 = b[8]  * c0; i1 = b[9]  * c4; i2 = b[10] * c8; i3 = b[11] * c12;
		i4 = b[12] * c0; i5 = b[13] * c4; i6 = b[14] * c8; i7 = b[15] * c12;
		a[10] = i0 + i1 + i2 + i3;
		a[14] = i4 + i5 + i6 + i7;

		c0 = c[3]; c4 = c[7]; c8 = c[11]; c12 = c[15];
		i0 = b[0] * c0; i1 = b[1] * c4; i2 = b[2] * c8; i3 = b[3] * c12;
		i4 = b[4] * c0; i5 = b[5] * c4; i6 = b[6] * c8; i7 = b[7] * c12;
		a[3] = i0 + i1 + i2 + i3;
		a[7] = i4 + i5 + i6 + i7;

		i0 = b[8]  * c0; i1 = b[9]  * c4; i2 = b[10] * c8; i3 = b[11] * c12;
		i4 = b[12] * c0; i5 = b[13] * c4; i6 = b[14] * c8; i7 = b[15] * c12;
		a[11] = i0 + i1 + i2 + i3;
		a[15] = i4 + i5 + i6 + i7;

	} else {
		register double b0, b1, b2, b3;

		b0 = b[0]; b1 = b[1]; b2 = b[2]; b3 = b[3];
		i0 = b0 * c[0]; i1 = b1 * c[4]; i2 = b2 * c[8]; i3 = b3 * c[12];
		i4 = b0 * c[1]; i5 = b1 * c[5]; i6 = b2 * c[9]; i7 = b3 * c[13];
		a[0] = i0 + i1 + i2 + i3;
		a[1] = i4 + i5 + i6 + i7;

		i0 = b0 * c[2]; i1 = b1 * c[6]; i2 = b2 * c[10]; i3 = b3 * c[14];
		i4 = b0 * c[3]; i5 = b1 * c[7]; i6 = b2 * c[11]; i7 = b3 * c[15];
		a[2] = i0 + i1 + i2 + i3;
		a[3] = i4 + i5 + i6 + i7;

		b0 = b[4]; b1 = b[5]; b2 = b[6]; b3 = b[7];
		i0 = b0 * c[0]; i1 = b1 * c[4]; i2 = b2 * c[8]; i3 = b3 * c[12];
		i4 = b0 * c[1]; i5 = b1 * c[5]; i6 = b2 * c[9]; i7 = b3 * c[13];
		a[4] = i0 + i1 + i2 + i3;
		a[5] = i4 + i5 + i6 + i7;

		i0 = b0 * c[2]; i1 = b1 * c[6]; i2 = b2 * c[10]; i3 = b3 * c[14];
		i4 = b0 * c[3]; i5 = b1 * c[7]; i6 = b2 * c[11]; i7 = b3 * c[15];
		a[6] = i0 + i1 + i2 + i3;
		a[7] = i4 + i5 + i6 + i7;
	
		b0 = b[8]; b1 = b[9]; b2 = b[10]; b3 = b[11];
		i0 = b0 * c[0]; i1 = b1 * c[4]; i2 = b2 * c[8]; i3 = b3 * c[12];
		i4 = b0 * c[1]; i5 = b1 * c[5]; i6 = b2 * c[9]; i7 = b3 * c[13];
		a[8] = i0 + i1 + i2 + i3;
		a[9] = i4 + i5 + i6 + i7;

		i0 = b0 * c[2]; i1 = b1 * c[6]; i2 = b2 * c[10]; i3 = b3 * c[14];
		i4 = b0 * c[3]; i5 = b1 * c[7]; i6 = b2 * c[11]; i7 = b3 * c[15];
		a[10] = i0 + i1 + i2 + i3;
		a[11] = i4 + i5 + i6 + i7;

		b0 = b[12]; b1 = b[13]; b2 = b[14]; b3 = b[15];
		i0 = b0 * c[0]; i1 = b1 * c[4]; i2 = b2 * c[8]; i3 = b3 * c[12];
		i4 = b0 * c[1]; i5 = b1 * c[5]; i6 = b2 * c[9]; i7 = b3 * c[13];
		a[12] = i0 + i1 + i2 + i3;
		a[13] = i4 + i5 + i6 + i7;

		i0 = b0 * c[2]; i1 = b1 * c[6]; i2 = b2 * c[10]; i3 = b3 * c[14];
		i4 = b0 * c[3]; i5 = b1 * c[7]; i6 = b2 * c[11]; i7 = b3 * c[15];
		a[14] = i0 + i1 + i2 + i3;
		a[15] = i4 + i5 + i6 + i7;
	}
	
	return result;
}

AwMatrix &AwMatrix::invertIt()
//
//	Description: 
//		Sets the matrix to be its own inverse.
//
//	Notes:  
//		If the matrix is singular, return the identity matrix;
//
{
	// Create an identity matrix on which to mirror all of our operations.
	// The identity matrix is it's own inverse.
	//
	AwMatrix inv;

	int i, j, k;
	double factor, divisor, temp;

	// phase one:  eliminate all the numbers under the diagonal.  This is done
	// in a loop, considering successively smaller matrices as the top-left
	// corner of each is set to 1.  (This does not need to be done for the
	// last row, clearly)
	//
	for (i = 0; i < 3; i++) {
		// step one:  find the largest value in column i;
		// swap that row to be row i
		//
		int bigOne = i;
		for (j = i; j < 4; j++){
			if (matrix[j][i] * matrix[j][i] >
				matrix[bigOne][i] * matrix[bigOne][i]) {
				bigOne = j;
			}
		}
		if (bigOne != i) {
			for (j = 0; j < 4; j++) {
				temp = matrix[bigOne][j];
				matrix[bigOne][j] = matrix[i][j];
				matrix[i][j] = temp;
				temp = inv.matrix[bigOne][j];
				inv.matrix[bigOne][j] = inv.matrix[i][j];
				inv.matrix[i][j] = temp;
			}
		}
	
		//	step two: by appropriate subtraction, eliminate all the numbers
		//	under the diagonal element i.  Singular elements in the matrix
		//	are skipped.
		//
		if (matrix[i][i] > 1.0e-15 || matrix[i][i] < -1.0e-15) {
			for (j = i + 1; j < 4; j++) {
				factor = matrix[j][i] / matrix[i][i];
				for (k = 0; k < 4; k++) {
					matrix[j][k] -= matrix[i][k] * factor;
					inv.matrix[j][k] -= inv.matrix[i][k] * factor;
				}
				// set the column value to exactly zero in case
				// numeric roundoff left it a very tiny number
				matrix[j][i] = 0.0;
			}
		}
	}

	// phase two: set all the diagonal values to 1
	//
	for (i = 0; i < 4; i++) {
		divisor = matrix[i][i];
		if (divisor > 1.0e-15 || divisor < -1.0e-15) {
			for (j = 0; j < 4; j++) {
				matrix[i][j] = matrix[i][j] / divisor;
				inv.matrix[i][j] = inv.matrix[i][j] / divisor;
			}
			// set the diagonal to 1.0 exactly to avoid
			// possible round-off error
			matrix[i][i] = 1.0;
		}
	}

	// phase three: eliminate all the zeros above the diagonal
	//
	for (i = 0; i < 3; i++) {
		for (j = i + 1; j < 4; j++) {
			factor = matrix[i][j];
			for (k = 0; k < 4; k++) {
				matrix[i][k] -= matrix[j][k] * factor;
				inv.matrix[i][k] -= inv.matrix[j][k] * factor;
			}
			matrix[i][j] = 0.0;	// in case of round-off error
		}
	}

	// At this point inv should contain the correct inverse matrix to the
	// original matrix we started with.  Copy it to this matrix.
	//
	*this = inv;
	return *this;
}

AwMatrix &AwMatrix::transposeIt()
//
//	Description: 
//		Sets the matrix to be its own transpose.
//
{
	int i, j;
	double temp;
	for (i = 0; i < 3; i++){
		for (j = i + 1; j < 4; j++) {
			temp = matrix[i][j];
			matrix[i][j] = matrix[j][i];
			matrix[j][i] = temp;
		}
	}
	
	return *this;
}

bool AwMatrix::isEquivalent(const AwMatrix &otherM, double tolerance) const
//
//	Description:
//		This method returns true if this matrix is equal, within some
//		given tolerance, to otherM.
//
//	Notes:
//		tolerance is asserted to be non-negative, and defaults to
//		kMatrixEquivalentTolerance.
//
{
	assert(tolerance >= 0.0);
	
	return (
		fabs(matrix[0][0] - otherM.matrix[0][0]) <= tolerance
	&&	fabs(matrix[0][1] - otherM.matrix[0][1]) <= tolerance
	&&	fabs(matrix[0][2] - otherM.matrix[0][2]) <= tolerance
	&&	fabs(matrix[0][3] - otherM.matrix[0][3]) <= tolerance
	&&	fabs(matrix[1][0] - otherM.matrix[1][0]) <= tolerance
	&&	fabs(matrix[1][1] - otherM.matrix[1][1]) <= tolerance
	&&	fabs(matrix[1][2] - otherM.matrix[1][2]) <= tolerance
	&&	fabs(matrix[1][3] - otherM.matrix[1][3]) <= tolerance
	&&	fabs(matrix[2][0] - otherM.matrix[2][0]) <= tolerance
	&&	fabs(matrix[2][1] - otherM.matrix[2][1]) <= tolerance
	&&	fabs(matrix[2][2] - otherM.matrix[2][2]) <= tolerance
	&&	fabs(matrix[2][3] - otherM.matrix[2][3]) <= tolerance
	&&	fabs(matrix[3][0] - otherM.matrix[3][0]) <= tolerance
	&&	fabs(matrix[3][1] - otherM.matrix[3][1]) <= tolerance
	&&	fabs(matrix[3][2] - otherM.matrix[3][2]) <= tolerance
	&&	fabs(matrix[3][3] - otherM.matrix[3][3]) <= tolerance );
}

AwMatrix AwMatrix::operator*(double mult) const
//
//	Description:
//		This method multiplies the matrix by a constant
//
{
	AwMatrix temp(*this);
	temp[0][0] *= mult; temp[0][1] *= mult;
	temp[0][2] *= mult; temp[0][3] *= mult;
	temp[1][0] *= mult; temp[1][1] *= mult;
	temp[1][2] *= mult; temp[1][3] *= mult;
	temp[2][0] *= mult; temp[2][1] *= mult;
	temp[2][2] *= mult; temp[2][3] *= mult;
	temp[3][0] *= mult; temp[3][1] *= mult;
	temp[3][2] *= mult; temp[3][3] *= mult;
	return temp;
}

bool AwMatrix::operator==(const AwMatrix &otherM) const
//
//	Description:
//		This method returns true if this matrix is identically equal to otherM.
//
{
	return (
		matrix[0][0] == otherM.matrix[0][0]
	&&	matrix[0][1] == otherM.matrix[0][1]
	&&	matrix[0][2] == otherM.matrix[0][2]
	&&	matrix[0][3] == otherM.matrix[0][3]
	&&	matrix[1][0] == otherM.matrix[1][0]
	&&	matrix[1][1] == otherM.matrix[1][1]
	&&	matrix[1][2] == otherM.matrix[1][2]
	&&	matrix[1][3] == otherM.matrix[1][3]
	&&	matrix[2][0] == otherM.matrix[2][0]
	&&	matrix[2][1] == otherM.matrix[2][1]
	&&	matrix[2][2] == otherM.matrix[2][2]
	&&	matrix[2][3] == otherM.matrix[2][3]
	&&	matrix[3][0] == otherM.matrix[3][0]
	&&	matrix[3][1] == otherM.matrix[3][1]
	&&	matrix[3][2] == otherM.matrix[3][2]
	&&	matrix[3][3] == otherM.matrix[3][3] );
}

double AwMatrix::det3x3() const
//
//	Description:
//		Return the determinant of the upper-left 3 by 3 submatrix.
//
{
	return (matrix[0][0] * (matrix[1][1] * matrix[2][2] -
							matrix[2][1] * matrix[1][2]) -
			matrix[0][1] * (matrix[1][0] * matrix[2][2] -
							matrix[2][0] * matrix[1][2]) +
			matrix[0][2] * (matrix[1][0] * matrix[2][1] -
							matrix[2][0] * matrix[1][1]));
}

double AwMatrix::det4x4() const
//
//	Description:
//		Return the determinant of this matrix.
//
{
	double a1, a2, a3, a4;
	double b1, b2, b3, b4;
	double c1, c2, c3, c4;
	double d1, d2, d3, d4;

	// assign to individual variable names to aid selecting	correct elements
	a1 = matrix[0][0]; b1 = matrix[0][1]; c1 = matrix[0][2]; d1 = matrix[0][3];
	a2 = matrix[1][0]; b2 = matrix[1][1]; c2 = matrix[1][2]; d2 = matrix[1][3];
	a3 = matrix[2][0]; b3 = matrix[2][1]; c3 = matrix[2][2]; d3 = matrix[2][3];
	a4 = matrix[3][0]; b4 = matrix[3][1]; c4 = matrix[3][2]; d4 = matrix[3][3];

	return (
		a1 * DET3X3(b2, b3, b4, c2, c3, c4, d2, d3, d4) -
		b1 * DET3X3(a2, a3, a4, c2, c3, c4, d2, d3, d4) +
		c1 * DET3X3(a2, a3, a4, b2, b3, b4, d2, d3, d4) -
		d1 * DET3X3(a2, a3, a4, b2, b3, b4, c2, c3, c4)
	);
}

bool AwMatrix::isOrthogonal() const
//
//	Description:
//		Return true if the upper 3x3 sub-matrix does not
//		contain non-proportional scaling or skewing.
//
{
	#define rowdot(i,j)	(matrix[i][0]*matrix[j][0] + \
						 matrix[i][1]*matrix[j][1] + \
						 matrix[i][2]*matrix[j][2])
	#define rownorm(i)	(rowdot(i,i))

	//	First, test for non-proportional scales since they will
	//	occur more frequently than skewing.
	//
	double norm0 = rownorm(0);
	return (
		::equivalent(rownorm(1),  norm0) &&		// Non-prop scale tests
		::equivalent(rownorm(2),  norm0) &&
		::equivalent(rowdot(0,1), 0.0)   &&		// Skewing tests
		::equivalent(rowdot(1,2), 0.0)   &&
		::equivalent(rowdot(0,2), 0.0)
	);
	#undef rownorm
	#undef rowdot
}	

/*friend*/ ostream &operator<<(ostream &os, const AwMatrix &m)
//
//	Description:
//		Stream output.
//
{
	os << "[";
	for (int i = 0; i < 4; ++i) {
		if (i) os << ", ";
		os << "[";
		for (int j = 0; j < 4; ++j) {
			if (j) os << ", ";
			os << m[i][j];
		}
		os << "]";
	}
	os << "]";
	return os;
}
