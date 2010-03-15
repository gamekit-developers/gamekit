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
#ifndef _luGameObjects_h_
#define _luGameObjects_h_

#include "luUtils.h"
#include "luLoadable.h"

class gkEntity;
class gkCamera;
class gkLight;
class gkGameObject;


class luGameObject : public luLoadable
{
    luClassHeader;
public:

    luGameObject(gkObject *ob);
    ~luGameObject();

    static int              create(luObject &L, gkGameObject *ob);
    static luGameObject&    getArg(luObject &L, int v) { return L.toclassRefT<luGameObject>(v); }
    static void             bind(luBinder &L);

    // Math.Vector3 GameObject:getPosition()
    int getPosition(luClass *self, luObject &L);

    // Math.Vector3 GameObject:getRotation()
    int getRotation(luClass *self, luObject &L);

    // Math.Quaternion GameObject:getOrientation()
    int getOrientation(luClass *self, luObject &L);

    // Math.Vector3 GameObject:getScale()
    int getScale(luClass *self, luObject &L);

    // Math.Vector3 GameObject:getWorldPosition()
    int getWorldPosition(luClass *self, luObject &L);

    // Math.Vector3 GameObject:getWorldRotation()
    int getWorldRotation(luClass *self, luObject &L);

    // Math.Quaternion GameObject:getWorldOrientation()
    int getWorldOrientation(luClass *self, luObject &L);

    // Math.Vector3 GameObject:getLinearVelocity()
    int getLinearVelocity(luClass *self, luObject &L);

    // Math.Vector3 GameObject:getAngularVelocity()
    int getAngularVelocity(luClass *self, luObject &L);

    // nil GameObject:setLinearVelocity(Math.Vector3)
    int setLinearVelocity(luClass *self, luObject &L);

    // nil GameObject:setAngularVelocity(Math.Vector3)
    int setAngularVelocity(luClass *self, luObject &L);

    // nil GameObject:setPosition(Math.Vector3)
    int setPosition(luClass *self, luObject &L);

    // nil GameObject:setRotation(Math.Vector3)
    int setRotation(luClass *self, luObject &L);

    // nil GameObject:setOrientation(Math.Quaternion)
    int setOrientation(luClass *self, luObject &L);

    // Enum GameObject:getType()
    int _getType(luClass *self, luObject &L);

    // nil GameObject:rotate(Math.Vector3, bool=false)
    // nil GameObject:rotate(Math.Quaternion, bool=false)
    int rotate(luClass *self, luObject &L);

    // nil GameObject:translate(Math.Vector3, bool=false)
    int translate(luClass *self, luObject &L);

    // nil GameObject:scale(Math.Vector3)
    int scale(luClass *self, luObject &L);

    // nil GameObject:yaw(Number, bool=false)
    int yaw(luClass *self, luObject &L);

    // nil GameObject:pitch(Number, bool=false)
    int pitch(luClass *self, luObject &L);

    // nil GameObject:roll(Number, bool=false)
    int roll(luClass *self, luObject &L);

    // Scene GameObject:getScene()
    int getScene(luClass *self, luObject &L);
};

class luCamera : public luGameObject
{
    luClassHeader;
public:

    luCamera(gkObject *ob);
    ~luCamera();

    static luCamera&    getArg(luObject &L, int v) { return L.toclassRefT<luCamera>(v); }

};

class luEntity : public luGameObject
{
    luClassHeader;
public:

    luEntity(gkObject *ob);
    ~luEntity();

    static luEntity&    getArg(luObject &L, int v) { return L.toclassRefT<luEntity>(v); }

    // nil Entity:playAction(String, Number=0)
    int playAction(luClass *self, luObject &L);
};


class luLight : public luGameObject
{
    luClassHeader;
public:

    luLight(gkObject *ob);
    ~luLight();

    static luLight&    getArg(luObject &L, int v) { return L.toclassRefT<luLight>(v); }
};


#endif//_luGameObjects_h_
