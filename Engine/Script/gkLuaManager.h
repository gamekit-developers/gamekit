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
#ifndef _gkLuaManager_h_
#define _gkLuaManager_h_

#include "gkCommon.h"
#include "gkMathUtils.h"
#include "gkString.h"
#include "OgreSingleton.h"

// Expression parsing (expression nodes, not used right now)
class gkScriptExpression
{
public:
    enum ExpressionResult
    {
        EXPR_ERROR = -1,
        EXPR_FALSE,
        EXPR_TRUE
    };

    gkScriptExpression() {}
    virtual ~gkScriptExpression() {}

    virtual size_t getHandle() = 0;
    virtual bool isOk() = 0;
    virtual int run() = 0;
    virtual void addConstant(const gkString &name, int v) = 0;
    virtual void addConstant(const gkString &name, gkScalar v) = 0;
    virtual void addConstant(const gkString &name, bool v) = 0;
    virtual void addConstant(const gkString &name, gkString v) = 0;
};

struct lua_State;


// Lua script representation  
class gkLuaScript
{
protected:
    int m_script;
    const gkString m_name, m_text;
    bool m_error;
    class gkLuaManager* m_owner;

public:
    gkLuaScript(gkLuaManager *parent, const gkString& name, const gkString& text);
    ~gkLuaScript();

    GK_INLINE const gkString& getName(void) {return m_name;}
    GK_INLINE bool compiled(void)           {return !m_error;}


    // compile & run the script 
    bool execute(void);
};


// Lua interface to OgreKit
class gkLuaManager : public Ogre::Singleton<gkLuaManager>
{
public:
    typedef utHashTable<gkHashedString, gkLuaScript*> ScriptMap;

private:
    lua_State *m_vm;
    ScriptMap m_scripts;

public:
    gkLuaManager();
    virtual ~gkLuaManager();

    // access to the lua virtual machine
    GK_INLINE lua_State *getLua(void) {return m_vm;}

    gkLuaScript* getScript(const gkString& name);

    // Create new script from text buffer
    gkLuaScript* create(const gkString& name, const gkString &text);

    // create from internal text file manager
    gkLuaScript* create(const gkString& name);

    // Destroys named file
    void destroy(const gkString& name);

    // Destroys file pointer
    void destroy(gkLuaScript *ob);

    // Destroys all internal files
    void destroyAll(void);

    // Test for file existance
    bool hasScript(const gkString& name);


    static gkLuaManager& getSingleton();
    static gkLuaManager* getSingletonPtr();
};

#endif//_gkLuaManager_h_
