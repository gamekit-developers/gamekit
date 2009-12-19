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
#ifndef _gkSceneObject_h_
#define _gkSceneObject_h_

#include "gkCommon.h"
#include "OgreString.h"
#include "OgreSharedPtr.h"
#include "OgreResource.h"
#include "OgreResourceManager.h"

#include "gkSceneObjectTypes.h"



// ----------------------------------------------------------------------------
class gkSceneObject : public Ogre::Resource
{
public:
	gkSceneObject(Ogre::ResourceManager* creator,
				const Ogre::String& name,
				Ogre::ResourceHandle handle,
				const Ogre::String& group,
				bool isManual= false,
				Ogre::ManualResourceLoader* loader= 0);

	virtual ~gkSceneObject();

	void _loadManual(void);

	void _notifyObjectLoaded(gkGameObject *gobject);
	void _notifyObjectUnloaded(gkGameObject *gobject);
	void _notifyObjectUpdate(gkGameObject *gobject);
	void _addTempObject(gkGameObject *gobject);
	void _notifyObjectMoved(gkGameObject *gobject);

	void update(Ogre::Real tickRate, bool smooth_tick);


	Ogre::SceneManager* getManager(void);
	gkSceneProperties &getProperties(void);

	Ogre::Viewport* getViewport(void);

	void applyViewport(Ogre::Viewport *vp);
	void setMainCamera(gkCameraObject *cam);
	gkCameraObject* getMainCamera(void);

	bool hasObject(const gkHashedString& ob);
	bool hasObject(gkGameObject *ob);
	gkGameObject* getObject(const gkHashedString& name);

	gkGameObject* createObject(const Ogre::String &name, gkManualObjectLoader *loader=0);
	gkLightObject* createLight(const Ogre::String &name, gkManualObjectLoader *loader=0);
	gkCameraObject* createCamera(const Ogre::String &name, gkManualObjectLoader *loader=0);
	gkEntityObject* createEntity(const Ogre::String &name, gkManualObjectLoader *loader=0);


	// manual ogre objects (only makes sense for manual loading)
	Ogre::Mesh* createMesh(const Ogre::String &name, Ogre::ManualResourceLoader *loader);
	Ogre::Skeleton* createSkeleton(const Ogre::String &name, Ogre::ManualResourceLoader *loader);

	/// groups
	gkGameObjectGroup* createGroup(const Ogre::String &name);
	bool hasGroup(const Ogre::String &name);
	gkGameObjectGroup* getObjectGroup(const Ogre::String &name);

	void destroyGroup(const Ogre::String &name);
	void destroyGroup(gkGameObjectGroup* group);
	void destroyGroups(void);

	gkGameObjectList &getLoadedObjects(void);

protected:
	Ogre::SceneManager *mManager;
	gkCameraObject *mStartCam;

	Ogre::Viewport *mViewport;
	gkLogicManager *mLogicManager;

	gkGameObjectGroupMap mGroups;
	gkGameObjectHashMap mObjects;
	gkSceneProperties mBaseProps;

	gkGameObjectArray mTransformedObjects;
	gkGameObjectArray mUpdateObjects;

	gkGameObjectList mLoadedObjects;
	gkGameObjectList mTempObjects;

	void setShadows();

	virtual void loadImpl();
	virtual void unloadImpl();
	virtual size_t calculateSize() const;
};

// ----------------------------------------------------------------------------
GK_INLINE gkSceneProperties &gkSceneObject::getProperties()
{
	return mBaseProps;
}

// ----------------------------------------------------------------------------
GK_INLINE Ogre::Viewport* gkSceneObject::getViewport()
{
	return mViewport;
}

// ----------------------------------------------------------------------------
GK_INLINE Ogre::SceneManager* gkSceneObject::getManager()
{
	return mManager;
}

// ----------------------------------------------------------------------------
GK_INLINE gkCameraObject* gkSceneObject::getMainCamera(void)
{
	return mStartCam;
}

// ----------------------------------------------------------------------------
GK_INLINE gkGameObjectList &gkSceneObject::getLoadedObjects(void)
{
	return mLoadedObjects;
}

// ----------------------------------------------------------------------------
class gkSceneObjectPtr : public Ogre::SharedPtr<gkSceneObject>
{
public:
	gkSceneObjectPtr() : Ogre::SharedPtr<gkSceneObject>() {}
	explicit gkSceneObjectPtr(gkSceneObject* rep): Ogre::SharedPtr<gkSceneObject>(rep) {}
	gkSceneObjectPtr(const gkSceneObjectPtr& r) : Ogre::SharedPtr<gkSceneObject>(r)  {}
	gkSceneObjectPtr(const Ogre::ResourcePtr& r);
	gkSceneObjectPtr& operator=(const Ogre::ResourcePtr& r);
};





#endif//_gkSceneObject_h_
