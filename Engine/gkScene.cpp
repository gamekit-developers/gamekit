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
#include "OgreRoot.h"
#include "OgreSceneManager.h"
#include "OgreRenderWindow.h"
#include "OgreViewport.h"
#include "OgreStringConverter.h"

#include "gkWindowSystem.h"
#include "gkCamera.h"
#include "gkEntity.h"
#include "gkLight.h"
#include "gkSkeleton.h"
#include "gkGameObjectGroup.h"
#include "gkEngine.h"
#include "gkScene.h"
#include "gkLogicManager.h"
#include "gkNodeManager.h"
#include "Script/Lua/gkLuaManager.h"
#include "gkLogger.h"
#include "gkDynamicsWorld.h"
#include "gkRigidBody.h"
#include "gkCharacter.h"
#include "gkUserDefs.h"
#include "gkDebugger.h"
#include "gkMeshManager.h"
#include "Thread/gkActiveObject.h"
#include "gkStats.h"
#include "gkUtils.h"

#include "gkConstraintManager.h"
#include "gkGroupManager.h"

#ifdef OGREKIT_OPENAL_SOUND
#include "gkSoundManager.h"
#endif


using namespace Ogre;


// ----------------------------------------------------------------------------
gkScene::gkScene(const gkString &name)
	:    gkObject(name),
	     m_manager(0),
	     m_startCam(0),
	     m_viewport(0),
	     m_baseProps(),
	     m_constraintManager(new gkConstraintManager()),
	     m_physicsWorld(0),
	     m_meshManager(new gkMeshManager()),
	     m_groupManager(new gkGroupManager()),
	     m_debugger(0),
	     m_hasLights(false),
	     m_markDBVT(false),
	     m_cloneCount(0),
	     m_layers(0xFFFFFFFF)
{
}


// ----------------------------------------------------------------------------
gkScene::~gkScene()
{


	gkGameObjectHashMap::Iterator it = m_objects.iterator();
	while (it.hasMoreElements())
		delete it.getNext().second;


	if (m_groupManager)
	{
		delete m_groupManager;
		m_groupManager = 0;
	}


	if (m_constraintManager)
	{
		// Free constraints.

		delete m_constraintManager;
		m_constraintManager = 0;
	}

	if (m_meshManager)
	{
		// Free all mesh objects.

		m_meshManager->destroyAll();
		delete m_meshManager;
		m_meshManager = 0;
	}
}



// ----------------------------------------------------------------------------
bool gkScene::hasObject(const gkHashedString &ob)
{
	return m_objects.find(ob) != GK_NPOS;
}


// ----------------------------------------------------------------------------
bool gkScene::hasObject(gkGameObject *ob)
{
	GK_ASSERT(ob);
	return m_objects.find(ob->getName()) != GK_NPOS;
}



// ----------------------------------------------------------------------------
gkGameObject *gkScene::getObject(const gkHashedString &name)
{
	UTsize pos = m_objects.find(name);
	if (pos != GK_NPOS)
		return m_objects.at(pos);
	return 0;
}

// ----------------------------------------------------------------------------
bool gkScene::hasMesh(const gkHashedString &ob)
{
	GK_ASSERT(m_meshManager);
	return m_meshManager->hasMesh(ob);
}

// ----------------------------------------------------------------------------
gkMesh *gkScene::getMesh(const gkHashedString &name)
{
	GK_ASSERT(m_meshManager);
	return m_meshManager->getMesh(name);
}


// ----------------------------------------------------------------------------
gkMesh *gkScene::createMesh(const gkHashedString &name)
{
	GK_ASSERT(m_meshManager);
	return m_meshManager->create(name);
}


// ----------------------------------------------------------------------------
gkGameObject *gkScene::createObject(const gkHashedString &name)
{
	if (m_objects.find(name) != GK_NPOS)
	{
		gkPrintf("Scene: Duplicate object '%s' found\n", name.str().c_str());
		return 0;
	}

	gkGameObject *ob = new gkGameObject(this, name.str(), GK_OBJECT);

	m_objects.insert(name, ob);
	return ob;
}



// ----------------------------------------------------------------------------
gkLight *gkScene::createLight(const gkHashedString &name)
{
	if (m_objects.find(name) != GK_NPOS)
	{
		gkPrintf("Scene: Duplicate object '%s' found\n", name.str().c_str());
		return 0;
	}

	gkLight *ob = new gkLight(this, name.str());
	m_objects.insert(name, ob);

	m_hasLights = true;
	return ob;
}



