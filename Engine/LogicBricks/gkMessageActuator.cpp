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

#include "gkMessageActuator.h"
#include "gkMessageManager.h"
#include "gkGameObject.h"
#include "gkVariable.h"

gkMessageActuator::gkMessageActuator(gkGameObject* object, gkLogicLink* link, const gkString& name)
	:    gkLogicActuator(object, link, name), m_to(""), m_subject(""), m_bodyType(BT_TEXT), m_bodyText(""), m_bodyProp("")
{

}


gkLogicBrick* gkMessageActuator::clone(gkLogicLink* link, gkGameObject* dest)
{
	gkMessageActuator* act = new gkMessageActuator(*this);
	act->cloneImpl(link, dest);
	return act;
}


void gkMessageActuator::execute(void)
{
	if (isPulseOff())
		return;

	gkString from, body;

	from = m_object->getName();

	if (m_bodyType == BT_TEXT)
	{
		body = m_bodyText;
	}
	else if (m_bodyType == BT_PROP && m_object->hasVariable(m_bodyProp))
	{
		body = m_object->getVariable(m_bodyProp)->getValueString();
	}

	gkMessageManager::getSingleton().sendMessage(from, m_to, m_subject, body);

	setPulse(BM_OFF);
}
