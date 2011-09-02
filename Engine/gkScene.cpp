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
#include "gkCommon.h"
#include "OgreRoot.h"
#include "OgreSceneManager.h"
#include "OgreRenderWindow.h"
#include "OgreViewport.h"
#include "OgreStringConverter.h"

#include "gkWindowSystem.h"
#include "gkWindow.h"
#include "gkViewport.h"
#include "gkCamera.h"
#include "gkEntity.h"
#include "gkLight.h"
#include "gkSkeleton.h"
#include "gkParticleObject.h"
#include "gkGameObjectGroup.h"
#include "gkEngine.h"
#include "gkScene.h"
#include "gkLogicManager.h"
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
#include "gkGameObjectManager.h"

#ifdef OGREKIT_USE_NNODE
#include "gkNodeManager.h"
#endif

#ifdef OGREKIT_OPENAL_SOUND
#include "gkSoundManager.h"
#endif

#ifdef OGREKIT_USE_LUA
#include "Script/Lua/gkLuaManager.h"
#endif


#if GK_EXTERNAL_RENDER == GK_EXTERNAL_RENDER_OGREKIT
#include "External/Ogre/gkOgreMaterialLoader.h"
#include "External/Ogre/gkOgreSkyBoxGradient.h"
#endif

#ifdef OGREKIT_USE_RTSHADER_SYSTEM
#include "OgreRTShaderSystem.h"
#endif

//using namespace Ogre;

#define DEFAULT_STARTUP_LUA_FILE		"OnInit.lua"



gkScene::gkScene(gkInstancedManager* creator, const gkResourceName& name, const gkResourceHandle& handle)
	:    gkInstancedObject(creator, name, handle),
	     m_manager(0),
	     m_startCam(0),
	     m_viewport(0),
	     m_baseProps(),
	     m_constraintManager(0),
	     m_physicsWorld(0),
	     m_meshManager(gkMeshManager::getSingletonPtr()),
	     m_debugger(0),
	     m_hasLights(false),
	     m_markDBVT(false),
	     m_cloneCount(0),
	     m_layers(0xFFFFFFFF),
	     m_skybox(0),
		 m_window(0),
		 m_updateFlags(UF_ALL),
		 m_blendFile(0)
{
}



gkScene::~gkScene()
{
	if (m_skybox)
	{
		delete m_skybox;
		m_skybox = 0;
	}

	if (m_constraintManager)
	{
		// unlink all constraints.

		gkGameObjectHashMap::Iterator it = m_objects.iterator();
		while (it.hasMoreElements())
		{
			gkGameObject* gobj = it.getNext().second;
			gobj->setOwner(0);
			m_constraintManager->notifyObjectDestroyed(gobj);
		}

		delete m_constraintManager;
		m_constraintManager = 0;
	}

	m_objects.clear();
}


gkWindow* gkScene::getDisplayWindow(void)
{
	return m_window ? m_window : gkWindowSystem::getSingleton().getMainWindow();
}

void gkScene::setDisplayWindow(gkWindow* window)
{
	m_window = window;
	if (m_window)
		m_window->setRenderScene(this);
}


bool gkScene::hasObject(const gkHashedString& ob)
{
	bool result = m_objects.find(ob) != GK_NPOS;

	if (!result)
	{
		gkGameObjectManager& mgr = gkGameObjectManager::getSingleton();
		gkResourceName rname(ob, m_name.group);
		if (mgr.exists(rname))
		{
			gkGameObject* obj = mgr.getByName<gkGameObject>(rname);
			if (obj && obj->getOwner() == this)
				result = true;
		}
	}
	return result;

}


bool gkScene::hasObject(gkGameObject* gobj)
{
	GK_ASSERT(gobj);
	bool result = m_objects.find(gobj->getName()) != GK_NPOS;

	if (!result)
	{
		gkGameObjectManager& mgr = gkGameObjectManager::getSingleton();
		if (mgr.exists(gobj->getResourceHandle()))
		{
			gkGameObject* obj = mgr.getByHandle<gkGameObject>(gobj->getResourceHandle());
			if (obj && obj->getOwner() == this)
				result = true;
		}
	}
	return result;
}




