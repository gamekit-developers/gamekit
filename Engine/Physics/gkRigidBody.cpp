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
#include "gkRigidBody.h"
#include "gkDynamicsWorld.h"
#include "gkGameObject.h"
#include "gkScene.h"
#include "gkRigidBody.h"
#include "gkEntity.h"
#include "gkMesh.h"
#include "btBulletDynamicsCommon.h"
#include "OgreSceneNode.h"

gkRigidBody::gkRigidBody(const gkString& name, gkGameObject *object, gkDynamicsWorld *owner)
:       gkObject(name), m_owner(owner), m_object(object), m_body(0), m_shape(0), m_oldActivationState(-1)
{
}


gkRigidBody::~gkRigidBody()
{
    if (m_body)
        delete m_body;
    if (m_shape)
        delete m_shape;
}


void gkRigidBody::loadImpl(void)
{
    GK_ASSERT(m_object);

    gkGameObjectProperties  &props  = m_object->getProperties();
    gkPhysicsProperties     &phy    = props.m_physics;

    if (phy.isContactListener())
        setFlags(getFlags() | gkObject::RBF_CONTACT_INFO);

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

    if (phy.isRigidOrDynamic())
    {
        btVector3 inertia;
        m_shape->calculateLocalInertia(phy.m_mass, inertia);

        m_body = new btRigidBody(phy.m_mass, this, m_shape, inertia);

        if (phy.isDynamic())
            m_body->setAngularFactor(0.f);

        m_body->setDamping(phy.m_linearDamp, phy.m_angularDamp);

    }
    else
    {
        m_body = new btRigidBody(0.f, 0, m_shape, btVector3(0,0,0));
        m_body->setDamping(0.f, 0.f);
        m_body->setAngularFactor(0.f);
        m_body->setLinearFactor(btVector3(0.f, 0.f, 0.f));
    }


    if (m_body != 0)
    {
        btDynamicsWorld *dyn = m_owner->getBulletWorld();
        m_body->setUserPointer(this);

        m_body->setFriction(phy.m_friction);
        m_body->setRestitution(phy.m_restitution);


        m_body->setWorldTransform(props.m_transform.toTransform());

        if (!phy.isDosser())
            m_body->setActivationState(m_body->getActivationState() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

        dyn->addRigidBody(m_body);
    }
}



void gkRigidBody::unloadImpl(void)
{
    if (m_body)
    {
        // intertwine
        m_body->setUserPointer(0);

        if (!m_body->isStaticOrKinematicObject())
            m_body->setMotionState(0);

        m_owner->getBulletWorld()->removeRigidBody(m_body);

        delete m_shape;
        m_shape = 0;

        delete m_body;
        m_body = 0;
    }
}

void gkRigidBody::setTransformState(const gkTransformState& state)
{
    if (m_body)
    {
        m_body->activate();
        m_body->setCenterOfMassTransform(state.toTransform());
    }


}


void gkRigidBody::updateTransform(void)
{
    if (!isLoaded())
        return;

    if (m_body->isStaticOrKinematicObject())
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

    m_body->setCenterOfMassTransform(worldTrans);
}



void gkRigidBody::setLinearVelocity(const gkVector3 &v, int tspace)
{
    if (!isLoaded())
        return;

    // only dynamic bodies
    if (m_body->isStaticOrKinematicObject())
        return;

    if (v.squaredLength() > GK_EPSILON*GK_EPSILON) m_body->activate();




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
    m_object->notifyUpdate();
}


void gkRigidBody::setAngularVelocity(const gkVector3& v, int tspace)
{
    if (!isLoaded())
        return;

    // only dynamic bodies
    if (m_body->isStaticOrKinematicObject())
        return;

    if (v.squaredLength() > GK_EPSILON*GK_EPSILON) m_body->activate();


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
    m_object->notifyUpdate();
}


void gkRigidBody::applyTorque(const gkVector3 &v, int tspace)
{
    if (!isLoaded())
        return;
    // only dynamic bodies
    if (m_body->isStaticOrKinematicObject())
        return;

    if (v.squaredLength() > GK_EPSILON*GK_EPSILON) m_body->activate();

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
    m_object->notifyUpdate();
}


void gkRigidBody::applyForce(const gkVector3 &v, int tspace)
{
    if (!isLoaded())
        return;

    // only dynamic bodies
    if (m_body->isStaticOrKinematicObject())
        return;

    if (v.squaredLength() > GK_EPSILON*GK_EPSILON) m_body->activate();


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

    m_object->notifyUpdate();
}


gkVector3 gkRigidBody::getLinearVelocity(void)
{
    if (m_body->isStaticOrKinematicObject() || !isLoaded())
        return gkVector3(0, 0, 0);

    btVector3 vel = m_body->getLinearVelocity();
    return gkVector3(vel.x(), vel.y(), vel.z());
}

gkVector3 gkRigidBody::getAngularVelocity()
{
    if (m_body->isStaticOrKinematicObject() || !isLoaded())
        return gkVector3(0, 0, 0);

    btVector3 vel = m_body->getAngularVelocity();
    return gkVector3(vel.x(), vel.y(), vel.z());
}


// transform access
void gkRigidBody::getWorldTransform(btTransform& worldTrans) const
{
    if (!m_object->isLoaded() || !isLoaded())
        return;

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
}


void gkRigidBody::setWorldTransform(const btTransform& worldTrans)
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

btCollisionObject* gkRigidBody::getCollisionObject() 
{ 
	return m_body;
}

Ogre::AxisAlignedBox gkRigidBody::getAabb() const
{
	if(m_body)
	{
		btVector3 aabbMin;
		btVector3 aabbMax;

		m_body->getAabb(aabbMin, aabbMax);

		gkVector3 min_aabb(aabbMin.x(), aabbMin.y(), aabbMin.z());
		gkVector3 max_aabb(aabbMax.x(), aabbMax.y(), aabbMax.z());

		return Ogre::AxisAlignedBox(min_aabb, max_aabb);
	}

	return Ogre::AxisAlignedBox();
}

