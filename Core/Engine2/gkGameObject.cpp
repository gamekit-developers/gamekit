/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

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
#include "OgreSceneManager.h"
#include "OgreSceneNode.h"
#include "OgreException.h"
#include "OgreEntity.h"
#include "OgreStringConverter.h"

#include "gkSceneManager.h"
#include "gkScene.h"
#include "gkGameObject.h"
#include "gkCamera.h"
#include "gkLight.h"
#include "gkEntity.h"

#include "gkEngine.h"
#include "gkLogger.h"

#include "gkLogicManager.h"
#include "gkConstraint.h"
#include "gkGameObjectGroup.h"
#include "gkRigidBody.h"

using namespace Ogre;


gkGameObject::gkGameObject(gkScene *scene, const gkString& name, gkGameObjectTypes type, gkObject::Loader* loader)
:       gkObject(name, loader), m_type(type), m_baseProps(), m_parent(0),
        m_scene(scene), m_startPose(StringUtil::BLANK), m_node(0),
        m_logic(0), m_activeLayer(true), m_groupRef(0), m_instance(0), m_outOfDate(false),
        m_rigidBody(0), m_lockTransform(false)
{
    GK_ASSERT(m_scene);
}


gkGameObject::~gkGameObject()
{
    clearVariables();
    destroyConstraints();
}




gkGameObject *gkGameObject::duplicate(const gkString &newName)
{
    return 0;
}


void gkGameObject::attachLogic(gkLogicTree *tree)
{
}



void gkGameObject::attachToGroup(gkGameObjectGroup *g)
{
}


void gkGameObject::attachToGroupInstance(gkGameObjectGroupInstance *g)
{
}


void gkGameObject::detachFromGroup(void)
{
}


gkGameObject *gkGameObject::getGroupParent(void)
{
    return 0;
}


void gkGameObject::addConstraint(gkConstraint *c)
{
    GK_ASSERT(c);
    m_constraints.push_back(c);
}


void gkGameObject::loadImpl(void)
{
    SceneManager *manager = m_scene->getManager();
    SceneNode *parentNode = 0;

    if (m_parent)
    {
        if (!m_parent->isLoaded()) m_parent->load();
        parentNode = m_parent->m_node;
    }
    // Create from parent or root node
    m_node = parentNode ? parentNode->createChildSceneNode(m_name) : manager->getRootSceneNode()->createChildSceneNode(m_name);

    m_node->setPosition(m_baseProps.position);
    m_node->setOrientation(m_baseProps.orientation);
    m_node->setScale(m_baseProps.scale);
    m_node->showBoundingBox(m_baseProps.showAABB);


    // save transform states
    m_cur.loc   = m_baseProps.position;
    m_cur.rot   = m_baseProps.orientation;
    m_cur.scl   = m_baseProps.scale;
    m_prev      = m_cur;

    m_node->setInitialState();

    if (m_rigidBody)
        m_rigidBody->load();
}


void gkGameObject::postLoadImpl(void)
{
    // tell scene
    m_scene->notifyObjectLoaded(this);
}


void gkGameObject::unloadImpl(void)
{
    SceneManager *manager = m_scene->getManager();


    // per loaded object
    clearVariables();

    // destroy gamelogic
    if (m_logic != 0)
    {
        //gkLogicManager::getSingleton().destroy(m_logic);
        m_logic = 0;
    }

    detachFromGroup();

    if (m_rigidBody)
        m_rigidBody->unload();

    destroyConstraints();

    if (m_node)
    {
        // Destroy and detach from scene graph
        // TODO attach children to parent scene node
        m_node->detachAllObjects();
        m_node->removeAllChildren();
        manager->destroySceneNode(m_node);
        m_node = 0;
    }


    m_scene->notifyObjectUnloaded(this);
}

Ogre::MovableObject *gkGameObject::getMovable(void)
{
    if (!isLoaded())
        return 0;

    switch (m_type)
    {
    case GK_CAMERA:
        return getCamera()->getCamera();
    case GK_ENTITY:
        return getEntity()->getEntity();
    case GK_LIGHT:
        return getLight()->getLight();
    }
    return 0;
}


gkMatrix4 gkGameObject::getWorldTransform(void)
{
    if (m_node != 0)
        return m_node->_getFullTransform();
    return getLocalTransform().toMatrix();
}


