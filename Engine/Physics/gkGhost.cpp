/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Thomas Trocha

    Contributor(s): non yet.
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
#include "gkGhost.h"
#include "gkDynamicsWorld.h"
#include "gkGameObject.h"
#include "gkScene.h"
#include "gkEntity.h"
#include "gkMesh.h"
#include "gkRigidBody.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"



gkGhost::gkGhost(gkGameObject* object, gkDynamicsWorld* owner)
	: gkPhysicsController(object, owner)
{
}




gkGhost::~gkGhost()
{
}




void gkGhost::create(void)
{
	if (m_collisionObject)
		return;


	GK_ASSERT(m_object && m_object->isInstanced() && m_object->isInActiveLayer());

	// use the most up to date transform.
	const gkTransformState& trans = m_object->getTransformState();


	createShape();


	if (!m_shape)
		return;


	m_collisionObject = new btPairCachingGhostObject();
	m_collisionObject->setUserPointer(this);
	m_collisionObject->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
	m_collisionObject->setCollisionShape(m_shape);
	m_collisionObject->setWorldTransform(trans.toTransform());
	btDynamicsWorld* dyn = getOwner();
	dyn->addCollisionObject(m_collisionObject);
	dyn->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());



}







void gkGhost::destroy(void)
{
	GK_ASSERT(m_object);

	btDynamicsWorld* dyn = getOwner();


	if (m_collisionObject)
	{
		// intertwine
		m_collisionObject->setUserPointer(0);

		if (!m_suspend)
			dyn->removeCollisionObject(m_collisionObject);

		delete m_shape;
		m_shape = 0;

		delete m_collisionObject;

		m_collisionObject = 0;
	}

}





void gkGhost::setWorldTransform(const btTransform& worldTrans)
{
	if (!m_collisionObject)
		return;

	gkPhysicsController::setTransform(worldTrans);

}


void gkGhost::_handleManifold(btPersistentManifold* manifold)
{
	gkPhysicsController* colA = castController(manifold->getBody0());
	gkPhysicsController* colB = castController(manifold->getBody1());

	gkPhysicsController* collider = colB;

	if (collider == this)
	{
		collider = colA;
	}

	m_localContacts.reserve(1);

	gkContactInfo cinf;
	cinf.collider = collider;
	m_localContacts.push_back(cinf);
}






