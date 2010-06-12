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
#ifndef _gkLuaGameObject_h_
#define _gkLuaGameObject_h_

#include "gkLuaObject.h"
#include "gkLuaProperty.h"



// ----------------------------------------------------------------------------
class GameObject : public Object
{
#ifndef SWIG
public:

    GameObject(gkObject *oth) : Object(oth) {}
#endif

public:

    GameObject() : Object() {}

    Vector3 getPosition(void);

    // Math.Vector3 GameObject:getRotation()
    Vector3 getRotation(void);

    // Math.Quaternion GameObject:getOrientation()
    Quaternion getOrientation(void);

    // Math.Vector3 GameObject:getScale()
    Vector3 getScale(void);

    // Math.Vector3 GameObject:getWorldPosition()
    Vector3 getWorldPosition(void);

    // Math.Vector3 GameObject:getWorldRotation()
    Vector3 getWorldRotation(void);

    // Math.Quaternion GameObject:getWorldOrientation()
    Quaternion getWorldOrientation(void);

    // Math.Vector3 GameObject:getLinearVelocity()
    Vector3 getLinearVelocity(void);

    // Math.Vector3 GameObject:getAngularVelocity()
    Vector3 getAngularVelocity(void);

    // nil GameObject:setLinearVelocity(Math.Vector3)
    void setLinearVelocity(const Vector3 &v);
    void setLinearVelocity(float x, float y, float z);

    // nil GameObject:setAngularVelocity(Math.Vector3)
    void setAngularVelocity(const Vector3 &v);
    void setAngularVelocity(float x, float y, float z);

    // nil GameObject:setPosition(Math.Vector3)
    void setPosition(const Vector3 &v);
    void setPosition(float x, float y, float z);

    // nil GameObject:setRotation(Math.Vector3)
    void setRotation(const Vector3 &v);
    void setRotation(float yaw, float pitch, float roll);

    // nil GameObject:setOrientation(Math.Quaternion)
    void setOrientation(const Quaternion &quat);
    void setOrientation(float w, float x, float y, float z);

    // GameObjectTypes GameObject:getType()
    GameObjectTypes getType(void);

    void rotate(float dx, float dy, float dz);
    void rotate(const Vector3 &v);
    void rotate(const Quaternion &v);
    void rotate(float dx, float dy, float dz, TransformSpace ts);
    void rotate(const Vector3 &v, TransformSpace ts);
    void rotate(const Quaternion &v, TransformSpace ts);


    void translate(float x, float y, float z);
    void translate(const Vector3 &v);
    void translate(float x, float y, float z, TransformSpace ts);
    void translate(const Vector3 &v, TransformSpace ts);


    // nil GameObject:scale(Math.Vector3)
    void scale(const Vector3 &v);
    void scale(float x, float y, float z);

    // nil GameObject:yaw(Number, bool=false)
    void yaw(float deg);
    void yaw(float deg, TransformSpace ts);

    // nil GameObject:pitch(Number, bool=false)
    void pitch(float deg);
    void pitch(float deg, TransformSpace ts);

    // nil GameObject:roll(Number, bool=false)
    void roll(float deg);
    void roll(float deg, TransformSpace ts);

    int getState(void);

    // Scene GameObject:getScene()
    ScenePtr getScene(void);

    // property access
    Property    __getitem__(const char *prop);
    void        __setitem__(const char *prop, bool  v);
    void        __setitem__(const char *prop, float v);
    void        __setitem__(const char *prop, const char* v);
};


#endif//_gkLuaGameObject_h_