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
#include "OgreAnimation.h"
#include "OgreEntity.h"
#include "OgreStringConverter.h"

#include "gkAnimationNode.h"
#include "gkLogicSocket.h"


#include "gkEntityObject.h"
#include "gkEngine.h"
#include "gkLogger.h"
#include "gkConsole.h"

using namespace Ogre;


// ----------------------------------------------------------------------------
gkAnimationNode::gkAnimationNode(gkLogicTree *parent, size_t id) :
		gkLogicNode(parent, NT_ANIM, id), mAnim(""), mFunc(AF_LOOP)
{

	ADD_ISOCK(mSockets[0], this, gkLogicSocket::ST_BOOL);
	ADD_ISOCK(mSockets[1], this, gkLogicSocket::ST_REAL);
	ADD_ISOCK(mSockets[2], this, gkLogicSocket::ST_REAL);
	ADD_ISOCK(mSockets[3], this, gkLogicSocket::ST_REAL);
	ADD_ISOCK(mSockets[4], this, gkLogicSocket::ST_REAL);
}

// ----------------------------------------------------------------------------
bool gkAnimationNode::evaluate(Real tick)
{
	return mSockets[0].getValueBool();
}

// ----------------------------------------------------------------------------
void gkAnimationNode::update(Real tick)
{
	gkGameObject *ob= mSockets[0].getGameObject();
	if (ob == 0)
		ob= mObject;

	if (ob != 0)
	{
		/* else no anim yet */
		if (ob->getType() != GK_ENTITY)
			return;


		gkEntityObject *ent= ob->getEntity();
		if (ent->isLoaded())
		{
			gkAnimProperties& props= ent->getAnimationProperties();
			props.state= mFunc == AF_LOOP ? gkAnimProperties::APS_LOOP : gkAnimProperties::APS_PLAY_STOP;
			props.anim=  mAnim;
			props.blend= mSockets[1].getValueReal();
			props.start= mSockets[2].getValueReal();
			props.end=  mSockets[3].getValueReal();
			props.speed= mSockets[4].getValueReal();

			ent->updateAnimations();
		}
	}
}
