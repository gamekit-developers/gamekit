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
#include "gkCharacter.h"
#include "gkDynamicsWorld.h"
#include "gkRigidBody.h"
#include "gkEngine.h"
#include "gkUserDefs.h"
#include "gkPhysicsDebug.h"
#include "gkGameObject.h"
#include "gkScene.h"
#include "gkCamera.h"
#include "gkVariable.h"
#include "gkDbvt.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"





gkDynamicsWorld::gkDynamicsWorld(const gkString &name, gkScene *scene)
	:       m_scene(scene),
	        m_dynamicsWorld(0), 
			m_collisionConfiguration(0),
	        m_pairCache(0), 
			m_ghostPairCallback(0), 
			m_dispatcher(0), 
			m_constraintSolver(0),
	        m_debug(0), 
			m_handleContacts(true), 
			m_dbvt(0)
{
	createInstanceImpl();
}



gkDynamicsWorld::~gkDynamicsWorld()
{
	destroyInstanceImpl();
}



void gkDynamicsWorld::createInstanceImpl(void)
{
	// prepare world

	if (m_dynamicsWorld)
		return;

	m_collisionConfiguration = new btDefaultCollisionConfiguration();

	m_pairCache = new btDbvtBroadphase();

	m_ghostPairCallback = new btGhostPairCallback();
	m_pairCache->getOverlappingPairCache()->setInternalGhostPairCallback(m_ghostPairCallback);

	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
	m_constraintSolver = new btSequentialImpulseConstraintSolver();
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_pairCache, m_constraintSolver, m_collisionConfiguration);

	gkVector3 &grav = m_scene->getProperties().m_gravity;
	m_dynamicsWorld->setGravity(btVector3(grav.x, grav.y, grav.z));
	m_dynamicsWorld->setWorldUserInfo(this);
	m_dynamicsWorld->setInternalTickCallback(substepCallback, static_cast<void *>(this));

	enableDebugPhysics(gkEngine::getSingleton().getUserDefs().debugPhysics, gkEngine::getSingleton().getUserDefs().debugPhysicsAabb);

	if (gkEngine::getSingleton().getUserDefs().useBulletDbvt)
		m_dbvt = new gkDbvt();
}



void gkDynamicsWorld::destroyInstanceImpl(void)
{
	delete m_dynamicsWorld;
	m_dynamicsWorld = 0;

	delete m_constraintSolver;
	m_constraintSolver = 0;

	delete m_dispatcher;
	m_dispatcher = 0;

	delete m_ghostPairCallback;
	m_ghostPairCallback = 0;

	delete m_pairCache;
	m_pairCache = 0;

	delete m_collisionConfiguration;
	m_collisionConfiguration = 0;

	delete m_debug;
	m_debug = 0;

	delete m_dbvt;
	m_dbvt = 0;

	if (!m_objects.empty())
	{
		gkPhysicsControllers::Iterator iter = m_objects.iterator();
		while (iter.hasMoreElements())
			delete iter.getNext();

		m_objects.clear();
	}

}



void gkDynamicsWorld::enableDebugPhysics(bool enable, bool debugAabb)
{
	if(enable)
	{
		if(!m_debug)
		{
			m_debug = new gkPhysicsDebug(this);

			m_dynamicsWorld->setDebugDrawer(m_debug);
		}

		if (debugAabb)
			m_debug->setDebugMode(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawAabb);
		else
			m_debug->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	}
	else if(m_debug)
	{
		m_dynamicsWorld->setDebugDrawer(0);

		delete m_debug;

		m_debug = 0;
	}
}


gkRigidBody *gkDynamicsWorld::createRigidBody(gkGameObject *state)
{
	GK_ASSERT(state);
	gkRigidBody *rb = new gkRigidBody(state, this);
	rb->create();
	m_objects.push_back(rb);
	return rb;
}



gkCharacter *gkDynamicsWorld::createCharacter(gkGameObject *state)
{
	GK_ASSERT(state);
	gkCharacter *character = new gkCharacter(state, this);
	character->create();
	m_objects.push_back(character);
	return character;
}



void gkDynamicsWorld::destroyObject(gkPhysicsController *cont)
{
	UTsize pos;
	if ((pos = m_objects.find(cont)) != UT_NPOS)
	{
		m_objects.erase(pos);

		cont->destroy();
		delete cont;
	}
}




