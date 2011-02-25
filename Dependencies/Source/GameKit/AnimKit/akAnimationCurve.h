/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): Xavier T.
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
#ifndef _akBezierSpline_h_
#define _akBezierSpline_h_


#include "akCommon.h"
#include "akMathUtils.h"

#include "utTypes.h"


/// Base component of an animation clip (and animation channel).
/// Contains "samples" of the animated values and theiur time postions. 
/// Interpolation for time values between samples can be constant, linear
/// or using bezier curves (for which you need to set the handles).
class akAnimationCurve
{
public:
	enum Codes
	{
		SC_LOC_X,
		SC_LOC_Y,
		SC_LOC_Z,
		SC_SCL_X,
		SC_SCL_Y,
		SC_SCL_Z,
		SC_ROT_QUAT_X,
		SC_ROT_QUAT_Y,
		SC_ROT_QUAT_Z,
		SC_ROT_QUAT_W,
		SC_ROT_EULER_X,
		SC_ROT_EULER_Y,
		SC_ROT_EULER_Z,
	};
	
	enum BezierInterpolation
	{
		BEZ_LINEAR,
		BEZ_CONSTANT,
		BEZ_CUBIC,
	};

protected:
	UTsize                          m_numSamples;
	akScalar                        *m_times;
	akScalar                        *m_values;
	akScalar                        *m_handles;
	int                             m_code;
	BezierInterpolation             m_interpMethod;


	int solveRoots(const akScalar& x,
	               const akScalar p0,
	               const akScalar p1,
	               const akScalar p2,
	               const akScalar p3,
	               akScalar& s) const;

	akScalar interpolate(const akScalar& t,
	                     const akScalar& p0,
	                     const akScalar& p1,
	                     const akScalar& p2,
	                     const akScalar& p3) const;


	void updateHandles(akScalar* p0, akScalar* p1, akScalar* p2, akScalar* p3) const;


public:
	akAnimationCurve(UTsize numSamples, int code, BezierInterpolation binterp);
	~akAnimationCurve();

	akScalar evaluate(akScalar time, akScalar delta=0.5f) const;

	/// Set the sample time postion and value as well as the bezier handles.
	/// Handles are discarted if interpoaltion is not cubic
	void setSample(UTsize i, akScalar time, akScalar value,
							akScalar handle1t, akScalar handle1v,
							akScalar handle2t, akScalar handle2v);

	
	UT_INLINE int getNumSamples(void) const
	{
		return(int)m_numSamples;
	}

	/// Gets the code bound to this spline.
	/// ie; where to apply results, loc, rot, ...
	UT_INLINE int getCode(void) const
	{
		return m_code;
	}

	UT_INLINE BezierInterpolation getInterpolationMethod(void) const
	{
		return m_interpMethod;
	}
};


#endif//_akBezierSpline_h_
