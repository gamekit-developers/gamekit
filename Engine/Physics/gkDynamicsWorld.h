/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): Nestor Silveira.
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
#ifndef _gkDynamicsWorld_h_
#define _gkDynamicsWorld_h_


#include "gkCommon.h"
#include "gkMathUtils.h"
#include "LinearMath/btScalar.h"

class btDynamicsWorld;
class btCollisionConfiguration;
class btBroadphaseInterface;
class btDispatcher;
class btConstraintSolver;
class btRigidBody;
class btTypedConstraint;
class gkRigidBody;
class gkCharacter;
class btTriangleMesh;
class btCollisionShape;
class btGhostPairCallback;
class gkPhysicsDebug;
class gkDbvt;
class gkPhysicsConstraintProperties;

class gkDynamicsWorld
{
public:

	class Listener
	{
	public:
		virtual ~Listener() {}
		virtual void presubtick(gkScalar rate) = 0;
		virtual void subtick(gkScalar rate) = 0;
	};

	typedef utArray<Listener*> Listeners;


protected:

	gkScene*                    m_scene;
	btDynamicsWorld*            m_dynamicsWorld;
	btCollisionConfiguration*   m_collisionConfiguration;;
	btBroadphaseInterface*      m_pairCache;
	btGhostPairCallback*        m_ghostPairCallback;
	btDispatcher*               m_dispatcher;
	btConstraintSolver*         m_constraintSolver;
	gkPhysicsControllers        m_objects;
	gkPhysicsDebug*             m_debug;
	bool                        m_handleContacts;
	gkDbvt*                     m_dbvt;
	Listeners                   m_listeners;


	// drawing all but static wireframes
	void localDrawObject(gkPhysicsController* phyCon);

	void createInstanceImpl(void);
	void destroyInstanceImpl(void);

	static void substepCallback(btDynamicsWorld* dyn, btScalar tick);
	static void presubstepCallback(btDynamicsWorld *dyn, btScalar tick);

public:
	gkDynamicsWorld(const gkString& name, gkScene* scene);
	virtual ~gkDynamicsWorld();

	// Do one full physics step
	void step(gkScalar tick);
	void presubstep(gkScalar tick);
	void substep(gkScalar tick);

	void EnableContacts(bool enable) { m_handleContacts = enable; }

	btRigidBody* getFixedBody();
	btTypedConstraint* createConstraint(btRigidBody* rbA, btRigidBody* rbB, const gkPhysicsConstraintProperties& props);

	gkRigidBody* createRigidBody(gkGameObject* state);

	gkCharacter* createCharacter(gkGameObject* state);

	void destroyObject(gkPhysicsController* cont);

	GK_INLINE btDynamicsWorld* getBulletWorld(void) {GK_ASSERT(m_dynamicsWorld); return m_dynamicsWorld;}
	GK_INLINE gkScene* getScene(void)               {GK_ASSERT(m_scene); return m_scene;}

	void enableDebugPhysics(bool enable, bool debugAabb);

	void resetContacts();

	void handleDbvt(gkCamera* cam);

	gkPhysicsDebug* getDebug() const { return m_debug; }

	void DrawDebug();

	gkVariable* getDBVTInfo(void);

	void exportBullet(const gkString& fileName);
	
	void addListener(Listener *listener);
	void removeListener(Listener *listener);
};


#endif//_gkDynamicsWorld_h_
