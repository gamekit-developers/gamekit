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
#ifndef _gkLuaUtils_h_
#define _gkLuaUtils_h_

extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}
#include "Utils/utTypes.h"

#define GK_UPVALUETYPE      1
#define GK_UPVALUEMETHOD    2
#define GK_NOPARAM         -1
#define GK_PARAM            1


// -----------------------------------------------------------------------------
typedef int (*gkLuaMethod)(lua_State *L);

// -----------------------------------------------------------------------------
struct gkLuaMethodDef
{
    const char      *m_name;
    gkLuaMethod      m_meth;
    int              m_flag;
    const char      *m_params;
    // internal
    unsigned int     m_hash;
};

// -----------------------------------------------------------------------------
struct gkLuaTypeDef
{
    const char              *m_name;
    gkLuaTypeDef            *m_parent;
    gkLuaMethodDef          *m_methods;
};


enum gkLuaDataTypes
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
};

typedef struct gkLuaCurState
{
    lua_State *L;
    int m_id;
}gkLuaCurState;


class gkLuaObject
{
private:
    lua_State *L;
    int m_ref;
public:

    gkLuaObject() : L(0), m_ref(-1) {}
    gkLuaObject(lua_State *_L, int input) : L(_L), m_ref(-1) {ref(input);}
    ~gkLuaObject() { unref(); }

    void ref(int input)
    {
        if (m_ref != -1)
            unref();

        if (L)
        {
            lua_pushvalue(L, input);
            m_ref = luaL_ref(L, LUA_REGISTRYINDEX);
        }
    }

    void unref(void)
    {
        if (m_ref != -1 && L)
        {
            luaL_unref(L, LUA_REGISTRYINDEX, m_ref);
            m_ref = -1;
        }
    }

    int         get(void)   {return m_ref;}
    lua_State*  getL(void)  {return L;}
};

class gkLuaEvent
{
protected:

    lua_State   *L;
    gkLuaObject *m_self;
    gkLuaObject *m_callback;
    int          m_callArgs;

public:

    // global function
    gkLuaEvent(gkLuaCurState fnc);

    // member function
    gkLuaEvent(gkLuaCurState self, gkLuaCurState fnc);
    virtual ~gkLuaEvent();




    // push callback and self if present,
    void beginCall(void);
    // add args,
    void addArgument(bool val);
    void addArgument(int val);
    void addArgument(float val);
    // push extra args, 
    // do the call 
    bool call();


    // return bool 
    bool call(bool &result);

};


// -----------------------------------------------------------------------------
class gkLuaState
{
private:
    lua_State *L;
    int m_top;
public:
    gkLuaState(lua_State *_L) : L(_L) { m_top = lua_gettop(L); }
    ~gkLuaState() {lua_settop(L, m_top);}
};


#define lua_savestate(L) gkLuaState ___lock___(L);

// -----------------------------------------------------------------------------
extern void lua_popall(lua_State *L);
extern void lua_dumpstack(lua_State *L);

extern bool lua_isclass(lua_State *L, int idx);

extern gkLuaTypeDef *lua_getclasstype(lua_State *L);
extern gkLuaMethodDef *lua_getmethodtype(lua_State *L);
extern int lua_findclassmethod(lua_State *L, const char *name, gkLuaTypeDef *cls);


// binding 
extern void lua_beginnamespace(lua_State *L, const char *nsp);
extern void lua_endnamespace(lua_State *L);
extern void lua_addclasstype(lua_State *L, gkLuaTypeDef *type);
extern void lua_addmethods(lua_State *L, gkLuaMethodDef *type);
extern void lua_addconstant(lua_State *L, const char *name, double v);
extern void lua_addconstant(lua_State *L, const char *name, float v);
extern void lua_addconstant(lua_State *L, const char *name, int v);
extern void lua_addconstant(lua_State *L, const char *name, const char *v);

#endif//_gkLuaUtils_h_
