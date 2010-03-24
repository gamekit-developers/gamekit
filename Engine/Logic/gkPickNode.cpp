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

#include "gkPickNode.h"
#include "gkEngine.h"
#include "gkScene.h"
#include "gkDynamicsWorld.h"
#include "gkRigidBody.h"
#include "gkUtils.h"
#include "OgreRoot.h"
#include "btBulletDynamicsCommon.h"

using namespace Ogre;

gkPickNode::gkPickNode(gkLogicTree *parent, size_t id)
: gkLogicNode(parent, id),
m_pickedBody(0),
m_constraint(0),
m_oldPickingPos(0, 0, 0),
m_oldPickingDist(0),
m_angularFactor(gkVector3::ZERO)
{
	ADD_ISOCK(UPDATE, false);
	ADD_ISOCK(CREATE_PICK, false);
	ADD_ISOCK(RELEASE_PICK, false);
	ADD_ISOCK(X, 0);
	ADD_ISOCK(Y, 0);
	ADD_ISOCK(DISABLE_ROTATION, false);
	ADD_OSOCK(CAUGHT_TRUE, false);
	ADD_OSOCK(CAUGHT_FALSE, true);
}

gkPickNode::~gkPickNode()
{
	ReleasePick();
}

bool gkPickNode::evaluate(Real tick)
{
	bool enable = GET_SOCKET_VALUE(UPDATE);

	if(!enable)
	{
		ReleasePick();
	}

	return enable;
}

void gkPickNode::update(Real tick)
{
	if(GET_SOCKET_VALUE(CREATE_PICK))
	{
		CreatePick();
	}
	else if(GET_SOCKET_VALUE(RELEASE_PICK))
	{
		ReleasePick();
	}
	else
	{
		UpdatePick();
	}
}

void gkPickNode::CreatePick()
{
	ReleasePick();

	m_pickedBody = 0;

	Ogre::Ray ray = GetRay();
	
	gkVector3 hitPointWorld;
	
	m_pickedBody = gkUtils::PickBody(ray, hitPointWorld);

	if(m_pickedBody)
	{
		btRigidBody* body = m_pickedBody->getBody();

		if (!(body->isStaticObject() || body->isKinematicObject()))
		{
			m_angularFactor = gkVector3(body->getAngularFactor());

			if(GET_SOCKET_VALUE(DISABLE_ROTATION))
			{
				body->setAngularFactor(0);
			}

			body->setActivationState(DISABLE_DEACTIVATION);

			btVector3 hitPos(hitPointWorld.x, hitPointWorld.y, hitPointWorld.z);

			btVector3 localPivot = body->getCenterOfMassTransform().inverse() * hitPos;

			m_constraint = new btPoint2PointConstraint(*body, localPivot);

			static btScalar mousePickClamping = 30.f;

			m_constraint->m_setting.m_impulseClamp = mousePickClamping;

			gkScene* pScene = gkEngine::getSingleton().getActiveScene();

			GK_ASSERT(pScene);

			btDynamicsWorld* pWorld = pScene->getDynamicsWorld()->getBulletWorld();

			GK_ASSERT(pWorld);

			pWorld->addConstraint(m_constraint, false);

			Vector3 from = ray.getOrigin();
			Vector3 to = ray.getOrigin() + ray.getDirection();

			btVector3 rayFrom(from.x, from.y, from.z);
			btVector3 rayTo(to.x, to.y, to.z);

			//save mouse position for dragging
			m_oldPickingPos = rayTo;

			m_oldPickingDist = (hitPos-rayFrom).length();

			//very weak constraint for picking
			m_constraint->m_setting.m_tau = 0.1f;

			SET_SOCKET_VALUE(CAUGHT_TRUE, true);
			SET_SOCKET_VALUE(CAUGHT_FALSE, false);
		}
	}
}

void gkPickNode::ReleasePick()
{
	if(m_constraint)
	{
		gkScene* pScene = gkEngine::getSingleton().getActiveScene();

		GK_ASSERT(pScene);

		btDynamicsWorld* pWorld = pScene->getDynamicsWorld()->getBulletWorld();

		GK_ASSERT(pWorld);

		pWorld->removeConstraint(m_constraint);

		delete m_constraint;

		m_constraint = 0;

		GK_ASSERT(m_pickedBody);

		m_pickedBody->getBody()->setAngularFactor(btVector3(m_angularFactor.x, m_angularFactor.y, m_angularFactor.z));

		SET_SOCKET_VALUE(CAUGHT_TRUE, false);
		SET_SOCKET_VALUE(CAUGHT_FALSE, true);
	}
}

void gkPickNode::UpdatePick()
{
	if(m_constraint)
	{
		gkVector3 newPivotB = GetPivotPosition();

		m_constraint->setPivotB(btVector3(newPivotB.x, newPivotB.y, newPivotB.z));
	}
}

Ogre::Ray gkPickNode::GetRay()
{
	return gkUtils::CreateCameraRay(GET_SOCKET_VALUE(X), GET_SOCKET_VALUE(Y));
}

gkVector3 gkPickNode::GetPivotPosition()
{
	Ray ray = GetRay();

	gkVector3 dir(ray.getDirection());
	
	dir.normalise();

	dir *= m_oldPickingDist;

	return ray.getOrigin() + dir;

}

