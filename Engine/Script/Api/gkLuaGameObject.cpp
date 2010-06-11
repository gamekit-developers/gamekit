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
#include "gkGameObject.h"
#include "gkLuaGameObject.h"
#include "gkLuaScene.h"
#include "gkScene.h"
#include "gkLuaGameLogic.h"


// ----------------------------------------------------------------------------
Vector3 GameObject::getPosition(void)
{
    if (m_object)
        return Vector3(cast<gkGameObject>()->getPosition());
    return Vector3();
}

// ----------------------------------------------------------------------------
// Math.Vector3 GameObject:getRotation()
Vector3 GameObject::getRotation(void)
{
    if (m_object)
        return Vector3(cast<gkGameObject>()->getRotation().toVector3());
    return Vector3();
}

// ----------------------------------------------------------------------------
// Math.Quaternion GameObject:getOrientation()
Quaternion GameObject::getOrientation(void)
{
    if (m_object)
        return Quaternion(cast<gkGameObject>()->getOrientation());
    return Quaternion();
}

// Math.Vector3 GameObject:getScale()
Vector3 GameObject::getScale(void)
{
    if (m_object)
        return Vector3(cast<gkGameObject>()->getScale());
    return Vector3();
}

// Math.Vector3 GameObject:getWorldPosition()
Vector3 GameObject::getWorldPosition(void)
{
    if (m_object)
        return Vector3(cast<gkGameObject>()->getWorldPosition());
    return Vector3();
}

// Math.Vector3 GameObject:getWorldRotation()
Vector3 GameObject::getWorldRotation(void)
{
    if (m_object)
        return Vector3(cast<gkGameObject>()->getWorldRotation().toVector3());
    return Vector3();
}

// Math.Quaternion GameObject:getWorldOrientation()
Quaternion GameObject::getWorldOrientation(void)
{
    if (m_object)
        return Quaternion(cast<gkGameObject>()->getWorldOrientation());
    return Quaternion();
}


// Math.Vector3 GameObject:getLinearVelocity()
Vector3 GameObject::getLinearVelocity(void)
{
    if (m_object)
        return Vector3(cast<gkGameObject>()->getLinearVelocity());
    return Vector3();
}

// Math.Vector3 GameObject:getAngularVelocity()
Vector3 GameObject::getAngularVelocity(void)
{
    if (m_object)
        return Vector3(cast<gkGameObject>()->getAngularVelocity());
    return Vector3();
}

// nil GameObject:setLinearVelocity(Math.Vector3)
void GameObject::setLinearVelocity(const Vector3 &v)
{
    if (m_object)
        cast<gkGameObject>()->setLinearVelocity(v);
}

void GameObject::setLinearVelocity(float x, float y, float z)
{
    if (m_object)
        cast<gkGameObject>()->setLinearVelocity(gkVector3(x,y,z));
}

// nil GameObject:setAngularVelocity(Math.Vector3)
void GameObject::setAngularVelocity(const Vector3 &v)
{
    if (m_object)
        cast<gkGameObject>()->setAngularVelocity(v);
}

void GameObject::setAngularVelocity(float x, float y, float z)
{
    if (m_object)
        cast<gkGameObject>()->setAngularVelocity(gkVector3(x,y,z));
}

// nil GameObject:setPosition(Math.Vector3)
void GameObject::setPosition(const Vector3 &v)
{
    if (m_object)
        cast<gkGameObject>()->setPosition(v);
}

void GameObject::setPosition(float x, float y, float z)
{
    if (m_object)
        cast<gkGameObject>()->setPosition(gkVector3(x,y,z));
}

// nil GameObject:setRotation(Math.Vector3)
void GameObject::setRotation(const Vector3 &v)
{
    if (m_object)
        cast<gkGameObject>()->setOrientation(gkEuler(v));
}

void GameObject::setRotation(float yaw, float pitch, float roll)
{
    if (m_object)
        cast<gkGameObject>()->setOrientation(gkEuler(yaw, pitch, roll));
}

// nil GameObject:setOrientation(Math.Quaternion)
void GameObject::setOrientation(const Quaternion &quat)
{
    if (m_object)
        cast<gkGameObject>()->setOrientation(quat);
}

void GameObject::setOrientation(float w, float x, float y, float z)
{
    if (m_object)
        cast<gkGameObject>()->setOrientation(gkQuaternion(w, x, y, z));
}

// Enum GameObject:getType()
GameObjectTypes GameObject::getType(void)
{
    if (m_object)
        return (GameObjectTypes)cast<gkGameObject>()->getType();
    return OB_UNKNOWN;
}


