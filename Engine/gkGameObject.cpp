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
#include "gkLogicLink.h"
#include "gkNodeManager.h"
#include "gkLogicTree.h"
#include "gkConstraint.h"
#include "gkGameObjectGroup.h"
#include "gkRigidBody.h"
#include "gkCharacter.h"
#include "gkDynamicsWorld.h"
#include "gkMesh.h"

using namespace Ogre;


gkGameObject::gkGameObject(gkScene *scene, const gkString &name, gkGameObjectTypes type)
	:       gkObject(name),  m_type(type),  m_baseProps(),
	        m_parent(0), m_scene(scene),  m_node(0),
	        m_logic(0),  m_bricks(0),
	        m_rigidBody(0),  m_character(0),  m_groupRef(0),  m_instance(0),
	        m_groupOwn(false), m_state(0),
	        m_activeLayer(true),  m_outOfDate(false), m_isClone(false)
{
	GK_ASSERT(m_scene);
	m_life.tick = 0;
	m_life.timeToLive = 0;
}


gkGameObject::~gkGameObject()
{
	clearVariables();
	destroyConstraints();


	if (m_bricks)
	{
		gkLogicManager::getSingleton().destroy(m_bricks);
		m_bricks = 0;
	}
}

void gkGameObject::attachLogic(gkLogicTree *tree)
{
	if (!m_logic)
	{
		m_logic = tree;
		m_logic->attachObject(this);
	}
}


void gkGameObject::attachLogic(gkLogicLink *bricks)
{
	UT_ASSERT(!m_bricks);
	m_bricks = bricks;
}

void gkGameObject::attachToGroup(gkGameObjectGroup *g)
{
	// ignore
	if (m_groupRef)
		return;
	m_groupRef = g;
}


void gkGameObject::attachToGroupInstance(gkGameObjectInstance *g)
{
	// ignore
	if (m_instance)
		return;
	m_instance = g;
}


void gkGameObject::detachFromGroup(void)
{
}


gkGameObject *gkGameObject::getGroupParent(void)
{
	return m_instance ? m_instance->getOwner() : 0;
}


gkObject *gkGameObject::clone(const gkString &name)
{
	gkGameObject *ob = new gkGameObject(m_scene, m_name, m_type);
	cloneImpl(ob);
	return ob;
}


void gkGameObject::cloneImpl(gkGameObject *clob)
{
	clob->m_activeLayer = clob->m_activeLayer;
	memcpy(&clob->m_baseProps, &m_baseProps, sizeof(gkGameObjectProperties));
	clob->m_isClone = true;

	// clone variables
	utHashTableIterator<VariableMap> iter(m_variables);
	while (iter.hasMoreElements())
	{
		gkVariable *ovar = iter.getNext().second;
		gkVariable *nvar = ovar->clone();

		// no debug
		nvar->setDebug(false);
		clob->m_variables.insert(nvar->getName(), nvar);
	}


	if (!m_constraints.empty())
	{
		// clone constraints
		ConstraintIterator  cit = getConstraintIterator();
		while (cit.hasMoreElements())
			clob->m_constraints.push_back(cit.getNext()->clone());
	}

	// clone logic nodes

	// clone logic bricks
	if (m_bricks != 0)
		clob->m_bricks = m_bricks->clone(clob);
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

	applyTransformState(m_baseProps.m_transform);

	//////////////////////////////////
	// Reattach children (ogre) nodes
	gkGameObjectArrayIterator iter(m_children);
	while (iter.hasMoreElements())
	{
		gkGameObject *pChild = iter.getNext();

		if(pChild->m_node)
		{
			Ogre::SceneNode *pParentNode = pChild->m_node->getParentSceneNode();
			if (pParentNode)
			{
				Ogre::Node *pChildNode = pParentNode->removeChild(pChild->m_node);
				m_node->addChild(pChildNode);
			}
			else
				m_node->addChild(pChild->m_node);
		}
	}
	//////////////////////////////////


	// save transform states
	m_cur   = m_baseProps.m_transform;
	m_prev  = m_cur;

	m_node->setInitialState();
}

void gkGameObject::postLoadImpl(void)
{
	loadPhysics();

	// tell scene
	m_scene->notifyObjectLoaded(this);
}

void gkGameObject::postUnloadImpl(void)
{
	destroyPhysics();

	// tell logic
	if (m_bricks)
		m_bricks->unload();

	// tell scene
	m_scene->notifyObjectUnloaded(this);
}


