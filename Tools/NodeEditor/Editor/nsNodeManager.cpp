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
#include "nsNodeManager.h"
#include "nsMainWindow.h"

// ----------------------------------------------------------------------------
NS_IMPLEMENT_SINGLETON(nsNodeManager);


// ----------------------------------------------------------------------------
nsSocket::nsSocket(nsNode *nd, nsSocketType *st)
    :   m_rect(st->m_rect), 
        m_derrivedRect(st->m_rect), 
        m_type(st), 
        m_parent(nd), 
        m_from(0), 
        m_index(UT_NPOS)
{
    UT_ASSERT(m_parent);
    m_value.setValue(st->m_default);
    updateFromParent();
}


// ----------------------------------------------------------------------------
nsSocket::~nsSocket()
{
    m_tosockets.clear();
    m_from = 0;
}


// ----------------------------------------------------------------------------
void nsSocket::connect(nsSocket *oth)  
{
    // ignore links on the same node
    if (oth && oth->m_parent == m_parent)
        return;

    if (oth)
    {
        if (m_from == oth)
        {
            // ignore linking to the same socket
            return;
        }

        if (m_from != 0)
        {
            ToLinks &links = m_from->m_tosockets;

            // other is zero so erase this in the from node 
            UT_ASSERT((!links.empty() && links.find(this)) && "Socket improperly linked!");
            links.erase(this);

            bool isFirst = getPrev() == 0;

            nsSocket *sock = 0;

            if (isFirst)
                sock = getNext();
            else
            {
                // try top down
                sock = getPrev();
                if (sock)
                {
                    nsSocket *root = sock;
                    while (root)
                    {
                        sock = root;
                        root = root->getPrev();
                        if (!root)
                            break;
                    }
                }
            }

            while (sock)
            {
                if (sock != this && !sock->isConnected())
                {
                    if (m_from->canConnect(sock))
                    {
                        sock->connect(m_from);
                        break;
                    }
                }
                sock = sock->getNext();
            }
        }

        // linking to 
        UT_ASSERT(oth->isOutput() && isInput());

        m_from = oth;
        m_from->m_tosockets.push_back(this);
    }
    else
    {
        // make sure this is an input 
        UT_ASSERT(isInput());


        if (m_from)
        {
            ToLinks &links = m_from->m_tosockets;

            // other is zero so erase this in the from node 
            UT_ASSERT((!links.empty() && links.find(this)) && "Socket improperly linked!");
            links.erase(this);
            m_from = 0;
        }
    }
}

// ----------------------------------------------------------------------------
void nsSocket::updateFromParent(void)
{
    if (!m_parent)
        return;


    NSrect prect = m_parent->getRect();
    m_derrivedRect = m_type->m_rect;
    m_derrivedRect.x = prect.x + m_rect.x;
    m_derrivedRect.y = prect.y + m_rect.y;
}

// ----------------------------------------------------------------------------
nsSocket* nsSocket::clone(nsNode *newParent)
{
    UT_ASSERT(newParent && m_type);

    // copy default vaues
    nsSocket *sock = new nsSocket(newParent, m_type);
    sock->m_value = m_value;
    return sock;
}


// ----------------------------------------------------------------------------
void nsSocket::unlink(void)
{
    // remove all refs to this 

    if (m_type->m_direction == nsSocketType::Out)
    {
        if (!m_tosockets.empty())
        {
            utListIterator<ToLinks> it(m_tosockets);
            while (it.hasMoreElements())
            {
                nsSocket *sock = it.getNext();
                if (sock->m_from == this)
                    sock->m_from = 0;
            }
        }
    }
    else
    {
        if (m_from)
        {
            ToLinks &links = m_from->m_tosockets;
            UT_ASSERT((!links.empty() && links.find(this)) && "Socket improperly linked!");
            links.erase(this);
        }
    }

    m_tosockets.clear();
    m_from = 0;
}


// ----------------------------------------------------------------------------
nsNode::nsNode(nsNodeTree *tree, nsNodeType *nt)
    :   m_type(nt), 
        m_tree(tree), 
        m_state(NDST_INACTIVE), 
        m_editOutputs(false), 
        m_rect(0,0, nt->m_size.x, nt->m_size.y)
{

    // create all socket types

    nsNodeType::SocketIterator in(nt->m_inputs);
    while (in.hasMoreElements())
    {
        nsSocketType *sin= in.getNext();
        nsSocket *isock = new nsSocket(this, sin);

        // save socket index, needed for re linking
        isock->setIndex(m_inputs.size());
        m_inputs.push_back(isock);
    }


    nsNodeType::SocketIterator out(nt->m_outputs);
    while (out.hasMoreElements())
    {
        nsSocketType *sout= out.getNext();
        nsSocket *osock = new nsSocket(this, sout);

            // save socket index, needed for re linking
        osock->setIndex(m_outputs.size());
        m_outputs.push_back(osock);
    }
}


