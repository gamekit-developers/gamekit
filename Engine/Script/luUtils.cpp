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
#include "luUtils.h"


unsigned int luBinder_hash(const char *key)
{
    utCharHashKey chk(key);
    return chk.hash();
}


// index finder
void luBinder_findMethod(const char *name, luMethodDef *def, int *out)
{
    int i;
    for (i=0; def[i].m_name != 0; ++i)
    {
        if (utCharEq(def[i].m_name, name))
        {
            *out = i;
            break;
        }
    }
}

// default __tostring method
static int luDef_ToString(ltState *L)
{
    luTypeDef *tstr = static_cast<luTypeDef *>(lua_touserdata(L, lua_upvalueindex(LU_UPVALUETYPE)));
    if (tstr)
    {
        if (lua_isuserdata(L, 1))
        {
            char buffer[64];
            sprintf(buffer, "%s: 0x%p", tstr->m_name, lua_topointer(L, 1));
            lua_pushstring(L, buffer);
        }
        else
            lua_pushstring(L, tstr->m_name);
        return 1;
    }
    return 0;
}


// validates function/method arguments
static bool luDef_ValidateParams(const char *dt, luObject &ob)
{
    bool isOpt = false;
    for (int i=0; dt[i] != LU_NIL; ++i)
    {
        if (!isOpt && dt[i] == LU_OPT)
        {
            isOpt = true;
            continue;
        }
        switch (dt[i])
        {
        case LU_BOOL:
            if (!isOpt && !ob.isBoolean(i+1))
                return false;
            break;
        case LU_ENUM:
        case LU_INT:
        case LU_FLOAT:
        case LU_DOUBLE:
        case LU_NUMBER:
            if (!isOpt && !ob.isNumber(i+1))
                return false;
            break;
        case LU_STRING:
            if (!isOpt && !ob.isString(i+1))
                return false;
            break;
        case LU_INSTANCE:
            if (!isOpt && !ob.isClass(i+1))
                return false;
            break;
        case LU_FUNCTION:
            if (!isOpt && !lua_isfunction(ob, i+1))
                return false;
        }
    }
    return true;
}

// generates an error for function/method arguments
static luString luDef_ErrorParams(const char *meth, const char *dt)
{
    luConverterString stream;

    stream << "Expected " << meth << "(";
    bool isOpt = false;
    for (int i=0; dt[i] != LU_NIL; ++i)
    {
        if (!isOpt && dt[i] == LU_OPT)
        {
            isOpt = true;
            continue;
        }
        switch (dt[i])
        {
        case LU_BOOL:
            if (!isOpt)
                stream << "bool";
            break;
        case LU_NUMBER:
            if (!isOpt)
                stream << "number";
            break;
        case LU_INT:
            if (!isOpt)
                stream << "int";
            break;
        case LU_ENUM:
            if (!isOpt)
                stream << "enum";
            break;
        case LU_FLOAT:
            if (!isOpt)
                stream << "float";
            break;
        case LU_DOUBLE:
            if (!isOpt)
                stream << "double";
            break;
        case LU_STRING:
            if (!isOpt)
                stream << "string";
            break;
        case LU_INSTANCE:
            if (!isOpt)
                stream << "class";
            break;
        case LU_FUNCTION:
            if (!isOpt)
                stream << "function";
            break;
        }

        if (dt[i+1] != LU_NIL)
        {
            if (!isOpt || dt[i+1] != LU_OPT)
                stream << ", ";
        }
    }

    stream << ")";
    return stream.str();
}


static unsigned int __getter_hash   = luBinder_hash("__getter");
static unsigned int __setter_hash   = luBinder_hash("__setter");
static unsigned int __constructor   = luBinder_hash("constructor");
static unsigned int __destructor    = luBinder_hash("destructor");
static unsigned int __tostring      = luBinder_hash("__tostring");
static unsigned int __lgc           = luBinder_hash("__gc");
static unsigned int __add           = luBinder_hash("__add");
static unsigned int __sub           = luBinder_hash("__sub");
static unsigned int __mul           = luBinder_hash("__mul");
static unsigned int __div           = luBinder_hash("__div");
static unsigned int __unm           = luBinder_hash("__unm");


