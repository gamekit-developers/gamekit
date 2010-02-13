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
#include "gkGroupNode.h"
#include "gkLogicTree.h"

using namespace Ogre;



gkGroupNode::gkGroupNode(gkLogicTree *parent, size_t id) :
        gkLogicNode(parent, NT_GROUP, id)
{
}


gkGroupNode::~gkGroupNode()
{
    gkSocketAlias::iterator it;

    it = m_inpAlias.begin();
    while (it != m_inpAlias.end())
    {
        delete it->second;
        ++it;
    }

    it = m_outAlias.begin();
    while (it != m_outAlias.end())
    {
        delete it->second;
        ++it;
    }
}


void gkGroupNode::update(gkScalar tick)
{
    if (m_group)
    {
        if (m_group->getAttachedObject() != m_object)
            m_group->attachObject(m_object);

        gkSocketAlias::iterator it, end;
        it =  m_inpAlias.begin();
        end = m_inpAlias.end();
        while (it != end)
        {
            it->first->copy(*it->second);
            ++it;
        }

        it =  m_outAlias.begin();
        end = m_outAlias.end();
        while (it != end)
        {
            it->first->copy(*it->second);
            ++it;
        }

        // execute tree
        m_group->execute(tick);
    }
}


void gkGroupNode::pushInput(gkLogicSocket *sock)
{
    if (sock != 0)
    {
        gkLogicSocket *ret = new gkLogicSocket();
        ret->copy(*sock);

        m_inpAlias.insert(std::make_pair(sock, ret));
        m_inputs.push_back(ret);
    }
}


void gkGroupNode::pushOutput(gkLogicSocket *sock)
{
    if (sock != 0)
    {
        gkLogicSocket *ret = new gkLogicSocket();
        ret->copy(*sock);

        m_outAlias.insert(std::make_pair(sock, ret));
        m_outputs.push_back(ret);
    }
}
