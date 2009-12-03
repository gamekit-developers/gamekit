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
#include "gkSceneObjectManager.h"
#include "gkSceneObject.h"

using namespace Ogre;



GK_IMPLEMENT_SINGLETON(gkSceneObjectManager);

// ----------------------------------------------------------------------------
gkSceneObjectManager::gkSceneObjectManager()
{
	mResourceType= "Scene";
	ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
}

// ----------------------------------------------------------------------------
gkSceneObjectManager::~gkSceneObjectManager()
{
	ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
}

// ----------------------------------------------------------------------------
void gkSceneObjectManager::parseScript(DataStreamPtr& stream, const String& groupName)
{
}

// ----------------------------------------------------------------------------
gkSceneObject* gkSceneObjectManager::createScene(const String &name, const String &group, ManualResourceLoader *loader)
{
	gkSceneObjectPtr ptr= create(name, group, loader !=0, loader);
	return ptr.getPointer();
}

// ----------------------------------------------------------------------------
Resource* gkSceneObjectManager::createImpl(const String& name,
		ResourceHandle handle,
		const String& group,
		bool isManual,
		ManualResourceLoader* loader,
		const NameValuePairList* createParams)
{
	return new gkSceneObject(this, name, handle, group, isManual, loader);
}
