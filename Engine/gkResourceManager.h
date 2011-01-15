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
#ifndef _gkResourceManager_h_
#define _gkResourceManager_h_

#include "gkCommon.h"


class gkResourceManager
{
public:

	typedef utHashTable<utIntHashKey, gkResource*> Resources;
	typedef Resources::Iterator ResourceIterator;



	///Resource managment events
	class ResourceListener
	{
	public:
		virtual ~ResourceListener() {}

		virtual void notifyResourceCreated(gkResource* res) {}
		virtual void notifyResourceDestroyed(gkResource* res) {}

	};

	typedef utArray<ResourceListener*> Listeners;

public:

	gkResourceManager(const gkString& type, const gkString& rtype);
	virtual ~gkResourceManager();

	gkResource* getByHandle(const gkResourceHandle& handle);
	gkResource* getByName(const gkResourceName& name);
	gkResource* create(const gkResourceName& name);



	template<typename T> GK_INLINE T* getByHandle(const gkResourceHandle& handle) {return static_cast<T*>(getByHandle(handle));}
	template<typename T> GK_INLINE T* getByName(const gkResourceName& name)       {return static_cast<T*>(getByName(name));}
	template<typename T> GK_INLINE T* create(const gkResourceName& name)          {return static_cast<T*>(create(name));}



	void destroy(const gkResourceHandle& handle);
	void destroy(const gkResourceName& name);
	void destroy(gkResource* ob);
	void destroyGroup(const gkResourceNameString& group);
	void destroyAll(void);

	bool exists(const gkResourceName& name);
	bool exists(const gkResourceHandle& handle);


	GK_INLINE ResourceIterator getResourceIterator(void) {return m_resources.iterator();}
	GK_INLINE Resources& getResources(void) {return m_resources;}


	void addResourceListener(ResourceListener* res);
	void removeResourceListener(ResourceListener* res);

	GK_INLINE const gkString& getResourceType(void) {return m_resourceType;}
	GK_INLINE const gkString& getManagerType(void)  {return m_managerType;}

	GK_INLINE UTsize getResourceCount(void) {return m_resources.size();}
	GK_INLINE UTsize getListenerCount(void) {return m_listeners.size();}
protected:

	void notifyResourceCreated(gkResource* res);
	void notifyResourceDestroyed(gkResource* res);

	virtual gkResource* createImpl(const gkResourceName& name, const gkResourceHandle& handle) = 0;

	virtual void notifyDestroyAllInstancesImpl(void) {}
	virtual void notifyDestroyAllImpl(void) {}

	virtual void notifyDestroyGroupInstancesImpl(const gkResourceNameString& group) {}
	virtual void notifyDestroyGroupImpl(const gkResourceNameString& group) {}

	virtual void notifyResourceCreatedImpl(gkResource* res) {}
	virtual void notifyResourceDestroyedImpl(gkResource* res) {}

	Resources m_resources;
	Listeners m_listeners;
	gkString  m_managerType, m_resourceType;


private:
	gkResourceHandle m_resourceHandles;
};


#endif//_gkResourceManager_h_
