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
#include "gkLuaUtils.h"
#include "utString.h"
#include "gkLogger.h"
#include "Generated/gsTemplates.h"


void install_Templates(lua_State* L)
{
	// Local built-in .lua bindings

	lua_pushvalue(L, LUA_GLOBALSINDEX);
	luaL_dostring(L, SYSTEM); // printf, dPrintf
	luaL_dostring(L, CLASS);  // Class, BaseClass
	lua_pop(L, 1);
}

extern "C" int luaopen_OgreKit(lua_State* L);


extern "C" int _OgreKitLua_install(lua_State* L)
{
	install_Templates(L);
	return luaopen_OgreKit(L);
}



int lua_pushtraceback(lua_State* L)
{
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
	return 2;
}


gkLuaEvent::gkLuaEvent(gkLuaCurState fnc)
	:   L(fnc.L), m_self(0), m_callback(0), m_callArgs(0), m_trace(-1), m_error(false)
{
	m_callback = new gkLuaObject(L, fnc.m_id);
}



gkLuaEvent::gkLuaEvent(gkLuaCurState self, gkLuaCurState fnc)
	:   L(fnc.L), m_self(0), m_callback(0), m_callArgs(0), m_trace(-1), m_error(false)
{
	m_self = new gkLuaObject(L, self.m_id);
	m_callback = new gkLuaObject(L, fnc.m_id);
}


gkLuaEvent::~gkLuaEvent()
{
	delete m_self;
	delete m_callback;
}



gkLuaEvent* gkLuaEvent::clone(void)
{
	gkLuaEvent* evt = new gkLuaEvent(*this);
	evt->m_self = m_self ? new gkLuaObject(*m_self) : 0;
	evt->m_callback = m_callback ? new gkLuaObject(*m_callback) : 0;
	return evt;
}



void gkLuaEvent::beginCall(void)
{

	if (m_error || !L || !m_callback) return;

	m_callArgs = 0;

	lua_pushtraceback(L);
	m_trace = lua_gettop(L);


	// push callback function
	lua_rawgeti(L, LUA_REGISTRYINDEX, m_callback->get());

	if (m_self)
	{
		lua_rawgeti(L, LUA_REGISTRYINDEX, m_self->get());
		assert(lua_istable(L, -1));
		m_callArgs++;
	}

}


void gkLuaEvent::addArgument(bool val)
{
	if (m_error) return;

	lua_pushboolean(L, val ? 1 : 0);
	++m_callArgs;
}


void gkLuaEvent::addArgument(int val)
{
	if (m_error) return;

	lua_pushnumber(L, (lua_Number)val);
	++m_callArgs;
}


void gkLuaEvent::addArgument(float val)
{
	if (m_error) return;

	lua_pushnumber(L, (lua_Number)val);
	++m_callArgs;
}


bool gkLuaEvent::call(bool& result)
{

	result = false;
	if (m_error) return false;
	if (m_callArgs == 0) return false;

	if (lua_pcall(L, m_callArgs, 1, m_trace) != 0)
	{
		gkPrintf("%s\n", lua_tostring(L, -1));
		// re throw
		// lua_error(L);
		lua_pop(L, 1);
		m_error = true;

		return false;
	}

	result = lua_toboolean(L, -1) != 0;
	m_callArgs = 0;
	return true;
}


bool gkLuaEvent::call()
{
	if (m_error) return false;
	if (m_callArgs == 0) return false;

	if (lua_pcall(L, m_callArgs, 0, m_trace) != 0)
	{
		gkPrintf("%s\n", lua_tostring(L, -1));
		// re throw
		// lua_error(L);
		lua_pop(L, 1);
		m_error = true;
		return false;
	}
	m_callArgs = 0;
	return true;
}


void lua_popall(lua_State* L)
{
	int t = lua_gettop(L);
	if (t > 0)
		lua_pop(L, t);
}


