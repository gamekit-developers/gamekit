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
#include "luLoadable.h"
#include "gkObject.h"
#include "gkEngine.h"


luLoadable::luLoadable(gkObject *ob) : m_object(ob)
{
    UT_ASSERT(m_object);
}

luLoadable::~luLoadable()
{
}

// String Object:getName()
int luLoadable::getName(luClass *self, luObject &L)
{
    return L.push(m_object->getName().c_str());
}

// bool Object:isLoaded()
int luLoadable::isLoaded(luClass *self, luObject &L)
{
    return L.push(m_object->isLoaded());
}

// nil Object:load()
int luLoadable::load(luClass *self, luObject &L)
{
    if (!m_object->isLoaded())
        gkEngine::getSingleton().addLoadable(m_object, LQ_LOAD);
    return 0;
}

// nil Object:unload()
int luLoadable::unload(luClass *self, luObject &L)
{
    if (m_object->isLoaded())
        gkEngine::getSingleton().addLoadable(m_object, LQ_UNLOAD);
    return 0;
}

// nil Object:reload()
int luLoadable::reload(luClass *self, luObject &L)
{
    if (m_object->isLoaded())
        gkEngine::getSingleton().addLoadable(m_object, LQ_RELOAD);
    return 0;
}


// ----------------------------------------------------------------------------
// Globals
luGlobalTableBegin(luLoadable)
luGlobalTableEnd()


// ----------------------------------------------------------------------------
// Locals
luClassTableBegin(luLoadable)
luClassTable("getName",     luLoadable, getName,    LU_NOPARAM,   ".")
luClassTable("isLoaded",    luLoadable, isLoaded,   LU_NOPARAM,   ".")
luClassTable("load",        luLoadable, load,       LU_NOPARAM,   ".")
luClassTable("unload",      luLoadable, unload,     LU_NOPARAM,   ".")
luClassTable("reload",      luLoadable, reload,     LU_NOPARAM,   ".")
luClassTableEnd()

luClassImpl("Object", luLoadable, 0);
