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
#ifndef _gkV8Manager_h_
#define _gkV8Manager_h_


#include "gkV8Utils.h"
#include "OgreSingleton.h"

class gkV8Script;


class gkV8Manager : public Ogre::Singleton<gkV8Manager>
{
public:

    typedef utList<gkV8Script*> ScriptList;
    typedef utList<v8Module> Modules;

private:
    v8::HandleScope __scope__;
    v8Globals       m_globals;
    v8Context       m_context;
    ScriptList      m_scripts;
    Modules         m_modules;

public:

    gkV8Manager();
    ~gkV8Manager();

    void initializeBindings(v8BindModule *extras);

    v8Context& getContext(void) {return m_context;}
    // Create new script from text buffer
    gkV8Script* create(const gkString& name, const gkString &text);
    gkV8Script* create(const gkString& file);


    static gkV8Manager& getSingleton();
    static gkV8Manager* getSingletonPtr();

};


#endif//_gkV8Manager_h_
