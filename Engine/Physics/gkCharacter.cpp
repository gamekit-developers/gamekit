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
#include "gkEntity.h"
#include "gkMesh.h"
#include "gkRigidBody.h"
#include "OgreSceneNode.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"

gkCharacter::gkCharacter(const gkString& name, gkGameObject *object, gkDynamicsWorld *owner) 
: gkObject(name), m_owner(owner), m_object(object), m_ghostObject(0), m_character(0), m_shape(0)
{
}

gkCharacter::~gkCharacter()
{
    if (m_ghostObject)
        delete m_ghostObject;
    if (m_shape)
        delete m_shape;
}

void gkCharacter::loadImpl(void)
{
    GK_ASSERT(m_object);

    gkGameObjectProperties  &props  = m_object->getProperties();
    gkPhysicsProperties     &phy    = props.m_physics;

    gkMesh *me = 0;
    gkEntity *ent = m_object->getEntity();
    if (ent != 0)
        me = ent->getEntityProperties().m_mesh;

    // extract the shape's size
    gkVector3 size(1.f, 1.f, 1.f);
    if (me != 0)
        size = me->getBoundingBox().getHalfSize();
    else
        size *= phy.m_radius;

    switch (phy.m_shape)
    {
    case SH_BOX: 
        m_shape = new btBoxShape(btVector3(size.x, size.y, size.z));
        break;
    case SH_CONE: 
        m_shape = new btConeShapeZ(gkMax(size.x, size.y), 2.f * size.z);
        break;
    case SH_CYLINDER: 
        m_shape = new btCylinderShapeZ(btVector3(size.x, size.y, size.z));
        break;
    case SH_CONVEX_TRIMESH:
    case SH_GIMPACT_MESH:
    case SH_BVH_MESH:
        {
            if (me != 0)
            {
                btTriangleMesh *triMesh = me->getTriMesh();
                if (triMesh->getNumTriangles() > 0)
                {
                    if (phy.m_shape == SH_CONVEX_TRIMESH)
                        m_shape = new btConvexTriangleMeshShape(triMesh);
                    else if (phy.m_shape == SH_GIMPACT_MESH)
                        m_shape = new btConvexTriangleMeshShape(triMesh);
                    else
                        m_shape = new btBvhTriangleMeshShape(triMesh, true);
                    break;
                }
                else 
                    return;
            }
        }
    case SH_SPHERE: 
        m_shape = new btSphereShape(gkMax(size.x, gkMax(size.y, size.z)));
        break;
    }

    if (!m_shape) 
        return;

    m_shape->setMargin(phy.m_margin);
    m_shape->setLocalScaling(props.m_transform.toLocalScaling());

	m_ghostObject = new btPairCachingGhostObject();
	m_ghostObject->setCollisionShape(m_shape);


    // basic material properties
    m_ghostObject->setFriction(phy.m_friction);
    m_ghostObject->setRestitution(phy.m_restitution);

    m_ghostObject->setWorldTransform(props.m_transform.toTransform());


    // intertwine
    m_ghostObject->setUserPointer(this);

    btDynamicsWorld *dyn = m_owner->getBulletWorld();

	//m_ghostObject->setCollisionFlags(btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

	m_ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

	btCollisionShape* pShape = m_ghostObject->getCollisionShape();

	gkScalar stepHeight = getAabb().getSize().z/2.5f;

	m_character = new btKinematicCharacterController(
		m_ghostObject, static_cast<btConvexShape*>(m_ghostObject->getCollisionShape()), stepHeight);

	m_character->setUpAxis(2);

	dyn->addCollisionObject(m_ghostObject, btBroadphaseProxy::CharacterFilter);

	dyn->addAction(this);

    /*		m_owner->getBulletWorld()->getPairCache()->cleanProxyFromPairs(
			m_ghostObject->getBroadphaseHandle(), m_owner->getBulletWorld()->getDispatcher());*/
}

void gkCharacter::unloadImpl(void)
{
    GK_ASSERT(m_object);

    if (m_ghostObject)
    {
        // intertwine
        m_ghostObject->setUserPointer(0);

        // add character to bullet world
        if (m_object->isInActiveLayer())
		{
			m_owner->getBulletWorld()->removeAction(m_character);
            m_owner->getBulletWorld()->removeCollisionObject(m_ghostObject);
		}
        delete m_shape;

        m_shape = 0;

		delete m_character;

		m_character = 0;

        delete m_ghostObject;

        m_ghostObject = 0;
    }
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
