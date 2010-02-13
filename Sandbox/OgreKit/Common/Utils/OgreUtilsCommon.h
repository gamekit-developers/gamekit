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
#ifndef _OgreUtilsCommon_h_
#define _OgreUtilsCommon_h_

#include "OgrePlatform.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
# if defined(__MINGW32__) || defined(__CYGWIN__) || (defined (_MSC_VER) && _MSC_VER < 1300)
#  define OGRE_INLINE inline
# else
#  define OGRE_INLINE __forceinline
# endif
#else
#  define OGRE_INLINE inline
#endif

#define OKIT_EXCEPT_ASSERT 0

#if defined(DEBUG) || defined(_DEBUG)
# include <assert.h>
# define OGRE_DEBUG 1
# if OKIT_EXCEPT_ASSERT == 0
#  define OGRE_ASSERT(x) assert(x)
# else
#  include "OgreException.h"
#  define OGRE_ASSERT(x) if (!(x)) OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, #x" assertion failed", __FILE__)
# endif
#else
# define OGRE_DEBUG 0
# if OKIT_EXCEPT_ASSERT == 0
#  define OGRE_ASSERT(x)
# else
#  include "OgreException.h"
#  define OGRE_ASSERT(x) if (!(x)) OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE,  #x"assertion failed", __FILE__)
# endif
#endif


#endif//_OgreUtilsCommon_h_
