/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): Nestor Silveira.
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
#include "gkDynamicsWorld.h"
#include "gkGameObject.h"
#include "gkRigidBody.h"
#include "gkScene.h"

#include "btBulletDynamicsCommon.h"




gkRigidBody::gkRigidBody(gkGameObject* object, gkDynamicsWorld* owner)
	:    gkPhysicsController(object, owner),
	     m_body(0),
	     m_oldActivationState(-1)
{
}



gkRigidBody::~gkRigidBody()
{
	UTsize i;	
	for (i = 0; i < m_constraints.size(); i++)
	{		
		delete m_constraints[i];
	}
	m_constraints.clear();

	delete m_body;
	m_body = 0;

	m_collisionObject = 0;
}



btRigidBody* gkRigidBody::getBody(void)
{
	return m_body;
}



void gkRigidBody::create(void)
{
	if (m_body || m_collisionObject)
		return;

	GK_ASSERT(m_object && m_object->isInstanced() && m_object->isInActiveLayer());

	createShape();
	if (!m_shape)
		return;


	const gkGameObjectProperties& props = m_object->getProperties();
	const gkPhysicsProperties& phy = props.m_physics;

	// use the most up to date transform.
	const gkTransformState& tstate = m_object->getTransformState();


	if (phy.isRigidOrDynamic())
	{
		btVector3 inertia;
		m_shape->calculateLocalInertia(phy.m_mass, inertia);

		m_body = new btRigidBody(phy.m_mass, this, m_shape, inertia);

		m_body->setLinearFactor(
		    btVector3(
		        (phy.m_mode & GK_LOCK_LINV_X) ? 0.f : 1.f,
		        (phy.m_mode & GK_LOCK_LINV_Y) ? 0.f : 1.f,
		        (phy.m_mode & GK_LOCK_LINV_Z) ? 0.f : 1.f
		    ));

		if (phy.isDynamic())
			m_body->setAngularFactor(0.f);
		else
		{
			m_body->setAngularFactor(
			    btVector3(
			        (phy.m_mode & GK_LOCK_ANGV_X) ? 0.f : 1.f,
			        (phy.m_mode & GK_LOCK_ANGV_Y) ? 0.f : 1.f,
			        (phy.m_mode & GK_LOCK_ANGV_Z) ? 0.f : 1.f
			    ));
		}

		m_body->setDamping(phy.m_linearDamp, phy.m_angularDamp);
	}
	else
	{
		m_body = new btRigidBody(0.f, 0, m_shape, btVector3(0, 0, 0));
		m_body->setDamping(0.f, 0.f);
		m_body->setAngularFactor(0.f);
	}

	btDynamicsWorld* dyn = getOwner();
	m_body->setUserPointer(this);

	m_body->setFriction(phy.m_friction);
	m_body->setRestitution(phy.m_restitution);
	m_body->setWorldTransform(tstate.toTransform());

	if (!phy.isDosser())
		m_body->setActivationState(m_body->getActivationState() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

	m_collisionObject = m_body;
	dyn->addRigidBody(m_body);
}

void gkRigidBody::removeConstaints(void)
{
	btDynamicsWorld* dyn = getOwner();

	UTsize i;
	for (i = 0; i < m_constraints.size(); i++)
	{
		dyn->removeConstraint(m_constraints[i]);
		delete m_constraints[i];
	}
	m_constraints.clear();
}

void gkRigidBody::destroy(void)
{
	btDynamicsWorld* dyn = getOwner();

	removeConstaints();

	if (m_body)
	{
		// intertwine
		m_body->setUserPointer(0);
		m_body->setMotionState(0);

		if (!m_suspend)
			dyn->removeRigidBody(m_body);

		delete m_shape;
		m_shape = 0;

		delete m_body;
		m_body = 0;

		m_collisionObject = 0;
	}	
}

void gkRigidBody::addConstraint(btTypedConstraint* constraint, bool disableLinkedCollision)
{
	GK_ASSERT(constraint && m_constraints.find(constraint) == UT_NPOS);

	getOwner()->addConstraint(constraint, disableLinkedCollision);
	m_constraints.push_back(constraint);
}

void gkRigidBody::removeConstraint(btTypedConstraint* constraint)
{
	GK_ASSERT(constraint && m_constraints.find(constraint) != UT_NPOS);

	m_constraints.erase(m_constraints.find(constraint));
}


void gkRigidBody::createConstraints(void)
{
	gkGameObject* obj = getObject();
	gkScene* scene = obj->getOwner();

	gkGameObjectProperties&  props  = obj->getProperties();

	UTsize i;
	for (i = 0; i < props.m_physics.m_constraints.size(); i++)
	{
		gkPhysicsConstraintProperties& cprops = props.m_physics.m_constraints[i];

		btRigidBody* rbA = getBody();
		btRigidBody* rbB = 0;

		if (!cprops.m_target.empty())
		{
			gkGameObject* target = scene->findInstancedObject(cprops.m_target);

			GK_ASSERT(target && target->getAttachedBody());

			rbB = target->getAttachedBody()->getBody();
		}

		btTypedConstraint* constraint = m_owner->createConstraint(rbA, rbB, cprops);

		if (constraint)
			addConstraint(constraint, cprops.m_disableLinkedCollision);

	}
}


void gkRigidBody::setLinearVelocity(const gkVector3& v, int tspace)
{
	if (m_suspend) // block
		return;

	if (!m_body)
		return;

	// only dynamic bodies
	if (m_body->isStaticOrKinematicObject())
		return;

	if (v.squaredLength() > GK_EPSILON * GK_EPSILON)
		m_body->activate();

	gkVector3 vel;
	switch (tspace)
	{
	case TRANSFORM_LOCAL:
		{
			btTransform trans = m_body->getCenterOfMassTransform();
			btQuaternion quat = trans.getRotation();
			vel = gkQuaternion(quat.w(), quat.x(), quat.y(), quat.z())  * v;
		}
		break;
	case TRANSFORM_WORLD:
	case TRANSFORM_PARENT:
	default:
		vel = v;
		break;
	}

	btVector3 lf = m_body->getLinearFactor();
	if (gkFuzzy(lf.length2()))
		return;

	btVector3 nvel = btVector3(vel.x, vel.y, vel.z) * lf;
	if (gkFuzzy(nvel.length2()))
		return;

	m_body->setLinearVelocity(nvel);
}



void gkRigidBody::setAngularVelocity(const gkVector3& v, int tspace)
{
	if (m_suspend) // block
		return;

	if (!m_body)
		return;


	// only rigid bodies
	if (!getProperties().isRigid())
		return;

	if (v.squaredLength() > GK_EPSILON * GK_EPSILON)
		m_body->activate();



	gkVector3 vel;
	switch (tspace)
	{
	case TRANSFORM_LOCAL:
		{
			btTransform trans = m_body->getCenterOfMassTransform();
			btQuaternion quat = trans.getRotation();
			vel = gkQuaternion(quat.w(), quat.x(), quat.y(), quat.z())  * v;
		}
		break;
	case TRANSFORM_WORLD:
	case TRANSFORM_PARENT:
	default:
		vel = v;
		break;
	}


	btVector3 af = m_body->getAngularFactor();
	if (gkFuzzy(af.length2()))
		return;


	btVector3 nvel = btVector3(vel.x, vel.y, vel.z) * af;
	if (gkFuzzy(nvel.length2()))
		return;

	m_body->setAngularVelocity(nvel);
}



void gkRigidBody::applyTorque(const gkVector3& v, int tspace)
{
	if (m_suspend) // block
		return;

	if (!m_body)
		return;

	if (m_body->isStaticOrKinematicObject())
		return;

	if (v.squaredLength() > GK_EPSILON * GK_EPSILON) m_body->activate();

	gkVector3 vel;
	switch (tspace)
	{
	case TRANSFORM_LOCAL:
		{

			btTransform trans = m_body->getCenterOfMassTransform();
			btQuaternion quat = trans.getRotation();
			vel = gkQuaternion(quat.w(), quat.x(), quat.y(), quat.z())  * v;
		}
		break;
	case TRANSFORM_WORLD:
	case TRANSFORM_PARENT:
	default:
		vel = v;
		break;
	}

	m_body->applyTorque(btVector3(vel.x, vel.y, vel.z));
}



void gkRigidBody::applyForce(const gkVector3& v, int tspace)
{
	if (m_suspend) // block
		return;

	if (!m_body)
		return;

	// only dynamic bodies
	if (m_body->isStaticOrKinematicObject())
		return;

	if (v.squaredLength() > GK_EPSILON * GK_EPSILON)
		m_body->activate();


	gkVector3 vel;

	switch (tspace)
	{
	case TRANSFORM_LOCAL:
		{

			btTransform trans = m_body->getCenterOfMassTransform();
			btQuaternion quat = trans.getRotation();
			vel = gkQuaternion(quat.w(), quat.x(), quat.y(), quat.z())  * v;
		}
		break;
	case TRANSFORM_WORLD:
	case TRANSFORM_PARENT:
	default:
		vel = v;
		break;
	}

	m_body->applyCentralForce(btVector3(vel.x, vel.y, vel.z));
}



gkVector3 gkRigidBody::getLinearVelocity(void)
{
	if (m_suspend || m_body->isStaticOrKinematicObject() || !m_body)
		return gkVector3(0, 0, 0);

	return gkMathUtils::get(m_body->getLinearVelocity());
}


gkVector3 gkRigidBody::getAngularVelocity()
{
	// only rigid bodies
	if (m_suspend || !getProperties().isRigid() || !m_body)
		return gkVector3(0, 0, 0);

	return gkMathUtils::get(m_body->getAngularVelocity());
}




void gkRigidBody::getWorldTransform(btTransform& worldTrans) const
{
	if (m_suspend || !m_object->isInstanced() || !m_body)
		return;

	worldTrans.setIdentity();


	gkQuaternion rot;
	gkVector3 loc;

	// see if we can benefit from cached transforms

	if (!m_object->hasParent())
	{
		rot = m_object->getOrientation();
		loc = m_object->getPosition();
	}
	else
	{
		// must derrive
		rot = m_object->getWorldOrientation();
		loc = m_object->getWorldPosition();
	}


	worldTrans.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));
	worldTrans.setOrigin(btVector3(loc.x, loc.y, loc.z));
}



void gkRigidBody::setWorldTransform(const btTransform& worldTrans)
{
	if (m_suspend || !m_object->isInstanced() || !m_body)
		return;

	gkPhysicsController::setTransform(worldTrans);
}


void gkRigidBody::recalLocalInertia(void)
{
	GK_ASSERT(m_body && m_shape);
	if (!m_body || !m_shape) return;

	gkScalar mass = 1.f/ m_body->getInvMass();	

	btVector3 localInertia;
	m_shape->calculateLocalInertia(mass, localInertia);
	m_body->setMassProps(mass, localInertia);
}