gkGameObject* gkScene::getObject(const gkHashedString& name)
{
	UTsize pos = m_objects.find(name);
	if (pos != GK_NPOS)
		return m_objects.at(pos);
	else
	{
		gkGameObjectManager& mgr = gkGameObjectManager::getSingleton();

		gkResourceName rname(name, getGroupName());
		if (mgr.exists(rname))
		{
			gkGameObject* obj = mgr.getByName<gkGameObject>(rname);
			if (obj && obj->getOwner() == this)
				return obj;
		}

	}
	return 0;
}


bool gkScene::hasMesh(const gkHashedString& ob)
{
	GK_ASSERT(m_meshManager);
	return m_meshManager->exists(ob);
}


gkMesh* gkScene::getMesh(const gkHashedString& name)
{
	GK_ASSERT(m_meshManager);
	return (gkMesh*)m_meshManager->getByName(name);
}



gkMesh* gkScene::createMesh(const gkHashedString& name)
{
	GK_ASSERT(m_meshManager);
	return (gkMesh*)m_meshManager->create(gkResourceName(name, getGroupName()));
}



gkGameObject* gkScene::createObject(const gkHashedString& name)
{
	if (m_objects.find(name) != GK_NPOS)
	{
		gkPrintf("Scene: Duplicate object '%s' found\n", name.str().c_str());
		return 0;
	}

	gkGameObject* gobj = gkGameObjectManager::getSingleton().createObject(gkResourceName(name, getGroupName()));
	addObject(gobj);
	return gobj;
}


gkLight* gkScene::createLight(const gkHashedString& name)
{
	if (m_objects.find(name) != GK_NPOS)
	{
		gkPrintf("Scene: Duplicate object '%s' found\n", name.str().c_str());
		return 0;
	}

	gkLight* gobj = gkGameObjectManager::getSingleton().createLight(gkResourceName(name, getGroupName()));
	addObject(gobj);
	return gobj;
}




gkCamera* gkScene::createCamera(const gkHashedString& name)
{
	if (m_objects.find(name) != GK_NPOS)
	{
		gkPrintf("Scene: Duplicate object '%s' found\n", name.str().c_str());
		return 0;
	}

	gkCamera* gobj = gkGameObjectManager::getSingleton().createCamera(gkResourceName(name, getGroupName()));
	addObject(gobj);
	return gobj;
}



gkEntity* gkScene::createEntity(const gkHashedString& name)
{
	if (m_objects.find(name) != GK_NPOS)
	{
		gkPrintf("Scene: Duplicate object '%s' found\n", name.str().c_str());
		return 0;
	}


	gkEntity* gobj = gkGameObjectManager::getSingleton().createEntity(gkResourceName(name, getGroupName()));
	addObject(gobj);
	return gobj;
}




gkSkeleton* gkScene::createSkeleton(const gkHashedString& name)
{
	if (m_objects.find(name) != GK_NPOS)
	{
		gkPrintf("Scene: Duplicate object '%s' found\n", name.str().c_str());
		return 0;
	}


	gkSkeleton* gobj = gkGameObjectManager::getSingleton().createSkeleton(gkResourceName(name, getGroupName()));
	addObject(gobj);
	return gobj;
}

gkParticleObject* gkScene::createParticleObject(const gkHashedString& name)
{
	if (m_objects.find(name) != GK_NPOS)
	{
		gkPrintf("Scene: Duplicate object '%s' found\n", name.str().c_str());
		return 0;
	}

	gkParticleObject* gobj = gkGameObjectManager::getSingleton().createParticleObject(gkResourceName(name, getGroupName()));
	addObject(gobj);
	return gobj;
}


bool gkScene::_replaceObjectInScene(gkGameObject* gobj, gkScene* osc, gkScene* nsc)
{
	GK_ASSERT(gobj && gobj->getOwner() == osc && gobj->getOwner() != nsc);

	if (nsc->getObject(gobj->getName()) != 0)
	{
		gkLogMessage("Scene: Another object by the name "
		             << gobj->getName() << " exists in scene " << nsc->getName() << ". Cannot replace!");
		return false;
	}


	osc->_eraseObject(gobj);


	gobj->setOwner(nsc);
	gobj->setActiveLayer(true);
	gobj->setLayer(m_layers);


	nsc->m_objects.insert(gobj->getName(), gobj);

	if (nsc->isInstanced())
		gobj->createInstance();

	return true;
}



