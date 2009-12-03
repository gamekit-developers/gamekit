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
#include "gkGameObjectGroup.h"

using namespace Ogre;



// ----------------------------------------------------------------------------
gkGameObjectGroupInstance::gkGameObjectGroupInstance(gkGameObject *owner, gkGameObjectGroup *parent, size_t handle) :
		mParent(parent), mOwner(owner), mHandle(handle)
{
}


// ----------------------------------------------------------------------------
gkGameObject* gkGameObjectGroupInstance::getObject(const String &name)
{
	gkGameObjectMap::iterator iter= mRealNameToObjectMap.find(name);
	if (iter == mRealNameToObjectMap.end())
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE,
					String("instance has no object '") + name + "'",
					"gkGameObjectGroupInstance::getObject");
	}

	return iter->second;
}

// ----------------------------------------------------------------------------
const String& gkGameObjectGroupInstance::getMangledName(const String &forName)
{
	if (hasObject(forName))
		return mRealNameToObjectMap[forName]->getName();
	return StringUtil::BLANK;
}


// ----------------------------------------------------------------------------
void gkGameObjectGroupInstance::addObject(gkGameObject *ob)
{
	// create a new instance of ob
	String map_name= ob->getName();

	if (hasObject(map_name))
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM,
					String("duplicate instance of object '") + map_name + "'",
					"gkGameObjectGroupInstance::addObject");
	}

	/* mangle */
	String new_name= ob->getName() + "::" +
					 StringConverter::toString(mHandle) + "::" +
					 mParent->getName();


	gkGameObject *newOb= ob->duplicate(new_name);
	newOb->_attachToGroupInstance(this);

	assert(newOb);
	mRealNameToObjectMap.insert(std::make_pair(map_name, newOb));
}


// ----------------------------------------------------------------------------
void gkGameObjectGroupInstance::removeObject(const String &name)
{
	gkGameObjectMap::iterator iter= mRealNameToObjectMap.find(name);
	if (iter != mRealNameToObjectMap.end())
	{
		// we are responsible for this object
		// so kill all references

		gkGameObject *ob= iter->second;
		ob->unload();

		// FIX THIS
		// GameObjectManager::getSingleton().remove(ob->getHandle());

		mRealNameToObjectMap.erase(iter);
		if (mRealNameToObjectMap.empty())
			mRealNameToObjectMap.clear();
	}
}

// ----------------------------------------------------------------------------
gkGameObjectGroup::gkGameObjectGroup(const String &name) :
		mName(name), mInstances(0), mUniqueHandle(0)
{
}

// ----------------------------------------------------------------------------
gkGameObjectGroup::~gkGameObjectGroup()
{
	destroyAllInstances();
}

// ----------------------------------------------------------------------------
void gkGameObjectGroup::addObject(gkGameObject *ob)
{
	if (!ob)
		return;

	gkGameObjectMap::iterator it= mInternalObjects.find(ob->getName());
	if (it != mInternalObjects.end())
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE,
					String("duplicate object ") + "'" + ob->getName()
					+ "'" + String("within group context ") + mName,
					"gkGameObjectGroup::addObject");
	}

	ob->_attachToGroup(this);
	mInternalObjects.insert(std::make_pair(ob->getName(), ob));
}

// ----------------------------------------------------------------------------
void gkGameObjectGroup::destroyAllInstances()
{
	gkInstanceListIterator iter= getInstanceIterator();
	while (iter.hasMoreElements())
	{
		gkGameObjectGroupInstance *inst= iter.getNext();
		delete inst;
	}
	mInstances.clear();
}

// ----------------------------------------------------------------------------
void gkGameObjectGroup::_notifyUnloaded(gkGameObject *ob)
{
	gkGameObjectMap::iterator it= mInternalObjects.begin();
	while (it != mInternalObjects.end())
	{
		if (it->second == ob)
		{
			mInternalObjects.erase(it);

			gkInstanceListIterator iter= getInstanceIterator();
			while (iter.hasMoreElements())
			{
				gkGameObjectGroupInstance *inst= iter.getNext();

				if (inst->hasObject(ob->getName()))
					inst->removeObject(ob->getName());
			}

			/* done */
			return;
		}
		++it;
	}
}

// ----------------------------------------------------------------------------
gkGameObjectGroupInstance* gkGameObjectGroup::createInstance(gkGameObject *instOwner)
{
	gkGameObjectGroupInstance *newInst= new gkGameObjectGroupInstance(instOwner, this, mUniqueHandle++);
	mInstances.push_back(newInst);

	gkGameObjectMap::iterator it= mInternalObjects.begin();
	while (it != mInternalObjects.end())
	{
		newInst->addObject(it->second);
		++it;
	}
	return newInst;
}
