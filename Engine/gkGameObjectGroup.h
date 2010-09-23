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
#include "gkResource.h"
#include "gkInstancedManager.h"


///Groups are a list of game objects that are grouped together by a common name.
///They manage gkGameObjectInstance objects which are created by cloning the group as a whole.
///This allows multiple objects from the same group to be placed anywhere in the scene together.
class gkGameObjectGroup : public gkResource
{
public:
	typedef utHashTable<gkHashedString, gkGameObject*>  Objects;



	class InstanceManager : public gkInstancedManager
	{
	public:


		InstanceManager(gkGameObjectGroup* group);
		virtual ~InstanceManager() {}


		gkResource* createImpl(const gkResourceName& name, const gkResourceHandle& handle);

	protected:
		gkGameObjectGroup* m_group;
	};



protected:

	Ogre::StaticGeometry*   m_geometry;
	UTsize                  m_handle;
	Objects                 m_objects;
	InstanceManager*        m_instanceManager;


public:

	gkGameObjectGroup(gkResourceManager* creator, const gkResourceName& name, const gkResourceHandle& handle);
	virtual ~gkGameObjectGroup();


	void addObject(gkGameObject* v);
	void destroyObject(gkGameObject* v);

	///Destroys all gkGameObjectInstance objects managed by this group.
	void destroyAllInstances(void);



	bool           hasObject(const gkHashedString& name);
	gkGameObject*  getObject(const gkHashedString& name);


	gkGameObjectInstance* createGroupInstance(gkScene* scene, const gkResourceName& name);
	void                  destroyGroupInstance(gkGameObjectInstance* inst);


	///This will group all meshes based on their material to be
	///rendered by Ogre with one call to the underlying graphics API
	///In OgreKit, this only works for truly static objects.
	///Things like grass, tree leaves, or basically
	///any gkEntity that does not respond to collisions (GK_NO_COLLISION).
	///\todo This needs a better static object check.
	void createStaticBatches(gkScene* scene);
	void destroyStaticBatches(gkScene* scene);


	///Places all gkGameObjectInstance objects in the Ogre scene
	void createGameObjectInstances(gkScene* scene);

	///Removes all gkGameObjectInstance objects from the Ogre scene
	void destroyGameObjectInstances(void);



	void cloneObjects(gkScene* scene,
	                  const gkTransformState& from, int time,
	                  const gkVector3& linearVelocity = gkVector3::ZERO,
	                  bool tsLinLocal = true,
	                  const gkVector3& angularVelocity = gkVector3::ZERO,
	                  bool tsAngLocal = true);


	GK_INLINE InstanceManager&           getInstances(void)      {GK_ASSERT(m_instanceManager); return *m_instanceManager;}
	GK_INLINE Objects&                   getObjects(void)        {return m_objects;}
	GK_INLINE bool                       isEmpty(void)           {return m_objects.empty();}



};

#endif//_gkGameObjectGroup_h_