// ----------------------------------------------------------------------------
gkCamera *gkScene::createCamera(const gkHashedString &name)
{
	if (m_objects.find(name) != GK_NPOS)
	{
		gkPrintf("Scene: Duplicate object '%s' found\n", name.str().c_str());
		return 0;
	}

	gkCamera *ob = new gkCamera(this, name.str());

	m_objects.insert(name, ob);
	return ob;
}



// ----------------------------------------------------------------------------
gkEntity *gkScene::createEntity(const gkHashedString &name)
{
	if (m_objects.find(name) != GK_NPOS)
	{
		gkPrintf("Scene: Duplicate object '%s' found\n", name.str().c_str());
		return 0;
	}

	gkEntity *ob = new gkEntity(this, name.str());

	m_objects.insert(name, ob);
	return ob;
}



// ----------------------------------------------------------------------------
gkSkeleton *gkScene::createSkeleton(const gkHashedString &name)
{
	if (m_objects.find(name) != GK_NPOS)
	{
		gkPrintf("Scene: Duplicate object '%s' found\n", name.str().c_str());
		return 0;
	}

	gkSkeleton *ob = new gkSkeleton(this, name.str());

	m_objects.insert(name, ob);
	return ob;
}


// ----------------------------------------------------------------------------
gkDebugger *gkScene::getDebugger(void)
{
	if (isLoaded() && !m_debugger)
		m_debugger = new gkDebugger(this);
	return m_debugger;
}

// ----------------------------------------------------------------------------
gkConstraintManager *gkScene::getConstraintManager(void)
{
	return m_constraintManager;
}


// ----------------------------------------------------------------------------
void gkScene::applyViewport(Viewport *vp)
{
	const gkVector2 &size = gkWindowSystem::getSingleton().getMouse()->winsize;
	vp->setDimensions(0, 0, 1, 1);

	Camera *c = vp->getCamera();
	if (c != 0)
		c->setAspectRatio(size.x / size.y);
}



// ----------------------------------------------------------------------------
gkGameObject *gkScene::findLoadedObject(const gkString &name)
{
	gkGameObjectSet::Iterator it = m_loadedObjects.iterator();
	while (it.hasMoreElements())
	{
		gkGameObject *obj = it.getNext();
		if (obj->getName() == name) 
			return obj;
	}
	return 0;
}


// ----------------------------------------------------------------------------
void gkScene::setMainCamera(gkCamera *cam)
{
	if (!cam)
		return;


	m_startCam = cam;
	Camera *main = m_startCam->getCamera();
	GK_ASSERT(main);

	if (!m_viewport)
	{
		// create a new viewport
		RenderWindow *window = gkWindowSystem::getSingleton().getMainWindow();
		if (window)
			m_viewport = window->addViewport(main);
	}
	else
		m_viewport->setCamera(main);

	GK_ASSERT(m_viewport);
	applyViewport(m_viewport);
}



// ----------------------------------------------------------------------------
void gkScene::createPhysicsObject(gkGameObject *obj)
{
	GK_ASSERT(obj && obj->isLoaded() && m_physicsWorld);

	gkGameObjectProperties  &props  = obj->getProperties();

	if (!props.isPhysicsObject() || obj->hasParent())
		return;

	// TODO make compound shape from children.

	gkEntity *ent = obj->getEntity();
	if (ent && props.m_physics.isMeshShape())
	{
		// can happen with the newer mesh objects
		// which test for the collision face flag on each triangle

		gkMesh *me = ent->getEntityProperties().m_mesh;
		btTriangleMesh *trimesh = me->getTriMesh();

		if (trimesh->getNumTriangles() == 0)
			props.m_physics.m_type = GK_NO_COLLISION;
	}

	// Replace controller.
	if (obj->getPhysicsController())
		m_physicsWorld->destroyObject(obj->getPhysicsController());


	if (props.isGhost())
	{
		gkCharacter *con = m_physicsWorld->createCharacter(obj);
		obj->attachCharacter(con);
	}
	else
	{
		gkRigidBody *con = m_physicsWorld->createRigidBody(obj);
		obj->attachRigidBody(con);

		if (con->isStaticObject())
		{
			m_staticControllers.insert(con);
			m_limits.merge(con->getAabb());
		}
	}
}


