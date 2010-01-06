#ifndef _AwMath
#define _AwMath
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
//	AwMath
//
// *****************************************************************************
#include <math.h>

#define kFloatEpsilon		1.0e-5F
#define kDoubleEpsilon		1.0e-10
#define kDoubleEpsilonSqr	1.0e-20
#define kExtendedEpsilon	kDoubleEpsilon
#define kPi					3.14159265358979323846264338327950

#if COMPILER_NEEDS_BOOL_TYPEDEF
typedef	char bool;
#define true 1
#define false 0
#endif

inline bool equivalent(double x, double y, double fudge = kDoubleEpsilon) 
{
	return ((x > y) ? (x - y <= fudge) : (y - x <= fudge));
}

inline double clamp(double a, double l, double h)
{
	return ((a) < (l) ? (l) : (a) > (h) ? (h) : (a));
}

#endif /* _AwMath */

