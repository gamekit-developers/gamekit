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
#ifndef _gkInstancedManager_h_
#define _gkInstancedManager_h_

#include "gkResourceManager.h"

class gkInstancedManager : public gkResourceManager
{
public:

	///Resource managment events
	class InstancedListener
	{
	public:
		virtual ~InstancedListener() {}

		virtual void notifyInstanceCreated(gkInstancedObject* inst) {}
		virtual void notifyInstanceDestroyed(gkInstancedObject* inst) {}

	};

	typedef utArray<InstancedListener*> InstanceListeners;
	typedef utHashSet<gkInstancedObject*> Instances;



	// Do user create commands outside, logic / physics systems
	struct InstanceParam
	{
		enum Type
		{
			REINSTANCE,
			CREATE,
			DESTROY
		};

		gkInstancedObject* first;
		Type second;

		GK_INLINE bool operator == (const InstanceParam& cmd) const
		{ return first == cmd.first && second == cmd.second; }
	};

	typedef utArray<InstanceParam> InstanceParams;

public:

	gkInstancedManager(const gkString& type, const gkString& rtype);
	~gkInstancedManager();

	void addCreateInstanceQueue(gkInstancedObject* iobj);
	void addDestroyInstanceQueue(gkInstancedObject* iobj);
	void addReInstanceQueue(gkInstancedObject* iobj);
	void postProcessQueue(void);

	void destroyGroupInstances(const gkString& group);
	void destroyAllInstances(void);

	void notifyInstanceCreated(gkInstancedObject* iobj);
	void notifyInstanceDestroyed(gkInstancedObject* iobj);


	void addInstanceListener(InstancedListener* li);
	void removeInstanceListener(InstancedListener* li);


	Instances& getInstances(void)                  {return m_instances; }
	Instances::Iterator getInstanceIterator(void)  {return m_instances.iterator(); }

	bool hasInstances(void) { return !m_instances.empty(); }

protected:

	InstanceParams m_instanceQueue;

	Instances m_instances;
	InstanceListeners m_instanceListeners;

	virtual void notifyInstanceCreatedImpl(gkInstancedObject* iobj) {}
	virtual void notifyInstanceDestroyedImpl(gkInstancedObject* iobj) {}

	virtual void notifyDestroyAllInstancesImpl(void);

};

#endif//_gkInstancedManager_h_
