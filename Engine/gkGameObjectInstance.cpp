/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Charlie C.

    Contributor(s): Thomas Trocha (dertom)
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
#include "gkGameObjectInstance.h"
#include "gkGameObjectGroup.h"
#include "gkGameObject.h"
#include "gkGameObjectManager.h"
#include "gkScene.h"
#include "gkUtils.h"
#include "gkLogger.h"
#include "gkValue.h"
#include "gkSkeleton.h"
#include "gkEntity.h"
#include "gkBone.h"
#include "gkSkeletonResource.h"
#include "gkGroupManager.h"


gkGameObjectInstance::gkGameObjectInstance(gkInstancedManager* creator, const gkResourceName& name, const gkResourceHandle& handle)
	:    gkInstancedObject(creator, name, handle),
	     m_parent(0), m_scene(0), m_owner(0),
	     m_uidName(gkString("/UID{" + gkToString((int)handle) + "}")),
	     m_layer(0xFFFFFFFF)

{
}


gkGameObjectInstance::~gkGameObjectInstance()
{
	if (m_owner != 0)
		gkGameObjectManager::getSingleton().destroy(m_owner->getResourceHandle());

	m_owner = 0;


	Objects::Iterator iter = m_objects.iterator();
	while (iter.hasMoreElements())
	{
		gkGameObject* gobj = iter.getNext().second;
		m_objInitialTransformstates.remove(gobj);
		GK_ASSERT(!gobj->isInstanced());
		delete gobj;
	}

	m_objects.clear();
}

void gkGameObjectInstance::_setExternalRoot(gkGameObjectGroup* group, gkGameObject* root)
{
	GK_ASSERT(!m_parent);

	if (!m_parent)
	{
		m_parent = group;
		m_owner = root;

		if (root != 0)
			root->addEventListener(this);

	}
}

void gkGameObjectInstance::_updateFromGroup(gkGameObjectGroup* group)
{
	GK_ASSERT(!m_parent);

	if (!m_parent)
	{
		m_parent = group;


		// create the root object
		m_owner = gkGameObjectManager::getSingleton().createObject(m_name);
		if (m_owner != 0)
			m_owner->addEventListener(this);

	}
}



void gkGameObjectInstance::addObject(gkGameObject* gobj)
{
	if (!gobj)
		return;

	const gkResourceNameString& name = gobj->getName() + m_uidName;


	if (m_objects.find(name) != UT_NPOS)
	{
		gkLogMessage("GameObjectInstance: Duplicate object " << name.str() << ".");
		return;
	}

	gkGameObject* ngobj = gobj->cloneToScene(name.str(),m_owner->getOwner());

	// store the inital-transformstates
	gkTransformState initalTransformState = ngobj->getTransformState();
	m_objInitialTransformstates.insert(ngobj,initalTransformState);

	// modify the parent (if there is one) to fit the group's uid
	gkString propParent = ngobj->getProperties().m_parent;
	if (!propParent.empty())
	{
		ngobj->getProperties().m_parent = propParent + m_uidName;
	}



	//	store the new object using the original name and not the uid-name
	//  making it easier to lookup an object by name 
	m_objects.insert(gobj->getName(), ngobj);


	// Lightly attach
	ngobj->_makeGroupInstance(this);

	gkScene* instanceScene = getRoot()->getOwner();
	ngobj->setActiveLayer((instanceScene->getLayer() & m_layer) != 0);
	ngobj->setLayer(m_layer);
}

void gkGameObjectInstance::addGroupInstance(const gkGameObjectGroup::GroupInstance* grpDesc)
{
	if (!grpDesc)
		return;


	gkGroupManager* mgr = gkGroupManager::getSingletonPtr();
	if (mgr->exists(grpDesc->m_groupName))
	{
		gkGameObjectGroup* ggobj = (gkGameObjectGroup*)mgr->getByName(grpDesc->m_groupName);

		gkScene* toScene = m_owner->getOwner();
		// clone the root with all of its properties and logicbricks
		gkGameObject* rootClone = grpDesc->m_root->clone(gkUtils::getUniqueName(grpDesc->m_root->getName()));
		gkGameObjectInstance* inst = ggobj->createGroupInstance(toScene, gkResourceName(gkUtils::getUniqueName("gi"+grpDesc->m_root->getName()), ""), rootClone, toScene->getLayer());

		gkGameObject* instRoot = inst->getRoot();

		// store the inital-transformstates
		gkTransformState initalTransformState = instRoot->getTransformState();
		m_objInitialTransformstates.insert(instRoot,initalTransformState);

		m_groupInstances.insert(grpDesc->m_root->getName(), inst);

		// modify the parent (if there is one) to fit the group's uid
		gkString propParent = instRoot->getProperties().m_parent;
		if (!propParent.empty())
		{
			instRoot->getProperties().m_parent = propParent + m_uidName;
		}
	}

}