// global method wrapper
static int luDef_MethodWrapper(ltState *L)
{
    luObject ob(L);
    luMethodDef *mdef = ob.getMethod();

    if (mdef != 0)
    {
        if (mdef->m_meth != 0)
        {
            if (mdef->m_flag != LU_NOPARAM && mdef->m_params != 0)
            {
                bool isCtor = mdef->m_hash == __constructor;
                if (!luDef_ValidateParams(mdef->m_params, ob))
                {
                    luaL_error(L, "%s", luDef_ErrorParams(isCtor ? ob.getType()->m_name : mdef->m_name, mdef->m_params).c_str());
                    return 1;
                }
            }
            return mdef->m_meth(ob);
        }
    }

    luaL_error(L, "unknwon error");
    return 0;
}

// find class / base class methods
static int luDef_PushBaseMethod(ltState *L, unsigned int index, luTypeDef *type)
{

    if (type->m_methods)
    {
        int gidx = -1, i;
        for (i=0; type->m_methods[i].m_name != 0; ++i)
        {
            if (type->m_methods[i].m_hash == __getter_hash)
                gidx = i;
            else if (index == type->m_methods[i].m_hash )
            {
                lua_pushlightuserdata(L, type);
                lua_pushlightuserdata(L, &type->m_methods[i]);
                lua_pushcclosure(L, luDef_MethodWrapper, 2);
                return 1;
            }
        }

        if (gidx != -1)
        {
            luObject ob(L);
            int ret = 0;
            if ((ret = type->m_methods[gidx].m_meth(ob)) != 0)
                return ret;
        }
    }

    if (type->m_parent)
        return luDef_PushBaseMethod(L, index, type->m_parent);
    return 0;
}

/// index method
static int luDef_Indexer(ltState *L)
{
    luObject ob(L);
    luTypeDef *tstr = ob.getType();
    if (tstr)
    {
        // find by name
        if (ob.isString(2))
            return luDef_PushBaseMethod(L, luBinder_hash(ob.getValueString(2).c_str()), tstr);
    }
    return 0;
}


static int luDef_NewIndexer(ltState *L)
{
    luObject ob(L);
    luMethodDef *mdef = ob.getMethod();

    if (mdef != 0)
    {
        if (mdef->m_meth != 0)
        {
            if (mdef->m_flag != LU_NOPARAM && mdef->m_params != 0)
            {
                if (!luDef_ValidateParams(mdef->m_params, ob))
                {
                    luaL_error(L, "%s", luDef_ErrorParams(mdef->m_name, mdef->m_params).c_str());
                    return 1;
                }
            }


            int ret = 0;
            if ((ret = mdef->m_meth(ob)) != 0)
                return ret;
        }
    }
    return 0;
}

static int luDef_Gc(ltState *L)
{
    luObject ob(L);
    luTypeDef *tstr = ob.getType();
    if (tstr)
    {
        delete ob.getValueClass(1);
        return 0;
    }
    return 0;
}

static bool luDef_isBuiltinMethod(luMethodDef *meth)
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

#define luDef_DefaultMethod(type, name, index, meth)        \
    if (index != -1) {                                      \
        lua_pushstring(L, name);                            \
        lua_pushlightuserdata(L, type);                     \
        lua_pushlightuserdata(L,  &type->m_methods[index]); \
        lua_pushcclosure(L, meth, 2);                       \
        lua_settable(L, -3);                                \
    }


