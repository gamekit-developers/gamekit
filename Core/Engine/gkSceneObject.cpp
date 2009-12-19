/*
-------------------------------------------------------------------------------
	This file is part of the Ogre GameKit port.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#include "OgreMeshManager.h"
#include "OgreSkeletonManager.h"
#include "OgreMesh.h"
#include "OgreSkeleton.h"

#include "gkWindowSystem.h"
#include "gkGameObject.h"
#include "gkGameObjectGroup.h"
#include "gkEngine.h"
#include "gkSceneObject.h"
#include "OgreException.h"
#include "gkLogicManager.h"
#include "gkConsole.h"
#include "gkLightObject.h"
#include "gkCameraObject.h"
#include "gkEntityObject.h"
#include "gkLogger.h"


using namespace Ogre;



// ----------------------------------------------------------------------------
gkSceneObjectPtr::gkSceneObjectPtr(const ResourcePtr& r): SharedPtr<gkSceneObject>()
{
	// lock & copy other mutex pointer
	OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME)
	{
		OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
		OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
		pRep= static_cast<gkSceneObject*>(r.getPointer());
		pUseCount= r.useCountPointer();

		if (pUseCount)
		{
			++(*pUseCount);
		}
	}
}

// ----------------------------------------------------------------------------
gkSceneObjectPtr& gkSceneObjectPtr::operator=(const ResourcePtr & r)
{
	if (pRep == static_cast<gkSceneObject*>(r.getPointer()))
		return *this;

	release();

	// lock & copy other mutex pointer
	OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME)
	{
		OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
		OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
		pRep= static_cast<gkSceneObject*>(r.getPointer());
		pUseCount= r.useCountPointer();

		if (pUseCount)
		{
			++(*pUseCount);
		}
	}
	else
	{
		// RHS must be a null pointer
		assert(r.isNull() && "RHS must be null if it has no mutex!");
		setNull();
	}
	return *this;
}

// ----------------------------------------------------------------------------
gkSceneObject::gkSceneObject(ResourceManager* creator,
						 const String& name,
						 ResourceHandle handle,
						 const String& group,
						 bool isManual,
						 ManualResourceLoader* loader):
		Resource(creator, name, handle, group, isManual, loader),
		mManager(0), mStartCam(0), mViewport(0), mLogicManager(0),
		mBaseProps()
{
}

// ----------------------------------------------------------------------------
gkSceneObject::~gkSceneObject()
{
	destroyGroups();

	GameObjectHashMapIterator it(mObjects);
	while (it.hasMoreElements())
	{
		GameObjectHashMapIterator::Pair p= it.getNext();
		delete p.second;
	}
}


// ----------------------------------------------------------------------------
gkGameObjectGroup* gkSceneObject::createGroup(const String &name)
{
	gkGameObjectGroupMap::iterator it= mGroups.find(name);
	if (it != mGroups.end())
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE,
					String("duplicate group ") + name,
					"GameObjectManager::createGroup");
	}

	gkGameObjectGroup *group= new gkGameObjectGroup(name);
	mGroups.insert(std::make_pair(name, group));
	return group;
}

// ----------------------------------------------------------------------------
bool gkSceneObject::hasGroup(const String &name)
{
	return mGroups.find(name) != mGroups.end();
}


// ----------------------------------------------------------------------------
void gkSceneObject::destroyGroup(const String &name)
{
	gkGameObjectGroupMap::iterator it= mGroups.find(name);
	if (it != mGroups.end())
	{
		delete it->second;
		mGroups.erase(it);
	}

	if (mGroups.empty())
		mGroups.clear();
}

// ----------------------------------------------------------------------------
void gkSceneObject::destroyGroup(gkGameObjectGroup* group)
{
	destroyGroup(group->getName());
}

// ----------------------------------------------------------------------------
gkGameObjectGroup* gkSceneObject::getObjectGroup(const String &name)
{
	gkGameObjectGroupMap::iterator it= mGroups.find(name);
	if (it == mGroups.end())
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
					String("no group found by the name ") + name,
					"GameObjectManager::getObjectGroup");
	}

	return it->second;
}

// ----------------------------------------------------------------------------
void gkSceneObject::destroyGroups()
{
	gkGameObjectGroupMap::iterator it= mGroups.begin();
	while (it != mGroups.end())
	{
		delete it->second;
		++it;
	}

	mGroups.clear();
}


// ----------------------------------------------------------------------------
void gkSceneObject::setShadows()
{
	if (!mManager)
		return;
}

// ----------------------------------------------------------------------------
bool gkSceneObject::hasObject(const gkHashedString& ob)
{
	return mObjects.find(ob) != GK_NPOS;
}

// ----------------------------------------------------------------------------
bool gkSceneObject::hasObject(gkGameObject *ob)
{
	GK_ASSERT(ob);
	return mObjects.find(ob->getHashedName()) != GK_NPOS;
}

// ----------------------------------------------------------------------------
gkGameObject* gkSceneObject::getObject(const gkHashedString& name)
{
	size_t pos= mObjects.find(name);
	if (pos != GK_NPOS)
		return mObjects.at(pos);
	return 0;
}

// ----------------------------------------------------------------------------
gkGameObject* gkSceneObject::createObject(const String &name,
									  gkManualObjectLoader *loader)
{
	if (mObjects.find(name) != GK_NPOS)
	{
		OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM,
					"duplicate object found",
					"gkSceneObject::createObject");
	}

	gkHashedString nameHash= name;
	gkGameObject *ob= new gkGameObject(this, name, GK_OBJECT, loader);

	mObjects.insert(nameHash, ob);
	return ob;
}

// ----------------------------------------------------------------------------
gkLightObject* gkSceneObject::createLight(const String &name,
									  gkManualObjectLoader *loader)
{
	if (mObjects.find(name) != GK_NPOS)
	{
		OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM,
					"duplicate object found",
					"gkSceneObject::createLight");
	}

	gkHashedString nameHash= name;
	gkLightObject *ob= new gkLightObject(this, name, loader);

	mObjects.insert(nameHash, ob);
	return ob;
}

// ----------------------------------------------------------------------------
gkCameraObject* gkSceneObject::createCamera(const String &name,
										gkManualObjectLoader *loader)
{
	if (mObjects.find(name) != GK_NPOS)
	{
		OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM,
					"duplicate object found",
					"gkSceneObject::createCamera");
	}

	gkHashedString nameHash= name;
	gkCameraObject *ob= new gkCameraObject(this, name, loader);

	mObjects.insert(nameHash, ob);
	return ob;
}

// ----------------------------------------------------------------------------
gkEntityObject* gkSceneObject::createEntity(const String &name,
										gkManualObjectLoader *loader)
{
	if (mObjects.find(name) != GK_NPOS)
	{
		OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM,
					"duplicate object found",
					"gkSceneObject::createEntity");
	}
	gkHashedString nameHash= name;
	gkEntityObject *ob= new gkEntityObject(this, name, loader);

	mObjects.insert(nameHash, ob);
	return ob;
}

// ----------------------------------------------------------------------------
Mesh* gkSceneObject::createMesh(const String &name,
							  ManualResourceLoader *loader)
{
	MeshPtr ob= MeshManager::getSingleton().create(name, mGroup, loader!=0, loader);
	if (ob.isNull())
	{
		OGRE_EXCEPT(Exception::ERR_INVALID_STATE,
					"failed to create object",
					"gkSceneObject::createMesh");
	}
	return ob.getPointer();
}

// ----------------------------------------------------------------------------
Skeleton* gkSceneObject::createSkeleton(const String &name, ManualResourceLoader *loader)
{
	SkeletonPtr ob= SkeletonManager::getSingleton().create(name, mGroup, loader!=0, loader);
	if (ob.isNull())
	{
		OGRE_EXCEPT(Exception::ERR_INVALID_STATE,
					"failed to create object",
					"gkSceneObject::createSkeleton");
	}
	return ob.getPointer();
}

// ----------------------------------------------------------------------------
void gkSceneObject::applyViewport(Viewport *vp)
{
	const gkRectangle& rect= gkWindowSystem::getSingleton().getArea();
	vp->setDimensions(0, 0, 1, 1);
	Camera *c= vp->getCamera();
	if (c != 0)
		c->setAspectRatio(rect.width / rect.height);
}

// ----------------------------------------------------------------------------
void gkSceneObject::setMainCamera(gkCameraObject *cam)
{
	if (!mStartCam)
		return;

	mStartCam= cam;
	Camera* main= mStartCam->getCamera();

	if (main == 0)
		return;

	if (!mViewport)
	{
		// create a new viewport
		RenderWindow *window= gkWindowSystem::getSingleton().getWindow();
		if (window)
			mViewport= window->addViewport(main);
	}
	else
		mViewport->setCamera(main);

	GK_ASSERT(mViewport);
	applyViewport(mViewport);
}

// ----------------------------------------------------------------------------
void gkSceneObject::loadImpl()
{
	if (mObjects.empty())
	{
		OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
					String("no objects in scene '") + mName + "'",
					"gkSceneObject::loadImpl");
	}

	mManager= Root::getSingleton().createSceneManager(ST_GENERIC, mName);
	mManager->setAmbientLight(mBaseProps.ambient);

	// game logic manager
	mLogicManager= gkLogicManager::getSingletonPtr();

	GameObjectHashMapIterator it(mObjects);
	while (it.hasMoreElements())
	{
		GameObjectHashMapIterator::Pair p= it.getNext();
		gkGameObject* obptr= p.second;

		if (!obptr->isLoaded() && obptr->isInActiveLayer())
		{
			// if there is one camera use it incase
			if (!mStartCam && obptr->getType() == GK_CAMERA)
				mStartCam= (gkCameraObject*)obptr;

			obptr->load();
		}
	}

	if (!mViewport)
	{
		// setMainCamera was not called, try to add one
		if (mStartCam)
			setMainCamera(mStartCam);
		else
		{
			mStartCam = createCamera(" -- No Camera -- ");
			mStartCam->getProperties().position		= Ogre::Vector3(0, -5, 0);
			mStartCam->getProperties().orientation	= gkMathUtils::getQuatFromEuler(Ogre::Vector3(90, 0, 0));
			mStartCam->load();
			setMainCamera(mStartCam);
		}
	}

	GK_ASSERT(mViewport);
	mViewport->setBackgroundColour(mBaseProps.world_color);
	gkEngine::getSingleton().setActiveScene(this);
}


// ----------------------------------------------------------------------------
void gkSceneObject::unloadImpl()
{
	if (mObjects.empty())
		return;

	GameObjectHashMapIterator it(mObjects);
	while (it.hasMoreElements())
	{
		GameObjectHashMapIterator::Pair p= it.getNext();
		gkGameObject* obptr= p.second;

		if (obptr->isLoaded())
			obptr->unload();
	}

	mLoadedObjects.clear();
	mUpdateObjects.clear();

	/// clear ogre scene manager
	if (mManager)
		Root::getSingleton().destroySceneManager(mManager);

	if (mLogicManager != 0)
	{
		// destroy all scene trees
		mLogicManager->clear();
	}

	// finalize vp
	if (mViewport)
	{
		RenderWindow *window= gkWindowSystem::getSingleton().getWindow();
		if (window)
		{
			window->removeViewport(0);
			mViewport= 0;
		}
	}

	mLogicManager= 0;
	gkEngine::getSingleton().setActiveScene(0);
}

// ----------------------------------------------------------------------------
size_t gkSceneObject::calculateSize() const
{
	return 0;
}

// ----------------------------------------------------------------------------
void gkSceneObject::_loadManual()
{
	// hook for manual loaders
	loadImpl();
}

// ----------------------------------------------------------------------------
void gkSceneObject::_notifyObjectLoaded(gkGameObject *gobject)
{
	mLoadedObjects.push_back(gobject);
}

// ----------------------------------------------------------------------------
void gkSceneObject::_notifyObjectUnloaded(gkGameObject *gobject)
{
	gkGameObjectList::iterator it= std::find(mLoadedObjects.begin(), mLoadedObjects.end(), gobject);
	if (it != mLoadedObjects.end())
		mLoadedObjects.erase(it);

	if (mLoadedObjects.empty())
		mLoadedObjects.clear();
}

// ----------------------------------------------------------------------------
void gkSceneObject::_addTempObject(gkGameObject *gobject)
{
	gkGameObjectList::iterator it= std::find(mTempObjects.begin(), mTempObjects.end(), gobject);
	if (it == mTempObjects.end())
		mTempObjects.push_back(gobject);
}

// ----------------------------------------------------------------------------
void gkSceneObject::_notifyObjectUpdate(gkGameObject *gobject)
{
	if (mUpdateObjects.find(gobject) == GK_NPOS)
		mUpdateObjects.push_back(gobject);
}

// ----------------------------------------------------------------------------
void gkSceneObject::_notifyObjectMoved(gkGameObject *gobject)
{
	if (mTransformedObjects.find(gobject) == GK_NPOS)
		mTransformedObjects.push_back(gobject);
}

// ----------------------------------------------------------------------------
void gkSceneObject::update(Real tickRate, bool smooth_tick)
{
	if (!isLoaded())
		return;

	if (!smooth_tick)
	{
		// remove all transformed objects
		mTransformedObjects.resize(0);

		// update logic trees
		if (mLogicManager != 0)
			mLogicManager->update(tickRate);

		// apply constraints
		if (!mUpdateObjects.empty())
		{
			gkGameObjectArrayIterator con(mUpdateObjects);
			while (con.hasMoreElements())
				con.getNext()->update();
		}
	}
	else
	{
		if (!mTransformedObjects.empty())
		{
			// synchronize motion states
			tickRate= gkClamp(tickRate, 0.0, 1.0);

			gkGameObjectArrayIterator sync(mTransformedObjects);
			while (sync.hasMoreElements())
				sync.getNext()->synchronizeMotion(tickRate);
		}
	}
}
