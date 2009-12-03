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
#ifndef _gkLogicNode_h_
#define _gkLogicNode_h_

#include "gkLogicCommon.h"
#include "gkLogicSocket.h"
#include "OgreString.h"


// ----------------------------------------------------------------------------
class gkGameObject;
class gkLogicTree;
class LogicLink;

// ----------------------------------------------------------------------------
class gkLogicNode
{
public:
	gkLogicNode(gkLogicTree *parent, const gkNodeTypes& nt, size_t handle);
	virtual ~gkLogicNode() {}

	/* do the update logic */
	virtual void update(Ogre::Real tick);

	/* see if an update is wanted */
	virtual bool evaluate(Ogre::Real tick);

	/* do init stuff */
	virtual void _initialize();

	void block(bool truth);
	bool isBlocked();


	gkLogicSocket*	getInputSocket(size_t index);
	gkLogicSocket*	getOutputSocket(size_t index);

	void			attachObject(gkGameObject *);
	gkGameObject*	 getAttachedObject();

	gkNodeTypes	   getType();
	const size_t	getHandle();
	bool			hasLinks();
	void			setLinked();

	gkSocketBaseIterator getInputs();
	gkSocketBaseIterator getOutputs();

protected:
	bool mBlocked;
	const size_t mHandle;
	gkGameObject* mObject, *mOther;
	gkNodeTypes mType;
	gkLogicTree* mParent;
	bool mHasLinks;

	gkSocketBase mInputs;
	gkSocketBase mOutputs;
};

// ----------------------------------------------------------------------------
GK_INLINE gkNodeTypes gkLogicNode::getType()
{
	return mType;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkLogicNode::attachObject(gkGameObject * ob)
{
	mObject= ob;
}

// ----------------------------------------------------------------------------
GK_INLINE gkGameObject* gkLogicNode::getAttachedObject()
{
	return mOther != 0 ? mOther : mObject;
}

// ----------------------------------------------------------------------------
GK_INLINE const size_t gkLogicNode::getHandle()
{
	return mHandle;
}

// ----------------------------------------------------------------------------
GK_INLINE bool gkLogicNode::hasLinks()
{
	return mHasLinks;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkLogicNode::setLinked()
{
	mHasLinks= true;
}

// ----------------------------------------------------------------------------
GK_INLINE bool gkLogicNode::evaluate(Ogre::Real tick)
{
	/* the default */
	return true;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkLogicNode::_initialize()
{
	/* none needed */
}

// ----------------------------------------------------------------------------
GK_INLINE bool gkLogicNode::isBlocked()
{
	return mBlocked;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkLogicNode::update(Ogre::Real tick)
{
	/* no update logic needed */
}

// ----------------------------------------------------------------------------
GK_INLINE gkSocketBaseIterator gkLogicNode::getInputs()
{
	return gkSocketBaseIterator(mInputs.begin(), mInputs.end());
}

// ----------------------------------------------------------------------------
GK_INLINE gkSocketBaseIterator gkLogicNode::getOutputs()
{
	return gkSocketBaseIterator(mOutputs.begin(), mOutputs.end());
}

// ----------------------------------------------------------------------------
GK_INLINE void gkLogicNode::block(bool truth)
{
	if (truth != mBlocked) mBlocked= truth;
}




#endif//_gkLogicNode_h_
