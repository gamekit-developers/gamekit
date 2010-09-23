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

#include "btBulletDynamicsCommon.h"
#include "gkGrabNode.h"
#include "gkRigidBody.h"
#include "gkEngine.h"
#include "gkScene.h"
#include "gkDynamicsWorld.h"
#include "gkGameObject.h"
#include "gkUtils.h"
#include "OgreRoot.h"

gkGrabNode::gkGrabNode(gkLogicTree* parent, size_t id)
	: gkPickNode(parent, id),
	  m_target(0)
{
	ADD_ISOCK(THROW_OBJECT, false);
	ADD_ISOCK(TARGET, 0);
	ADD_ISOCK(GRAB_DIRECTION, gkVector3::ZERO);
	ADD_ISOCK(THROW_VEL, gkVector3::ZERO);
	ADD_ISOCK(RELATED_OFFSET_POSITION, gkVector3::ZERO);
	ADD_OSOCK(THROWED, false);
}

gkGrabNode::~gkGrabNode()
{
}

bool gkGrabNode::evaluate(gkScalar tick)
{
	m_target = GET_SOCKET_VALUE(TARGET);

	if (m_target && !m_target->isInstanced())
	{
		ReleasePick();
	}

	if (GET_SOCKET_VALUE(THROWED))
	{
		SET_SOCKET_VALUE(THROWED, false);
	}

	bool enable = gkPickNode::evaluate(tick);

	return enable && m_target && m_target->isInstanced();
}

void gkGrabNode::update(gkScalar tick)
{
	gkPickNode::update(tick);

	if (GET_SOCKET_VALUE(THROW_OBJECT))
	{
		ThrowObject();
	}
}

void gkGrabNode::ThrowObject()
{
	gkVector3 vel = GET_SOCKET_VALUE(THROW_VEL);

	if (m_pickedBody && vel != gkVector3::ZERO)
	{
		m_pickedBody->setLinearVelocity(m_target->getOrientation() * vel);

		ReleasePick();

		SET_SOCKET_VALUE(THROWED, true);
	}
}

Ogre::Ray gkGrabNode::GetRay()
{
	gkVector3 dir = m_target->getOrientation() * GET_SOCKET_VALUE(GRAB_DIRECTION);

	Ogre::Ray ray(m_target->getPosition(), dir);

	return ray;
}

gkVector3 gkGrabNode::GetPivotPosition()
{
	gkVector3 newPivotB = gkPickNode::GetPivotPosition();

	gkVector3 offsetPos = m_target->getOrientation() * GET_SOCKET_VALUE(RELATED_OFFSET_POSITION);

	newPivotB += offsetPos;

	return newPivotB;
}
