/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): silveira.nestor.
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
#include "gkIfNode.h"


using namespace Ogre;




gkIfNode::gkIfNode(gkLogicTree *parent, size_t id) 
:       gkLogicNode(parent, id), m_stmt(CMP_NULL)
{
    ADD_ISOCK(m_sockets[0], this, gkLogicSocket::ST_REAL);
    ADD_ISOCK(m_sockets[1], this, gkLogicSocket::ST_REAL);
    ADD_OSOCK(m_sockets[2], this, gkLogicSocket::ST_BOOL);
    ADD_OSOCK(m_sockets[3], this, gkLogicSocket::ST_BOOL);

    m_sockets[0].setValue(0.f);
    m_sockets[1].setValue(0.f);
    m_sockets[2].setValue(false);
    m_sockets[3].setValue(false);
}

        
bool gkIfNode::evaluate(gkScalar tick)
{
    bool result = false;
    switch (m_stmt)
    {
    case CMP_TRUE:
        {
            if (m_sockets[0].isLinked())
                result = m_sockets[0].getValueBool();
            else if (m_sockets[1].isLinked())
                result = m_sockets[1].getValueBool();
            else
                result = m_sockets[0].getValueBool();
        }
        break;
    case CMP_NOT:
    case CMP_FALSE:
        {
            if (m_sockets[0].isLinked())
                result = !m_sockets[0].getValueBool();
            else if (m_sockets[1].isLinked())
                result = !m_sockets[1].getValueBool();
            else
                result = !m_sockets[0].getValueBool();
        }
        break;
    case CMP_AND:
        result = m_sockets[0].getValueBool() && m_sockets[1].getValueBool();
        break;
    case CMP_OR:
        result = m_sockets[0].getValueBool() || m_sockets[1].getValueBool();
        break;
    case CMP_EQUALS:
        result = m_sockets[0].getValueReal() == m_sockets[1].getValueReal();
        break;
    case CMP_NOT_EQUAL:
        result = m_sockets[0].getValueReal() != m_sockets[1].getValueReal();
        break;
    case CMP_GREATER:
        result = (m_sockets[0].getValueReal() > m_sockets[1].getValueReal());
        break;
    case CMP_GTHAN:
        result = (m_sockets[0].getValueReal() >= m_sockets[1].getValueReal());
        break;
    case CMP_LESS:
        result = (m_sockets[0].getValueReal() < m_sockets[1].getValueReal());
        break;
    case CMP_LTHAN:
        result = (m_sockets[0].getValueReal() <= m_sockets[1].getValueReal());
        break;
    case CMP_NULL:
    default:
        break;
    }
    m_sockets[2].setValue(result);
    m_sockets[3].setValue(!result);
    return false;
}
