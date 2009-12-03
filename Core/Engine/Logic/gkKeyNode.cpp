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
#include "gkWindowSystem.h"
#include "gkKeyNode.h"
#include "gkEngine.h"
#include "gkLogger.h"

using namespace Ogre;



// ----------------------------------------------------------------------------
gkKeyNode::gkKeyNode(gkLogicTree *parent, size_t id) :
		gkLogicNode(parent, NT_KEY, id), mIsPressed(false), mPressed(false),
		mDelay(0), mCounter(0), mKey(KC_NONE)
{
	ADD_ISOCK(mSockets[0], this, gkLogicSocket::ST_BOOL);
	ADD_OSOCK(mSockets[1], this, gkLogicSocket::ST_BOOL);
	ADD_OSOCK(mSockets[2], this, gkLogicSocket::ST_BOOL);
	ADD_OSOCK(mSockets[3], this, gkLogicSocket::ST_BOOL);
}


// ----------------------------------------------------------------------------
bool gkKeyNode::evaluate(Real tick)
{
	if (!mSockets[0].getValueBool())
	{
		/* block outputs */
		mSockets[1].setValue(false);
		mSockets[2].setValue(false);
		mSockets[3].setValue(false);

		mSockets[1].block(true);
		mSockets[2].block(true);
		mSockets[3].block(true);

		return false;
	}

	if (mDelay != 0)
	{
		mCounter += 1;

		if (mCounter < mDelay)
		{
			mSockets[1].block(true);
			mSockets[2].block(true);
			mSockets[3].block(true);
			return false;
		}
		mCounter= 0;
	}

	mIsPressed= gkWindowSystem::getSingleton().getKeyboard().isKeyDown(mKey);
	mSockets[1].setValue(mIsPressed);

	if (mSockets[1].isBlocked())
		mSockets[1].block(false);


	if (mIsPressed && !mPressed)
	{
		mPressed= true;
		mSockets[2].setValue(true);
		mSockets[3].setValue(false);

		if (mSockets[2].isBlocked()) mSockets[2].block(false);
		if (!mSockets[3].isBlocked())mSockets[3].block(true);
	}
	else if (!mIsPressed && mPressed)
	{
		mPressed= false;
		mSockets[2].setValue(false);
		mSockets[3].setValue(true);


		if (!mSockets[2].isBlocked()) mSockets[2].block(true);
		if (mSockets[3].isBlocked())  mSockets[3].block(false);
	}
	else
	{
		mSockets[2].setValue(false);
		mSockets[3].setValue(false);
		mSockets[2].block(true);
		mSockets[3].block(true);
	}

	return false;
}
