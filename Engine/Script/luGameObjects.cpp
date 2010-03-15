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
#include "luScene.h"
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

int luGameObject::create(luObject &L, gkGameObject *ob)
{
    GK_ASSERT(ob);

    switch (ob->getType())
    {
    case GK_CAMERA:
        new (&luCamera::Type, L) luCamera(ob);
        return 1;
    case GK_LIGHT:
        new (&luLight::Type, L) luLight(ob);
        return 1;
    case GK_ENTITY:
        new (&luEntity::Type, L) luEntity(ob);
        return 1;
    case GK_SKELETON:
    case GK_OBJECT:
        new (&luGameObject::Type, L) luGameObject(ob);
        return 1;
    }
    return 0;
}


// Math.Vector3 GameObject:getPosition()
int luGameObject::getPosition(luClass *self, luObject &L)
{
    return luVector3::create(L, ref<gkGameObject>().getPosition());
}

// Math.Vector3 GameObject:getRotation()
int luGameObject::getRotation(luClass *self, luObject &L)
{
    return luVector3::create(L, ref<gkGameObject>().getRotation().toVector3());
}

// Math.Quaternion GameObject:getOrientation()
int luGameObject::getOrientation(luClass *self, luObject &L)
{
    return luQuat::create(L, ref<gkGameObject>().getOrientation());
}


// Math.Vector3 GameObject:getScale()
int luGameObject::getScale(luClass *self, luObject &L)
{
    return luVector3::create(L, ref<gkGameObject>().getScale());
}

// Math.Vector3 GameObject:getWorldPosition()
int luGameObject::getWorldPosition(luClass *self, luObject &L)
{
    return luVector3::create(L, ref<gkGameObject>().getWorldPosition());
}


// Math.Vector3 GameObject:getWorldRotation()
int luGameObject::getWorldRotation(luClass *self, luObject &L)
{
    return luVector3::create(L, ref<gkGameObject>().getWorldRotation().toVector3());
}


// Math.Quaternion GameObject:getWorldOrientation()
int luGameObject::getWorldOrientation(luClass *self, luObject &L)
{
    return luQuat::create(L, ref<gkGameObject>().getWorldOrientation());
}


// Math.Vector3 GameObject:getLinearVelocity()
int luGameObject::getLinearVelocity(luClass *self, luObject &L)
{
    return luVector3::create(L, ref<gkGameObject>().getLinearVelocity());
}


// Math.Vector3 GameObject:getAngularVelocity()
int luGameObject::getAngularVelocity(luClass *self, luObject &L)
{
    return luVector3::create(L, ref<gkGameObject>().getAngularVelocity());
}


// nil GameObject:setLinearVelocity(Math.Vector3)
int luGameObject::setLinearVelocity(luClass *self, luObject &L)
{
    if (!luVector3::isType(L, 2))
        return L.pushError("expected setLinearVelocity(Vector3)");

    ref<gkGameObject>().setLinearVelocity(luVector3::getArg(L, 2));
    return 0;
}


// nil GameObject:setAngularVelocity(Math.Vector3)
int luGameObject::setAngularVelocity(luClass *self, luObject &L)
{
    if (!luVector3::isType(L, 2))
        return L.pushError("expected setAngularVelocity(Vector3)");

    ref<gkGameObject>().setAngularVelocity(luVector3::getArg(L, 2));
    return 0;
}


// nil GameObject:setPosition(Math.Vector3)
int luGameObject::setPosition(luClass *self, luObject &L)
{
    if (!luVector3::isType(L, 2))
        return L.pushError("expected setPosition(Vector3)");

    ref<gkGameObject>().setPosition(luVector3::getArg(L, 2));
    return 0;
}


// nil GameObject:setRotation(Math.Vector3)
int luGameObject::setRotation(luClass *self, luObject &L)
{
    if (!luVector3::isType(L, 2))
        return L.pushError("expected setRotation(Vector3)");

    ref<gkGameObject>().setOrientation(gkEuler(luVector3::getArg(L, 2)).toQuaternion());
    return 0;
}


// nil GameObject:setOrientation(Math.Quaternion)
int luGameObject::setOrientation(luClass *self, luObject &L)
{
    if (!luQuat::isType(L, 2))
        return L.pushError("expected setOrientation(Quaternion)");

    ref<gkGameObject>().setOrientation(luQuat::getArg(L, 2));
    return 0;
}


// Enum GameObject:getType()
int luGameObject::_getType(luClass *self, luObject &L)
{
    return L.push((int)ref<gkGameObject>().getType());
}

static int luGameObject_getTransformSpace(luObject &L, int n)
{
    int space = TRANSFORM_PARENT;
    if (L.isBoolean(n))
        space = L.toboolean(n)? TRANSFORM_LOCAL : TRANSFORM_PARENT;
    return space;
}


// nil GameObject:rotate(Math.Vector3, bool=false)
// nil GameObject:rotate(Math.Quaternion, bool=false)
int luGameObject::rotate(luClass *self, luObject &L)
{
    if (!luVector3::isType(L, 2) && !luQuat::isType(L, 2))
        return L.pushError("expected rotate((Vector3 or Quaternion), bool)");


    gkQuaternion rot;
    if (luVector3::isType(L, 2))
        rot = gkEuler(luVector3::getArg(L, 2)).toQuaternion();
    else 
        rot = luQuat::getArg(L, 2);

    ref<gkGameObject>().rotate(rot, luGameObject_getTransformSpace(L, L.toboolean(3)));
    return 0;
}


