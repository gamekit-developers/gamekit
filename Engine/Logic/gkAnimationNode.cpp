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
	ADD_ISOCK(ANIM_NAME, "");
	ADD_ISOCK(BLEND_FRAMES, 0);
	ADD_ISOCK(TARGET, 0);
	ADD_OSOCK(CURRENT_ANIM_NAME, "");
	ADD_OSOCK(HAS_REACHED_END, false);
	ADD_OSOCK(NOT_HAS_REACHED_END, false);

	SET_SOCKET_VALUE(NOT_HAS_REACHED_END, true);
	SET_SOCKET_VALUE(BLEND_FRAMES, 10);
}

bool gkAnimationNode::evaluate(gkScalar tick)
{
	gkGameObject* pObj = GET_SOCKET_VALUE(TARGET);

	return pObj && pObj->isLoaded() && !GET_SOCKET_VALUE(ANIM_NAME).empty();
}

void gkAnimationNode::update(gkScalar tick)
{
	gkGameObject* pObj = GET_SOCKET_VALUE(TARGET);

	GK_ASSERT(pObj->getType() == GK_ENTITY);

	gkEntity *ent = pObj->getEntity();

	gkString animName = GET_SOCKET_VALUE(ANIM_NAME);

	gkString currentAnimName = GET_SOCKET_VALUE(CURRENT_ANIM_NAME);

	if(currentAnimName != animName)
	{
		SET_SOCKET_VALUE(HAS_REACHED_END, false);
		SET_SOCKET_VALUE(NOT_HAS_REACHED_END, true);
	}

	SET_SOCKET_VALUE(CURRENT_ANIM_NAME, animName);

	if (ent->isLoaded())
	{
		ent->playAction(GET_SOCKET_VALUE(ANIM_NAME), GET_SOCKET_VALUE(BLEND_FRAMES));

		if(!GET_SOCKET_VALUE(HAS_REACHED_END))
		{
			gkAction* pAct = ent->getActiveAction();

			GK_ASSERT(pAct);

			gkScalar time = pAct->getTimePosition();

			if(time >= pAct->getEnd())
			{
				SET_SOCKET_VALUE(HAS_REACHED_END, true);
				SET_SOCKET_VALUE(NOT_HAS_REACHED_END, false);
			}
		}
    }
}
