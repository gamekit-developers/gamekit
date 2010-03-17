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
#include "gkWindowSystem.h"
#include "gkSetterNode.h"
#include "gkEngine.h"
#include "gkScene.h"
#include "gkLogger.h"
#include "gkUtils.h"
#include "gkRigidBody.h"
#include "btBulletDynamicsCommon.h"
#include "OgreRoot.h"

gkSetterNode::gkSetterNode(gkLogicTree *parent, size_t id) 
: gkLogicNode(parent, id)
{
	ADD_ISOCK(*getUpdate(), this, gkLogicSocket::ST_BOOL);
}

bool gkSetterNode::evaluate(gkScalar tick)
{
	return getUpdate()->getValueBool();
}


/////////////////////////////////////////////
gkStringSetterNode::gkStringSetterNode(gkLogicTree *parent, size_t id)
	: gkSetterNode(parent, id)
{
	ADD_ISOCK(*getInput(), this, gkLogicSocket::ST_STRING);
	ADD_OSOCK(*getOutput(), this, gkLogicSocket::ST_STRING);
}

void gkStringSetterNode::update(gkScalar tick)
{
	getOutput()->setValue(getInput()->getValueString());
}

/////////////////////////////////////////////

gkObjectSetterNode::gkObjectSetterNode(gkLogicTree *parent, size_t id)
	: gkSetterNode(parent, id), m_type(NAME)
{
	ADD_ISOCK(*getInput(), this, gkLogicSocket::ST_STRING);

	ADD_ISOCK(*getX(), this, gkLogicSocket::ST_REAL);
	ADD_ISOCK(*getY(), this, gkLogicSocket::ST_REAL);

	ADD_OSOCK(*getOutput(), this, gkLogicSocket::ST_GAME_OBJECT);
	ADD_OSOCK(*getHitPoint(), this, gkLogicSocket::ST_VECTOR);
	
}

void gkObjectSetterNode::update(gkScalar tick)
{
	gkGameObject* pObj = 0;

	if(m_type == NAME)
	{
		gkScene* pScene = gkEngine::getSingleton().getActiveScene();

		pObj = pScene->getObject(getInput()->getValueString());
	}
	else
	{
		GK_ASSERT(m_type == SCREEN_XY && "Invalid type");

		Ogre::Ray ray = gkUtils::CreateCameraRay(getX()->getValueReal(), getY()->getValueReal());

		gkVector3 rayPoint;

		gkRigidBody* pBody = gkUtils::PickBody(ray, rayPoint);

		if(pBody)
		{
			getHitPoint()->setValue(rayPoint);
	
			pObj = pBody->getObject();
		}
	}

	getOutput()->setValue(pObj);
}

//////////////////////////////////////////////////////////////

gkPositionSetterNode::gkPositionSetterNode(gkLogicTree *parent, size_t id)
	: gkSetterNode(parent, id)
{
	ADD_ISOCK(*getInput(), this, gkLogicSocket::ST_GAME_OBJECT);
	ADD_OSOCK(*getOutput(), this, gkLogicSocket::ST_VECTOR);
}

void gkPositionSetterNode::update(gkScalar tick)
{
	getOutput()->setValue(getInput()->getValueGameObject()->getPosition());
}

//////////////////////////////////////////////////////////////

gkOrientationSetterNode::gkOrientationSetterNode(gkLogicTree *parent, size_t id)
	: gkSetterNode(parent, id)
{
	ADD_ISOCK(*getInput(), this, gkLogicSocket::ST_GAME_OBJECT);
	ADD_OSOCK(*getOutput(), this, gkLogicSocket::ST_QUAT);
}

void gkOrientationSetterNode::update(gkScalar tick)
{
	getOutput()->setValue(getInput()->getValueGameObject()->getOrientation());
}