void gkScene::_eraseObject(gkGameObject* gobj)
{
	const gkHashedString name = gobj->getName();
	if (m_objects.find(name) == UT_NPOS)
	{
		gkPrintf("Scene: object '%s' not found in this scene\n", name.str().c_str());
		return;
	}

	gobj->destroyInstance();
	gobj->setOwner(0);

	if (m_constraintManager)
		m_constraintManager->notifyObjectDestroyed(gobj);

	m_objects.remove(name);
}

void gkScene::_eraseAllObjects()
{
	gkGameObjectHashMap::Iterator it = m_objects.iterator();
	while (it.hasMoreElements())
	{
		gkGameObject* gobj = it.getNext().second;
		
		//gobj->destroyInstance();
		
		gkGameObjectManager::getSingleton().destroy(gobj);
		//gobj->setOwner(0);		
	}

	m_objects.clear();

	if (m_constraintManager) 
		m_constraintManager->clear();	
}


void gkScene::addObject(gkGameObject* gobj)
{
	GK_ASSERT(gobj);

	const gkHashedString name = gobj->getName();

	if (m_objects.find(name) != GK_NPOS)
	{
		gkPrintf("Scene: Duplicate object '%s' found in this scene\n", name.str().c_str());
		return;
	}


	if (gobj->getType() == GK_LIGHT)
		m_hasLights = true;


	if (gobj->getOwner() != this)
	{
		if (gobj->getOwner() != 0)
		{
			if (!_replaceObjectInScene(gobj, gobj->getOwner(), this))
				return;
		}
		else
		{
			gobj->setOwner(this);
			m_objects.insert(name, gobj);
		}
	}

}



void gkScene::removeObject(gkGameObject* gobj)
{
	const gkHashedString name = gobj->getName();
	if (m_objects.find(name) != GK_NPOS)
	{
		gkPrintf("Scene: object '%s' not found in this scene\n", name.str().c_str());
		return;
	}

	gobj->destroyInstance();
}


void gkScene::destroyObject(gkGameObject* gobj)
{
	const gkHashedString name = gobj->getName();
	if (gobj->getOwner() != this)
	{
		gkPrintf("Scene: object '%s' not found in this scene\n", name.str().c_str());
		return;
	}


	gobj->destroyInstance();
	m_objects.remove(name);
	gkGameObjectManager::getSingleton().destroy(gobj);
	//delete gobj;
}


void gkScene::setSceneManagerType(int type)
{
	// Runtime switching is not allowed, unless the entire scene is reinstanced.
	m_baseProps.m_manager = type;
}


int gkScene::getSceneManagerType(void)
{
	return m_baseProps.m_manager;
}



void gkScene::setHorizonColor(const gkColor& col)
{
	if (isInstanced())
	{
		GK_ASSERT(m_manager && m_viewport);
		m_viewport->getViewport()->setBackgroundColour(col);
	}
	else
	{
		// save for reinstanced state.
		m_baseProps.m_material.m_horizon = col;
	}
}


const gkColor& gkScene::getHorizonColor(void)
{
	return m_baseProps.m_material.m_horizon;
}



void gkScene::setZenithColor(const gkColor& col)
{
	if (isInstanced())
	{
		GK_ASSERT(m_manager && m_viewport);
		m_viewport->getViewport()->setBackgroundColour(col);
	}
	else
	{
		// save for reinstanced state.
		m_baseProps.m_material.m_horizon = col;
	}
}


const gkColor& gkScene::getZenithColor(void)
{
	return m_baseProps.m_material.m_zenith;
}


void gkScene::setAmbientColor(const gkColor& col)
{
	if (isInstanced())
	{
		GK_ASSERT(m_manager);
		m_manager->setAmbientLight(col);
	}
	else
	{
		m_baseProps.m_material.m_ambient = col;
	}
}



const gkColor& gkScene::getAmbientColor(void)
{
	return m_baseProps.m_material.m_ambient;
}


void gkScene::setGravity(const gkVector3& grav)
{
	if (m_physicsWorld)
		m_physicsWorld->getBulletWorld()->setGravity(gkMathUtils::get(grav));
	else
		m_baseProps.m_gravity = grav;

}



const gkVector3& gkScene::getGravity(void)
{
	return m_baseProps.m_gravity;
}


gkDebugger* gkScene::getDebugger(void)
{
	if (isInstanced() && !m_debugger)
		m_debugger = new gkDebugger(this);
	return m_debugger;
}


gkDynamicsWorld* gkScene::getDynamicsWorld(void)
{
	if (!m_physicsWorld)
		m_physicsWorld = new gkDynamicsWorld(m_name.getName(), this);
	return m_physicsWorld;
}


