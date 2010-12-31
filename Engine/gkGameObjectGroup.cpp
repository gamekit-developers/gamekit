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
#include "gkGroupManager.h"
#include "gkGameObjectGroup.h"
#include "gkGameObjectInstance.h"
#include "gkGameObject.h"
#include "gkLogger.h"

#include "gkEntity.h"
#include "gkScene.h"

#include "OgreSceneManager.h"
#include "OgreEntity.h"
#include "OgreStaticGeometry.h"



gkGameObjectGroup::InstanceManager::InstanceManager(gkGameObjectGroup* group)
	:    gkInstancedManager("GroupInstanceManager", group->getName()),
	     m_group(group)
{
}


gkResource* gkGameObjectGroup::InstanceManager::createImpl(const gkResourceName& name, const gkResourceHandle& handle)
{
	return new gkGameObjectInstance(this, name, handle);
}


gkGameObjectGroup::gkGameObjectGroup(gkResourceManager* creator, const gkResourceName& name, const gkResourceHandle& handle)
	:   gkResource(creator, name, handle),
	    m_geometry(0)
{
	m_instanceManager = new InstanceManager(this);
}




gkGameObjectGroup::~gkGameObjectGroup()
{
	destroyAllInstances();
	delete m_instanceManager;
	m_instanceManager = 0;

	m_objects.clear();
}



void gkGameObjectGroup::addObject(gkGameObject* gobj)
{
	if (!gobj)
		return;


	const gkHashedString& name = gobj->getName();
	if (m_objects.find(name) != UT_NPOS)
	{
		gkLogMessage("GameObjectGroup: Duplicate object " << name.str() << " found within this group!.");
		return;
	}


	// Mark it as part of a group
	gobj->_makeGroup(this);
	m_objects.insert(name, gobj);
}



void gkGameObjectGroup::destroyObject(gkGameObject* gobj)
{
	if (!gobj)
		return;

	const gkHashedString& name = gobj->getName();
	if (m_objects.find(name) == UT_NPOS)
	{
		gkLogMessage("GameObjectGroup: Missing object " << name.str() << ". Object was not removed.");
		return;
	}

	// Clear group flag
	gobj->destroyInstance();
	gobj->_makeGroup(0);

	m_objects.erase(name);
}



bool gkGameObjectGroup::hasObject(const gkHashedString& name)
{
	return m_objects.find(name) != UT_NPOS;
}



gkGameObject* gkGameObjectGroup::getObject(const gkHashedString& name)
{
	UTsize pos = 0;
	if ((pos = m_objects.find(name)) == UT_NPOS)
	{
		gkLogMessage("GameObjectGroup: Missing object " << name.str() << ".");
		return 0;
	}

	return m_objects.at(pos);
}



void gkGameObjectGroup::destroyAllInstances(void)
{

	if (m_instanceManager)
		m_instanceManager->destroyAll();
}


gkGameObjectInstance* gkGameObjectGroup::createGroupInstance(gkScene* scene, const gkResourceName& name)
{
	GK_ASSERT(m_instanceManager);

	if (m_instanceManager->exists(name))
	{
		gkLogMessage("GameObjectGroup: Duplicate instance '" << name.getName() << "(" << name.getGroup() << ")'");
		return 0;
	}


	gkGameObjectInstance* newInst = m_instanceManager->create<gkGameObjectInstance>(name);
	newInst->_updateFromGroup(this);


	gkGameObject* obj = newInst->getRoot();
	GK_ASSERT(obj);
	if (obj)
		obj->setOwner(scene);



	Objects::Iterator iter = m_objects.iterator();
	while (iter.hasMoreElements())
		newInst->addObject(iter.getNext().second);

	return newInst;
}


void gkGameObjectGroup::destroyGroupInstance(gkGameObjectInstance* inst)
{
	if (!inst || !m_instanceManager->exists(inst->getResourceHandle()))
	{
		gkLogMessage("GameObjectGroup: Missing instance.");
		return;
	}

	inst->destroyInstance();
	m_instanceManager->destroy(inst);
}




void gkGameObjectGroup::createGameObjectInstances(gkScene* scene)
{
	gkResourceManager::ResourceIterator it = m_instanceManager->getResourceIterator();
	while (it.hasMoreElements())
	{
		gkGameObjectInstance* inst = static_cast<gkGameObjectInstance*>(it.getNext().second);
		if (!inst->isInstanced())
		{

			gkGameObject* obj = inst->getRoot();

			if (obj && obj->getOwner() != scene)
				obj->setOwner(scene);

			inst->createInstance();
		}

	}
}



void gkGameObjectGroup::destroyGameObjectInstances(void)
{
	gkResourceManager::ResourceIterator it = m_instanceManager->getResourceIterator();
	while (it.hasMoreElements())
	{
		gkGameObjectInstance* inst = static_cast<gkGameObjectInstance*>(it.getNext().second);

		if (inst->isInstanced())
			inst->destroyInstance();
	}
}


void gkGameObjectGroup::cloneObjects(gkScene* scene,
                                     const gkTransformState& from,
                                     int time,
                                     const gkVector3& linearVelocity, bool tsLinLocal,
                                     const gkVector3& angularVelocity, bool tsAngLocal)
{

	const gkMatrix4 plocal = from.toMatrix();


	Objects::Iterator iter = m_objects.iterator();
	while (iter.hasMoreElements())
	{
		gkGameObject* oobj = iter.getNext().second;
		gkGameObject* nobj = scene->cloneObject(oobj, time);



		// be sure this info was not cloned!
		GK_ASSERT(!nobj->isGroupInstance() && !nobj->isInGroup() && !oobj->isClone());



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




void gkGameObjectGroup::createStaticBatches(gkScene* scene)
{
	Ogre::SceneManager* mgr = scene->getManager();
	if (m_geometry)
		mgr->destroyStaticGeometry(m_geometry);

	m_geometry = 0;


	// Span all instances.


	gkResourceManager::ResourceIterator it = m_instanceManager->getResourceIterator();
	while (it.hasMoreElements())
	{
		gkGameObjectInstance* inst = static_cast<gkGameObjectInstance*>(it.getNext().second);



		gkGameObjectInstance::Objects::Iterator instIt = inst->getObjects().iterator();
		while (instIt.hasMoreElements())
		{
			gkGameObject* obj = instIt.getNext().second;
			obj->createInstance();


			if (obj->getType() == GK_ENTITY)
			{
				const gkGameObjectProperties& props = obj->getProperties();

				if (!props.isPhysicsObject())
				{
					gkEntity* ent = obj->getEntity();

					if (!m_geometry)
						m_geometry = mgr->createStaticGeometry(m_name.getName());


					m_geometry->addEntity(ent->getEntity(),
					                      obj->getWorldPosition(),
					                      obj->getWorldOrientation(),
					                      obj->getWorldScale());

					// no longer needed
					ent->_destroyAsStaticGeometry();
				}
			}
		}
	}

	if (m_geometry)
	{
		m_geometry->build();
		m_geometry->setCastShadows(false);
	}

}



void gkGameObjectGroup::destroyStaticBatches(gkScene* scene)
{
	bool isSceneUnloading = scene->isBeingDestroyed();

	if (!isSceneUnloading)
	{
		if (m_geometry)
		{
			Ogre::SceneManager* mgr = scene->getManager();
			mgr->destroyStaticGeometry(m_geometry);
		}
	}
	else
	{
		// TODO: reinstance entities.
		m_geometry = 0;
	}
}
