/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): silveira.nestor.
-------------------------------------------------------------------------------
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
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
#include "gkAnimationNode.h"
#include "gkLogicSocket.h"
#include "gkEntity.h"
#include "gkEngine.h"
#include "gkAction.h"
#include "gkLogger.h"

using namespace Ogre;

gkAnimationNode::gkAnimationNode(gkLogicTree *parent, size_t id) 
: gkLogicNode(parent, id)
{
	ADD_ISOCK(*getAnimName(), this, gkLogicSocket::ST_STRING);
	ADD_ISOCK(*getBlend(), this, gkLogicSocket::ST_REAL);
	ADD_ISOCK(*getTarget(), this, gkLogicSocket::ST_GAME_OBJECT);
	ADD_OSOCK(*getCurrentAnimName(), this, gkLogicSocket::ST_STRING);
	ADD_OSOCK(*getHasReachedEnd(), this, gkLogicSocket::ST_BOOL);
	ADD_OSOCK(*getNotHasReachedEnd(), this, gkLogicSocket::ST_BOOL);

	getNotHasReachedEnd()->setValue(true);
	
	getBlend()->setValue(10);
}

bool gkAnimationNode::evaluate(gkScalar tick)
{
	gkGameObject* pObj = getTarget()->getValueGameObject();

	return pObj && pObj->isLoaded() && !getAnimName()->getValueString().empty();
}

void gkAnimationNode::update(gkScalar tick)
{
	gkGameObject* pObj = getTarget()->getValueGameObject();

	GK_ASSERT(pObj->getType() == GK_ENTITY);

	gkEntity *ent = pObj->getEntity();

	gkString animName = getAnimName()->getValueString();

	gkString currentAnimName = getCurrentAnimName()->getValueString();

	if(currentAnimName != animName)
	{
		getHasReachedEnd()->setValue(false);
		getNotHasReachedEnd()->setValue(true);
	}

	getCurrentAnimName()->setValue(animName);

	if (ent->isLoaded())
	{
		ent->playAction(getAnimName()->getValueString(), getBlend()->getValueReal());

		if(!getHasReachedEnd()->getValueBool())
		{
			gkAction* pAct = ent->getActiveAction();

			GK_ASSERT(pAct);

			gkScalar time = pAct->getTimePosition();

			if(time >= pAct->getEnd())
			{
				getHasReachedEnd()->setValue(true);
				getNotHasReachedEnd()->setValue(false);
			}
		}
    }
}
