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

// bool Scene:hasObject(String)
int luScene::hasObject(luClass *self, luObject &L)
{
    return L.push(ref<gkScene>().hasObject(L.tostring(1)));
}

// GameObject Scene:getObject(String)
int luScene::getObject(luClass *self, luObject &L)
{
    gkScene &sc = ref<gkScene>();

    luString look = L.tostring(2);
    if (!sc.hasObject(look))
        return 0;

    return luGameObject::create(L, sc.getObject(look));
}

// GameObject Scene:createEmpty(String)
int luScene::createEmpty(luClass *self, luObject &L)
{
    return L.push("TODO");
}

// GameObject Scene:createCamera(String)
int luScene::createCamera(luClass *self, luObject &L)
{
    return L.push("TODO");
}

// GameObject Scene:createMesh(String, String)
int luScene::createMesh(luClass *self, luObject &L)
{
    return L.push("TODO");
}


// GameObject Scene:createLamp(String)
int luScene::createLamp(luClass *self, luObject &L)
{
    return L.push("TODO");
}


int luScene::create(luObject &L, gkObject *ob)
{
    new (&Type, L) luScene(ob);
    return 1;
}



// ----------------------------------------------------------------------------
// Globals
luGlobalTableBegin(luScene)
luGlobalTableEnd()


// ----------------------------------------------------------------------------
// Locals
luClassTableBegin(luScene)
luClassTable("hasObject",       luScene, hasObject,     LU_PARAM,   ".s")
luClassTable("getObject",       luScene, getObject,     LU_PARAM,   ".s")
luClassTable("createEmpty",     luScene, createEmpty,   LU_PARAM,   ".s")
luClassTable("createCamera",    luScene, createCamera,  LU_PARAM,   ".s")
luClassTable("createMesh",      luScene, createMesh,    LU_PARAM,   ".ss")
luClassTable("createLamp",      luScene, createLamp,    LU_PARAM,   ".s")
luClassTableEnd()

luClassImpl("Scene", luScene, &luLoadable::Type);
