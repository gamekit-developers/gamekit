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
#include "gkWindowSystem.h"
#include "gkKeyNode.h"
#include "gkEngine.h"
#include "gkLogger.h"

using namespace Ogre;




gkKeyNode::gkKeyNode(gkLogicTree *parent, size_t id) :
        gkLogicNode(parent, NT_KEY, id), m_isPressed(false), m_pressed(false),
        m_delay(0), m_cnt(0), m_key(KC_NONE)
{
    ADD_ISOCK(m_sockets[0], this, gkLogicSocket::ST_BOOL);
    ADD_OSOCK(m_sockets[1], this, gkLogicSocket::ST_BOOL);
    ADD_OSOCK(m_sockets[2], this, gkLogicSocket::ST_BOOL);
    ADD_OSOCK(m_sockets[3], this, gkLogicSocket::ST_BOOL);
}



bool gkKeyNode::evaluate(gkScalar tick)
{
    if (!m_sockets[0].getValueBool())
    {
        // block outputs
        m_sockets[1].setValue(false);
        m_sockets[2].setValue(false);
        m_sockets[3].setValue(false);

        m_sockets[1].block(true);
        m_sockets[2].block(true);
        m_sockets[3].block(true);

        return false;
    }

    if (m_delay != 0)
    {
        m_cnt += 1;

        if (m_cnt < m_delay)
        {
            m_sockets[1].block(true);
            m_sockets[2].block(true);
            m_sockets[3].block(true);
            return false;
        }
        m_cnt = 0;
    }

    m_isPressed = gkWindowSystem::getSingleton().getKeyboard()->isKeyDown(m_key);
    m_sockets[1].setValue(m_isPressed);

    if (m_sockets[1].isBlocked())
        m_sockets[1].block(false);


    if (m_isPressed && !m_pressed)
    {
        m_pressed = true;
        m_sockets[2].setValue(true);
        m_sockets[3].setValue(false);

        if (m_sockets[2].isBlocked()) m_sockets[2].block(false);
        if (!m_sockets[3].isBlocked())m_sockets[3].block(true);
    }
    else if (!m_isPressed && m_pressed)
    {
        m_pressed = false;
        m_sockets[2].setValue(false);
        m_sockets[3].setValue(true);


        if (!m_sockets[2].isBlocked()) m_sockets[2].block(true);
        if (m_sockets[3].isBlocked())  m_sockets[3].block(false);
    }
    else
    {
        m_sockets[2].setValue(false);
        m_sockets[3].setValue(false);
        m_sockets[2].block(true);
        m_sockets[3].block(true);
    }

    return false;
}
