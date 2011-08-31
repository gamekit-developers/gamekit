/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

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

#include "gsScript.h"
#include "gsCore.h"


gsLuaScript::gsLuaScript(gkLuaScript* script) : m_luaScript(script)
{
}

bool gsLuaScript::execute(void)
{
	if(m_luaScript)
		return m_luaScript->execute();
		
	return false;
}

const gkString& gsLuaScript::getName(void)
{
	static gkString localStr = "";
	if(m_luaScript)
		return m_luaScript->getName();
		
	return localStr;
}

gsLuaManager::gsLuaManager()
{
	m_luaManager = gkLuaManager::getSingletonPtr();
}

gsLuaManager::~gsLuaManager()
{
}

gkLuaScript* gsLuaManager::getScript(const gkString& name)
{
	gkLuaManager* lptr = gkLuaManager::getSingletonPtr();

	if (lptr)
	{
		gkLuaScript* script = lptr->getByName<gkLuaScript>(name);
		
		if(script)
			return script;
	}
	return 0;
}

gkLuaScript* gsLuaManager::create(const gkString& name, const gkString& text)
{

	gkLuaManager* lptr = gkLuaManager::getSingletonPtr();

	if (lptr)
	{
		gkLuaScript* script = lptr->createFromText(name, text);
		
		if(script)
			return script;
	}
	return 0;
}
