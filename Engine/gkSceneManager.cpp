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
#include "gkSceneManager.h"
#include "gkScene.h"
#include "gkResourceGroupManager.h"
#include "gkGameObject.h"
#include "gkGroupManager.h"
#include "gkUtils.h"
#include "gkEngine.h"
#include "gkUserDefs.h"


gkSceneManager::gkSceneManager()
	:    gkInstancedManager("SceneManager", "Scene")
{
}


gkSceneManager::~gkSceneManager()
{
	destroyAllInstances();
	destroyAll();
}



gkResource* gkSceneManager::createImpl(const gkResourceName& name, const gkResourceHandle& handle)
{
	gkResourceGroupManager::getSingleton().createResourceGroup(name.getGroup(), false);
	return new gkScene(this, name, handle);
}


void gkSceneManager::notifyResourceDestroyedImpl(gkResource* res)
{
	GK_ASSERT(res);
	static_cast<gkScene*>(res)->destroyInstance();
}


void gkSceneManager::copyObjects(gkScene* fromScene, gkScene* toScene, int exceptObjectTypes)
{
	GK_ASSERT(fromScene && toScene);

	gkGameObjectSet objects; 
	
	gkGameObjectHashMap::Iterator it = fromScene->getObjects();
	while (it.hasMoreElements())
	{
		gkGameObject* obj = it.getNext().second;
		gkGameObjectTypes type = obj->getType();

		if ((type == GK_CAMERA    && (exceptObjectTypes & OBJ_CAMERA))   ||
			(type == GK_LIGHT     && (exceptObjectTypes & OBJ_LIGHT))    ||
			(type == GK_ENTITY    && (exceptObjectTypes & OBJ_ENTITY))   ||
			(type == GK_OBJECT    && (exceptObjectTypes & OBJ_OBJECT))   ||
			(type == GK_SKELETON  && (exceptObjectTypes & OBJ_SKELETON)) ||
			(type == GK_PARTICLES && (exceptObjectTypes & OBJ_PARTICLES)) )
			continue;

		if (obj->isInActiveLayer())
		{
			toScene->addObject(obj);
			objects.insert(obj);
		}
	}

	gkGroupManager& groups = gkGroupManager::getSingleton();

	gkGroupManager::Groups::Iterator giter = groups.getAttachedGroupIterator(fromScene);
	while (giter.hasMoreElements())
	{
		gkGameObjectGroup* g = giter.getNext();
		if (g->getGroupName() == fromScene->getGroupName())
			groups.attachGroupToScene(toScene, g);
	}

	if (toScene->isInstanced())
	{
		gkGroupManager::getSingleton().createGameObjectInstances(toScene);

		if (gkEngine::getSingleton().getUserDefs().buildStaticGeometry)
			gkGroupManager::getSingleton().createStaticBatches(toScene);

		toScene->_applyBuiltinParents(objects);
		toScene->_applyBuiltinPhysics(objects);
	}
}

gkScene* gkSceneManager::createEmptyScene(const gkString& sceneName, const gkString& cameraName, const gkString& group)
{
	gkString groupName = group.empty() ? gkUtils::getUniqueName("EMPTY") : group;

	gkScene* scene = static_cast<gkScene*>(create(gkResourceName(sceneName, groupName))); GK_ASSERT(scene);

	
	if (scene)
	{
		gkCamera* camera = scene->createCamera(cameraName); GK_ASSERT(camera);
	}

	return scene;
		
}



UT_IMPLEMENT_SINGLETON(gkSceneManager);
