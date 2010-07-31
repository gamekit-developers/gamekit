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
#ifndef _gkGameObjectGroup_h_
#define _gkGameObjectGroup_h_

#include "gkCommon.h"
#include "gkGameObject.h"

class gkGameObjectGroup;
class gkGameObjectInstance;


typedef utHashTable<gkHashedString, gkGameObjectGroup *>    gkGroupTable;
typedef utHashTableIterator<gkGroupTable>                   gkGroupTableIterator;
typedef utArray<gkGameObjectInstance *>                     gkGroupInstances;
typedef utArrayIterator<gkGroupInstances>                   gkGroupInstanceIterator;

namespace Ogre
{
class StaticGeometry;
}

// Copy of original group
class gkGameObjectInstance : public gkObject
{
public:
	typedef utHashTable<gkHashedString, gkGameObject *>  InstanceObjects;
	typedef utHashTableIterator<InstanceObjects>        InstanceObjectIterator;

protected:

	gkGameObject            *m_owner;
	gkGameObjectGroup       *m_parent;
	const UTsize            m_handle;
	InstanceObjects         m_objects;

	void loadImpl(void);
	void unloadImpl(void);

public:
	gkGameObjectInstance(gkGameObject *owner, gkGameObjectGroup *group, UTsize uid);
	~gkGameObjectInstance();

	// object access

	gkGameObject    *getObject(const gkHashedString &name);
	void            removeObject(const gkHashedString &name);
	void            unloadObject(const gkHashedString &name);
	void            addObject(gkGameObject *v);

	GK_INLINE gkGameObject          *getOwner(void)     {return m_owner;}
	GK_INLINE gkGameObjectGroup     *getGroup(void)     {return m_parent;}
	GK_INLINE const UTsize           getHandle(void)    {return m_handle;}

	GK_INLINE InstanceObjectIterator  getObjectIterator(void) {return InstanceObjectIterator(m_objects);}


	GK_INLINE bool hasObject(const gkHashedString &name)
	{
		return m_objects.find(name) != UT_NPOS;
	}

};


// Collection of game objects that can be instantiated
class gkGameObjectGroup
{
public:
	typedef utHashTable<gkHashedString, gkGameObject *>  Objects;
	typedef utHashTableIterator<Objects>                ObjectIterator;

protected:

	const gkHashedString    m_name;
	gkGroupTable            m_table;
	gkGroupInstances        m_instances;
	// for generating unique handles on instance objects
	UTsize                  m_handle;

	// local lookup of game objects
	Objects                 m_internal;

	Ogre::StaticGeometry    *m_geom;

public:

	gkGameObjectGroup(const gkHashedString &name);
	~gkGameObjectGroup();


	void addObject(gkGameObject *v);


	// destroy all instances
	void clear(void);


	// notify that the owning object to
	// this group has been unloaded / destroyed
	void notifyObject(gkGameObject *ob);

	// duplication method
	gkGameObjectInstance *createInstance(gkGameObject *instPar);

	// build geometry.
	void build(Ogre::SceneManager *mgr);



	GK_INLINE const gkHashedString      &getName(void)                  {return m_name;}
	GK_INLINE gkGroupInstanceIterator   getInstanceIterator(void)       {return gkGroupInstanceIterator(m_instances);}
	GK_INLINE gkGroupInstances          &getInstances(void)             {return m_instances;}
	GK_INLINE ObjectIterator            getObjects(void)                {return ObjectIterator(m_internal);}

	GK_INLINE bool hasObject(gkGameObject *v)
	{
		if (v)
			return m_table.find(v->getName()) != UT_NPOS;
		return false;
	}

	GK_INLINE bool hasObject(const gkString &name)
	{
		return m_table.find(name) != UT_NPOS;
	}


};

#endif//_gkGameObjectGroup_h_
