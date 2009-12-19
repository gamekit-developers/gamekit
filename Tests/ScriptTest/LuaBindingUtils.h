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

#ifndef _LuaBindingUtils_h_
#define _LuaBindingUtils_h_


#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "LuaCommon.h"
#include "LuaType.h"


#define LuaNew(L, Ptr, TypeDef, Type, Ctor) {\
	Ptr = (Type*)LuaBind::instance(L, TypeDef, sizeof(Type));\
	new (Ptr)Ctor;\
}

#define LuaDelete(P, T) P->~T()


// utility to dump stack contents for examination
#define LuaStackTrace(L) LuaBind::stacktrace(L, __FUNCTION__);

#ifdef _MSC_VER
# define lua_vsnprintf _vsnprintf
#else
# define lua_vsnprintf vsnprintf
#endif
#define	LuaCharNEq(a, b, n)	 ((a &&	b) && !strncmp(a, b, n))
#define	LuaCharEq(a, b)		 ((a &&	b) && (*a == *b) &&	!strcmp(a, b))
#define	LuaCharEqL(a, b, l)	 ((a &&	b) && (*a == *b) &&	!memcmp(a, b, l))

class LuaBind
{
public:

	static int beginBinding(LuaState *L);
	static void endBinding(LuaState *L, int idx);

	static void stacktrace(LuaState *L, const char *function);

	static bool load(LuaState *L, const char *file);
	static int seterror(LuaState *L, const char *fmt, ...);


	static void *instance(LuaState *L, const LuaTypeDef& def, size_t size);



	/// argument parsing utils
	static bool parse(LuaState *L, const char *fmt, ...);

	// use self for userdata
	template <typename T> 
	static T* self(LuaState *L) { return reinterpret_cast<T*>(lua_touserdata(L, 1)); }

	static bool getArg(LuaState *L, int idx, unsigned int &val);

	static bool getArg(LuaState *L, int idx, char **dest);
	static bool getArg(LuaState *L, int idx, int *dest);
	static bool getArg(LuaState *L, int idx, float *dest);
	static bool getArg(LuaState *L, int idx, bool *dest);

	// return value push
	static int push(LuaState *L, const char *val);
	static int push(LuaState *L, const int val);
	static int push(LuaState *L, const float val);
	static int push(LuaState *L, const bool val);

	static void set(LuaState *L, const char *name, float val);
	static void set(LuaState *L, const char *name, int val);
	static void set(LuaState *L, const char *name, const char *val);
	static void set(LuaState *L, const char *name, int size, const char **val);
	static void set(LuaState *L, const char *name, LuaMethod meth);
	static void set(LuaState *L, LuaMethodDef& def);
	static void set(LuaState *L, LuaMethodDef *defarray);
	static void set(LuaState *L, LuaTypeDef *type);

	static void table(LuaState *L, const char *name);
	static void table(LuaState *L);


	static unsigned int hash(const char *s);

};


#endif//_LuaBindingUtils_h_