void luBinder::addType(luTypeDef *type)
{
    int ctor = -1, dtor = -1, tstr = -1, nidx = -1;
    int addi = -1, subi = -1, muli = -1, divi = -1;
    int unmi = -1;

    if (type->m_methods)
    {
        luBinder_findMethod("constructor",  type->m_methods,   &ctor);
        luBinder_findMethod("__tostring",   type->m_methods,   &tstr);
        luBinder_findMethod("destructor",   type->m_methods,   &dtor);
        luBinder_findMethod("__setter",     type->m_methods,   &nidx);

        // math operators
        luBinder_findMethod("__add",     type->m_methods,   &addi);
        luBinder_findMethod("__sub",     type->m_methods,   &subi);
        luBinder_findMethod("__mul",     type->m_methods,   &muli);
        luBinder_findMethod("__div",     type->m_methods,   &divi);
        luBinder_findMethod("__unm",     type->m_methods,   &unmi);
    }


    if (ctor != -1)
    {
        lua_pushstring(L, type->m_name);
        lua_pushlightuserdata(L, type);
        lua_pushlightuserdata(L, &type->m_methods[ctor]);
        lua_pushcclosure(L, luDef_MethodWrapper, 2);
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
        lua_pushcclosure(L, luDef_MethodWrapper, 2);
        lua_settable(L, -3);
    }
    else
    {
        // default __tostring
        lua_pushstring(L, "__tostring");
        lua_pushlightuserdata(L, type);
        lua_pushcclosure(L, luDef_ToString, 1);
        lua_settable(L, -3);
    }

    if (dtor != -1)
    {
        lua_pushstring(L, "__gc");
        lua_pushlightuserdata(L, type);
        lua_pushlightuserdata(L,  &type->m_methods[dtor]);
        lua_pushcclosure(L, luDef_MethodWrapper, 2);
        lua_settable(L, -3);
    }
    else
    {
        lua_pushstring(L, "__gc");
        lua_pushlightuserdata(L, type);
        lua_pushcclosure(L, luDef_Gc, 1);
        lua_settable(L, -3);
    }

    lua_pushstring(L, "__index");
    lua_pushlightuserdata(L, type);
    lua_pushcclosure(L, luDef_Indexer, 1);
    lua_settable(L, -3);


    luDef_DefaultMethod(type, "__newindex", nidx, luDef_NewIndexer);
    luDef_DefaultMethod(type, "__add",      addi, luDef_MethodWrapper);
    luDef_DefaultMethod(type, "__sub",      subi, luDef_MethodWrapper);
    luDef_DefaultMethod(type, "__mul",      muli, luDef_MethodWrapper);
    luDef_DefaultMethod(type, "__div",      divi, luDef_MethodWrapper);
    luDef_DefaultMethod(type, "__unm",      unmi, luDef_MethodWrapper);

    if (type->m_methods)
    {
        int i;
        for (i=0; type->m_methods[i].m_name != 0; ++i)
        {
            type->m_methods[i].m_hash = luBinder_hash(type->m_methods[i].m_name);

            if (luDef_isBuiltinMethod(&type->m_methods[i]))
                continue;


            lua_pushstring(L, type->m_methods[i].m_name);
            lua_pushlightuserdata(L, type);
            lua_pushlightuserdata(L, &type->m_methods[i]);
            lua_pushcclosure(L, luDef_MethodWrapper, 2);
            lua_settable(L, -3);
        }
    }
    lua_pop(L, 1);
}

void luBinder::addMethods(luMethodDef *methods)
{
    int i;
    for (i=0; methods[i].m_name != 0; ++i)
    {

        methods[i].m_hash = luBinder_hash(methods[i].m_name);
        if (luDef_isBuiltinMethod(&methods[i]))
            continue;

        lua_pushstring(L, methods[i].m_name);
        lua_pushnumber(L, -1);
        lua_pushlightuserdata(L, &methods[i]);
        lua_pushcclosure(L, luDef_MethodWrapper, 2);
        lua_settable(L, -3);
    }
}


bool luObject::isNil(int idx) const
{
    return lua_isnil(L, idx) != 0;
}

bool luObject::isBoolean(int idx) const
{
    return lua_isboolean(L, idx) != 0;
}

bool luObject::isNumber(int idx) const
{
    return lua_isnumber(L, idx) != 0;
}

bool luObject::isString(int idx) const
{
    return lua_isstring(L, idx) != 0;
}

bool luObject::isClass(int idx) const
{
    return lua_isuserdata(L, idx) != 0 && lua_upvalueindex(1) != 0;
}

bool luObject::getValueBool(int idx) const
{
    if (!isBoolean(idx)) return false;
    return lua_toboolean(L, idx) != 0;
}

double luObject::getValueNumber(int idx) const
{
    if (!isNumber(idx)) return 0.0;
    return (double)lua_tonumber(L, idx);
}

float luObject::getValueFloat(int idx) const
{
    if (!isNumber(idx)) return 0.f;
    return (float)lua_tonumber(L, idx);
}

int luObject::getValueInt(int idx) const
{
    if (!isNumber(idx)) return 0;
    return (int)lua_tonumber(L, idx);
}

luString luObject::getValueString(int idx) const
{
    if (!isString(idx)) return "";
    return lua_tostring(L, idx);
}

luClass *luObject::getValueClass(int idx) const
{
    if (!isClass(idx)) return 0;
    return static_cast<luClass *>(lua_touserdata(L, idx));
}

luTypeDef *luObject::getType(void)
{
    return static_cast<luTypeDef *>(lua_touserdata(L, lua_upvalueindex(LU_UPVALUETYPE)));
}

