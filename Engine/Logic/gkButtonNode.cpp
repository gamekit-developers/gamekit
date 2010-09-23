/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Nestor Silveira.

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
#include "gkWindowSystem.h"
#include "gkButtonNode.h"
#include "gkEngine.h"
#include "gkLogger.h"


gkButtonNode::gkButtonNode(gkLogicTree* parent, size_t id)
	: gkLogicNode(parent, id),
	  m_pressed(false),
	  m_delay(0),
	  m_counter(0)
{
	ADD_ISOCK(UPDATED, true);
	ADD_OSOCK(IS_DOWN, false);
	ADD_OSOCK(NOT_IS_DOWN, false);
	ADD_OSOCK(PRESS, false);
	ADD_OSOCK(RELEASE, false);
}

bool gkButtonNode::evaluate(gkScalar tick)
{
	bool ok = GET_SOCKET_VALUE(UPDATED);

	if (ok && ++m_counter > m_delay)
	{
		m_counter = 0;
	}

	return ok && !m_counter;
}

void gkButtonNode::update(gkScalar tick)
{
	bool isPressed = isButtonDown();

	SET_SOCKET_VALUE(IS_DOWN, isPressed);
	SET_SOCKET_VALUE(NOT_IS_DOWN, !isPressed);

	if (isPressed && !m_pressed)
	{
		m_pressed = true;
		SET_SOCKET_VALUE(PRESS, true);
		SET_SOCKET_VALUE(RELEASE, false);
	}
	else if (!isPressed && m_pressed)
	{
		m_pressed = false;
		SET_SOCKET_VALUE(PRESS, false);
		SET_SOCKET_VALUE(RELEASE, true);
	}
	else
	{
		SET_SOCKET_VALUE(PRESS, false);
		SET_SOCKET_VALUE(RELEASE, false);
	}
}

//////////////////////////////////////////
bool gkKeyNode::isButtonDown() const
{
	return gkWindowSystem::getSingleton().getKeyboard()->isKeyDown(m_key);
}

///////////////////////////////////////////
bool gkMouseButtonNode::isButtonDown() const
{
	return gkWindowSystem::getSingleton().getMouse()->isButtonDown(m_button);
}
