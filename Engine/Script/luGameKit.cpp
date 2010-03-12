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
#include "gkLuaManager.h"
#include "gkInput.h"
#include "gkEngine.h"
#include "gkScene.h"
#include "gkGameObject.h"
#include "gkCamera.h"
#include "gkEntity.h"
#include "gkLight.h"
#include "gkSkeleton.h"

#include "luUtils.h"
#include "luInput.h"
#include "luScene.h"
#include "luGameObjects.h"


enum ListenerHook
{
    LH_UPDATE,
    // TODO other event hooks
};


static int GameKit_addListener(luObject &ob)
{
    ltState *L = ob;

    if (ob.getValueInt(1) == LH_UPDATE)
    {
        if (lua_isfunction(L, 2))
            gkLuaManager::getSingleton().addListener(ob.getValueInt(1), L, 2);
    }
    return 0;
}


static int GameKit_getActiveScene(luObject &ob)
{
    luScene *scene = new (&luScene::Type, ob) luScene(gkEngine::getSingleton().getActiveScene());
    return 1;
}

luMethodDef GameKit_Methods[] = 
{
    {"addListener",     GameKit_addListener,        LU_PARAM, "eF"},
    {"getActiveScene",  GameKit_getActiveScene,     LU_NOPARAM, ""},
    {0,0,0,0}
};





void luGameKit_Open(ltState *L)
{
    // keys are global
    luBinder lua(L);

    luKeyboard::bind(lua);
    luMouse::bind(lua);

    lua.beginNamespace("GameKit");

    // constants
    lua.addConstant("UPDATE", LH_UPDATE);
    luGameObject::bind(lua);

    lua.addMethods(GameKit_Methods);

    lua.addType(&luKeyboard::Type);
    lua.addType(&luMouse::Type);
    lua.addType(&luLoadable::Type);
    lua.addType(&luScene::Type);
    lua.addType(&luGameObject::Type);
    lua.addType(&luCamera::Type);
    lua.addType(&luEntity::Type);
    lua.addType(&luLight::Type);

    lua.endNamespace();
}
