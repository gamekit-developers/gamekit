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
#ifndef _gkIfNode_h_
#define _gkIfNode_h_

#include "gkLogicNode.h"



// ----------------------------------------------------------------------------
class gkVariable;


// ----------------------------------------------------------------------------
typedef enum gkBoolStatement
{
	CMP_NULL=0,
	CMP_TRUE,
	CMP_FALSE,
	CMP_AND,
	CMP_OR,
	CMP_NOT,
	CMP_EQUALS,
	CMP_NOT_EQUAL,
	CMP_GREATER,
	CMP_LESS,
	CMP_GTHAN,
	CMP_LTHAN
} gkBoolStatement;



// ----------------------------------------------------------------------------
class gkIfNode : public gkLogicNode
{
public:
	gkIfNode(gkLogicTree *parent, size_t id);
	virtual ~gkIfNode() {}

	bool evaluate(Ogre::Real tick);

	void setStatement(gkBoolStatement stmt) {mStatement= stmt;}
private:
	gkLogicSocket mSockets[4];
	gkBoolStatement mStatement;
};



#endif//_gkIfNode_h_
