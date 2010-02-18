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
#ifndef _utCommon_h_
#define _utCommon_h_

#define	UT_PLATFORM_WIN32	0
#define	UT_PLATFORM_LINUX	2
#define	UT_PLATFORM_APPLE	3

#define	UT_PLATFORM_FREEWIN	4
#define	UT_PLATFORM_MSVC	5

#if	defined	(_WIN32)
#	define UT_PLATFORM UT_PLATFORM_WIN32
#	if defined (__MINGW32__) ||	defined(__CYGWIN__)
#		define UT_SUB_PLATFORM UT_PLATFORM_FREEWIN
#	else
#		define UT_SUB_PLATFORM UT_PLATFORM_MSVC
#	endif
#elif defined(__APPLE__)
#	define UT_PLATFORM		UT_PLATFORM_APPLE
#	define UT_SUB_PLATFORM	UT_PLATFORM_APPLE
#else
#	define UT_PLATFORM		UT_PLATFORM_LINUX
#	define UT_SUB_PLATFORM	UT_PLATFORM_LINUX
#endif

#define	UT_COMPILER_MSVC	0
#define	UT_COMPILER_GNU		1


#if	defined(_MSC_VER)
# define UT_COMPILER UT_COMPILER_MSVC
#elif defined(__GNUC__)
# define UT_COMPILER UT_COMPILER_GNU
#else // bail
# error	unknown	compiler
#endif

#define	UT_ENDIAN_LITTLE	0
#define	UT_ENDIAN_BIG		1

#if	defined(__sgi) || defined (__sparc)	|| defined (__sparc__) || defined (__PPC__)	|| defined (__ppc__) ||	defined	(__BIG_ENDIAN__)
# define UT_ENDIAN UT_ENDIAN_BIG
#else
# define UT_ENDIAN UT_ENDIAN_LITTLE
#endif


#if	UT_PLATFORM	== UT_PLATFORM_WIN32
# if defined(__MINGW32__) || defined(__CYGWIN__) ||	(defined (_MSC_VER)	&& _MSC_VER	< 1300)
#  define UT_INLINE	inline
# else
#  define UT_INLINE	__forceinline
# endif
#else
#  define UT_INLINE	inline
#endif


#ifdef __cplusplus
# define UT_CDECL extern "C"
#else
# define UT_CDECL
#endif

#if	defined(DEBUG) || defined(_DEBUG)
# include <assert.h>
# define UT_DEBUG 1
# define UT_ASSERT(x) assert(x)
#else
# define UT_DEBUG 0
# define UT_ASSERT(x)
#endif

#define	UT_ALLOCATOR_DL		0
#define	UT_ALLOCATOR_NED	1
#define	UT_ALLOCATOR_STD	2
#define	UT_ALLOCATOR		2

#define	UT_Max(	lhs, rhs )		((lhs) < (rhs) ? (rhs) : (lhs))
#define	UT_Min(	lhs, rhs )		((rhs) < (lhs) ? (rhs) : (lhs))
#define	UT_NPOS					((size_t)-1)


#endif//_utCommon_h_
