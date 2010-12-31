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
#include "gkLogger.h"


extern "C" int _OgreKitLua_install(lua_State* L);



gkLuaManager::gkLuaManager()
	:   gkResourceManager("LuaManager", "Lua"),
		L(0)
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

gkResource* gkLuaManager::createImpl(const gkResourceName& name, const gkResourceHandle& handle)
{
	return new gkLuaScript(this, name, handle);
}


void gkLuaManager::notifyResourceCreatedImpl(gkResource* res)
{
	gkTextFile* intern = (gkTextFile*)gkTextManager::getSingleton().getByName(res->getName());

	if (intern)
	{
		gkLuaScript* script = (gkLuaScript*)res;
		script->setScript(intern->getText());		
	}
}

void gkLuaManager::notifyResourceDestroyedImpl(gkResource* res)
{
}


void gkLuaManager::decompileAll(void)
{
	Resources::Iterator iter = m_resources.iterator();
	while (iter.hasMoreElements())
	{
		gkLuaScript* script = (gkLuaScript*)iter.peekNextValue();
		script->decompile();
		iter.next();
	}

}


void gkLuaManager::decompileGroup(const gkString& group)
{
	Resources::Iterator iter = m_resources.iterator();
	while (iter.hasMoreElements())
	{
		gkLuaScript* script = (gkLuaScript*)iter.peekNextValue();
		if (script->getGroupName() == group)
			script->decompile();
		iter.next();
	}
}

gkLuaScript* gkLuaManager::createFromText(const gkResourceName& name, const gkString& text)
{
	if (exists(name))
		return 0;

	gkLuaScript* script = create<gkLuaScript>(name);

	script->setScript(text);
	return script;
}


gkLuaScript* gkLuaManager::createFromTextBlock(const gkResourceName& name)
{
	if (exists(name))
		return 0;

	gkTextFile* intern = (gkTextFile*)gkTextManager::getSingleton().getByName(name);

	if (intern == 0)
	{
		gkPrintf("Invalid internal text file %s\n", name.getName().c_str());
		return 0;
	}

	return createFromText(name, intern->getText());
}

UT_IMPLEMENT_SINGLETON(gkLuaManager);
