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
#include "gkSceneManager.h"
#include "gkGameObjectGroup.h"
#include "gkLogger.h"



gkGroupManager::gkGroupManager()
	:    gkResourceManager("GroupManager", "Group"),
	     m_handles(0), m_sceneListener(0)
{
}

gkGroupManager::~gkGroupManager()
{
	destroyAll();
}


gkResource* gkGroupManager::createImpl(const gkResourceName& name, const gkResourceHandle& handle)
{
	return new gkGameObjectGroup(this, name, handle);
}



void gkGroupManager::attachGroupToScene(gkScene* scene, gkGameObjectGroup* group)
{
	class Listener : public gkResourceManager::ResourceListener
	{
	public:
		gkGroupManager* m_this;
	public:
		Listener(gkGroupManager* par) : m_this(par) {}

		virtual ~Listener() {}

		void notifyResourceDestroyed(gkResource* res)
		{
			if (gkGroupManager::getSingletonPtr())
				m_this->m_attachements.erase((gkScene*)res);
		}
	};

	if (!m_sceneListener)
	{
		m_sceneListener = new Listener(this);
		gkSceneManager::getSingleton().addResourceListener(m_sceneListener);
	}

	UTsize pos;
	if ((pos = m_attachements.find(scene)) != UT_NPOS)
		m_attachements.at(pos).push_back(group);
	else
	{
		Groups groups;
		groups.push_back(group);
		m_attachements.insert(scene, groups);
	}
}


gkGroupManager::Groups::Iterator gkGroupManager::getAttachedGroupIterator(gkScene* sc)
{
	UTsize pos;
	if ((pos = m_attachements.find(sc)) != UT_NPOS)
		return m_attachements.at(pos).iterator();
	return Groups::Iterator();
}



void gkGroupManager::createGameObjectInstances(gkScene* scene)
{
	UTsize pos;
	if ((pos = m_attachements.find(scene)) != UT_NPOS)
	{
		Groups::Iterator it = m_attachements.at(pos).iterator();
		while (it.hasMoreElements())
			it.getNext()->createGameObjectInstances(scene);
	}
}



void gkGroupManager::destroyGameObjectInstances(gkScene* scene)
{
	UTsize pos;
	if ((pos = m_attachements.find(scene)) != UT_NPOS)
	{
		Groups::Iterator it = m_attachements.at(pos).iterator();
		while (it.hasMoreElements())
			it.getNext()->destroyGameObjectInstances();
	}
}


void gkGroupManager::createStaticBatches(gkScene* scene)
{
	UTsize pos;
	if ((pos = m_attachements.find(scene)) != UT_NPOS)
	{
		Groups::Iterator it = m_attachements.at(pos).iterator();
		while (it.hasMoreElements())
			it.getNext()->createStaticBatches(scene);
	}
}


void gkGroupManager::destroyStaticBatches(gkScene* scene)
{
	UTsize pos;
	if ((pos = m_attachements.find(scene)) != UT_NPOS)
	{
		Groups::Iterator it = m_attachements.at(pos).iterator();
		while (it.hasMoreElements())
			it.getNext()->destroyStaticBatches(scene);
	}

}

void gkGroupManager::notifyDestroyAllImpl(void)
{
	m_attachements.clear();

	if (m_sceneListener)
	{
		if (gkSceneManager::getSingletonPtr())
			gkSceneManager::getSingleton().removeResourceListener(m_sceneListener);

		delete m_sceneListener;
		m_sceneListener = 0;
	}

	gkResourceManager::ResourceIterator it = getResourceIterator();
	while (it.hasMoreElements())
	{
		gkGameObjectGroup* grp = static_cast<gkGameObjectGroup*>(it.getNext().second);
		grp->destroyAllInstances();
	}
}


void gkGroupManager::notifyResourceDestroyedImpl(gkResource* res)
{
	gkGameObjectGroup* grp = static_cast<gkGameObjectGroup*>(res);
	GK_ASSERT(grp);
	if (grp) grp->destroyGameObjectInstances();
}


UT_IMPLEMENT_SINGLETON(gkGroupManager);