// nil GameObject:translate(Math.Vector3, bool=false)
int luGameObject::translate(luClass *self, luObject &L)
{
    if (!luVector3::isType(L, 2))
        return L.pushError("expected translate(Vector3, bool)");

    ref<gkGameObject>().translate(luVector3::getArg(L, 2), luGameObject_getTransformSpace(L, L.toboolean(3)));
    return 0;
}

// nil GameObject:scale(Math.Vector3)
int luGameObject::scale(luClass *self, luObject &L)
{
    if (!luVector3::isType(L, 2))
        return L.pushError("expected scale(Vector3)");

    ref<gkGameObject>().scale(luVector3::getArg(L, 2));
    return 0;
}


// nil GameObject:yaw(Number, bool=false)
int luGameObject::yaw(luClass *self, luObject &L)
{
    ref<gkGameObject>().yaw(gkRadian(L.tofloat(2)), luGameObject_getTransformSpace(L, L.toboolean(3)));
    return 0;
}

// nil GameObject:pitch(Number, bool=false)
int luGameObject::pitch(luClass *self, luObject &L)
{
    ref<gkGameObject>().pitch(gkRadian(L.tofloat(2)), luGameObject_getTransformSpace(L, L.toboolean(3)));
    return 0;
}


// nil GameObject:roll(Number, bool=false)
int luGameObject::roll(luClass *self, luObject &L)
{
    ref<gkGameObject>().roll(gkRadian(L.tofloat(2)), luGameObject_getTransformSpace(L, L.toboolean(3)));
    return 0;
}

// Scene GameObject:getScene()
int luGameObject::getScene(luClass *self, luObject &L)
{
    return luScene::create(L, ref<gkGameObject>().getOwner());
}




// ----------------------------------------------------------------------------
// Globals
luGlobalTableBegin(luGameObject)
luGlobalTableEnd()


// ----------------------------------------------------------------------------
// Locals
luClassTableBegin(luGameObject)
    luClassTable("getPosition",         luGameObject,getPosition,           LU_PARAM, ".")
    luClassTable("getRotation",         luGameObject,getRotation,           LU_PARAM, ".")
    luClassTable("getOrientation",      luGameObject,getOrientation,        LU_PARAM, ".")
    luClassTable("getScale",            luGameObject,getScale,              LU_PARAM, ".")
    luClassTable("getWorldPosition",    luGameObject,getWorldPosition,      LU_PARAM, ".")
    luClassTable("getWorldRotation",    luGameObject,getWorldRotation,      LU_PARAM, ".")
    luClassTable("getWorldOrientation", luGameObject,getWorldOrientation,   LU_PARAM, ".")
    luClassTable("getLinearVelocity",   luGameObject,getLinearVelocity,     LU_PARAM, ".")
    luClassTable("getAngularVelocity",  luGameObject,getAngularVelocity,    LU_PARAM, ".")
    luClassTable("setLinearVelocity",   luGameObject,setLinearVelocity,     LU_PARAM, ".")
    luClassTable("setAngularVelocity",  luGameObject,setAngularVelocity,    LU_PARAM, ".")
    luClassTable("setPosition",         luGameObject,setPosition,           LU_PARAM, "..")
    luClassTable("setRotation",         luGameObject,setRotation,           LU_PARAM, "..")
    luClassTable("setOrientation",      luGameObject,setOrientation,        LU_PARAM, "..")
    luClassTable("getType",             luGameObject,_getType,              LU_PARAM, ".")
    luClassTable("rotate",              luGameObject,rotate,                LU_PARAM, "..|b")
    luClassTable("translate",           luGameObject,translate,             LU_PARAM, "..|b")
    luClassTable("scale",               luGameObject,scale,                 LU_PARAM, "..")
    luClassTable("yaw",                 luGameObject,yaw,                   LU_PARAM, ".n|b")
    luClassTable("pitch",               luGameObject,pitch,                 LU_PARAM, ".n|b")
    luClassTable("roll",                luGameObject,roll,                  LU_PARAM, ".n|b")
    luClassTable("getScene",            luGameObject,getScene,              LU_PARAM, ".")
luClassTableEnd()

luClassImpl("GameObject", luGameObject, &luLoadable::Type);


// ----------------------------------------------------------------------------
luCamera::luCamera(gkObject *ob) : luGameObject(ob)
{
}

luCamera::~luCamera()
{
}

// Globals
luGlobalTableBegin(luCamera)
luGlobalTableEnd()

// Locals
luClassTableBegin(luCamera)
// TODO
luClassTableEnd()

luClassImpl("Camera", luCamera, &luGameObject::Type);



// ----------------------------------------------------------------------------
luEntity::luEntity(gkObject *ob) : luGameObject(ob)
{
}

luEntity::~luEntity()
{
}

// nil Entity:playAction(String, Number=0)
int luEntity::playAction(luClass *self, luObject &L)
{
    int blend = 0;
    if (L.isNumber(3)) blend = L.toint(3);

    ref<gkEntity>().playAction(L.tostring(2), blend);
    return 0;
}

// Globals
luGlobalTableBegin(luEntity)
luGlobalTableEnd()

// Locals
luClassTableBegin(luEntity)
    luClassTable("playAction",  luEntity,playAction,    LU_PARAM, ".s|i")
luClassTableEnd()

luClassImpl("Entity", luEntity, &luGameObject::Type);





// ----------------------------------------------------------------------------
luLight::luLight(gkObject *ob) : luGameObject(ob)
{
}

luLight::~luLight()
{
}

// Globals
luGlobalTableBegin(luLight)
luGlobalTableEnd()

// Locals
luClassTableBegin(luLight)
// TODO
luClassTableEnd()

luClassImpl("Light", luLight, &luGameObject::Type);
