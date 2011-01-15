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
#include "gkResourceManager.h"
#include "gkResource.h"
#include "gkLogger.h"
#include "gkValue.h"




gkResourceManager::gkResourceManager(const gkString& type, const gkString& rtype)
	:  m_managerType(type), m_resourceType(rtype), m_resourceHandles(0)
{
}



gkResourceManager::~gkResourceManager()
{
	GK_ASSERT(m_resources.empty() && "Resources not cleared in derived class");
}



gkResource* gkResourceManager::getByName(const gkResourceName& name)
{
	gkResource* obCan = 0;
	Resources::Iterator iter = m_resources.iterator();
	while (iter.hasMoreElements())
	{
		gkResource* ob = iter.peekNextValue();
		if (ob->getResourceName().isNameEqual(name))
		{
			if (ob->getResourceName().isGroupEqual(name))
				return ob;
			else if (name.isGroupEmpty())
				obCan = ob;
		}

		iter.next();
	}
	return obCan;
}


gkResource* gkResourceManager::getByHandle(const gkResourceHandle& handle)
{
	UTsize pos;
	if ((pos = m_resources.find(handle)) != GK_NPOS)
		return m_resources.at(pos);
	return 0;
}



gkResource* gkResourceManager::create(const gkResourceName& name)
{

	if (exists(name))
	{
		gkLogMessage(m_managerType << ": Duplicate " << m_resourceType << " " << name.getName() << "(" << name.getGroup() << ") found ");
		return 0;
	}

	gkResource* ob = createImpl(name, m_resourceHandles++);

	if (!ob)
	{
		gkLogMessage(m_managerType << ": Failed to create "  << m_resourceType << " " << name.getName() << "(" << name.getGroup() << ")");
		return 0;
	}

	notifyResourceCreated(ob);

	m_resources.insert(ob->getResourceHandle(), ob);
	return ob;
}


void gkResourceManager::destroy(const gkResourceHandle& handle)
{
	UTsize pos = m_resources.find(handle);
	if (pos != UT_NPOS)
	{
		gkResource* res = m_resources.at(pos);
		res->notifyResourceDestroying();



		notifyResourceDestroyed(res);
		m_resources.remove(handle);
		delete res;
	}
}


void gkResourceManager::destroy(const gkResourceName& name)
{
	gkResource* res = getByName(name);
	if (res)
	{
		res->notifyResourceDestroying();

		notifyResourceDestroyed(res);
		m_resources.remove(res->getResourceHandle());
		delete res;
	}
}



void gkResourceManager::destroy(gkResource* ob)
{
	if (ob)
		destroy(ob->getResourceHandle());
}

void gkResourceManager::destroyGroup(const gkResourceNameString& group)
{
	notifyDestroyGroupInstancesImpl(group);
	notifyDestroyGroupImpl(group);

	Resources tmp;
	tmp.reserve(m_resources.size());

	Resources::Iterator iter = m_resources.iterator();
	while (iter.hasMoreElements())
	{
		gkResource* ob = iter.peekNextValue();
		if (ob->getResourceName().group == group)
		{
			ob->notifyResourceDestroying();
			notifyResourceDestroyed(ob);

			delete ob;
		}
		else
			tmp.insert(iter.peekNextKey(), ob);
		iter.next();
	}

	m_resources = tmp;
}

void gkResourceManager::destroyAll(void)
{
	notifyDestroyAllInstancesImpl();
	notifyDestroyAllImpl();


	Resources::Iterator iter = m_resources.iterator();
	while (iter.hasMoreElements())
	{
		gkResource* ob = iter.peekNextValue();
		ob->notifyResourceDestroying();
		notifyResourceDestroyed(ob);


		delete ob;
		iter.next();
	}

	m_resources.clear();
}


void gkResourceManager::notifyResourceCreated(gkResource* res)
{
	if (res && !m_listeners.empty())
	{
		UTsize i;
		for (i = 0; i < m_listeners.size(); ++i)
			m_listeners[i]->notifyResourceCreated(res);
	}

	notifyResourceCreatedImpl(res);
}


void gkResourceManager::notifyResourceDestroyed(gkResource* res)
{
	if (res && !m_listeners.empty())
	{
		UTsize i;
		for (i = 0; i < m_listeners.size(); ++i)
			m_listeners[i]->notifyResourceDestroyed(res);
	}

	notifyResourceDestroyedImpl(res);
}


void gkResourceManager::addResourceListener(gkResourceManager::ResourceListener* res)
{
	if (m_listeners.find(res) == UT_NPOS)
		m_listeners.push_back(res);
}


void gkResourceManager::removeResourceListener(gkResourceManager::ResourceListener* res)
{
	UTsize pos;
	if ((pos = m_listeners.find(res)) != UT_NPOS)
		m_listeners.erase(pos);
}


bool gkResourceManager::exists(const gkResourceHandle& handle)
{
	return m_resources.find(handle) != GK_NPOS;
}


bool gkResourceManager::exists(const gkResourceName& name)
{
	return getByName(name) != NULL;
}
