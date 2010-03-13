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


luLoadable::luLoadable(gkObject *ob) : m_object(ob)
{
}

luLoadable::~luLoadable()
{
}


static int luLoadable_getName(luObject &L)
{
    gkObject &ob = L.getValueClassT<luLoadable>(1)->ref<gkObject>();
    return L.push(ob.getName().c_str());
}

static int luLoadable_isLoaded(luObject &L)
{
    gkObject &ob = L.getValueClassT<luLoadable>(1)->ref<gkObject>();
    return L.push(ob.isLoaded());
}

static int luLoadable_load(luObject &L)
{
    gkObject &ob = L.getValueClassT<luLoadable>(1)->ref<gkObject>();

    luClass *cls = L.getValueClass(1);
    if (!cls->isTypeOf(cls->getType(), "Scene"))
        ob.load();
    return 0;
}

static int luLoadable_unload(luObject &L)
{
    gkObject &ob = L.getValueClassT<luLoadable>(1)->ref<gkObject>();

    luClass *cls = L.getValueClass(1);
    if (!cls->isTypeOf(cls->getType(), "Scene"))
        ob.unload();
    return 0;
}

static int luLoadable_reload(luObject &L)
{
    gkObject &ob = L.getValueClassT<luLoadable>(1)->ref<gkObject>();

    // scenes will require a load / unload query in gkEngine
    luClass *cls = L.getValueClass(1);
    if (!cls->isTypeOf(cls->getType(), "Scene"))
        ob.reload();
    return 0;
}



luMethodDef luLoadable::Methods[] = {

    {"getName",     luLoadable_getName,     LU_NOPARAM, ""},
    {"isLoaded",    luLoadable_isLoaded,    LU_NOPARAM, ""},
    {"load",        luLoadable_load,        LU_NOPARAM, ""},
    {"unload",      luLoadable_unload,      LU_NOPARAM, ""},
    {"reload",      luLoadable_reload,      LU_NOPARAM, ""},
    {0,0,0,0}
};

luTypeDef luLoadable::Type = {"Object", 0, Methods};
