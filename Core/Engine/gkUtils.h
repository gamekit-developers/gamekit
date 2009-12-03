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

#ifndef _gkUtils_h_
#define _gkUtils_h_


#include "gkCommon.h"
#include "gkPath.h"



// ----------------------------------------------------------------------------
class gkUtils
{
public:
	// call platform sleep
	static void sleep(int milSec);
	static bool isResource(const Ogre::String &name, const Ogre::String &group=Ogre::StringUtil::BLANK);

	static gkPath getHome(void);
	// get environment variable
	static Ogre::String getEnvironment(const Ogre::String &key);
};






#endif//_gkUtils_h_
