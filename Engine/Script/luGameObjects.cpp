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
#include "luGameObjects.h"
#include "luMath.h"
#include "gkScene.h"
#include "gkGameObject.h"
#include "gkCamera.h"
#include "gkEntity.h"
#include "gkLight.h"
#include "gkSkeleton.h"


// ----------------------------------------------------------------------------
luGameObject::luGameObject(gkObject *ob) : luLoadable(ob)
{
}

luGameObject::~luGameObject()
{
}

void luGameObject::bind(luBinder &L)
{
    L.addConstant("OB_CAMERA",     GK_CAMERA);
    L.addConstant("OB_LIGHT",      GK_LIGHT);
    L.addConstant("OB_ENTITY",     GK_ENTITY);
    L.addConstant("OB_EMPTY",      GK_OBJECT);
    L.addConstant("OB_SKELETON",   GK_SKELETON);
}

static int luGameObject_getPosition(luObject &L)
{
    if (!LU_IsGameObject(L, 1))
        return L.pushError("expected GameObject");

    return LU_Vec3New(L, LU_GetGameObject(L, 1).getPosition());
}

static int luGameObject_getRotation(luObject &L)
{
    if (!LU_IsGameObject(L, 1))
        return L.pushError("expected GameObject");

    return LU_Vec3New(L, LU_GetGameObject(L, 1).getRotation().toVector3());
}


static int luGameObject_getOrientation(luObject &L)
{
    if (!LU_IsGameObject(L, 1))
        return L.pushError("expected GameObject");

    return LU_QuatNew(L, LU_GetGameObject(L, 1).getOrientation());
}


static int luGameObject_getScale(luObject &L)
{
    if (!LU_IsGameObject(L, 1))
        return L.pushError("expected GameObject");

    return LU_Vec3New(L, LU_GetGameObject(L, 1).getScale());
}



static int luGameObject_getWorldPosition(luObject &L)
{
    if (!LU_IsGameObject(L, 1))
        return L.pushError("expected GameObject");

    return LU_Vec3New(L, LU_GetGameObject(L, 1).getWorldPosition());
}

static int luGameObject_getWorldRotation(luObject &L)
{
    if (!LU_IsGameObject(L, 1))
        return L.pushError("expected GameObject");

    return LU_Vec3New(L, LU_GetGameObject(L, 1).getWorldRotation().toVector3());
}


static int luGameObject_getWorldOrientation(luObject &L)
{
    if (!LU_IsGameObject(L, 1))
        return L.pushError("expected GameObject");

    return LU_QuatNew(L, LU_GetGameObject(L, 1).getWorldOrientation());
}


static int luGameObject_getLinearVelocity(luObject &L)
{
    if (!LU_IsGameObject(L, 1))
        return L.pushError("expected GameObject");

    return LU_Vec3New(L, LU_GetGameObject(L, 1).getLinearVelocity());
}

static int luGameObject_getAngularVelocity(luObject &L)
{
    if (!LU_IsGameObject(L, 1))
        return L.pushError("expected GameObject");

    return LU_Vec3New(L, LU_GetGameObject(L, 1).getAngularVelocity());
}

static int luGameObject_setLinearVelocity(luObject &L)
{
    if (!LU_IsGameObject(L, 1))
        return L.pushError("expected GameObject");

    if (!LU_IsVec3(L, 2))
        return L.pushError("expected setLinearVelocity(Vector3)");

    LU_GetGameObject(L, 1).setLinearVelocity(LU_GetVec3Arg(L, 2));
    return 0;
}


static int luGameObject_setAngularVelocity(luObject &L)
{
    if (!LU_IsGameObject(L, 1))
        return L.pushError("expected GameObject");

    if (!LU_IsVec3(L, 2))
        return L.pushError("expected setAngularVelocity(Vector3)");

    LU_GetGameObject(L, 1).setAngularVelocity(LU_GetVec3Arg(L, 2));
    return 0;
}

static int luGameObject_setPosition(luObject &L)
{
    if (!LU_IsGameObject(L, 1))
        return L.pushError("expected GameObject");

    if (!LU_IsVec3(L, 2))
        return L.pushError("expected setPosition(Vector3)");

    LU_GetGameObject(L, 1).setPosition(LU_GetVec3Arg(L, 2));
    return 0;
}

static int luGameObject_setRotation(luObject &L)
{
    if (!LU_IsGameObject(L, 1))
        return L.pushError("expected GameObject");

    if (!LU_IsVec3(L, 2))
        return L.pushError("expected setRotation(Vector3)");

    LU_GetGameObject(L, 1).setOrientation(gkEuler(LU_GetVec3Arg(L, 2)).toQuaternion());
    return 0;
}


static int luGameObject_setOrientation(luObject &L)
{
    if (!LU_IsGameObject(L, 1))
        return L.pushError("expected GameObject");
    if (!LU_IsQuat(L, 2))
        return L.pushError("expected setOrientation(Quaternion)");

    LU_GetGameObject(L, 1).setOrientation(LU_GetQuatArg(L, 2));
    return 0;
}



static int luGameObject_getType(luObject &L)
{
    if (!LU_IsGameObject(L, 1))
        return L.pushError("expected GameObject");

    return L.push(LU_GetGameObject(L, 1).getType());
}

static int luGameObject_getTransformSpace(luObject &L, int n)
{
    int space = TRANSFORM_PARENT;
    if (L.isBoolean(n))
        space = L.toboolean(n)? TRANSFORM_LOCAL : TRANSFORM_PARENT;
    return space;
}

