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




gkResourceManager::gkResourceManager() : m_resourceHandles(0)
{
}



gkResourceManager::~gkResourceManager()
{
}




gkResource* gkResourceManager::getByName(const gkResourceName& name)
{
	Resources::Iterator iter = m_resources.iterator();
	while (iter.hasMoreElements())
	{
		gkResource* ob = iter.peekNextValue();
		if (ob->getResourceName() == name)
			return ob;

		iter.next();
	}
	return 0;
}


gkResource* gkResourceManager::getByHandle(const gkResourceHandle& handle)
{
	UTsize pos;
	if ((pos = m_resources.find(handle)) != GK_NPOS)
		return m_resources.at(pos);
	return 0;
}



gkResource* gkResourceManager::create(const gkResourceName& name, const gkParameterMap *params)
{

	if (exists(name))
	{
		gkLogMessage("ResourceManager: Duplicate resource " << name.str() << " found ");
		return 0;
	}

	gkResource* ob = createImpl(name, m_resourceHandles++, params);

	if (!ob)
	{
		gkLogMessage("ResourceManager: Failed to create resource " << name.str());
		return 0;
	}

	notifyResourceCreated(ob);

	m_resources.insert(ob->getResourceHandle(), ob);
	return ob;
}



gkResource* gkResourceManager::clone(gkResource* res, bool track, const gkParameterMap *params)
{
	if (!res)
		return 0;


	gkString cloneName = res->getResourceName().str() + gkToString((int)m_resourceHandles + 1);
	gkResource* ob = cloneImpl(res, cloneName, m_resourceHandles + 1, params);

	if (ob != 0)
	{
		m_resourceHandles ++;
		notifyResourceCreated(ob);
		if (track)
			m_resources.insert(ob->getResourceHandle(), ob);
		return ob;
	}

	return 0;
}



void gkResourceManager::destroy(const gkResourceHandle& handle)
{
	UTsize pos = m_resources.find(handle);
	if (pos != UT_NPOS)
	{
		gkResource* res = m_resources.at(pos);

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



void gkResourceManager::destroyAll(void)
{
	notifyDestroyAllInstancesImpl();
	notifyDestroyAllImpl();


	Resources::Iterator iter = m_resources.iterator();
	while (iter.hasMoreElements())
	{
		gkResource* ob = iter.peekNextValue();
		notifyResourceDestroyed(ob);


		delete ob;
		iter.next();
	}

	m_resources.clear();
}


void gkResourceManager::notifyResourceCreated(gkResource* res)
{
	if (res && !m_listsners.empty())
	{
		UTsize i;
		for (i = 0; i<m_listsners.size(); ++i)
			m_listsners[i]->notifyResourceCreated(res);
	}

	notifyResourceCreatedImpl(res);
}


void gkResourceManager::notifyResourceDestroyed(gkResource* res)
{
	if (res && !m_listsners.empty())
	{
		UTsize i;
		for (i = 0; i<m_listsners.size(); ++i)
			m_listsners[i]->notifyResourceDestroyed(res);
	}

	notifyResourceDestroyedImpl(res);
}


void gkResourceManager::addResourceListener(gkResourceManager::ResourceListener* res)
{
	if (m_listsners.find(res) == UT_NPOS)
		m_listsners.push_back(res);
}


void gkResourceManager::removeResourceListener(gkResourceManager::ResourceListener* res)
{
	UTsize pos;
	if ((pos = m_listsners.find(res)) != UT_NPOS)
		m_listsners.erase(pos);
}


bool gkResourceManager::exists(const gkResourceHandle& handle)
{
	return m_resources.find(handle) != GK_NPOS;
}


bool gkResourceManager::exists(const gkResourceName& name)
{
	Resources::Iterator iter = m_resources.iterator();
	while (iter.hasMoreElements())
	{
		gkResource* ob = iter.peekNextValue();
		if (ob->getResourceName() == name)
			return true;

		iter.next();
	}

	return false;
}
