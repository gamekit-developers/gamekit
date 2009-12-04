/*
-------------------------------------------------------------------------------
	This file is part of the Ogre GameKit port.
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
#ifndef _gkHash_h_
#define _gkHash_h_

#include <stdlib.h>

#include "gkCommon.h"
#include "gkTypes.inl"

// ----------------------------------------------------------------------------
// Defined as (unsigned int)((0.5f * (1.f + sqrtf(5.f))) * UINT_MAX)
#define GK_GOLDEN_RATIO 2654435769U



// ----------------------------------------------------------------------------
typedef size_t			GKhash;
typedef long			GKlong;
typedef unsigned long   GKulong;
typedef int				GKint32;
typedef unsigned int	GKuint32;
typedef short			GKint16;
typedef unsigned short  GKuint16;
typedef signed char		GKint8;
typedef unsigned char   GKuint8;
typedef GKuint8			GKubyte;
typedef GKint8			GKbyte;
typedef bool			GKint1;
typedef float			GKfloat;


// ----------------------------------------------------------------------------
/// Support for generic overloading
template <typename T>
GK_INLINE GKhash gkHashT(const T& v)
{
	return gkHash(v);
}


// ----------------------------------------------------------------------------
/// Pointer gkHash
GK_INLINE GKhash gkHash(void *p)
{
	/// take memory address
	return static_cast<GKhash>((uintptr_t)(p));
}

// ----------------------------------------------------------------------------
GK_INLINE GKhash gkHash(GKint32 p)
{
	return static_cast<GKhash>(p) * GK_GOLDEN_RATIO;
}


// ----------------------------------------------------------------------------
GK_INLINE GKhash gkHash(GKuint32 p)
{
	return static_cast<GKhash>(p) * GK_GOLDEN_RATIO;
}


// ----------------------------------------------------------------------------
GK_INLINE GKhash gkHash(GKint16 p)
{
	return static_cast<GKhash>(p) * GK_GOLDEN_RATIO;
}


// ----------------------------------------------------------------------------
GK_INLINE GKhash gkHash(GKuint16 p)
{
	return static_cast<GKhash>(p) * GK_GOLDEN_RATIO;
}


// ----------------------------------------------------------------------------
GK_INLINE GKhash gkHash(GKint8 p)
{
	return static_cast<GKhash>(p) * GK_GOLDEN_RATIO;
}

// ----------------------------------------------------------------------------
GK_INLINE GKhash gkHash(GKuint8 p)
{
	return static_cast<GKhash>(p) * GK_GOLDEN_RATIO;
}

#endif//_gkHash_h_
