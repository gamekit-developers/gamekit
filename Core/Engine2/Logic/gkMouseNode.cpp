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
#include "gkMouseNode.h"
#include "gkLogicSocket.h"
#include "gkEngine.h"
#include "gkLogger.h"


using namespace Ogre;



gkMouseNode::gkMouseNode(gkLogicTree *parent, size_t id) :
        gkLogicNode(parent, NT_MOUSE, id)
{
    ADD_ISOCK(m_sockets[0],  this, gkLogicSocket::ST_REAL);
    ADD_ISOCK(m_sockets[1],  this, gkLogicSocket::ST_REAL);
    m_sockets[0].setValue(1.f);
    m_sockets[1].setValue(1.f);

    ADD_OSOCK(m_sockets[2],  this, gkLogicSocket::ST_BOOL);
    ADD_OSOCK(m_sockets[3],  this, gkLogicSocket::ST_REAL);
    ADD_OSOCK(m_sockets[4],  this, gkLogicSocket::ST_REAL);
    ADD_OSOCK(m_sockets[5],  this, gkLogicSocket::ST_REAL);
    ADD_OSOCK(m_sockets[6],  this, gkLogicSocket::ST_REAL);
    ADD_OSOCK(m_sockets[7],  this, gkLogicSocket::ST_REAL);
}


bool gkMouseNode::evaluate(gkScalar tick)
{
    gkMouse* dev = gkWindowSystem::getSingleton().getMouse();

    gkScalar x = 0, y = 0;
    gkScalar x_scale = m_sockets[0].getValueReal();
    gkScalar y_scale = m_sockets[1].getValueReal();

    if (dev->moved)
    {
        if (m_sockets[3].isConnected()) x = dev->relitave.x * x_scale;
        if (m_sockets[4].isConnected()) y = dev->relitave.y * y_scale;
    }

    if (m_sockets[5].isConnected()) m_sockets[5].setValue(dev->position.x * x_scale);
    if (m_sockets[6].isConnected()) m_sockets[6].setValue(dev->position.y * y_scale);

    m_sockets[3].setValue(x);
    m_sockets[4].setValue(y);
    m_sockets[2].setValue(dev->moved);
    m_sockets[7].setValue(dev->wheelDelta);
    return false;
}
