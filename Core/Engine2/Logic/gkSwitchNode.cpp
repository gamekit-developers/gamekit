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
#include "gkSwitchNode.h"


using namespace Ogre;



gkSwitchNode::gkSwitchNode(gkLogicTree *parent, size_t id) :
        gkLogicNode(parent, NT_SWITCH, id), m_labels(0)
{
    ADD_ISOCK(m_sockets[0],  this, gkLogicSocket::ST_BOOL);
    ADD_ISOCK(m_sockets[1],  this, gkLogicSocket::ST_REAL);

    for (int i = START_IN; i < START_OT; i++)
        ADD_ISOCK(m_sockets[i], this, gkLogicSocket::ST_REAL);

    for (int i = START_OT; i < CASE_MAX; i++)
        ADD_OSOCK(m_sockets[i], this, gkLogicSocket::ST_BOOL);
}


bool gkSwitchNode::evaluate(gkScalar tick)
{
    bool res = m_sockets[0].getValueBool();
    if (!res)   _doBlocks(-1);
    else        _doBlocks(-2);
    return res;
}


void gkSwitchNode::_initialize()
{
    for (int i = START_OT; i < CASE_MAX; i++)
    {
        m_sockets[i].setValue(false);
        m_sockets[i].block(true);
    }
}


void gkSwitchNode::update(gkScalar tick)
{
    gkScalar swtch = m_sockets[1].getValueReal();

    int ltest = 0;
    int offs = 1;
    for (int i = START_IN; i < START_OT && ltest < (m_labels); i++, ltest++, offs++)
    {
        // test label
        if (swtch == m_sockets[i].getValueReal())
        {
            _doBlocks(START_OT + offs);
            return;
        }
    }

    // default label
    if (m_sockets[START_OT].isConnected())
        _doBlocks(START_OT);
}


void gkSwitchNode::_doBlocks(int idx)
{
    for (int i = START_OT; i < CASE_MAX; i++)
    {
        if (idx == -1)
        {
            m_sockets[i].block(true);
            m_sockets[i].setValue(false);
            continue;
        }
        else if (idx == -2)
        {
            m_sockets[i].block(false);
            m_sockets[i].setValue(false);
            continue;
        }

        if (i == idx && m_sockets[i].isConnected())
        {
            m_sockets[i].block(false);
            m_sockets[i].setValue(true);
        }
        else
        {
            if (m_sockets[i].isConnected())
            {
                m_sockets[i].block(true);
                m_sockets[i].setValue(false);
            }
        }
    }
}
