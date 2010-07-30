/*
-------------------------------------------------------------------------------
--                                                                           --
--  This file is part of GameKit.                                            --
--  http://gamekit.googlecode.com/                                           --
--                                                                           --
--                                                                           --
--  Released under the zlib license.                                         --
--  http://www.gzip.org/zlib/zlib_license.html                               --
--                                                                           --
--                                                                           --
-------------------------------------------------------------------------------
*/
#include "gkUtils.h"
#include "Script/Lua/gkLuaUtils.h"

#if UT_PLATFORM == UT_PLATFORM_WIN32
# define OGREKIT_EXPORT extern "C" __declspec(dllexport)
#else
# define OGREKIT_EXPORT extern "C"
#endif

extern "C" int _OgreKitLua_install(lua_State *L);

OGREKIT_EXPORT int luaopen_OgreKitLua(lua_State *L)
{
    gkUtils::IS_LUA_PACKAGE = true;

    // Open standalone 
    return _OgreKitLua_install(L);
}
