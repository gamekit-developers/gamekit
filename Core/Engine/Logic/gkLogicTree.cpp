/*
-------------------------------------------------------------------------------
	This file is part of the Ogre GameKit port.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#include "gkLogicManager.h"

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


// ----------------------------------------------------------------------------
gkLogicTree::gkLogicTree(gkLogicManager* creator, size_t id, const String &name) :
		mHandle(id), mUniqueHandle(0), mCreator(creator), mObject(0),
		mName(name), mIsInitialized(false)
{
}

// ----------------------------------------------------------------------------
gkLogicTree::gkLogicTree(gkLogicManager* creator, size_t id) :
		mHandle(id), mUniqueHandle(0), mCreator(creator), mObject(0),
		mName(StringUtil::BLANK), mIsInitialized(false)
{
}

// ----------------------------------------------------------------------------
gkLogicTree::~gkLogicTree()
{
	destroyNodes();
}

// ----------------------------------------------------------------------------
void gkLogicTree::attachObject(gkGameObject *ob)
{
	mObject= ob;

	/* notify all */

	gkNodeBaseIterator iter= getNodeIterator();
	while (iter.hasMoreElements())
		iter.getNext()->attachObject(mObject);
}

// ----------------------------------------------------------------------------
void gkLogicTree::destroyNodes()
{

	gkNodeBaseIterator iter= getNodeIterator();
	while (iter.hasMoreElements())
		delete iter.getNext();

	mNodes.clear();
	mUniqueHandle= 0;
}

// ----------------------------------------------------------------------------
void gkLogicTree::freeUnused()
{
	/* delete nodes that have no links */

	gkLogicNodeIterator it= mNodes.begin(), end= mNodes.end();
	while (it != end)
	{
		if (!(*it)->hasLinks())
		{
			delete(*it);

			mNodes.erase(it);
			it= mNodes.begin(); end= mNodes.end();
		}
		else
			++it;
	}

	if (mNodes.empty())
		mNodes.clear();
}

// ----------------------------------------------------------------------------
gkLogicNode* gkLogicTree::createNode(const gkNodeTypes& nt)
{
	gkLogicNode *node= 0;
	switch (nt)
	{
	case NT_GROUP:
		node= new gkGroupNode(this, mUniqueHandle);
		break;
	case NT_OBJECT:
		node= new gkObjectNode(this, mUniqueHandle);
		break;
	case NT_MOUSE:
		node= new gkMouseNode(this, mUniqueHandle);
		break;
	case NT_KEY:
		node= new gkKeyNode(this, mUniqueHandle);
		break;
	case NT_MATH:
		node= new gkMathNode(this, mUniqueHandle);
		break;
	case NT_MOTION:
		node= new gkMotionNode(this, mUniqueHandle);
		break;
	case NT_PRINT:
		node= new gkPrintNode(this, mUniqueHandle);
		break;
	case NT_ANIM:
		node= new gkAnimationNode(this, mUniqueHandle);
		break;
	case NT_RAND:
		node= new gkRandomNode(this, mUniqueHandle);
		break;
	case NT_VALUE:
		node= new gkValueNode(this, mUniqueHandle);
		break;
	case NT_VARIABLE:
		node= new gkVariableNode(this, mUniqueHandle);
		break;
	case NT_VARIABLE_OP:
		node= new VariableOpNode(this, mUniqueHandle);
		break;
	case NT_EXPR:
		node= new gkExpressionNode(this, mUniqueHandle);
		break;
	case NT_SWITCH:
		node= new gkSwitchNode(this, mUniqueHandle);
		break;
	case NT_IF:
		node= new gkIfNode(this, mUniqueHandle);
		break;
	case NT_TIMER:
		node= new gkTimerNode(this, mUniqueHandle);
		break;
	case NT_EXIT:
		node= new gkExitNode(this, mUniqueHandle);
		break;

	}

	if (node != 0)
	{
		/* attach object if any */
		if (mObject != 0)
			node->attachObject(mObject);

		mNodes.push_back(node);
		/* bump index */
		mUniqueHandle ++;
	}
	return node;
}

// ----------------------------------------------------------------------------
gkLogicNode* gkLogicTree::getNode(size_t idx)
{
	if (idx >= 0 && idx < mNodes.size())
		return mNodes.at(idx);
	return 0;
}

// ----------------------------------------------------------------------------
void gkLogicTree::findToNodes(gkLogicSocket *from, gkLogicNodeBase &nodes)
{
	gkNodeBaseIterator iter= getNodeIterator();
	while (iter.hasMoreElements())
	{
		gkLogicNode *node= iter.getNext();

		gkSocketBaseIterator inputs= node->getInputs();
		while (inputs.hasMoreElements())
		{
			gkLogicSocket *isock= inputs.getNext();
			if (!isock->isLinked())
				continue;

			if (isock->getFrom() == from)
				nodes.push_back(isock->getParent());
		}
	}
}

