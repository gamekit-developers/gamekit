/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Nestor Silveira.

    Contributor(s): Charlie C. Danil Ko.
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
#include "gkCharacter.h"
#include "gkDynamicsWorld.h"
#include "gkGameObject.h"
#include "gkScene.h"
#include "gkEntity.h"
#include "gkMesh.h"
#include "gkRigidBody.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"



gkCharacter::gkCharacter(gkGameObject* object, gkDynamicsWorld* owner)
	: gkPhysicsController(object, owner), m_character(0)
{
}




gkCharacter::~gkCharacter()
{
	delete m_character;
	m_character = 0;

	delete m_collisionObject;
	m_collisionObject = 0;
}




void gkCharacter::create(void)
{
	if (m_character || m_collisionObject)
		return;


	GK_ASSERT(m_object && m_object->isInstanced() && m_object->isInActiveLayer());

	// use the most up to date transform.
	const gkTransformState& trans = m_object->getTransformState();


	createShape();


	if (!m_shape)
		return;


	btPairCachingGhostObject* ghost = new btPairCachingGhostObject();
	ghost->setCollisionShape(m_shape);

	m_collisionObject = ghost;

	// basic material properties
	ghost->setFriction(m_props.m_friction);
	ghost->setRestitution(m_props.m_restitution);

	ghost->setWorldTransform(trans.toTransform());

	// intertwine
	ghost->setUserPointer(this);

	btDynamicsWorld* dyn = getOwner();

	ghost->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

	btCollisionShape* pShape = ghost->getCollisionShape();

	gkScalar stepHeight = getAabb().getSize().z / 1.5f;

	m_character = new btKinematicCharacterController(
	    ghost, static_cast<btConvexShape*>(ghost->getCollisionShape()), stepHeight);

	m_character->setUpAxis(2);

	dyn->addCollisionObject(ghost, btBroadphaseProxy::CharacterFilter);

	dyn->addAction(this);
}



btPairCachingGhostObject* gkCharacter::getGhostObject() const
{
	GK_ASSERT(m_collisionObject);
	return static_cast<btPairCachingGhostObject*>(m_collisionObject);
}



void gkCharacter::destroy(void)
{
	GK_ASSERT(m_object);

	if (m_collisionObject)
	{
		// intertwine
		m_collisionObject->setUserPointer(0);

		GK_ASSERT(m_object->isInActiveLayer());

		m_owner->getBulletWorld()->removeAction(m_character);
		m_owner->getBulletWorld()->removeCollisionObject(m_collisionObject);

		delete m_shape;

		m_shape = 0;

		delete m_character;

		m_character = 0;

		delete m_collisionObject;

		m_collisionObject = 0;
	}
}



void gkCharacter::setVelocity(const gkVector3& v, gkScalar timeInterval)
{
	if (!m_character || !m_collisionObject)
		return;

	btVector3 velocity = btVector3(v.x, v.y, v.z);

	m_character->setVelocityForTimeInterval(velocity, timeInterval);
}

void gkCharacter::setLinearVelocity(gkScalar forward, gkScalar backward, gkScalar rightward, gkScalar leftward, gkScalar timeInterval)
{
	btTransform xform =  getGhostObject()->getWorldTransform();

	gkVector3 walkDirection(0.f, 0.f, 0.f);

	if (leftward != 0)
		walkDirection -= gkVector3(leftward, 0.f, 0.f);

	if (rightward != 0)
		walkDirection += gkVector3(rightward, 0.f, 0.f);

	if (forward != 0)
		walkDirection += gkVector3(0.f, forward, 0.f);

	if (backward != 0)
		walkDirection -= gkVector3(0.f, backward, 0.f);

	gkVector3 result = gkMathUtils::get(xform.getRotation()) * walkDirection;

	setVelocity(result, timeInterval); 
} 




void gkCharacter::setWorldTransform(const btTransform& worldTrans)
{
	if (!m_character || !m_collisionObject)
		return;

	gkPhysicsController::setTransform(worldTrans);

}




void gkCharacter::updateAction( btCollisionWorld* collisionWorld, btScalar deltaTime)
{
	if (!m_character || !m_collisionObject)
		return;

	m_character->updateAction(collisionWorld, deltaTime);

	setWorldTransform(m_collisionObject->getWorldTransform());
}



void gkCharacter::setGravity(gkScalar gravity)
{
	m_character->setGravity(btScalar(gravity));
}

void gkCharacter::setRotation(const gkVector3& axis, gkScalar scalar)
{
	btMatrix3x3 orn = m_character->getGhostObject()->getWorldTransform().getBasis();
	orn *= btMatrix3x3(btQuaternion(gkMathUtils::get(axis), scalar));
	m_character->getGhostObject()->getWorldTransform().setBasis(orn);
}

void gkCharacter::setJumpSpeed(gkScalar scalar)
{
	m_character->setJumpSpeed(scalar);
}

void gkCharacter::jump(void)
{
	m_character->jump();
}

bool gkCharacter::isOnGround(void)
{
	return m_character->onGround();
}