gkGameObject* gkGameObjectInstance::getObject(const gkHashedString& name)
{
	UTsize pos;
	if ((pos = m_objects.find(name)) == UT_NPOS)
	{
		gkLogMessage("GameObjectInstance: Missing object " << name.str() << ".");
		return 0;
	}
	return m_objects.at(pos);
}


void gkGameObjectInstance::destroyObject(gkGameObject* gobj)
{
	if (!gobj)
		return;

	if (gobj->getGroupInstance() != this)
	{
		gkLogMessage("GameObjectInstance: Attempting to remove an object that does not belong to this instance!");
		return;
	}


	const gkHashedString name = gobj->getName();

	UTsize pos;
	if ((pos = m_objects.find(name)) == UT_NPOS)
	{
		gkLogMessage("GameObjectInstance: Missing object " << name.str() << ".");
		return;
	}

	m_objects.remove(name);
	delete gobj;
}




void gkGameObjectInstance::destroyObject(const gkHashedString& name)
{

	UTsize pos;
	if ((pos = m_objects.find(name)) == UT_NPOS)
	{
		gkLogMessage("GameObjectInstance: Missing object " << name.str() << ".");
		return;
	}

	gkGameObject* gobj = m_objects.at(pos);
	gobj->destroyInstance();


	m_objects.remove(name);
	delete gobj;
}




bool gkGameObjectInstance::hasObject(const gkHashedString& name)
{
	return m_objects.find(name) != UT_NPOS;
}



void gkGameObjectInstance::applyTransform(const gkTransformState& trans)
{
	if (!isInstanced())
		return;


	const gkMatrix4 plocal = trans.toMatrix();

	Objects::Iterator iter = m_objects.iterator();
	while (iter.hasMoreElements())
	{
		gkGameObject* obj = iter.getNext().second;

		if (obj->hasParent())
		{
			continue;
		}

		gkTransformState* initalTransformState = m_objInitialTransformstates.get(obj);

		// Update transform relative to owner
		gkMatrix4 clocal;
		initalTransformState->toMatrix(clocal);
		obj->setTransform(plocal * clocal);
	}

	GroupInstances::Iterator grpInstIter = m_groupInstances.iterator();
	while (grpInstIter.hasMoreElements())
	{
		gkGameObjectInstance* ginst = grpInstIter.getNext().second;

		gkGameObject* obj = ginst->getRoot();
		if (obj->hasParent())
		{
			continue;
		}

		gkTransformState* initalTransformState = m_objInitialTransformstates.get(obj);

		// Update transform relative to owner
		gkMatrix4 clocal;
		initalTransformState->toMatrix(clocal);
		obj->setTransform(plocal * clocal);
	}

}



bool gkGameObjectInstance::hasObject(gkGameObject* gobj)
{
	return gobj && m_objects.find(gobj->getName()) && gobj->getGroupInstance() == this;
}


void gkGameObjectInstance::cloneObjects(gkScene* scene, const gkTransformState& from, int time,
                                        const gkVector3& linearVelocity, bool tsLinLocal,
                                        const gkVector3& angularVelocity, bool tsAngLocal)
{
	if (!isInstanced())
		return;

	GK_ASSERT(scene);


	const gkMatrix4 plocal = from.toMatrix();


	Objects::Iterator iter = m_objects.iterator();
	while (iter.hasMoreElements())
	{
		gkGameObject* oobj = iter.getNext().second;
		gkGameObject* nobj = scene->cloneObject(oobj, time);



		// be sure this info was not cloned!
		GK_ASSERT(!nobj->isGroupInstance());


		// Update transform relative to owner
		gkGameObjectProperties& props = nobj->getProperties();

		gkMatrix4 clocal;
		props.m_transform.toMatrix(clocal);

		// merge back to transform state
		props.m_transform = gkTransformState(plocal * clocal);


		nobj->createInstance();


		if (props.isRigidOrDynamic() || props.isGhost())
		{
			if (!linearVelocity.isZeroLength())
				nobj->setLinearVelocity(linearVelocity, tsLinLocal ? TRANSFORM_LOCAL : TRANSFORM_PARENT);
		}

		if (props.isRigid())
		{
			if (!angularVelocity.isZeroLength())
				nobj->setAngularVelocity(angularVelocity, tsAngLocal ? TRANSFORM_LOCAL : TRANSFORM_PARENT);
		}
	}

}


