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
#include "utMemUtils.h"

#if UT_ALLOCATOR == UT_ALLOCATOR_NED
#undef assert
# include "Alloc/ned/nedmalloc.c"
# define ut_palloc(size) nedalloc::nedmalloc(size)
# define ut_pfree(ptr) nedalloc::nedfree(ptr)
#elif UT_ALLOCATOR == UT_ALLOCATOR_DL
# define USE_DL_PREFIX
# include "Alloc/dl/malloc.c"
# define ut_palloc(size) dlmalloc(size)
# define ut_pfree(ptr) dlfree(ptr)
#else
# include <stdlib.h>
# include <memory.h>
# define ut_palloc(size) malloc(size)
# define ut_pfree(ptr) free(ptr)
#endif


// -----------------------------------------------------------------------------
void *utMemUtils::alloc(size_t sizeInBytes)
{
	return ut_palloc(sizeInBytes);
}

// -----------------------------------------------------------------------------
void utMemUtils::dealloc(void *ptr)
{
	if (ptr)
		ut_pfree(ptr);
}