// ----------------------------------------------------------------------------
void gkScene::destroyPhysicsObject(gkGameObject *obj)
{
	GK_ASSERT(obj && m_physicsWorld);

	bool isSceneUnloading = isUnloading();

	if (obj->getPhysicsController())
	{
		gkPhysicsController *phyCon = obj->getPhysicsController();

		obj->attachRigidBody(0);
		obj->attachCharacter(0);

		if (!isSceneUnloading)
		{
			bool isStatic = phyCon->isStaticObject();
			if (isStatic)
				m_staticControllers.erase(phyCon);

			m_physicsWorld->destroyObject(phyCon);


			if (isStatic)
			{
				// Re-merge the static Aabb.
				calculateLimits();
			}
		}
	}
}


// ----------------------------------------------------------------------------
void gkScene::applyBuiltinParents(void)
{
	// One time setup on load.
	GK_ASSERT(isLoading());

	gkGameObjectSet::Iterator it = m_loadedObjects.iterator();
	while (it.hasMoreElements())
	{
		gkGameObject *gobj = it.getNext(), *pobj=0;



		GK_ASSERT(gobj->isLoaded());

		const gkHashedString pname = gobj->getProperties().m_parent;

		if (!pname.str().empty())
			pobj = findLoadedObject(pname.str());

		if (pobj)
		{

			pobj->addChild(gobj);

			// m_transform no longer contains
			// parent information, we must do it here.


			gkMatrix4 omat, pmat;

			gobj->getProperties().m_transform.toMatrix(omat);
			pobj->getProperties().m_transform.toMatrix(pmat);

			omat = pmat.inverse() * omat;

			gkTransformState st;
			gkMathUtils::extractTransform(omat, st.loc, st.rot, st.scl);


			// apply
			gobj->setTransform(st);
		}
	}
}


// ----------------------------------------------------------------------------
void gkScene::applyBuiltinPhysics(void)
{
	GK_ASSERT(isLoading());


	gkGameObjectSet::Iterator it = m_loadedObjects.iterator();
	while (it.hasMoreElements())
		createPhysicsObject(it.getNext());
}

// ----------------------------------------------------------------------------
void gkScene::calculateLimits(void)
{
	// Update bounding info
	m_limits = gkBoundingBox::BOX_NULL;


	gkPhysicsControllerSet::Iterator it = m_staticControllers.iterator();
	while (it.hasMoreElements())
	{
		gkPhysicsController *phycon = it.getNext();
		m_limits.merge(phycon->getAabb());
	}
}


// ----------------------------------------------------------------------------
void gkScene::loadImpl(void)
{
	if (m_objects.empty())
	{
		gkPrintf("Scene: Has no objects in to load.\n", m_name.c_str());
		m_loadingState = ST_LOADFAILED;
		return;
	}

	// generic for now, but later scene properties will be used
	// to extract more detailed management information

	m_manager = Root::getSingleton().createSceneManager(ST_GENERIC, m_name);


	if (!m_baseProps.m_skyMat.empty())
		m_manager->setSkyBox(true, m_baseProps.m_skyMat, m_baseProps.m_skyDist, true, m_baseProps.m_skyOri);



	// create the world
	m_physicsWorld = new gkDynamicsWorld(m_name, this);


	gkGameObjectHashMap::Iterator it = m_objects.iterator();
	while (it.hasMoreElements())
	{
		gkGameObject *gobj = it.getNext().second;

		if (!gobj->isLoaded())
		{
			// Skip load of inactive layers
			if (m_layers & gobj->getLayer())
			{
				// call builder
				gobj->load();
			}
		}
	}

	// Build groups.
	m_groupManager->loadAll();

	// Build parent / child hierarchy.
	applyBuiltinParents();

	// Build physics.
	applyBuiltinPhysics();

	if (!m_viewport)
	{

		// setMainCamera has not been called, try to call
		if (m_startCam)
			setMainCamera(m_startCam);
		else
		{
			if (!m_cameras.empty())
				setMainCamera(m_cameras.at(0));
			else
			{
				m_startCam = createCamera(" -- No Camera -- ");

				m_startCam->getProperties().m_transform.set(
				    gkVector3(0, -5, 0),
				    gkEuler(90.f, 0.f, 0.f).toQuaternion(),
				    gkVector3(1.f, 1.f, 1.f)
				);
				m_startCam->load();
				setMainCamera(m_startCam);
			}
		}
	}

	GK_ASSERT(m_viewport);

	m_viewport->setBackgroundColour(m_baseProps.m_world);
	m_manager->setAmbientLight(m_baseProps.m_ambient);

	if (m_baseProps.m_fog.m_mode != gkFogParams::FM_NONE)
	{
		m_manager->setFog(  m_baseProps.m_fog.m_mode == gkFogParams::FM_QUAD ?  Ogre::FOG_EXP2 :
		                    m_baseProps.m_fog.m_mode == gkFogParams::FM_LIN  ?  Ogre::FOG_LINEAR :
		                    Ogre::FOG_EXP,
		                    m_baseProps.m_fog.m_color,
		                    m_baseProps.m_fog.m_intensity,
		                    m_baseProps.m_fog.m_start,
		                    m_baseProps.m_fog.m_end);
	}

	//Enable Shadows
	setShadows();

	// notify main scene
	gkEngine::getSingleton().setActiveScene(this);
}