void gkDynamicsWorld::localDrawObject(gkPhysicsController *phyCon)
{
	btCollisionObject *colObj = phyCon->getCollisionObject();

	GK_ASSERT(m_debug && m_dynamicsWorld && colObj);

	if(colObj->isStaticObject()) 
		return;


	btVector3 color(btScalar(1.), btScalar(1.), btScalar(1.));
	switch (colObj->getActivationState())
	{
	case ACTIVE_TAG:
		color = btVector3(btScalar(1.), btScalar(1.), btScalar(1.)); break;
	case ISLAND_SLEEPING:
		color = btVector3(btScalar(0.), btScalar(1.), btScalar(0.)); break;
	case WANTS_DEACTIVATION:
		color = btVector3(btScalar(0.), btScalar(1.), btScalar(1.)); break;
	case DISABLE_DEACTIVATION:
		color = btVector3(btScalar(1.), btScalar(0.), btScalar(0.)); break;
	case DISABLE_SIMULATION:
		color = btVector3(btScalar(1.), btScalar(1.), btScalar(0.)); break;
	default:
		color = btVector3(btScalar(1), btScalar(0.), btScalar(0.));
	};

	if (phyCon->isSuspended())
		color = btVector3(btScalar(1), btScalar(0.5), btScalar(0.));


	m_dynamicsWorld->debugDrawObject(colObj->getWorldTransform(),
	                                 colObj->getCollisionShape(),
	                                 color);



	if (m_debug->getDebugMode() & btIDebugDraw::DBG_DrawAabb)
	{
		color[0] = btScalar(1.);
		color[1] = color[2] = btScalar(0.);
		btVector3 minAabb, maxAabb;

		colObj->getCollisionShape()->getAabb(colObj->getWorldTransform(), minAabb, maxAabb);
		m_debug->drawAabb(minAabb, maxAabb, color);
	}
}



void gkDynamicsWorld::step(gkScalar tick)
{
	GK_ASSERT(m_dynamicsWorld);
	m_dynamicsWorld->stepSimulation(tick);
}



void gkDynamicsWorld::resetContacts()
{
	if (m_handleContacts && !m_objects.empty())
	{
		gkPhysicsControllers::Iterator iter = m_objects.iterator();
		while (iter.hasMoreElements())
		{
			iter.getNext()->_resetContactInfo();
		}
	}
}



void gkDynamicsWorld::substep(gkScalar tick)
{
	if (m_handleContacts)
	{
		int nr = m_dispatcher->getNumManifolds();

		for (int i = 0; i < nr; ++i)
		{
			btPersistentManifold *manifold = m_dispatcher->getManifoldByIndexInternal(i);

			gkPhysicsController *colA = gkPhysicsController::castController(manifold->getBody0());
			gkPhysicsController *colB = gkPhysicsController::castController(manifold->getBody1());


			colA->_resetContactInfo();
			colB->_resetContactInfo();
		}

		for (int i = 0; i < nr; ++i)
		{
			btPersistentManifold *manifold = m_dispatcher->getManifoldByIndexInternal(i);

			gkPhysicsController *colA = gkPhysicsController::castController(manifold->getBody0());
			gkPhysicsController *colB = gkPhysicsController::castController(manifold->getBody1());

			colA->_handleManifold(manifold);
			colB->_handleManifold(manifold);
		}
	}
}



void gkDynamicsWorld::DrawDebug()
{
	if (m_debug)
	{
		if (!m_objects.empty())
		{
			gkPhysicsControllers::Iterator iter = m_objects.iterator();
			while (iter.hasMoreElements())
			{
				gkPhysicsController *cont = iter.getNext();


				gkGameObject *ob = cont->getObject();
				if (ob->isInstanced())
					localDrawObject(cont);
			}
		}
	}
}



void gkDynamicsWorld::substepCallback(btDynamicsWorld *dyn, btScalar tick)
{
	gkDynamicsWorld *world = static_cast<gkDynamicsWorld *>(dyn->getWorldUserInfo());
	GK_ASSERT(world);
	world->substep(tick);
}




gkVariable *gkDynamicsWorld::getDBVTInfo(void)
{
	return m_dbvt ? m_dbvt->getInfo() : 0;
}




void gkDynamicsWorld::handleDbvt(gkCamera *cam)
{
	if (!m_dbvt)
		return;

	m_dbvt->mark(cam, (btDbvtBroadphase *)m_pairCache, m_objects);
}
