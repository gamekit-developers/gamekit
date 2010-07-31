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
#include "Script/Api/Generated/OgreKitTemplates.h"


extern "C" int _OgreKitLua_install(lua_State *L);


/// Interpreter, for a builtin OgreKitLua executable
/// This is a work around for not having a Lua system package installed.


int main(int argc, char **argv)
{
	TestMemory;

	if (argc < 2)
	{
		printf("Usage: %s file.lua [file.lua arguments]\n", argv[0]);
		return 1;
	}

	lua_State *L = lua_open();

	// standard libraries
	luaL_openlibs(L);

	// install local code
	_OgreKitLua_install(L);


	// set up cmd line
	lua_pushvalue(L, LUA_GLOBALSINDEX);
	lua_pushstring(L, "arg");
	lua_newtable(L);

	int nr = argc-1;
	for (int i=nr; i>=0; --i)
	{
		lua_pushnumber(L, i-1);
		lua_pushstring(L, argv[i]);
		lua_settable(L, -3);
	}

	lua_settable(L, -3);

	// pop global
	lua_pop(L, 1);

	UT_ASSERT(lua_gettop(L) == 0);

	lua_pushtraceback(L);
	int tb = lua_gettop(L);

	int status = luaL_loadfile(L, argv[1]);
	if (status != 0)
	{
		printf("%s\n", lua_tostring(L, -1));
		lua_pop(L, 1);
	}
	else
	{
		if (lua_pcall(L, 0, LUA_MULTRET, tb) != 0)
		{
			printf("%s\n", lua_tostring(L, -1));
			lua_pop(L, 1);
		}
	}

	lua_close(L);
	return 0;
}
