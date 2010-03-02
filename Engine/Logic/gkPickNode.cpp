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
#include "gkUtils.h"
#include "OgreRoot.h"
#include "btBulletDynamicsCommon.h"

using namespace Ogre;

gkPickNode::gkPickNode(gkLogicTree *parent, size_t id)
: gkLogicNode(parent, id),
m_constraint(0),
m_hitPos(0, 0, 0),
m_oldPickingPos(0, 0, 0),
m_oldPickingDist(0)
{
	ADD_ISOCK(*getEnable(), this, gkLogicSocket::ST_BOOL);
	ADD_ISOCK(*getCreatePick(), this, gkLogicSocket::ST_BOOL);
	ADD_ISOCK(*getReleasePick(), this, gkLogicSocket::ST_BOOL);
	ADD_ISOCK(*getUpdate(), this, gkLogicSocket::ST_BOOL);

	ADD_ISOCK(*getX(), this, gkLogicSocket::ST_REAL);
	ADD_ISOCK(*getY(), this, gkLogicSocket::ST_REAL);
}

gkPickNode::~gkPickNode()
{
}

bool gkPickNode::evaluate(Real tick)
{
	bool enable = getEnable()->getValueBool();

	if(!enable)
	{
		ReleasePick();
	}

	return enable && (getCreatePick()->getValueBool() || getReleasePick()->getValueBool() || getUpdate()->getValueBool());
}

void gkPickNode::update(Real tick)
{
	if(getCreatePick()->getValueBool())
	{
		CreatePick();
	}
	else if(getReleasePick()->getValueBool())
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

	Ray ray = gkUtils::CreateCameraRay(getX()->getValueReal(), getY()->getValueReal());

	Vector3 from = ray.getOrigin();
	Vector3 to = ray.getOrigin() + ray.getDirection();

	btVector3 rayFrom(from.x, from.y, from.z);
	btVector3 rayTo(to.x, to.y, to.z);

	btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom, rayTo);
	rayCallback.m_collisionFilterGroup = btBroadphaseProxy::AllFilter;
	rayCallback.m_collisionFilterMask = btBroadphaseProxy::AllFilter;

	gkScene* pScene = gkEngine::getSingleton().getActiveScene();

	GK_ASSERT(pScene);

	btDynamicsWorld* pWorld = pScene->getDynamicsWorld()->getBulletWorld();

	GK_ASSERT(pWorld);

	pWorld->rayTest(rayFrom, rayTo, rayCallback);

	if(rayCallback.hasHit())
	{
		btRigidBody* body = btRigidBody::upcast(rayCallback.m_collisionObject);

		if(body)
		{
			if (!(body->isStaticObject() || body->isKinematicObject()))
			{
				m_pickedBody = body;

				m_pickedBody->setActivationState(DISABLE_DEACTIVATION);

				m_hitPos = rayCallback.m_hitPointWorld;

				btVector3 localPivot = body->getCenterOfMassTransform().inverse() * m_hitPos;

				m_constraint = new btPoint2PointConstraint(*body, localPivot);

				static btScalar mousePickClamping = 30.f;

				m_constraint->m_setting.m_impulseClamp = mousePickClamping;

				pWorld->addConstraint(m_constraint, false);

				//save mouse position for dragging
				m_oldPickingPos = rayTo;

				m_oldPickingDist = (m_hitPos-rayFrom).length();

				//very weak constraint for picking
				m_constraint->m_setting.m_tau = 0.1f;
			}
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
	}
}

void gkPickNode::UpdatePick()
{
	if(m_constraint)
	{
		Ray ray = gkUtils::CreateCameraRay(getX()->getValueReal(), getY()->getValueReal());

		Vector3 dir(ray.getDirection());
		
		dir.normalise();
		
		dir *= m_oldPickingDist;

		Vector3 newPivotB = ray.getOrigin() + dir;

		m_constraint->setPivotB(btVector3(newPivotB.x, newPivotB.y, newPivotB.z));
	}
}

