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
#include "gkMeshManager.h"
#include "gkMesh.h"



gkMeshManager::gkMeshManager()
{
}


gkMeshManager::~gkMeshManager()
{
	destroyAll();
}


gkMesh *gkMeshManager::getMesh(const gkHashedString &name)
{
	size_t pos;
	if ((pos = m_objects.find(name)) == GK_NPOS)
		return 0;
	return m_objects.at(pos);
}



gkMesh *gkMeshManager::create(const gkHashedString &name)
{
	size_t pos;
	if ((pos = m_objects.find(name)) != GK_NPOS)
		return 0;

	gkMesh *ob = new gkMesh(name.str());
	m_objects.insert(name, ob);
	return ob;
}


void gkMeshManager::destroy(const gkHashedString &name)
{
	size_t pos;
	if ((pos = m_objects.find(name)) != GK_NPOS)
	{
		gkMesh *ob = m_objects.at(pos);
		m_objects.remove(name);
		delete ob;
	}
}


void gkMeshManager::destroy(gkMesh *ob)
{
	GK_ASSERT(ob);

	gkString name = ob->getName();
	size_t pos;
	if ((pos = m_objects.find(name)) != GK_NPOS)
	{
		gkMesh *ob = m_objects.at(pos);
		m_objects.remove(name);
		delete ob;
	}
}



void gkMeshManager::destroyAll(void)
{
	utHashTableIterator<ObjectMap> iter(m_objects);
	while (iter.hasMoreElements())
	{
		gkMesh *ob = iter.peekNextValue();
		delete ob;
		iter.next();
	}

	m_objects.clear();
}



bool gkMeshManager::hasMesh(const gkHashedString &name)
{
	return m_objects.find(name) != GK_NPOS;
}



GK_IMPLEMENT_SINGLETON(gkMeshManager);
