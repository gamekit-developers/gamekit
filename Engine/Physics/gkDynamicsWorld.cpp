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
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"

class gkDbvt : public btDbvt::ICollide
{
private:

	int         m_tvs, m_tot;
	gkVariable *m_debug;

public:
	gkDbvt();
	~gkDbvt();

	void Process(const btDbvtNode *nd);

	void update(gkDynamicsWorld::ObjectList &oblist);
	void drawDebug(void);
};


static bool CustomMaterialCombinerCallback(btManifoldPoint &cp,	const btCollisionObject *pObj0, int partId0, int index0, const btCollisionObject *pObj1, int partId1, int index1)
{
	return true;
}


gkDynamicsWorld::gkDynamicsWorld(const gkString &name, gkScene *scene)
	:       m_scene(scene),
	        m_dynamicsWorld(0), m_collisionConfiguration(0),
	        m_pairCache(0), m_ghostPairCallback(0), m_dispatcher(0), m_constraintSolver(0),
	        m_debug(0), m_handleContacts(true), m_dbvt(0)

{
//	gContactAddedCallback = CustomMaterialCombinerCallback;
	loadImpl();

}

gkDynamicsWorld::~gkDynamicsWorld()
{
	unloadImpl();
}


void gkDynamicsWorld::loadImpl(void)
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

void gkDynamicsWorld::unloadImpl(void)
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
		gkObject *rb = m_objects.begin();

		while (rb)
		{
			// remove bullet body
			gkObject *tmp = rb;
			rb = rb->getNext();
			delete tmp;
		}
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


void gkDynamicsWorld::createParentChildHierarchy(void)
{

	int i;
	btAlignedObjectArray<btCollisionObject *> children;

	for (i = 0; i < m_dynamicsWorld->getNumCollisionObjects(); i++)
	{
		btCollisionObject *childColObj = m_dynamicsWorld->getCollisionObjectArray()[i];
		gkObject *childNode = static_cast<gkObject *>(childColObj->getUserPointer());
		if (!childNode)
			continue;

		gkGameObject *obj = childNode->getObject();
		if (obj->getParent())
			children.push_back(childColObj);
	}

	for (i = 0; i < children.size(); i++)
	{
		btCollisionObject *childColObj = children[i];
		gkObject *childNode = static_cast<gkObject *>(childColObj->getUserPointer());

		m_objects.erase(childNode);

		gkGameObject *ob = childNode->getObject();
		if (ob->isInActiveLayer())
			childNode->unload();
		delete childNode;
	}

}

gkRigidBody *gkDynamicsWorld::createRigidBody(gkGameObject *state)
{
	GK_ASSERT(state);
	gkRigidBody *rb = new gkRigidBody(state->getName(), state, this);
	m_objects.push_back(rb);
	return rb;
}

gkCharacter *gkDynamicsWorld::createCharacter(gkGameObject *state)
{
	GK_ASSERT(state);
	gkCharacter *character = new gkCharacter(state->getName(), state, this);
	m_objects.push_back(character);
	return character;
}


void gkDynamicsWorld::destroyObject(gkObject *state)
{
	if (m_objects.find(state))
	{
		m_objects.erase(state);
		state->unload();
		delete state;
	}
}

