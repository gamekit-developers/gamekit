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
luVector3::luVector3(const gkVector3 &_vec)
    :   vec(_vec)
{
}
luVector3::~luVector3()
{
}


static int luVector3_new(luObject &L)
{
    gkVector3 v = gkVector3::ZERO;
    if (L.isNumber(1)) v.x = L.getValueFloat(1);
    if (L.isNumber(2)) v.y = L.getValueFloat(2);
    if (L.isNumber(3)) v.z = L.getValueFloat(3);

    new(&luVector3::Type, L) luVector3(v);
    return 1;
}


static int luVector3_get(luObject &L)
{
    gkVector3 &vec = L.getValueClassT<luVector3>(1)->vec;
    if (L.isString(2))
    {
        int val = L.getValueString(2)[0];
        switch (val)
        {
        case '0':
        case 'x':
            return L.push(vec.x);
        case '1':
        case 'y':
            return L.push(vec.y);
        case '2':
        case 'z':
            return L.push(vec.z);
        }
    }
    return 0;
}

static int luVector3_set(luObject &L)
{
    gkVector3 &vec = L.getValueClassT<luVector3>(1)->vec;
    if (L.isString(2))
    {
        int val = L.getValueString(2)[0];
        switch (val)
        {
        case '0':
        case 'x':
            vec.x = L.getValueFloat(3);
            break;
        case '1':
        case 'y':
            vec.y = L.getValueFloat(3);
            break;
        case '2':
        case 'z':
            vec.z = L.getValueFloat(3);
            break;
        }
    }
    return 0;
}

static int luVector3_tstr(luObject &L)
{
    gkVector3 &vec = L.getValueClassT<luVector3>(1)->vec;
    char buf[128];
    sprintf(buf, "%f,%f,%f", vec.x, vec.y, vec.z);
    return L.push(buf);
}



static int luVector3_add(luObject &L)
{
    if (!LU_IsVec3(L, 1))
        return L.pushError("expected Vector3 self");

    gkVector3 &vec = LU_GetVec3Arg(L, 1);

    if (LU_IsVec3(L, 2))
        return LU_Vec3New(L, vec + LU_GetVec3Arg(L, 2));
    
    if (L.isNumber(2))
        return LU_Vec3New(L, vec + L.tofloat(2));

    return L.pushError("unknown operator for Vector3 + '%s'", lua_typename(L, 2)); 
}

static int luVector3_sub(luObject &L)
{
    if (!LU_IsVec3(L, 1))
        return L.pushError("expected Vector3 self");

    gkVector3 &vec = LU_GetVec3Arg(L, 1);

    if (LU_IsVec3(L, 2))
        return LU_Vec3New(L, vec - LU_GetVec3Arg(L, 2));
    
    if (L.isNumber(2))
        return LU_Vec3New(L, vec - L.tofloat(2));

    return L.pushError("unknown operator for Vector3 - '%s'", lua_typename(L, 2)); 
}

static int luVector3_mul(luObject &L)
{
    if (!LU_IsVec3(L, 1))
        return L.pushError("expected Vector3 self");

    gkVector3 &vec = LU_GetVec3Arg(L, 1);

    if (LU_IsVec3(L, 2))
        return LU_Vec3New(L, vec * LU_GetVec3Arg(L, 2));
    
    if (L.isNumber(2))
        return LU_Vec3New(L, vec * L.tofloat(2));

    return L.pushError("unknown operator for Vector3 * '%s'", lua_typename(L, 2)); 
}



static int luVector3_div(luObject &L)
{
    if (!LU_IsVec3(L, 1))
        return L.pushError("expected Vector3 self");

    gkVector3 &vec = LU_GetVec3Arg(L, 1);

    if (LU_IsVec3(L, 2))
    {
        gkVector3 &rhs = LU_GetVec3Arg(L, 2);
        if (rhs.isZeroLength())
            return LU_Vec3New(L, vec);
        return LU_Vec3New(L, vec / rhs );
    }

    if (L.isNumber(2))
    {
        float fv =  L.tofloat(2);
        return LU_Vec3New(L, vec / (gkFuzzy(fv) ? 1.0 : fv));
    }

    return L.pushError("unknown operator for Vector3 / '%s'", lua_typename(L, 2)); 
}



static int luVector3_unm(luObject &L)
{
    if (!LU_IsVec3(L, 1))
        return L.pushError("expected Vector3 self");
    return LU_Vec3New(L, -LU_GetVec3Arg(L, 1));
}


