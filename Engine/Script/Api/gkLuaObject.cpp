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
#include "gkObject.h"
#include "gkLuaObject.h"
#include "gkLuaGameLogic.h"
#include "gkEngine.h"


// ----------------------------------------------------------------------------
void Object::load(void)
{
    if (m_object)
        gkEngine::getSingleton().addLoadable(m_object, LQ_LOAD);
}

// ----------------------------------------------------------------------------
void Object::unload(void)
{
    if (m_object)
        gkEngine::getSingleton().addLoadable(m_object, LQ_UNLOAD);
}


// ----------------------------------------------------------------------------
void Object::reload(void)
{
    if (m_object)
        gkEngine::getSingleton().addLoadable(m_object, LQ_RELOAD);
}

// ----------------------------------------------------------------------------
const char *Object::getName(void)
{
    static char buf[72] = {0};
    if (m_object)
        sprintf(buf, "%s", m_object->getName().c_str());
    return buf;
}