void gkScene::addConstraint(gkGameObject* gobj, gkConstraint* co)
{
	if (gobj && co)
	{
		gkConstraintManager* mgr = getConstraintManager();

		mgr->addConstraint(gobj, co);

		if (gobj->isInstanced())
		{
			mgr->notifyInstanceCreated(gobj);
		}

	}
}


gkConstraintManager* gkScene::getConstraintManager(void)
{
	if (!m_constraintManager)
		m_constraintManager = new gkConstraintManager();
	return m_constraintManager;
}


void gkScene::getGroups(gkGroupArray& groups)
{
}


gkGameObject* gkScene::findInstancedObject(const gkString& name)
{
	gkGameObjectSet::Iterator it = m_instanceObjects.iterator();
	while (it.hasMoreElements())
	{
		gkGameObject* obj = it.getNext();
		if (obj->getName() == name)
			return obj;
	}
	return 0;
}



void gkScene::setMainCamera(gkCamera* cam)
{
	if (!cam)
		return;

	m_startCam = cam;

	Ogre::Camera* main = m_startCam->getCamera();
	GK_ASSERT(main);


	if (!m_viewport)
	{
		gkWindow* window = getDisplayWindow(); GK_ASSERT(window);
		m_viewport = window->addViewport(cam);
	}
	else
		m_viewport->getViewport()->setCamera(main);

	GK_ASSERT(m_viewport);

	gkVector2 size = getDisplayWindow()->getSize();

	main->setAspectRatio(size.x / size.y);
	cam->setFov(gkDegree(cam->getFov()));
}



gkRigidBody* gkScene::createRigidBody(gkGameObject* obj, gkPhysicsProperties& prop)
{
	_destroyPhysicsObject(obj);
	obj->getProperties().m_physics = prop;

	gkRigidBody* con = m_physicsWorld->createRigidBody(obj);

	obj->attachRigidBody(con);

	if (con->isStaticObject())
	{
		m_staticControllers.insert(con);
		m_limits.merge(con->getAabb());
	}

	return 0;
}


void gkScene::_createPhysicsObject(gkGameObject* obj)
{
	GK_ASSERT(obj && obj->isInstanced() && m_physicsWorld);

	gkGameObjectProperties&  props  = obj->getProperties();

	if (!props.isPhysicsObject() || obj->hasParent())
		return;

	gkEntity* ent = obj->getEntity();
	if (ent && props.m_physics.isMeshShape())
	{
		// can happen with the newer mesh objects
		// which test for the collision face flag on each triangle

		gkMesh* me = ent->getEntityProperties().m_mesh;
		btTriangleMesh* trimesh = me->getTriMesh();

		if (trimesh->getNumTriangles() == 0)
			props.m_physics.m_type = GK_NO_COLLISION;
	}

	// Replace controller.
	if (obj->getPhysicsController())
		m_physicsWorld->destroyObject(obj->getPhysicsController());


	if (props.isGhost())
	{
		gkCharacter* con = m_physicsWorld->createCharacter(obj);
		obj->attachCharacter(con);
	}
	else
	{
		gkRigidBody* con = m_physicsWorld->createRigidBody(obj);
		obj->attachRigidBody(con);

		if (con->isStaticObject())
		{
			m_staticControllers.insert(con);
			m_limits.merge(con->getAabb());
		}
	}
}

void gkScene::_postCreatePhysicsObject(gkGameObject* obj)
{
	GK_ASSERT(obj && obj->isInstanced() && m_physicsWorld);

	gkGameObjectProperties&  props  = obj->getProperties();
	if (!props.isPhysicsObject())
		return;

	if (props.m_physics.hasPhysicsConstraint())
		_createPhysicsConstraint(obj);

	if (props.m_physics.isCompoundChild())
	{
		gkGameObject* parent = obj->getParent(); 
		while (parent && parent->getParent()) parent = parent->getParent();
		if (!parent || !parent->getProperties().isRigidOrDynamic() || !parent->getProperties().m_physics.isCompound()) 
			return;

		gkPhysicsController* parentCont = parent->getPhysicsController(); 
		GK_ASSERT(parentCont && parentCont->getShape() && parentCont->getShape()->isCompound());
		btCompoundShape* compShape = static_cast<btCompoundShape*>(parentCont->getShape());
		
		gkPhysicsController cont(obj, m_physicsWorld);
		btCollisionShape *shape = cont._createShape();
		
		gkMatrix4 m;
		if (obj->getParent() != parent)
			m = parent->getWorldTransform().inverse() * obj->getWorldTransform();
		else
			m = obj->getTransform();

		shape->setLocalScaling(gkMathUtils::get(obj->getWorldScale()));
		compShape->addChildShape(gkMathUtils::get(m), shape);

		gkRigidBody* body = static_cast<gkRigidBody*>(parent->getPhysicsController());
		GK_ASSERT(body);
		body->recalLocalInertia();
	}
}


