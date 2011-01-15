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
#include "gkMemoryTest.h"
#include "Script/Lua/gkLuaUtils.h"
#include "gkLogger.h"
#include "gkUtils.h"
#include "opts.h"

#ifdef WIN32
#include <direct.h>
#endif

extern "C" int _OgreKitLua_install(lua_State* L);


/// Interpreter, for a builtin OgreKitLua executable
/// This is a work around for not having a Lua system package installed.

#define DEFAULT_LUA_FILE "Test1.lua"


int main(int argc, char** argv)
{
	TestMemory;

	const char* startLuaFile = DEFAULT_LUA_FILE;
	char* workingDir = 0;
	char* luaFile = 0; 
	char* logFile = 0;
	int pauseFlag = 0;
	int helpFlag = 0;
	
	option options[] = 
	{
		{ OTYPE_STR, 'd', "dir", "working directory", OFLAG_NONE, &workingDir, 0, 0, 0 },
		{ OTYPE_STR, 's', "lua", "start lua file", OFLAG_NONE, &luaFile, 0, 0, 0 },
		{ OTYPE_STR, 'l', "log", "log file", OFLAG_NONE, &logFile, 0, 0, 0 },
		{ OTYPE_BOL, 'p', "pause", "pause flags", OFLAG_NONE, &pauseFlag, 0, 0, 0 },
		{ OTYPE_BOL, 'h', "help", "print usage", OFLAG_NONE, &helpFlag, 0, 0, 0 },
		{ OTYPE_END, '\0', "", "", OFLAG_NONE, 0, 0, 0, 0 }, //end of options
	};

	optsetstyle(0);

	int ret = optsgets(argc, argv, options);

	if (helpFlag != 0 || (ret != 0 && optsind >= argc))
	{
		printf("Usage: %s\n", argv[0]);
		for (option* op = &options[0]; op->type; ++op)
			if (!((op->type == OTYPE_NUL) && (op->flags & OFLAG_ARG)))
				printf("\t%s\n", optsusage(op));
		printf("\t[lua arguments ...]\n");
		
		optsclean(options);
		
		return 1;
	}

	if (luaFile)
		startLuaFile = luaFile;
	else
	{
		if (optsind < argc && strstr(argv[optsind], "lua"))
			startLuaFile = argv[optsind++];
	}

	if (workingDir)
	{
		printf("Set working directory: %s\n", workingDir);
#ifdef WIN32
		_chdir(workingDir);
#else
		chdir(workingDir);
#endif
	}

	if (logFile)
	{
		gkLogger::enable(logFile, false);
	}
		

	lua_State* L = lua_open();

	// standard libraries
	luaL_openlibs(L);

	// install local code
	_OgreKitLua_install(L);


	// set up cmd line
	lua_pushvalue(L, LUA_GLOBALSINDEX);
	lua_pushstring(L, "arg");
	lua_newtable(L);

	//remaining args
	for (int i = optsind; i < argc; ++i) 
	{
		lua_pushnumber(L, i - optsind + 1); //start index: 1
		lua_pushstring(L, argv[i]);
		lua_settable(L, -3);
	}

	lua_settable(L, -3);

	// pop global
	lua_pop(L, 1);

	UT_ASSERT(lua_gettop(L) == 0);

	lua_pushtraceback(L);
	int tb = lua_gettop(L);

	int status = luaL_loadfile(L, gkUtils::getFile(startLuaFile).c_str());

	if (status != 0)
	{
		gkPrintf("%s\n", lua_tostring(L, -1));
		lua_pop(L, 1);
	}
	else
	{
		if (lua_pcall(L, 0, LUA_MULTRET, tb) != 0)
		{
			gkPrintf("%s\n", lua_tostring(L, -1));
			lua_pop(L, 1);
		}
	}

	lua_close(L);

	optsclean(options);

#ifdef WIN32
	if (pauseFlag) system("PAUSE");
#endif

	return 0;
}
