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
#ifndef _nsUtCommon_h_
#define _nsUtCommon_h_



// Platform defines



#if defined(DEBUG) || defined(_DEBUG)
# include <assert.h>
# define NS_DEBUG 1
# define NS_ASSERT(x) assert(x)
#else
# define NS_DEBUG 0
# define NS_ASSERT(x)
#endif
#define NS_ASSERTSAFE(x)    ( ((x) == 0) ? true : false )
#define NS_ASSERTCOMP(x, n) typedef bool n[(x) ? 1 : 0];

#define NS_PLATFORM_WIN32    0
#define NS_PLATFORM_LINUX    2
#define NS_PLATFORM_APPLE    3

#if defined (_WIN32)
# define NS_PLATFORM NS_PLATFORM_WIN32
#elif defined(__APPLE__)
# define NS_PLATFORM NS_PLATFORM_APPLE
#else
# define NS_PLATFORM NS_PLATFORM_LINUX
#endif

#define NS_COMPILER_MSVC    0
#define NS_COMPILER_GNU     1


#if defined(_MSC_VER)
# define NS_COMPILER NS_COMPILER_MSVC
#elif defined(__GNUC__)
# define NS_COMPILER NS_COMPILER_GNU
#else // bail
# error unknown compiler
#endif

#define NS_ENDIAN_LITTLE    0
#define NS_ENDIAN_BIG       1

#if defined(__sgi)      ||  defined (__sparc)        || \
    defined (__sparc__) ||  defined (__PPC__)        || \
    defined (__ppc__)   ||  defined (__BIG_ENDIAN__)
#define NS_ENDIAN NS_ENDIAN_BIG
#else
#define NS_ENDIAN NS_ENDIAN_LITTLE
#endif


#if NS_ENDIAN == NS_ENDIAN_BIG
# define NS_ID(a,b,c,d) ( (int)(a)<<24 | (int)(b)<<16 | (c)<<8 | (d) )
# define IT_ID2(c, d)   ( (c)<<8 | (d) )
#else
# define NS_ID(a,b,c,d) ( (int)(d)<<24 | (int)(c)<<16 | (b)<<8 | (a) )
# define NS_ID2(c, d)   ( (d)<<8 | (c) )
#endif


#define NS_ARCH_32 0
#define NS_ARCH_64 1

#if defined(__x86_64__)     || defined(_M_X64)      || \
    defined(__powerpc64__)  || defined(__alpha__)   || \
    defined(__ia64__)       || defined(__s390__)    || \
    defined(__s390x__)
# define NS_ARCH NS_ARCH_64
NS_ASSERTCOMP(sizeof(void*) == 8, VOID_IS_8);
#else
#define NS_ARCH NS_ARCH_32
NS_ASSERTCOMP(sizeof(void*) == 4, VOID_IS_4);
#endif


#if NS_PLATFORM == NS_PLATFORM_WIN32
# if defined(__MINGW32__) || \
     defined(__CYGWIN__)  || \
     (defined (_MSC_VER) && _MSC_VER < 1300)
#  define NS_INLINE inline
# else
#  define NS_INLINE __forceinline
# endif
#else
#  define NS_INLINE    inline
#endif


// Integer types



typedef long            NSlong;
typedef unsigned long   NSulong;
typedef int             NSint32;
typedef unsigned int    NSuint32;
typedef short           NSint16;
typedef unsigned short  NSuint16;
typedef signed char     NSint8;
typedef unsigned char   NSuint8;
typedef unsigned char   NSubyte;
typedef signed char     NSbyte;
typedef bool            NSint1;


#if NS_COMPILER == NS_COMPILER_GNU
typedef long long          NSint64;
typedef unsigned long long NSuint64;
#else
typedef __int64          NSint64;
typedef unsigned __int64 NSuint64;
#endif



// Arch dependent types



#if NS_ARCH == NS_ARCH_64
typedef NSuint64    NSsize;
typedef NSuint64    NSuintPtr;
typedef NSint64     NSintPtr;
#define NS_NPOS     ((NSuint64)-1)
#else
typedef NSuint32    NSsize;
typedef NSuint32    NSuintPtr;
typedef NSint32     NSintPtr;
#define NS_NPOS     ((NSuint32)-1)
#endif

typedef NSsize NShash;


#endif//_nsUtCommon_h_
