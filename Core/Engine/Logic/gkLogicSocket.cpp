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
#include "gkLogicSocket.h"
#include "gkLogicNode.h"
#include "gkLogger.h"
#include "gkConsole.h"



using namespace Ogre;


// ----------------------------------------------------------------------------
gkGameObject* gkLogicSocket::getGameObject() const
{
	if (mFrom && mFrom->mParent)
		return mFrom->mParent->getAttachedObject();
	return mParent ? mParent->getAttachedObject() : 0;
}

// ----------------------------------------------------------------------------
bool gkLogicSocket::has_link(gkLogicNode *link)
{
	if (mConnected)
		return std::find(mNodes.begin(), mNodes.end(), link) != mNodes.end();
	return false;
}

// ----------------------------------------------------------------------------
void gkLogicSocket::link(gkLogicSocket *fsock)
{
	if (!mFrom)
	{
		if (fsock)
		{
			fsock->mConnected= mConnected= true;
			mFrom= fsock;

			if (mParent)
			{
				mParent->setLinked();
				gkLogicNode *nd= fsock->getParent();
				if (nd) nd->setLinked();
			}
		}
	}

	// else error, only one from socket
}

// ----------------------------------------------------------------------------
void gkLogicSocket::push_link(gkLogicNode *link)
{
	if (mConnected)
		mNodes.push_back(link);
}

// ----------------------------------------------------------------------------
void gkLogicSocket::block(bool truth)
{
	/* block socket io */
	if (mConnected && mBlocked != truth)
	{
		mBlocked= truth;

		gkNodeBaseIterator iter= getNodeIterator();
		while (iter.hasMoreElements())
		{
			gkLogicNode *node= iter.getNext();
			node->block(mBlocked);
		}
	}
}
