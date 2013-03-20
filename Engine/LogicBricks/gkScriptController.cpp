/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Charlie C.

    Contributor(s): Thomas Trocha(dertom)
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
#include "gkCommon.h"
#ifdef OGREKIT_USE_LUA

#include "gkScriptController.h"
#include "gkTextManager.h"
#include "gkTextFile.h"
#include "Script/Lua/gkLuaManager.h"
#include "Script/Lua/gkLuaUtils.h"
#include "gkUtils.h"






gkScriptController::gkScriptController(gkGameObject* object, gkLogicLink* link, const gkString& name)
	:       gkLogicController(object, link, name), m_script(0), m_error(false), m_isModule(false), m_event(0)
{
}

gkScriptController::~gkScriptController()
{
	if (m_event)
		delete m_event;
}


gkLogicBrick* gkScriptController::clone(gkLogicLink* link, gkGameObject* dest)
{
	gkScriptController* cont = new gkScriptController(*this);
	cont->cloneImpl(link, dest);

	return cont;
}


void gkScriptController::setScript(const gkString& str)
{
	gkLuaScript* scrpt = gkLuaManager::getSingleton().getByName<gkLuaScript>(gkResourceName(str, getObjectGroupName()));
	if (scrpt)
		m_script = scrpt;
	else
	{
		// Create on demand.
		gkTextFile* tf = (gkTextFile*)gkTextManager::getSingleton().getByName(str);
		if (tf)
		{
			scrpt = gkLuaManager::getSingleton().createFromText(
				gkResourceName(tf->getName(), getObjectGroupName()), tf->getText());
			if (scrpt)
				m_script = scrpt;
		}
	}
}

void gkScriptController::setScriptByString(const gkString& str)
{
	gkLuaScript* scrpt;
	scrpt = gkLuaManager::getSingleton().createFromText(
			gkResourceName(gkUtils::getUniqueName("customscript"),getObjectGroupName()),str);
	if (scrpt)
		m_script = scrpt;
}

void gkScriptController::setLuaScript(const gkLuaCurState& self, const gkLuaCurState& function)
{
	m_event = new gkLuaEvent(self,function);
}

void gkScriptController::setLuaScript(const gkLuaCurState& function)
{
	m_event = new gkLuaEvent(function);
}


void gkScriptController::execute(void)
{
	if (m_error || m_sensors.empty())
		return;

	// Main script, can be null.
	if (m_script != 0)
		m_error = !m_script->execute();

	if (m_event)
	{
		m_event->beginCall();
		m_event->addArgument(true);
		bool result = true;
		m_event->call(result);
//		m_error = !
	}
}


#endif //OGREKIT_USE_LUA
