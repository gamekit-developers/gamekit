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
#include "gkRandomNode.h"

using namespace Ogre;


// ----------------------------------------------------------------------------
gkRandomNode::gkRandomNode(gkLogicTree *parent, size_t id) :
		gkLogicNode(parent, NT_RAND, id)
{

	ADD_ISOCK(mSockets[0], this, gkLogicSocket::ST_BOOL);
	ADD_ISOCK(mSockets[1], this, gkLogicSocket::ST_REAL);
	ADD_ISOCK(mSockets[2], this, gkLogicSocket::ST_REAL);
	ADD_OSOCK(mSockets[3], this, gkLogicSocket::ST_REAL);
}

// ----------------------------------------------------------------------------
bool gkRandomNode::evaluate(Real tick)
{
	bool result= mSockets[0].getValueBool();

	if (!result)
	{
		if (!mSockets[3].isBlocked() && mSockets[3].isConnected())
			mSockets[3].block(true);
	}
	else
		if (mSockets[3].isBlocked() && mSockets[3].isConnected())
			mSockets[3].block(false);

	return result;
}

// ----------------------------------------------------------------------------
void gkRandomNode::update(Real tick)
{
	mSockets[3].setValue(Math::RangeRandom(mSockets[1].getValueReal(), mSockets[2].getValueReal()));
}
