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
#include "gkSwitchNode.h"


using namespace Ogre;


// ----------------------------------------------------------------------------
gkSwitchNode::gkSwitchNode(gkLogicTree *parent, size_t id) :
		gkLogicNode(parent, NT_SWITCH, id), mLabels(0)
{
	ADD_ISOCK(mSockets[0],  this, gkLogicSocket::ST_BOOL);
	ADD_ISOCK(mSockets[1],  this, gkLogicSocket::ST_REAL);

	for (int i=START_IN; i<START_OT; i++)
		ADD_ISOCK(mSockets[i], this, gkLogicSocket::ST_REAL);

	for (int i=START_OT; i<CASE_MAX; i++)
		ADD_OSOCK(mSockets[i], this, gkLogicSocket::ST_BOOL);
}

// ----------------------------------------------------------------------------
bool gkSwitchNode::evaluate(Real tick)
{
	bool res= mSockets[0].getValueBool();
	if (!res)   _doBlocks(-1);
	else		_doBlocks(-2);
	return res;
}

// ----------------------------------------------------------------------------
void gkSwitchNode::_initialize()
{
	for (int i=START_OT; i<CASE_MAX; i++)
	{
		mSockets[i].setValue(false);
		mSockets[i].block(true);
	}
}

// ----------------------------------------------------------------------------
void gkSwitchNode::update(Real tick)
{
	Real swtch= mSockets[1].getValueReal();

	int ltest= 0;
	int offs= 1;
	for (int i=START_IN; i<START_OT && ltest < (mLabels); i++, ltest++, offs++)
	{
		/* test label */
		if (swtch == mSockets[i].getValueReal())
		{
			_doBlocks(START_OT + offs);
			return;
		}
	}

	/* default label */
	if (mSockets[START_OT].isConnected())
		_doBlocks(START_OT);
}

// ----------------------------------------------------------------------------
void gkSwitchNode::_doBlocks(int idx)
{
	for (int i=START_OT; i<CASE_MAX; i++)
	{
		if (idx == -1)
		{
			mSockets[i].block(true);
			mSockets[i].setValue(false);
			continue;
		}
		else if (idx == -2)
		{
			mSockets[i].block(false);
			mSockets[i].setValue(false);
			continue;
		}

		if (i == idx && mSockets[i].isConnected())
		{
			mSockets[i].block(false);
			mSockets[i].setValue(true);
		}
		else
		{
			if (mSockets[i].isConnected())
			{
				mSockets[i].block(true);
				mSockets[i].setValue(false);
			}
		}
	}
}
