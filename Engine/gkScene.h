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
#include "AI/gkNavMeshData.h"
#include "Thread/gkAsyncResult.h"
#include "gkRecast.h"

// Game scene
class gkScene : public gkObject
{
public:

	gkScene(const gkString &name);
	virtual ~gkScene();

	// Updates the game state
	void update(gkScalar tickRate);
	void beginFrame(void);

	// property access
	GK_INLINE gkSceneProperties        &getProperties(void)    { return m_baseProps;  }
	GK_INLINE gkSoundSceneProperties   &getSoundScene(void)    { return m_soundScene; }


	// The main viewport. Only support for one at the moment.
	GK_INLINE Ogre::Viewport *getViewport(void) { return m_viewport; }
	void applyViewport(Ogre::Viewport *vp);

	// Returns aabb for all static objects in the scene
	const gkBoundingBox &getLimits() const { return m_limits; }

	// The Ogre scene manager is only available
	// when this scene is loaded.
	GK_INLINE Ogre::SceneManager *getManager(void) { GK_ASSERT(m_manager); return m_manager; }


	GK_INLINE gkGameObjectSet &getInitializedObjects(void) { return m_initObjects; }

	// physics
	GK_INLINE gkDynamicsWorld *getDynamicsWorld(void)       { return m_physicsWorld; }

	// notifications
	void notifyObjectInitialized(gkGameObject *gobject);
	void notifyObjectFinalized(gkGameObject *gobject);
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
	GK_INLINE gkGroupManager *getGroupManager(void) {return m_groupManager;}



	// global debugging
	gkDebugger          *getDebugger(void);

	GK_INLINE void setNavMeshData(PNAVMESHDATA navMeshData) { m_navMeshData = navMeshData; }

	typedef gkAsyncResult<PDT_NAV_MESH > ASYNC_DT_RESULT;

	bool asyncTryToCreateNavigationMesh(gkActiveObject &activeObj, const gkRecast::Config &config, ASYNC_DT_RESULT result);


	// Update scene constraints.
	void applyConstraints(void);

	// Access to the constraint manager.
	gkConstraintManager *getConstraintManager(void);

	// Access to static physics information.
	gkPhysicsControllerSet &getStaticControllers(void) {return m_staticControllers;}

	// Force update of scene limits.
	void calculateLimits(void);


	// Camera access.

	GK_INLINE gkCamera       *getMainCamera(void)   { return m_startCam; }
	GK_INLINE bool           hasDefaultCamera(void) { return m_startCam != 0; }
	GK_INLINE bool           hasCameras(void)       { return !m_cameras.empty(); }
	GK_INLINE gkCameraSet    &getCameras(void)      { return m_cameras; }
	void setMainCamera(gkCamera *cam);


	// Light access.

	GK_INLINE gkLightSet    &getLights(void) {return m_lights;}
	GK_INLINE bool          hasLights(void)  {return isInitialized() ? !m_lights.empty() : m_hasLights;}


	GK_INLINE void      setLayer(UTuint32 v)     {m_layers = v; }
	GK_INLINE UTuint32  getLayer(void)           {return m_layers;}



	// Linear search.
	gkGameObject *findInitializedObject(const gkString &name);

private:

	void applyBuiltinParents(void);
	void applyBuiltinPhysics(void);
	void applyNavigationMeshes(void);


	void createPhysicsObject(gkGameObject *obj);
	void destroyPhysicsObject(gkGameObject *obj);

	void finalizeAndDestroy(gkGameObject *obj);


	void postInitializeImpl(void);
	void initializeImpl(void);
	void finalizeImpl(void);
	void setShadows(void);
	void tickClones(void);
	void destroyClones(void);
	void endObjects(void);

	// Ogre scene manager
	Ogre::SceneManager     *m_manager;

	// Main camera
	gkCamera               *m_startCam;

	// Main viewport
	Ogre::Viewport         *m_viewport;

	// Properties for this scene
	gkSceneProperties       m_baseProps;
	gkSoundSceneProperties  m_soundScene;

	gkDebugger              *m_debugger;

	gkGameObjectHashMap     m_objects;
	gkGameObjectSet         m_initObjects;


	gkGameObjectArray       m_clones;
	gkGameObjectArray       m_tickClones;
	gkGameObjectSet         m_endObjects;
	gkPhysicsControllerSet  m_staticControllers;
	gkCameraSet             m_cameras;
	gkLightSet              m_lights;

	gkConstraintManager    *m_constraintManager;
	gkDynamicsWorld        *m_physicsWorld;
	gkMeshManager          *m_meshManager;
	gkGroupManager         *m_groupManager;

	bool                    m_hasLights;
	bool                    m_markDBVT;
	int                     m_cloneCount;
	UTuint32                m_layers;
	gkBoundingBox           m_limits;
	PNAVMESHDATA            m_navMeshData;
};

#endif//_gkSceneObject_h_
