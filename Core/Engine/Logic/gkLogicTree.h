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
#ifndef _gkLogicTree_h_
#define _gkLogicTree_h_

#include "gkLogicCommon.h"
#include "gkLogicNode.h"




// ----------------------------------------------------------------------------
class gkGameObject;
class gkLogicManager;

// ----------------------------------------------------------------------------
class gkLogicTree
{
public:
	gkLogicTree(gkLogicManager* creator, size_t id, const Ogre::String &name);
	gkLogicTree(gkLogicManager* creator, size_t id);
	~gkLogicTree();
	void execute(Ogre::Real tick);

	const Ogre::String& getName() const;
	const size_t getHandle();
	gkLogicManager* getCreator();
	gkGameObject* getAttachedObject();
	bool hasNodes();

	/* only group trees have a name */
	bool isGroup();

	void attachObject(gkGameObject *ob);

	void markDirty();


	/* node access */
	gkLogicNode*		  createNode(const gkNodeTypes& nt);
	gkLogicNode*		  getNode(size_t idx);
	void				destroyNodes();
	void				freeUnused();
	void				solveOrder();

	gkNodeBaseIterator	getNodeIterator();

protected:
	void findSubNodeForward(gkLogicNodeBase &root, gkLogicNode *rfnode, gkLogicNode *from, gkLogicSocket *caller, bool skip);
	void pushSocketTree(gkLogicNode *from);

	bool isConnectedIn(gkLogicNode *from, gkLogicNode *test);
	void findToNodes(gkLogicSocket *from, gkLogicNodeBase &nodes);

	bool			mIsInitialized;
	const size_t	mHandle;
	const Ogre::String	mName;
	size_t		  mUniqueHandle;
	gkLogicManager*   mCreator;

	gkGameObject*	 mObject;
	gkLogicNodeBase		mNodes;
};

// ----------------------------------------------------------------------------
GK_INLINE gkNodeBaseIterator gkLogicTree::getNodeIterator()
{
	return gkNodeBaseIterator(mNodes.begin(), mNodes.end());
}

// ----------------------------------------------------------------------------
GK_INLINE bool gkLogicTree::isGroup()
{
	return !mName.empty();
}

// ----------------------------------------------------------------------------
GK_INLINE const Ogre::String& gkLogicTree::getName() const
{
	return mName;
}

// ----------------------------------------------------------------------------
GK_INLINE const size_t gkLogicTree::getHandle()
{
	return mHandle;
}

// ----------------------------------------------------------------------------
GK_INLINE gkLogicManager* gkLogicTree::getCreator()
{
	return mCreator;
}

// ----------------------------------------------------------------------------
GK_INLINE gkGameObject* gkLogicTree::getAttachedObject()
{
	return mObject;
}

// ----------------------------------------------------------------------------
GK_INLINE bool gkLogicTree::hasNodes()
{
	return !mNodes.empty();
}

// ----------------------------------------------------------------------------
GK_INLINE void gkLogicTree::markDirty()
{
	mIsInitialized= false;
}




#endif//_gkLogicTree_h_
