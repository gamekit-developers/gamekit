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
#ifndef _utAllocator_h_
#define _utAllocator_h_

#include "utCommon.h"
#include "utMemUtils.h"


class utAllocator
{
public:
	
	template<typename T>
	UT_INLINE T *talloc(void)
	{
		void* ptr = utMemUtils::alloc(sizeof(T));
		new(ptr) T();
		return (T*)ptr;
	}

	template<typename T>
	UT_INLINE void tfree(T *ptr)
	{
		if (ptr) 
		{
			ptr->~T();
			utMemUtils::dealloc(ptr);
		}
	}
	UT_INLINE void *alloc(size_t sizeInBytes)
	{
		return utMemUtils::alloc(sizeInBytes);
	}
	UT_INLINE void *calloc(size_t sizeInBytes, size_t nr)
	{
		return utMemUtils::alloc(sizeInBytes * nr);
	}
	UT_INLINE void free(void *ptr)
	{
		if (ptr) utMemUtils::dealloc(ptr);
	}
};

class utAllocObject
{
public:
	void* operator new(size_t sizeInBytes)
	{
		return utMemUtils::alloc(sizeInBytes);
	}
	void operator delete(void *ptr)
	{
		if (ptr) utMemUtils::dealloc(ptr);
	}

	void* operator new[](size_t sizeInBytes)
	{
		return utMemUtils::alloc(sizeInBytes);
	}
	void operator delete[](void *ptr)
	{
		if (ptr) utMemUtils::dealloc(ptr);
	}
};


#endif//_utAllocator_h_
