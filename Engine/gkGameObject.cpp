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
#include "gkConstraintManager.h"
#include "gkGameObjectGroup.h"
#include "gkRigidBody.h"
#include "gkCharacter.h"
#include "gkDynamicsWorld.h"
#include "gkMesh.h"
#include "gkVariable.h"


using namespace Ogre;




gkGameObject::gkGameObject(gkScene *scene, const gkString &name, gkGameObjectTypes type)
	:	gkObject(name),
	    m_type(type),
	    m_baseProps(),
	    m_parent(0),
	    m_scene(scene),
	    m_node(0),
	    m_logic(0), // FIXME: Detach
 	    m_bricks(0), // FIXME: Detach
	    m_rigidBody(0),
	    m_character(0),
	    m_groupID(0),
		m_group(0),
	    m_state(0),
	    m_activeLayer(true),
	    m_layer(0xFFFFFFFF),
	    m_isClone(false),
	    m_flags(0)
{
	GK_ASSERT(m_scene);


	m_life.tick = 0;
	m_life.timeToLive = 0;
}




gkGameObject::~gkGameObject()
{
	clearVariables();


	if (m_scene)
	{
		// TODO, should be the same for bricks and nodes.
		// (No game object internal dependencies)!

		gkConstraintManager *mgr = m_scene->getConstraintManager();
		mgr->notifyObjectDestroyed(this);
	}


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



gkGameObject *gkGameObject::clone(const gkString &name)
{
	gkGameObject *ob = new gkGameObject(m_scene, name, m_type);
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



	if (m_scene)
	{
		gkConstraintManager *mgr = m_scene->getConstraintManager();
		mgr->notifyObjectCloned(this, clob);
	}


	// clone logic nodes



	// clone logic bricks
	if (m_bricks != 0)
		clob->m_bricks = m_bricks->clone(clob);
}




void gkGameObject::createInstanceImpl(void)
{
	SceneManager *manager = m_scene->getManager();
	SceneNode *parentNode = 0;


	if (!m_scene->isBeingCreated())
	{
		// Scene is not currently loading, meaning
		// a specific object is being loaded. (maintain previous parent)

		if (m_parent)
		{
			if (!m_parent->isInstanced())
				m_parent->createInstance();

			parentNode = m_parent->m_node;
			GK_ASSERT(parentNode);
		}
	}

	m_node = parentNode ? parentNode->createChildSceneNode(m_name) : manager->getRootSceneNode()->createChildSceneNode(m_name);


	applyTransformState(m_baseProps.m_transform);


	if (!m_scene->isBeingCreated())
	{
		// Reattach children (ogre) nodes
		gkGameObjectArray::Iterator iter = m_children.iterator();
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
	}

	m_node->setInitialState();
}




void gkGameObject::postCreateInstanceImpl(void)
{
	// tell scene
	m_scene->notifyInstanceCreated(this);
}



void gkGameObject::postDestroyInstanceImpl(void)
{
	// tell logic
	if (m_bricks)
		m_bricks->destroyInstance();


	// tell scene
	m_scene->notifyInstanceDestroyed(this);

	if (m_scene->isBeingDestroyed())
	{
		m_parent = 0;
		m_children.clear();
	}
}




void gkGameObject::destroyInstanceImpl(void)
{
	SceneManager *manager = m_scene->getManager();


	if (!m_scene->isBeingDestroyed())
	{
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
		}
	}

	m_node = 0;


	// Reset variables
	utHashTableIterator<VariableMap> iter(m_variables);
	while (iter.hasMoreElements())
	{
		gkVariable *cvar = iter.getNext().second;
		cvar->reset();
	}
}




bool gkGameObject::hasSensorMaterial(const gkString &name, bool onlyFirst)
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
	if (!isInstanced())
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



const gkTransformState &gkGameObject::getTransformState(void)
{
	static gkTransformState m_state;
	m_state.loc = getPosition();
	m_state.rot = getOrientation();
	m_state.scl = getScale();
	return m_state;
}



const gkMatrix4 &gkGameObject::getTransform(void)
{
	static gkMatrix4 temp;
	if (m_node != 0)
	{
		getTransformState().toMatrix(temp);
		return temp;
	}
	return gkMatrix4::IDENTITY;
}



