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
#ifndef _gkGroupManager_h_
#define _gkGroupManager_h_

#include "gkCommon.h"
#include "gkResourceManager.h"
#include "utSingleton.h"



class gkGroupManager : public gkResourceManager, public utSingleton<gkGroupManager>
{
public:
	UT_DECLARE_SINGLETON(gkGroupManager);

	typedef utArray<gkGameObjectGroup*> Groups;
	typedef utHashTable<utPointerHashKey, Groups> GroupAttachements;


public:
	gkGroupManager();
	virtual ~gkGroupManager();


	///Place all of gkGameObjectGroup's instances in the Ogre scene
	void createGameObjectInstances(gkScene* scene);

	///Remove all of gkGameObjectGroup's instances in the Ogre scene
	void destroyGameObjectInstances(gkScene* scene);


	void createStaticBatches(gkScene* scene);
	void destroyStaticBatches(gkScene* scene);


	void attachGroupToScene(gkScene* sc, gkGameObjectGroup* group);
	Groups::Iterator getAttachedGroupIterator(gkScene* sc);

private:

	gkResource* createImpl(const gkResourceName& name, const gkResourceHandle& handle);

	gkResourceManager::ResourceListener* m_sceneListener;


	GroupAttachements m_attachements;

	void notifyDestroyAllImpl(void);


	virtual void notifyResourceDestroyedImpl(gkResource* res);

	friend class gkGameObjectGroup;
	UTsize _getHandle(void) {return m_handles++;}

	UTsize m_handles;

};


#endif//_gkGroupManager_h_
