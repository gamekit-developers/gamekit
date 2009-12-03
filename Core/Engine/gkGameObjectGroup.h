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
#ifndef _gkGameObjectGroup_h_
#define _gkGameObjectGroup_h_

#include "gkCommon.h"
#include "gkGameObject.h"





// ---------------------------------------------------------------------------
class gkGameObjectGroup;


// ---------------------------------------------------------------------------
class gkGameObjectGroupInstance
{
public:
	gkGameObjectGroupInstance(gkGameObject *owner, gkGameObjectGroup *parent, size_t handle);
	~gkGameObjectGroupInstance() {};

	gkGameObjectGroup* getParent();
	bool hasObject(const Ogre::String &name);
	void removeObject(const Ogre::String &name);
	const Ogre::String& getName();
	const size_t getHandle();
	gkGameObject* getOwner();

	const Ogre::String& getMangledName(const Ogre::String &forName);

	gkGameObject *getObject(const Ogre::String &name);
private:

	friend class gkGameObjectGroup;
	void addObject(gkGameObject *ob);

	gkGameObjectMap mRealNameToObjectMap;
	gkGameObjectGroup *mParent;
	gkGameObject *mOwner;
	const size_t mHandle;
};


// ---------------------------------------------------------------------------
class gkGameObjectGroup
{

public:
	gkGameObjectGroup(const Ogre::String &name);
	~gkGameObjectGroup();

	void addObject(gkGameObject *ob);
	bool hasObject(const Ogre::String &alias);
	gkGameObjectMapIterator getIterator();

	const Ogre::String &getName();
	void _notifyUnloaded(gkGameObject *ob);

	gkGameObjectGroupInstance* createInstance(gkGameObject *instOwner);

	gkInstanceListIterator getInstanceIterator();
	size_t getInstanceCount();

	void destroyAllInstances();

private:

	Ogre::String		  mName;
	gkGameObject*	 mOwner;
	gkGameObjectMap   mInternalObjects;
	gkInstanceList	mInstances;

	size_t		  mUniqueHandle;
};


// ---------------------------------------------------------------------------
GK_INLINE gkGameObjectGroup* gkGameObjectGroupInstance::getParent()
{
	assert(mParent);
	return mParent;
}

// ---------------------------------------------------------------------------
GK_INLINE bool gkGameObjectGroupInstance::hasObject(const Ogre::String &name)
{
	return mRealNameToObjectMap.find(name) != mRealNameToObjectMap.end();
}

// ---------------------------------------------------------------------------
GK_INLINE const Ogre::String& gkGameObjectGroupInstance::getName()
{
	assert(mParent);
	return mParent->getName();
}

// ---------------------------------------------------------------------------
GK_INLINE const size_t gkGameObjectGroupInstance::getHandle()
{
	return mHandle;
}

// ---------------------------------------------------------------------------
GK_INLINE gkGameObject *gkGameObjectGroupInstance::getOwner()
{
	assert(mOwner);
	return mOwner;
}

// ---------------------------------------------------------------------------
GK_INLINE const Ogre::String &gkGameObjectGroup::getName()
{
	return mName;
}

// ---------------------------------------------------------------------------
GK_INLINE bool gkGameObjectGroup::hasObject(const Ogre::String &alias)
{
	return mInternalObjects.find(alias) != mInternalObjects.end();
}

// ---------------------------------------------------------------------------
GK_INLINE gkGameObjectMapIterator gkGameObjectGroup::getIterator()
{
	return gkGameObjectMapIterator(mInternalObjects.begin(), mInternalObjects.end());
}

// ---------------------------------------------------------------------------
GK_INLINE gkInstanceListIterator gkGameObjectGroup::getInstanceIterator()
{
	return gkInstanceListIterator(mInstances.begin(), mInstances.end());
}

// ---------------------------------------------------------------------------
GK_INLINE size_t gkGameObjectGroup::getInstanceCount()
{
	return mInstances.size();
}






#endif//_gkGameObjectGroup_h_