luMethodDef *luObject::getMethod(void)
{
    return static_cast<luMethodDef *>(lua_touserdata(L, lua_upvalueindex(LU_UPVALUEMETHOD)));
}


int luObject::push( const char *val)
{
    UT_ASSERT(val);
    lua_pushstring(L, val);
    return 1;
}

int luObject::push( const int val)
{
    lua_pushnumber(L, val);
    return 1;
}

int luObject::push(const float val)
{
    lua_pushnumber(L, val);
    return 1;
}

int luObject::push(const bool val)
{
    lua_pushboolean(L, val ? 1 : 0);
    return 1;
}


int luObject::push(luClass *val)
{
    if (val)
    {
        luTypeDef *tdef = val->getType();

        lua_pushlightuserdata(L, val); 
        // bind to table 
        luaL_getmetatable(L, tdef->m_name);
        lua_setmetatable(L, -2);
        return 1;
    }
    return -1;
}

int luObject::push(void)
{
    lua_pushnil(L);
    return 1;
}

int luObject::pushError(const char *fmt, ...)
{
#ifdef _MSC_VER
# define lua_vsnprintf _vsnprintf
#else
# define lua_vsnprintf vsnprintf
#endif

    char buf[256];
    va_list lst;
    va_start(lst, fmt);
    int size = lua_vsnprintf(buf, 256, fmt, lst);
    va_end(lst);

    if (size < 0)
    {
        size = 256;
        buf[size] = 0;
    }
    luaL_error(L, "%s", buf);
    return 1;
}


bool luClass::isTypeOf(luTypeDef *root, luTypeDef *def)
{
    if (utCharEq(root->m_name, def->m_name))
        return true;

    if (root->m_parent)
        return isTypeOf(root->m_parent, def);
    return false;
}



luBinder::luBinder(ltState *_L) : L(_L)
{
    UT_ASSERT(L);
    lua_pushvalue(L, LUA_GLOBALSINDEX);
    m_val = lua_gettop(L);
}

luBinder::~luBinder()
{
    lua_settop(L, m_val);
    lua_pop(L, 1);
}


void luBinder::beginNamespace(const char *title)
{
    lua_pushstring(L, title);
    lua_newtable(L);
}

void luBinder::endNamespace()
{
    lua_settable(L, -3);
}

void luBinder::addConstant(const char *name, float v)
{
    lua_pushnumber(L, (lua_Number)v);
    lua_setfield(L, -2, name);
}

void luBinder::addConstant(const char *name, int v)
{
    lua_pushnumber(L, (lua_Number)v);
    lua_setfield(L, -2, name);
}

void luBinder::addConstant(const char *name, const char *v)
{
    lua_pushstring(L, v);
    lua_setfield(L, -2, name);
}


void luStackDump::dump(void)
{
    int top = lua_gettop(L);
    while (top >= 0)
    {
        char extra[256];
        extra[0] = 0;
        bool ret = false;

        switch (lua_type(L, top))
        {
        case LUA_TNIL:
            sprintf(extra, "LUA_TNIL                : NULL");
            break;
        case LUA_TBOOLEAN:
            sprintf(extra, "LUA_TBOOLEAN            : %s", lua_toboolean(L, top) ? "true" : "false");
            break;
        case LUA_TLIGHTUSERDATA:
            sprintf(extra, "LUA_TLIGHTUSERDATA      : %p", lua_touserdata(L, top));
            break;
        case LUA_TUSERDATA:
            sprintf(extra, "LUA_TUSERDATA           : %p", lua_touserdata(L, top));
            break;
        case LUA_TNUMBER:
            sprintf(extra, "LUA_TNUMBER             : %f", lua_tonumber(L, top));
            break;
        case LUA_TSTRING:
            sprintf(extra, "LUA_TSTRING             : %s", lua_tostring(L, top));
            break;
        case LUA_TTABLE:
            sprintf(extra, "LUA_TTABLE              : %p", lua_topointer(L, top));
            break;
        case LUA_TFUNCTION:
            sprintf(extra, "LUA_TFUNCTION           : %p", lua_topointer(L, top));
            break;
        case LUA_TTHREAD:
            sprintf(extra, "LUA_TTHREAD             : %p", lua_topointer(L, top));
            break;
        }
        printf("%i: %s\n", top, extra);
        --top;
    }
}
