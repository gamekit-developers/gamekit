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
#include "OgreEntity.h"
#include "gkEntity.h"
#include "gkScene.h"
#include "gkEngine.h"
#include "gkUserDefs.h"
#include "gkSkeleton.h"
#include "gkMesh.h"



gkEntity::gkEntity(gkInstancedManager* creator, const gkResourceName& name, const gkResourceHandle& handle)
	:    gkGameObject(creator, name, handle, GK_ENTITY),
	     m_entityProps(new gkEntityProperties()),
	     m_entity(0),
	     m_skeleton(0)
{
}




gkEntity::~gkEntity()
{
	delete m_entityProps;
}



void gkEntity::createInstanceImpl(void)
{
	gkGameObject::createInstanceImpl();

	GK_ASSERT(!m_entity);


	if (!m_entityProps->m_mesh)
		return;

	if (m_skeleton)
		m_skeleton->createInstance();

	Ogre::SceneManager* manager = m_scene->getManager();
	m_entity = manager->createEntity(m_name.getName(), m_entityProps->m_mesh->getResourceName().getName(), 
		m_name.getGroup().empty() ? Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME : m_name.getGroup());


	m_entity->setCastShadows(m_entityProps->m_casts);
	m_node->attachObject(m_entity);

	if (m_skeleton)
		m_skeleton->updateFromController();


	if (!m_entityProps->m_startPose.empty())
		_resetPose();

	if (m_baseProps.isInvisible())
		m_node->setVisible(false, false);
}




void gkEntity::destroyInstanceImpl(void)
{
	if (m_entity)
	{

		Ogre::SceneManager* manager = m_scene->getManager();

		if (!m_entityProps->m_startPose.empty())
			_resetPose();

		if (!m_scene->isBeingDestroyed())
		{
			if (m_node)
				m_node->detachObject(m_entity);

			manager->destroyEntity(m_entity);
		}
	}

	m_entity = 0;


	gkGameObject::destroyInstanceImpl();
}


void gkEntity::setSkeleton(gkSkeleton* skel)
{
	if (m_skeleton == 0)
	{
		m_skeleton = skel;
		if (m_skeleton)
			m_skeleton->setController(this);
	}
}



void gkEntity::_destroyAsStaticGeometry(void)
{
	if (m_entity != 0)
	{
		Ogre::SceneManager* manager = m_scene->getManager();

		if (!m_entityProps->m_startPose.empty())
			_resetPose();

		if (m_node)
			m_node->detachObject(m_entity);

		manager->destroyEntity(m_entity);

		m_entity = 0;
	}
}


gkGameObject* gkEntity::clone(const gkString& name)
{
	gkEntity* cl = new gkEntity(getInstanceCreator(), name, -1);

	*cl->m_entityProps = *m_entityProps;

	gkGameObject::cloneImpl(cl);
	return cl;
}




void gkEntity::_resetPose(void)
{
	if (m_skeleton)
	{
		if (!m_entityProps->m_startPose.empty())
		{
			gkAnimationPlayer* act = getAnimationPlayer(m_entityProps->m_startPose);
			
			if(!act)
				act = addAnimation(m_entityProps->m_startPose);
			
			if (act)
			{
				act->setTimePosition(0);
				act->evaluate(0.0f);
			}
		}
	}
}



gkBoundingBox gkEntity::getAabb() const
{
	return m_entityProps->m_mesh ? m_entityProps->m_mesh->getBoundingBox() : gkGameObject::getAabb();
}
