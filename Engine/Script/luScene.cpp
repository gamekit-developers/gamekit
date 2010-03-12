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
#include "luScene.h"
#include "luLoadable.h"
#include "luGameObjects.h"
#include "gkScene.h"
#include "gkGameObject.h"
#include "gkCamera.h"
#include "gkEntity.h"
#include "gkLight.h"
#include "gkSkeleton.h"


luScene::luScene(gkObject *ob) : luLoadable(ob)
{
}


luScene::~luScene()
{
}


static int luScene_hasObject(luObject &L)
{
    gkScene &sc = L.getValueClassT<luScene>(1)->ref<gkScene>();
    return L.push(sc.hasObject(L.getValueString(2)));
}

static int luScene_getObject(luObject &L)
{
    gkScene &sc = L.getValueClassT<luScene>(1)->ref<gkScene>();
    luString look = L.getValueString(2);
    if (!sc.hasObject(look))
    {
        return L.push();
    }

    gkGameObject *ob = sc.getObject(look);

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

static int luScene_createEmpty(luObject &L)
{
    gkScene &sc = L.getValueClassT<luScene>(1)->ref<gkScene>();
    return 0;
}

static int luScene_createCamera(luObject &L)
{
    gkScene &sc = L.getValueClassT<luScene>(1)->ref<gkScene>();
    return 0;
}

static int luScene_createMesh(luObject &L)
{
    gkScene &sc = L.getValueClassT<luScene>(1)->ref<gkScene>();
    return 0;
}

static int luScene_createLamp(luObject &L)
{
    gkScene &sc = L.getValueClassT<luScene>(1)->ref<gkScene>();
    return 0;
}


luMethodDef luScene::Methods[] = {
    {"hasObject",       luScene_hasObject,      LU_PARAM, ".s"},
    {"getObject",       luScene_getObject,      LU_PARAM, ".s"},
    {"createEmpty",     luScene_createEmpty,    LU_PARAM, ".s"},
    {"createCamera",    luScene_createCamera,   LU_PARAM, ".s"},
    {"createMesh",      luScene_createMesh,     LU_PARAM, ".ss"},
    {"createLamp",      luScene_createLamp,     LU_PARAM, ".s"},
    {0,0,0,0}
};

luTypeDef luScene::Type = {"Scene", &luLoadable::Type, Methods};
