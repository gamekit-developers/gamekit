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
#include "gkLuaManager.h"
#include "gkLuaScript.h"
#include "gkLuaUtils.h"
#include "gkDebugScreen.h"



// ----------------------------------------------------------------------------
gkLuaScript::gkLuaScript(gkLuaManager *parent, const utString &name, const utString &buffer)
    :       m_script(0), m_name(name), m_text(buffer), m_compiled(false),m_isInvalid(false), m_owner(parent)
{
}

// ----------------------------------------------------------------------------
gkLuaScript::~gkLuaScript()
{
    lua_State *L = m_owner->getLua();
    if (m_script != -1)
        luaL_unref(L, LUA_REGISTRYINDEX, m_script);
}


// ----------------------------------------------------------------------------
void gkLuaScript::unload(void)
{
    lua_State *L = m_owner->getLua();
    if (m_script != -1)
        luaL_unref(L, LUA_REGISTRYINDEX, m_script);
    m_script = -1;
    m_compiled = false;
    m_isInvalid = false;
    lua_gc(L, LUA_GCCOLLECT, 0);

}

// ----------------------------------------------------------------------------
void gkLuaScript::compile(void)
{
    if (m_isInvalid)
        return;

    lua_State *L = m_owner->getLua();
    //lua_dumpstack(L);
    {
        lua_pushvalue(L, LUA_GLOBALSINDEX);
        if (luaL_loadbuffer(L, m_text.c_str(), m_text.size()-1, m_name.c_str()) != 0)
        {
            printf("%s\n", lua_tostring(L, -1));
            dsPrintf("%s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
            m_isInvalid = true;
            return;
        }


        // script function is now on the stack
        int fnc = lua_gettop(L);
        if (!lua_isfunction(L, fnc))
        {
            lua_pop(L, 1);
            m_isInvalid = true;
            return;
        }

        lua_pushvalue(L, fnc);
        m_script = luaL_ref(L, LUA_REGISTRYINDEX);
        lua_popall(L);
    }
    m_compiled = true;
}



// ----------------------------------------------------------------------------
bool gkLuaScript::execute(void)
{
    if (!m_compiled)
        compile();

    if (m_isInvalid) 
        return false;

    lua_State *L = m_owner->getLua();
    //lua_dumpstack(L);

    lua_rawgeti(L, LUA_REGISTRYINDEX, m_script);
    if (lua_pcall(L, 0, 0, 0) != 0)
    {
        printf("%s\n", lua_tostring(L, -1));
        dsPrintf("%s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
        m_isInvalid = true;
        return false;
    }
    lua_gc(L, LUA_GCSTEP, 1);
    lua_popall(L);
    return true;
}
