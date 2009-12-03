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
#ifndef _gkVariableNode_h_
#define _gkVariableNode_h_

#include "gkLogicNode.h"



// ----------------------------------------------------------------------------
class gkVariable;

// ----------------------------------------------------------------------------
class gkVariableNode : public gkLogicNode
{
public:
	gkVariableNode(gkLogicTree *parent, size_t id);
	virtual ~gkVariableNode() {}

	bool evaluate(Ogre::Real tick);
	void _initialize();

	void setName(const Ogre::String &varname) {mVarName= varname;}
	void setDebug() {mDebug= true;}
private:
	gkLogicSocket mSockets[3];
	Ogre::String mVarName;
	gkVariable *mProp;
	bool mDebug;
};

// ----------------------------------------------------------------------------
class VariableOpNode : public gkLogicNode
{
public:
	VariableOpNode(gkLogicTree *parent, size_t id);
	virtual ~VariableOpNode() {}

	bool evaluate(Ogre::Real tick);
	void _initialize();

	void setFunction(int func) {mFunction= func;}
	void setName(const Ogre::String &varname) {mVarName= varname;}
	void setToDegree(bool v) {mDeg= v;}
private:
	gkLogicSocket mSockets[3];
	int mFunction;
	Ogre::String mVarName;
	bool mDeg;

	gkVariable *mProp;
};




#endif//_gkVariableNode_h_
