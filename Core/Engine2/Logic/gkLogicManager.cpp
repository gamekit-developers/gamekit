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
#include "gkLogicManager.h"
#include "gkLogicTree.h"


using namespace Ogre;



gkLogicManager::gkLogicManager() :
        m_uniqueHandle(0), m_trees()
{
}


gkLogicManager::~gkLogicManager()
{
    clear();
}


gkLogicTree* gkLogicManager::create()
{
    // no unique id|name so make one

    gkLogicTree *tree = new gkLogicTree(this, m_uniqueHandle);
    m_trees.insert(std::make_pair(m_uniqueHandle, tree));

    // a constant update tree
    m_locals.push_back(tree);

    m_uniqueHandle++;
    return tree;
}


gkLogicTree* gkLogicManager::create(const gkString &name)
{
    gkLogicTree *tree = new gkLogicTree(this, m_uniqueHandle, name);
    m_trees.insert(std::make_pair(m_uniqueHandle, tree));
    m_uniqueHandle++;
    return tree;
}


gkLogicTree* gkLogicManager::get(size_t id)
{
    NodeTree::iterator it = m_trees.find(id);
    return it != m_trees.end() ? it->second : 0;
}


gkLogicTree* gkLogicManager::get(const gkString &name)
{
    // may be a dup tree
    NodeTree::iterator it = m_trees.begin(), end = m_trees.end();
    for (; it != end; ++it)
        if (it->second->getName() == name)
            return it->second;

    return 0;
}


gkLogicManager::TreeList gkLogicManager::get(gkGameObject* ob)
{
    // tree list of all attached objects
    TreeList objs;

    NodeTree::iterator it = m_trees.begin(), end = m_trees.end();
    for (; it != end; ++it)
    {
        if (it->second->getAttachedObject() == ob)
            objs.push_back(it->second);
    }

    return objs;
}


void gkLogicManager::destroy(gkLogicTree* tree)
{
    destroy(tree->getHandle());
}


void gkLogicManager::destroy(size_t handle)
{
    NodeTree::iterator it = m_trees.find(handle);
    if (it != m_trees.end())
    {
        TreeList::iterator locals = std::find(m_locals.begin(), m_locals.end(), it->second);
        if (locals != m_locals.end())
            m_locals.erase(locals);

        delete it->second;
        m_trees.erase(it);
    }

    if (m_locals.empty())
        m_locals.clear();

    if (m_trees.empty())
        m_trees.clear();
}


void gkLogicManager::update(gkScalar tick)
{
    TreeList::iterator it = m_locals.begin(), end = m_locals.end();
    for (; it != end; ++it)
    {
        gkLogicTree *tree = (*it);

        // test/execute initial branches
        tree->execute(tick);
    }
}


gkLogicManager::NodeTreeIterator gkLogicManager::getIterator()
{
    return NodeTreeIterator(m_trees.begin(), m_trees.end());
}


void gkLogicManager::clear()
{
    for (gkLogicManager::NodeTree::iterator it = m_trees.begin(); it != m_trees.end(); ++it)
        delete(it->second);
    m_trees.clear();
    m_locals.clear();
}

GK_IMPLEMENT_SINGLETON(gkLogicManager);
