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
#include "gkLuaManager.h"
#include "gkLuaSystem.h"
#include "gkLuaType.h"
#include "gkTextManager.h"


gkLuaScript::gkLuaScript(gkLuaManager *parent, const gkString& name, const gkString& text)
:       m_script(-1), m_name(name), m_text(text), m_error(false), m_owner(parent)
{
}

gkLuaScript::~gkLuaScript()
{
    // replace with nil
    if (m_script != -1) {
        gkLuaState* L = m_owner->getLua();
        lua_pushnil(L);
        lua_replace(L, m_script);
    }

}

bool gkLuaScript::execute(void)
{
    GK_ASSERT(m_owner);

    if (m_error)
        return false;

    gkLuaState* L = m_owner->getLua();

    if (m_script == -1) {

        // compile
        if (luaL_loadbuffer(L, m_text.c_str(), m_text.size()-1, m_name.c_str()) != 0) {
            printf("%s\n", lua_tostring(L, -1));
            lua_pop(L, 1);

            m_error = true;
            return false;
        }

        // save stack
        m_script = lua_gettop(L);
    }

    GK_ASSERT(m_script != -1);
    //gkLuaBind::stacktrace(L, "execute");

    lua_pushvalue(L, m_script);

    // do call
    if (lua_pcall(L, 0, LUA_MULTRET, 0) != 0) {
        printf("%s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
        m_error = true;
        return false;
    }
    return true;
}



gkLuaManager::gkLuaManager()
:       m_vm(0)
{
    m_vm = lua_open();
    luaL_openlibs(m_vm);
    gkLuaSystem_initialize(m_vm);
}



gkLuaManager::~gkLuaManager()
{
    destroyAll();

    if (m_vm)
        lua_close(m_vm);
}


gkLuaScript* gkLuaManager::getScript(const gkString& name)
{
    UTsize pos;
    if ((pos = m_scripts.find(name)) == GK_NPOS)
        return 0;
    return m_scripts.at(pos);
}

gkLuaScript* gkLuaManager::create(const gkString& name, const gkString &text)
{
    UTsize pos;
    if ((pos = m_scripts.find(name)) != GK_NPOS)
        return 0;

    gkLuaScript *ob = new gkLuaScript(this, name, text);
    m_scripts.insert(name, ob);
    return ob;
}

gkLuaScript* gkLuaManager::create(const gkString& name)
{
    UTsize pos;
    if ((pos = m_scripts.find(name)) != GK_NPOS)
        return 0;

    gkTextFile *intern = gkTextManager::getSingleton().getFile(name);

    if (intern == 0) {
        printf("Invalid internal text file %s\n", name.c_str());
        return 0;
    }

    gkLuaScript *ob = new gkLuaScript(this, name, intern->getText());
    m_scripts.insert(name, ob);
    return ob;
}

void gkLuaManager::destroy(const gkString& name)
{
    UTsize pos;
    if ((pos = m_scripts.find(name)) != GK_NPOS) {
        gkLuaScript *ob = m_scripts.at(pos);
        m_scripts.remove(name);
        delete ob;
    }
}

void gkLuaManager::destroy(gkLuaScript *ob)
{
    GK_ASSERT(ob);

    gkString name = ob->getName();
    UTsize pos;
    if ((pos = m_scripts.find(name)) != GK_NPOS) {
        gkLuaScript *ob = m_scripts.at(pos);
        m_scripts.remove(name);
        delete ob;
    }
}


void gkLuaManager::destroyAll(void)
{
    utHashTableIterator<ScriptMap> iter(m_scripts);
    while (iter.hasMoreElements()) {
        gkLuaScript *ob = iter.peekNextValue();
        delete ob;
        iter.next();

    }

    // clear stack
    lua_pop(m_vm, lua_gettop(m_vm));
    m_scripts.clear();
}



bool gkLuaManager::hasScript(const gkString& name)
{
    return m_scripts.find(name) != GK_NPOS;
}



GK_IMPLEMENT_SINGLETON(gkLuaManager);
