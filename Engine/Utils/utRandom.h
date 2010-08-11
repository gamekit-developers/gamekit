/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

    Contributor(s): nonw yet.
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

#ifndef UTRANDOM_H
#define UTRANDOM_H

#include "utCommon.h"

class utRandomNumberGenerator
{
private:
	UTuint32 m_seed;
	unsigned long m_mt[624];
	int m_mti;

public:
	utRandomNumberGenerator(UTuint32 seed);
	virtual ~utRandomNumberGenerator() {}
	
	void setSeed(UTuint32 seed);
	UT_INLINE UTuint32 getSeed(void)      {return m_seed;}
	
	UTuint32 rand32();
	float randUnit();
	float randRange(float min, float max);
	float randNormal(float mean, float deviation);
	float randNegativeExponential(float halflife);
	int randRangeInt(int min, int max);
	int randPoisson(float mean);
};

#endif // UTRANDOM_H
