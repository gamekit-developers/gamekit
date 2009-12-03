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
#ifndef _gkObjectNode_h_
#define _gkObjectNode_h_

#include "gkLogicNode.h"




// ----------------------------------------------------------------------------
class gkObjectNode : public gkLogicNode
{
public:
	gkObjectNode(gkLogicTree *parent, size_t id);
	virtual ~gkObjectNode() {}
	void _initialize();

	void update(Ogre::Real tick);
	bool evaluate(Ogre::Real tick);
	void setOtherObject(const Ogre::String &name) {mOtherName= name;}
	void setFunction(gkObjectFunction f) {mFunc= f;}
private:
	gkLogicSocket mSockets[6];

	Ogre::String mOtherName;
	gkObjectFunction mFunc;
};




#endif//_gkObjectNode_h_