gkVector3 gkGameObject::getWorldPosition(void)
{
    if (m_node != 0)
        return m_node->_getDerivedPosition();
    return m_baseProps.position;
}


gkVector3 gkGameObject::getWorldScale(void)
{
    if (m_node != 0)
        return m_node->_getDerivedScale();
    return m_baseProps.scale;
}


gkQuaternion gkGameObject::getWorldOrientation(void)
{
    if (m_node != 0)
        return m_node->_getDerivedOrientation();
    return m_baseProps.orientation;
}


gkEuler gkGameObject::getWorldRotation(void)
{
    if (m_node != 0)
        return m_node->_getDerivedOrientation();
    return m_baseProps.orientation;
}


void gkGameObject::notifyUpdate(void)
{
    // tell scene about this transform
    if (m_scene && m_node != 0)
    {
        if (!m_outOfDate)
        {
            m_scene->notifyObjectMoved(this);
            m_outOfDate = true;
        }

        m_prev = m_cur;
        m_cur.loc = m_node->getPosition();
        m_cur.rot = m_node->getOrientation();
        m_cur.scl = m_node->getScale();

        if (!m_lockTransform)
        {
            m_lockTransform = true;
            applyConstraints();
            m_lockTransform = false;
        }
    }
}



void gkGameObject::applyTransformState(const gkTransformState& newstate)
{
    if (m_node)
    {
        m_node->setPosition(newstate.loc);
        m_node->setOrientation(newstate.rot);
        m_node->setScale(newstate.scl);

        if (m_rigidBody)
            m_rigidBody->setTransformState(newstate);
    }
}


void gkGameObject::blendTransform(gkScalar blend)
{
    if (m_scene && m_node && m_outOfDate)
    {
        gkVector3 dloc      = gkMathUtils::interp(m_prev.loc, m_cur.loc, blend);
        gkVector3 dscl      = gkMathUtils::interp(m_prev.scl, m_cur.scl, blend);
        gkQuaternion drot   = gkMathUtils::interp(m_prev.rot, m_cur.rot, blend);

        gkTransformState bstate(dloc, drot, dscl);
        // rework this
        if (blend >= 0.9)
            m_outOfDate = false;

        applyTransformState(bstate);
    }
}


void gkGameObject::applyConstraints(void)
{
    if (!m_constraints.empty())
    {
        gkConstraint *cnst = m_constraints.begin();

        while (cnst)
        {
            cnst->setMatrix(m_cur);

            // apply if we've constrained
            if (cnst->update(this))
                setTransform(cnst->getMatrix());
            cnst = cnst->getNext();
        }
    }
}

void gkGameObject::destroyConstraints(void)
{
    if (!m_constraints.empty())
    {
        gkConstraint *cnst = m_constraints.begin(), *tmp;
        while (cnst)
        {
            tmp = cnst;
            cnst = cnst->getNext();
            delete tmp;
        }

        m_constraints.clear();
    }
}


void gkGameObject::setTransform(const gkMatrix4 &v)
{
    if (m_node != 0)
    {
        gkTransformState st;
        gkMathUtils::extractTransformFast(v, st.loc, st.rot, st.scl);
        applyTransformState(st);
        notifyUpdate();
    }
}

void gkGameObject::setTransform(const gkTransformState &v)
{
    applyTransformState(v);
    notifyUpdate();
}


void gkGameObject::setPosition(const gkVector3 &v)
{
    if (m_node != 0)
    {
        m_node->setPosition(v);
        notifyUpdate();

        // update the rigid body state
        if (m_rigidBody != 0)
            m_rigidBody->updateTransform();
    }
}


void gkGameObject::setScale(const gkVector3 &v)
{
    if (m_node != 0)
    {
        m_node->setScale(v);
        notifyUpdate();
    }
}


void gkGameObject::setOrientation(const gkQuaternion& q)
{
    if (m_node != 0)
    {
        m_node->setOrientation(q);
        notifyUpdate();

        // update the rigid body state
        if (m_rigidBody != 0)
            m_rigidBody->updateTransform();
    }
}


void gkGameObject::setOrientation(const gkEuler& v)
{
    if (m_node != 0)
    {
        gkQuaternion q = v.toQuaternion();
        m_node->setOrientation(q);
        notifyUpdate();

        // update the rigid body state
        if (m_rigidBody != 0)
            m_rigidBody->updateTransform();
    }
}