// ----------------------------------------------------------------------------
nsNode::nsNode(nsNode *cpy, nsNodeTree *tree)
    :   m_type(0), m_tree(tree), m_state(NDST_INACTIVE), m_editOutputs(false)
{
    UT_ASSERT(cpy);

    m_type          = cpy->m_type;
    m_rect          = cpy->m_rect;
    m_editOutputs   = cpy->m_editOutputs;


    // clone sockets
    nsSocket *sock, *nsock;
    if (!cpy->m_inputs.empty())
    {
        sock = cpy->m_inputs.begin();
        while (sock)
        {
            nsock = sock->clone(this);

            // save socket index, needed for re linking
            nsock->setIndex(m_inputs.size());

            m_inputs.push_back(nsock);
            sock = sock->getNext();
        }
    }

    if (!cpy->m_outputs.empty())
    {
        sock = cpy->m_outputs.begin();
        while (sock)
        {
            nsock = sock->clone(this);

            // save socket index, needed for re linking
            nsock->setIndex(m_outputs.size());

            m_outputs.push_back(nsock);
            sock = sock->getNext();
        }
    }
}


// ----------------------------------------------------------------------------
nsNode::~nsNode()
{
    nsSocket *sock, *tsock;

    if (!m_inputs.empty())
    {
        sock = m_inputs.begin();
        while (sock)
        {
            tsock = sock;
            sock = sock->getNext();
            delete tsock;
        }
        m_inputs.clear();
    }


    if (!m_outputs.empty())
    {
        sock = m_outputs.begin();
        while (sock)
        {
            tsock = sock;
            sock = sock->getNext();
            delete tsock;
        }

        m_outputs.clear();
    }
}

// ----------------------------------------------------------------------------
void nsNode::unlink(void)
{
    // remove all refs to this 
    nsSocket *sock;

    if (!m_inputs.empty())
    {
        sock = m_inputs.begin();
        while (sock)
        {
            sock->unlink();
            sock = sock->getNext();
        }
    }

    if (!m_outputs.empty())
    {
        sock = m_outputs.begin();
        while (sock)
        {
            sock->unlink();
            sock = sock->getNext();
        }
    }
}


// ----------------------------------------------------------------------------
nsNode* nsNode::clone(nsNodeTree *newTree)
{
    return new nsNode(this, newTree);
}


// ----------------------------------------------------------------------------
void nsNode::translate(NSfloat x, NSfloat y)
{
    setPosition(m_rect.x + x, m_rect.y + y);
}


// ----------------------------------------------------------------------------
void nsNode::setPosition(NSfloat x, NSfloat y)
{
    m_rect.x = x;
    m_rect.y = y;

    nsSocket *sock;

    if (!m_inputs.empty())
    {
        sock = m_inputs.begin();
        while (sock)
        {
            sock->updateFromParent();
            sock = sock->getNext();
        }
    }

    if (!m_outputs.empty())
    {
        sock = m_outputs.begin();
        while (sock)
        {
            sock->updateFromParent();
            sock = sock->getNext();
        }
    }
}


// ----------------------------------------------------------------------------
nsNodeTree::nsNodeTree(const utString& name)
    :   m_name(name), 
        m_isGroup(false), 
        m_groupName(), 
        m_limits(0,0), 
        m_projection(0,0,0,0), 
        m_client(0), 
        m_size(0,0)
{
}


// ----------------------------------------------------------------------------
nsNodeTree::~nsNodeTree()
{
    clear();
}


// ----------------------------------------------------------------------------
void nsNodeTree::deleteNode(nsNode *node)
{
    if (node && m_nodes.find(node) != 0)
    {
        node->unlink();
        m_nodes.erase(node);
        delete node;
    }
}


// ----------------------------------------------------------------------------
nsNode *nsNodeTree::createNode(nsNodeType *nt)
{
    nsNode *nd = new nsNode(this, nt);
    m_nodes.push_back(nd);
    return nd;
}


// ----------------------------------------------------------------------------
nsNode *nsNodeTree::createCloneNode(nsNode *nd)
{
    nsNode *nnd = nd->clone(this);
    m_nodes.push_back(nnd);
    return nnd;
}

// ----------------------------------------------------------------------------
void nsNodeTree::bringToFront(nsNodes &list, nsNode *node)
{
    // make node the first to be rendered / hit tested 
    if (list.empty()) 
        return;

    if ( node != 0 )
    {
        if (list.back() == node)
            return;

        nsNodes::Pointer found = list.find( node );
        if ( found != 0 )
        {
            list.erase( found );
            list.push_back( node );
        }
    }
}


// ----------------------------------------------------------------------------
void nsNodeTree::bringToFront(nsNode *node)
{
    bringToFront(m_nodes, node);
}

// ----------------------------------------------------------------------------
void nsNodeTree::clear(void)
{
    if (!m_nodes.empty())
    {
        nsNodeIterator it = getNodeIterator();
        while(it.hasMoreElements())
            delete it.getNext();

        m_nodes.clear();
    }
}


// ----------------------------------------------------------------------------
nsNodeManager::nsNodeManager()
{
}

// ----------------------------------------------------------------------------
nsNodeManager::~nsNodeManager()
{
    if (!m_trees.empty())
    {
        nsTreeIterator it = getTreeIterator();
        while(it.hasMoreElements())
            delete it.getNext();
    }
}



// ----------------------------------------------------------------------------
void nsNodeManager::deleteTree(nsNodeTree *tree)
{
    if (m_trees.find(tree) != 0)
    {
        m_trees.erase(tree);
        delete tree;
    }
}

// ----------------------------------------------------------------------------
nsNodeTree *nsNodeManager::createTree(const utString& name)
{
    nsNodeTree *nt = new nsNodeTree(name);
    m_trees.push_back(nt);
    return nt;
}
