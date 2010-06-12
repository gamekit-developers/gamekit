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
#include "gkScene.h"
#include "gkGameObject.h"
#include "gkLuaGameObject.h"
#include "gkLuaScene.h"
#include "gkEngine.h"


// ----------------------------------------------------------------------------
bool Scene::hasObject(const char *name)
{
    if (m_object)
        return cast<gkScene>()->hasObject(name);
    return false;
}

// ----------------------------------------------------------------------------
GameObjectPtr Scene::getObject(const char *name)
{
    if (m_object)
        return GameObjectPtr(new GameObject(cast<gkScene>()->getObject(name)));
    return GameObjectPtr();
}



// ----------------------------------------------------------------------------
ScenePtr getCurrentScene(void)
{
    return ScenePtr(new Scene(gkEngine::getSingleton().getActiveScene()));
}
