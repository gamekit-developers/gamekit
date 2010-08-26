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
#include "gkTransformState.h"
#include "gkMathUtils.h"


// Collection of game objects that can be batch created.
class gkGameObjectGroup
{
public:
	typedef utHashTable<gkHashedString, gkGameObject *>  Objects;
	typedef utHashSet<gkGameObjectInstance *>            Instances;


protected:

	Ogre::StaticGeometry    *m_geometry;

	const gkHashedString    m_name;

	// List of object instances
	Instances               m_instances;



	// for generating unique handles on instance objects
	UTsize                  m_handle;


	// local lookup of game objects
	Objects                 m_objects;


	// Parent manager.
	gkGroupManager          *m_manager;


public:

	gkGameObjectGroup(gkGroupManager *manager, const gkHashedString &name);
	~gkGameObjectGroup();


	// Object insertion.
	void addObject(gkGameObject *v);

	// Object removal
	void removeObject(gkGameObject *v);

	// Object queries.

	bool           hasObject(const gkHashedString &name);
	gkGameObject  *getObject(const gkHashedString &name);




	// destroy all instances
	gkGameObjectInstance *createInstance(gkScene *scene);
	void                  destroyInstance(gkGameObjectInstance *inst);



	// id lookup
	gkGameObjectInstance* findInstanceBy(UTsize id);

	// Create static batch geometry.
	void createStaticBatches(gkScene *scene);
	void destroyStaticBatches(gkScene *scene);


	// Delete all instances.
	void clearInstances(void);

	void initializeInstances(void);
	void finalizeInstances(void);


	// Clones all objects in this group.
	void cloneObjects(gkScene *scene, 
		const gkTransformState &from, int time, 
		const gkVector3& linearVelocity=gkVector3::ZERO, bool tsLinLocal = true,
		const gkVector3& angularVelocity=gkVector3::ZERO, bool tsAngLocal = true);


	GK_INLINE Instances                 &getInstances(void)      {return m_instances;}
	GK_INLINE const gkHashedString      &getName(void)           {return m_name;}
	GK_INLINE Objects                   &getObjects(void)        {return m_objects;}
	GK_INLINE bool                       isEmpty(void)           {return m_objects.empty();}
	GK_INLINE gkGroupManager            *getOwner(void)          {return m_manager;}

};

#endif//_gkGameObjectGroup_h_