void gkGameObject::unloadImpl(void)
{
	SceneManager *manager = m_scene->getManager();

	if (m_node)
	{
		// Destroy and detach from scene graph
		Ogre::SceneNode *pParentNode = m_node->getParentSceneNode();

		unsigned short n = m_node->numChildren();

		for(unsigned short i=0; i<n; i++)
		{
			Ogre::Node *pChildNode = m_node->removeChild(i);

			pParentNode->addChild(pChildNode);

		}
		manager->destroySceneNode(m_node);
		m_node = 0;
	}

	// Reset variables
	utHashTableIterator<VariableMap> iter(m_variables);
	while (iter.hasMoreElements())
	{
		gkVariable *cvar = iter.getNext().second;
		cvar->reset();
	}
}



bool gkGameObject::hasSensorMaterial(const gkString& name, bool onlyFirst)
{
	gkEntity *ent = getEntity();
	if (ent)
	{
		gkMesh *me = ent->getEntityProperties().m_mesh;
		if (me)
		{
			if (onlyFirst)
				return me->getFirstMaterial().m_name == name;
			else
			{
				gkMesh::SubMeshIterator iter = me->getSubMeshIterator();
				while (iter.hasMoreElements())
				{
					gkSubMesh *sme = iter.getNext();
					if (sme->getMaterialName() == name)
						return true;
				}
			}
		}
	}
	return false;
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
	return m_baseProps.m_transform.loc;
}


gkVector3 gkGameObject::getWorldScale(void)
{
	if (m_node != 0)
		return m_node->_getDerivedScale();
	return m_baseProps.m_transform.scl;
}


gkQuaternion gkGameObject::getWorldOrientation(void)
{
	if (m_node != 0)
		return m_node->_getDerivedOrientation();
	return m_baseProps.m_transform.rot;
}


gkEuler gkGameObject::getWorldRotation(void)
{
	if (m_node != 0)
		return m_node->_getDerivedOrientation();
	return m_baseProps.m_transform.rot;
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

		m_scene->notifyObjectUpdate(this);
	}
}

