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
#include "btBulletDynamicsCommon.h"
#include "OgreSceneNode.h"

gkRigidBody::gkRigidBody(const gkString& name, gkGameObject *object, gkDynamicsWorld *owner, gkObject::Loader *manual)
:       gkObject(name, manual), m_owner(owner), m_object(object), m_rigidBody(0)
{
    if (m_object)
        m_object->attachRigidBody(this);
}


gkRigidBody::~gkRigidBody()
{
    if (m_rigidBody)
        delete m_rigidBody;
    if (m_object)
        m_object->attachRigidBody(0);
}


void gkRigidBody::_reinstanceBody(btRigidBody *body)
{

    // do not use outside of manual loading!
    if (m_rigidBody)
        delete m_rigidBody;
    m_rigidBody = body;
}


void gkRigidBody::loadImpl(void)
{
    GK_ASSERT(m_object);
    if (isManual() && m_rigidBody)
    {
        // intertwine
        m_rigidBody->setUserPointer(this);

		//m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

        if (!m_rigidBody->isStaticOrKinematicObject())
            m_rigidBody->setMotionState(this);


        // add body to bullet world
        if (m_object->isInActiveLayer())
        {
            btDynamicsWorld *dyn = m_owner->getBulletWorld();
            dyn->addRigidBody(m_rigidBody);

            btVector3 lf = m_rigidBody->getLinearFactor();
            btVector3 af = m_rigidBody->getAngularFactor();
 
            if (m_flags & RBF_LIMIT_LVEL_X) lf[0] = 0.f;
            if (m_flags & RBF_LIMIT_LVEL_Y) lf[1] = 0.f;
            if (m_flags & RBF_LIMIT_LVEL_Z) lf[2] = 0.f;
            if (m_flags & RBF_LIMIT_AVEL_X) af[0] = 0.f;
            if (m_flags & RBF_LIMIT_AVEL_Y) af[1] = 0.f;
            if (m_flags & RBF_LIMIT_AVEL_Z) af[2] = 0.f;

            m_rigidBody->setLinearFactor(lf);
            m_rigidBody->setAngularFactor(af);
        }
    }

    // else fixme (non manual | default) loading via btWorldImporter & bullet serialization
    // for now this assumes manual loader callbacks
}



void gkRigidBody::unloadImpl(void)
{
    GK_ASSERT(m_object);
    if (isManual() && m_rigidBody)
    {
        // intertwine
        m_rigidBody->setUserPointer(0);

        if (!m_rigidBody->isStaticOrKinematicObject())
            m_rigidBody->setMotionState(0);

        // add body to bullet world
        if (m_object->isInActiveLayer())
            m_owner->getBulletWorld()->removeRigidBody(m_rigidBody);


        delete m_rigidBody;
        m_rigidBody = 0;
    }

    // else fixme (non manual | default) loading via btWorldImporter & bullet serialization
    // for now this assumes manual loader callbacks
}

void gkRigidBody::setTransformState(const gkTransformState& state)
{
    if (!isLoaded())
        return;

    if (m_rigidBody->isStaticOrKinematicObject())
        return;
    btTransform worldTrans;
    worldTrans.setIdentity();

    worldTrans.setRotation(btQuaternion(state.rot.x, state.rot.y, state.rot.z, state.rot.w));
    worldTrans.setOrigin(btVector3(state.loc.x, state.loc.y, state.loc.z));

    m_rigidBody->setCenterOfMassTransform(worldTrans);
}


void gkRigidBody::updateTransform(void)
{
    if (!isLoaded())
        return;

    if (m_rigidBody->isStaticOrKinematicObject())
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

    m_rigidBody->setCenterOfMassTransform(worldTrans);
}



