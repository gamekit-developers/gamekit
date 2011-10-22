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
#include "gkStateActuator.h"
#include "gkLogicLink.h"
#include "gkLogicManager.h"
#include "gkGameObject.h"



gkStateActuator::gkStateActuator(gkGameObject* object, gkLogicLink* link, const gkString& name)
	:       gkLogicActuator(object, link, name), m_stateMask(0), m_op(OP_NILL)
{
}


gkStateActuator::~gkStateActuator()
{
}


gkLogicBrick* gkStateActuator::clone(gkLogicLink* link, gkGameObject* dest)
{
	gkStateActuator* act = new gkStateActuator(*this);
	act->cloneImpl(link, dest);
	return act;
}


void gkStateActuator::execute(void)
{
	if (isPulseOff())
		return;

	unsigned int oldState = m_link->getState();

	switch (m_op)
	{
	case OP_ADD:
		m_link->setState(m_link->getState() | m_stateMask);
		break;
	case OP_SUB:
		m_link->setState(m_link->getState() & ~m_stateMask);
		break;
	case OP_CPY:
		m_link->setState(m_stateMask);
		break;
	case OP_INV:
		m_link->setState(m_link->getState() ^ m_stateMask);
		break;
	case OP_NILL:
	default:
		break;
	}

	unsigned int newState = m_link->getState();
	if (oldState != newState)
	{
		m_object->setState(newState);
		m_link->notifyState();

		gkLogicManager::getSingleton().notifyState(newState, m_link);
	}
	setPulse(BM_OFF);

}
