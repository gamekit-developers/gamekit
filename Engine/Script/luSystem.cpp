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
#include "gkString.h"
#include "gkTextManager.h"

#include "luSystem.h"
#include "luUtils.h"
#include "luNodeTree.h"
#include "luGameKit.h"
#include "luMath.h"


// System builtin module type
struct luSystem_Builtin {
    const char *    name;
    bool            imported;
    void (*builtin)(ltState*);
};


// System builtin module definition
luSystem_Builtin luSystem_BuiltinModules[] = {
#if ENABLE_NODE_BINDINGS
    {"NodeTree",    false, luNodeTree_Open},
#endif
    {"GameKit",     false, luGameKit_Open},
    {"Math",        false, luMath_Open},
    {0,             false, 0}
};


bool luSyatem_load(ltState *L, const char *name, char *memBuf=0, int memLen=0)
{
    LUA_SCOPE_LOCK;

    if (memBuf)
    {
        if (luaL_loadbuffer(L, memBuf, memLen, name) != 0) {
            printf("%s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
            return false;
        }
    }
    else
    {
        if (luaL_loadfile(L, name) != 0) {
            printf("%s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
            return false;
        }

    }

    if (lua_pcall(L, 0, LUA_MULTRET, 0) != 0) {
        printf("%s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
        return false;
    }
    return true;
}


// Module lookup
luSystem_Builtin luSystem_FinmdModule(const char *name, luSystem_Builtin *modules)
{
    while (modules && modules->name) {
        if (utCharEq(modules->name, name))
            return *modules;
        ++modules;
    }
    luSystem_Builtin b = {0, 0, false};
    return b;
}


// Module/script Impoter
static int luSystem_import(luObject &L)
{
    luString import = L.getValueString(1);

    luSystem_Builtin v = luSystem_FinmdModule(import.c_str(), luSystem_BuiltinModules);
    if (v.builtin && !v.imported) {

        // execute builtin reg 
        v.builtin(L);
        v.imported = true;
        return 0;

    } else if (v.name != 0) {
        // is a todo
        return 0;

    } else if (v.imported) 
        return 0;

    if (gkTextManager::getSingleton().hasFile(import))
    {
        // import internal text file
        gkTextFile *fp = gkTextManager::getSingleton().getFile(import);

        if (!luSyatem_load(L, import.c_str(), const_cast<char*>(fp->getText().c_str()), fp->getSize()))
            return -1;
        return 0;
    }
    // external source

    if (!luSyatem_load(L, import.c_str()))
        return -1;

    return 0;
}


luMethodDef luSystem_Methods[] = 
{
    {"import", luSystem_import, LU_PARAM, "s"},
    {0, 0}
};

// Lua System module
void luSystem_Open(ltState *L)
{
    luBinder lua(L);

    // System table
    lua.beginNamespace("System");
    lua.addMethods(luSystem_Methods);
    lua.endNamespace();
}
