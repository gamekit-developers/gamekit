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
	: gkResourceManager("NodeManager", "LogicNode")
{
}

gkNodeManager::~gkNodeManager()
{
	destroyAll();
}

gkResource* gkNodeManager::createImpl(const gkResourceName& name, const gkResourceHandle& handle)
{
	return new gkLogicTree(this, name, handle);
}

void gkNodeManager::notifyResourceCreatedImpl(gkResource* res)
{
	gkLogicTree* tree = static_cast<gkLogicTree*>(res);
	if (tree->getName().empty())
		m_locals.push_back(tree);
}

void gkNodeManager::notifyResourceDestroyedImpl(gkResource* res)
{
	gkLogicTree* tree = static_cast<gkLogicTree*>(res);
	if (tree->getName().empty())
		m_locals.erase(tree);
}

void gkNodeManager::notifyDestroyAllImpl()
{
	m_locals.clear();
}

void gkNodeManager::notifyDestroyGroupImpl(const gkResourceNameString &group)
{
	TreeList::Iterator iter(m_locals);

	while (iter.hasMoreElements())
	{
		gkLogicTree* tree = iter.getNext();
		if (group == tree->getResourceName().group)
			m_locals.erase(tree);
	}
}

gkLogicTree* gkNodeManager::createLogicTree(const gkString& groupName) //create normal tree
{
	gkLogicTree* tree = create<gkLogicTree>(gkResourceName("", groupName));
	return tree;
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

UT_IMPLEMENT_SINGLETON(gkNodeManager);
