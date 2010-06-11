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
#ifndef _gkLuaScene_h_
#define _gkLuaScene_h_


#include "gkLuaObject.h"



// ----------------------------------------------------------------------------
class Scene : public Object
{
#ifndef SWIG

public:
    Scene(gkObject *oth) : Object(oth) {}
#endif

public:
    Scene() : Object() {}

    virtual ~Scene() {}

    bool            hasObject(const char *name);
    GameObjectPtr   getObject(const char *name);
};

extern ScenePtr getCurrentScene(void);

#endif//_gkLuaScene_h_