void lua_dumpstack(lua_State* L)
{
	int top = lua_gettop(L), i = 0;
	while (top > 0)
	{
		char extra[256];
		extra[0] = 0;
		
		switch (lua_type(L, top))
		{
		case LUA_TNONE:             sprintf(extra, "LUA_TNONE               : NONE"); break;
		case LUA_TNIL:              sprintf(extra, "LUA_TNIL                : NULL"); break;
		case LUA_TBOOLEAN:          sprintf(extra, "LUA_TBOOLEAN            : %s", lua_toboolean(L, top) ? "true" : "false"); break;
		case LUA_TLIGHTUSERDATA:    sprintf(extra, "LUA_TLIGHTUSERDATA      : %p", lua_touserdata(L, top)); break;
		case LUA_TUSERDATA:         sprintf(extra, "LUA_TUSERDATA           : %p", lua_touserdata(L, top)); break;
		case LUA_TNUMBER:           sprintf(extra, "LUA_TNUMBER             : %f", lua_tonumber(L, top));   break;
		case LUA_TSTRING:           sprintf(extra, "LUA_TSTRING             : %s", lua_tostring(L, top));   break;
		case LUA_TTABLE:            sprintf(extra, "LUA_TTABLE              : %p", lua_topointer(L, top));  break;
		case LUA_TFUNCTION:         sprintf(extra, "LUA_TFUNCTION           : %p", lua_topointer(L, top));  break;
		case LUA_TTHREAD:           sprintf(extra, "LUA_TTHREAD             : %p", lua_topointer(L, top));  break;
		}

		gkPrintf("%i: %s\n", i, extra);
		i++;
		top--;
	}
}


void lua_beginnamespace(lua_State* L, const char* nsp)
{
	lua_pushstring(L, nsp);
	lua_newtable(L);
}


void lua_endnamespace(lua_State* L)
{
	lua_settable(L, -3);
}


void lua_addconstant(lua_State* L, const char* name, double v)
{
	lua_pushnumber(L, (lua_Number)v);
	lua_setfield(L, -2, name);
}


void lua_addconstant(lua_State* L, const char* name, float v)
{
	lua_addconstant(L, name, (double)v);
}


void lua_addconstant(lua_State* L, const char* name, int v)
{
	lua_addconstant(L, name, (double)v);
}


void lua_addconstant(lua_State* L, const char* name, const char* v)
{
	lua_pushstring(L, v);
	lua_setfield(L, -2, name);
}


bool lua_isclass(lua_State* L, int idx)
{
	return lua_isuserdata(L, idx) != 0 && lua_upvalueindex(1) != 0;
}


gkLuaTypeDef* lua_getclasstype(lua_State* L)
{
	return static_cast<gkLuaTypeDef*>(lua_touserdata(L, lua_upvalueindex(GK_UPVALUETYPE)));
}



gkLuaMethodDef* lua_getmethodtype(lua_State* L)
{
	return static_cast<gkLuaMethodDef*>(lua_touserdata(L, lua_upvalueindex(GK_UPVALUEMETHOD)));
}



unsigned int lua_bindhash(const char* key)
{
	utCharHashKey chk(key);
	return chk.hash();
}

static unsigned int __getter_hash   = lua_bindhash("__getter");
static unsigned int __setter_hash   = lua_bindhash("__setter");
static unsigned int __constructor   = lua_bindhash("constructor");
static unsigned int __destructor    = lua_bindhash("destructor");
static unsigned int __tostring      = lua_bindhash("__tostring");
static unsigned int __lgc           = lua_bindhash("__gc");
static unsigned int __add           = lua_bindhash("__add");
static unsigned int __sub           = lua_bindhash("__sub");
static unsigned int __mul           = lua_bindhash("__mul");
static unsigned int __div           = lua_bindhash("__div");
static unsigned int __unm           = lua_bindhash("__unm");



void lua_bindfindmethod(const char* name, gkLuaMethodDef* def, int* out)
{
	int i;
	for (i = 0; def[i].m_name != 0; ++i)
	{
		if (utCharEq(def[i].m_name, name))
		{
			*out = i;
			break;
		}
	}
}


