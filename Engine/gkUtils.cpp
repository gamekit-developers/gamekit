/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): Nestor Silveira.
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
#include "OgrePlatform.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
# include <windows.h>
#else
# include <unistd.h>
#endif

#include "OgreResourceGroupManager.h"
#include "gkUtils.h"
#include "gkEngine.h"
#include "OgreRoot.h"
#include "gkCamera.h"
#include "gkScene.h"
#include "gkWindowSystem.h"
#include "OgreRenderWindow.h"

using namespace Ogre;

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

    char* bundlePath = AppleGetBundleDirectory();
    //cut off the .app filename
    char* lastSlash = 0;
    if (lastSlash = strrchr(bundlePath, '/'))
        * lastSlash = '\0';
    sprintf(newName, "%s/%s", bundlePath, "game.blend");
    sprintf(newName, "%s/%s/%s", AppleGetBundleDirectory(), "Contents/Resources", in.c_str());



#else

    sprintf(newName, "%s", in.c_str());

#endif

    return newName;
}


bool gkUtils::isResource(const gkString &name, const gkString &group)
{
    if (group.empty())
        return Ogre::ResourceGroupManager::getSingleton().resourceExistsInAnyGroup(name);
    return Ogre::ResourceGroupManager::getSingleton().resourceExists(group, name);
}

Ogre::Ray gkUtils::CreateCameraRay(gkScalar x, gkScalar y)
{
	gkScene* pScene = gkEngine::getSingleton().getActiveScene();

	GK_ASSERT(pScene);

	gkCamera* pCamera = pScene->getMainCamera();

	GK_ASSERT(pCamera);

	Vector2 pos(x, y);

	gkWindowSystem* pWindowSystem = gkWindowSystem::getSingletonPtr();

	Real width = pWindowSystem->getMainWindow()->getWidth();

	Real height = pWindowSystem->getMainWindow()->getHeight();

	GK_ASSERT(width && height);

	Ray ray(pCamera->getCamera()->getCameraToViewportRay(pos.x/width, pos.y/height));

	Vector3 p0 = ray.getOrigin();

	const Real MAX_DISTANCE = 1000000;

	Vector3 p1 = p0 + ray.getDirection() * MAX_DISTANCE;

	ray.setDirection(p1-p0);

	return ray;
}

