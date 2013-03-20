/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 harkon.kr.

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

#include "gkExpressionController.h"
#include "gkLogicManager.h"
#include "gkTextManager.h"
#include "gkTextFile.h"
#include "gkUtils.h"
#include "Script/Lua/gkLuaManager.h"
#include "Script/Lua/gkLuaUtils.h"


gkExpressionController::gkExpressionController(gkGameObject* object, gkLogicLink* link, const gkString& name)
	:       gkLogicController(object, link, name), m_script(0), m_error(false), m_isModule(false)
{
}

gkExpressionController::~gkExpressionController()
{
	if (m_script)
	{
		//gkLuaManager::getSingleton().destroy(m_script); //TODO:use shared ptr destroy
		m_script = 0;
	}
}

gkLogicBrick* gkExpressionController::clone(gkLogicLink* link, gkGameObject* dest)
{
	gkExpressionController* cont = new gkExpressionController(*this);
	cont->cloneImpl(link, dest);

	return cont;
}


void gkExpressionController::setExpression(const gkString& str)
{

	gkString expr = "return " + str + "\n";
	gkLuaScript* scrpt = gkLuaManager::getSingleton().createFromText(
		gkResourceName(gkUtils::getUniqueName(m_name), getObjectGroupName()), expr);
	if (scrpt)
		m_script = scrpt;	
}


void gkExpressionController::execute(void)
{
	if (m_error || m_sensors.empty())
		return;

	// Main script, can be null.
	if (m_script != 0)
	{
		m_error = !m_script->execute();
		if (!m_error && !m_actuators.empty())
		{
			bool ret = m_script->getReturnBoolValue();
			gkLogicManager* mgr = m_link->getLogicManager();
			gkActuatorIterator it(m_actuators);
			while (it.hasMoreElements())
			{
				gkLogicActuator* act = it.getNext();
				mgr->push(this, act, ret);
			}
		}
	}

}
#endif //OGREKIT_USE_LUA
