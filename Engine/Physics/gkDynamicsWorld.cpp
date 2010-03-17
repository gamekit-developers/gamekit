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
#include "gkDynamicsWorld.h"
#include "gkRigidBody.h"
#include "gkEngine.h"
#include "gkUserDefs.h"
#include "gkPhysicsDebug.h"
#include "gkGameObject.h"
#include "gkScene.h"
#include "btBulletDynamicsCommon.h"



gkDynamicsWorld::gkDynamicsWorld(const gkString& name, gkScene *scene, gkObject::Loader *manual)
:       gkObject(name, manual), m_scene(scene),
        m_dynamicsWorld(0), m_collisionConfiguration(0),
        m_pairCache(0), m_dispatcher(0), m_constraintSolver(0),
        m_debug(0), m_handleContacts(false)

{
}

gkDynamicsWorld::~gkDynamicsWorld()
{
}


void gkDynamicsWorld::preLoadImpl(void)
{
    // prepare world

    if (m_dynamicsWorld)
        return;

    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    m_pairCache              = new btDbvtBroadphase();
    m_dispatcher             = new btCollisionDispatcher(m_collisionConfiguration);
    m_constraintSolver       = new btSequentialImpulseConstraintSolver();
    m_dynamicsWorld          = new btDiscreteDynamicsWorld(m_dispatcher,
            m_pairCache,
            m_constraintSolver,
            m_collisionConfiguration);

    gkVector3& grav = m_scene->getProperties().gravity;
    m_dynamicsWorld->setGravity(btVector3(grav.x, grav.y, grav.z));
    m_dynamicsWorld->setWorldUserInfo(this);
    m_dynamicsWorld->setInternalTickCallback(substepCallback, static_cast<void*>(this));

	enableDebugPhysics(gkEngine::getSingleton().getUserDefs().debugPhysics, gkEngine::getSingleton().getUserDefs().debugPhysicsAabb);
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

void gkDynamicsWorld::loadImpl(void)
{

    if (!m_dynamicsWorld)
        preLoadImpl();


    m_handleContacts = false;

    if (!m_bodies.empty())
    {
        gkRigidBody *rb = m_bodies.begin();

        while (rb)
        {
            if (!rb->isLoaded())
            {
                gkGameObject *ob = rb->getObject();
                if (ob->isInActiveLayer())
                {
                    if (!m_handleContacts && rb->wantsContactInfo())
                        m_handleContacts = true;
                    rb->load();
                }
                rb = rb->getNext();
            }
        }
        createParentChildHierarchy();
    }
}


void gkDynamicsWorld::unloadImpl(void)
{
    delete m_dynamicsWorld;
    m_dynamicsWorld = 0;

    delete m_constraintSolver;
    m_constraintSolver = 0;

    delete m_dispatcher;
    m_dispatcher = 0;

    delete m_pairCache;
    m_pairCache = 0;

    delete m_collisionConfiguration;
    m_collisionConfiguration = 0;

    delete m_debug;
    m_debug = 0;

    if (!m_bodies.empty())
    {
        gkRigidBody *rb = m_bodies.begin();

        while (rb)
        {
            // remove bullet body
            gkRigidBody *tmp = rb;
            rb = rb->getNext();
            delete tmp;
        }
        m_bodies.clear();
    }

}

void gkDynamicsWorld::createParentChildHierarchy(void)
{

    int i;
    btAlignedObjectArray<btCollisionObject*> children;

    for (i = 0;i < m_dynamicsWorld->getNumCollisionObjects();i++)
    {
        btCollisionObject* childColObj = m_dynamicsWorld->getCollisionObjectArray()[i];
        gkRigidBody* childNode = (gkRigidBody*)childColObj->getUserPointer();
        if (!childNode)
            continue;

        gkGameObject* obj = childNode->getObject();
        if (obj->getParent())
            children.push_back(childColObj);
    }

    for (i = 0;i < children.size();i++)
    {
        btCollisionObject* childColObj = children[i];
        gkRigidBody* childNode = (gkRigidBody*)childColObj->getUserPointer();

        m_bodies.erase(childNode);

        gkGameObject *ob = childNode->getObject();
        if (ob->isInActiveLayer())
            childNode->unload();
        delete childNode;
    }

}


// Creates the runtime body.
// gkRigidBody takes care of setting motion states, as well as
// applying physics calculations on a gkGameObject level

gkRigidBody *gkDynamicsWorld::createRigidBody(gkGameObject *state, gkObject::Loader *manual)
{
    GK_ASSERT(state);
    gkRigidBody *rb = new gkRigidBody(state->getName(), state, this, manual);
    m_bodies.push_back(rb);
    return rb;
}

void gkDynamicsWorld::localDrawObject(btRigidBody *colObj)
{
    GK_ASSERT(m_debug && m_dynamicsWorld && colObj);

    btVector3 color(btScalar(1.), btScalar(1.), btScalar(1.));
    switch (colObj->getActivationState())
    {
    case ACTIVE_TAG:
        color = btVector3(btScalar(1.), btScalar(1.), btScalar(1.)); break;
    case ISLAND_SLEEPING:
        color = btVector3(btScalar(0.), btScalar(1.), btScalar(0.));break;
    case WANTS_DEACTIVATION:
        color = btVector3(btScalar(0.), btScalar(1.), btScalar(1.));break;
    case DISABLE_DEACTIVATION:
        color = btVector3(btScalar(1.), btScalar(0.), btScalar(0.));break;
    case DISABLE_SIMULATION:
        color = btVector3(btScalar(1.), btScalar(1.), btScalar(0.));break;
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
    if (!isLoaded())
        return;

    GK_ASSERT(m_dynamicsWorld);
    m_dynamicsWorld->stepSimulation(tick);
}



void gkDynamicsWorld::substep(gkScalar tick)
{
    if (m_handleContacts)
    {
        int nr = m_dispatcher->getNumManifolds(), i, j;

        if (!nr && !m_bodies.empty())
        {
            gkRigidBody *rb = m_bodies.begin();
            while (rb)
            {
                if (rb->isLoaded() && rb->wantsContactInfo())
                    rb->getContacts().resize(0);
                rb = rb->getNext();
            }
        }

        for (i = 0; i < nr; ++i)
        {
            btPersistentManifold* manifold = m_dispatcher->getManifoldByIndexInternal(i);

            gkRigidBody* colA = static_cast<gkRigidBody*>(static_cast<btCollisionObject*>(manifold->getBody0())->getUserPointer());

            if (colA->wantsContactInfo())
			{
				gkRigidBody::ContactArray &destA = colA->getContacts();
                destA.resize(0);
			}

            gkRigidBody* colB = static_cast<gkRigidBody*>(static_cast<btCollisionObject*>(manifold->getBody1())->getUserPointer());

            if (colB->wantsContactInfo())
			{
	            gkRigidBody::ContactArray &destB = colB->getContacts();
                destB.resize(0);
			}
		}

        for (i = 0; i < nr; ++i)
        {
            btPersistentManifold* manifold = m_dispatcher->getManifoldByIndexInternal(i);

            gkRigidBody* colA = static_cast<gkRigidBody*>(static_cast<btCollisionObject*>(manifold->getBody0())->getUserPointer());
            gkRigidBody* colB = static_cast<gkRigidBody*>(static_cast<btCollisionObject*>(manifold->getBody1())->getUserPointer());

            int nrc = manifold->getNumContacts();

            gkRigidBody::ContactArray &destA = colA->getContacts();
            gkRigidBody::ContactArray &destB = colB->getContacts();

            if (nrc)
            {
                for (j = 0; j < nrc; ++j)
                {
                    gkRigidBody::ContactInfo cinf;
                    btManifoldPoint &pt = manifold->getContactPoint(j);

                    if (pt.getDistance() < 0.f)
                    {
                        if (colA->wantsContactInfo()) {
                            cinf.collider = colB; cinf.point = pt;
                            destA.push_back(cinf); 
                        }

                        if (colB->wantsContactInfo()) {
                            cinf.collider = colA; cinf.point = pt;
                            destB.push_back(cinf);
                        }
                    }
                }
            }
        }
    }

    if (m_debug)
    {
        if (!m_bodies.empty())
        {
            gkRigidBody *rb = m_bodies.begin();
            while (rb)
            {
                if (rb->isLoaded())
                {
                    btRigidBody *body = rb->getBody();
                    if (!body->isStaticOrKinematicObject())
                        localDrawObject(body);
                }
                rb = rb->getNext();
            }
        }
        m_debug->flush();
    }
}

void gkDynamicsWorld::substepCallback(btDynamicsWorld *dyn, btScalar tick)
{
    gkDynamicsWorld *world = static_cast<gkDynamicsWorld*>(dyn->getWorldUserInfo());
    GK_ASSERT(world);
    world->substep(tick);
}
