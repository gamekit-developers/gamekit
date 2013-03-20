/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Charlie C.

    Contributor(s): Thomas Trocha(dertom)
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
#include "gkGameObjectGroup.h"


class gkGameObjectInstance : public gkInstancedObject, public gkGameObject::Notifier
{
public:
	typedef utHashTable<gkHashedString, gkGameObject*>  Objects;
	typedef utHashTable<gkHashedString, gkGameObjectInstance*> GroupInstances;

	typedef utHashTable<utPointerHashKey, gkTransformState> InitialTransformstates;

protected:
	Objects                 m_objects;
	GroupInstances			m_groupInstances;
	gkTransformState        m_transform;
	InitialTransformstates  m_objInitialTransformstates;

	gkGameObjectGroup*      m_parent;
	gkScene*                m_scene;
	gkGameObject*           m_owner;
	const gkString          m_uidName;
	UTuint32				m_layer;


	void notifyGameObjectEvent(gkGameObject* gobj, const gkGameObject::Notifier::Event& id);

	void createInstanceImpl(void);
	void postCreateInstanceImpl(void);
	void destroyInstanceImpl(void);
	void postDestroyInstanceImpl(void);

public:


	gkGameObjectInstance(gkInstancedManager* creator, const gkResourceName& name, const gkResourceHandle& handle);
	virtual ~gkGameObjectInstance();


	gkGameObject* getObject(const gkHashedString& name);

	void          destroyObject(const gkHashedString& name);
	void          destroyObject(gkGameObject* gobj);


	///addObject will create a clone of this object, then add it the main list.
	void          addObject(gkGameObject* gobj);
	void		  addGroupInstance(const gkGameObjectGroup::GroupInstance* inst);


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
	void _setExternalRoot(gkGameObjectGroup* group, gkGameObject* root);

	GK_INLINE gkTransformState&      getOwnerTransform(void)  {return m_transform;}
	GK_INLINE gkGameObjectGroup*     getGroup(void)           {return m_parent;}
	GK_INLINE Objects&               getObjects(void)         {return m_objects; }
	GK_INLINE gkGameObject*          getRoot(void)            {return m_owner;}
	GK_INLINE void			         setLayer(UTuint32 layer) {m_layer = layer;}
	GK_INLINE UTuint32		         getLayer(void)            {return m_layer;}
};

#endif //_gkGameObjectInstance_h_
