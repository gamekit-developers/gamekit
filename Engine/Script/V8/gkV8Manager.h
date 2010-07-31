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
