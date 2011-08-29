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
#include "gkApplication.h"
#include "gkGameLevel.h"
#include "gkLogger.h"
#include "gkUserDefs.h"
#include "Graphics/gkHUDManager.h"
#include "gkMemoryTest.h"



int main(int argc, char** argv)
{
	TestMemory;


	gkLogger::enable("AppCppDemo.log", GK_CPP_VERBOSE_LOG);


	gkUserDefs prefs;
	prefs.rendersystem = OGRE_RS_GL;
	//prefs.winsize.x = 1360.f;
	//prefs.winsize.y = 768.f;
	//prefs.fullscreen = true;
	prefs.wintitle = "OgreKit C++ Testbed";
	prefs.verbose = GK_CPP_VERBOSE_LOG;
	//prefs.fsaa = true;
	//prefs.debugPhysics = true;
	//prefs.debugFps = true;
	//prefs.grabInput = false;


	gkEngine eng(&prefs);

	//Setup everything. Ogre, gk etc.
	eng.initialize();


	if (!eng.isInitialized())
	{
		gkPrintf("CppDemo: failed to initialize engine.\n");
		return 1;
	}

	{
		gkGameLevel game;
		game.loadLevel(GK_LEVEL_PICKUP);


		//Connect game tick callbacks
		eng.addListener(&game);


		if (!eng.initializeStepLoop())
		{
			gkPrintf("CppDemo: failed to initialize main loop.\n");
			return 1;
		}


		gkPrintf("CppDemo: enter main loop.\n");

		for (;;)
		{
			if (!eng.stepOneFrame())
			{
				gkPrintf("CppDemo: exit main loop.\n");
				break;
			}
		}
	}

	eng.finalize();

	return 0;
}
