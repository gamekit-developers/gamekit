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
#ifndef _OgreMemUtils_h_
#define _OgreMemUtils_h_

#include "OgreUtilsCommon.h"
#include "OgreMemoryAllocatorConfig.h"

#if OGRE_MEMORY_ALLOCATOR == OGRE_MEMORY_ALLOCATOR_NEDPOOLING

#define utMalloc(size)                      Ogre::NedPoolingPolicy::allocateBytes(size)
#define utCalloc(size, len)                 Ogre::NedPoolingPolicy::allocateBytes(size * len)
#define utFree(ptr)                         Ogre::NedPoolingPolicy::deallocateBytes(ptr)

#define utMallocAllign(align, size)         Ogre::NedPoolingAlignedPolicy<align>::allocateBytes(size)
#define utCallocAllign(align, size, len)    Ogre::NedPoolingAlignedPolicy<align>::allocateBytes(size * len)
#define utFreeAllign(align, ptr)            Ogre::NedPoolingAlignedPolicy<align>::deallocateBytes(ptr)

#elif OGRE_MEMORY_ALLOCATOR == OGRE_MEMORY_ALLOCATOR_NED


#define utMalloc(size)                      Ogre::NedAllocPolicy::allocateBytes(size)
#define utCalloc(size, len)                 Ogre::NedAllocPolicy::allocateBytes(size * len)
#define utFree(ptr)                         Ogre::NedAllocPolicy::deallocateBytes(ptr)

#define utMallocAllign(align, size)         Ogre::NedAlignedAllocPolicy<align>::allocateBytes(size)
#define utCallocAllign(align, size, len)    Ogre::NedAlignedAllocPolicy<align>::allocateBytes(size * len)
#define utFreeAllign(align, ptr)            Ogre::NedAlignedAllocPolicy<align>::deallocateBytes(ptr)


#else


#define utMalloc(size)                      Ogre::StdAllocPolicy::allocateBytes(size)
#define utCalloc(size, len)                 Ogre::StdAllocPolicy::allocateBytes(size * len)
#define utFree(ptr)                         Ogre::StdAllocPolicy::deallocateBytes(ptr)

#define utMallocAllign(align, size)         Ogre::StdAlignedAllocPolicy<align>::allocateBytes(size)
#define utCallocAllign(align, size, len)    Ogre::StdAlignedAllocPolicy<align>::allocateBytes(size * len)
#define utFreeAllign(align, ptr)            Ogre::StdAlignedAllocPolicy<align>::deallocateBytes(ptr)

#endif

#define utMemset ::memset
#define utMemcpy ::memcpy
#define utMemmove ::memmove


namespace Ogre
{


class MemUtils
{
public:

    template<typename T>
    static OGRE_INLINE T *talloc(bool zero = false)
    {
        void* ptr = utMalloc(sizeof(T));
        if (zero) utMemset(ptr, 0, sizeof(T));
        return (T*)ptr;
    }

    template<typename T>
    static OGRE_INLINE T *tcalloc(size_t nr, bool zero = false)
    {
        void* ptr = utMalloc(sizeof(T) * nr);
        if (zero) utMemset(ptr, 0, sizeof(T) * nr);
        return (T*)ptr;
    }

    template<typename T>
    static OGRE_INLINE void tdealloc(T *ptr)
    {
        if (ptr)
        {
            ptr->~T();
            utFree(ptr);
        }
    }

    static OGRE_INLINE void *alloc(size_t sizeInBytes)
	{
		return utMalloc(sizeInBytes);
	}

	static OGRE_INLINE void dealloc(void *p)
	{
		if (p)
			utFree(p);
	}
};


}


#endif//_OgreMemUtils_h_