// ----------------------------------------------------------------------------
static int luVector3_Normalize(luObject &L)
{
    if (!LU_IsVec3(L, 1))
        return L.pushError("expected Vec3Normalize(Vector3)");

    return L.push(LU_GetVec3Arg(L, 1).normalise());
}

static int luVector3_Dot(luObject &L)
{
    if (!LU_IsVec3(L, 1) || !LU_IsVec3(L, 2))
        return L.pushError("expected Vec3Dot(Vector3, Vector3)");

    return L.push(LU_GetVec3Arg(L, 1).dotProduct(LU_GetVec3Arg(L, 2)));
}

static int luVector3_Cross(luObject &L)
{
    if (!LU_IsVec3(L, 1) || !LU_IsVec3(L, 2))
        return L.pushError("expected Vec3Cross(Vector3, Vector3)");

    return LU_Vec3New(L, LU_GetVec3Arg(L, 1).crossProduct(LU_GetVec3Arg(L, 2)));
}

static int luVector3_Len(luObject &L)
{
    if (!LU_IsVec3(L, 1))
        return L.pushError("expected Vec3Len(Vector3)");
    return L.push(LU_GetVec3Arg(L, 1).length());
}


static int luVector3_LenSq(luObject &L)
{
    if (!LU_IsVec3(L, 1))
        return L.pushError("expected Vec3LenSq(Vector3)");
    return L.push(LU_GetVec3Arg(L, 1).squaredLength());
}


static int luVector3_Dist(luObject &L)
{
    if (!LU_IsVec3(L, 1) || !LU_IsVec3(L, 2))
        return L.pushError("expected Vec3Dist(Vector3, Vector3)");
    return L.push(LU_GetVec3Arg(L, 1).distance(LU_GetVec3Arg(L, 2)));
}

static int luVector3_DistSq(luObject &L)
{
    if (!LU_IsVec3(L, 1) || !LU_IsVec3(L, 2))
        return L.pushError("expected Vec3DistSq(Vector3, Vector3)");
    return L.push(LU_GetVec3Arg(L, 1).squaredDistance(LU_GetVec3Arg(L, 2)));
}

luMethodDef luVector3::Methods[] =
{
    {"constructor",     luVector3_new,          LU_NOPARAM, "|fff"},
    {"normalize",       luVector3_Normalize,    LU_PARAM,   "."},
    {"dot",             luVector3_Dot,          LU_PARAM,   ".."},
    {"cross",           luVector3_Cross,        LU_PARAM,   ".."},
    {"length",          luVector3_Len,          LU_PARAM,   "."},
    {"length2",         luVector3_LenSq,        LU_PARAM,   "."},
    {"distance",        luVector3_Dist,         LU_PARAM,   ".."},
    {"distance2",       luVector3_DistSq,       LU_PARAM,   ".."},
    {"__tostring",      luVector3_tstr,         LU_PARAM,   "."},
    {"__getter",        luVector3_get,          LU_PARAM,   ".s"},
    {"__setter",        luVector3_set,          LU_PARAM,   ".sf"},
    {"__add",           luVector3_add,          LU_NOPARAM, ""},
    {"__sub",           luVector3_sub,          LU_NOPARAM, ""},
    {"__mul",           luVector3_mul,          LU_NOPARAM, ""},
    {"__mul",           luVector3_div,          LU_NOPARAM, ""},
    {"__unm",           luVector3_unm,          LU_NOPARAM, ""},
    {0,0,0,0}
};

luTypeDef luVector3::Type = {"Vector3", 0, Methods};

// ----------------------------------------------------------------------------
luQuat::luQuat(const gkQuaternion &_q)
    :   quat(_q)
{
}

luQuat::~luQuat()
{
}


static int luQuat_new(luObject &L)
{
    gkQuaternion q = gkQuaternion::IDENTITY;
    if (L.isNumber(1)) q.w = L.getValueFloat(1);
    if (L.isNumber(2)) q.x = L.getValueFloat(2);
    if (L.isNumber(3)) q.y = L.getValueFloat(3);
    if (L.isNumber(4)) q.z = L.getValueFloat(4);


    new(&luQuat::Type, L) luQuat(q);
    return 1;
}

static int luQuat_get(luObject &L)
{
    gkQuaternion &q= L.getValueClassT<luQuat>(1)->quat;
    if (L.isString(2))
    {
        int val = L.getValueString(2)[0];
        switch (val)
        {
        case '1':
        case 'x':
            return L.push(q.x);
        case '2':
        case 'y':
            return L.push(q.y);
        case '3':
        case 'z':
            return L.push(q.z);
        case '0':
        case 'w':
            return L.push(q.w);
        }
    }
    return 0;
}


