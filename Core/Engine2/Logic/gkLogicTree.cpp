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
#include "gkLogicTree.h"
#include "gkNodeManager.h"

#include "gkGroupNode.h"
#include "gkObjectNode.h"
#include "gkMouseNode.h"
#include "gkKeyNode.h"
#include "gkMathNode.h"
#include "gkMotionNode.h"
#include "gkPrintNode.h"
#include "gkAnimationNode.h"
#include "gkRandomNode.h"
#include "gkValueNode.h"
#include "gkVariableNode.h"
#include "gkIfNode.h"
#include "gkSwitchNode.h"
#include "gkExpressionNode.h"
#include "gkTimerNode.h"
#include "gkExitNode.h"

using namespace Ogre;



gkLogicTree::gkLogicTree(gkNodeManager* creator, size_t id, const gkString &name) :
        m_handle(id), m_uniqueHandle(0), m_creator(creator), m_object(0),
        m_name(name), m_initialized(false)
{
}


gkLogicTree::gkLogicTree(gkNodeManager* creator, size_t id) :
        m_handle(id), m_uniqueHandle(0), m_creator(creator), m_object(0),
        m_name(StringUtil::BLANK), m_initialized(false)
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

    gkNodeBaseIterator iter = getNodeIterator();
    while (iter.hasMoreElements())
        iter.getNext()->attachObject(m_object);
}


void gkLogicTree::destroyNodes()
{

    gkNodeBaseIterator iter = getNodeIterator();
    while (iter.hasMoreElements())
        delete iter.getNext();

    m_nodes.clear();
    m_uniqueHandle = 0;
}


void gkLogicTree::freeUnused()
{
    // delete nodes that have no links

    gkLogicNodeIterator it = m_nodes.begin(), end = m_nodes.end();
    while (it != end)
    {
        if (!(*it)->hasLinks())
        {
            delete(*it);

            m_nodes.erase(it);
            it = m_nodes.begin(); end = m_nodes.end();
        }
        else
            ++it;
    }

    if (m_nodes.empty())
        m_nodes.clear();
}


gkLogicNode* gkLogicTree::createNode(const gkNodeTypes& nt)
{
    gkLogicNode *node = 0;
    switch (nt)
    {
    case NT_GROUP:
        node = new gkGroupNode(this, m_uniqueHandle);
        break;
    case NT_OBJECT:
        node = new gkObjectNode(this, m_uniqueHandle);
        break;
    case NT_MOUSE:
        node = new gkMouseNode(this, m_uniqueHandle);
        break;
    case NT_KEY:
        node = new gkKeyNode(this, m_uniqueHandle);
        break;
    case NT_MATH:
        node = new gkMathNode(this, m_uniqueHandle);
        break;
    case NT_MOTION:
        node = new gkMotionNode(this, m_uniqueHandle);
        break;
    case NT_PRINT:
        node = new gkPrintNode(this, m_uniqueHandle);
        break;
    case NT_ANIM:
        node = new gkAnimationNode(this, m_uniqueHandle);
        break;
    case NT_RAND:
        node = new gkRandomNode(this, m_uniqueHandle);
        break;
    case NT_VALUE:
        node = new gkValueNode(this, m_uniqueHandle);
        break;
    case NT_VARIABLE:
        node = new gkVariableNode(this, m_uniqueHandle);
        break;
    case NT_VARIABLE_OP:
        node = new VariableOpNode(this, m_uniqueHandle);
        break;
    case NT_EXPR:
        node = new gkExpressionNode(this, m_uniqueHandle);
        break;
    case NT_SWITCH:
        node = new gkSwitchNode(this, m_uniqueHandle);
        break;
    case NT_IF:
        node = new gkIfNode(this, m_uniqueHandle);
        break;
    case NT_TIMER:
        node = new gkTimerNode(this, m_uniqueHandle);
        break;
    case NT_EXIT:
        node = new gkExitNode(this, m_uniqueHandle);
        break;

    }

    if (node != 0)
    {
        if (m_object != 0)
            node->attachObject(m_object);

        m_nodes.push_back(node);
        m_uniqueHandle ++;
    }
    return node;
}


gkLogicNode* gkLogicTree::getNode(size_t idx)
{
    if (idx >= 0 && idx < m_nodes.size())
        return m_nodes.at(idx);
    return 0;
}


void gkLogicTree::findToNodes(gkLogicSocket *from, gkLogicNodeBase &nodes)
{
    gkNodeBaseIterator iter = getNodeIterator();
    while (iter.hasMoreElements())
    {
        gkLogicNode *node = iter.getNext();

        gkSocketBaseIterator inputs = node->getInputs();
        while (inputs.hasMoreElements())
        {
            gkLogicSocket *isock = inputs.getNext();
            if (!isock->isLinked())
                continue;

            if (isock->getFrom() == from)
                nodes.push_back(isock->getParent());
        }
    }
}


