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
#include "gkFallTestNode.h"
#include "gkLogger.h"
#include "gkUtils.h"
#include "gkGameObject.h"
#include "Ogre.h"
#include "btBulletDynamicsCommon.h"

gkFallTestNode::gkFallTestNode(gkLogicTree *parent, size_t id) 
: gkLogicNode(parent, id)
{
	ADD_ISOCK(ENABLE, false);
	ADD_ISOCK(TARGET, 0);
	ADD_ISOCK(GROUND_OFFSET, 0.08f);
	ADD_ISOCK(COLLIDES_WITH, "");
	ADD_OSOCK(FALLING, false);
	ADD_OSOCK(NOT_FALLING, false);
	ADD_OSOCK(COLLIDED_OBJ, 0);
	ADD_OSOCK(CONTACT_POSITION, gkVector3::ZERO);
}

bool gkFallTestNode::evaluate(gkScalar tick)
{
	m_object = GET_SOCKET_VALUE(TARGET);

	if(m_object && !m_object->isLoaded())
	{
		m_object = 0;
	}

	return m_object != 0;
}

void gkFallTestNode::update(gkScalar tick)
{
	SET_SOCKET_VALUE(FALLING, true);
	SET_SOCKET_VALUE(NOT_FALLING, false);

	if(GET_SOCKET_VALUE(ENABLE))
	{
		Ogre::AxisAlignedBox bb = m_object->getAabb();

		gkScalar minZ = bb.getHalfSize().z + GET_SOCKET_VALUE(GROUND_OFFSET);

		gkVector3 position = m_object->getPosition();

		gkVector3 origin = position - gkVector3(0, 0, minZ);

		gkVector3 dir = position - origin;

		Ogre::Ray ray(origin, dir);
		
		gkVector3 hitPointWorld;
		
		btCollisionObject* pCol = gkUtils::PickBody(ray, hitPointWorld);

		if(pCol)
		{
			gkObject* pObj = static_cast<gkObject*>(pCol->getUserPointer());

			if(pObj != m_object)
			{
				SET_SOCKET_VALUE(CONTACT_POSITION, hitPointWorld);
				SET_SOCKET_VALUE(COLLIDED_OBJ, pObj->getObject());
				SET_SOCKET_VALUE(FALLING, false);
				SET_SOCKET_VALUE(NOT_FALLING, true);
			}
		}
	}
}
