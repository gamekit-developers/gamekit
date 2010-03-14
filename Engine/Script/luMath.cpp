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
#include "luUtils.h"
#include "luMath.h"

// ----------------------------------------------------------------------------
luVector3::~luVector3()
{
}

int luVector3::create(luObject &L, const gkVector3 &vec)
{
    luVector3 *nv = new (&Type, L) luVector3();
    nv->x = vec.x;
    nv->y = vec.y;
    nv->z = vec.z;
    return 1;
}

int luVector3::_normalize(luClass *self, luObject &L)
{
    return L.push(normalise());
}

int luVector3::_dot(luClass *self, luObject &L)
{
    if (!isType(L, 2))
        return L.pushError("expected Vector3.dot(Vector3)");
    return L.push(dotProduct(luVector3::getArg(L, 2)));
}


int luVector3::_cross(luClass *self, luObject &L)
{
    if (!isType(L, 2))
        return L.pushError("expected Vector3.cross(Vector3)");
    return create(L, crossProduct(luVector3::getArg(L, 2)));
}


int luVector3::_length(luClass *self, luObject &L)
{
    return L.push(length());
}

int luVector3::_length2(luClass *self, luObject &L)
{
    return L.push(squaredLength());
}

int luVector3::_dist(luClass *self, luObject &L)
{
    if (!isType(L, 2))
        return L.pushError("expected Vector3.distance(Vector3)");
    return L.push(distance(luVector3::getArg(L, 2)));
}

int luVector3::_dist2(luClass *self, luObject &L)
{
    if (!isType(L, 2))
        return L.pushError("expected Vector3.distance2(Vector3)");
    return L.push(squaredDistance(luVector3::getArg(L, 2)));
}


int luVector3::_get(luClass *self, luObject &L)
{
    if (L.isString(2))
    {
        int val = L.getValueString(2)[0];
        switch (val)
        {
        case '0':
        case 'x':
            return L.push(x);
        case '1':
        case 'y':
            return L.push(y);
        case '2':
        case 'z':
            return L.push(z);
        }
    }
    return 0;
}

int luVector3::_set(luClass *self, luObject &L)
{
    if (L.isString(2))
    {
        int val = L.getValueString(2)[0];
        switch (val)
        {
        case '0':
        case 'x':
            x = L.getValueFloat(3);
            break;
        case '1':
        case 'y':
            y = L.getValueFloat(3);
            break;
        case '2':
        case 'z':
            z = L.getValueFloat(3);
            break;
        }
    }
    return 0;
}

int luVector3::_add(luClass *self, luObject &L)
{
    if (isType(L, 2))
        return create(L, (*this) + luVector3::getArg(L, 2));

    if (L.isNumber(2))
        return create(L, (*this) + L.tofloat(2));

    return L.pushError("unknown operator for Vector3 + '%s'", lua_typename(L, 2));
}

int luVector3::_sub(luClass *self, luObject &L)
{
    if (isType(L, 2))
        return create(L, (*this) - luVector3::getArg(L, 2));

    if (L.isNumber(2))
        return create(L, (*this) - L.tofloat(2));

    return L.pushError("unknown operator for Vector3 - '%s'", lua_typename(L, 2));
}


int luVector3::_mul(luClass *self, luObject &L)
{
    if (isType(L, 2))
        return create(L, (*this) * luVector3::getArg(L, 2));

    if (L.isNumber(2))
        return create(L, (*this) * L.tofloat(2));

    return L.pushError("unknown operator for Vector3 * '%s'", lua_typename(L, 2));
}

int luVector3::_div(luClass *self, luObject &L)
{
    if (isType(L, 2))
    {
        gkVector3 &rhs = luVector3::getArg(L, 2);
        if (rhs.isZeroLength())
            return create(L, *this);
        return create(L, (*this) / rhs );
    }

    if (L.isNumber(2))
    {
        float fv =  L.tofloat(2);
        return create(L, (*this) / (gkFuzzy(fv) ? 1.0 : fv));
    }

    return L.pushError("unknown operator for Vector3 / '%s'", lua_typename(L, 2));
}

int luVector3::_neg(luClass *self, luObject &L)
{
    return create(L, -(*this));
}

