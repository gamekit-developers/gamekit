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


// ----------------------------------------------------------------------------
gkGameObjectGroup::gkGameObjectGroup(gkGroupManager *manager, const gkHashedString &name)
	:   m_name(name), m_manager(manager), m_geometry(0)
{
	GK_ASSERT(manager);
}


// ----------------------------------------------------------------------------
gkGameObjectGroup::~gkGameObjectGroup()
{
	clearInstances();
}



// ----------------------------------------------------------------------------
void gkGameObjectGroup::addObject(gkGameObject *gobj)
{
	if (!gobj) return;



	const gkHashedString &name = gobj->getName();
	if (m_objects.find(name) != UT_NPOS)
	{
		gkLogMessage("GameObjectGroup: Duplicate object " << name.str() << " found within this group!.");
		return;
	}


	// Mark it as part of a group
	gobj->_makeGroup(this);

	m_objects.insert(name, gobj);
}



// ----------------------------------------------------------------------------
void gkGameObjectGroup::removeObject(gkGameObject *gobj)
{
	if (!gobj) return;



	const gkHashedString &name = gobj->getName();
	if (m_objects.find(name) == UT_NPOS)
	{
		gkLogMessage("GameObjectGroup: Missing object " << name.str() << ". Object was not removed.");
		return;
	}

	// Clear group flag
	gobj->_makeGroup(0);

	m_objects.erase(name);
}


// ----------------------------------------------------------------------------
bool gkGameObjectGroup::hasObject(const gkHashedString &name)
{
	return m_objects.find(name) != UT_NPOS;
}


// ----------------------------------------------------------------------------
gkGameObject *gkGameObjectGroup::getObject(const gkHashedString &name)
{
	UTsize pos = 0;
	if ((pos = m_objects.find(name)) == UT_NPOS)
	{
		gkLogMessage("GameObjectGroup: Missing object " << name.str() << ".");
		return 0;
	}


	return m_objects.at(pos);
}


// ----------------------------------------------------------------------------
void gkGameObjectGroup::clearInstances(void)
{
	// Free all instances

	Instances::Iterator iter = m_instances.iterator();
	while (iter.hasMoreElements())
		delete iter.getNext();


	m_instances.clear();
}


// ----------------------------------------------------------------------------
gkGameObjectInstance *gkGameObjectGroup::createInstance(gkScene *scene)
{
	gkGameObjectInstance *newInst = new gkGameObjectInstance(this, scene, m_manager->_getHandle());


	// Owner is local only to the instance
	GK_ASSERT(newInst->getOwner());

	m_instances.insert(newInst);


	// push new objects


	Objects::Iterator iter = m_objects.iterator();
	while (iter.hasMoreElements())
	{
		// Adds a carbon copy.
		newInst->addObject(iter.getNext().second);
	}


	return newInst;
}

// ----------------------------------------------------------------------------
void gkGameObjectGroup::destroyInstance(gkGameObjectInstance *inst)
{

	UTsize pos;

	if ((pos = m_instances.find(inst)) == UT_NPOS )
	{
		gkLogMessage("GameObjectGroup: Missing instance. Instance not destroyed");
		return;
	}

	delete m_instances.at(pos);
	m_instances.erase(inst);
}



// ----------------------------------------------------------------------------
void gkGameObjectGroup::loadInstances(void)
{
	Instances::Iterator it = m_instances.iterator();
	while (it.hasMoreElements())
		it.getNext()->load();
}


// ----------------------------------------------------------------------------
void gkGameObjectGroup::unloadInstances(void)
{
	Instances::Iterator it = m_instances.iterator();
	while (it.hasMoreElements())
		it.getNext()->unload();
}


// ----------------------------------------------------------------------------
gkGameObjectInstance *gkGameObjectGroup::findInstanceBy(UTsize id)
{
	gkGameObjectInstance *ret = 0;

	Instances::Iterator it = m_instances.iterator();
	while (it.hasMoreElements())
	{
		ret = it.getNext();
		if (ret->getId() == id)
			break;
		else ret = 0;
	}

	return ret;
}




// ----------------------------------------------------------------------------
void gkGameObjectGroup::cloneObjects(gkScene *scene,
									 const gkTransformState &from,
                                     int time,
                                     const gkVector3 &linearVelocity, bool tsLinLocal,
                                     const gkVector3 &angularVelocity, bool tsAngLocal)
{

	const gkMatrix4 plocal = from.toMatrix();


	Objects::Iterator iter = m_objects.iterator();
	while (iter.hasMoreElements())
	{
		gkGameObject *oobj = iter.getNext().second;
		gkGameObject *nobj = scene->cloneObject(oobj, time);



		// be sure this info was not cloned!
		GK_ASSERT(!nobj->isInstance() && !nobj->isInGroup() && !oobj->isClone());

		// Update transform relitave to owner
		gkGameObjectProperties &props = nobj->getProperties();

		gkMatrix4 clocal;
		props.m_transform.toMatrix(clocal);

		// merge back to transform state
		props.m_transform = gkTransformState(plocal * clocal);


		nobj->load();


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



// ----------------------------------------------------------------------------
void gkGameObjectGroup::createStaticBatches(gkScene *scene)
{
	/// this only works for truly static objects.
	/// Things like grass, tree leaves, or basically
	/// any entity that does not respond to collisions (GK_NO_COLLISION).



	Ogre::SceneManager *mgr = scene->getManager();
	if (m_geometry)
		mgr->destroyStaticGeometry(m_geometry);

	m_geometry = 0;


	// Span all instances.


	Instances::Iterator it = m_instances.iterator();
	while (it.hasMoreElements())
	{
		gkGameObjectInstance *inst = it.getNext();


		gkGameObjectInstance::Objects::Iterator instIt = inst->getObjects().iterator();
		while (instIt.hasMoreElements())
		{
			gkGameObject *obj = instIt.getNext().second;
			obj->load();


			if (obj->getType()==GK_ENTITY)
			{
				const gkGameObjectProperties &props = obj->getProperties();

				if (!props.isPhysicsObject())
				{
					gkEntity *ent = obj->getEntity();

					if (!m_geometry)
						m_geometry = mgr->createStaticGeometry(m_name.str());


					m_geometry->addEntity(ent->getEntity(),
					                      obj->getWorldPosition(),
					                      obj->getWorldOrientation(),
					                      obj->getWorldScale());

					// no longer needed
					ent->_unloadAsInstance();
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


// ----------------------------------------------------------------------------
void gkGameObjectGroup::destroyStaticBatches(gkScene *scene)
{
	bool isSceneUnloading = scene->isUnloading();

	if (!isSceneUnloading)
	{
		if (m_geometry)
		{
			Ogre::SceneManager *mgr = scene->getManager();
			mgr->destroyStaticGeometry(m_geometry);
		}
	}
	else
	{
		// TODO: reload entities.
		m_geometry = 0;
	}
}
