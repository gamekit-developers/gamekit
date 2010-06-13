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
#include "gkLuaScene.h"
#include "gkScene.h"
#include "gkLuaGameObject.h"
#include "gkGameObject.h"
#include "gkDebugger.h"
#include "gkDebugScreen.h"
#include "gkLuaDebugger.h"
#include "gkEngine.h"



gkLuaDebugger::gkLuaDebugger(const ScenePtr &sc) 
    :   m_debugger(0)
{
    if (!sc.isNull())
    {
        gkScene *scene = sc->cast<gkScene>();
        if (scene)
            m_debugger = scene->getDebugger();

    }
}


void gkLuaDebugger::drawLine(const Vector3& from, const Vector3& to, const Vector3& color)
{
    if (m_debugger)
        m_debugger->drawLine(from, to, color);
}


void gkLuaDebugger::drawObjectAxis(const GameObjectPtr &ptr, float size)
{
    if (m_debugger && !ptr.isNull())
    {
        gkGameObject *ob = ptr->cast<gkGameObject>();
        if (ob)
        {
            const gkVector3     &axis   = ob->getWorldPosition();
            const gkQuaternion  &ori    = ob->getOrientation();

            gkVector3 x = (ori * gkVector3(size, 0, 0));
            gkVector3 y = (ori * gkVector3(0, size, 0));
            gkVector3 z = (ori * gkVector3(0, 0, size));


            m_debugger->drawLine(axis, axis + x, gkVector3(1,0,0));
            m_debugger->drawLine(axis, axis + y, gkVector3(0,1,0));
            m_debugger->drawLine(axis, axis + z, gkVector3(0,0,1));
        }
    }
}

void gkLuaDebugger::print(const char *str)
{
    gkDebugScreen::printTo(str);
}

void gkLuaDebugger::clear(void)
{
    if (m_debugger)
        m_debugger->clear();
}
