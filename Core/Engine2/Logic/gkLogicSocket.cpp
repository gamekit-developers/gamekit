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
#include "gkLogicSocket.h"
#include "gkLogicNode.h"
#include "gkLogger.h"

using namespace Ogre;



gkGameObject* gkLogicSocket::getGameObject() const
{
    if (m_from && m_from->m_parent)
        return m_from->m_parent->getAttachedObject();
    return m_parent ? m_parent->getAttachedObject() : 0;
}


bool gkLogicSocket::has_link(gkLogicNode *link)
{
    if (m_connected)
        return std::find(m_nodes.begin(), m_nodes.end(), link) != m_nodes.end();
    return false;
}


void gkLogicSocket::link(gkLogicSocket *fsock)
{
    if (!m_from)
    {
        if (fsock)
        {
            fsock->m_connected = m_connected = true;
            m_from = fsock;

            if (m_parent)
            {
                m_parent->setLinked();
                gkLogicNode *nd = fsock->getParent();
                if (nd) nd->setLinked();
            }
        }
    }

    // else error, only one from socket
}


void gkLogicSocket::push_link(gkLogicNode *link)
{
    if (m_connected)
        m_nodes.push_back(link);
}


void gkLogicSocket::block(bool truth)
{
    // block socket io
    if (m_connected && m_blocked != truth)
    {
        m_blocked = truth;

        gkNodeBaseIterator iter = getNodeIterator();
        while (iter.hasMoreElements())
        {
            gkLogicNode *node = iter.getNext();
            node->block(m_blocked);
        }
    }
}
