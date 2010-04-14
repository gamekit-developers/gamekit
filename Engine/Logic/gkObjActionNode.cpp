/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Nestor Silveira.

    Contributor(s): none yet.
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
#include "gkObjActionNode.h"
#include "gkEngine.h"
#include "gkGameObject.h"
#include "gkRigidBody.h"
#include "btBulletDynamicsCommon.h"

using namespace Ogre;

gkObjActionNode::gkObjActionNode(gkLogicTree *parent, size_t id) 
: gkLogicNode(parent, id)
{
    ADD_ISOCK(UPDATE, false);
	ADD_ISOCK(TARGET, 0);
}

bool gkObjActionNode::evaluate(gkScalar tick)
{
	gkGameObject* pObj = GET_SOCKET_VALUE(TARGET);

	return pObj && pObj->isLoaded() && GET_SOCKET_VALUE(UPDATE);
}

////////////////////////////////////

gkRotateNode::gkRotateNode(gkLogicTree *parent, size_t id) 
: gkObjActionNode(parent, id)
{
    ADD_ISOCK(X, 0);
    ADD_ISOCK(Y, 0);
    ADD_ISOCK(Z, 0);
}

void gkRotateNode::update(gkScalar tick)
{
    gkVector3 xyz(GET_SOCKET_VALUE(X), GET_SOCKET_VALUE(Y), GET_SOCKET_VALUE(Z));

	GET_SOCKET_VALUE(TARGET)->rotate(-xyz, TRANSFORM_LOCAL);
}

////////////////////////////////////

gkLinearVelNode::gkLinearVelNode(gkLogicTree *parent, size_t id) 
: gkObjActionNode(parent, id)
{
    ADD_ISOCK(X, 0);
    ADD_ISOCK(Y, 0);
    ADD_ISOCK(Z, 0);
	ADD_OSOCK(MOTION, false);
}

bool gkLinearVelNode::evaluate(gkScalar tick)
{
	bool motion = gkObjActionNode::evaluate(tick);

	SET_SOCKET_VALUE(MOTION, motion);

	return motion;
}

void gkLinearVelNode::update(gkScalar tick)
{
    gkVector3 xyz(GET_SOCKET_VALUE(X), GET_SOCKET_VALUE(Y), GET_SOCKET_VALUE(Z));

	GET_SOCKET_VALUE(TARGET)->setLinearVelocity(xyz, TRANSFORM_LOCAL);
}

/////////////////////////////////////////

bool gkLoadNode::evaluate(gkScalar tick)
{
	gkGameObject* pObj = GET_SOCKET_VALUE(TARGET);

	return pObj && !pObj->isLoaded() && GET_SOCKET_VALUE(UPDATE);
}

void gkLoadNode::update(gkScalar tick)
{
	GET_SOCKET_VALUE(TARGET)->load();
}

/////////////////////////////////////////

void gkUnloadNode::update(gkScalar tick)
{
	GET_SOCKET_VALUE(TARGET)->unload();
}

/////////////////////////////////////////

void gkReloadNode::update(gkScalar tick)
{
	GET_SOCKET_VALUE(TARGET)->reload();
}

////////////////////////////////////

gkSetOrientationNode::gkSetOrientationNode(gkLogicTree *parent, size_t id) 
: gkObjActionNode(parent, id)
{
	ADD_ISOCK(INPUT, gkQuaternion::IDENTITY);
}

void gkSetOrientationNode::update(gkScalar tick)
{
	GET_SOCKET_VALUE(TARGET)->setOrientation(GET_SOCKET_VALUE(INPUT));
}

/////////////////////////////////////////

void gkDisableDeactivationNode::update(gkScalar tick)
{
	gkGameObject* pObj = GET_SOCKET_VALUE(TARGET);

	gkObject* pObject = pObj->getAttachedObject();

	if(pObject)
	{
		pObject->getCollisionObject()->setActivationState(DISABLE_DEACTIVATION);
	}
}