/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): Nestor Silveira.
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
#ifndef _gkSceneObject_h_
#define _gkSceneObject_h_

#include "gkObject.h"
#include "gkMathUtils.h"
#include "gkSerialize.h"
#include "gkGameObjectGroup.h"
#include "OgreAxisAlignedBox.h"
#include "AI/gkNavMeshData.h"
#include "Thread/gkAsyncResult.h"
#include "gkRecast.h"

class gkDynamicsWorld;
class gkDebugger;
class gkScene;
class gkActiveObject;

// shorthand types
typedef std::set<gkGameObject *>						gkGameObjectSet;
typedef utHashTable<gkHashedString, gkGameObject *>  gkGameObjectHashMap;
typedef utHashTableIterator<gkGameObjectHashMap>    gkGameObjectHashMapIterator;
typedef utArray<gkGameObject *>                      gkGameObjectArray;
typedef utArrayIterator<gkGameObjectArray>          gkGameObjectArrayIterator;

// Game scene
class gkScene : public gkObject
{
public:

	gkScene(const gkString &name);
	virtual ~gkScene();

	// Updates the game state
	void update(gkScalar tickRate);
	void beginFrame(void);


	// Interpolates transformed objects,
	// for smoothing motion between game ticks
	void synchronizeMotion(gkScalar blend);

	// property access
	GK_INLINE gkSceneProperties &getProperties(void)    { return m_baseProps; }


	// The main viewport. Only support for one at the moment.
	GK_INLINE Ogre::Viewport *getViewport(void) { return m_viewport; }
	void applyViewport(Ogre::Viewport *vp);

	// Returns aabb for all static objects in the scene
	const Ogre::AxisAlignedBox &getLimits() const { return m_Limits; }

	// The Ogre scene manager is only available
	// when this scene is loaded. Each scene has
	// it's own manager
	GK_INLINE Ogre::SceneManager *getManager(void)
	{ GK_ASSERT(m_manager); return m_manager; }


	// The active camera
	GK_INLINE gkCamera *getMainCamera(void) { return m_startCam; }
	void setMainCamera(gkCamera *cam);

	GK_INLINE gkGameObjectSet &getLoadedObjects(void)      { return m_loadedObjects; }

	// physics
	GK_INLINE gkDynamicsWorld *getDynamicsWorld(void)       { return m_physicsWorld; }


	// notifications
	GK_INLINE void notifyObjectMoved(gkGameObject *gobject) { m_transformObjects.push_back(gobject); }

	void notifyObjectLoaded(gkGameObject *gobject);
	void notifyObjectUnloaded(gkGameObject *gobject);
	void notifyObjectUpdate(gkGameObject *gobject);

	bool            hasObject(const gkHashedString &ob);
	bool            hasObject(gkGameObject *ob);
	gkGameObject   *getObject(const gkHashedString &name);

	bool            hasMesh(const gkHashedString &ob);
	gkMesh         *getMesh(const gkHashedString &name);


	// Translates to a blank scene node or empty object.
	gkGameObject   *createObject(const gkHashedString &name);
	gkLight        *createLight(const gkHashedString &name);
	gkCamera       *createCamera(const gkHashedString &name);
	gkEntity       *createEntity(const gkHashedString &name);
	gkSkeleton     *createSkeleton(const gkHashedString &name);
	gkMesh         *createMesh(const gkHashedString &name);


	gkGameObject   *cloneObject(gkGameObject *obj, int life);
	void            endObject(gkGameObject *obj);


	// group access

	gkGameObjectGroup   *createGroup(const gkHashedString &name);
	gkGameObjectGroup   *getGroup(const gkHashedString &name);
	void                destroyGroup(const gkHashedString &name);
	void                destroyGroup(gkGameObjectGroup *group);
	void                destroyGroups(void);

	// instance creation
	gkGameObjectInstance *createInstance(gkGameObject *owner, gkGameObjectGroup *group);

	GK_INLINE bool      hasGroup(const gkHashedString &name) {return m_groups.find(name) != UT_NPOS;}

	// global debugging
	gkDebugger          *getDebugger(void);

	GK_INLINE bool      hasLights(void) {return m_hasLights;}

	GK_INLINE void setNavMeshData(PNAVMESHDATA navMeshData) { m_navMeshData = navMeshData; }

	typedef gkAsyncResult<PDT_NAV_MESH > ASYNC_DT_RESULT;

	bool asyncTryToCreateNavigationMesh(gkActiveObject &activeObj, const gkRecast::Config &config, ASYNC_DT_RESULT result);

protected:

	// Ogre scene manager
	Ogre::SceneManager     *m_manager;

	// Main camera
	gkCamera               *m_startCam;

	// Main viewport
	Ogre::Viewport         *m_viewport;

	// Properties for this scene
	gkSceneProperties       m_baseProps;

	gkDebugger              *m_debugger;

	gkGameObjectHashMap     m_objects;
	gkGameObjectArray       m_transformObjects;
	gkGameObjectSet			m_loadedObjects;
	gkGameObjectArray       m_constraintObjects;
	gkGameObjectArray       m_clones;
	gkGameObjectArray       m_tickClones;
	gkGameObjectArray       m_endObjects;

	gkDynamicsWorld        *m_physicsWorld;
	gkGroupTable            m_groups;
	gkGroupInstances        m_instances;
	bool                    m_hasLights;
	bool                    m_markDBVT;
	int                     m_cloneCount;

private:

	void postLoadImpl(void);
	void loadImpl(void);
	void unloadImpl(void);
	void setShadows(void);
	void applyConstraints(void);
	void tickClones(void);
	void destroyClones(void);
	void endObjects(void);


	Ogre::AxisAlignedBox m_Limits;

	PNAVMESHDATA m_navMeshData;
};

#endif//_gkSceneObject_h_
