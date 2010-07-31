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