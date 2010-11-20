/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): none yet.
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

struct akBezierVertex
{
	akScalar h1[2];
	akScalar cp[2];
	akScalar h2[2];
};


class akBezierSpline
{
public:

	enum BezierInterpolation
	{
		BEZ_LINEAR,
		BEZ_CONSTANT,
		BEZ_CUBIC,
	};

protected:
	utArray<akBezierVertex>         m_verts;
	int                             m_code;
	BezierInterpolation             m_interpMethod;


	int solveRoots(const double& x,
	               const double p0,
	               const double p1,
	               const double p2,
	               const double p3,
	               double& s) const;

	akScalar interpolate(const double& t,
	                     const double& p0,
	                     const double& p1,
	                     const double& p2,
	                     const double& p3) const;


	void updateHandles(akScalar* p0, akScalar* p1, akScalar* p2, akScalar* p3) const;


public:
	akBezierSpline(int code) : m_code(code), m_interpMethod(BEZ_LINEAR) {}
	~akBezierSpline() {}

	// interpolate across this spline
	// delta is the normalized time
	// time is the current frame number
	akScalar interpolate(akScalar delta, akScalar time) const;

	UT_INLINE void addVertex(const akBezierVertex& v)
	{m_verts.push_back(v);}

	UT_INLINE const akBezierVertex* getVerts(void) const
	{return m_verts.ptr();}

	UT_INLINE int getNumVerts(void) const
	{return(int)m_verts.size();}

	///Gets the code bound to this spline.
	///ie; where to apply results, loc, rot, ...
	UT_INLINE int getCode(void) const
	{ return m_code ;}


	UT_INLINE void setInterpolationMethod(const BezierInterpolation& meth)
	{ m_interpMethod = meth; }


	UT_INLINE BezierInterpolation getInterpolationMethod(void) const
	{ return m_interpMethod; }
};


#endif//_akBezierSpline_h_
