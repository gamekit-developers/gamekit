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
#ifndef _gkBezierSpline_h_
#define _gkBezierSpline_h_

#include "gkCommon.h"
#include "gkMathUtils.h"

struct gkBezierVertex
{
	gkScalar h1[2];
	gkScalar cp[2];
	gkScalar h2[2];
};


class gkBezierSpline
{
public:

	enum BezierInterpolation
	{
		BEZ_LINEAR,
		BEZ_CONSTANT,
		BEZ_CUBIC,
	};

protected:
	utArray<gkBezierVertex>         m_verts;
	int                             m_code;
	BezierInterpolation             m_interpMethod;


	int solveRoots(const double& x,
	               const double p0,
	               const double p1,
	               const double p2,
	               const double p3,
	               double& s) const;

	gkScalar interpolate(const double& t,
	                     const double& p0,
	                     const double& p1,
	                     const double& p2,
	                     const double& p3) const;


	void updateHandles(gkScalar* p0, gkScalar* p1, gkScalar* p2, gkScalar* p3) const;


public:
	gkBezierSpline(int code) : m_code(code), m_interpMethod(BEZ_LINEAR) {}
	~gkBezierSpline() {}

	// interpolate across this spline
	// delta is the normalized time
	// time is the current frame number
	gkScalar interpolate(gkScalar delta, gkScalar time) const;

	GK_INLINE void addVertex(const gkBezierVertex& v)
	{m_verts.push_back(v);}

	GK_INLINE const gkBezierVertex* getVerts(void) const
	{return m_verts.ptr();}

	GK_INLINE int getNumVerts(void) const
	{return(int)m_verts.size();}

	///Gets the code bound to this spline.
	///ie; where to apply results, loc, rot, ...
	GK_INLINE int getCode(void) const
	{ return m_code ;}


	GK_INLINE void setInterpolationMethod(const BezierInterpolation& meth)
	{ m_interpMethod = meth; }


	GK_INLINE BezierInterpolation getInterpolationMethod(void)
	{ return m_interpMethod; }
};


#endif//_gkBezierSpline_h_
