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
#ifndef _gkPhysicsController_h_
#define _gkPhysicsController_h_


#include "btBulletDynamicsCommon.h"
#include "BulletCollision/NarrowPhaseCollision/btManifoldPoint.h"


#include "gkSerialize.h"

class btDynamicsWorld;
class btTriangleMesh;
class gkDynamicsWorld;



struct gkContactInfo
{
	gkPhysicsController *collider;
	btManifoldPoint      point;

	typedef utArray<gkContactInfo> Array;
	typedef utArrayIterator<Array> Iterator;
};


// Base class for a physics object that gets updated along side a gkGameObject.
class gkPhysicsController
{
public:
	gkPhysicsController(gkGameObject *object, gkDynamicsWorld *owner);
	virtual ~gkPhysicsController();


    // Explicitly set the world transform.
	virtual void setTransformState(const gkTransformState &state);

	
	// Set the world transform from the parent gkGameObject
	virtual void updateTransform(void);

	bool isStaticObject(void);


	// Enable or disable physics 
	void suspend(bool v);
	bool isSuspended(void) {return m_suspend;}


	// Access to the gkGameObject physics properties.
	gkPhysicsProperties& getProperties(void);


	// Enable/Disable contact listeners.
	void enableContactProcessing(bool v);

	// Contact access.
	gkContactInfo::Array&    getContacts(void);
	gkContactInfo::Iterator  getContactIterator(void);


	// Collision tests.
	bool collidesWith(gkGameObject *ob, gkContactInfo* cpy=0);
	bool collidesWith(const gkString& name, gkContactInfo* cpy=0, bool emptyFilter=true);

	// Sensor tests, 
	// If prop is empty and material is empty, return any old collision.
	// If onlyActor is true, filter collision on actor settings (gkGameObjectProperties).
	// If testAllMaterials is true, test all assigned opposed to only testing the first assigned.
	bool sensorCollides(const gkString& prop, const gkString& material="", bool onlyActor=false, bool testAllMaterials=false);
	static bool sensorTest(gkGameObject *ob, const gkString& prop, const gkString& material="", bool onlyActor=false, bool testAllMaterials=false);


	// Userdata casts
	static gkPhysicsController* castController(btCollisionObject *colObj);
	static gkPhysicsController* castController(void *colObj);
	static gkGameObject* castObject(btCollisionObject *colObj);
	static gkGameObject* castObject(const btCollisionObject *colObj);


	btCollisionObject *getCollisionObject(void);

	btCollisionShape *getShape(void);

	btDynamicsWorld *getOwner(void);

	gkGameObject *getObject(void);


	gkBoundingBox getAabb(void) const;


	// Note: passes ownership
	void setShape(btCollisionShape *shape);


	virtual void create(void)  = 0;
	virtual void destroy(void) = 0;


	// Internal contact processing
	void _handleManifold(btPersistentManifold *manifold);

	// Internal contact processing
	void _resetContactInfo(void);


	bool _markDbvt(bool v);

protected:

	void setTransform(const btTransform &worldTrans);


	void createShape(void);


	gkContactInfo::Array m_localContacts;


	// Parent world
	gkDynamicsWorld *m_owner;


	// modifier object
	gkGameObject *m_object;

	btCollisionObject *m_collisionObject;

	btCollisionShape *m_shape;

	// Suspend all dynamics.
	bool m_suspend;

	bool m_dbvtMark;

	gkPhysicsProperties m_props;
};

#endif//_gkPhysicsController_h_
