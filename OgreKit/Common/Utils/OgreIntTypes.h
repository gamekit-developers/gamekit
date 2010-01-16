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
#ifndef _OgreIntTypes_h_
#define _OgreIntTypes_h_

#include "OgreUtilsCommon.h"
#include "OgrePlatform.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#    if OGRE_COMPILER == OGRE_COMPILER_GNUC
#        include <stddef.h>
#        include <stdint.h>
#    else
#        include <stddef.h>
#    endif
#elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX
#       include <stddef.h>
#       include <stdint.h>
#else
#    include <inttypes.h>
#endif

#define OGRE_NPOS ((size_t)-1)


#endif//_OgreIntTypes_h_