void gkDynamicsWorld::localDrawObject(btCollisionObject *colObj)
{
	if(colObj->isStaticObject()) return;

	GK_ASSERT(m_debug && m_dynamicsWorld && colObj);

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

// Do one full physics step
void gkDynamicsWorld::step(gkScalar tick)
{
	GK_ASSERT(m_dynamicsWorld);
	m_dynamicsWorld->stepSimulation(tick);
}

void gkDynamicsWorld::resetContacts()
{
	if (m_handleContacts && !m_objects.empty())
	{
		gkObject *rb = m_objects.begin();

		while(rb)
		{
			if(rb->isLoaded())
				rb->resetContactInfo();

			rb = rb->getNext();
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

			gkObject *colA = static_cast<gkObject *>(static_cast<btCollisionObject *>(manifold->getBody0())->getUserPointer());
			gkObject *colB = static_cast<gkObject *>(static_cast<btCollisionObject *>(manifold->getBody1())->getUserPointer());

			colA->resetContactInfo();
			colB->resetContactInfo();
		}

		for (int i = 0; i < nr; ++i)
		{
			btPersistentManifold *manifold = m_dispatcher->getManifoldByIndexInternal(i);

			gkObject *colA = static_cast<gkObject *>(static_cast<btCollisionObject *>(manifold->getBody0())->getUserPointer());
			gkObject *colB = static_cast<gkObject *>(static_cast<btCollisionObject *>(manifold->getBody1())->getUserPointer());

			colA->handleManifold(manifold);
			colB->handleManifold(manifold);
		}
	}
}

void gkDynamicsWorld::DrawDebug()
{
	if (m_debug)
	{
		if (!m_objects.empty())
		{
			gkObject *rb = m_objects.begin();

			while (rb)
			{
				if (rb->isLoaded())
				{
					localDrawObject(rb->getCollisionObject());
				}
				rb = rb->getNext();
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


gkDbvt::gkDbvt() : m_tvs(0), m_tot(0), m_debug(0)
{
	if (gkEngine::getSingleton().getUserDefs().showDebugProps)
	{
		m_debug = new gkVariable("btDbvt", true);
		gkEngine::getSingleton().addDebugProperty(m_debug);
	}
}

gkDbvt::~gkDbvt()
{
	if (m_debug)
	{
		gkEngine::getSingleton().removeDebugProperty(m_debug);
		delete m_debug;
		m_debug = 0;
	}
}

void gkDbvt::Process(const btDbvtNode *nd)
{
	btBroadphaseProxy *proxy = (btBroadphaseProxy *)nd->data;
	btCollisionObject *colob = (btCollisionObject *)proxy->m_clientObject;
	gkGameObject *object = ((gkGameObject *)colob->getUserPointer())->getObject();

	if (object->getType() == GK_ENTITY)
	{
		Ogre::MovableObject *mov = object->getMovable();
		if (mov)
		{
			if (!mov->isVisible() && !object->getProperties().isInvisible())
			{
				mov->setVisible(true);
				m_tvs++;
			}
		}
	}

}

void gkDbvt::update(gkDynamicsWorld::ObjectList &oblist)
{
	m_tot = 0;
	m_tvs = 0;
	if (!oblist.empty())
	{
		gkObject *rb = oblist.begin();

		while (rb)
		{
			if (rb->isLoaded())
			{
				gkGameObject *ob = rb->getObject();
				if (ob->getType() == GK_ENTITY && !ob->getProperties().isInvisible())
				{
					Ogre::MovableObject *mov = ob->getMovable();
					if (mov)
					{
						m_tot++;
						mov->setVisible(false);
					}
				}
			}
			rb = rb->getNext();
		}
	}
}

void gkDbvt::drawDebug(void)
{
	if (m_debug)
	{
		char buf[72];
		sprintf(buf, "Tot Visible %i, of %i Bodies\n", m_tvs, m_tot);
		m_debug->setValue(gkString(buf));
	}
}




void gkDynamicsWorld::handleDbvt(gkCamera *cam)
{
	if (!m_dbvt)
		return;

	btDbvtBroadphase *cullTree = (btDbvtBroadphase *)m_pairCache;

	const Ogre::Plane *planes = cam->getCamera()->getFrustumPlanes();
	btVector3 normals[6];
	btScalar offsets[6];

	for (int i=0; i<6; ++i)
	{
		normals[i].setValue(planes[i].normal.x, planes[i].normal.y, planes[i].normal.z);
		offsets[i] = planes[i].d;
	}

	m_dbvt->update(m_objects);
	btDbvt::collideKDOP(cullTree->m_sets[1].m_root, normals, offsets, 6, *m_dbvt);
	btDbvt::collideKDOP(cullTree->m_sets[0].m_root, normals, offsets, 6, *m_dbvt);
	m_dbvt->drawDebug();
}
