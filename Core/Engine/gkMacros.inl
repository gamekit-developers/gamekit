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


// ----------------------------------------------------------------------------
#define GK_IMPLEMENT_SINGLETON(cls)\
	template<> cls* Ogre::Singleton<cls>::ms_Singleton= 0;\
	cls& cls::getSingleton() {  \
		assert(ms_Singleton);   \
		return *ms_Singleton;   \
	}\
	cls* cls::getSingletonPtr(){ \
		return ms_Singleton;\
	}


// ----------------------------------------------------------------------------
#define GK_INLINE inline

#define GK_NPOS (0xFFFFFFFF)

// ----------------------------------------------------------------------------
#if defined(DEBUG) || defined(_DEBUG)
# include <assert.h>
# define GK_ASSERT(x) assert(x)
#else
# define GK_ASSERT(x)
#endif


// ----------------------------------------------------------------------------
#define gkMakeHandle(name) typedef struct name##__ { int unused; } *name

#define GK_DEF_GROUP "General"
