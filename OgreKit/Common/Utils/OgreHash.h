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
#ifndef _OgreHash_h_
#define _OgreHash_h_


#include "OgreUtilsCommon.h"
#include "OgreIntTypes.h"

#define OgreIntHash(key)          \
        key += ~(key << 15);    \
        key ^=  (key >> 10);    \
        key +=  (key << 3);     \
        key ^=  (key >> 6);     \
        key += ~(key << 11);    \
        key ^=  (key >> 16);

namespace Ogre
{

typedef size_t hash_t;

OGRE_INLINE hash_t OgreHash(void *p)        { return static_cast<hash_t>((uintptr_t)(p));}
OGRE_INLINE hash_t OgreHash(int p)          { hash_t key = static_cast<hash_t>(p); OgreIntHash(key); return key; }
OGRE_INLINE hash_t OgreHash(unsigned long int p) { hash_t key = static_cast<hash_t>(p); OgreIntHash(key); return key; }

template <typename T>
OGRE_INLINE hash_t OgreTHash(const T& v)    { return OgreHash(v); }

///////////////////////////////////////////////////////////////////////////////
OGRE_INLINE hash_t OgreHash(const char* str)
{
    // Lifted from btHashMap.h

    // magic numbers from http://www.isthe.com/chongo/tech/comp/fnv/
    static const unsigned int  InitialFNV = 2166136261u;
    static const unsigned int FNVMultiple = 16777619u;

    // Fowler / Noll / Vo (FNV) Hash
    unsigned int hash = InitialFNV;
    for (int i = 0; str[i]; i++)
    {
        hash = hash ^(str[i]);        // xor  the low 8 bits
        hash = hash * FNVMultiple;  // multiply by the magic number
    }
    return (hash_t)hash;
}

}

#endif//_OgreHash_h_
