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
	ADD_ISOCK(GROUND_OFFSET, 0.1f);
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
	if(GET_SOCKET_VALUE(ENABLE))
	{
		Ogre::AxisAlignedBox bb = m_object->getAabb();

		gkScalar minZ = bb.getHalfSize().z + GET_SOCKET_VALUE(GROUND_OFFSET);

		gkVector3 origin = m_object->getPosition();

		static const int N = 9;
		
		gkVector3 dir[N];
		
		dir[0] = gkVector3(0, 0, -minZ);
		dir[1] = gkVector3(0, minZ, -minZ);
		dir[2] = gkVector3(0, -minZ, -minZ);
		dir[3] = gkVector3(minZ, 0, -minZ);
		dir[4] = gkVector3(-minZ, 0, -minZ);
		dir[5] = gkVector3(minZ, minZ, -minZ);
		dir[6] = gkVector3(minZ, -minZ, -minZ);
		dir[7] = gkVector3(-minZ, minZ, -minZ);
		dir[8] = gkVector3(-minZ, -minZ, -minZ);

		int i=0;
		for(i; i<N; i++)
		{
			Ogre::Ray ray(origin, dir[i]);
			
			gkVector3 hitPointWorld;
			
			btCollisionObject* pCol = gkUtils::PickBody(ray, hitPointWorld);

			if(pCol)
			{
				gkObject* pObj = static_cast<gkObject*>(pCol->getUserPointer());

				SET_SOCKET_VALUE(CONTACT_POSITION, hitPointWorld);
				SET_SOCKET_VALUE(COLLIDED_OBJ, pObj->getObject());
				SET_SOCKET_VALUE(FALLING, false);
				SET_SOCKET_VALUE(NOT_FALLING, true);

				break;
			}
		}

		if(i == N)
		{
			SET_SOCKET_VALUE(FALLING, true);
			SET_SOCKET_VALUE(NOT_FALLING, false);
		}
	}
	else
	{
		SET_SOCKET_VALUE(FALLING, false);
		SET_SOCKET_VALUE(NOT_FALLING, false);
	}
}