int luVector3::_tostring(luClass *self, luObject &L)
{
    char buf[128];
    sprintf(buf, "%f,%f,%f", x, y, z);
    return L.push(buf);
}

// ----------------------------------------------------------------------------
// Globals
static int luVector3_constructor(luObject &L)
{
    gkVector3 v = gkVector3::ZERO;
    if (L.isNumber(1)) v.x = L.getValueFloat(1);
    if (L.isNumber(2)) v.y = L.getValueFloat(2);
    if (L.isNumber(3)) v.z = L.getValueFloat(3);
    return luVector3::create(L, v);
}

luGlobalTableBegin(luVector3)
luGlobalTable("constructor", luVector3_constructor, LU_NOPARAM, ".")
luGlobalTableEnd()


// ----------------------------------------------------------------------------
// Locals
luClassTableBegin(luVector3)
luClassTable("normalize",       luVector3, _normalize,      LU_PARAM,   ".")
luClassTable("dot",             luVector3, _dot,            LU_PARAM,   "..")
luClassTable("cross",           luVector3, _cross,          LU_PARAM,   "..")
luClassTable("length",          luVector3, _length,         LU_PARAM,   ".")
luClassTable("length2",         luVector3, _length2,        LU_PARAM,   ".")
luClassTable("distance",        luVector3, _dist,           LU_PARAM,   "..")
luClassTable("distance2",       luVector3, _dist2,          LU_PARAM,   "..")
luClassTable("__tostring",      luVector3, _tostring,       LU_PARAM,   ".")
luClassTable("__getter",        luVector3, _get,            LU_PARAM,   ".s")
luClassTable("__setter",        luVector3, _set,            LU_PARAM,   ".sf")
luClassTable("__add",           luVector3, _add,            LU_NOPARAM, "")
luClassTable("__sub",           luVector3, _sub,            LU_NOPARAM, "")
luClassTable("__mul",           luVector3, _mul,            LU_NOPARAM, "")
luClassTable("__div",           luVector3, _div,            LU_NOPARAM, "")
luClassTable("__unm",           luVector3, _neg,            LU_NOPARAM, "")
luClassTableEnd()

luClassImpl("Vector3", luVector3, 0);


// ----------------------------------------------------------------------------

luQuat::~luQuat()
{
}

int luQuat::create(luObject &L, const gkQuaternion &q)
{
    luQuat *nv = new (&Type, L) luQuat();
    nv->x = q.x;
    nv->y = q.y;
    nv->z = q.z;
    nv->w = q.w;
    return 1;
}

int luQuat::_normalize(luClass *self, luObject &L)
{
    return L.push(normalise());
}

int luQuat::_inverse(luClass *self, luObject &L)
{
    return create(L, Inverse());
}

int luQuat::_dot(luClass *self, luObject &L)
{
    if (!isType(L, 2))
        return L.pushError("expected Quaternion.dot(Quaternion)");

    return create(L, Dot(luQuat::getArg(L, 2)));

}

int luQuat::_get(luClass *self, luObject &L)
{
    if (L.isString(2))
    {
        int val = L.getValueString(2)[0];
        switch (val)
        {
        case '0':
        case 'w':
            return L.push(w);
        case '1':
        case 'x':
            return L.push(x);
        case '2':
        case 'y':
            return L.push(y);
        case '3':
        case 'z':
            return L.push(z);
        }
    }
    return 0;
}

int luQuat::_set(luClass *self, luObject &L)
{
    if (L.isString(2))
    {
        int val = L.getValueString(2)[0];
        switch (val)
        {
        case '0':
        case 'w':
            w = L.getValueFloat(3);
            break;
        case '1':
        case 'x':
            x = L.getValueFloat(3);
            break;
        case '2':
        case 'y':
            y = L.getValueFloat(3);
            break;
        case '3':
        case 'z':
            z = L.getValueFloat(3);
            break;
        }
    }
    return 0;
}

int luQuat::_mul(luClass *self, luObject &L)
{
    if (luVector3::isType(L, 2))
        return luVector3::create(L, (*this) * luVector3::getArg(L, 2));

    if (isType(L, 2))
        return create(L, (*this) *  luQuat::getArg(L, 2));

    if (L.isNumber(2))
        return create(L, (*this) *   L.tofloat(2) );

    return L.pushError("unknown operator for Quaternion * '%s'", lua_typename(L, 2));
}


