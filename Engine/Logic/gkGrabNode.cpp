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

#include "gkGrabNode.h"
#include "gkRigidBody.h"
#include "gkEngine.h"
#include "gkScene.h"
#include "gkDynamicsWorld.h"
#include "gkUtils.h"
#include "OgreRoot.h"
#include "btBulletDynamicsCommon.h"

using namespace Ogre;

gkGrabNode::gkGrabNode(gkLogicTree *parent, size_t id)
: gkPickNode(parent, id),
m_target(0)
{
	ADD_ISOCK(*getThrowObject(), this, gkLogicSocket::ST_BOOL);
	ADD_ISOCK(*getTarget(), this, gkLogicSocket::ST_GAME_OBJECT);
	ADD_ISOCK(*getGrabDirection(), this, gkLogicSocket::ST_VECTOR);
	ADD_ISOCK(*getThrowVelocity(), this, gkLogicSocket::ST_VECTOR);
	ADD_ISOCK(*getOffsetPosition(), this, gkLogicSocket::ST_VECTOR);

	getThrowVelocity()->setValue(gkVector3::ZERO);
	getOffsetPosition()->setValue(gkVector3::ZERO);
}

gkGrabNode::~gkGrabNode()
{
}

bool gkGrabNode::evaluate(Real tick)
{
	m_target = getTarget()->getValueGameObject();

	if(m_target && !m_target->isLoaded())
	{
		ReleasePick();
	}

	bool enable = gkPickNode::evaluate(tick);

	return enable && m_target && m_target->isLoaded();
}

void gkGrabNode::update(Real tick)
{
	gkPickNode::update(tick);

	if(m_pickedBody)
	{
		m_pickedBody->setAngularVelocity(gkVector3::ZERO);
	}

	if(getThrowObject()->getValueBool())
	{
		ReleasePick();
		ThrowObject();
	}
}

void gkGrabNode::ThrowObject()
{
	gkVector3 vel = getThrowVelocity()->getValueVector3();

	if(m_pickedBody && vel != gkVector3::ZERO)
	{
		m_pickedBody->setLinearVelocity(m_target->getOrientation() * vel);
	}
}

Ogre::Ray gkGrabNode::GetRay() 
{
	Vector3 dir = m_target->getOrientation() * getGrabDirection()->getValueVector3();

	Ogre::Ray ray(m_target->getPosition(), dir);

	return ray;
}

gkVector3 gkGrabNode::GetPivotPosition() 
{
	gkVector3 newPivotB = gkPickNode::GetPivotPosition();

	Vector3 offsetPos = m_target->getOrientation() * getOffsetPosition()->getValueVector3();

	newPivotB += offsetPos;

 	return newPivotB;
}
