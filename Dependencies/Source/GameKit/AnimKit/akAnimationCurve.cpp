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
#include "akAnimationCurve.h"

#include <float.h>
#include <math.h>



akScalar CubicRoot(const akScalar d)
{
	static const akScalar o3 = 1.0 / 3.0;
	return d > 0.0 ?  pow(d, o3)  :
	       d < 0.0 ? -pow(-d, o3) : 0.0;
}



bool StepCubic(const akScalar* P0, const akScalar* P1, const akScalar* P2, const akScalar* P3, akScalar time, akScalar& cval)
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

akAnimationCurve::akAnimationCurve(UTsize numSamples, UTuint32 code, BezierInterpolation binterp)
	: m_numSamples(numSamples), m_code(code), m_interpMethod(binterp), m_handles(0)
{
	m_times = new akScalar[m_numSamples];
	m_values = new akScalar[m_numSamples];
	
	if(m_interpMethod == BEZ_CUBIC)
	{
		m_handles = new akScalar[4*m_numSamples];
	}
}

akAnimationCurve::~akAnimationCurve()
{
	delete[] m_times;
	delete[] m_values;
	
	if(m_handles)
		delete[] m_handles;
}

void akAnimationCurve::setSample(UTsize i, akScalar time, akScalar value, akScalar handle1t, akScalar handle1v, akScalar handle2t, akScalar handle2v)
{
	m_times[i] = time;
	m_values[i] = value;
	
	if(m_interpMethod == BEZ_CUBIC)
	{
		m_handles[i*4+0] = handle1t;
		m_handles[i*4+1] = handle1v;
		m_handles[i*4+2] = handle2t;
		m_handles[i*4+3] = handle2v;
	}
}

akScalar akAnimationCurve::interpolate(const akScalar& t,
                                     const akScalar& p0,
                                     const akScalar& p1,
                                     const akScalar& p2,
                                     const akScalar& p3) const
{
	akScalar c0, c1, c2, c3;
	akScalar t2 = t * t;
	akScalar t3 = t2 * t;

	c0 = p0;
	c1 = -3.0 * p0 + 3.0 * p1;
	c2 = 3.0 * p0 - 6.0 * p1 + 3.0 * p2;
	c3 = -p0 + 3.0 * p1 - 3.0 * p2 + p3;

	return (akScalar)(c0 + t * c1 + t2 * c2 + t3 * c3);
}



int akAnimationCurve::solveRoots(const akScalar& x,
                               const akScalar p0,
                               const akScalar p1,
                               const akScalar p2,
                               const akScalar p3,
                               akScalar& s) const
{

	// Adapped from Graphics Gems
	// And from Graphics Gems IV
	// Roots3And4.c, solver.c
	// by Jochen Schwarze

	static const akScalar o2    = 2.0 / 27.0;
	static const akScalar o3    = 1.0 / 3.0;
	static const akScalar tPI    = (4.0 * atan(1.0)) * o3;

	akScalar a, b, c, d, p, q, aa, cp, ao3;
	akScalar c0, c1, c2, c3;

	s = 0.0;

	c0 = p0 - x;
	c1 = -3.0 * p0 + 3.0 * p1;
	c2 = 3.0 * p0 - 6.0 * p1 + 3.0 * p2;
	c3 = -p0 + 3.0 * p1 - 3.0 * p2 + p3;

//	if (m_interpMethod == akAnimationCurve::BEZ_LINEAR)
//	{
//		// do linear
//		if (akFuzzy(c1))
//			return 0;

//		s = -c0 / c1;
//		return akEq(s) ? 1 : 0;
//	}

	if (akFuzzy(c3))
	{
		if (!akFuzzy(c1))
			s = -c0 / c1;

		if (akEq(s))
			return 1;

		if (akFuzzy(c0))
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

	if (akFuzzy(d))
	{
		if (akFuzzy(q))
		{
			// one triple solution
			s = 0.0;
			return 1;
		}
		else
		{
			akScalar u = CubicRoot(-q);

			// one single and one double solution
			s = 2.0 * u;
			// try next
			if (!akEq(s - ao3))
				s = -u;
		}
	}
	else if (d < 0.0)
	{
		// three real solutions
		akScalar phi = o3 * acos(-q / sqrt(-cp));
		akScalar t = 2.0 * sqrt(-p);

		s = t * cos(phi);
		if (!akEq(s - ao3))
		{
			// try next
			s = -t *  cos(phi + tPI);
			if (!akEq(s - ao3))
				s = -t * cos(phi - tPI);
		}
	}
	else
	{
		// one real solution
		akScalar S = sqrt(d);
		akScalar u = CubicRoot(S - q);
		akScalar v = -CubicRoot(S + q);
		s = u  + v;
	}

	// resubstitute
	s -= ao3;
	return akEq(s) ? 1 : 0;
}



void akAnimationCurve::updateHandles(akScalar* p0, akScalar* p1, akScalar* p2, akScalar* p3) const
{
	akScalar lh = akAbs((p0[0] - p1[0]) + (p3[0] - p2[0]));
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



akScalar akAnimationCurve::evaluate(akScalar time, akScalar delta) const
{
	if (m_numSamples==0)
		return 0.f;

	// at the start
	if (m_times[0] >= time)  return m_values[1];

	// at the end
	if (m_times[m_numSamples-1] <= time) return m_values[m_numSamples-1];

	// find closest segment to time
	int segment = (int)(delta * (m_numSamples - 1));
	while (segment && m_times[segment] > time)
		--segment;

	segment = akMax(segment, 0);

	for (int s = segment; s < (m_numSamples - 1); ++s)
	{
		if (m_times[s] <= time && m_times[s+1] >= time)
		{
			switch(m_interpMethod)
			{
			case BEZ_CONSTANT:
			{
				// take constant
				return m_values[s];
			}
				
			case BEZ_LINEAR:
			{
				akScalar a = (time - m_times[s]) / (m_times[s+1] - m_times[s]);
				return (1-a)*m_values[s] + a* m_values[s+1];
			}
				
			case BEZ_CUBIC:
			{
				akScalar p0[2], p1[2], p2[2], p3[2];
				akScalar r = 0.0;
				
				p0[0] = m_times[s+0];
				p0[1] = m_values[s+0];
				p1[0] = m_handles[4*(s+0)+2];
				p1[1] = m_handles[4*(s+0)+3];
				p2[0] = m_handles[4*(s+1)+0];
				p2[1] = m_handles[4*(s+1)+1];
				p3[0] = m_times[s+1];
				p3[1] = m_values[s+1];
				
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
		}
	}
	return 0.f;
}