void gkGameObject::rotate(const gkEuler &drot, int tspace)
{
    // convert to quaternion and rotate normally
    rotate(drot.toQuaternion(), tspace);
}

void gkGameObject::rotate(const gkQuaternion &dq, int tspace)
{

    if (m_node != 0)
    {
        m_node->rotate(dq, (Node::TransformSpace)tspace);
        notifyUpdate();

        // update the rigid body state
        if (m_rigidBody != 0)
            m_rigidBody->updateTransform();
    }
}



void gkGameObject::yaw(const gkRadian &v, int tspace)
{
    if (m_node != 0)
    {
        m_node->yaw(v, (Node::TransformSpace)tspace);
        notifyUpdate();

        // update the rigid body state
        if (m_rigidBody != 0)
            m_rigidBody->updateTransform();
    }

}

void gkGameObject::pitch(const gkRadian &v, int tspace )
{
    if (m_node != 0)
    {
        m_node->pitch(v, (Node::TransformSpace)tspace);
        notifyUpdate();

        // update the rigid body state
        if (m_rigidBody != 0)
            m_rigidBody->updateTransform();
    }
}

void gkGameObject::roll(const gkRadian &v, int tspace)
{
    if (m_node != 0)
    {
        m_node->roll(v, (Node::TransformSpace)tspace);
        notifyUpdate();

        // update the rigid body state
        if (m_rigidBody != 0)
            m_rigidBody->updateTransform();
    }
}


void gkGameObject::translate(const gkVector3 &dloc, int tspace)
{
    if (m_node != 0)
    {
        m_node->translate(dloc, (Node::TransformSpace)tspace);
        notifyUpdate();

        // update the rigid body state
        if (m_rigidBody != 0)
            m_rigidBody->updateTransform();
    }
}


void gkGameObject::scale(const gkVector3 &dscale)
{
    if (m_node != 0)
    {
        m_node->scale(dscale);
        notifyUpdate();
    }
}


void gkGameObject::setLinearVelocity(const gkVector3 &v, int tspace)
{
    if (m_rigidBody != 0)
        m_rigidBody->setLinearVelocity(v, tspace);
}


void gkGameObject::setAngularVelocity(const gkVector3& v, int tspace)
{
    if (m_rigidBody != 0)
        m_rigidBody->setAngularVelocity(v, tspace);
}


void gkGameObject::applyTorque(const gkVector3 &v, int tspace)
{
    if (m_rigidBody != 0)
        m_rigidBody->applyTorque(v, tspace);
}


void gkGameObject::applyForce(const gkVector3 &v, int tspace)
{
    if (m_rigidBody != 0)
        m_rigidBody->applyForce(v, tspace);
}

gkVector3 gkGameObject::getLinearVelocity(void)
{
    if (m_rigidBody != 0)
        return m_rigidBody->getLinearVelocity();
    return gkVector3::ZERO;
}


gkVector3 gkGameObject::getAngularVelocity()
{
    if (m_rigidBody != 0)
        return m_rigidBody->getAngularVelocity();
    return gkVector3::ZERO;
}

void gkGameObject::clearVariables(void)
{
    // destroy all variables ( blender game object properties )
    gkEngine &eng = gkEngine::getSingleton();
    utHashTableIterator<VariableMap> iter(m_variables);

    while (iter.hasMoreElements())
    {
        gkVariable *var = iter.getNext().second;

        // remove from debug list
        if (var->isDebug())
            eng.removeDebugProperty(var);

        delete var;
    }

    m_variables.clear();
}


gkVariable *gkGameObject::createVariable(const gkString &name, bool debug)
{
    gkHashedString findName(name);

    UTsize pos = m_variables.find(findName);
    if (pos != UT_NPOS)
    {
        gkPrintf("duplicate property found while trying to create.\n");
        return 0;
    }


    gkVariable *prop = new gkVariable(name, debug);
    m_variables.insert(findName, prop);
    // add to the debugging interface
    if (debug)
        gkEngine::getSingleton().addDebugProperty(prop);

    return prop;
}

gkVariable *gkGameObject::getVariable(const gkString &name)
{
    gkHashedString findName(name);
    UTsize pos = m_variables.find(findName);
    if (pos != UT_NPOS)
        return m_variables.at(pos);
    return 0;
}


bool gkGameObject::hasVariable(const gkString &name)
{
    return m_variables.find(name) != UT_NPOS;
}
