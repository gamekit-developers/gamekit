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
#include "OgreRoot.h"
#include "gkWindowSystem.h"
#include "gkSetterNode.h"
#include "gkEngine.h"
#include "gkScene.h"
#include "gkLogger.h"
#include "gkUtils.h"
#include "gkRigidBody.h"
#include "gkGameObject.h"
#include "btBulletDynamicsCommon.h"


gkSetterNode::gkSetterNode(gkLogicTree *parent, size_t id) 
: gkLogicNode(parent, id)
{
	ADD_ISOCK(UPDATE, false);
}

bool gkSetterNode::evaluate(gkScalar tick)
{
	return GET_SOCKET_VALUE(UPDATE);
}


/////////////////////////////////////////////
gkStringSetterNode::gkStringSetterNode(gkLogicTree *parent, size_t id)
	: gkSetterNode(parent, id)
{
	ADD_ISOCK(INPUT, "");
	ADD_OSOCK(OUTPUT, "");
}

void gkStringSetterNode::update(gkScalar tick)
{
	SET_SOCKET_VALUE(OUTPUT, GET_SOCKET_VALUE(INPUT));
}

/////////////////////////////////////////////

gkObjectSetterNode::gkObjectSetterNode(gkLogicTree *parent, size_t id)
	: gkSetterNode(parent, id), m_type(NAME)
{
	ADD_ISOCK(INPUT, "");
	ADD_OSOCK(OUTPUT, 0);
	ADD_ISOCK(X, 0);
	ADD_ISOCK(Y, 0);
	ADD_ISOCK(RESET, false);
	ADD_OSOCK(HAS_OBJ, false);
	ADD_OSOCK(HIT_POINT, gkVector3::ZERO);
}

void gkObjectSetterNode::update(gkScalar tick)
{
	gkGameObject* pObj = 0;

	if(!GET_SOCKET_VALUE(RESET))
	{
		if(m_type == NAME)
		{
			gkScene* pScene = gkEngine::getSingleton().getActiveScene();

			pObj = pScene->getObject(GET_SOCKET_VALUE(INPUT));
		}
		else
		{
			GK_ASSERT(m_type == SCREEN_XY && "Invalid type");

			Ogre::Ray ray = gkUtils::CreateCameraRay(GET_SOCKET_VALUE(X), GET_SOCKET_VALUE(Y));

			gkVector3 rayPoint;

			btCollisionObject* pCol = gkUtils::PickBody(ray, rayPoint);

			if(pCol)
			{
				SET_SOCKET_VALUE(HIT_POINT, rayPoint);
		
				pObj = static_cast<gkObject*>(pCol->getUserPointer())->getObject();
			}
		}
	}

	SET_SOCKET_VALUE(HAS_OBJ, pObj ? true : false);
	SET_SOCKET_VALUE(OUTPUT, pObj);
}

//////////////////////////////////////////////////////////////

gkPositionSetterNode::gkPositionSetterNode(gkLogicTree *parent, size_t id)
	: gkSetterNode(parent, id)
{
	ADD_ISOCK(INPUT, 0);
	ADD_OSOCK(OUTPUT, gkVector3::ZERO);
}

bool gkPositionSetterNode::evaluate(gkScalar tick)
{
	return gkSetterNode::evaluate(tick) && GET_SOCKET_VALUE(INPUT);
}

void gkPositionSetterNode::update(gkScalar tick)
{
	SET_SOCKET_VALUE(OUTPUT, GET_SOCKET_VALUE(INPUT)->getPosition());
}

//////////////////////////////////////////////////////////////

gkOrientationSetterNode::gkOrientationSetterNode(gkLogicTree *parent, size_t id)
	: gkSetterNode(parent, id)
{
	ADD_ISOCK(INPUT, 0);
	ADD_OSOCK(OUTPUT, gkQuaternion::IDENTITY);
}

bool gkOrientationSetterNode::evaluate(gkScalar tick)
{
	return gkSetterNode::evaluate(tick) && GET_SOCKET_VALUE(INPUT);
}

void gkOrientationSetterNode::update(gkScalar tick)
{
	SET_SOCKET_VALUE(OUTPUT, GET_SOCKET_VALUE(INPUT)->getOrientation());
}
