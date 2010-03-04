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
#include "gkLogicTree.h"
#include "gkNodeManager.h"

using namespace Ogre;

gkLogicTree::gkLogicTree(gkNodeManager* creator, UTsize id, const gkString &name)
:       m_handle(id), m_uniqueHandle(0), m_creator(creator), m_object(0),
        m_name(name), m_initialized(false), m_sorted(false)
{
}


gkLogicTree::gkLogicTree(gkNodeManager* creator, UTsize id)
:       m_handle(id), m_uniqueHandle(0), m_creator(creator), m_object(0),
        m_name(StringUtil::BLANK), m_initialized(false), m_sorted(false)
{
}


gkLogicTree::~gkLogicTree()
{
    destroyNodes();
}


void gkLogicTree::attachObject(gkGameObject *ob)
{
    m_object = ob;

    // notify all
    if (!m_nodes.empty())
    {
        NodeIterator iter(m_nodes);
        while (iter.hasMoreElements())
            iter.getNext()->attachObject(m_object);
    }
}


void gkLogicTree::destroyNodes(void)
{
    if (!m_nodes.empty())
    {
        NodeIterator iter(m_nodes);
        while (iter.hasMoreElements())
            delete iter.getNext();
    }
    m_nodes.clear();
    m_uniqueHandle = 0;
}


void gkLogicTree::freeUnused(void)
{
    // delete nodes that have no links
}

gkLogicNode* gkLogicTree::getNode(UTsize idx)
{
    if (idx >= 0 && idx < m_nodes.size())
        return m_nodes.at(idx);
    return 0;
}


// Sort trees for optimal execution
class gkLogicSolver
{
public:
    typedef gkLogicNode* gkLogicNodeT;

    static bool sort(const gkLogicNodeT& a, const gkLogicNodeT& b)
    {
        return a->getPriority() < b->getPriority();
    }

public:

    void solve(gkLogicTree *tree)
    {
        gkLogicTree::NodeIterator iter = tree->getNodeIterator();
        while (iter.hasMoreElements())
            setPriority(tree, iter.getNext());
    }

    void setPriority(gkLogicTree* tree, gkLogicNode *node)
    {
        // setting priority + 1 for each connection 

        GK_ASSERT(node);
        gkLogicNode::Sockets &sockets = node->getInputs();
        if (!sockets.empty())
        {
            gkLogicNode::SocketIterator sockit(sockets);
            while (sockit.hasMoreElements())
            {
                gkLogicSocket *sock = sockit.getNext();
                if (sock->isLinked())
                {
                    gkLogicSocket *fsock = sock->getFrom();
                    if (!fsock) continue;

                    gkLogicTree::NodeIterator iter = tree->getNodeIterator();
                    while (iter.hasMoreElements())
                    {
                        gkLogicNode* onode = iter.getNext();
                        if (onode != node && onode == fsock->getParent())
                        {
                            onode->setPriority(node->getPriority() + 1);
                            setPriority(tree, onode);
                        }
                    }
                }
            }
        }

    }
};


#define NT_DUMP_ORDER 0

#if NT_DUMP_ORDER == 1
char *NTGetName(int nt)
{
    switch (nt)
    {
    case NT_GROUP:      return "NT_GROUP        ";
    case NT_OBJECT:     return "NT_OBJECT       ";
    case NT_MOUSE:      return "NT_MOUSE        ";
    case NT_MOUSEBUTTON:return "NT_MOUSEBUTTON  ";
    case NT_KEY:        return "NT_KEY          ";
    case NT_MATH:       return "NT_MATH         ";
    case NT_MOTION:     return "NT_MOTION       ";
    case NT_PRINT:      return "NT_PRINT        ";
    case NT_ANIM:       return "NT_ANIM         ";
    case NT_RAND:       return "NT_RAND         ";
    case NT_VALUE:      return "NT_VALUE        ";
    case NT_VARIABLE:   return "NT_VARIABLE     ";
    case NT_VARIABLE_OP:return "NT_VARIABLE_OP  ";
    case NT_EXPR:       return "NT_EXPR         ";
    case NT_SWITCH:     return "NT_SWITCH       ";
    case NT_IF:         return "NT_IF           ";
    case NT_TIMER:      return "NT_TIMER        ";
    case NT_EXIT:       return "NT_EXIT         ";
    }
    return "NT_UNKNOWN      ";
}

#endif

void gkLogicTree::solveOrder(bool forceSolve)
{
    if (m_sorted && !forceSolve)
        return;

    if (m_sorted)
    {
        NodeIterator iter(m_nodes);
        while (iter.hasMoreElements())
            iter.getNext()->setPriority(0);
    }
    m_sorted = true;

    gkLogicSolver s;
    s.solve(this);

#if NT_DUMP_ORDER == 0
    m_nodes.sort(gkLogicSolver::sort);
#else

    printf("--- node order before sort ---\n");
    NodeIterator iter(m_nodes);
    while (iter.hasMoreElements())
    {
        gkLogicNode *lnode = iter.getNext();
        printf("%s:%i\n", (NTGetName((int)lnode->getType())), lnode->getPriority());
    }

    m_nodes.sort(gkLogicSolver::sort);


    printf("--- node order after sort ---\n");
    NodeIterator iter2(m_nodes);
    while (iter2.hasMoreElements())
    {
        gkLogicNode *lnode = iter2.getNext();
        printf("%s:%i\n", (NTGetName((int)lnode->getType())), lnode->getPriority());
    }
#endif
 
}

void gkLogicTree::execute(gkScalar tick)
{
    if (m_nodes.empty())
    {
        // undefined
        return;
    }

    if (!m_sorted)
        solveOrder();


    if (!m_initialized)
    {
        NodeIterator iter(m_nodes);
        while (iter.hasMoreElements())
            iter.getNext()->initialize();
        m_initialized = true;
    }

    NodeIterator iter(m_nodes);
    while (iter.hasMoreElements())
    {
        gkLogicNode *node = iter.getNext();
        // can continue
        if (node->evaluate(tick)) 
            node->update(tick);
    }
}