const gkVector3 &gkGameObject::getPosition(void)
{
	if (m_node != 0)
		return m_node->getPosition();
	return m_baseProps.m_transform.loc;
}


const gkVector3 &gkGameObject::getScale(void)
{
	if (m_node != 0)
		return m_node->getScale();
	return m_baseProps.m_transform.scl;
}



const gkQuaternion &gkGameObject::getOrientation(void)
{
	if (m_node != 0)
		return m_node->getOrientation();
	return m_baseProps.m_transform.rot;
}



gkEuler gkGameObject::getRotation(void)
{
	return gkEuler(getOrientation());
}


const gkTransformState &gkGameObject::getWorldTransformState(void)
{
	static gkTransformState m_state;
	m_state.loc = getWorldPosition();
	m_state.rot = getWorldOrientation();
	m_state.scl = getWorldScale();
	return m_state;
}


const gkMatrix4 &gkGameObject::getWorldTransform(void)
{
	if (m_node != 0)
		return m_node->_getFullTransform();
	return gkMatrix4::IDENTITY;
}



const gkVector3 &gkGameObject::getWorldPosition(void)
{
	if (m_node != 0)
		return m_node->_getDerivedPosition();
	return m_baseProps.m_transform.loc;
}


const gkVector3 &gkGameObject::getWorldScale(void)
{
	if (m_node != 0)
		return m_node->_getDerivedScale();
	return m_baseProps.m_transform.scl;
}



const gkQuaternion &gkGameObject::getWorldOrientation(void)
{
	if (m_node != 0)
		return m_node->_getDerivedOrientation();
	return m_baseProps.m_transform.rot;
}



gkEuler gkGameObject::getWorldRotation(void)
{
	return gkEuler(getWorldOrientation());
}



void gkGameObject::notifyUpdate(void)
{
	if (m_scene)
		m_scene->notifyObjectUpdate(this);
}




