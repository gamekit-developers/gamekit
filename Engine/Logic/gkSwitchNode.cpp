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
#include "gkSwitchNode.h"

/*
using namespace Ogre;



gkSwitchNode::gkSwitchNode(gkLogicTree *parent, size_t id) :
        gkLogicNode(parent, id), m_labels(0)
{
    ADD_ISOCK(m_sockets[0],  this, gkLogicSocket::ST_BOOL);
    ADD_ISOCK(m_sockets[1],  this, gkLogicSocket::ST_REAL);

    m_sockets[0].setValue(true);
    m_sockets[1].setValue(0.f);


    for (int i = START_IN; i < START_OT; i++)
    {
        ADD_ISOCK(m_sockets[i], this, gkLogicSocket::ST_REAL);
        m_sockets[i].setValue(0.f);
    }

    for (int i = START_OT; i < CASE_MAX; i++)
    {
        ADD_OSOCK(m_sockets[i], this, gkLogicSocket::ST_BOOL);
        m_sockets[i].setValue(0.f);
    }
}


bool gkSwitchNode::evaluate(gkScalar tick)
{
    return m_sockets[0].getValueBool();
}

void gkSwitchNode::initialize()
{
    for (int i = START_OT; i < CASE_MAX; i++)
        m_sockets[i].setValue(false);
}


void gkSwitchNode::update(gkScalar tick)
{
    // clear returns
    for (int i = START_OT; i < CASE_MAX; i++)
        m_sockets[i].setValue(false);

    gkScalar swtch = m_sockets[1].getValueReal();

    int ltest = 0;
    int offs = 1;
    for (int i = START_IN; i < START_OT && ltest < (m_labels); i++, ltest++, offs++)
    {
        // test label
        if (swtch == m_sockets[i].getValueReal())
        {
            m_sockets[(START_OT+1) + (i-START_IN)].setValue(true);
            return;
        }
    }

    // default label
    if (m_sockets[START_OT].isConnected())
         m_sockets[START_OT].setValue(true);
}
*/