bool has_link(gkLogicNodeBase base, gkLogicNode *node)
{
    return std::find(base.begin(), base.end(), node) != base.end();
}


bool SortLess(gkLogicNode *a, gkLogicNode *b)
{
    return a->getHandle() < b->getHandle();
}


void gkLogicTree::findSubNodeForward(gkLogicNodeBase &root, gkLogicNode *rfnode, gkLogicNode *from, gkLogicSocket *caller, bool skip)
{
    if (!from) return;

    gkSocketBaseIterator outputs = from->getOutputs();
    while (outputs.hasMoreElements())
    {
        gkLogicSocket *osock = outputs.getNext();
        if (!osock->isConnected())
            continue;

        // Only sort calling socket.
        if (skip && osock != caller)
            continue;


        gkLogicNodeBase outs;
        findToNodes(osock, outs);

        gkNodeBaseIterator iter = gkNodeBaseIterator(outs.begin(), outs.end());
        while (iter.hasMoreElements())
        {
            gkLogicNode *to = iter.getNext();

            if (!has_link(root, to))
            {
                root.push_back(to);
                findSubNodeForward(root, rfnode, to, 0, false);
            }
        }

        // The calling socket has been sorted.
        if (skip && osock == caller) return;
    }

    if (!skip)   /* only test the first 'to' node onward*/
    {
        gkSocketBaseIterator inputs = from->getInputs();
        while (inputs.hasMoreElements())
        {
            gkLogicSocket *isock = inputs.getNext();
            if (!isock->isLinked())
                continue;

            gkLogicNode *fnode = isock->getFrom()->getParent();

            if (fnode != 0)
            {
                // stop at the first input of the calling node
                if (fnode == rfnode) continue;

                if (!has_link(root, fnode))
                {
                    root.push_back(fnode);
                    findSubNodeForward(root, rfnode, fnode, 0, false);
                }
            }
        }
    }
}


bool gkLogicTree::isConnectedIn(gkLogicNode *from, gkLogicNode *test)
{
    gkSocketBaseIterator inputs = from->getInputs();
    while (inputs.hasMoreElements())
    {
        gkLogicSocket *isock = inputs.getNext();
        if (!isock->isLinked())
            continue;

        gkSocketBaseIterator outputs = test->getOutputs();
        while (outputs.hasMoreElements())
        {
            gkLogicSocket *osock = outputs.getNext();
            if (!osock->isConnected())
                continue;

            if (isock->getFrom() == osock)
                return true;
        }
    }
    return false;
}


void gkLogicTree::pushSocketTree(gkLogicNode *from)
{
    gkSocketBaseIterator outputs = from->getOutputs();
    while (outputs.hasMoreElements())
    {
        gkLogicSocket *osock = outputs.getNext();
        if (!osock->isConnected())
            continue;

        gkLogicNodeBase root;
        findSubNodeForward(root, from, from, osock, true);

        if (root.empty())
            continue;

        std::sort(root.begin(), root.end(), SortLess);


        // 'root' should contain all subnodes from  'osock's' to link
        gkNodeBaseIterator iter = gkNodeBaseIterator(root.begin(), root.end());
        while (iter.hasMoreElements())
        {
            gkLogicNode *node = iter.getNext();

            // break circular links
            if (isConnectedIn(from, node) || node == from)
                continue;


            // push subtree for io blocking see gkLogicSocket::block
            if (!osock->has_link(node)) osock->push_link(node);
        }
    }
}


void gkLogicTree::solveOrder()
{
    //Need to store sub-trees from
    //each socket so io blocking can
    //take place
    //So, for each connected socket
    //store direct and indirect links
    //from that socket

    gkNodeBaseIterator iter = getNodeIterator();
    while (iter.hasMoreElements())
    {
        gkLogicNode *node = iter.getNext();
        pushSocketTree(node);
    }
}


void gkLogicTree::execute(gkScalar tick)
{

    if (!m_initialized)
    {
        // first execution
        gkNodeBaseIterator iter = getNodeIterator();
        while (iter.hasMoreElements())
        {
            gkLogicNode *node = iter.getNext();
            node->_initialize();
        }

        m_initialized = true;
    }


    gkNodeBaseIterator iter = getNodeIterator();
    while (iter.hasMoreElements())
    {
        gkLogicNode *node = iter.getNext();
        if (!node->isBlocked() && node->evaluate(tick))
        {
            // can continue
            node->update(tick);
        }
    }
}
