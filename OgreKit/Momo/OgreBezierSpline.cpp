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
#include "OgreBezierSpline.h"

using namespace Ogre;


#define SplineTOL		DBL_EPSILON
#define SplineAbs(x)	fabs(x)
#define SplineFuzzy( x ) (SplineAbs( x ) <= SplineTOL)
#define SplineEq(x) ((x) >= (-SplineTOL) && (x) <= (1.+SplineTOL))
#define SplineFuzzyEq(x, y) (SplineAbs(((x) - (y))) <= SplineTOL)




double CubicRoot(const double d)
{
	static const double o3 = 1.0 / 3.0;
	return d > 0.0 ?  pow(d, o3)  :
	       d < 0.0 ? -pow(-d, o3) : 0.0;
}



bool StepCubic(const Vector2 &P0, const Vector2 &P1, const Vector2 &P2, const Vector2 &P3, Real time, double &cval)
{
	Real ts = (P3.x - P0.x);
	if (ts <= 1.0)
		ts = 2.0;
	if (ts >= 60.0)
		ts = 60.0;


	const Real step = (Real(1.0) / ts) / 4.f;
	/// 240 max, anything else is choppy

	cval = Real(0.0);
	Real s = Real(1.0), sc, sst;
	Real t = Real(0.0), tc, tts;


	while (t <= 1.0)
	{
		sc =  s * s * s;
		tc =  t * t * t;
		sst = Real(3.0) * s * s * t;
		tts = Real(3.0) * t * t * s;

		if ((sc * P0.x + sst * P1.x + tts * P2.x + tc * P3.x) >= time)
		{
			cval = (sc * P0.y + sst * P1.y + tts * P2.y + tc * P3.y);
			return true;
		}

		t += step;
		s = Real(1.0) - t;
	}
	return false;
}


Real BezierSpline::interpolate(const double &t,
                               const double &p0,
                               const double &p1,
                               const double &p2,
                               const double& p3) const
{
	double c0, c1, c2, c3;
	double t2 = t * t;
	double t3 = t2 * t;

	c0 = p0;
	c1 = -3.0 * p0 + 3.0 * p1;
	c2 = 3.0 * p0 - 6.0 * p1 + 3.0 * p2;
	c3 = -p0 + 3.0 * p1 - 3.0 * p2 + p3;

	return (Real)(c0 + t * c1 + t2 * c2 + t3 * c3);
}



int BezierSpline::solveRoots(const double &x,
                             const double p0,
                             const double p1,
                             const double p2,
                             const double p3,
                             double &s) const
{

	// Adapped from Graphics Gems
	// And from Graphics Gems IV
	// Roots3And4.c, solver.c
	// by Jochen Schwarze

	static const double o2	= 2.0 / 27.0;
	static const double o3	= 1.0 / 3.0;
	static const double tPI	= (4.0 * atan( 1.0 )) * o3;

	double a, b, c, d, p, q, aa, cp, ao3;
	double c0, c1, c2, c3;

	s = 0.0;

	c0 = p0 - x;
	c1 = -3.0 * p0 + 3.0 * p1;
	c2 = 3.0 * p0 - 6.0 * p1 + 3.0 * p2;
	c3 = -p0 + 3.0 * p1 - 3.0 * p2 + p3;

	if (m_interpMethod == BezierSpline::BEZ_LINEAR)
	{
		// do linear
		if (SplineFuzzy(c1))
			return 0;

		s = -c0 / c1;
		return SplineEq(s) ? 1 : 0;
	}

	if (SplineFuzzy(c3))
	{
		if (!SplineFuzzy(c1))
			s = -c0 / c1;

		if (SplineEq(s))
			return 1;

		if (SplineFuzzy(c0))
			return 1;

		return 0;
	}


	// normal form:
	// x^3 + Ax^2 + Bx + C = 0

	a = c2 / c3;
	b = c1 / c3;
	c = c0 / c3;

	// substitute x = y - a/3 to eliminate quadric term:
	// x^3 +px + q = 0

	ao3 = a * o3;
	aa = a * a;

	p = o3 * (-o3 * aa + b);
	q = 0.5 * (o2 * a * aa - o3 * a * b + c);

	// use Cardano's formula
	cp = p * p * p;
	d = q * q + cp;

	if (SplineFuzzy(d))
	{
		if (SplineFuzzy(q))
		{
			// one triple solution
			s = 0.0;
			return 1;
		}
		else
		{
			double u = CubicRoot(-q);

			// one single and one double solution
			s = 2.0 * u;
			// try next
			if (!SplineEq(s - ao3))
				s = -u;
		}
	}
	else if (d < 0.0)
	{
		// three real solutions
		double phi = o3 * acos(-q / sqrt(-cp));
		double t = 2.0 * sqrt(-p);

		s = t * cos(phi);
		if (!SplineEq(s - ao3))
		{
			// try next
			s = -t *  cos(phi + tPI);
			if (!SplineEq(s - ao3))
				s = -t * cos(phi - tPI);
		}
	}
	else
	{
		// one real solution
		double S = sqrt(d);
		double u = CubicRoot(S - q);
		double v = -CubicRoot(S + q);
		s = u  + v;
	}

	// resubstitute
	s -= ao3;
	return SplineEq(s) ? 1 : 0;
}



void BezierSpline::updateHandles(Vector2 &p0, Vector2 &p1, Vector2 &p2, Vector2 &p3) const
{
	Real lh = Math::Abs((p0.x - p1.x) + (p3.x - p2.x));
	if (lh != 0.0)
	{
		if (lh > (p3.x - p0.x))
		{
			Real f = (p3.x - p0.x) / lh;
			p1.x = (p0.x - f * OgreAbs((p0.x - p1.x)));
			p1.y = (p0.y - f * OgreAbs((p0.y - p1.y)));
			p2.x = (p3.x - f * OgreAbs((p3.x - p2.x)));
			p2.y = (p3.y - f * OgreAbs((p3.y - p2.y)));
		}
	}
}



Real BezierSpline::interpolate(Real delta, Real time) const
{
	const BezierVertex *vp = m_verts.ptr();
	int totvert = (int)m_verts.size();

	if (!vp || !totvert)
		return 0.f;

	// at the start
	if (vp[0].cp.x >= time)  return vp[0].cp.y;

	// at the end
	if (vp[totvert-1].cp.x <= time) return vp[totvert-1].cp.y;

	/// find closest segment to time

	int segment = (int)(delta * (totvert-1));
	while (segment && vp[segment].cp.x > time)
		--segment;

	segment = OgreMax(segment, 0);

	Vector2 p0, p1, p2, p3; double r=0.0;
	for (int s = segment; s < (totvert - 1); ++s)
	{
		if (vp[s].cp.x <= time && vp[s+1].cp.x >= time)
		{
			if (m_interpMethod == BEZ_CONSTANT)
			{
				// take constant
				return vp[s+0].cp.y;
			}

			p0 = vp[s+0].cp; p1 = vp[s+0].h2;
			p2 = vp[s+1].h1; p3 = vp[s+1].cp;
			updateHandles(p0, p1, p2, p3);

			if (solveRoots(time, p0.x, p1.x, p2.x, p3.x, r))
				return interpolate(r, p0.y, p1.y, p2.y, p3.y);
			else
			{
				//printf("Failed %f\n", r);
				if (StepCubic(p0, p1, p2, p3, time, r))
					return (Real)r;
			}
		}
	}
	return 0.f;
}
