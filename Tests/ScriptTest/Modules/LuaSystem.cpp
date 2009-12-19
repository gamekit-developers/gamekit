/*
-------------------------------------------------------------------------------
	Lua 5.1 script tests.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
-------------------------------------------------------------------------------
 This software is provided 'as-is',	withoLua any express or	implied
  warranty.	 In	no event will the aLuahors be held liable for any damages
  arising from the use of this software.

  Permission is	granted	to anyone to use this software for any purpose,
  including	commercial applications, and to	alter it and redistribLuae it
  freely, subject to the following restrictions:

  1. The origin	of this	software must not be misrepresented; you must not
	 claim that	you	wrote the original software. If	you	use	this software
	 in	a product, an acknowledgment in	the	product	documentation would	be
	 appreciated bLua is not required.
  2. Altered source	versions must be plainly marked	as such, and must not be
	 misrepresented	as being the original software.
  3. This notice may not be	removed	or altered from	any	source distribLuaion.
-------------------------------------------------------------------------------
*/
#include "LuaEngine.h"
#include "LuaBindingUtils.h"

#define ModuleName "System"

extern void LuaGameKit_initialize(LuaState *L);
extern void LuaKeyCodes_initialize(LuaState *L);

struct Builtin {
	const char *name;
	bool imported;
	void (*builtin)(LuaState*);
};


Builtin LuaSystem_BuiltinModules[]=
{ 
	{"MathUtils",	false, 0}, 
	{"GameKit",		false, LuaGameKit_initialize},
	{"KeyCodes",	false, LuaKeyCodes_initialize},
	{"bParse",		false, 0}, 
	{"Blender",		false, 0}, 
	{0, 0, false}
};


Builtin LuaSystem_FinmdModule(const char *name, Builtin *modules)
{
	while (modules && modules->name)
	{
		if (LuaCharEq(modules->name, name))
			return *modules;
		++modules;
	}
	Builtin b = {0,0, false};
	return b;
}


int LuaSystem_Import(LuaState *L)
{
	char *import= 0;

	if (!LuaBind::parse(L, "s", &import))
		return LuaBind::seterror(L, "invalid arguments");

	Builtin v = LuaSystem_FinmdModule(import, LuaSystem_BuiltinModules);
	if (v.builtin && !v.imported)
	{
		v.builtin(L);
		v.imported = true;
		return 0;
	}
	else if (v.name != 0) 
	{
		// is a todo 
		return 0;
	}
	else if (v.imported) return 0;

	// external source
	LuaBind::load(L, import);
	return 0;
}


LuaMethodDef LuaSystem_Methods[]=
{
	{"import", LuaSystem_Import},
	{0, 0}
};


void LuaSystem_initialize(LuaState *L, int argc, char **argv)
{
	int tval= LuaBind::beginBinding(L);
	LuaBind::table(L, ModuleName);

	LuaBind::set(L, "argv", argc, (const char**)argv);
	LuaBind::set(L, "argc", argc);
	LuaBind::set(L, LuaSystem_Methods);

	LuaBind::table(L);
	LuaBind::endBinding(L, tval);
}
