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
#include "gkGroupNode.h"
#include "gkLogicTree.h"

using namespace Ogre;


// ----------------------------------------------------------------------------
gkGroupNode::gkGroupNode(gkLogicTree *parent, size_t id) :
		gkLogicNode(parent, NT_GROUP, id)
{
}

// ----------------------------------------------------------------------------
gkGroupNode::~gkGroupNode()
{
	gkSocketAlias::iterator it;

	it= mInpAlias.begin();
	while (it != mInpAlias.end())
	{
		delete it->second;
		++it;
	}

	it= mOutAlias.begin();
	while (it != mOutAlias.end())
	{
		delete it->second;
		++it;
	}
}

// ----------------------------------------------------------------------------
void gkGroupNode::update(Real tick)
{
	if (mGroup)
	{
		if (mGroup->getAttachedObject() != mObject)
			mGroup->attachObject(mObject);

		/* set the alias value */
		{
			gkSocketAlias::iterator it, end;
			it=  mInpAlias.begin();
			end= mInpAlias.end();
			while (it != end)
			{
				it->first->copy(*it->second);
				++it;
			}

			/* XXX, fix this out's after exec */
			it=  mOutAlias.begin();
			end= mOutAlias.end();
			while (it != end)
			{
				it->first->copy(*it->second);
				++it;
			}
		}

		/* execute tree */
		mGroup->execute(tick);
	}
}

// ----------------------------------------------------------------------------
void gkGroupNode::pushInput(gkLogicSocket *sock)
{
	if (sock != 0)
	{
		gkLogicSocket *ret= new gkLogicSocket();
		ret->copy(*sock);

		mInpAlias.insert(std::make_pair(sock, ret));
		mInputs.push_back(ret);
	}
}

// ----------------------------------------------------------------------------
void gkGroupNode::pushOutput(gkLogicSocket *sock)
{
	if (sock != 0)
	{
		gkLogicSocket *ret= new gkLogicSocket();
		ret->copy(*sock);

		mOutAlias.insert(std::make_pair(sock, ret));
		mOutputs.push_back(ret);
	}
}