int lua_bindtostring(lua_State* L)
{
	gkLuaTypeDef* tstr = static_cast<gkLuaTypeDef*>(lua_touserdata(L, lua_upvalueindex(GK_UPVALUETYPE)));
	if (tstr)
	{
		if (lua_isuserdata(L, 1))
		{
			static char buffer[72];
			sprintf(buffer, "%s: 0x%p", tstr->m_name, lua_topointer(L, 1));
			lua_pushstring(L, buffer);
		}
		else
			lua_pushstring(L, tstr->m_name);
		return 1;
	}
	return 0;
}



bool lua_bindvalidateparams(const char* dt, lua_State* L)
{
	// validates function/method arguments
	for (int i = 0; dt[i] != LU_NIL; ++i)
	{
		if (dt[i] == LU_OPT)
			return true;

		int idx = i + 1;
		switch (dt[i])
		{
		case LU_BOOL:
			if (!lua_isboolean(L, idx))
				return false;
			break;
		case LU_ENUM:
		case LU_INT:
		case LU_FLOAT:
		case LU_DOUBLE:
		case LU_NUMBER:
			if (!lua_isnumber(L, idx))
				return false;
			break;
		case LU_STRING:
			if (!lua_isstring(L, idx))
				return false;
			break;
		case LU_INSTANCE:
			if (!lua_isclass(L, idx))
				return false;
			break;
		case LU_FUNCTION:
			if (!lua_isfunction(L, i + 1))
				return false;
			break;
		}
	}
	return true;
}



int lua_binderrorparams(const char* meth, const char* dt, char* dest)
{
	// generates an error for function/method arguments
	if (!dest)
		return 0;


	int len = sprintf(dest, "Expected: %s (", meth), tl = 0;
	for (int i = 0; dt[i] != LU_NIL; ++i)
	{
		if ( dt[i] == LU_OPT)
			tl = sprintf(dest, "optionally");

		switch (dt[i])
		{
		case LU_BOOL:       {tl = sprintf(dest, "bool");     break;}
		case LU_NUMBER:     {tl = sprintf(dest, "number");   break;}
		case LU_INT:        {tl = sprintf(dest, "int");      break;}
		case LU_ENUM:       {tl = sprintf(dest, "enum");     break;}
		case LU_FLOAT:      {tl = sprintf(dest, "float");    break;}
		case LU_DOUBLE:     {tl = sprintf(dest, "double");   break;}
		case LU_STRING:     {tl = sprintf(dest, "string");   break;}
		case LU_INSTANCE:   {tl = sprintf(dest, "class");    break;}
		case LU_FUNCTION:   {tl = sprintf(dest, "function"); break;}
		}


		if (dt[i+1] != LU_NIL)
			tl = sprintf(dest, ", ");

		len += tl;
		tl = 0;
	}

	len += sprintf(dest, ")");
	return len;
}



int lua_bindmethodwrapper(lua_State* L)
{
	gkLuaMethodDef* mdef = lua_getmethodtype(L);

	if (mdef != 0)
	{
		if (mdef->m_meth != 0)
		{
			if (mdef->m_flag != GK_NOPARAM && mdef->m_params != 0)
			{
				if (!lua_bindvalidateparams(mdef->m_params, L))
				{
					static char errBuf[1024];
					int len = lua_binderrorparams(mdef->m_name, mdef->m_params, errBuf);
					errBuf[len > 1024 ? 1024 : len] = 0;
					luaL_error(L, "%s", errBuf);
					return 1;
				}
			}
			return mdef->m_meth(L);
		}
	}

	luaL_error(L, "unknwon error");
	return 1;
}