// ----------------------------------------------------------------------------
void gkScene::postLoadImpl(void)
{
	gkLuaScript *script = gkLuaManager::getSingleton().getScript("OnLoad.lua");

	// load user application
	if (script)
		script->execute();
}



// ----------------------------------------------------------------------------
void gkScene::unloadImpl(void)
{
	if (m_objects.empty())
		return;

	if(m_navMeshData.get())
		m_navMeshData->unloadAll();

	// Free scripts
	gkLuaManager::getSingleton().unload();


	// Free all load lists
	m_cameras.clear(true);
	m_lights.clear(true);
	m_staticControllers.clear(true);

	m_groupManager->unloadAll();

	gkGameObjectSet::Iterator it = m_loadedObjects.iterator();
	while (it.hasMoreElements())
	{
		gkGameObject *gobj = it.getNext();

		GK_ASSERT(gobj->isLoaded());

		gobj->unload();
	}

	m_loadedObjects.clear(true);

	// Free cloned.
	destroyClones();

	// Remove any pending
	endObjects();


	if (m_physicsWorld)
	{
		delete m_physicsWorld;
		m_physicsWorld = 0;
	}

	if (m_debugger)
	{
		delete m_debugger;
		m_debugger = 0;
	}


	m_startCam = 0;
	m_limits = gkBoundingBox::BOX_NULL;



	// Clear ogre scene manager.
	if (m_manager)
	{
		Ogre::Root::getSingleton().destroySceneManager(m_manager);
		m_manager = 0;
	}



	// Finalize vp
	if (m_viewport)
	{
		RenderWindow *window = gkWindowSystem::getSingleton().getMainWindow();

		if (window)
		{
			window->removeViewport(0);
			m_viewport = 0;
		}
	}

	gkLogicManager::getSingleton().notifySceneUnloaded();
	gkWindowSystem::getSingleton().clearStates();
	gkEngine::getSingleton().setActiveScene(0);



#ifdef OGREKIT_OPENAL_SOUND

	gkSoundManager::getSingleton().stopAllSounds();

#endif
}


// ----------------------------------------------------------------------------
static Ogre::ShadowTechnique ParseShadowTechnique(const gkString &technique)
{
	gkString techniqueLower = technique;
	Ogre::StringUtil::toLowerCase(techniqueLower);

	if (techniqueLower == "none")
		return Ogre::SHADOWTYPE_NONE;
	else if (techniqueLower == "stencilmodulative")
		return Ogre::SHADOWTYPE_STENCIL_MODULATIVE;
	else if (techniqueLower == "stenciladditive")
		return Ogre::SHADOWTYPE_STENCIL_ADDITIVE;
	else if (techniqueLower == "texturemodulative")
		return Ogre::SHADOWTYPE_TEXTURE_MODULATIVE;
	else if (techniqueLower == "textureadditive")
		return Ogre::SHADOWTYPE_TEXTURE_ADDITIVE;
	else if (techniqueLower == "texturemodulativeintegrated")
		return Ogre::SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED;
	else if (techniqueLower == "textureadditiveintegrated")
		return Ogre::SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED;

	Ogre::StringUtil::StrStreamType errorMessage;
	errorMessage << "Invalid shadow technique specified: " << technique;

	OGRE_EXCEPT
	(
	    Ogre::Exception::ERR_INVALIDPARAMS,
	    errorMessage.str(),
	    "ParseShadowTechnique"
	);
}


