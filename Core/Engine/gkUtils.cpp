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
#include "OgrePlatform.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
# include <windows.h>
# include <process.h>
# include <direct.h>
#else
# include <unistd.h>
#endif

#include "OgreResourceGroupManager.h"
#include "gkUtils.h"

#define MAX_FILE_DIR 160
#define MAX_FILE_FILE 80

using namespace Ogre;


// ----------------------------------------------------------------------------
gkPath gkUtils::getHome(void)
{
	gkPath home;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	char buffer[MAX_FILE_DIR + MAX_FILE_FILE];
	GetModuleFileName(0, buffer, MAX_FILE_DIR + MAX_FILE_FILE);
	home= buffer;
#else
	home= getEnvironment("HOME");
#endif
	if (!home.exists())
		return home;


	home= home.directory();
	home.append("OgreKit");
	home.normalizePlatform();

	if (!home.isDir())
		return "";

	return home;
}

// ----------------------------------------------------------------------------
void gkUtils::sleep(int milSec)
{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	::Sleep(milSec);
#else
	if (milSec >= 1000)
	{
		sleep(milSec / 1000);
		milSec= (milSec % 1000);
	}

	usleep(milSec*1000);
#endif
}

// ----------------------------------------------------------------------------
bool gkUtils::isResource(const String &name, const String &group)
{
	if (group.empty())
		return ResourceGroupManager::getSingleton().resourceExistsInAnyGroup(name);
	return ResourceGroupManager::getSingleton().resourceExists(group, name);
}

// ----------------------------------------------------------------------------
String gkUtils::getEnvironment(const String &key)
{
	return getenv(key.c_str());
}