int lua_bindpushclassmethodget(lua_State* L, unsigned int index, gkLuaTypeDef* type)
{
	// find class / base class methods
	if (type->m_methods)
	{
		int gidx = -1, i;
		for (i = 0; type->m_methods[i].m_name != 0; ++i)
		{
			if (type->m_methods[i].m_hash == __getter_hash)
				gidx = i;
			else if (index == type->m_methods[i].m_hash )
			{
				lua_pushlightuserdata(L, type);
				lua_pushlightuserdata(L, &type->m_methods[i]);
				lua_pushcclosure(L, lua_bindmethodwrapper, 2);
				return 1;
			}
		}
		if (gidx != -1)
		{
			// call get attr
			int ret = 0;
			if ((ret = (type->m_methods[gidx].m_meth)(L)) != 0)
				return ret;
		}
	}

	// get up index
	if (type->m_parent)
		return lua_bindpushclassmethodget(L, index, type->m_parent);
	return 0;
}



int lua_bindpushclassmethodset(lua_State* L, unsigned int index, gkLuaTypeDef* type)
{
	// find class / base class methods
	if (type->m_methods)
	{
		int sidx = -1, i;
		for (i = 0; type->m_methods[i].m_name != 0; ++i)
		{
			if (type->m_methods[i].m_hash == __setter_hash)
				sidx = i;
			else if (index == type->m_methods[i].m_hash )
			{
				lua_pushlightuserdata(L, type);
				lua_pushlightuserdata(L, &type->m_methods[i]);
				lua_pushcclosure(L, lua_bindmethodwrapper, 2);
				return 1;
			}
		}
		if (sidx != -1)
		{
			// call get attr
			int ret = 0;
			if ((ret = (type->m_methods[sidx].m_meth)(L)) != 0)
				return ret;
		}
	}

	// get up index
	if (type->m_parent)
		return lua_bindpushclassmethodset(L, index, type->m_parent);
	return 0;
}



int lua_bindclassindexerget(lua_State* L)
{
	// index method
	gkLuaTypeDef* type = lua_getclasstype(L);
	if (type)
	{
		// find by name
		if (lua_isstring(L, 2))
			return lua_bindpushclassmethodget(L, lua_bindhash(lua_tostring(L, 2)), type);
	}
	return 0;
}


int lua_bindclassindexerset(lua_State* L)
{
	// index method
	gkLuaTypeDef* type = lua_getclasstype(L);
	if (type)
	{
		// find by name
		if (lua_isstring(L, 2))
			return lua_bindpushclassmethodset(L, lua_bindhash(lua_tostring(L, 2)), type);
	}
	return 0;
}


int lua_findclassmethod(lua_State* L, const char* name, gkLuaTypeDef* cls)
{
	return lua_bindpushclassmethodget(L, lua_bindhash(name), cls);
}


bool lua_bindisbuiltinmethod(gkLuaMethodDef* meth)
{
	return (meth->m_hash == __constructor   ||
	        meth->m_hash == __destructor    ||
	        meth->m_hash == __getter_hash   ||
	        meth->m_hash == __setter_hash   ||
	        meth->m_hash == __tostring      ||
	        meth->m_hash == __lgc           ||
	        meth->m_hash == __add           ||
	        meth->m_hash == __sub           ||
	        meth->m_hash == __mul           ||
	        meth->m_hash == __div           ||
	        meth->m_hash == __unm
	       );
}


#define lua_binddefaultclassmethod(type, name, index, meth)         \
    if (index != -1) {                                              \
        lua_pushstring(L, name);                                    \
        lua_pushlightuserdata(L, type);                             \
        lua_pushlightuserdata(L,  &type->m_methods[index]);         \
        lua_pushcclosure(L, meth, 2);                               \
        lua_settable(L, -3);                                        \
    }



