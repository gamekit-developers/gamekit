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
#ifndef _OgreKitApplication_h_
#define _OgreKitApplication_h_


#include <v8.h>
#include "OgreKit.h"

typedef v8::Persistent<v8::Function> CallbackObject;


// Application events
enum CallbackCode
{
    ON_TICK,        // function(tickRate)
    ON_LOAD,        // function()
};

// Settings
class UserDefs
{
public:
    UserDefs() 
        :   winx(800), winy(600), fullscreen(false), verbose(true), grab(true), 
            wintitle("")
    {
    }
    int winx, winy;
    bool fullscreen, verbose, grab;
    gkString wintitle;
};


// Main JavaScript application access
class OgreKitApplication 
#ifndef SWIG
    : public gkCoreApplication,
      public gkWindowSystem::Listener
#endif
{
    gkScene         *m_scene;
    gkString        m_blend;
    CallbackObject  m_update;
    CallbackObject  m_load;

    bool load(void);
    void keyReleased(const gkKeyboard& key, const gkScanCode& sc);
    void tick(gkScalar rate);

    UserDefs m_defs;

public:
    OgreKitApplication(const gkString& blend);
    virtual ~OgreKitApplication();

    void addListener(CallbackCode code, CallbackObject fp);
    UserDefs& getUserDefs(void) {return m_defs;}

    void loadBlend(const char *name);
    void start(void);
};

#endif//_OgreKitApplication_h_