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
#include "akBezierSpline.h"

#include <float.h>
#include <math.h>

#define SplineTOL                DBL_EPSILON
#define SplineAbs(x)            fabs(x)
#define SplineFuzzy( x )        (SplineAbs( x ) <= SplineTOL)
#define SplineEq(x)                ((x) >= (-SplineTOL) && (x) <= (1.+SplineTOL))
#define SplineFuzzyEq(x, y)        (SplineAbs(((x) - (y))) <= SplineTOL)
#define SplineMax( lhs, rhs )    ((lhs) < (rhs) ? (rhs) : (lhs))




double CubicRoot(const double d)
{
	static const double o3 = 1.0 / 3.0;
	return d > 0.0 ?  pow(d, o3)  :
	       d < 0.0 ? -pow(-d, o3) : 0.0;
}



bool StepCubic(const akScalar* P0, const akScalar* P1, const akScalar* P2, const akScalar* P3, akScalar time, double& cval)
{
	akScalar ts = (P3[0] - P0[0]);
	if (ts <= 1.f)
		ts = 2.f;
	if (ts >= 60.f)
		ts = 60.f;


	const akScalar step = (1.f / ts) / 4.f;
	// 240 max, anything else is choppy

	cval = 0.0;
	akScalar s = 1.f, sc, sst;
	akScalar t = 0.f, tc, tts;


	while (t <= 1.0)
	{
		sc =  s * s * s;
		tc =  t * t * t;
		sst = 3.f * s * s * t;
		tts = 3.f * t * t * s;

		if ((sc * P0[0] + sst * P1[0] + tts * P2[0] + tc * P3[0]) >= time)
		{
			cval = (sc * P0[1] + sst * P1[1] + tts * P2[1] + tc * P3[1]);
			return true;
		}

		t += step;
		s = 1.f - t;
	}
	return false;
}


akScalar akBezierSpline::interpolate(const double& t,
                                     const double& p0,
                                     const double& p1,
                                     const double& p2,
                                     const double& p3) const
{
	double c0, c1, c2, c3;
	double t2 = t * t;
	double t3 = t2 * t;

	c0 = p0;
	c1 = -3.0 * p0 + 3.0 * p1;
	c2 = 3.0 * p0 - 6.0 * p1 + 3.0 * p2;
	c3 = -p0 + 3.0 * p1 - 3.0 * p2 + p3;

	return (akScalar)(c0 + t * c1 + t2 * c2 + t3 * c3);
}



int akBezierSpline::solveRoots(const double& x,
                               const double p0,
                               const double p1,
                               const double p2,
                               const double p3,
                               double& s) const
{

	// Adapped from Graphics Gems
	// And from Graphics Gems IV
	// Roots3And4.c, solver.c
	// by Jochen Schwarze

	static const double o2    = 2.0 / 27.0;
	static const double o3    = 1.0 / 3.0;
	static const double tPI    = (4.0 * atan(1.0)) * o3;

	double a, b, c, d, p, q, aa, cp, ao3;
	double c0, c1, c2, c3;

	s = 0.0;

	c0 = p0 - x;
	c1 = -3.0 * p0 + 3.0 * p1;
	c2 = 3.0 * p0 - 6.0 * p1 + 3.0 * p2;
	c3 = -p0 + 3.0 * p1 - 3.0 * p2 + p3;

	if (m_interpMethod == akBezierSpline::BEZ_LINEAR)
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



void akBezierSpline::updateHandles(akScalar* p0, akScalar* p1, akScalar* p2, akScalar* p3) const
{
	akScalar lh = SplineAbs((p0[0] - p1[0]) + (p3[0] - p2[0]));
	if (lh != 0.0)
	{
		if (lh > (p3[0] - p0[0]))
		{
			akScalar f = (p3[0] - p0[0]) / lh;
			p1[0] = (p0[0] - f * akAbs((p0[0] - p1[0])));
			p1[1] = (p0[1] - f * akAbs((p0[1] - p1[1])));
			p2[0] = (p3[0] - f * akAbs((p3[0] - p2[0])));
			p2[1] = (p3[1] - f * akAbs((p3[1] - p2[1])));
		}
	}
}



akScalar akBezierSpline::interpolate(akScalar delta, akScalar time) const
{
	const akBezierVertex* vp = m_verts.ptr();
	int totvert = (int)m_verts.size();
	akScalar p0[2], p1[2], p2[2], p3[2];
	double r = 0.0;

	if (!vp || !totvert)
		return 0.f;

	// at the start
	if (vp[0].cp[0] >= time)  return vp[0].cp[1];

	// at the end
	if (vp[totvert-1].cp[0] <= time) return vp[totvert-1].cp[1];

	// find closest segment to time

	int segment = (int)(delta * (totvert - 1));
	while (segment && vp[segment].cp[0] > time)
		--segment;

	segment = SplineMax(segment, 0);

	for (int s = segment; s < (totvert - 1); ++s)
	{
		if (vp[s].cp[0] <= time && vp[s+1].cp[0] >= time)
		{
			if (m_interpMethod == BEZ_CONSTANT)
			{
				// take constant
				return vp[s+0].cp[1];
			}

			p0[0] = vp[s+0].cp[0]; p0[1] = vp[s+0].cp[1];
			p1[0] = vp[s+0].h2[0]; p1[1] = vp[s+0].h2[1];
			p2[0] = vp[s+1].h1[0]; p2[1] = vp[s+1].h1[1];
			p3[0] = vp[s+1].cp[0]; p3[1] = vp[s+1].cp[1];

			updateHandles(p0, p1, p2, p3);

			if (solveRoots(time, p0[0], p1[0], p2[0], p3[0], r))
				return interpolate(r, p0[1], p1[1], p2[1], p3[1]);
			else
			{
				//printf("Failed %f\n", r);
				if (StepCubic(p0, p1, p2, p3, time, r))
					return (akScalar)r;
			}
		}
	}
	return 0.f;
}
