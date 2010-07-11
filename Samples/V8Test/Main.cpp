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
-- Runtime test using the V8 JavaScript Engine                               -- 
-------------------------------------------------------------------------------
*/
#include "OgreKit.h"
#include "Script/V8/gkV8Manager.h"
#include "Script/V8/gkV8Script.h"


extern v8::Persistent<v8::ObjectTemplate> OgreKit_OpenV8(v8::Persistent<v8::ObjectTemplate>&);

static v8BindModule modules[] = {
    OgreKit_OpenV8,
    0
};

// ----------------------------------------------------------------------------
int main(int argc, char **argv)
{
    TestMemory;

    gkV8Manager mgr;
    mgr.initializeBindings(modules);


    gkV8Script *script = mgr.create(gkUtils::getFile("OgreKit.js"));
    if (script) 
        script->execute();
    return 0;
}
