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
#ifndef _gkLuaType_h_
#define _gkLuaType_h_

#include "gkCommon.h"



extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

typedef lua_State gkLuaState;
typedef int (*gkLuaMethod)(gkLuaState *L);



// Script method definition
struct gkLuaMethodDef
{
	const char	*name;
	gkLuaMethod	meth;
};

// Script type definition (eg; table, class)
struct gkLuaTypeDef
{
	const char*     name;
	gkLuaMethod		ctor;
	gkLuaMethod		dtor;
	gkLuaMethod		get;
	gkLuaMethod		set;
	gkLuaMethodDef* methods;
};




#define gkLuaNew(L, Ptr, TypeDef, Type, Ctor) {\
	Ptr = (Type*)gkLuaBind::instance(L, TypeDef, sizeof(Type));\
	new (Ptr)Ctor;\
}

#define gkLuaDelete(P, T) P->~T()

#ifdef _MSC_VER
# define lua_vsnprintf _vsnprintf
#else
# define lua_vsnprintf vsnprintf
#endif

// Static binding utilities
class gkLuaBind
{
public:

	static int beginBinding(gkLuaState *L);
	static void endBinding(gkLuaState *L, int idx);

	static void stacktrace(gkLuaState *L, const char *function);

    static bool load(gkLuaState *L, gkLuaMethod &dest);
	static bool load(gkLuaState *L, const char *name, const char *memBuf, int memLen);
	static bool load(gkLuaState *L, const char *file);
	static int seterror(gkLuaState *L, const char *fmt, ...);

    static void *instance(gkLuaState *L, const gkLuaTypeDef& def, size_t size);

    // argument parsing utils
	static bool parse(gkLuaState *L, const char *fmt, ...);

	// use self for userdata
	template <typename T> 
	static T* self(gkLuaState *L) { return reinterpret_cast<T*>(lua_touserdata(L, 1)); }

	static bool getArg(gkLuaState *L, int idx, unsigned int &val);

	static bool getArg(gkLuaState *L, int idx, char **dest);
	static bool getArg(gkLuaState *L, int idx, int *dest);
	static bool getArg(gkLuaState *L, int idx, float *dest);
	static bool getArg(gkLuaState *L, int idx, bool *dest);
    static bool getArg(gkLuaState *L, int idx, gkLuaMethod *dest);

	// return value push
	static int push(gkLuaState *L, const char *val);
	static int push(gkLuaState *L, const int val);
	static int push(gkLuaState *L, const float val);
	static int push(gkLuaState *L, const bool val);

	static void set(gkLuaState *L, const char *name, float val);
	static void set(gkLuaState *L, const char *name, int val);
	static void set(gkLuaState *L, const char *name, const char *val);
	static void set(gkLuaState *L, const char *name, int size, const char **val);
	static void set(gkLuaState *L, const char *name, gkLuaMethod meth);
	static void set(gkLuaState *L, gkLuaMethodDef& def);
	static void set(gkLuaState *L, gkLuaMethodDef *defarray);
	static void set(gkLuaState *L, gkLuaTypeDef *type);

	static void table(gkLuaState *L, const char *name);
	static void table(gkLuaState *L);


	static unsigned int hash(const char *s);

};



#endif//_gkLuaType_h_