// ----------------------------------------------------------------------------
bool has_link(gkLogicNodeBase base, gkLogicNode *node)
{
	return std::find(base.begin(), base.end(), node) != base.end();
}

// ----------------------------------------------------------------------------
bool SortLess(gkLogicNode *a, gkLogicNode *b)
{
	return a->getHandle() < b->getHandle();
}

// ----------------------------------------------------------------------------
void gkLogicTree::findSubNodeForward(gkLogicNodeBase &root, gkLogicNode *rfnode, gkLogicNode *from, gkLogicSocket *caller, bool skip)
{
	if (!from) return;

	gkSocketBaseIterator outputs= from->getOutputs();
	while (outputs.hasMoreElements())
	{
		gkLogicSocket *osock= outputs.getNext();
		if (!osock->isConnected())
			continue;

		/* Only sort calling socket. */
		if (skip && osock != caller)
			continue;


		gkLogicNodeBase outs;
		findToNodes(osock, outs);

		gkNodeBaseIterator iter= gkNodeBaseIterator(outs.begin(), outs.end());
		while (iter.hasMoreElements())
		{
			gkLogicNode *to= iter.getNext();

			if (!has_link(root, to))
			{
				root.push_back(to);
				findSubNodeForward(root, rfnode, to, 0, false);
			}
		}

		/* The calling socket has been sorted. */
		if (skip && osock == caller) return;
	}

	if (!skip)   /* only test the first 'to' node onward*/
	{
		gkSocketBaseIterator inputs= from->getInputs();
		while (inputs.hasMoreElements())
		{
			gkLogicSocket *isock= inputs.getNext();
			if (!isock->isLinked())
				continue;

			gkLogicNode *fnode= isock->getFrom()->getParent();

			if (fnode != 0)
			{
				/* stop at the first input of the calling node
					(if that makes any sense)
				*/
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

// ----------------------------------------------------------------------------
bool gkLogicTree::isConnectedIn(gkLogicNode *from, gkLogicNode *test)
{
	gkSocketBaseIterator inputs= from->getInputs();
	while (inputs.hasMoreElements())
	{
		gkLogicSocket *isock= inputs.getNext();
		if (!isock->isLinked())
			continue;

		gkSocketBaseIterator outputs= test->getOutputs();
		while (outputs.hasMoreElements())
		{
			gkLogicSocket *osock= outputs.getNext();
			if (!osock->isConnected())
				continue;

			if (isock->getFrom() == osock)
				return true;
		}
	}
	return false;
}

// ----------------------------------------------------------------------------
void gkLogicTree::pushSocketTree(gkLogicNode *from)
{
	gkSocketBaseIterator outputs= from->getOutputs();
	while (outputs.hasMoreElements())
	{
		gkLogicSocket *osock= outputs.getNext();
		if (!osock->isConnected())
			continue;

		gkLogicNodeBase root;
		findSubNodeForward(root, from, from, osock, true);

		if (root.empty())
			continue;

		std::sort(root.begin(), root.end(), SortLess);


		/* 'root' should contain all subnodes from  'osock's' to link */
		gkNodeBaseIterator iter= gkNodeBaseIterator(root.begin(), root.end());
		while (iter.hasMoreElements())
		{
			gkLogicNode *node= iter.getNext();

			/* break circular links */
			if (isConnectedIn(from, node) || node == from)
				continue;



			/* push subtree for io blocking see gkLogicSocket::block */
			if (!osock->has_link(node)) osock->push_link(node);
		}
	}
}

// ----------------------------------------------------------------------------
void gkLogicTree::solveOrder()
{
	/*
		Need to store sub-trees from
		each socket so io blocking can
		take place

		So, for each connected socket
		store direct and indirect links
		from that socket
	*/

	gkNodeBaseIterator iter= getNodeIterator();
	while (iter.hasMoreElements())
	{
		gkLogicNode *node= iter.getNext();
		pushSocketTree(node);
	}
}

// ----------------------------------------------------------------------------
void gkLogicTree::execute(Real tick)
{

	if (!mIsInitialized)
	{
		/* first execution */
		gkNodeBaseIterator iter= getNodeIterator();
		while (iter.hasMoreElements())
		{
			gkLogicNode *node= iter.getNext();
			node->_initialize();
		}

		mIsInitialized= true;
	}


	gkNodeBaseIterator iter= getNodeIterator();
	while (iter.hasMoreElements())
	{
		gkLogicNode *node= iter.getNext();
		if (!node->isBlocked() && node->evaluate(tick))
		{
			/* can continue */
			node->update(tick);
		}
	}
}
