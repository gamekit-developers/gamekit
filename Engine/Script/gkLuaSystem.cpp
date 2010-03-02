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
#include "gkString.h"
#include "gkLuaSystem.h"
#include "gkTextManager.h"


// System builtin module type
struct gkSystem_Builtin {
    const char *    name;
    bool            imported;
    void (*builtin)(gkLuaState*);
};


// System builtin module definition
gkSystem_Builtin gkSystem_BuiltinModules[] = {
    {0, 0, false}
};


// Module lookup
gkSystem_Builtin gkSystem_FinmdModule(const char *name, gkSystem_Builtin *modules)
{
    while (modules && modules->name) {
        if (utCharEq(modules->name, name))
            return *modules;
        ++modules;
    }
    gkSystem_Builtin b = {0, 0, false};
    return b;
}


// Module/script Impoter
static int gkSystem_import(gkLuaState *L)
{
    char *import = 0;

    if (!gkLuaBind::parse(L, "s", &import))
        return gkLuaBind::seterror(L, "invalid arguments");

    gkSystem_Builtin v = gkSystem_FinmdModule(import, gkSystem_BuiltinModules);
    if (v.builtin && !v.imported) {

        // execute builtin reg 
        v.builtin(L);
        v.imported = true;
        return 0;

    } else if (v.name != 0) {
        // is a todo
        return 0;

    } else if (v.imported) 
        return 0;

    if (gkTextManager::getSingleton().hasFile(import))
    {
        // import internal text file
        gkTextFile *fp = gkTextManager::getSingleton().getFile(import);

        if (!gkLuaBind::load(L, import, fp->getText().c_str(), fp->getSize()))
            return -1;
        return 0;
    }


    // external source
    if (!gkLuaBind::load(L, import))
        return -1;

    return 0;
}



// System methods
static gkLuaMethodDef gkSystem_Methods[] = {
    {"import",      gkSystem_import},
    {0, 0}
};


// System type definition
static gkLuaTypeDef gkSystem_Type = {
    "System",           // name
    0,                  // constructor
    0,                  // destructor
    0,                  // get
    0,                  // set
    gkSystem_Methods    // methods
};


// Lua System module
int gkLuaSystem_initialize(gkLuaState *L)
{
    int top = gkLuaBind::beginBinding(L);

    // System table
    gkLuaBind::table(L, gkSystem_Type.name);
    gkLuaBind::set(L, gkSystem_Type.methods);

    gkLuaBind::table(L);

    gkLuaBind::endBinding(L, top);
    return 0;
}


// System type access
gkLuaTypeDef* gkLuaSystem_getType(void)
{
    return &gkSystem_Type;
}
