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

using namespace Ogre;

gkObjActionNode::gkObjActionNode(gkLogicTree *parent, size_t id) 
: gkLogicNode(parent, id)
{
    ADD_ISOCK(*getUpdate(), this, gkLogicSocket::ST_BOOL);
	ADD_ISOCK(*getTarget(), this, gkLogicSocket::ST_GAME_OBJECT);
}

bool gkObjActionNode::evaluate(gkScalar tick)
{
	gkGameObject* pObj = getTarget()->getValueGameObject();

	return pObj && pObj->isLoaded() && getUpdate()->getValueBool();
}

////////////////////////////////////

gkRotateNode::gkRotateNode(gkLogicTree *parent, size_t id) 
: gkObjActionNode(parent, id)
{
    ADD_ISOCK(*getX(), this, gkLogicSocket::ST_REAL);
    ADD_ISOCK(*getY(), this, gkLogicSocket::ST_REAL);
    ADD_ISOCK(*getZ(), this, gkLogicSocket::ST_REAL);
}

void gkRotateNode::update(gkScalar tick)
{
    gkVector3 xyz(getX()->getValueReal(), getY()->getValueReal(), getZ()->getValueReal());

	getTarget()->getValueGameObject()->rotate(-xyz, TRANSFORM_LOCAL);
}

////////////////////////////////////

gkLinearVelNode::gkLinearVelNode(gkLogicTree *parent, size_t id) 
: gkObjActionNode(parent, id)
{
    ADD_ISOCK(*getX(), this, gkLogicSocket::ST_REAL);
    ADD_ISOCK(*getY(), this, gkLogicSocket::ST_REAL);
    ADD_ISOCK(*getZ(), this, gkLogicSocket::ST_REAL);
}

void gkLinearVelNode::update(gkScalar tick)
{
    gkVector3 xyz(getX()->getValueReal(), getY()->getValueReal(), getZ()->getValueReal());

	getTarget()->getValueGameObject()->setLinearVelocity(xyz, TRANSFORM_LOCAL);
}

/////////////////////////////////////////

bool gkLoadNode::evaluate(gkScalar tick)
{
	gkGameObject* pObj = getTarget()->getValueGameObject();

	return pObj && !pObj->isLoaded() && getUpdate()->getValueBool();
}

void gkLoadNode::update(gkScalar tick)
{
	getTarget()->getValueGameObject()->load();
}

/////////////////////////////////////////

void gkUnloadNode::update(gkScalar tick)
{
	getTarget()->getValueGameObject()->unload();
}

/////////////////////////////////////////

void gkReloadNode::update(gkScalar tick)
{
	getTarget()->getValueGameObject()->reload();
}

////////////////////////////////////

gkSetOrientationNode::gkSetOrientationNode(gkLogicTree *parent, size_t id) 
: gkObjActionNode(parent, id)
{
    ADD_ISOCK(*getInput(), this, gkLogicSocket::ST_QUAT);
}

void gkSetOrientationNode::update(gkScalar tick)
{
	getTarget()->getValueGameObject()->setOrientation(getInput()->getValueQuaternion());
}
