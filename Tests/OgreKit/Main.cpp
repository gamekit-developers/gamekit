/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2009 Erwin Coumans & Charlie C
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
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include "OgreBlend.h"
#include "gkEngine.h"
#include "gkUserDefs.h"
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"


#ifdef __APPLE__
#define MAXPATHLEN 512
char* AppleGetBundleDirectory(void) {
	CFURLRef bundleURL;
	CFStringRef pathStr;
	static char path[MAXPATHLEN];
	memset(path,MAXPATHLEN,0);
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	
	bundleURL = CFBundleCopyBundleURL(mainBundle);
	pathStr = CFURLCopyFileSystemPath(bundleURL, kCFURLPOSIXPathStyle);
	CFStringGetCString(pathStr, path, MAXPATHLEN, kCFStringEncodingASCII);
	CFRelease(pathStr);
	CFRelease(bundleURL);
	return path;
}
#endif


Ogre::String getRenderSystemString(OgreRenderSystem sys)
{
	switch (sys)
	{
	case OGRE_RS_D3D9:	return "Direct3D 9";
	case OGRE_RS_D3D10: return "Direct3D 10";
	case OGRE_RS_GL:	return "OpenGL";
	case OGRE_RS_GLES:	return "OpenGL ES";
	}
	return "unknown render system";
}

int main(int argc, char **argv)
{
	char *fname= "momo_ogreSmallAnim.blend";


#if __APPLE__
	char newName[1024];
	
	char* bundlePath= AppleGetBundleDirectory();
	//cut off the .app filename
	char* lastSlash=0;
	if (lastSlash= strrchr(bundlePath, '/'))
		*lastSlash= '\0';
	sprintf(newName,"%s/%s",bundlePath,"game.blend");
	//	eng.loadBlendFile(newName);
	// how do you debug the Bundle execution, without a console?

	sprintf(newName,"%s/%s/%s",AppleGetBundleDirectory(),"Contents/Resources",fname);
	fname= newName;
	//FILE* dump= fopen ("out.txt","wb");
	//fwrite(newName,1,strlen(newName),dump);
	//fclose(dump);

#else
	if (argc > 1)
		fname= argv[argc-1];
#endif


	btCollisionConfiguration* collisionConfiguration= new btDefaultCollisionConfiguration();
	btBroadphaseInterface* pairCache= new btDbvtBroadphase();
	btDispatcher* dispatcher= new btCollisionDispatcher(collisionConfiguration);
	btConstraintSolver* constraintSolver= new btSequentialImpulseConstraintSolver();
	btDiscreteDynamicsWorld* physicsWorld= new btDiscreteDynamicsWorld(dispatcher,pairCache,constraintSolver,collisionConfiguration);


	gkEngine *eng = new gkEngine();
	eng->getUserDefs().rendersystem = gkFindRenderSystem(OGRE_RS_D3D9);
	eng ->initialize(false);
	if (eng->isInitialized())
	{
		OgreBlend *blend= new OgreBlend(physicsWorld);
		
		int status= blend->_readFile(fname);
		if (status)
		{
			blend->convertAllObjects();


			eng->initializeWindow(Ogre::String("OgreKit Test (Press Q to exit) [") + fname + "] - " + 
				getRenderSystemString(eng->getUserDefs().rendersystem), 800, 600, false);
			//eng->initializeWindow("", 1360, 768, true);

			static_cast<OgreBlend*>(blend)->load();
			eng->run();
		}

		delete blend;
	}

	delete eng;
	delete physicsWorld;
	delete constraintSolver;
	delete dispatcher;
	delete pairCache;
	delete collisionConfiguration;
	return 0;
}
