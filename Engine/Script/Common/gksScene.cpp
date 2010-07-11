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
-- Generic Script API Binding                                                --
-------------------------------------------------------------------------------
*/
#include "gkScene.h"
#include "gkGameObject.h"
#include "gksGameObject.h"
#include "gksScene.h"
#include "gkEngine.h"


// ----------------------------------------------------------------------------
bool gksScene::hasObject(const char *name)
{
    if (m_object)
        return cast<gkScene>()->hasObject(name);
    return false;
}

// ----------------------------------------------------------------------------
gksPointer<gksGameObject> gksScene::getObject(const char *name)
{
    if (m_object)
    {
        gkGameObject *gobj = cast<gkScene>()->getObject(name);
        if (gobj)
            return gksPointer<gksGameObject>(new gksGameObject(gobj));
    }
    return gksPointer<gksGameObject>();
}



// ----------------------------------------------------------------------------
gksPointer<gksScene> getCurrentScene(void)
{
    return gksPointer<gksScene>(new gksScene(gkEngine::getSingleton().getActiveScene()));
}
