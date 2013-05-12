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
#include "tclap/CmdLine.h"

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

	std::string startLuaFile = DEFAULT_LUA_FILE;

	std::string workingDir = "";
	std::string luaFile = ""; 
	std::string logFile = "";
	bool pauseFlag = false;
	std::vector<std::string> extraArgs;
	
	try
	{
		TCLAP::CmdLine cmdl("LuaRuntime", ' ', "n/a");
		cmdl.setExceptionHandling(false);
		//cmdl.ignoreUnmatched(true);

		TCLAP::ValueArg<std::string>			workingdir_arg	("d", "dir",	"working directory", false, workingDir, "string"); 
		TCLAP::ValueArg<std::string>			luafile_arg		("s", "lua",	"start lua file", false, luaFile, "string"); 
		TCLAP::ValueArg<std::string>			logfile_arg		("l", "log",	"log file", false, logFile, "string"); 
		TCLAP::ValueArg<bool>					pause_arg		("p", "puase",	"pause on exit", false, pauseFlag, "bool"); 
		TCLAP::UnlabeledMultiArg<std::string>	extra_arg		("extra",		"extra args", false, "strings", true);

		cmdl.add(workingdir_arg);
		cmdl.add(luafile_arg);
		cmdl.add(logfile_arg);
		cmdl.add(pause_arg);
		cmdl.add(extra_arg);

		cmdl.parse(argc, argv);

		workingDir = workingdir_arg.getValue();
		luaFile = luafile_arg.getValue();
		logFile = logfile_arg.getValue();
		pauseFlag = pause_arg.getValue();

		extraArgs = extra_arg.getValue();
	}
	catch (TCLAP::ArgException& e)
	{
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
		return -1;
	}
	catch (TCLAP::ExitException&)
	{
		// just return and exit app
		return -1;
	}
	catch (...)
	{
		std::cerr << "Unknown exception." << std::endl;
		return -1;
	}


	if (!luaFile.empty())
		startLuaFile = luaFile;
	else
	{
		if (!extraArgs.empty() && extraArgs[0].find("lua") != std::string::npos)
			startLuaFile = extraArgs[0];			
	}

	if (!workingDir.empty())
	{
		printf("Set working directory: %s\n", workingDir.c_str());
#ifdef WIN32
		_chdir(workingDir.c_str());
#else
		chdir(workingDir.c_str());
#endif
	}

	if (!logFile.empty())
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
	for (size_t i = 0; i < extraArgs.size(); i++) 
	{
		printf("lua args: %d %s\n", i, extraArgs[i].c_str());
		lua_pushnumber(L, i + 1); //start index: 1
		lua_pushstring(L, extraArgs[i].c_str());
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

#ifdef WIN32
	if (pauseFlag) system("PAUSE");
#endif

	return 0;
}
