/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 harkon.kr

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


#include "StdAfx.h"

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
};


#include "liUtils.h"
#include "liLuaUtils.h"


lua_State* LUA_init(bool useLibs);
void LUA_uninit(lua_State* L);

gkString LUA_popStr(lua_State* L); //pop stack top gkString
gkString LUA_getGlobalStr(lua_State* L, const gkString& name); //return gkString from global table

bool LUA_call(lua_State* L, const gkString& filename, gkString& error); //call from file
bool LUA_call(lua_State* L, const gkString& buf, const gkString& name, gkString& error); //call from buffer

void LUA_dumpStack(lua_State* L); //dump stack

//--

void LUA_defLog(const char* str) { gkPrintf(str); }

LUA_log gLOG = LUA_defLog;

LUA_log LUA_getLog() { return gLOG; }

void LUA_setLog(LUA_log log)
{
	gLOG = log ? log : LUA_defLog;
}

//--

gkString LUA_popStr(lua_State* L)
{
	GK_ASSERT(L); if (!L) return "";

	if (lua_gettop(L) <= 0 || !luaL_checkstring(L, -1)) return "";

	gkString msg(lua_tostring(L, -1));
	lua_pop(L, 1);
	return msg;
}

int CPRINT(lua_State* L)
{
	gkString msg = LUA_popStr(L);

	//gLOG("%s", msg.c_str());
	gLOG(msg.c_str());

	return 0;
}

int LUA_traceback(lua_State* L)
{
	GK_ASSERT(L); if (!L) return 1;

	lua_getfield(L, LUA_GLOBALSINDEX, "debug");
	if (!lua_istable(L, -1)) 
	{
		lua_pop(L, 1);
		return 1;
	}

	lua_getfield(L, -1, "traceback");
	if (!lua_isfunction(L, -1)) 
	{
		lua_pop(L, 2);
		return 1;
	}

	lua_pushvalue(L, 1);   // pass error message
	lua_pushinteger(L, 2); // skip this function and traceback
	lua_call(L, 2, 1);     // call debug.traceback
	return 1;
}

//support BOM encoding text file
bool LUA_loadfile(lua_State* L, const gkString& filename, gkString& text)
{
	GK_ASSERT(L); if (!L) return false;

	FILE *fp = fopen(filename.c_str(), "rt");
	if (!fp) return false;

	fseek(fp, 0, SEEK_END);
	long len = ftell(fp);
	if (len <= 0) return false; //empty file?
	fseek(fp, 0, SEEK_SET);

	char *buf = new char[len+1];
	buf[len]=0;
	size_t rlen = fread(buf, sizeof(char), len, fp);
	if (rlen == 0) 
	{
		SAFE_DELETE_ARRAY(buf);
		return false;
	}
	buf[rlen]=0;


	//strip BOM
	size_t blen = 0;
	BOM bom = getTextEncoding(buf, rlen);
	if (bom != BOM_NONE)
		blen = getBOMLength(bom);

	text = (buf+blen);

	SAFE_DELETE_ARRAY(buf);
	fclose(fp); fp = NULL;

	return true;
}

void LUA_dumpStack(lua_State* L)
{
	GK_ASSERT(L); if (!L) return;

	gkString dump;

	int count = lua_gettop(L);
	dump += utStringFormat("[LUA] dump stack count: %d\n", count);

	for (int i = 1; i <= count; i++)
	{
		dump += utStringFormat("[%d] ", i);
		int t = lua_type(L, i);
		switch (t)
		{
		case LUA_TSTRING:
			dump += utStringFormat("'%s' ", lua_tostring(L, i)); break;
		case LUA_TBOOLEAN:
			dump += utStringFormat("%s ", lua_toboolean(L, i) ? "true" : "false"); break;
		case LUA_TNUMBER:
			dump += utStringFormat("%g ", lua_tonumber(L, i)); break;
		default:
			dump += utStringFormat("%s ", lua_typename(L, t)); break;
		}
	}

	gLOG(dump.c_str());
}

//--

gkString LUA_getGlobalStr(lua_State* L, const gkString& name)
{
	GK_ASSERT(L); if (!L) return "";

	int top = lua_gettop(L);
	lua_getglobal(L, name.c_str());

	gkString value;

	if (top < lua_gettop(L) && lua_isstring(L, -1))
	{
		const char *str = lua_tostring(L, -1);
		if (str) value = str;
	}

	lua_settop(L, top);  //clear stack

	return value;
}



//call from file
bool LUA_call(lua_State* L, const gkString& filename, gkString& error)
{
	GK_ASSERT(L); if (!L) return false;

	error.empty();

	gkString text;

	if (!LUA_loadfile(L, filename, text)) 
	{
		error = utStringFormat("[LUA] ERROR: can't load lua file: '%s'", filename.c_str());
		gLOG(error.c_str());
		return false;
	}

	return LUA_call(L, text, filename, error);
}


