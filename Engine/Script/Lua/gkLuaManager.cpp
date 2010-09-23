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
#include "gkLuaScript.h"
#include "gkLuaUtils.h"
#include "gkUtils.h"
#include "gkTextManager.h"
#include "gkTextFile.h"


extern "C" int _OgreKitLua_install(lua_State* L);



gkLuaManager::gkLuaManager()
	:       L(0)
{
	L = lua_open();
	luaL_openlibs(L);

	_OgreKitLua_install(L);
}


gkLuaManager::~gkLuaManager()
{
	destroyAll();
	if (L) lua_close(L);
}


void gkLuaManager::decompile(void)
{
	utHashTableIterator<ScriptMap> iter(m_scripts);
	while (iter.hasMoreElements())
		iter.getNext().second->decompile();
}



gkLuaScript* gkLuaManager::getScript(const gkString& name)
{
	UTsize pos;
	if ((pos = m_scripts.find(name)) == GK_NPOS)
		return 0;
	return m_scripts.at(pos);
}



gkLuaScript* gkLuaManager::create(const gkString& name, const gkString& text)
{
	UTsize pos;
	if ((pos = m_scripts.find(name)) != GK_NPOS)
		return 0;

	gkLuaScript* ob = new gkLuaScript(this, name, text);
	m_scripts.insert(name, ob);
	return ob;
}


gkLuaScript* gkLuaManager::create(const gkString& name)
{
	UTsize pos;
	if ((pos = m_scripts.find(name)) != GK_NPOS)
		return 0;

	gkTextFile* intern = (gkTextFile*)gkTextManager::getSingleton().getByName(name);

	if (intern == 0)
	{
		printf("Invalid internal text file %s\n", name.c_str());
		return 0;
	}

	gkLuaScript* ob = new gkLuaScript(this, name, intern->getText());
	m_scripts.insert(name, ob);
	return ob;
}


void gkLuaManager::destroy(const gkString& name)
{
	UTsize pos;
	if ((pos = m_scripts.find(name)) != GK_NPOS)
	{
		gkLuaScript* ob = m_scripts.at(pos);
		m_scripts.remove(name);
		delete ob;
	}
}


void gkLuaManager::destroy(gkLuaScript* ob)
{
	GK_ASSERT(ob);
	destroy(ob->getName());
}


void gkLuaManager::destroyAll(void)
{
	utHashTableIterator<ScriptMap> iter(m_scripts);
	while (iter.hasMoreElements())
	{
		gkLuaScript* ob = iter.peekNextValue();
		delete ob;
		iter.next();
	}
	m_scripts.clear();
}



bool gkLuaManager::hasScript(const gkString& name)
{
	return m_scripts.find(name) != GK_NPOS;
}



GK_IMPLEMENT_SINGLETON(gkLuaManager);
