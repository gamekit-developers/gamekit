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
#include "luUtils.h"

static gkLuaScript *gsCurrent = 0;


gkLuaScript::gkLuaScript(gkLuaManager *parent, const gkString &name, const gkString &text)
    :       m_script(0),m_main(0), m_name(name), m_text(text), m_error(false), m_owner(parent)
{
}

gkLuaScript::~gkLuaScript()
{
    ltState *L = m_owner->getLua();

    luCallbackIterator it(m_functions);
    while (it.hasMoreElements())
    {
        luRefObject *ref = it.getNext().second;
        ref->unref(L);
        delete ref;
    }

    if (m_main != 0)
    {
        m_main->unref(L);
        delete m_main;
    }
    if (m_script != 0)
    {
        m_script->unref(L);
        delete m_script;
    }
}


bool gkLuaScript::handleError(lua_State *L)
{
    printf("%s\n", lua_tostring(L, -1));
    lua_pop(L, 1);
    m_error = true;
    gsCurrent = 0;
    return false;
}

bool gkLuaScript::execute(void)
{
    GK_ASSERT(m_owner);
    gsCurrent = this;

    if (m_error) 
        return false;

    ltState *L = m_owner->getLua();
    {
        LUA_SCOPE_LOCK;

        if (m_script == 0)
        {
            // compile
            if (luaL_loadbuffer(L, m_text.c_str(), m_text.size()-1, m_name.c_str()) != 0)
                return handleError(L);

            bool has_main = true;
            m_script = new luRefObject();
            m_script->addRef(L, lua_gettop(L));
        } 

        m_script->push(L);
        if (lua_pcall(L, 0, 0, 0) != 0)
            return handleError(L);

        lua_getglobal(L, "Main");
        if (lua_isfunction(L, -1))
        {
            if (lua_pcall(L, 0, 0, 0) != 0)
                return handleError(L);
        }
        else lua_pop(L, 1);

        lua_gc(L, LUA_GCCOLLECT, 0);
        gsCurrent = 0;
    }
    return true;
}


void gkLuaScript::update(gkScalar tickRate)
{
    if (m_error || m_script == 0 || m_functions.empty())
        return;


    ltState *L = m_owner->getLua();
    {
        LUA_SCOPE_LOCK;
        m_script->push(L);

        luCallbackIterator it(m_functions);
        while (it.hasMoreElements())
        {
            LUA_SCOPE_LOCK;
            it.getNext().second->push(L);
            lua_pushnumber(L, tickRate);
            if (lua_pcall(L, 1, 0, 0) != 0)
            {
                printf("%s\n", lua_tostring(L, -1));
                lua_pop(L, 1);
                m_error = true;
                break;
            }
        }
        lua_gc(L, LUA_GCSTEP, 0);
    }
}



gkLuaManager::gkLuaManager()
    :       L(0)
{
    L = lua_open();
    {
        LUA_SCOPE_LOCK;
        luaL_openlibs(L);
        luSystem_Open(L);
    }
}

gkLuaManager::~gkLuaManager()
{
    destroyAll();
    if (L) lua_close(L);
}

void gkLuaManager::addListener(int type, ltState *L, int index)
{
    if (gsCurrent != 0)
    {
        if (!gsCurrent->hasFunction(type))
        {
            luRefObject *ref = new luRefObject();
            ref->addRef(L, index);
            gsCurrent->addFunction(type, ref);

            if (m_updateHooks.find(gsCurrent) == 0)
                m_updateHooks.push_back(gsCurrent);
        }
    }
}

void gkLuaManager::update(gkScalar tick)
{
    if (!m_updateHooks.empty())
    {
        utListIterator<ScriptList> it(m_updateHooks);
        while (it.hasMoreElements())
            it.getNext()->update(tick);
    }
}

gkLuaScript *gkLuaManager::getScript(const gkString &name)
{
    UTsize pos;
    if ((pos = m_scripts.find(name)) == GK_NPOS)
        return 0;
    return m_scripts.at(pos);
}

gkLuaScript *gkLuaManager::create(const gkString &name, const gkString &text)
{
    UTsize pos;
    if ((pos = m_scripts.find(name)) != GK_NPOS)
        return 0;

    gkLuaScript *ob = new gkLuaScript(this, name, text);
    m_scripts.insert(name, ob);
    return ob;
}

gkLuaScript *gkLuaManager::create(const gkString &name)
{
    UTsize pos;
    if ((pos = m_scripts.find(name)) != GK_NPOS)
        return 0;

    gkTextFile *intern = gkTextManager::getSingleton().getFile(name);

    if (intern == 0)
    {
        printf("Invalid internal text file %s\n", name.c_str());
        return 0;
    }

    gkLuaScript *ob = new gkLuaScript(this, name, intern->getText());
    m_scripts.insert(name, ob);
    return ob;
}

void gkLuaManager::destroy(const gkString &name)
{
    UTsize pos;
    if ((pos = m_scripts.find(name)) != GK_NPOS)
    {
        gkLuaScript *ob = m_scripts.at(pos);

        ScriptList::Pointer p;
        if ((p = m_updateHooks.find(ob)) != 0)
            m_updateHooks.erase(p);

        m_scripts.remove(name);
        delete ob;
    }
}

void gkLuaManager::destroy(gkLuaScript *ob)
{
    GK_ASSERT(ob);
    destroy(ob->getName());
}

void gkLuaManager::destroyAll(void)
{
    utHashTableIterator<ScriptMap> iter(m_scripts);
    while (iter.hasMoreElements())
    {
        gkLuaScript *ob = iter.peekNextValue();
        delete ob;
        iter.next();
    }

    m_updateHooks.clear();
    m_scripts.clear();
}


bool gkLuaManager::hasScript(const gkString &name)
{
    return m_scripts.find(name) != GK_NPOS;
}

GK_IMPLEMENT_SINGLETON(gkLuaManager);
