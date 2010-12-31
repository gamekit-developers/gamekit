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
#ifndef _gkGameObjectInstance_h_
#define _gkGameObjectInstance_h_

#include "gkGameObject.h"
#include "gkTransformState.h"
#include "gkMathUtils.h"


class gkGameObjectInstance : public gkInstancedObject, public gkGameObject::Notifier
{
public:
	typedef utHashTable<gkHashedString, gkGameObject*>  Objects;


protected:
	Objects                 m_objects;
	gkTransformState        m_transform;


	gkGameObjectGroup*      m_parent;
	gkScene*                m_scene;
	gkGameObject*           m_owner;
	const gkString          m_uidName;


	void notifyGameObjectEvent(gkGameObject* gobj, const gkGameObject::Notifier::Event& id);

	void createInstanceImpl(void);
	void destroyInstanceImpl(void);
	void postCreateInstanceImpl(void);

public:


	gkGameObjectInstance(gkInstancedManager* creator, const gkResourceName& name, const gkResourceHandle& handle);
	virtual ~gkGameObjectInstance();


	gkGameObject* getObject(const gkHashedString& name);

	void          destroyObject(const gkHashedString& name);
	void          destroyObject(gkGameObject* gobj);


	///addObject will create a clone of this object, then add it the main list.
	void          addObject(gkGameObject* gobj);



	bool          hasObject(const gkHashedString& name);
	bool          hasObject(gkGameObject* gobj);


	///Applies a transform to all objects in this instance.
	void          applyTransform(const gkTransformState& trans);



	void cloneObjects(gkScene* scene, const gkTransformState& from, int time,
	                  const gkVector3& linearVelocity = gkVector3::ZERO,
	                  bool tsLinLocal = true,
	                  const gkVector3& angularVelocity = gkVector3::ZERO,
	                  bool tsAngLocal = true);

	void _updateFromGroup(gkGameObjectGroup* group);


	GK_INLINE gkTransformState&      getOwnerTransform(void)  {return m_transform;}
	GK_INLINE gkGameObjectGroup*     getGroup(void)           {return m_parent;}
	GK_INLINE Objects&               getObjects(void)         {return m_objects; }
	GK_INLINE gkGameObject*          getRoot(void)            {return m_owner;}
};

#endif //_gkGameObjectInstance_h_
