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
#include "gkLogger.h"


// ----------------------------------------------------------------------------
gkGroupManager::gkGroupManager()
	:	m_handles(0)
{
}


// ----------------------------------------------------------------------------
gkGroupManager::~gkGroupManager()
{
	destroyAll();
}


// ----------------------------------------------------------------------------
gkGameObjectGroup *gkGroupManager::createGroup(const gkHashedString& name)
{
	if (m_groups.find(name) != UT_NPOS)
	{
		gkPrintf("GroupManager: Duplicate group '%s' found\n", name.str().c_str());
		return 0;
	}


	gkGameObjectGroup *group = new gkGameObjectGroup(this, name);

	m_groups.insert(name, group);
	return group;
}


// ----------------------------------------------------------------------------
gkGameObjectGroup *gkGroupManager::getGroup(const gkHashedString& name)
{
	UTsize pos;
	if ( (pos = m_groups.find(name)) == UT_NPOS)
	{
		gkPrintf("GroupManager: Missing group '%s'\n", name.str().c_str());
		return 0;
	}
	return m_groups.at(pos);
}

// ----------------------------------------------------------------------------
void gkGroupManager::destroyGroup(const gkHashedString& name)
{
	destroyGroup(getGroup(name));
}

// ----------------------------------------------------------------------------
void gkGroupManager::destroyGroup(gkGameObjectGroup *group)
{
	if (group && hasGroup(group->getName()))
	{
		m_groups.remove(group->getName());
		delete group;

		if (m_groups.empty())
			m_groups.clear();
	}
}

// ----------------------------------------------------------------------------
void gkGroupManager::destroyAll(void)
{
	Groups::Iterator it = m_groups.iterator();
	while (it.hasMoreElements())
		delete it.getNext().second;

	m_groups.clear();
	m_handles = 0;
}

// ----------------------------------------------------------------------------
bool gkGroupManager::hasGroup(const gkHashedString &name)
{
	return m_groups.find(name) != UT_NPOS;
}


// ----------------------------------------------------------------------------
void gkGroupManager::loadAll(void)
{
	// Pass command
	Groups::Iterator it = m_groups.iterator();
	while (it.hasMoreElements())
		it.getNext().second->loadInstances();
}


// ----------------------------------------------------------------------------
void gkGroupManager::unloadAll(void)
{
	Groups::Iterator it = m_groups.iterator();
	while (it.hasMoreElements())
		it.getNext().second->unloadInstances();
}

// ----------------------------------------------------------------------------
gkGameObjectInstance* gkGroupManager::findInstanceBy(UTsize id)
{
	gkGameObjectInstance *ret = 0;

	Groups::Iterator it = m_groups.iterator();
	while (it.hasMoreElements())
	{
		ret = it.getNext().second->findInstanceBy(id);
		if (ret) break;
	}

	return ret;
}

