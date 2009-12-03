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
#include "gkLogicManager.h"
#include "gkLogicTree.h"


using namespace Ogre;


// ----------------------------------------------------------------------------
gkLogicManager::gkLogicManager() :
		mUniqueHandle(0), mTrees()
{
}

// ----------------------------------------------------------------------------
gkLogicManager::~gkLogicManager()
{
	clear();
}

// ----------------------------------------------------------------------------
gkLogicTree* gkLogicManager::create()
{
	/* no unique id|name so make one */

	gkLogicTree *tree= new gkLogicTree(this, mUniqueHandle);
	mTrees.insert(std::make_pair(mUniqueHandle, tree));

	/* a constant update tree */
	mLocals.push_back(tree);

	mUniqueHandle++;
	return tree;
}

// ----------------------------------------------------------------------------
gkLogicTree* gkLogicManager::create(const String &name)
{
	gkLogicTree *tree= new gkLogicTree(this, mUniqueHandle, name);
	mTrees.insert(std::make_pair(mUniqueHandle, tree));
	mUniqueHandle++;
	return tree;
}

// ----------------------------------------------------------------------------
gkLogicTree* gkLogicManager::get(size_t id)
{
	NodeTree::iterator it= mTrees.find(id);
	return it != mTrees.end() ? it->second : 0;
}

// ----------------------------------------------------------------------------
gkLogicTree* gkLogicManager::get(const String &name)
{
	/* may be a dup tree (maybe fix this) */

	NodeTree::iterator it= mTrees.begin(), end= mTrees.end();
	for (; it != end; ++it)
		if (it->second->getName() == name)
			return it->second;

	return 0;
}

// ----------------------------------------------------------------------------
gkLogicManager::TreeList gkLogicManager::get(gkGameObject* ob)
{
	/* tree list of all attached objects */
	TreeList objs;

	NodeTree::iterator it= mTrees.begin(), end= mTrees.end();
	for (; it != end; ++it)
	{
		if (it->second->getAttachedObject() == ob)
			objs.push_back(it->second);
	}

	return objs;
}

// ----------------------------------------------------------------------------
void gkLogicManager::destroy(gkLogicTree* tree)
{
	destroy(tree->getHandle());
}

// ----------------------------------------------------------------------------
void gkLogicManager::destroy(size_t handle)
{
	NodeTree::iterator it= mTrees.find(handle);
	if (it != mTrees.end())
	{
		TreeList::iterator locals= std::find(mLocals.begin(), mLocals.end(), it->second);
		if (locals != mLocals.end())
			mLocals.erase(locals);

		delete it->second;
		mTrees.erase(it);
	}

	if (mLocals.empty())
		mLocals.clear();

	if (mTrees.empty())
		mTrees.clear();
}

// ----------------------------------------------------------------------------
void gkLogicManager::update(Real tick)
{
	TreeList::iterator it= mLocals.begin(), end= mLocals.end();
	for (; it != end; ++it)
	{
		gkLogicTree *tree= (*it);

		/* test/execute initial branches */
		tree->execute(tick);
	}
}

// ----------------------------------------------------------------------------
gkLogicManager::NodeTreeIterator gkLogicManager::getIterator()
{
	return NodeTreeIterator(mTrees.begin(), mTrees.end());
}

// ----------------------------------------------------------------------------
void gkLogicManager::clear()
{
	for (gkLogicManager::NodeTree::iterator it= mTrees.begin(); it != mTrees.end(); ++it)
		delete(it->second);
	mTrees.clear();
	mLocals.clear();
}

GK_IMPLEMENT_SINGLETON(gkLogicManager);