void gkGameObjectInstance::createInstanceImpl(void)
{
	if (!m_owner || !m_owner->getOwner())
	{
		m_instanceState = ST_ERROR;
		m_instanceError = "Root object is not in any scene!";
		return;
	}


	gkScene* scene = m_owner->getOwner();

	m_owner->createInstance();
	gkGameObjectSet parentingPhysicsObjs;


	// first check for entities that are parented to a skeleton
	{
		Objects::Iterator iter = m_objects.iterator();
		while (iter.hasMoreElements())
		{
			gkGameObject* gobj = iter.getNext().second;
			if (gobj->getType() == GK_ENTITY) {
				gkString parentName = gobj->getProperties().m_parent;

				gkGameObject* parent = NULL;
				Objects::Iterator iter2 = m_objects.iterator();
				while (iter2.hasMoreElements())
				{
					gkGameObject* pobj = iter2.getNext().second;
					if (pobj->getName()==parentName)
					{
						parent = pobj;
						break;
					}
				}

				if (parent && parent->getType()==GK_SKELETON){
					gkSkeleton* skel;
					skel = static_cast<gkSkeleton*>(parent);
					gkEntity* ent;
					ent = static_cast<gkEntity*>(gobj);

					if (!gobj->getProperties().hasBoneParent())
					{
						ent->setSkeleton(skel);

						gkSkeletonResource* skelRes = skel->getInternalSkeleton();

						gkBone::BoneList::Iterator roots = skelRes->getRootBoneList().iterator();

					}
				}

			}
		}

	}
	// now instantiate the group-instance-objects
	{
		Objects::Iterator iter = m_objects.iterator();
		while (iter.hasMoreElements())
		{
			gkGameObject* gobj = iter.getNext().second;

			gobj->setOwner(scene);
			gobj->createInstance();


			parentingPhysicsObjs.insert(gobj);
		}
	}

	// instantiate nested group-instances
	{
		GroupInstances::Iterator iter = m_groupInstances.iterator();
		while (iter.hasMoreElements()){
			gkGameObjectInstance* inst = iter.getNext().second;
			inst->createInstance();
			// add the group-instances root to be checked for parenting purposes
			parentingPhysicsObjs.insert(inst->getRoot());
		}
	}
	scene->_applyBuiltinParents(parentingPhysicsObjs);
	scene->_applyBuiltinPhysics(parentingPhysicsObjs);

}

void gkGameObjectInstance::postCreateInstanceImpl(void)
{



	applyTransform(m_owner->getTransformState());
}


void gkGameObjectInstance::destroyInstanceImpl(void)
{
	if (!m_owner || !m_owner->getOwner())
	{
		m_instanceState = ST_ERROR;
		m_instanceError = "Root object is not in any scene!";
		return;
	}


	Objects::Iterator iter = m_objects.iterator();
	while (iter.hasMoreElements())
	{
		gkGameObject* gobj = iter.getNext().second;

		gobj->destroyInstance();
		gobj->setOwner(0);
		gobj->setParent(0);
	}

	GroupInstances::Iterator grpInstIter = m_groupInstances.iterator();
	while (grpInstIter.hasMoreElements())
	{
		gkGameObjectInstance* gobj = grpInstIter.getNext().second;
		gobj->destroyInstance();
	}

	m_owner->destroyInstance();
}

void gkGameObjectInstance::postDestroyInstanceImpl() {
	m_owner->getOwner()->notifyGroupInstanceDestroyed(this);
}

void gkGameObjectInstance::notifyGameObjectEvent(gkGameObject* gobj, const gkGameObject::Notifier::Event& id)
{
	GK_ASSERT(gobj == m_owner);

	if (id == gkGameObject::Notifier::UPDATED)
		applyTransform(gobj->getTransformState());
}