// ----------------------------------------------------------------------------
void gkScene::setShadows()
{
	gkUserDefs &defs = gkEngine::getSingleton().getUserDefs();

	if(defs.enableshadows)
	{
		Ogre::ShadowTechnique shadowTechnique = ::ParseShadowTechnique(defs.shadowtechnique);

		m_manager->setShadowTechnique(shadowTechnique);

		m_manager->setShadowColour(defs.colourshadow);

		m_manager->setShadowFarDistance(defs.fardistanceshadow);
	}
}

// ----------------------------------------------------------------------------
void gkScene::notifyObjectLoaded(gkGameObject *gobj)
{
	bool result = m_loadedObjects.insert(gobj);
	UT_ASSERT(result);


	// Tell constraints
	GK_ASSERT(m_constraintManager);
	m_constraintManager->notifyObjectLoaded(gobj);


	bool isCurrentlyLoading = isLoading();

	if(m_navMeshData.get())
		m_navMeshData->updateOrLoad(gobj);


	// apply physics
	if (!isCurrentlyLoading)
		createPhysicsObject(gobj);


	if (gobj->getType() == GK_CAMERA)
		m_cameras.insert(gobj->getCamera());
	if (gobj->getType() == GK_LIGHT)
		m_lights.insert(gobj->getLight());
}


// ----------------------------------------------------------------------------
void gkScene::notifyObjectUnloaded(gkGameObject *gobj)
{
	m_loadedObjects.erase(gobj);


	// Tell constraints
	GK_ASSERT(m_constraintManager);
	m_constraintManager->notifyObjectUnloaded(gobj);


	if(m_navMeshData.get())
		m_navMeshData->unload(gobj);

	// destroy physics
	destroyPhysicsObject(gobj);


	if (!isUnloading())
	{
		if (gobj->getType() == GK_CAMERA)
			m_cameras.erase(gobj->getCamera());
		if (gobj->getType() == GK_LIGHT)
			m_lights.erase(gobj->getLight());
	}

}


// ----------------------------------------------------------------------------
void gkScene::notifyObjectUpdate(gkGameObject *gobj)
{
	m_markDBVT = true;

	if (!isLoading())
	{

		if(m_navMeshData.get())
			m_navMeshData->updateOrLoad(gobj);
	}
}


// ----------------------------------------------------------------------------
void gkScene::applyConstraints(void)
{
	GK_ASSERT(m_constraintManager);
	m_constraintManager->update(gkEngine::getStepRate());
}



// ----------------------------------------------------------------------------
void gkScene::destroyClones(void)
{
	if (!m_clones.empty())
	{
		gkGameObjectArray::Pointer buffer = m_clones.ptr();
		size_t size = m_clones.size();
		size_t i = 0;
		while (i < size)
		{
			gkGameObject *obj = buffer[i++];
			obj->unload();
			delete obj;
		}

		m_clones.clear();
	}
	if (!m_tickClones.empty())
	{
		gkGameObjectArray::Pointer buffer = m_tickClones.ptr();
		size_t size = m_tickClones.size();
		size_t i = 0;
		while (i < size)
		{
			gkGameObject *obj = buffer[i++];
			obj->unload();
			delete obj;
		}

		m_tickClones.clear();
	}
	m_cloneCount = 0;
}



// ----------------------------------------------------------------------------
void gkScene::tickClones(void)
{
	if (!m_tickClones.empty())
	{
		gkGameObjectArray::Pointer buffer = m_tickClones.ptr();
		UTsize size = m_tickClones.size();
		UTsize i = 0;


		while (i < size)
		{
			gkGameObject *obj = buffer[i++];
			gkGameObject::LifeSpan &life = obj->getLifeSpan();

			if ((life.tick++) > life.timeToLive)
				endObject(obj);
		}
	}
}



// ----------------------------------------------------------------------------
gkGameObject *gkScene::cloneObject(gkGameObject *obj, int lifeSpan)
{

	gkGameObject *nobj = obj->clone(gkUtils::getUniqueName(obj->getName()));
	nobj->setActiveLayer(true);

	gkGameObject::LifeSpan life = {0, lifeSpan};
	nobj->setLifeSpan(life);

	if (lifeSpan > 0)
		m_tickClones.push_back(nobj);
	else
		m_clones.push_back(nobj);

	return nobj;

}



