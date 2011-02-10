/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): Nestor Silveira.
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

gkLogicTree::gkLogicTree(gkResourceManager *creator, const gkResourceName &name, const gkResourceHandle &handle)
	:	gkResource(creator, name, handle),
		m_object(0),
		m_initialized(false), 
		m_sorted(false)
{
}


gkLogicTree::~gkLogicTree()
{
	destroyNodes();
}


void gkLogicTree::attachObject(gkGameObject* ob)
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

	typedef std::deque<gkLogicNode*> QUEUE;

	void solve(gkLogicTree* tree)
	{
		QUEUE l;
		gkLogicTree::NodeIterator iter = tree->getNodeIterator();
		while (iter.hasMoreElements())
			setPriority(tree, iter.getNext(), l);
	}

	void setPriority(gkLogicTree* tree, gkLogicNode* node, QUEUE& l)
	{
		// setting priority + 1 for each connection

		GK_ASSERT(node);

		QUEUE::const_iterator it = std::find(l.begin(), l.end(), node);
		if (it != l.end()) return;

		l.push_back(node);

		gkLogicNode::Sockets& sockets = node->getInputs();
		if (!sockets.empty())
		{
			gkLogicNode::SocketIterator sockit(sockets);
			while (sockit.hasMoreElements())
			{
				gkILogicSocket* sock = sockit.getNext();
				if (sock->isLinked())
				{
					gkILogicSocket* fsock = sock->getFrom();

					GK_ASSERT(fsock);

					gkLogicTree::NodeIterator iter = tree->getNodeIterator();
					while (iter.hasMoreElements())
					{
						gkLogicNode* onode = iter.getNext();
						if (onode != node && onode == fsock->getParent())
						{
							if (onode->getPriority() <= node->getPriority())
							{
								onode->setPriority(node->getPriority() + 1);
								setPriority(tree, onode, l);
							}
						}
					}
				}
			}
		}

		l.pop_back();
	}
};


#define NT_DUMP_ORDER 0

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
	FILE* fp = fopen("NodeTree_dump.txt", "wb");

	fprintf(fp, "--- node order before sort ---\n");
	NodeIterator iter(m_nodes);
	while (iter.hasMoreElements())
	{
		gkLogicNode* lnode = iter.getNext();
		fprintf(fp, "%s:%i\n", (typeid(*lnode).name()), lnode->getPriority());
	}

	m_nodes.sort(gkLogicSolver::sort);


	fprintf(fp, "--- node order after sort ---\n");
	NodeIterator iter2(m_nodes);
	while (iter2.hasMoreElements())
	{
		gkLogicNode* lnode = iter2.getNext();
		fprintf(fp, "%s:%i\n", (typeid(*lnode).name()), lnode->getPriority());
	}
	fclose(fp);
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
		gkLogicNode* node = iter.getNext();
		// can continue
		if (node->evaluate(tick))
			node->update(tick);
	}
}