void gkScene::_createPhysicsConstraint(gkGameObject* obj)
{
	GK_ASSERT(obj && obj->isInstanced() && m_physicsWorld);
	gkGameObjectProperties&  props  = obj->getProperties();

	GK_ASSERT(props.isPhysicsObject() && props.m_physics.hasPhysicsConstraint());
	if (!props.isPhysicsObject() || !props.m_physics.hasPhysicsConstraint() || obj->hasParent())
		return;

	gkRigidBody* body = obj->getAttachedBody();	
	GK_ASSERT(body);
	body->createConstraints();
}



void gkScene::_destroyPhysicsObject(gkGameObject* obj)
{
	GK_ASSERT(obj && m_physicsWorld);

	if (obj->getPhysicsController())
	{
		gkPhysicsController* phyCon = obj->getPhysicsController();

		obj->attachRigidBody(0);
		obj->attachCharacter(0);

		if (!isBeingDestroyed())
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



void gkScene::_applyBuiltinParents(gkGameObjectSet& instanceObjects)
{
	gkGameObjectSet::Iterator it = instanceObjects.iterator();
	while (it.hasMoreElements())
	{
		gkGameObject* gobj = it.getNext(), *pobj = 0;

		GK_ASSERT(gobj->isInstanced());

		const gkHashedString pname = gobj->getProperties().m_parent;

		if (!pname.str().empty())
			pobj = findInstancedObject(pname.str());

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



void gkScene::_applyBuiltinPhysics(gkGameObjectSet& instanceObjects)
{
	utArray<gkGameObject*> objs;
	objs.reserve(instanceObjects.size());

	gkGameObjectSet::Iterator it = instanceObjects.iterator();
	while (it.hasMoreElements())
	{
		gkGameObject* obj = it.getNext();
		if (obj->getProperties().isPhysicsObject())
		{
			_createPhysicsObject(obj);
			if (obj->getProperties().m_physics.isLinkedToOther())
				objs.push_back(obj);
		}
	}

	UTsize i;
	for (i = 0; i < objs.size(); i++)
		_postCreatePhysicsObject(objs[i]);
}



void gkScene::calculateLimits(void)
{
	// Update bounding info
	m_limits = gkBoundingBox::BOX_NULL;


	gkPhysicsControllerSet::Iterator it = m_staticControllers.iterator();
	while (it.hasMoreElements())
	{
		gkPhysicsController* phycon = it.getNext();
		m_limits.merge(phycon->getAabb());
	}
}



void gkScene::createInstanceImpl(void)
{
	if (m_objects.empty())
	{
		gkPrintf("Scene: '%s' Has no creatable objects.\n", m_name.getName().c_str());
		m_instanceState = ST_ERROR;
		return;
	}

	if (!m_window)
		setDisplayWindow(gkWindowSystem::getSingleton().getMainWindow());

	// generic for now, but later scene properties will be used
	// to extract more detailed management information

	m_manager = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC, m_name.getFullName());
#if OGREKIT_USE_RTSHADER_SYSTEM
	Ogre::RTShader::ShaderGenerator::getSingleton().addSceneManager(m_manager);
#endif

	m_skybox  = gkMaterialLoader::loadSceneSkyMaterial(this, m_baseProps.m_material);



	// create the world
	(void)getDynamicsWorld();


	gkGameObjectHashMap::Iterator it = m_objects.iterator();
	while (it.hasMoreElements())
	{
		gkGameObject* gobj = it.getNext().second;

		if (!gobj->isInstanced())
		{
			// Skip creation of inactive layers
			if (m_layers & gobj->getLayer())
			{
				// call builder
				gobj->createInstance();
			}
		}
	}

	// Build groups.
	gkGroupManager::getSingleton().createGameObjectInstances(this);

	if (gkEngine::getSingleton().getUserDefs().buildStaticGeometry)
		gkGroupManager::getSingleton().createStaticBatches(this);


	// Build parent / child hierarchy.
	_applyBuiltinParents(m_instanceObjects);

	// Build physics.
	_applyBuiltinPhysics(m_instanceObjects);

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
				m_startCam->createInstance();
				setMainCamera(m_startCam);
			}
		}
	}

	GK_ASSERT(m_viewport);

	m_viewport->getViewport()->setBackgroundColour(m_baseProps.m_material.m_horizon);
	m_manager->setAmbientLight(m_baseProps.m_material.m_ambient);



	const gkString& iparam = gkEngine::getSingleton().getUserDefs().viewportOrientation;

	if (!iparam.empty())
	{
		int oparam = Ogre::OR_PORTRAIT;
		if (iparam == "landscaperight") //viewport orientation is reversed.
			oparam = Ogre::OR_LANDSCAPELEFT;
		else if (iparam == "landscapeleft")
			oparam = Ogre::OR_LANDSCAPERIGHT;

		m_viewport->getViewport()->setOrientationMode((Ogre::OrientationMode)oparam);
	}


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


#ifdef OGREKIT_OPENAL_SOUND
	// Set sound scene.

	gkSoundManager& sndMgr = gkSoundManager::getSingleton();
	sndMgr.getProperties() = m_soundScene;

	sndMgr.updateSoundProperties();

#endif


	// notify main scene
	gkEngine::getSingleton().registerActiveScene(this);
}


