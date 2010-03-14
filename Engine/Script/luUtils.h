/*
-------------------------------------------------------------------------------
    Lua Utility Library

    Copyright (c) 2009-2010 Charlie C.
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
#ifndef _luUtils_h_
#define _luUtils_h_

#include <assert.h>
#include <sstream>
extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include "gkString.h"
#include "gkHashedString.h"



typedef utFixedString<32>   luIdentifier;
typedef utString            luString;
typedef gkHashedString      luHashedString;
typedef std::ostringstream  luConverterString;



typedef lua_State ltState;

class   luClass;
struct  luTypeDef;
struct  luMethodDef;
struct  luClassMethodDef;


// c++ usage wrapper
class luObject
{
protected:
    ltState     *L;

public:
    luObject(ltState *_L) : L(_L) {}
    ~luObject() {}

    UT_INLINE luObject &operator = (const luObject &rhs)
    {
        L = rhs.L;
        return *this;
    }

    bool        isNil(int idx) const;
    bool        isBoolean(int idx) const;
    bool        isNumber(int idx) const;
    bool        isString(int idx) const;
    bool        isClass(int idx) const;

    bool        getValueBool(int idx) const;
    double      getValueNumber(int idx) const;
    float       getValueFloat(int idx) const;
    int         getValueInt(int idx) const;
    luString    getValueString(int idx) const;
    luClass     *getValueClass(int idx) const;

    luTypeDef           *getType(void);
    luMethodDef         *getMethod(void);
    luClassMethodDef    *getClassMethod(void);


    bool typecheck(int idx, luTypeDef *tdef) const;

    // no type checks
    UT_INLINE bool      toboolean(int idx) const    { return lua_toboolean(L, idx) != 0;    }
    UT_INLINE double    tonumber(int idx) const     { return (double)lua_tonumber(L, idx);  }
    UT_INLINE float     tofloat(int idx) const      { return (float)lua_tonumber(L, idx);   }
    UT_INLINE int       toint(int idx) const        { return (int)lua_tonumber(L, idx);     }
    UT_INLINE luString  tostring(int idx) const     { return lua_tostring(L, idx);          }
    UT_INLINE luClass   *toclass(int idx) const     { return static_cast<luClass *>(lua_touserdata(L, idx)); }

    UT_INLINE luClass   &toclassRef(int idx) const  
    { 
        void *ptr = lua_touserdata(L, idx);
        UT_ASSERT(ptr);
        return *static_cast<luClass *>(ptr); 
    }

    template<typename T>
    UT_INLINE T   &toclassRefT(int idx) const  
    { 
        void *ptr = lua_touserdata(L, idx);
        UT_ASSERT(ptr);
        return *static_cast<T *>(ptr); 
    }


    template <typename T>
    UT_INLINE T  *toclassT(int idx) const
    { return static_cast<T *>(lua_touserdata(L, idx)); }

    int push(void);
    int push(const char *val);
    int push(const int val);
    int push(const float val);
    int push(const bool val);
    int push(luClass *val);
    int pushError(const char *fmt, ...);

    template<typename T> T *getValueClassT(int idx) const { return static_cast<T *>(getValueClass(idx));}

    operator ltState* (void) {return L;}
};


class luClass;

// Lua C-Function
typedef int (*luMethod)(luObject &L);
typedef int (luClass::*luClassMethod)(luClass *self, luObject &L);


// up-values stored with closures
#define LU_UPVALUETYPE      1
#define LU_UPVALUEMETHOD    2


// no parameter check flag
#define LU_NOPARAM          -1
// check parameters
#define LU_PARAM            1


// common implementation
#define luClassHeader                       \
public:                                     \
    static luMethodDef      Globals[];      \
    static luClassMethodDef Locals[];       \
    static luTypeDef Type;                  \
    virtual luTypeDef *getType(void)        \
    { return &Type; }                       \
    static bool isType(luObject &L, int v)  \
    { return L.typecheck(v, &Type); }          


// type definition implementation
#define luClassImpl(name, cls, parent) \
    luTypeDef cls::Type = {name, parent, Globals, Locals};

#define luGlobalTable(name, global, check, params)\
    {name, global, check, params},

#define luClassTable(name, cls, local, check, params)\
    {name, (luClassMethod)&cls::local, check, params},

// global functions
#define luGlobalTableBegin(cls)\
    luMethodDef cls::Globals[] = {
#define luGlobalTableEnd() {0,0,0,0}};


// class functions
#define luClassTableBegin(cls)\
    luClassMethodDef cls::Locals[] = {
#define luClassTableEnd() {0,0,0,0}};


// Lua function definition
struct luMethodDef
{
    const char      *m_name;
    luMethod        m_meth;
    int             m_flag;
    const char      *m_params;

    // internal
    unsigned int    m_hash;
};

struct luClassMethodDef
{
    const char      *m_name;
    luClassMethod   m_meth;
    int             m_flag;
    const char      *m_params;

    // internal
    unsigned int    m_hash;
};



struct luTypeDef
{
    const char          *m_name;
    luTypeDef           *m_parent;
    luMethodDef         *m_methods;
    luClassMethodDef    *m_classMethods;
};


class luScope
{
private:
    ltState *L;
    int m_top;
public:
    luScope(ltState *_L) : L(_L) { m_top = lua_gettop(L); }
    ~luScope() {lua_settop(L, m_top);}
};
#define LUA_SCOPE_LOCK luScope ___lock(L);


// debugging utility
class luStackDump
{
protected:
    ltState *L;
public:
    luStackDump(ltState *_L) : L(_L) {}
    void dump(void);
};

#define LUA_DUMP_STACK(L) {luStackDump __sd__(L); __sd__.dump();}


typedef enum luDataTypes
{
    LU_NIL      = '\0',
    LU_OPT      = '|',
    LU_BOOL     = 'b',
    LU_NUMBER   = 'n',
    LU_INT      = 'i',
    LU_ENUM     = 'e',
    LU_FLOAT    = 'f',
    LU_DOUBLE   = 'd',
    LU_STRING   = 's',
    LU_FUNCTION = 'F',
    LU_INSTANCE = '.',
} luDataTypes;


class luClass
{
public:

    inline void *operator new(size_t size, ltState *L)
    {
        luTypeDef *tdef = static_cast<luTypeDef *>(lua_touserdata(L, lua_upvalueindex(LU_UPVALUETYPE)));
        void *ptr = lua_newuserdata(L, size);
        if (tdef)
        {
            // bind to table
            luaL_getmetatable(L, tdef->m_name);
            lua_setmetatable(L, -2);
        }
        return ptr;
    }

    inline void *operator new(size_t size, luTypeDef *tdef, ltState *L)
    {
        void *ptr = lua_newuserdata(L, size);
        if (tdef)
        {
            // bind to table
            luaL_getmetatable(L, tdef->m_name);
            lua_setmetatable(L, -2);
        }
        return ptr;
    }

    inline void operator delete(void *ptr)                {}
    inline void operator delete(void *ptr, ltState *L)    {}
    inline void operator delete(void *ptr, luTypeDef *tdef, ltState *L)    {}

    luClass() {}
    virtual ~luClass() {}

    virtual luTypeDef *getType(void) = 0;
    bool isTypeOf(luTypeDef *type, luTypeDef *def);
    bool isTypeOf(luTypeDef *type, const char *name);
};


UT_INLINE bool luObject::typecheck(int idx, luTypeDef *tdef) const
{
    luClass *cls = toclass(idx);
    if (cls) return cls->isTypeOf(cls->getType(), tdef);
    return false;
}



// type/function binder
class luBinder
{
private:
    ltState *L;
    int m_val;
public:
    luBinder(ltState *_L);
    ~luBinder();

    void addClassType(luTypeDef *type);
    void addMethods(luMethodDef *type);

    void beginNamespace(const char *title);
    void endNamespace();

    void addConstant(const char *name, float v);
    void addConstant(const char *name, int v);
    void addConstant(const char *name, const char *v);

};


// For storing references to script/function objects
class luRefObject
{
private:
    int m_ref;

public:

    luRefObject() : m_ref(-1) {}
    ~luRefObject() {}

    void addRef(ltState *L, int ob)
    {
        if (m_ref != -1)
            unref(L);

        {
            LUA_SCOPE_LOCK
            lua_pushvalue(L, ob);
            m_ref = luaL_ref(L, LUA_REGISTRYINDEX);
        }
    }

    void unref(ltState *L)
    {
        if (m_ref != -1)
            luaL_unref(L, LUA_REGISTRYINDEX, m_ref);
        m_ref = -1;
    }

    void push(ltState *L)
    {
        if (m_ref != -1)
            lua_rawgeti(L, LUA_REGISTRYINDEX, m_ref);
    }
};

#endif//_luUtils_h_