void GameObject::rotate(float dx, float dy, float dz)
{
    if (m_object)
        cast<gkGameObject>()->rotate(gkEuler(dx, dy, dz), TRANSFORM_PARENT);
}

void GameObject::rotate(const Vector3 &v)
{
    if (m_object)
        cast<gkGameObject>()->rotate(gkEuler(v), TRANSFORM_PARENT);
}

void GameObject::rotate(const Quaternion &v)
{
    if (m_object)
        cast<gkGameObject>()->rotate(v, TRANSFORM_PARENT);
}

void GameObject::rotate(float dx, float dy, float dz, TransformSpace ts)
{
    if (m_object)
        cast<gkGameObject>()->rotate(gkEuler(dx, dy, dz), ts);
}

void GameObject::rotate(const Vector3 &v, TransformSpace ts)
{
    if (m_object)
        cast<gkGameObject>()->rotate(gkEuler(v), ts);
}

void GameObject::rotate(const Quaternion &v, TransformSpace ts)
{
    if (m_object)
        cast<gkGameObject>()->rotate(v, ts);
}


void GameObject::translate(const Vector3 &v)
{
    if (m_object)
        cast<gkGameObject>()->translate(v, TRANSFORM_PARENT);
}

void GameObject::translate(float x, float y, float z)
{
    if (m_object)
        cast<gkGameObject>()->translate(gkVector3(x,y,z), TRANSFORM_PARENT);
}

void GameObject::translate(const Vector3 &v, TransformSpace ts)
{
    if (m_object)
        cast<gkGameObject>()->translate(v, ts);
}

void GameObject::translate(float x, float y, float z, TransformSpace ts)
{
    if (m_object)
        cast<gkGameObject>()->translate(gkVector3(x,y,z), ts);
}


// nil GameObject:scale(Math.Vector3)
void GameObject::scale(const Vector3 &v)
{
    if (m_object)
        cast<gkGameObject>()->scale(v);
}

void GameObject::scale(float x, float y, float z)
{
    if (m_object)
        cast<gkGameObject>()->scale(gkVector3(x,y,z));
}

// nil GameObject:yaw(Number, bool=false)
void GameObject::yaw(float deg)
{
    if (m_object)
        cast<gkGameObject>()->yaw(gkDegree(deg), TRANSFORM_PARENT);
}

void GameObject::yaw(float deg, TransformSpace ts)
{
    if (m_object)
        cast<gkGameObject>()->yaw(gkRadian(deg), ts);
}


// nil GameObject:pitch(Number, bool=false)
void GameObject::pitch(float deg)
{
    if (m_object)
        cast<gkGameObject>()->pitch(gkRadian(deg), TRANSFORM_PARENT);
}

void GameObject::pitch(float deg, TransformSpace ts)
{
    if (m_object)
        cast<gkGameObject>()->pitch(gkRadian(deg), ts);
}

void GameObject::roll(float deg)
{
    if (m_object)
        cast<gkGameObject>()->roll(gkRadian(deg), TRANSFORM_PARENT);
}

void GameObject::roll(float deg, TransformSpace ts)
{
    if (m_object)
        cast<gkGameObject>()->roll(gkRadian(deg), ts);
}


Property GameObject::__getitem__(const char *prop)
{
    if (m_object)
    {
        gkVariable *var = cast<gkGameObject>()->getVariable(prop);
        if (var)
            return Property(var);
    }
    return Property();
}


void GameObject::__setitem__(const char *prop, bool  v)
{
    if (m_object)
    {
        gkVariable *var = cast<gkGameObject>()->getVariable(prop);
        if (var)
            (*var).setValue(v);
    }
}

void GameObject::__setitem__(const char *prop, float v)
{
    if (m_object)
    {
        gkVariable *var = cast<gkGameObject>()->getVariable(prop);
        if (var)
            (*var).setValue(v);
    }
}


void GameObject::__setitem__(const char *prop, const char* v)
{
    if (m_object)
    {
        gkVariable *var = cast<gkGameObject>()->getVariable(prop);
        if (var)
            (*var).setValue(gkString(v));
    }
}


// Scene GameObject:getScene()
ScenePtr GameObject::getScene(void)
{
    if (m_object)
        return ScenePtr(new Scene(cast<gkGameObject>()->getOwner()));
    return ScenePtr();
}

int GameObject::getState(void)
{
    if (m_object)
        return cast<gkGameObject>()->getState();
    return -1;
}

