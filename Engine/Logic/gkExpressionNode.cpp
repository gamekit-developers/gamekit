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
#include "gkExpressionNode.h"

/*
using namespace Ogre;



gkExpressionNode::gkExpressionNode(gkLogicTree *parent, size_t id) :
        gkLogicNode(parent, id), m_nr(0), m_expr(0), m_code(""), m_err(false)
{
    ADD_ISOCK(m_sockets[0], this, gkLogicSocket::ST_BOOL);
    m_sockets[0].setValue(true);

    for (int i = 1; i < 11; i++)
    {
        ADD_ISOCK(m_sockets[i], this, gkLogicSocket::ST_REAL);
        m_sockets[i].setValue(0.f);
    }

    ADD_OSOCK(m_sockets[11], this, gkLogicSocket::ST_BOOL);
    ADD_OSOCK(m_sockets[12], this, gkLogicSocket::ST_BOOL);
}


bool gkExpressionNode::evaluate(gkScalar tick)
{
    if (m_err || !m_sockets[0].getValueBool())
    {
        m_sockets[11].setValue(false);
        m_sockets[12].setValue(false);
    }
    return !m_err && m_sockets[0].getValueBool();
}


void gkExpressionNode::update(gkScalar tick)
{
    if (m_expr != 0 && m_expr->isOk())
    {
        int nr = 0;
        for (int i = 1; i < 11 && nr < m_nr; i++, nr++)
        {
            // m_expr->addConstant(m_sockets[i].getRealName(), m_sockets[i].getValueReal());
        }

        int result = m_expr->run();
        if (result == gkScriptExpression::EXPR_ERROR)
        {
            m_err = true;
            m_sockets[11].setValue(false);
            m_sockets[12].setValue(false);
            return;
        }

        m_sockets[11].setValue(result == gkScriptExpression::EXPR_TRUE);
        m_sockets[12].setValue(result == gkScriptExpression::EXPR_FALSE);
    }
}


void gkExpressionNode::initialize()
{
    if (m_expr == 0 && !m_code.empty())
        m_expr = 0;//gkScriptManager::getSingleton().createExpression(m_code);

    m_sockets[11].setValue(false);
    m_sockets[12].setValue(false);
}
*/
