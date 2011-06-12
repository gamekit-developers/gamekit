/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): none yet.
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
#include "gkCommon.h"
#include "OgrePlatform.h"

#if GK_PLATFORM == GK_PLATFORM_WIN32
# include <windows.h>
#else
# include <unistd.h>
#endif

#include "OgreResourceGroupManager.h"
#include "gkUtils.h"
#include "gkPath.h"

#include "LinearMath/btQuickprof.h"

//using namespace Ogre;

bool gkUtils::IS_LUA_PACKAGE = false;

#define UID_USE_TIME_STAMP 0


#ifdef __APPLE__
#define MAXPATHLEN 512
char* AppleGetBundleDirectory(void)
{
	CFURLRef bundleURL;
	CFStringRef pathStr;
	static char path[MAXPATHLEN];
	memset(path, MAXPATHLEN, 0);
	CFBundleRef mainBundle = CFBundleGetMainBundle();

	bundleURL = CFBundleCopyBundleURL(mainBundle);
	pathStr = CFURLCopyFileSystemPath(bundleURL, kCFURLPOSIXPathStyle);
	CFStringGetCString(pathStr, path, MAXPATHLEN, kCFStringEncodingASCII);
	CFRelease(pathStr);
	CFRelease(bundleURL);
	return path;
}
#endif


gkString gkUtils::getFile(const gkString& in)
{
	char newName[1024];

#ifdef __APPLE__
	FILE* f = fopen(in.c_str(), "rb");
	if (f) //first, check outside of bundle
	{
		fclose(f);
		sprintf(newName, "%s", in.c_str());
	}
	else
	{
		char* bundlePath = AppleGetBundleDirectory();

		//cut off the .app filename
		char* lastSlash = 0;
		if (lastSlash = strrchr(bundlePath, '/'))
			* lastSlash = '\0';

		sprintf(newName, "%s/%s", bundlePath, "game.blend");
		FILE* f = fopen(newName, "rb");

		if (f)
		{
			fclose(f);
		}
		else
		{
#ifdef OGREKIT_BUILD_IPHONE
			sprintf(newName, "%s/%s", AppleGetBundleDirectory(), in.c_str());
#else
			sprintf(newName, "%s/%s/%s", AppleGetBundleDirectory(), "Contents/Resources", in.c_str());
#endif
		}
	}
#else

	sprintf(newName, "%s", in.c_str());

#endif

	return newName;
}


bool gkUtils::isResource(const gkString& name, const gkString& group)
{
	if (group.empty())
		return Ogre::ResourceGroupManager::getSingleton().resourceExistsInAnyGroup(name);
	return Ogre::ResourceGroupManager::getSingleton().resourceExists(group, name);
}

gkString gkUtils::getUniqueName(const gkString& in)
{
#if UID_USE_TIME_STAMP == 1
	static btClock generator;
	static unsigned long prev = -1;
	if (prev = -1)
		generator.reset();

	unsigned long cur = generator.getTimeMicroseconds();
	while (prev == cur)
		prev = generator.getTimeMicroseconds();
	prev = cur = generator.getTimeMicroseconds();
	return in + "/TS{" + Ogre::StringConverter::toString(cur) + "}";
#else

	static unsigned long uidgen = 0;
	return in + "/UID{" + Ogre::StringConverter::toString(uidgen++) + "}";

#endif

}
