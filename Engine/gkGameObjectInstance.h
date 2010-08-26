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

#include "gkObject.h"
#include "gkTransformState.h"
#include "gkMathUtils.h"


// This is where the object should be added / removed from the scene.
class gkGameObjectInstance : public gkObject
{
public:
	typedef utHashTable<gkHashedString, gkGameObject *>  Objects;


protected:

	const UTsize            m_id;

	// Local object not part of the scene.
	gkGameObject            *m_owner;


	// Parent group.
	gkGameObjectGroup       *m_parent;



	// List of cloned objects.
	Objects                 m_objects;


	// Reference to the scene.
	gkScene                 *m_scene;

	bool                    m_firstLoad;

	virtual void createInstanceImpl(void);
	virtual void destroyInstanceImpl(void);

	void makeTransform(void);


public:


	gkGameObjectInstance(gkGameObjectGroup *group, gkScene *scene, UTsize id);
	~gkGameObjectInstance();


	// object access
	gkGameObject *getObject(const gkHashedString &name);

	// Removes cloned object.
	void          destroyObject(const gkHashedString &name);
	void          destroyObject(gkGameObject *gobj);

	// Creates a carbon copy.
	void          addObject(gkGameObject *gobj);


	// Existence testing.
	bool          hasObject(const gkHashedString &name);
	bool          hasObject(gkGameObject *gobj);


	void          applyTransform(const gkTransformState &trans);


	// Clones all objects in this instance.
	void cloneObjects(const gkTransformState &from, int time,
	                  const gkVector3 &linearVelocity=gkVector3::ZERO, bool tsLinLocal = true,
	                  const gkVector3 &angularVelocity=gkVector3::ZERO, bool tsAngLocal = true);


	GK_INLINE gkGameObject          *getOwner(void)     {return m_owner;}
	GK_INLINE gkGameObjectGroup     *getGroup(void)     {return m_parent;}
	GK_INLINE Objects               &getObjects(void)   {return m_objects; }
	GK_INLINE const UTsize          getId(void)         {return m_id;}
};

#endif //_gkGameObjectInstance_h_