void gkGameObject::applyTransformState(const gkTransformState &newstate)
{
	if (isImmovable())
		return;

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



void gkGameObject::setTransform(const gkMatrix4 &v)
{
	if (isImmovable())
		return;

	if (m_node != 0)
	{
		gkTransformState st;
		gkMathUtils::extractTransform(v, st.loc, st.rot, st.scl);
		applyTransformState(st);
		notifyUpdate();
	}
}



void gkGameObject::setTransform(const gkTransformState &v)
{
	if (isImmovable())
		return;

	applyTransformState(v);
	notifyUpdate();
}




void gkGameObject::setPosition(const gkVector3 &v)
{
	if (isImmovable())
		return;

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
	if (isImmovable())
		return;

	if (m_node != 0)
	{
		m_node->setScale(v);
		notifyUpdate();
	}
}




void gkGameObject::setOrientation(const gkQuaternion &q)
{
	if (isImmovable())
		return;

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
	if (isImmovable())
		return;

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
	if (isImmovable())
		return;

	// convert to quaternion and rotate normally
	rotate(drot.toQuaternion(), tspace);
}



void gkGameObject::rotate(const gkQuaternion &dq, int tspace)
{
	if (isImmovable())
		return;

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
	if (isImmovable())
		return;

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
	if (isImmovable())
		return;

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
	if (isImmovable())
		return;

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
	if (isImmovable())
		return;

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
	if (isImmovable())
		return;

	if (m_node != 0)
	{
		m_node->scale(dscale);
		notifyUpdate();
	}
}



void gkGameObject::setLinearVelocity(const gkVector3 &v, int tspace)
{
	if (isImmovable())
		return;

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
	if (isImmovable())
		return;

	if (m_rigidBody != 0)
		m_rigidBody->setAngularVelocity(v, tspace);
}



void gkGameObject::applyTorque(const gkVector3 &v, int tspace)
{
	if (isImmovable())
		return;

	if (m_rigidBody != 0)
		m_rigidBody->applyTorque(v, tspace);
}



void gkGameObject::applyForce(const gkVector3 &v, int tspace)
{
	if (isImmovable())
		return;

	if (m_rigidBody != 0)
		m_rigidBody->applyForce(v, tspace);
}



gkVector3 gkGameObject::getLinearVelocity(void)
{
	if (isImmovable())
		return gkVector3::ZERO;

	if (m_rigidBody != 0)
		return m_rigidBody->getLinearVelocity();
	return gkVector3::ZERO;
}




gkVector3 gkGameObject::getAngularVelocity()
{
	if (isImmovable())
		return gkVector3::ZERO;

	if (m_rigidBody != 0)
		return m_rigidBody->getAngularVelocity();
	return gkVector3::ZERO;
}




void gkGameObject::clearVariables(void)
{

	// Destroy all variables.
	gkEngine &eng = gkEngine::getSingleton();
	utHashTableIterator<VariableMap> iter(m_variables);


	while (iter.hasMoreElements())
	{
		gkVariable *v = iter.getNext().second;


		// remove from debug list
		if (v->isDebug())
			eng.removeDebugProperty(v);

		delete v;
	}

	m_variables.clear();
}




gkVariable *gkGameObject::createVariable(const gkString &name, bool debug)
{
	gkHashedString findName(name);


	UTsize pos = m_variables.find(findName);
	if (pos != UT_NPOS)
	{
		gkPrintf("Duplicate property found");
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

	UTsize pos = m_variables.find(name);
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
	if (!isInstanced() || isBeingCreated() || (par && (!par->isInstanced() || par->isBeingCreated())))
		return;

	if(par && par != this)
	{
		GK_ASSERT(!m_parent && "Already has a parent");
		par->addChild(this);
	}
}


void gkGameObject::setParentInPlace(gkGameObject *par)
{
	if(par && par != this)
	{
		GK_ASSERT(!m_parent && "Already has a parent");
		
		gkMatrix4 trans =  par->getWorldTransform().inverse() * getWorldTransform();
		
		setParent(par);
		setTransform(trans);
	}
}



void gkGameObject::clearParent()
{
	if (!isInstanced() || isBeingCreated())
		return;

	if(m_parent)
	{
		m_parent->removeChild(this);
	}
}


void gkGameObject::clearParentInPlace()
{
	if(m_parent)
	{
		gkMatrix4 trans =  m_parent->getWorldTransform()*getTransform();
		
		clearParent();
		setTransform(trans);
	}
}


void gkGameObject::addChild(gkGameObject *gobj)
{
	if (!isInstanced() || isBeingCreated() || (gobj && (!gobj->isInstanced() || gobj->isBeingCreated())))
		return;

	if (gobj && gobj != this)
	{
		GK_ASSERT(!gobj->m_parent && "Already has a parent");
		GK_ASSERT(!hasChild(gobj)  && "Already has this child");

		m_children.push_back(gobj);
		gobj->m_parent = this;

		if (!gobj->isInstanced())
			gobj->createInstance();

		// Suspend child updates.
		gkPhysicsController *cont = gobj->getPhysicsController();
		if (cont)
			cont->suspend(true);

		Ogre::SceneNode *node = gobj->getNode();
		if (node->getParentSceneNode())
			node->getParentSceneNode()->removeChild(node);

		m_node->addChild(gobj->getNode());
	}
}



void gkGameObject::removeChild(gkGameObject *gobj)
{
	if (!isInstanced() || isBeingCreated() || (gobj && (!gobj->isInstanced() || gobj->isBeingCreated())))
		return;

	if (gobj &&  gobj != this && hasChild(gobj))
	{
		GK_ASSERT(gobj->m_parent         && "Missing parent");
		GK_ASSERT(gobj->m_parent == this && "Parent mismatch");

		gobj->m_parent = 0;
		m_children.erase(gobj);


		// place in parent ogre node
		Ogre::SceneNode *node = gobj->getNode();
		GK_ASSERT(node->getParentSceneNode() == m_node && "Parent mismatch");

		m_node->removeChild(node);



		Ogre::SceneNode *pNode = m_node->getParentSceneNode();
		if (pNode)
			pNode->addChild(node);
		else
			m_scene->getManager()->getRootSceneNode()->addChild(node);

		// Re-enable physics

		gkPhysicsController *cont = gobj->getPhysicsController();
		if (cont)
		{
			cont->suspend(false);
			cont->updateTransform();
		}
	}
}



bool gkGameObject::hasChild(gkGameObject *gobj)
{
	return m_children.find(gobj) != UT_NPOS;
}




gkPhysicsController *gkGameObject::getPhysicsController()
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
	gkGameObjectArray::Iterator iter = m_children.iterator();
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
