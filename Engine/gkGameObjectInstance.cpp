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
#include "gkGameObjectInstance.h"
#include "gkGameObjectGroup.h"
#include "gkGameObject.h"
#include "gkScene.h"
#include "gkUtils.h"
#include "gkLogger.h"



// ----------------------------------------------------------------------------
gkGameObjectInstance::gkGameObjectInstance(gkGameObjectGroup *group, gkScene *scene, UTsize uid)
	:   gkObject(group->getName().str()),
	    m_id(uid),
	    m_owner(0),
	    m_parent(group),
	    m_scene(scene),
	    m_firstLoad(true)
{
	GK_ASSERT(m_scene && m_parent);


	// Create the owning object
	m_owner = new gkGameObject(m_scene, gkUtils::getUniqueName("GameObjectInstance"), GK_OBJECT);
}


// ----------------------------------------------------------------------------
gkGameObjectInstance::~gkGameObjectInstance()
{
	// free all objects

	Objects::Iterator iter = m_objects.iterator();
	while (iter.hasMoreElements())
	{
		gkGameObject *gobj = iter.getNext().second;
		gobj->unload();
		delete gobj;
	}

	m_objects.clear();

	delete m_owner;
	m_owner = 0;
}



// ----------------------------------------------------------------------------
void gkGameObjectInstance::addObject(gkGameObject *gobj)
{
	if (!gobj)
		return;

	const gkHashedString name = m_parent->getName().str() + "/" + gobj->getName() + "/Handle_" +  Ogre::StringConverter::toString(m_id);


	if (m_objects.find(name) != UT_NPOS)
	{
		gkLogMessage("GameObjectInstance: Duplicate object " << name.str() << ".");
		return;
	}


	gkGameObject *ngobj = gobj->clone(name.str());
	m_objects.insert(name, ngobj);

	// Lightly attach
	ngobj->_makeInstance(this);


	ngobj->setActiveLayer(m_owner->isInActiveLayer());
	ngobj->setLayer(m_owner->getLayer());
}



// ----------------------------------------------------------------------------
gkGameObject *gkGameObjectInstance::getObject(const gkHashedString &name)
{
	UTsize pos;
	if ((pos = m_objects.find(name)) == UT_NPOS)
	{
		gkLogMessage("GameObjectInstance: Missing object " << name.str() << ".");
		return 0;
	}


	return m_objects.at(pos);
}

// ----------------------------------------------------------------------------
void gkGameObjectInstance::destroyObject(gkGameObject *gobj)
{
	if (!gobj)
		return;

	if (gobj->getInstance() != this)
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



// ----------------------------------------------------------------------------
void gkGameObjectInstance::destroyObject(const gkHashedString &name)
{

	UTsize pos;
	if ((pos = m_objects.find(name)) == UT_NPOS)
	{
		gkLogMessage("GameObjectInstance: Missing object " << name.str() << ".");
		return;
	}



	gkGameObject *gobj = m_objects.at(pos);
	gobj->unload();


	m_objects.remove(name);
	delete gobj;
}



// ----------------------------------------------------------------------------
bool gkGameObjectInstance::hasObject(const gkHashedString &name)
{
	return m_objects.find(name) != UT_NPOS;
}


// ----------------------------------------------------------------------------
void gkGameObjectInstance::makeTransform(void)
{
	applyTransform(m_owner->getProperties().m_transform);
}


// ----------------------------------------------------------------------------
void gkGameObjectInstance::applyTransform(const gkTransformState &trans)
{
	const gkMatrix4 plocal = trans.toMatrix();


	Objects::Iterator iter = m_objects.iterator();
	while (iter.hasMoreElements())
	{
		gkGameObject *obj = iter.getNext().second;

		// Update transform relitave to owner
		gkGameObjectProperties &props = obj->getProperties();


		gkMatrix4 clocal;
		props.m_transform.toMatrix(clocal);

		// merge back to transform state
		props.m_transform = gkTransformState(plocal * clocal);
	}

}


// ----------------------------------------------------------------------------
bool gkGameObjectInstance::hasObject(gkGameObject *gobj)
{
	return gobj && m_objects.find(gobj->getName()) && gobj->getInstance() == this;
}



// ----------------------------------------------------------------------------
void gkGameObjectInstance::cloneObjects(const gkTransformState &from,
                                        int time,
                                        const gkVector3 &linearVelocity, bool tsLinLocal,
                                        const gkVector3 &angularVelocity, bool tsAngLocal)
{

	const gkMatrix4 plocal = from.toMatrix();


	Objects::Iterator iter = m_objects.iterator();
	while (iter.hasMoreElements())
	{
		gkGameObject *oobj = iter.getNext().second;
		gkGameObject *nobj = m_scene->cloneObject(oobj, time);



		// be sure this info was not cloned!
		GK_ASSERT(!nobj->isInstance());


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
void gkGameObjectInstance::loadImpl(void)
{
	// call load on all objects


	if (!m_owner->isInActiveLayer())
		return;

	if (m_firstLoad)
	{
		makeTransform();
		m_firstLoad = false;
	}

	Objects::Iterator iter = m_objects.iterator();
	while (iter.hasMoreElements())
		iter.getNext().second->load();
}



void gkGameObjectInstance::unloadImpl(void)
{

	// call unload on all objects


	Objects::Iterator iter = m_objects.iterator();
	while (iter.hasMoreElements())
	{
		iter.getNext().second->unload();
	}

}
