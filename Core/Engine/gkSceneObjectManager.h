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
#ifndef _gkSceneObjectManager_h_
#define _gkSceneObjectManager_h_

#include "gkCommon.h"
#include "OgreResource.h"
#include "OgreResourceManager.h"
#include "OgreSingleton.h"




// ----------------------------------------------------------------------------
class gkSceneObjectManager :  public Ogre::ResourceManager, public Ogre::Singleton<gkSceneObjectManager>
{
public:
	gkSceneObjectManager();
	virtual ~gkSceneObjectManager();

	void parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName);

	static gkSceneObjectManager& getSingleton(void);
	static gkSceneObjectManager* getSingletonPtr(void);

	gkSceneObject* createScene(const Ogre::String &name,
							 const Ogre::String &group= GK_DEF_GROUP,
							 Ogre::ManualResourceLoader *loader=0);

protected:

	virtual Ogre::Resource* createImpl(
		const Ogre::String& name,
		Ogre::ResourceHandle handle,
		const Ogre::String& group,
		bool isManual,
		Ogre::ManualResourceLoader* loader,
		const Ogre::NameValuePairList* createParams
	);
};




#endif//_gkSceneObjectManager_h_