void lua_addclasstype(lua_State* L, gkLuaTypeDef* type)
{
	int ctor = -1, dtor = -1, tstr = -1, nidx = -1;
	int addi = -1, subi = -1, muli = -1, divi = -1;
	int unmi = -1;

	if (type->m_methods)
	{
		lua_bindfindmethod("constructor",  type->m_methods,   &ctor);
		lua_bindfindmethod("destructor",   type->m_methods,   &dtor);
		lua_bindfindmethod("__tostring",   type->m_methods,   &tstr);
		lua_bindfindmethod("__setter",     type->m_methods,   &nidx);
		lua_bindfindmethod("__add",        type->m_methods,   &addi);
		lua_bindfindmethod("__sub",        type->m_methods,   &subi);
		lua_bindfindmethod("__mul",        type->m_methods,   &muli);
		lua_bindfindmethod("__div",        type->m_methods,   &divi);
		lua_bindfindmethod("__unm",        type->m_methods,   &unmi);
	}

	if (ctor != -1)
	{
		lua_pushstring(L, type->m_name);
		lua_pushlightuserdata(L, type);
		lua_pushlightuserdata(L, &type->m_methods[ctor]);
		lua_pushcclosure(L, lua_bindmethodwrapper, 2);
		lua_settable(L, -3);
	}

	luaL_newmetatable(L, type->m_name);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");

	if (tstr != -1)
	{
		lua_pushstring(L, "__tostring");
		lua_pushlightuserdata(L, type);
		lua_pushlightuserdata(L,  &type->m_methods[tstr]);
		lua_pushcclosure(L, lua_bindmethodwrapper, 2);
		lua_settable(L, -3);
	}
	else
	{
		// default __tostring
		lua_pushstring(L, "__tostring");
		lua_pushlightuserdata(L, type);
		lua_pushcclosure(L, lua_bindtostring, 1);
		lua_settable(L, -3);
	}

	if (dtor != -1)
	{
		lua_pushstring(L, "__gc");
		lua_pushlightuserdata(L, type);
		lua_pushlightuserdata(L,  &type->m_methods[dtor]);
		lua_pushcclosure(L, lua_bindmethodwrapper, 2);
		lua_settable(L, -3);
	}

	lua_pushstring(L, "__index");
	lua_pushlightuserdata(L, type);
	lua_pushcclosure(L, lua_bindclassindexerget, 1);
	lua_settable(L, -3);

	lua_binddefaultclassmethod(type, "__newindex", nidx, lua_bindclassindexerset);
	lua_binddefaultclassmethod(type, "__add",      addi, lua_bindmethodwrapper);
	lua_binddefaultclassmethod(type, "__sub",      subi, lua_bindmethodwrapper);
	lua_binddefaultclassmethod(type, "__mul",      muli, lua_bindmethodwrapper);
	lua_binddefaultclassmethod(type, "__div",      divi, lua_bindmethodwrapper);
	lua_binddefaultclassmethod(type, "__unm",      unmi, lua_bindmethodwrapper);

	if (type->m_methods)
	{
		int i;
		for (i = 0; type->m_methods[i].m_name != 0; ++i)
		{
			type->m_methods[i].m_hash = lua_bindhash(type->m_methods[i].m_name);
			if (lua_bindisbuiltinmethod(&type->m_methods[i]))
				continue;

			lua_pushstring(L, type->m_methods[i].m_name);
			lua_pushlightuserdata(L, type);
			lua_pushlightuserdata(L, &type->m_methods[i]);
			lua_pushcclosure(L, lua_bindmethodwrapper, 2);
			lua_settable(L, -3);
		}
	}

	lua_pop(L, 1);
}


void lua_addmethods(lua_State* L, gkLuaMethodDef* methods)
{
	int i;
	for (i = 0; methods[i].m_name != 0; ++i)
	{
		methods[i].m_hash = lua_bindhash(methods[i].m_name);
		if (lua_bindisbuiltinmethod(&methods[i]))
			continue;
		lua_pushstring(L, methods[i].m_name);
		lua_pushnumber(L, -1);
		lua_pushlightuserdata(L, &methods[i]);
		lua_pushcclosure(L, lua_bindmethodwrapper, 2);
		lua_settable(L, -3);
	}
}