void gkGameObject::applyTransformState(const gkTransformState &newstate)
{
	if (m_node)
	{
		m_node->setPosition(newstate.loc);
		m_node->setOrientation(newstate.rot);
		m_node->setScale(newstate.scl);

		if(m_rigidBody)
		{
			m_rigidBody->setTransformState(newstate);
		}
		else if(m_character)
		{
			m_character->setTransformState(newstate);
		}
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
		{
			m_rigidBody->updateTransform();
		}
		else if(m_character)
		{
			m_character->updateTransform();
		}
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


void gkGameObject::setOrientation(const gkQuaternion &q)
{
	if (m_node != 0)
	{
		m_node->setOrientation(q);
		notifyUpdate();

		// update the rigid body state
		if (m_rigidBody != 0)
		{
			m_rigidBody->updateTransform();
		}
		else if(m_character)
		{
			m_character->updateTransform();
		}
	}
}


void gkGameObject::setOrientation(const gkEuler &v)
{
	if (m_node != 0)
	{
		gkQuaternion q = v.toQuaternion();
		m_node->setOrientation(q);
		notifyUpdate();

		// update the rigid body state
		if (m_rigidBody != 0)
		{
			m_rigidBody->updateTransform();
		}
		else if(m_character)
		{
			m_character->updateTransform();
		}
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
		{
			m_rigidBody->updateTransform();
		}
		else if(m_character)
		{
			m_character->updateTransform();
		}
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
		{
			m_rigidBody->updateTransform();
		}
		else if(m_character)
		{
			m_character->updateTransform();
		}
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
		{
			m_rigidBody->updateTransform();
		}
		else if(m_character)
		{
			m_character->updateTransform();
		}
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
		{
			m_rigidBody->updateTransform();
		}
		else if(m_character)
		{
			m_character->updateTransform();
		}
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
		{
			m_rigidBody->updateTransform();
		}
		else if(m_character)
		{
			m_character->updateTransform();
		}
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
	{
		m_rigidBody->setLinearVelocity(v, tspace);
	}
	else if(m_character)
	{
		m_character->setVelocity(m_node->getOrientation() * v, gkEngine::getStepRate());
	}
}

void gkGameObject::setAngularVelocity(const gkVector3 &v, int tspace)
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

void gkGameObject::setParent(gkGameObject *par)
{
	GK_ASSERT(par != this);

	if(par)
	{
		GK_ASSERT(!m_parent && "Already has a parent");
		par->addChild(this);
	}
}


void gkGameObject::addChild(gkGameObject *gobj)
{
	GK_ASSERT(gobj != this);

	if (gobj)
	{
		GK_ASSERT(!gobj->m_parent && "Already has a parent");
		GK_ASSERT(!hasChild(gobj)  && "Already has this child");

		m_children.push_back(gobj);
		gobj->m_parent = this;


		if (isLoaded())
		{
			GK_ASSERT(m_node);

			if (!gobj->isLoaded())
				gobj->load();

			GK_ASSERT(gobj->getNode());

			if (gobj->getProperties().isPhysicsObject())
			{
				// Only root objects at the moment, anything else is undefined.
				gobj->destroyPhysics();
			}


			Ogre::SceneNode *node = gobj->getNode();

			if (node->getParentSceneNode())
				node->getParentSceneNode()->removeChild(node);


			m_node->addChild(gobj->getNode());
		}
	}
}


void gkGameObject::removeChild(gkGameObject *gobj)
{
	GK_ASSERT(gobj != this);

	if (gobj && hasChild(gobj))
	{
		GK_ASSERT(gobj->m_parent         && "Missing parent");
		GK_ASSERT(gobj->m_parent == this && "Parent mismatch");

		gobj->m_parent = 0;
		m_children.erase(gobj);



		// place in parent ogre node

		if (gobj->isLoaded())
		{
			Ogre::SceneNode *node = gobj->getNode();

			
			GK_ASSERT(node->getParentSceneNode() == m_node && "Parent mismatch");

			m_node->removeChild(node);
			m_scene->getManager()->getRootSceneNode()->addChild(node);

			// Re-enable physics
			if (gobj->getProperties().isPhysicsObject())
				gobj->loadPhysics();
		}
	}
}


bool gkGameObject::hasChild(gkGameObject *gobj)
{
	return m_children.find(gobj) != UT_NPOS;
}


gkObject *gkGameObject::getAttachedObject()
{
	if(m_rigidBody)
		return m_rigidBody;
	else if(m_character)
		return m_character;
	else
		return 0;
}

btCollisionObject *gkGameObject::getCollisionObject()
{
	if(m_rigidBody)
		return m_rigidBody->getCollisionObject();
	else if(m_character)
		return m_character->getCollisionObject();
	else
		return 0;
}

Ogre::AxisAlignedBox gkGameObject::getAabb() const
{
	return m_node->_getWorldAABB();
}


gkGameObject *gkGameObject::getChildEntity()
{
	gkGameObjectArrayIterator iter(m_children);

	while (iter.hasMoreElements())
	{
		gkGameObject *pChild = iter.getNext();

		if(pChild->m_type == GK_ENTITY)
		{
			return pChild;
		}
		else if(pChild->m_type == GK_SKELETON)
		{
			return pChild->getChildEntity();
		}
	}

	return 0;
}


void gkGameObject::loadPhysics(void)
{
	gkGameObjectProperties  &props  = getProperties();

	gkEntity *ent = getEntity();
	if (ent && props.m_physics.isMeshShape())
	{
		// can happen with the newer mesh objects
		// which test for the collision face flag on each triangle

		gkMesh *me = ent->getEntityProperties().m_mesh;
		btTriangleMesh *trimesh = me->getTriMesh();

		if (trimesh->getNumTriangles() == 0)
			props.m_physics.m_type = GK_NO_COLLISION;
	}


	if (!props.isPhysicsObject() || m_parent)
		return;


	if (props.isGhost())
	{
		m_character = m_scene->getDynamicsWorld()->createCharacter(this);
		m_character->load();
	}
	else
	{

		m_rigidBody = m_scene->getDynamicsWorld()->createRigidBody(this);
		m_rigidBody->load();
	}
}


void gkGameObject::destroyPhysics(void)
{
	if (m_character)
	{
		m_scene->getDynamicsWorld()->destroyObject(m_character);
		m_character = 0;
	}
	if (m_rigidBody)
	{
		m_scene->getDynamicsWorld()->destroyObject(m_rigidBody);
		m_rigidBody = 0;
	}
}