void gkRigidBody::setLinearVelocity(const gkVector3 &v, int tspace)
{
    if (!isLoaded())
        return;

    // only dynamic bodies
    if (m_rigidBody->isStaticOrKinematicObject())
        return;

    if (v.squaredLength() > GK_EPSILON*GK_EPSILON) m_rigidBody->activate();




    gkVector3 vel;
    switch (tspace)
    {
    case TRANSFORM_LOCAL:
        {
            btTransform trans = m_rigidBody->getCenterOfMassTransform();
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

    btVector3 lf = m_rigidBody->getLinearFactor();
    if (gkFuzzy(lf.length2()))
        return;
    btVector3 nvel = btVector3(vel.x, vel.y, vel.z) * lf;
    if (gkFuzzy(nvel.length2()))
        return;

    m_rigidBody->setLinearVelocity(nvel);
    m_object->notifyUpdate();
}


void gkRigidBody::setAngularVelocity(const gkVector3& v, int tspace)
{
    if (!isLoaded())
        return;

    // only dynamic bodies
    if (m_rigidBody->isStaticOrKinematicObject())
        return;

    if (v.squaredLength() > GK_EPSILON*GK_EPSILON) m_rigidBody->activate();


    gkVector3 vel;
    switch (tspace)
    {
    case TRANSFORM_LOCAL:
        {
            btTransform trans = m_rigidBody->getCenterOfMassTransform();
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

    btVector3 af = m_rigidBody->getAngularFactor();
    if (gkFuzzy(af.length2()))
        return;
    btVector3 nvel = btVector3(vel.x, vel.y, vel.z) * af;
    if (gkFuzzy(nvel.length2()))
        return;
 
    m_rigidBody->setAngularVelocity(nvel);
    m_object->notifyUpdate();
}


void gkRigidBody::applyTorque(const gkVector3 &v, int tspace)
{
    if (!isLoaded())
        return;
    // only dynamic bodies
    if (m_rigidBody->isStaticOrKinematicObject())
        return;

    if (v.squaredLength() > GK_EPSILON*GK_EPSILON) m_rigidBody->activate();

    gkVector3 vel;
    switch (tspace)
    {
    case TRANSFORM_LOCAL:
        {

            btTransform trans = m_rigidBody->getCenterOfMassTransform();
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

    m_rigidBody->applyTorque(btVector3(vel.x, vel.y, vel.z));
    m_object->notifyUpdate();
}


void gkRigidBody::applyForce(const gkVector3 &v, int tspace)
{
    if (!isLoaded())
        return;

    // only dynamic bodies
    if (m_rigidBody->isStaticOrKinematicObject())
        return;

    if (v.squaredLength() > GK_EPSILON*GK_EPSILON) m_rigidBody->activate();


    gkVector3 vel;
    switch (tspace)
    {
    case TRANSFORM_LOCAL:
        {

            btTransform trans = m_rigidBody->getCenterOfMassTransform();
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

    m_rigidBody->applyCentralForce(btVector3(vel.x, vel.y, vel.z));

    m_object->notifyUpdate();
}


gkVector3 gkRigidBody::getLinearVelocity(void)
{
    if (m_rigidBody->isStaticOrKinematicObject() || !isLoaded())
        return gkVector3(0, 0, 0);

    btVector3 vel = m_rigidBody->getLinearVelocity();
    return gkVector3(vel.x(), vel.y(), vel.z());
}

gkVector3 gkRigidBody::getAngularVelocity()
{
    if (m_rigidBody->isStaticOrKinematicObject() || !isLoaded())
        return gkVector3(0, 0, 0);

    btVector3 vel = m_rigidBody->getAngularVelocity();
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
	return m_rigidBody;
}

Ogre::AxisAlignedBox gkRigidBody::getAabb() const
{
	if(m_rigidBody)
	{
		btVector3 aabbMin;
		btVector3 aabbMax;

		m_rigidBody->getAabb(aabbMin, aabbMax);

		gkVector3 min_aabb(aabbMin.x(), aabbMin.y(), aabbMin.z());
		gkVector3 max_aabb(aabbMax.x(), aabbMax.y(), aabbMax.z());

		return Ogre::AxisAlignedBox(min_aabb, max_aabb);
	}

	return Ogre::AxisAlignedBox();
}

