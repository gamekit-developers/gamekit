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
#include "gkNodeManager.h"
#include "gkLogicTree.h"


gkNodeManager::gkNodeManager()
	:       m_uniqueHandle(0)
{
}

gkNodeManager::~gkNodeManager()
{
	clear();
}

gkLogicTree* gkNodeManager::create()
{
	// this is a normal tree
	gkLogicTree* tree = new gkLogicTree(this, m_uniqueHandle);
	m_trees.insert(m_uniqueHandle, tree);

	m_locals.push_back(tree);
	m_uniqueHandle++;
	return tree;
}


gkLogicTree* gkNodeManager::create(const gkString& name)
{
	// this is a group tree
	gkLogicTree* tree = new gkLogicTree(this, m_uniqueHandle, name);
	m_trees.insert(m_uniqueHandle, tree);

	m_uniqueHandle++;
	return tree;
}

gkLogicTree* gkNodeManager::get(int id)
{
	UTsize pos = m_trees.find(id);
	if (pos != UT_NPOS)
		return m_trees.at(pos);
	return 0;
}

gkLogicTree* gkNodeManager::get(const gkString& name)
{
	NodeTreeIterator iter(m_trees);
	while (iter.hasMoreElements())
	{
		NodeTreeIterator::Pair kv = iter.getNext();
		if (kv.second->getName() == name)
			return kv.second;
	}
	return 0;
}

void gkNodeManager::destroy(gkLogicTree* tree)
{
	if (tree)
		destroy(tree->getHandle());
}

void gkNodeManager::destroy(int handle)
{
	UTsize pos = m_trees.find(handle);
	if (pos != UT_NPOS)
	{
		gkLogicTree* ltree = m_trees.at(pos);

		TreeList::Pointer node = m_locals.find(ltree);
		if (node)
			m_locals.erase(node);

		delete ltree;
		m_trees.remove(handle);
	}
}


void gkNodeManager::update(gkScalar tick)
{
	// update all non group trees
	if (!m_locals.empty())
	{
		utListIterator<TreeList> iter(m_locals);
		while (iter.hasMoreElements())
			iter.getNext()->execute(tick);
	}
}

void gkNodeManager::clear()
{
	NodeTreeIterator iter(m_trees);
	while (iter.hasMoreElements())
		delete iter.getNext().second;
	m_trees.clear();
	m_locals.clear();
}

UT_IMPLEMENT_SINGLETON(gkNodeManager);