void gkScene::postCreateInstanceImpl(void)
{
#ifdef OGREKIT_USE_LUA
	gkLuaScript* script = gkLuaManager::getSingleton().getByName<gkLuaScript>(gkResourceName(DEFAULT_STARTUP_LUA_FILE, getGroupName()));

	if (script)
		script->execute();
#endif
}




void gkScene::destroyInstanceImpl(void)
{
	//if (m_objects.empty())
	//	return;

	if (m_navMeshData.get())
		m_navMeshData->destroyInstances();

#ifdef OGREKIT_USE_LUA
	// Free scripts
	gkLuaManager::getSingleton().decompileGroup(getGroupName());
#endif

	m_cameras.clear(true);
	m_lights.clear(true);
	m_staticControllers.clear(true);


	gkGroupManager::getSingleton().destroyGameObjectInstances(this);


	// Destroy all batched geometry
	if (gkEngine::getSingleton().getUserDefs().buildStaticGeometry)
		gkGroupManager::getSingleton().destroyStaticBatches(this);




	gkGameObjectSet::Iterator it = m_instanceObjects.iterator();
	while (it.hasMoreElements())
	{
		gkGameObject* gobj = it.getNext();

		GK_ASSERT(gobj->isInstanced());

		gobj->destroyInstance();
	//	gkGameObjectManager::getSingleton().destroy(gobj);
	}
	m_instanceObjects.clear(true);


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

	if (m_skybox)
	{
		delete m_skybox;
		m_skybox = 0;
	}


	m_startCam = 0;
	m_limits = gkBoundingBox::BOX_NULL;


	if (m_manager)
	{
#if OGREKIT_USE_RTSHADER_SYSTEM
		Ogre::RTShader::ShaderGenerator::getSingleton().removeSceneManager(m_manager);
#endif
		Ogre::Root::getSingleton().destroySceneManager(m_manager);
		m_manager = 0;
	}



	if (m_viewport)
	{
		getDisplayWindow()->removeViewport(m_viewport);		
		m_viewport = 0;		
	}

	gkLogicManager::getSingleton().notifySceneInstanceDestroyed();
	gkWindowSystem::getSingleton().clearStates();

	
	gkEngine::getSingleton().unregisterActiveScene(this);



#ifdef OGREKIT_OPENAL_SOUND
	gkSoundManager::getSingleton().stopAllSounds();
#endif

	//_eraseAllObjects();
}



static Ogre::ShadowTechnique ParseShadowTechnique(const gkString& technique)
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



void gkScene::setShadows()
{
	gkUserDefs& defs = gkEngine::getSingleton().getUserDefs();

	if (defs.enableshadows && defs.hasFixedCapability)
	{
		Ogre::ShadowTechnique shadowTechnique = ::ParseShadowTechnique(defs.shadowtechnique);
		m_manager->setShadowTechnique(shadowTechnique);
		m_manager->setShadowColour(defs.colourshadow);
		m_manager->setShadowFarDistance(defs.fardistanceshadow);
	}
}


