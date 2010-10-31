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
#include "gkLuaManager.h"
#include "gkLuaScript.h"
#include "gkLuaUtils.h"
#include "gkDebugScreen.h"
#include "gkLogger.h"




gkLuaScript::gkLuaScript(gkLuaManager* parent, const utString& name, const utString& buffer)
	:       m_script(0), m_name(name), m_text(buffer), m_compiled(false), m_isInvalid(false), m_owner(parent)
{
}


gkLuaScript::~gkLuaScript()
{
	lua_State* L = m_owner->getLua();
	if (m_script != -1)
		luaL_unref(L, LUA_REGISTRYINDEX, m_script);
}



void gkLuaScript::decompile(void)
{
	lua_State* L = m_owner->getLua();
	if (m_script != -1)
		luaL_unref(L, LUA_REGISTRYINDEX, m_script);
	m_script = -1;
	m_compiled = false;
	m_isInvalid = false;
	lua_gc(L, LUA_GCCOLLECT, 0);

}


void gkLuaScript::compile(void)
{
	if (m_isInvalid)
		return;

	lua_State* L = m_owner->getLua();
	//lua_dumpstack(L);
	{
		lua_pushvalue(L, LUA_GLOBALSINDEX);
		if (luaL_loadbuffer(L, m_text.c_str(), m_text.size() - 1, m_name.c_str()) != 0)
		{
			gkPrintf("%s\n", lua_tostring(L, -1));
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




bool gkLuaScript::execute(void)
{
	if (!m_compiled)
		compile();

	if (m_isInvalid)
		return false;

	lua_State* L = m_owner->getLua();
	//lua_dumpstack(L);
	lua_pushtraceback(L);
	int trace = lua_gettop(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, m_script);
	if (lua_pcall(L, 0, LUA_MULTRET, trace) != 0)
	{
		gkPrintf("%s\n", lua_tostring(L, -1));
		dsPrintf("%s\n", lua_tostring(L, -1));
		lua_pop(L, 1);
		m_isInvalid = true;
		return false;
	}
	lua_gc(L, LUA_GCSTEP, 1);
	lua_popall(L);
	return true;
}