int luQuat::_tostring(luClass *self, luObject &L)
{
    char buf[128];
    sprintf(buf, "%f,%f,%f,%f", w, x, y, z);
    return L.push(buf);
}



// ----------------------------------------------------------------------------
// Globals

static int luQuat_constructor(luObject &L)
{
    gkQuaternion q = gkQuaternion::IDENTITY;
    if (L.isNumber(1)) q.w = L.getValueFloat(1);
    if (L.isNumber(2)) q.x = L.getValueFloat(2);
    if (L.isNumber(3)) q.y = L.getValueFloat(3);
    if (L.isNumber(4)) q.z = L.getValueFloat(4);
    return luQuat::create(L, q);
}


luGlobalTableBegin(luQuat)
luGlobalTable("constructor", luQuat_constructor, LU_NOPARAM, ".")
luGlobalTableEnd()


// ----------------------------------------------------------------------------
// Locals
luClassTableBegin(luQuat)
luClassTable("normalize",       luQuat, _normalize,     LU_PARAM,   ".")
luClassTable("inverse",         luQuat, _inverse,       LU_PARAM,   ".")
luClassTable("dot",             luQuat, _dot,           LU_PARAM,   "..")
luClassTable("__tostring",      luQuat, _tostring,      LU_PARAM,   ".")
luClassTable("__getter",        luQuat, _get,           LU_PARAM,   ".s")
luClassTable("__setter",        luQuat, _set,           LU_PARAM,   ".sf")
luClassTable("__mul",           luQuat, _mul,           LU_NOPARAM, "")
luClassTableEnd()

luClassImpl("Quaternion", luQuat, 0);


// ----------------------------------------------------------------------------

static int luMath_LerpF(luObject &L)
{
    return L.push(gkMathUtils::interp(L.tofloat(1), L.tofloat(2), gkClampf(L.tofloat(3), 0.f, 1.f)));
}

static int luMath_Vec3Lerp(luObject &L)
{
    if (!luVector3::isType(L, 1) || !luVector3::isType(L, 2))
        return L.pushError("expected Vec3Lerp(Vector3, Vector3, float)");

    return luVector3::create(L, gkMathUtils::interp(luVector3::getArg(L, 1), luVector3::getArg(L, 2), gkClampf(L.tofloat(3),  0.f, 1.f)));
}



static int luMath_QuatLerp(luObject &L)
{
    if (!luQuat::isType(L, 1) || !luQuat::isType(L, 2))
        return L.pushError("expected QuatLerp(Quaternion, Quaternion, float)");
    return luQuat::create(L, gkMathUtils::interp(luQuat::getArg(L, 1), luQuat::getArg(L, 2), gkClampf(L.tofloat(3),  0.f, 1.f), true));
}

static int luMath_QuatSlerp(luObject &L)
{
    if (!luQuat::isType(L, 1) || !luQuat::isType(L, 2))
        return L.pushError("expected QuatLerp(Quaternion, Quaternion, float)");
    return luQuat::create(L, gkMathUtils::interp(luQuat::getArg(L, 1), luQuat::getArg(L, 2), gkClampf(L.tofloat(3),  0.f, 1.f), true));
}


luMethodDef Math_Methods[] =
{
    // interpolation
    {"LerpF",           luMath_LerpF,               LU_PARAM, "fff"},
    {"Vec3Lerp",        luMath_Vec3Lerp,            LU_PARAM, "..f"},
    {"QuatLerp",        luMath_QuatLerp,            LU_PARAM, "..f"},
    {"QuatSlerp",       luMath_QuatSlerp,           LU_PARAM, "..f"},
    {0,0,0,0}
};


void luMath_Open(ltState *L)
{
    // keys are global
    luBinder lua(L);
    lua.beginNamespace("Math");
    lua.addConstant("Pi",   gkPi);
    lua.addConstant("Pi2",  gkPi2);
    lua.addConstant("PiH",  gkPih);
    lua.addConstant("DPR",  gkDPR);
    lua.addConstant("RPD",  gkRPD);

    lua.addMethods(Math_Methods);
    lua.addClassType(&luQuat::Type);
    lua.addClassType(&luVector3::Type);

    lua.endNamespace();
}