void gkScene::notifyInstanceCreated(gkGameObject* gobj)
{
	bool result = m_instanceObjects.insert(gobj);
	UT_ASSERT(result);


	// Tell constraints
	if (m_constraintManager)
	{
		m_constraintManager->notifyInstanceCreated(gobj);
	}


	if (m_navMeshData.get())
		m_navMeshData->updateOrCreate(gobj);


	// apply physics
	if (!isBeingCreated())
	{
		_createPhysicsObject(gobj);
			_postCreatePhysicsObject(gobj);
	}


	if (gobj->getType() == GK_CAMERA)
		m_cameras.insert(gobj->getCamera());
	else if (gobj->getType() == GK_LIGHT)
		m_lights.insert(gobj->getLight());
}



void gkScene::notifyInstanceDestroyed(gkGameObject* gobj)
{
	m_instanceObjects.erase(gobj);


	// Tell constraints
	if (m_constraintManager)
		m_constraintManager->notifyInstanceDestroyed(gobj);


	if (m_navMeshData.get())
		m_navMeshData->destroyInstance(gobj);

	// destroy physics
	_destroyPhysicsObject(gobj);


	if (!isBeingDestroyed())
	{
		if (gobj->getType() == GK_CAMERA)
			m_cameras.erase(gobj->getCamera());
		else if (gobj->getType() == GK_LIGHT)
			m_lights.erase(gobj->getLight());
	}

}



void gkScene::notifyObjectUpdate(gkGameObject* gobj)
{
	m_markDBVT = true;

	if (!isBeingCreated())
	{

		if (m_navMeshData.get())
			m_navMeshData->updateOrCreate(gobj);
	}
}



void gkScene::applyConstraints(void)
{
	if (m_constraintManager)
		m_constraintManager->update(gkEngine::getStepRate());
}




void gkScene::destroyClones(void)
{
	if (!m_clones.empty())
	{
		gkGameObjectArray::Pointer buffer = m_clones.ptr();
		size_t size = m_clones.size();
		size_t i = 0;
		while (i < size)
		{
			gkGameObject* obj = buffer[i++];
			obj->destroyInstance();
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
			gkGameObject* obj = buffer[i++];
			obj->destroyInstance();
			delete obj;
		}

		m_tickClones.clear();
	}
	m_cloneCount = 0;
}




void gkScene::tickClones(void)
{
	if (!m_tickClones.empty())
	{
		gkGameObjectArray::Pointer buffer = m_tickClones.ptr();
		UTsize size = m_tickClones.size();
		UTsize i = 0;


		while (i < size)
		{
			gkGameObject* obj = buffer[i++];
			gkGameObject::LifeSpan& life = obj->getLifeSpan();

			if ((life.tick++) > life.timeToLive)
				endObject(obj);
		}
	}
}




gkGameObject* gkScene::cloneObject(gkGameObject* obj, int lifeSpan, bool instantiate)
{

	gkGameObject* nobj = obj->clone(gkUtils::getUniqueName(obj->getName()));
	nobj->setActiveLayer(true);

	gkGameObject::LifeSpan life = {0, lifeSpan};
	nobj->setLifeSpan(life);


	if (nobj->getOwner() != this)
		nobj->setOwner(this);

	if (lifeSpan > 0)
		m_tickClones.push_back(nobj);
	else
		m_clones.push_back(nobj);
	

	if (instantiate)
		nobj->createInstance();

	return nobj;

}




void gkScene::endObject(gkGameObject* obj)
{
	m_endObjects.insert(obj);
}



void gkScene::_unloadAndDestroy(gkGameObject* gobj)
{
	if (!gobj)
		return;

	gobj->destroyInstance();

	UTsize it;
	if (gobj->isClone())
	{
		if ((it = m_clones.find(gobj)) != UT_NPOS)
		{
			m_clones.erase(it);
			UT_ASSERT(!gobj->isGroupInstance());

			delete gobj;

			if (m_clones.empty())
				m_tickClones.clear(true);
		}
		else if ((it = m_tickClones.find(gobj)) != UT_NPOS)
		{
			m_tickClones.erase(it);
			UT_ASSERT(!gobj->isGroupInstance());

			delete gobj;

			if (m_tickClones.empty())
				m_tickClones.clear(true);
		}
		else
		{
			// Instances must remain! (reloadable)
			if (!gobj->isGroupInstance())
			{
				UT_ASSERT(0 && "Missing Object");
			}
		}

	}
}



