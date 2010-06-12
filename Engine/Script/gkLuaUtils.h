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

#if 0

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

#endif
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

#if 0
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

#endif
#endif//_gkLuaUtils_h_
