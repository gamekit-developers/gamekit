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
#ifndef _gkMotionNode_h_
#define _gkMotionNode_h_

#include "gkLogicNode.h"



// ----------------------------------------------------------------------------
class gkMotionNode : public gkLogicNode
{
public:
	gkMotionNode(gkLogicTree *parent, size_t id);
	virtual ~gkMotionNode() {}
	void _initialize();

	void update(Ogre::Real tick);
	bool evaluate(Ogre::Real tick);

	void setMotionType(gkMotionTypes t) {mMotionType= t;}
	void setSpace(int ts) {mSpace= ts;}

	void keepVelocity() {mKeep= true;}

	void setMinX(Ogre::Real v);
	void setMaxX(Ogre::Real v);

	void setMinY(Ogre::Real v);
	void setMaxY(Ogre::Real v);

	void setMinZ(Ogre::Real v);
	void setMaxZ(Ogre::Real v);

	void setOtherObject(const Ogre::String &obname) {mOtherObject= obname;}
private:
	gkLogicSocket mSockets[5];
	gkMotionTypes mMotionType;
	int mSpace;
	bool mKeep;

	int getLRS();

	void applyConstraints(int lrs);
	void applyObject(Ogre::Vector3 &vec);

	short mFlag;
	Ogre::Real x[2], y[2], z[2];

	gkGameObject *mCurrent;
	Ogre::String mOtherObject;
};

// ----------------------------------------------------------------------------
GK_INLINE void gkMotionNode::setMinX(Ogre::Real v)
{
	mFlag |= 1;
	x[0]= v;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkMotionNode::setMaxX(Ogre::Real v)
{
	mFlag |= 1;
	x[1]= v;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkMotionNode::setMinY(Ogre::Real v)
{
	mFlag |= 2;
	y[0]= v;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkMotionNode::setMaxY(Ogre::Real v)
{
	mFlag |= 2;
	y[1]= v;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkMotionNode::setMinZ(Ogre::Real v)
{
	mFlag |= 4;
	z[0]= v;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkMotionNode::setMaxZ(Ogre::Real v)
{
	mFlag |= 4;
	z[1]= v;
}





#endif//_gkMotionNode_h_