void gkScene::endObjects(void)
{
	if (!m_endObjects.empty())
	{

		gkGameObjectSet::Iterator it = m_endObjects.iterator();
		while (it.hasMoreElements())
			_unloadAndDestroy(it.getNext());

		m_endObjects.clear(true);
	}

}


void gkScene::pushAnimationUpdate(gkGameObject* obj)
{
	m_updateAnimObjects.insert(obj);
}

void gkScene::removeAnimationUpdate(gkGameObject* obj)
{
	m_updateAnimObjects.erase(obj);
}



void gkScene::updateObjectsAnimations(const gkScalar tick)
{
	gkScalar animtick = tick;
	
	// avoid huge step in animations while debugging
	if (animtick > 0.1f)
		animtick = 0.016667f;
	
	if (!m_updateAnimObjects.empty())
	{
		gkGameObjectSet::Iterator it = m_updateAnimObjects.iterator();
		while (it.hasMoreElements())
			it.getNext()->updateAnimationBlender(animtick);

		//m_endObjects.clear(true);
	}

}




void gkScene::beginFrame(void)
{
	if (!isInstanced())
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





void gkScene::update(gkScalar tickRate)
{
	if (!isInstanced())
		return;

	GK_ASSERT(m_physicsWorld);

	// update simulation
	if (m_updateFlags & UF_PHYSICS)
	{
		gkStats::getSingleton().startClock();
		m_physicsWorld->step(tickRate);
		gkStats::getSingleton().stopPhysicsClock();
	}


	// update logic bricks
	if (m_updateFlags & UF_LOGIC_BRICKS)
	{
		gkStats::getSingleton().startClock();
		gkLogicManager::getSingleton().update(tickRate);
		gkStats::getSingleton().stopLogicBricksClock();
	}

#ifdef OGREKIT_USE_NNODE
	// update node trees
	if (m_updateFlags & UF_NODE_TREES)
	{
		gkStats::getSingleton().startClock();
		gkNodeManager::getSingleton().update(tickRate);
		gkStats::getSingleton().stopLogicNodesClock();
	}
#endif

	// update animations
	if (m_updateFlags & UF_ANIMATIONS)
	{
		gkStats::getSingleton().startClock();
		updateObjectsAnimations(tickRate);
		gkStats::getSingleton().stopAnimationsClock();
	}


#ifdef OGREKIT_OPENAL_SOUND
	// update sound manager.
	if (m_updateFlags & UF_SOUNDS)
	{
		gkStats::getSingleton().startClock();
		gkSoundManager::getSingleton().update(this);
		gkStats::getSingleton().stopSoundClock();
	}
#endif

	if (m_updateFlags & UF_DBVT)
	{
		gkStats::getSingleton().startClock();
		if (m_markDBVT)
		{
			m_markDBVT = false;
			m_physicsWorld->handleDbvt(m_startCam);
		}
		gkStats::getSingleton().stopDbvtClock();
	}

	if (m_updateFlags & UF_DEBUG)
	{
		if (m_debugger)
		{
			m_physicsWorld->DrawDebug();
			m_debugger->flush();
		}
	}


	// tick life span
	tickClones();


	// Free any
	endObjects();
}



#ifdef OGREKIT_COMPILE_RECAST
bool gkScene::asyncTryToCreateNavigationMesh(gkActiveObject& activeObj, const gkRecast::Config& config, ASYNC_DT_RESULT result)
{
	class CreateNavMeshCall : public gkCall
	{
	public:

		CreateNavMeshCall(PMESHDATA meshData, const gkRecast::Config& config, ASYNC_DT_RESULT result)
			: m_meshData(meshData), m_config(config), m_result(result) {}

		~CreateNavMeshCall() {}

		void run() { m_result = gkRecast::createNavMesh(m_meshData, m_config); }


	private:

		PMESHDATA m_meshData;

		gkRecast::Config m_config;

		ASYNC_DT_RESULT m_result;
	};

	result.reset();

	if (m_navMeshData.get() && m_navMeshData->hasChanged())
	{
		gkPtrRef<gkCall> call(new CreateNavMeshCall(PMESHDATA(m_navMeshData->cloneData()), config, result));

		activeObj.enqueue(call);

		m_navMeshData->resetHasChanged();

		return true;
	}

	return false;
}
#endif