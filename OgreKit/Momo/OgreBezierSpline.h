/*
-------------------------------------------------------------------------------
	This file is part of OgreKit.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#ifndef _OgreBezierSpline_h_
#define _OgreBezierSpline_h_

#include "OgrePrerequisites.h"
#include "OgreMathUtils.h"
#include "OgreArray.h"
#include "OgreUtilsAllocator.h"

namespace Ogre
{

struct BezierVertex
{
	Vector2 h1, cp, h2;
};


/// Simple Bezier spline for IPO/Actions
class BezierSpline : public AllocObject
{
public:

	enum BezierInterpolation
	{
		BEZ_LINEAR,
		BEZ_CONSTANT,
		BEZ_CUBIC,
	};

protected:
	Ogre::Array<BezierVertex>	m_verts;
	int							m_code;
	BezierInterpolation			m_interpMethod;


	int solveRoots(const double &x,
	               const double p0,
	               const double p1,
	               const double p2,
	               const double p3,
	               double &s) const;

	Real interpolate(const double &t,
	                 const double &p0,
	                 const double &p1,
	                 const double &p2,
	                 const double &p3) const;


	void updateHandles(Vector2 &p0, Vector2 &p1, Vector2 &p2, Vector2 &p3) const;


public:
	BezierSpline(int code) : m_code(code), m_interpMethod(BEZ_LINEAR) {}
	~BezierSpline() {}

	/// interpolate across this spline
	/// delta is the normalized time
	/// time is the current frame number
	Real interpolate(Real delta, Real time) const;

	OGRE_INLINE void addVertex(const BezierVertex& v)
	{m_verts.push_back(v);}

	OGRE_INLINE const BezierVertex* getVerts(void) const
	{return m_verts.ptr();}

	OGRE_INLINE int getNumVerts(void) const
	{return(int)m_verts.size();}

	// Gets the code for this spline 
	// ie; where to apply results, loc, rot...
	OGRE_INLINE int getCode(void) const
	{ return m_code ;}

	/// Set how this spline is handled
	OGRE_INLINE void setInterpolationMethod(const BezierInterpolation& meth)
	{ m_interpMethod = meth; }


	OGRE_INLINE BezierInterpolation getInterpolationMethod(void)
	{ return m_interpMethod; }
};


}

#endif//_OgreBezierSpline_h_
