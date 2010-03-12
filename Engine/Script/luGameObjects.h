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
public:
    static luMethodDef Methods[];
    static luTypeDef Type;

public:
    static void bind(luBinder &L);

    luGameObject(gkObject *ob);
    ~luGameObject();
    luTypeDef *getType(void) { return &Type; }
};


class luCamera : public luGameObject
{
public:
    static luMethodDef Methods[];
    static luTypeDef Type;

public:

    luCamera(gkObject *ob);
    ~luCamera();
    luTypeDef *getType(void) { return &Type; }
};

class luEntity : public luGameObject
{
public:
    static luMethodDef Methods[];
    static luTypeDef Type;

public:

    luEntity(gkObject *ob);
    ~luEntity();
    luTypeDef *getType(void) { return &Type; }
};


class luLight : public luGameObject
{
public:
    static luMethodDef Methods[];
    static luTypeDef Type;

public:

    luLight(gkObject *ob);
    ~luLight();
    luTypeDef *getType(void) { return &Type; }
};


UT_INLINE bool          LU_IsLight(luObject &L, int n)          { return L.typecheck(n, &luLight::Type); }
UT_INLINE bool          LU_IsCamera(luObject &L, int n)         { return L.typecheck(n, &luCamera::Type); }
UT_INLINE bool          LU_IsEntity(luObject &L, int n)         { return L.typecheck(n, &luEntity::Type); }
UT_INLINE bool          LU_IsGameObject(luObject &L, int n)     { return L.typecheck(n, &luGameObject::Type); }
UT_INLINE gkEntity&     LU_GeEntity(luObject &L, int n)         { return L.toclassT<luEntity>(n)->ref<gkEntity>(); }
UT_INLINE gkCamera&     LU_GeCamera(luObject &L, int n)         { return L.toclassT<luCamera>(n)->ref<gkCamera>(); }
UT_INLINE gkLight&      LU_GeLight(luObject &L, int n)          { return L.toclassT<luLight>(n)->ref<gkLight>(); }
UT_INLINE gkGameObject& LU_GetGameObject(luObject &L, int n)    { return L.toclassT<luGameObject>(n)->ref<gkGameObject>(); }



#endif//_luGameObjects_h_
