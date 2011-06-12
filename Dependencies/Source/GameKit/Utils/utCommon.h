/*
-------------------------------------------------------------------------------
    General Purpose Utility Library, should be kept dependency free.
    Unless the dependency can be compiled along with this library.

    Copyright (c) 2009-2010 Charlie C.
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
#ifndef _utCommon_h_
#define _utCommon_h_

#include <assert.h>

#if (defined(DEBUG) || defined(_DEBUG)) && defined (UT_DEBUG_ASSERT) 
# define UT_DEBUG 1
# define UT_ASSERT(x) assert(x)
#else
# define UT_DEBUG 0
# define UT_ASSERT(x) ((void)0)
#endif
#define UT_ASSERTSAFE(x)    ( ((x) == 0) ? true : false )
#define UT_ASSERTCOMP(x, n) typedef bool n[(x) ? 1 : 0];

#define UT_PLATFORM_WIN32     0
#define UT_PLATFORM_LINUX     2
#define UT_PLATFORM_APPLE     3
#define UT_PLATFORM_APPLE_IOS 5
#define UT_PLATFORM_ANDROID   6

#if defined (_WIN32)
# define UT_PLATFORM UT_PLATFORM_WIN32
#elif defined(__APPLE__)
# if __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= 30000 || __IPHONE_OS_VERSION_MIN_REQUIRED >= 30000
#   define UT_PLATFORM UT_PLATFORM_APPLE_IOS
# else
#	define UT_PLATFORM UT_PLATFORM_APPLE
# endif
#elif defined(__ANDROID__)
# define UT_PLATFORM UT_PLATFORM_ANDROID
#else
# define UT_PLATFORM UT_PLATFORM_LINUX
#endif

#define UT_COMPILER_MSVC    0
#define UT_COMPILER_GNU     1

#if defined(_MSC_VER)
# define UT_COMPILER UT_COMPILER_MSVC
#elif defined(__GNUC__)
# define UT_COMPILER UT_COMPILER_GNU
#else
# error unknown compiler
#endif

#define UT_ENDIAN_LITTLE    0
#define UT_ENDIAN_BIG       1

#if defined(__sgi)      ||  defined (__sparc)        || \
    defined (__sparc__) ||  defined (__PPC__)        || \
    defined (__ppc__)   ||  defined (__BIG_ENDIAN__)
#define UT_ENDIAN UT_ENDIAN_BIG
#else
#define UT_ENDIAN UT_ENDIAN_LITTLE
#endif

#if UT_ENDIAN == UT_ENDIAN_BIG
# define UT_ID(a,b,c,d) ( (int)(a)<<24 | (int)(b)<<16 | (c)<<8 | (d) )
# define IT_ID2(c, d)   ( (c)<<8 | (d) )
#else
# define UT_ID(a,b,c,d) ( (int)(d)<<24 | (int)(c)<<16 | (b)<<8 | (a) )
# define UT_ID2(c, d)   ( (d)<<8 | (c) )
#endif

#define UT_ARCH_32 0
#define UT_ARCH_64 1

#if defined(__x86_64__)     || defined(_M_X64)      || \
    defined(__powerpc64__)  || defined(__alpha__)   || \
    defined(__ia64__)       || defined(__s390__)    || \
    defined(__s390x__)
# define UT_ARCH UT_ARCH_64
UT_ASSERTCOMP(sizeof(void *) == 8, VOID_IS_8);
#else
#define UT_ARCH UT_ARCH_32
UT_ASSERTCOMP(sizeof(void *) == 4, VOID_IS_4);
#endif

#if UT_PLATFORM == UT_PLATFORM_WIN32
# if defined(__MINGW32__) || \
     defined(__CYGWIN__)  || \
     (defined (_MSC_VER) && _MSC_VER < 1300)
#  define UT_INLINE inline
# else
#  define UT_INLINE __forceinline
# endif
#else
#  define UT_INLINE    inline
#endif

// Integer types
typedef long            UTlong;
typedef unsigned long   UTulong;
typedef int             UTint32;
typedef unsigned int    UTuint32;
typedef short           UTint16;
typedef unsigned short  UTuint16;
typedef signed char     UTint8;
typedef unsigned char   UTuint8;
typedef unsigned char   UTubyte;
typedef signed char     UTbyte;
typedef bool            UTint1;


#if UT_COMPILER == UT_COMPILER_GNU
typedef long long          UTint64;
typedef unsigned long long UTuint64;
#else
typedef __int64          UTint64;
typedef unsigned __int64 UTuint64;
#endif

// Arch dependent types

#if UT_ARCH == UT_ARCH_64
typedef UTuint64    UTuintPtr;
typedef UTint64     UTintPtr;
#else
typedef UTuint32    UTuintPtr;
typedef UTint32     UTintPtr;
#endif

typedef UTuint32	UTsize;
typedef UTsize		UThash;
#define UT_NPOS     0xFFFFFFFF

#endif//_utCommon_h_
