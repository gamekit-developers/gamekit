/*
-------------------------------------------------------------------------------
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C
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
#ifndef _utHash_h_
#define _utHash_h_


#include "utCommon.h"
#include "utIntTypes.h"

#define utIntHash(key)			\
		key += ~(key << 15);	\
		key ^=  (key >> 10);	\
		key +=  (key << 3);		\
		key ^=  (key >> 6);		\
		key += ~(key << 11);	\
		key ^=  (key >> 16);

typedef size_t UThash;

template <typename T>
UT_INLINE UThash utTHash(const T& v)	{ return utHash(v); }
UT_INLINE UThash utHash(void *p)		{ return static_cast<UThash>((uintptr_t)(p));}
UT_INLINE UThash utHash(int p)			{ UThash key = static_cast<UThash>(p); utIntHash(key); return key; }
UT_INLINE UThash utHash(unsigned int p)	{ UThash key = static_cast<UThash>(p); utIntHash(key); return key; }


UT_INLINE UThash utHash(const char* str)	
{
	// Lifted from btHashMap.h

	// magic numbers from http://www.isthe.com/chongo/tech/comp/fnv/
	static const unsigned int  InitialFNV = 2166136261u;
	static const unsigned int FNVMultiple = 16777619u;

	// Fowler / Noll / Vo (FNV) Hash
	unsigned int hash = InitialFNV;
	for (int i = 0; str[i]; i++)
	{
		hash = hash ^(str[i]);		// xor  the low 8 bits
		hash = hash * FNVMultiple;  // multiply by the magic number
	}
	return (UThash)hash;
}


#endif//_utHash_h_