static int luQuat_set(luObject &L)
{
    gkQuaternion &q = L.getValueClassT<luQuat>(1)->quat;
    if (L.isString(2))
    {
        int val = L.getValueString(2)[0];
        switch (val)
        {
        case '1':
        case 'x':
            q.x = L.getValueFloat(3);
            break;
        case '2':
        case 'y':
            q.y = L.getValueFloat(3);
            break;
        case '3':
        case 'z':
            q.z = L.getValueFloat(3);
            break;
        case '0':
        case 'w':
            q.w = L.getValueFloat(3);
            break;
        }
    }
    return 0;
}


static int luQuat_mul(luObject &L)
{
    if (!LU_IsQuat(L, 1))
        return L.pushError("expected Quaternion self");

    gkQuaternion &q = LU_GetQuatArg(L, 1);

    if (LU_IsVec3(L, 2))
        return LU_Vec3New(L, q * LU_GetVec3Arg(L, 2));

    if (LU_IsQuat(L, 2))
        return LU_QuatNew(L, q * LU_GetQuatArg(L, 2));
    
    if (L.isNumber(2))
        return LU_QuatNew(L, q * L.tofloat(2));

    return L.pushError("unknown operator for Quaternion * '%s'", lua_typename(L, 2)); 
}



static int luQuat_tstr(luObject &L)
{
    gkQuaternion &q = L.getValueClassT<luQuat>(1)->quat;

    char buf[128];
    sprintf(buf, "%f,%f,%f,%f", q.w, q.x, q.y, q.z);
    return L.push(buf);
}



static int luQuat_Dot(luObject &L)
{
    if (!LU_IsQuat(L, 1) || !LU_IsQuat(L, 2))
        return L.pushError("expected QuatDot(Quaternion, Quaternion)");
    return L.push(LU_GetQuatArg(L, 1).Dot(LU_GetQuatArg(L, 2)));
}


static int luQuat_Normalize(luObject &L)
{
    if (!LU_IsQuat(L, 1))
        return L.pushError("expected QuatNormalize(Quaternion)");

    return L.push(LU_GetQuatArg(L, 1).normalise());
}



luMethodDef luQuat::Methods[] =
{
    {"constructor",     luQuat_new,         LU_NOPARAM, "|fff"},
    {"dot",             luQuat_Dot,         LU_PARAM,   ".."},
    {"normalize",       luQuat_Normalize,   LU_PARAM,   "."},
    {"__tostring",      luQuat_tstr,        LU_PARAM,   "."},
    {"__getter",        luQuat_get,         LU_PARAM,   ".s"},
    {"__setter",        luQuat_set,         LU_PARAM,   ".sf"},
    {"__mul",           luQuat_mul,         LU_PARAM,   ".|"},
    {0,0,0,0}
};


luTypeDef luQuat::Type = {"Quaternion", 0, Methods};

// ----------------------------------------------------------------------------

static int luMath_LerpF(luObject &L)
{
    return L.push(gkMathUtils::interp(L.tofloat(1), L.tofloat(2), gkClampf(L.tofloat(3), 0.f, 1.f)));
}

static int luMath_Vec3Lerp(luObject &L)
{
    if (!LU_IsVec3(L, 1) || !LU_IsVec3(L, 2))
        return L.pushError("expected Vec3Lerp(Vector3, Vector3, float )");

    return LU_Vec3New(L,
                      gkMathUtils::interp(LU_GetVec3Arg(L, 1), LU_GetVec3Arg(L, 2),
                                          gkClampf(L.tofloat(3),  0.f, 1.f)));
}



static int luMath_QuatLerp(luObject &L)
{
    if (!LU_IsQuat(L, 1) || !LU_IsQuat(L, 2))
        return L.pushError("expected QuatLerp(Quaternion, Quaternion, float)");
    return LU_QuatNew(L,
                      gkMathUtils::interp(LU_GetQuatArg(L, 1), LU_GetQuatArg(L, 2),
                                          gkClampf(L.tofloat(3),  0.f, 1.f), true));
}


static int luMath_QuatSlerp(luObject &L)
{
    if (!LU_IsQuat(L, 1) || !LU_IsQuat(L, 2))
        return L.pushError("expected QuatSlerp(Quaternion, Quaternion, float)");

    return LU_QuatNew(L,
                      gkMathUtils::interp(LU_GetQuatArg(L, 1), LU_GetQuatArg(L, 2),
                                          gkClampf(L.tofloat(3),  0.f, 1.f), false));
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
    lua.addType(&luQuat::Type);
    lua.addType(&luVector3::Type);

    lua.endNamespace();
}
