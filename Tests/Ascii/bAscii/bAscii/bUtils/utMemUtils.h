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
#ifndef _utMemUtils_h_
#define _utMemUtils_h_

#include <memory.h>
#include <new>
#include "utCommon.h"
#include "utIntTypes.h"

#define utMalloc(size)          utMemUtils::alloc(size)
#define utCalloc(size, len)     utMemUtils::alloc(size * len)
#define utFree(ptr)             utMemUtils::dealloc(ptr)
#define utMemset                ::memset
#define utMemcpy                ::memcpy
#define utMemmove               ::memmove

// ----------------------------------------------------------------------------
class utMemUtils
{
public:

	template<typename T>
	static T *talloc(bool zero= false)
	{
		void* ptr = alloc(sizeof(T));
		if (zero) utMemset(ptr, 0, sizeof(T));
		return (T*)ptr;
	}

	template<typename T>
	static T *tcalloc(size_t nr, bool zero= false)
	{
		void* ptr = alloc(sizeof(T) * nr);
		if (zero) utMemset(ptr, 0, sizeof(T) * nr);
		return (T*)ptr;
	}

	template<typename T>
	static void tdealloc(T *ptr)
	{
		if (ptr)
		{
			ptr->~T();
			dealloc(ptr);
		}
	}

	static void *alloc(size_t sizeInBytes);
	static void dealloc(void *);
};



#endif//_utMemUtils_h_