static int luGameObject_rotate(luObject &L)
{
    if (!LU_IsGameObject(L, 1))
        return L.pushError("expected GameObject");

    if (!LU_IsVec3(L, 2) || !LU_IsQuat(L, 2))
        return L.pushError("expected rotate((Vector3 or Quaternion), bool)");

    gkQuaternion rot;
    if (LU_IsVec3(L, 2))
        rot = gkEuler(LU_GetVec3Arg(L, 2)).toQuaternion();
    else rot = LU_GetQuatArg(L, 2);

    LU_GetGameObject(L, 1).rotate(rot, luGameObject_getTransformSpace(L, 3));
    return 0;
}


static int luGameObject_translate(luObject &L)
{
    if (!LU_IsGameObject(L, 1))
        return L.pushError("expected GameObject");

    if (!LU_IsVec3(L, 2))
        return L.pushError("expected translate(Vector3, bool)");

    LU_GetGameObject(L, 1).translate(LU_GetVec3Arg(L, 2),luGameObject_getTransformSpace(L, 3));
    return 0;
}


static int luGameObject_scale(luObject &L)
{
    if (!LU_IsGameObject(L, 1))
        return L.pushError("expected GameObject");

    if (!LU_IsVec3(L, 2))
        return L.pushError("expected scale(Vector3, bool)");

    LU_GetGameObject(L, 1).scale(LU_GetVec3Arg(L, 2));
    return 0;
}

static int luGameObject_yaw(luObject &L)
{
    if (!LU_IsGameObject(L, 1))
        return L.pushError("expected GameObject");

    LU_GetGameObject(L, 1).yaw(gkRadian(L.tofloat(2)), luGameObject_getTransformSpace(L, 3));
    return 0;
}

static int luGameObject_pitch(luObject &L)
{
    if (!LU_IsGameObject(L, 1))
        return L.pushError("expected GameObject");

    LU_GetGameObject(L, 1).pitch(gkRadian(L.tofloat(2)), luGameObject_getTransformSpace(L, 3));
    return 0;
}

static int luGameObject_roll(luObject &L)
{
    if (!LU_IsGameObject(L, 1))
        return L.pushError("expected GameObject");

    LU_GetGameObject(L, 1).roll(gkRadian(L.tofloat(2)),luGameObject_getTransformSpace(L, 3));
    return 0;
}


luMethodDef luGameObject::Methods[] =
{
    {"getPosition",         luGameObject_getPosition,           LU_PARAM, "."},
    {"getRotation",         luGameObject_getRotation,           LU_PARAM, "."},
    {"getOrientation",      luGameObject_getOrientation,        LU_PARAM, "."},
    {"getScale",            luGameObject_getScale,              LU_PARAM, "."},
    {"getWorldPosition",    luGameObject_getWorldPosition,      LU_PARAM, "."},
    {"getWorldRotation",    luGameObject_getWorldRotation,      LU_PARAM, "."},
    {"getWorldOrientation", luGameObject_getWorldOrientation,   LU_PARAM, "."},

    {"getLinearVelocity",   luGameObject_getLinearVelocity,     LU_PARAM, "."},
    {"getAngularVelocity",  luGameObject_getAngularVelocity,    LU_PARAM, "."},
    {"setLinearVelocity",   luGameObject_setLinearVelocity,     LU_PARAM, "."},
    {"setAngularVelocity",  luGameObject_setAngularVelocity,    LU_PARAM, "."},


    {"setPosition",         luGameObject_setPosition,           LU_PARAM, ".."},
    {"setRotation",         luGameObject_setRotation,           LU_PARAM, ".."},
    {"setOrientation",      luGameObject_setOrientation,        LU_PARAM, ".."},
    {"getType",             luGameObject_getType,               LU_PARAM, "."},


    {"rotate",              luGameObject_rotate,                LU_PARAM, "..|b"},
    {"translate",           luGameObject_translate,             LU_PARAM, "..|b"},
    {"scale",               luGameObject_scale,                 LU_PARAM, ".."},

    {"yaw",                 luGameObject_yaw,                   LU_PARAM, ".n|b"},
    {"pitch",               luGameObject_pitch,                 LU_PARAM, ".n|b"},
    {"roll",                luGameObject_roll,                  LU_PARAM, ".n|b"},

    {0,0,0,0}
};

luTypeDef luGameObject::Type = {"GameObject", &luLoadable::Type, Methods};


// ----------------------------------------------------------------------------
luCamera::luCamera(gkObject *ob) : luGameObject(ob)
{
}

luCamera::~luCamera()
{
}



luMethodDef luCamera::Methods[] =
{
    {0,0,0,0}
};

luTypeDef luCamera::Type = {"Camera", &luGameObject::Type, Methods};




// ----------------------------------------------------------------------------
luEntity::luEntity(gkObject *ob) : luGameObject(ob)
{
}

luEntity::~luEntity()
{
}

static int luEntity_playAction(luObject &L)
{
    if (!LU_IsEntity(L, 1))
        return L.pushError("expected Entity");

    int blend = 0;
    if (L.isNumber(3)) blend = L.toint(3);

    LU_GeEntity(L, 1).playAction(L.tostring(2), blend);
    return 0;
}

luMethodDef luEntity::Methods[] =
{
    {"playAction",         luEntity_playAction,     LU_PARAM, ".s|i"},
    {0,0,0,0}
};

luTypeDef luEntity::Type = {"Entity", &luGameObject::Type, Methods};



// ----------------------------------------------------------------------------
luLight::luLight(gkObject *ob) : luGameObject(ob)
{
}

luLight::~luLight()
{
}



luMethodDef luLight::Methods[] =
{
    {0,0,0,0}
};

luTypeDef luLight::Type = {"Light", &luLight::Type, Methods};
