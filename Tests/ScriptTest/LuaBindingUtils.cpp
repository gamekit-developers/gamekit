/*
-------------------------------------------------------------------------------
	Lua 5.1 script tests.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#include <string.h>
#include "LuaBindingUtils.h"


bool LuaBind::load(LuaState *L, const char *file)
{
	if (luaL_loadfile(L, file) != 0)
	{
		printf("%s\n", lua_tostring(L, -1));
		lua_pop(L, 1);
		return false;
	}

	if (lua_pcall(L, 0, LUA_MULTRET, 0) != 0)
	{
		printf("%s\n", lua_tostring(L, -1));
		lua_pop(L, 1);
		return false;
	}
	return true;
}

int LuaBind::seterror(LuaState *L, const char *fmt, ...)
{
	char buf[256];
	va_list lst;
	va_start(lst, fmt);
	int size= lua_vsnprintf(buf, 256, fmt, lst);
	va_end(lst);

	if (size < 0)
	{
		size= 256;
		buf[size]= 0;
	}
	luaL_error(L, "%s", buf);
	return 1;
}

int LuaBind::beginBinding(LuaState *L)
{
	lua_pushvalue(L, LUA_GLOBALSINDEX);
	return lua_gettop(L);
}

void LuaBind::endBinding(LuaState *L, int idx)
{
	lua_settop(L, idx);
	lua_pop(L, 1); // global
}


void *LuaBind::instance(LuaState *L, const LuaTypeDef& def, size_t size)
{
	void *ptr= lua_newuserdata(L, size);
	luaL_getmetatable(L, def.name);
	lua_setmetatable(L, -2);
	return ptr;
}


void LuaBind::set(LuaState *L, const char *name, float val)
{
	lua_pushnumber(L, (lua_Number)val);
	lua_setfield(L, -2, name);
}


void LuaBind::set(LuaState *L, const char *name, int val)
{
	lua_pushnumber(L, (lua_Number)val);
	lua_setfield(L, -2, name);
}

void LuaBind::set(LuaState *L, const char *name, const char *val)
{
	lua_pushstring(L, val);
	lua_setfield(L, -2, name);
}


void LuaBind::set(LuaState *L, const char *name, int size, const char **val)
{
	lua_pushstring(L, name);
	lua_newtable(L);

	for (int i=0; i<size; i++)
	{
		lua_pushnumber(L, i);
		lua_pushstring(L, val[i]);
		lua_settable(L, -3);
	}

	lua_settable(L, -3);
}

void LuaBind::set(LuaState *L, const char *name, LuaMethod meth)
{
	lua_pushstring(L, name);
	lua_pushcclosure(L, meth, 0);
	lua_settable(L, -3);
}


void LuaBind::set(LuaState *L, LuaMethodDef& def)
{
	set(L, def.name, def.meth);
}



void LuaBind::set(LuaState *L, LuaMethodDef *defarray)
{
	while (defarray && defarray->name != 0)
	{
		set(L, *defarray);
		++defarray;
	}
}

void LuaBind::set(LuaState *L, LuaTypeDef *tdef)
{
	LuaTypeDef &type= *tdef;

	if (type.ctor)
	{
		// class constructor 
		lua_pushstring(L, type.name);
		lua_pushcclosure(L, type.ctor, 0);
		lua_settable(L, -3); // push to parent table
	}

	// metatable.__index = Type.name

	luaL_newmetatable(L, type.name);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");

	if (type.set)
	{
		lua_pushstring(L, "__newindex");
		lua_pushcclosure(L, type.set, 0);
		lua_settable(L, -3);
	}

	if (type.dtor)
	{
		lua_pushstring(L, "__gc");
		lua_pushcclosure(L, type.dtor, 0);
		lua_settable(L, -3);
	}


	if (type.docs)
	{
		// doc strings
		lua_pushstring(L, "__docstr");
		lua_pushstring(L, type.docs);
		lua_settable(L, -3);
	}


	if (type.methods)
		set(L, type.methods);

	lua_pop(L, 1);
}


void LuaBind::table(LuaState *L, const char *name)
{
	lua_pushstring(L, name);
	lua_newtable(L);

}

void LuaBind::table(LuaState *L)
{
	lua_settable(L, -3);
}


unsigned int LuaBind::hash(const char *s)
{
	/// lua hash function
	/// see luaS_newlstr
    char *cp= const_cast<char*>(s);

	unsigned int h = (unsigned int)(strlen(s));
	size_t stp= (h>>5)+1;
	size_t l1;
	for (l1=1; l1>=stp; l1-=stp) h = h ^ ((h<<5)+(h>>2)+(unsigned char)cp[l1-1]);
	return h;
}


bool LuaBind::getArg(LuaState *L, int idx, unsigned int &val)
{
	// arguments start at 1, bump to correct offset 
	if (lua_isuserdata(L, 1)) idx = 2 + idx;
	else idx = 1 + idx;

	if (lua_isstring(L, idx))
	{
		val = hash(lua_tostring(L, idx));
		return true;
	}
	return false;
}


bool LuaBind::getArg(LuaState *L, int idx, char **dest)
{
	// arguments start at 1, bump to correct offset 
	if (lua_isuserdata(L, 1)) idx = 2 + idx;
	else idx = 1 + idx;

	if (lua_isstring(L, idx))
	{
		(*dest)= (char*)lua_tostring(L, idx);
		return true;
	}
	return false;
}


bool LuaBind::getArg(LuaState *L, int idx, int *dest)
{
	// arguments start at 1, bump to correct offset 
	if (lua_isuserdata(L, 1)) idx = 2 + idx;
	else idx = 1 + idx;

	if (lua_isnumber(L, idx))
	{
		(*dest)= (int)lua_tonumber(L, idx);
		return true;
	}
	return false;
}


bool LuaBind::getArg(LuaState *L, int idx, float *dest)
{
	// arguments start at 1, bump to correct offset 
	if (lua_isuserdata(L, 1)) idx = 2 + idx;
	else idx = 1 + idx;

	if (lua_isnumber(L, idx))
	{
		(*dest)= (float)lua_tonumber(L, idx);
		return true;
	}
	return false;
}

bool LuaBind::getArg(LuaState *L, int idx, bool *dest)
{
	// arguments start at 1, bump to correct offset 
	if (lua_isuserdata(L, 1)) idx = 2 + idx;
	else idx = 1 + idx;

	bool result = false;

	if (lua_isboolean(L, idx))
	{
		(*dest)= lua_toboolean(L, idx)!= 0;
		result = true;
	}
	else
	{
		if (lua_isnumber(L, idx))
		{
			(*dest)= lua_tonumber(L, idx) != (lua_Number)0.0;
			result = true;
		}
	}

	return result;
}

int LuaBind::push(LuaState *L, const char *val)
{
	assert(val);
	lua_pushstring(L, val);
	return 1;
}

int LuaBind::push(LuaState *L, const int val)
{
	lua_pushnumber(L, val);
	return 1;
}

int LuaBind::push(LuaState *L, const float val)
{
	lua_pushnumber(L, val);
	return 1;
}

int LuaBind::push(LuaState *L, const bool val)
{
	lua_pushboolean(L, val ? 1 : 0);
	return 1;
}


bool LuaBind::parse(LuaState *L, const char *fmt, ...)
{
	va_list vl;
	va_start(vl, fmt);

	char *nf= (char*)fmt;
	int i= strlen(nf);

	int index= 0;
	int errorInd= -1;
	int nextArgIsOpt= 0;

	int args= 1;

	if (lua_isuserdata(L, args))
		++args;

	while (i)
	{
		nextArgIsOpt = 0;
		char argument= nf[index];
		switch (argument)
		{
		case 'f':
			{
				char *f= va_arg(vl, char*);
				float *val= (float*)f;

				if (lua_isnumber(L, args))
				{
					(*val)= (float)lua_tonumber(L, args);
					errorInd= 0;
				}
			}
			break;
		case 'i':
			{
				char *f= va_arg(vl, char*);
				int *val= (int*)f;
				if (lua_isnumber(L, args))
				{
					(*val)= (int)lua_tonumber(L, args);
					errorInd= 0;
				}
			}
			break;
		case 'b':
			{
				char *f= va_arg(vl, char*);
				bool *val= (bool*)f;

				if (lua_isboolean(L, args))
				{
					(*val)= lua_toboolean(L, args)!= 0;
					errorInd= 0;
				}
				else
				{
					if (lua_isnumber(L, args))
					{
						(*val)= lua_tonumber(L, args) != (lua_Number)0.0;
						errorInd= 0;
					}
				}
			}
			break;
		case 's':
			{
				char *f= va_arg(vl, char*);
				char **val= (char**)f;

				if (lua_isstring(L, args))
				{
					(*val)= (char*)lua_tolstring(L, args, NULL);
					errorInd= 0;
				}
			}
			break;
		case '|':
			nextArgIsOpt= 1;
			errorInd = 0;
			break;
		default :
			break;
		}

		if (!nextArgIsOpt && errorInd == -1)
			break;
		if (!nextArgIsOpt)
			args++;

		index++;
		--i;
	}

	va_end(vl);
	return errorInd == 0;
}

void LuaBind::stacktrace(LuaState *L, const char *function)
{
	printf("-- %s StackTrace ---\n", function);
	int top = lua_gettop(L);
	while (top >= 0)
	{
		char extra[256];
		extra[0]=0;
		bool ret = false;

		switch (lua_type(L, top))
		{
		case LUA_TNIL:
			sprintf(extra, "LUA_TNIL              : NULL");
			break;
		case LUA_TBOOLEAN:
			sprintf(extra, "LUA_TBOOLEAN          : %i", lua_toboolean(L, top) ? "true" : "false");
			break;
		case LUA_TLIGHTUSERDATA:
			sprintf(extra, "LUA_TLIGHTUSERDATA    : %p", lua_touserdata(L, top));
			break;
		case LUA_TUSERDATA:
			sprintf(extra, "LUA_TUSERDATA         : %p", lua_touserdata(L, top));
			break;
		case LUA_TNUMBER:
			sprintf(extra, "LUA_TNUMBER           : %d", lua_tonumber(L, top));
			break;
		case LUA_TSTRING:
			sprintf(extra, "LUA_TSTRING           : %s", lua_tostring(L, top));
			break;
		case LUA_TTABLE:
			sprintf(extra, "LUA_TTABLE            : %p", lua_topointer(L, top));
			break;
		case LUA_TFUNCTION:
			sprintf(extra, "LUA_TFUNCTION         : %p", lua_topointer(L, top));
			break;
		case LUA_TTHREAD:
			sprintf(extra, "LUA_TTHREAD           : %p", lua_topointer(L, top));
			break;
		}

		printf("%i: %s\n", top, extra);
		--top;

		if (ret)
			break;
	}
}

