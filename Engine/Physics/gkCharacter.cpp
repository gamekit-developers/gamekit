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
#include "gkCharacter.h"
#include "gkDynamicsWorld.h"
#include "gkGameObject.h"
#include "gkScene.h"
#include "gkRigidBody.h"
#include "OgreSceneNode.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"

gkCharacter::gkCharacter(const gkString& name, gkGameObject *object, gkDynamicsWorld *owner, gkObject::Loader *manual) 
: gkObject(name, manual), m_owner(owner), m_object(object), m_ghostObject(0), m_character(0)
{
    if (m_object)
        m_object->attachCharacter(this);
}

gkCharacter::~gkCharacter()
{
    if (m_ghostObject)
        delete m_ghostObject;

    if (m_object)
        m_object->attachCharacter(0);
}


void gkCharacter::_reinstanceCharacter(btPairCachingGhostObject *ghostObject)
{
    // do not use outside of manual loading!
    if (m_ghostObject)
        delete m_ghostObject;

    m_ghostObject = ghostObject;
}


void gkCharacter::loadImpl(void)
{
    GK_ASSERT(m_object);
    
	if (isManual() && m_ghostObject)
    {
        // intertwine
        m_ghostObject->setUserPointer(this);

        // add character to bullet world
        if (m_object->isInActiveLayer())
        {
            btDynamicsWorld *dyn = m_owner->getBulletWorld();

			//m_ghostObject->setCollisionFlags(btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

			m_ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

			gkScalar stepHeight = 0.4f;

			m_character = new btKinematicCharacterController(
				m_ghostObject, static_cast<btConvexShape*>(m_ghostObject->getCollisionShape()), stepHeight);

			m_character->setUpAxis(2);

			dyn->addCollisionObject(m_ghostObject, btBroadphaseProxy::CharacterFilter);

			dyn->addAction(this);

	/*		m_owner->getBulletWorld()->getPairCache()->cleanProxyFromPairs(
				m_ghostObject->getBroadphaseHandle(), m_owner->getBulletWorld()->getDispatcher());*/
        }
    }

    // else fixme (non manual | default) loading via btWorldImporter & bullet serialization
    // for now this assumes manual loader callbacks
}

void gkCharacter::unloadImpl(void)
{
    GK_ASSERT(m_object);

    if (isManual() && m_ghostObject)
    {
        // intertwine
        m_ghostObject->setUserPointer(0);

        // add character to bullet world
        if (m_object->isInActiveLayer())
		{
			m_owner->getBulletWorld()->removeAction(m_character);
            m_owner->getBulletWorld()->removeCollisionObject(m_ghostObject);
		}

		delete m_character;

		m_character = 0;

        delete m_ghostObject;

        m_ghostObject = 0;
    }

    // else fixme (non manual | default) loading via btWorldImporter & bullet serialization
    // for now this assumes manual loader callbacks
}

void gkCharacter::setTransformState(const gkTransformState& state)
{
    if (!isLoaded())
        return;

    btTransform worldTrans;
    worldTrans.setIdentity();

    worldTrans.setRotation(btQuaternion(state.rot.x, state.rot.y, state.rot.z, state.rot.w));
    worldTrans.setOrigin(btVector3(state.loc.x, state.loc.y, state.loc.z));

    m_ghostObject->setWorldTransform(worldTrans);
}

void gkCharacter::updateTransform()
{
    if (!isLoaded())
        return;

    btTransform worldTrans;
    worldTrans.setIdentity();

    gkQuaternion rot;
    gkVector3 loc;

    // see if we can benefit from cached transforms
    if (!m_object->getParent())
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

    m_ghostObject->setWorldTransform(worldTrans);
}

void gkCharacter::setVelocity(const gkVector3& v, gkScalar timeInterval)
{
    if (!isLoaded())
        return;

	btVector3 velocity = btVector3(v.x, v.y, v.z);

	m_character->setVelocityForTimeInterval(velocity, timeInterval);

	m_object->notifyUpdate();
}

void gkCharacter::setWorldTransform(const btTransform& worldTrans)
{
    if (!m_object->isLoaded() || !isLoaded())
        return;

    const btQuaternion &rot = worldTrans.getRotation();
    const btVector3 &loc = worldTrans.getOrigin();

    // apply to the node and sync state next update
    Ogre::SceneNode *node = m_object->getNode();
    node->setOrientation(gkQuaternion(rot.w(), rot.x(), rot.y(), rot.z()));
    node->setPosition(gkVector3(loc.x(), loc.y(), loc.z()));
    m_object->notifyUpdate();
}

void gkCharacter::updateAction( btCollisionWorld* collisionWorld, btScalar deltaTime)
{
    if (!m_object->isLoaded() || !isLoaded())
        return;

	m_character->updateAction(collisionWorld, deltaTime);

	setWorldTransform(m_ghostObject->getWorldTransform());
}

btCollisionObject* gkCharacter::getCollisionObject() 
{ 
	return m_ghostObject;
}

Ogre::AxisAlignedBox gkCharacter::getAabb() const
{
	if(m_ghostObject)
	{
		btVector3 aabbMin;
		btVector3 aabbMax;

		m_ghostObject->getCollisionShape()->getAabb(m_ghostObject->getWorldTransform(), aabbMin, aabbMax);

		gkVector3 min_aabb(aabbMin.x(), aabbMin.y(), aabbMin.z());
		gkVector3 max_aabb(aabbMax.x(), aabbMax.y(), aabbMax.z());

		return Ogre::AxisAlignedBox(min_aabb, max_aabb);
	}

	return Ogre::AxisAlignedBox();
}
