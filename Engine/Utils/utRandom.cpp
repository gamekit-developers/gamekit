/*
   Written by Xavier T. based on origninal code under the followind lisence:

   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.

   Before using, initialize the state by using init_genrand(seed)
   or init_by_array(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote
        products derived from this software without specific prior written
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/

#include "utRandom.h"

#include <float.h>
#include <math.h>

/* Period parameters */
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */


utRandomNumberGenerator::utRandomNumberGenerator(UTuint32 seed)
{
	this->setSeed(seed);
}


void utRandomNumberGenerator::setSeed(UTuint32 seed)
{
	m_seed = seed;
	
	// This is the important initialisation part copied from original Makoto's code
	m_mt[0]= m_seed & 0xffffffffUL;
	for (m_mti=1; m_mti<N; m_mti++) {
		m_mt[m_mti] =
				(1812433253UL * (m_mt[m_mti-1] ^ (m_mt[m_mti-1] >> 30)) + m_mti);
		/* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
		/* In the previous versions, MSBs of the seed affect   */
		/* only MSBs of the array mt[].                        */
		/* 2002/01/09 modified by Makoto Matsumoto             */
		m_mt[m_mti] &= 0xffffffffUL;
		/* for >32 bit machines */
	}
}


UTuint32 utRandomNumberGenerator::rand32()
{
	unsigned long y;
	static unsigned long mag01[2]={0x0UL, MATRIX_A};
	/* mag01[x] = x * MATRIX_A  for x=0,1 */
	
	if (m_mti >= N) { /* generate N words at one time */
		int kk;
		
		for (kk=0;kk<N-M;kk++) {
			y = (m_mt[kk]&UPPER_MASK)|(m_mt[kk+1]&LOWER_MASK);
			m_mt[kk] = m_mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
		}
		for (;kk<N-1;kk++) {
			y = (m_mt[kk]&UPPER_MASK)|(m_mt[kk+1]&LOWER_MASK);
			m_mt[kk] = m_mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
		}
		y = (m_mt[N-1]&UPPER_MASK)|(m_mt[0]&LOWER_MASK);
		m_mt[N-1] = m_mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];
		
		m_mti = 0;
	}
	
	y = m_mt[m_mti++];
	
	/* Tempering */
	y ^= (y >> 11);
	y ^= (y << 7) & 0x9d2c5680UL;
	y ^= (y << 15) & 0xefc60000UL;
	y ^= (y >> 18);
	
	return (UTuint32)y;
	
}


float utRandomNumberGenerator::randUnit()
{
	return (float)(rand32())/(float)((UTuint32) 0xffffffff);
}


float utRandomNumberGenerator::randRange(float min, float max)
{
	return (max-min)*randUnit() + min;
}


float utRandomNumberGenerator::randNormal(float mean, float deviation)
{
	float a, b, s=0;
	float dev= (deviation<0) ? 0:deviation;
	
	while(s<=0 ||s>=1)
	{
		a = randRange(-1.0f, 1.0f);
		b = randRange(-1.0f, 1.0f);
		s= a*a+b*b;
	}
	b = a * (float)sqrt((-2.0*log(s)) / s);
	return mean+ b*dev;
}


float utRandomNumberGenerator::randNegativeExponential(float halflife)
{
	float hl= (halflife<0) ? 0:halflife;
	return hl * (-log(1.0f-randUnit() ));
}


int utRandomNumberGenerator::randPoisson(float mean)
{
	float l, p;
	int k = 0;
	float m= (mean<0) ? 0:mean;
	
	p = 1.0f;
	l = exp(-m);
	l = (l<FLT_MIN) ? FLT_MIN:l;
	while(p > l)
	{
		p = p*randUnit();
		k++;
	} 
	return k;
}


int utRandomNumberGenerator::randRangeInt(int min, int max)
{
	return min + (rand32() % (1+max-min));
}
