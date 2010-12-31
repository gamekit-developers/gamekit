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
#include "gkLogicBrick.h"
#include "gkGameObject.h"
#include "gkLogicLink.h"
#include "gkLogicManager.h"

gkLogicBrick::gkLogicBrick(gkGameObject* object, gkLogicLink* link, const gkString& name)
	:       m_object(object), m_name(name), m_link(link), m_stateMask(0), m_pulseState(BM_IDLE),
	        m_debugMask(0), m_isActive(false), m_priority(0), m_listener(0)
{
	GK_ASSERT(m_object);
	m_scene = m_object->getOwner();
}


gkLogicBrick::~gkLogicBrick()
{
}

void gkLogicBrick::cloneImpl(gkLogicLink* link, gkGameObject* dest)
{
	m_object        = dest;
	m_scene         = m_object->getOwner();
	m_pulseState    = BM_IDLE;
	m_isActive      = false;
	m_link          = link;

	gkLogicManager::getSingleton().notifySort();
}


bool gkLogicBrick::inActiveState(void) const
{
	return (m_stateMask & m_link->getState()) != 0;
}


bool gkLogicBrick::wantsDebug(void) const
{
	return (m_debugMask) != 0;
}


void gkLogicBrick::setPriority(bool v)
{
	int op = m_priority;
	m_priority = v ? 1 : 0;
	if (op != m_priority)
		gkLogicManager::getSingleton().notifySort();
}


void gkLogicBrick::setPriority(int v)
{
	int op = m_priority;
	m_priority = v;
	if (op != m_priority)
		gkLogicManager::getSingleton().notifySort();
}


const gkString& gkLogicBrick::getObjectGroupName(void) const
{
	return m_object->getGroupName();
}