//call from buffer
bool LUA_call(lua_State* L, const gkString& buf, const gkString& name, gkString& error)
{
	GK_ASSERT(L); if (!L) return false;

	error.empty();

	if (luaL_loadbuffer(L, buf.c_str(), buf.size(), name.c_str()))
	{
		gkString msg = LUA_popStr(L);

		error = utStringFormat("[LUA] ERROR: can't load lua buf: '%s' '%s'", name.c_str(), msg.c_str());
		gLOG(error.c_str());
		return false;
	}

	int base = lua_gettop(L); //base: loadbuffer,counts of all stack elements
	lua_pushcfunction(L, LUA_traceback); //base+1
	lua_insert(L, base); //swap(base,base+1)

	//base: error handler
	int err = lua_pcall(L, 0, 0, base); //ret if 0: succ else: return error code
	lua_remove(L, base); //remove traceback function

	if (err != 0)
	{
		error = utStringFormat("[LUA] ERROR: %s (ret: %d)", lua_tostring(L, -1), err);
		gLOG(error.c_str());
		lua_pop(L, 1); //pop one-elements(error msg)
		return false;
	}

	return true;
}

int LUA_getTable(lua_State* L, const gkString& table) //return old top for stack clean, if error return -1
{
	GK_ASSERT(L); if (!L) return -1;

	int top = lua_gettop(L);
	lua_getglobal(L, table.c_str());
	if (!lua_istable(L, -1))
	{
		lua_settop(L, top);
		gLOG(utStringFormat("[LUA] ERROR: %s isn't table", table.c_str()).c_str());
		return -1;
	}

	return top;
}

gkString LUA_getfield(lua_State* L, const gkString& table, const gkString& field)
{
	int top = LUA_getTable(L, table);
	if (top < 0) return "";

	lua_getfield(L, -1, field.c_str());

	gkString value;

	if (top < lua_gettop(L) - 1 && lua_isstring(L, -1))
	{
		const char *str = lua_tostring(L, -1);
		if (str) value = str;
	}

	lua_settop(L, top);

	return value;
}

gkString LUA_getfield(lua_State* L, const gkString& table, int index)
{
	int top = LUA_getTable(L, table);
	if (top < 0) return "";

	lua_pushinteger(L, index);
	lua_gettable(L, -2);

	gkString value;

	if (top < lua_gettop(L) - 1 && lua_isstring(L, -1))
	{
		const char *str = lua_tostring(L, -1);
		if (str) value = str;
	}

	lua_settop(L, top);

	return value;
}

int LUA_getTableSize(lua_State* L, const gkString& table)
{
	int top = LUA_getTable(L, table);
	if (top < 0) return 0;

	int size = (int)lua_objlen(L, -1);

	lua_settop(L, top);
	return size;
}

//--

lua_State* LUA_init(bool useLibs)
{
	lua_State* L = lua_open();
	if (!L)
	{
		gLOG("[LUA] ERROR: initLua() is failed");
		return NULL;
	}

	if (useLibs)
		luaL_openlibs(L);

	// register common func
	lua_register(L, "print", CPRINT);

	return L;
}

void LUA_uninit(lua_State* L)
{
	if (!L) return;

	lua_close(L);
}

//--

class luLuaLog
{
	LUA_log m_olog;
public:
	luLuaLog(LUA_log log) : m_olog(LUA_getLog()) { LUA_setLog(log); }
	~luLuaLog() { LUA_setLog(m_olog); }
};

#define SET_LUALOG luLuaLog _log(m_log)

liLuaScript::liLuaScript(bool useLibs, LUA_log log) : m_L(NULL), m_log(log)
{
	SET_LUALOG;

	m_L = LUA_init(useLibs);

	if (!m_L)
		gLOG("[LUA] Can't Init Lua Scripts.");
}

liLuaScript::~liLuaScript()
{
	SET_LUALOG;

	if (m_L)
		LUA_uninit(m_L);
}

//pop stack top gkString
gkString liLuaScript::popStr()
{
	SET_LUALOG;

	return LUA_popStr(m_L);
}

//return gkString from global table
gkString liLuaScript::getGlobalStr(const gkString& name)
{
	SET_LUALOG;

	return LUA_getGlobalStr(m_L, name);
}

//call from file
bool liLuaScript::call(const gkString& filename)
{
	SET_LUALOG;

	return LUA_call(m_L, filename, m_error);
}

//call from buffer
bool liLuaScript::call(const gkString& buf, const gkString& name)
{
	SET_LUALOG;

	return LUA_call(m_L, buf, name, m_error);
}

//dump stack
void liLuaScript::dumpStack()
{
	SET_LUALOG;

	LUA_dumpStack(m_L);
}

int liLuaScript::getTop() const
{
	SET_LUALOG;

	return lua_gettop(m_L);
}

int liLuaScript::getTableSize(const gkString& table)
{
	SET_LUALOG;

	return LUA_getTableSize(m_L, table);
}

gkString liLuaScript::getTableStr(const gkString& table, const gkString& field)
{
	SET_LUALOG;

	return LUA_getfield(m_L, table, field);
}

gkString liLuaScript::getTableStr(const gkString& table, int index)
{
	SET_LUALOG;

	return LUA_getfield(m_L, table, index);
}
