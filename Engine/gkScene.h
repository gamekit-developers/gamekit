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
#include "gkResource.h"
#include "gkGameObjectGroup.h"
#include "AI/gkNavMeshData.h"
#include "Thread/gkAsyncResult.h"
#include "gkRecast.h"



class gkScene : public gkInstancedObject
{
public:

	gkScene(gkInstancedManager* creator, const gkResourceName &name, const gkResourceHandle& handle);
	virtual ~gkScene();


	void update(gkScalar tickRate);
	void beginFrame(void);



	GK_INLINE gkSceneProperties&        getProperties(void)    { return m_baseProps;  }

	GK_INLINE gkSoundSceneProperties&   getSoundScene(void)    { return m_soundScene; }


	///Returns aabb for all static objects in the scene
	const gkBoundingBox& getLimits() const { return m_limits; }


	///The Ogre scene manager is only available when this scene is instanced.
	GK_INLINE Ogre::SceneManager* getManager(void) { GK_ASSERT(m_manager); return m_manager; }


	GK_INLINE gkGameObjectSet&      getInstancedObjects(void)    { return m_instanceObjects; }
	GK_INLINE gkGameObjectHashMap&  getObjects(void)             { return m_objects; }


	gkDynamicsWorld* getDynamicsWorld(void);
	
	// Callback events

	void notifyInstanceCreated(gkGameObject* gobject);
	void notifyInstanceDestroyed(gkGameObject* gobject);
	void notifyObjectUpdate(gkGameObject* gobject);


	void            addObject(gkGameObject* obj);
	void            removeObject(gkGameObject* obj);
	void            destroyObject(gkGameObject* obj);


	bool            hasObject(const gkHashedString& ob);
	bool            hasObject(gkGameObject* ob);
	gkGameObject*   getObject(const gkHashedString& name);

	bool            hasMesh(const gkHashedString& ob);
	gkMesh*         getMesh(const gkHashedString& name);


	gkGameObject*   createObject(const gkHashedString& name);
	gkLight*        createLight(const gkHashedString& name);
	gkCamera*       createCamera(const gkHashedString& name);
	gkEntity*       createEntity(const gkHashedString& name);
	gkSkeleton*     createSkeleton(const gkHashedString& name);
	gkMesh*         createMesh(const gkHashedString& name);


	gkGameObject*   cloneObject(gkGameObject* obj, int life, bool instantiate = false);
	void            endObject(gkGameObject* obj);


	void             getGroups(gkGroupArray &groups);


	gkDebugger* getDebugger(void);

	GK_INLINE void setNavMeshData(PNAVMESHDATA navMeshData) { m_navMeshData = navMeshData; }

	typedef gkAsyncResult<PDT_NAV_MESH > ASYNC_DT_RESULT;

	bool asyncTryToCreateNavigationMesh(gkActiveObject& activeObj, const gkRecast::Config& config, ASYNC_DT_RESULT result);



	void applyConstraints(void);
	gkConstraintManager* getConstraintManager(void);
	void addConstraint(gkGameObject* gobj, gkConstraint* co);


	gkPhysicsControllerSet& getStaticControllers(void) {return m_staticControllers;}
	void calculateLimits(void);


	GK_INLINE gkCamera*       getMainCamera(void)   { return m_startCam; }
	GK_INLINE bool           hasDefaultCamera(void) { return m_startCam != 0; }
	GK_INLINE bool           hasCameras(void)       { return !m_cameras.empty(); }
	GK_INLINE gkCameraSet&    getCameras(void)      { return m_cameras; }
	void setMainCamera(gkCamera* cam);



	GK_INLINE gkLightSet&    getLights(void) {return m_lights;}
	GK_INLINE bool          hasLights(void)  {return isInstanced() ? !m_lights.empty() : m_hasLights;}


	GK_INLINE void      setLayer(UTuint32 v)     {m_layers = v; }
	GK_INLINE UTuint32  getLayer(void)           {return m_layers;}

	gkGameObject* findInstancedObject(const gkString& name);


	// Local property access.

	void setSceneManagerType(int type);
	int getSceneManagerType(void);


	void setWorldColor(const gkColor& col);
	const gkColor& getWorldColor(void);

	void setAmbientColor(const gkColor& col);
	const gkColor& getAmbientColor(void);


	void setGravity(const gkVector3& grav);
	const gkVector3& getGravity(void);


	gkRigidBody* createRigidBody(gkGameObject* obj, gkPhysicsProperties& prop);



	void _applyBuiltinParents(void);
	void _applyBuiltinPhysics(void);

	void _createPhysicsObject(gkGameObject* obj);
	void _destroyPhysicsObject(gkGameObject* obj);

	void _unloadAndDestroy(gkGameObject* obj);

	bool _replaceObjectInScene(gkGameObject* obj, gkScene* osc, gkScene* nsc);
	void _eraseObject(gkGameObject* obj);

private:

	void postCreateInstanceImpl(void);
	void createInstanceImpl(void);
	void destroyInstanceImpl(void);
	void setShadows(void);
	void tickClones(void);
	void destroyClones(void);
	void endObjects(void);

	Ogre::SceneManager*     m_manager;
	gkCamera*               m_startCam;
	Ogre::Viewport*         m_viewport;


	gkSceneProperties       m_baseProps;
	gkSoundSceneProperties  m_soundScene;

	gkDebugger*             m_debugger;

	gkGameObjectHashMap     m_objects;
	gkGameObjectSet         m_instanceObjects;


	gkGameObjectArray       m_clones;
	gkGameObjectArray       m_tickClones;
	gkGameObjectSet         m_endObjects;
	gkPhysicsControllerSet  m_staticControllers;
	gkCameraSet             m_cameras;
	gkLightSet              m_lights;

	gkConstraintManager*    m_constraintManager;
	gkDynamicsWorld*        m_physicsWorld;
	gkMeshManager*          m_meshManager;

	bool                    m_hasLights;
	bool                    m_markDBVT;
	int                     m_cloneCount;
	UTuint32                m_layers;
	gkBoundingBox           m_limits;
	PNAVMESHDATA            m_navMeshData;
};

#endif//_gkSceneObject_h_