// ----------------------------------------------------------------------------
void gkScene::endObject(gkGameObject *obj)
{
	m_endObjects.insert(obj);
}


// ----------------------------------------------------------------------------
void gkScene::unloadAndDestroy(gkGameObject *gobj)
{
	if (!gobj)
		return;

	gobj->unload();

	UTsize it;
	if (gobj->isClone())
	{
		if ((it = m_clones.find(gobj)) != UT_NPOS)
		{
			m_clones.erase(it);
			UT_ASSERT(!gobj->isInstance());

			delete gobj;

			if (m_clones.empty())
				m_tickClones.clear(true);
		}
		else if ((it = m_tickClones.find(gobj)) != UT_NPOS)
		{
			m_tickClones.erase(it);
			UT_ASSERT(!gobj->isInstance());

			delete gobj;

			if (m_tickClones.empty())
				m_tickClones.clear(true);
		}
		else 
		{
			// Instances must remain! (reloadable)
			if (!gobj->isInstance())
			{
				UT_ASSERT(0 && "Missing Object");
			}
		}

	}
}


// ----------------------------------------------------------------------------
void gkScene::endObjects(void)
{
	if (!m_endObjects.empty())
	{

		gkGameObjectSet::Iterator it = m_endObjects.iterator();
		while (it.hasMoreElements())
			unloadAndDestroy(it.getNext());

		m_endObjects.clear(true);
	}

}



// ----------------------------------------------------------------------------
void gkScene::beginFrame(void)
{
	if (!isLoaded())
		return;

	// end any objects up for removal
	endObjects();

	GK_ASSERT(m_physicsWorld);
	m_physicsWorld->resetContacts();

#ifdef OGREKIT_OPENAL_SOUND

	// process sounds waiting to be finished
	gkSoundManager::getSingleton().collectGarbage();

#endif
}




// ----------------------------------------------------------------------------
void gkScene::update(gkScalar tickRate)
{
	if (!isLoaded())
		return;

	GK_ASSERT(m_physicsWorld);

	// update simulation
	gkStats::getSingleton().startClock();
	m_physicsWorld->step(tickRate);
	gkStats::getSingleton().stopPhysicsClock();


	// update logic bricks
	gkStats::getSingleton().startClock();
	gkLogicManager::getSingleton().update(tickRate);
	gkStats::getSingleton().stopLogicBricksClock();

	// update node trees
	gkStats::getSingleton().startClock();
	gkNodeManager::getSingleton().update(tickRate);
	gkStats::getSingleton().stopLogicNodesClock();

#if OGREKIT_OPENAL_SOUND
	// update sound manager.
	gkStats::getSingleton().startClock();
	gkSoundManager::getSingleton().update(this);
	gkStats::getSingleton().stopSoundClock();
#endif


	gkStats::getSingleton().startClock();
	if (m_markDBVT)
	{
		m_markDBVT = false;
		m_physicsWorld->handleDbvt(m_startCam);
	}

	gkStats::getSingleton().stopDbvtClock();

	if (m_debugger)
	{
		m_physicsWorld->DrawDebug();
		m_debugger->flush();
	}


	// tick life span
	tickClones();


	// Free any 
	endObjects();
}



// ----------------------------------------------------------------------------
bool gkScene::asyncTryToCreateNavigationMesh(gkActiveObject &activeObj, const gkRecast::Config &config, ASYNC_DT_RESULT result)
{
	class CreateNavMeshCall : public gkCall
	{
	public:

		CreateNavMeshCall(PMESHDATA meshData, const gkRecast::Config &config, ASYNC_DT_RESULT result)
			: m_meshData(meshData), m_config(config), m_result(result) {}

		~CreateNavMeshCall() {}

		void run() { m_result = gkRecast::createNavMesh(m_meshData, m_config); }


	private:

		PMESHDATA m_meshData;

		gkRecast::Config m_config;

		ASYNC_DT_RESULT m_result;
	};

	result.reset();

	if(m_navMeshData.get() && m_navMeshData->hasChanged())
	{
		gkPtrRef<gkCall> call(new CreateNavMeshCall(PMESHDATA(m_navMeshData->cloneData()), config, result));

		activeObj.enqueue(call);

		m_navMeshData->resetHasChanged();

		return true;
	}

	return false;
}
