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





gkDynamicsWorld::gkDynamicsWorld(const gkString& name, gkScene* scene)
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

	gkVector3& grav = m_scene->getProperties().m_gravity;
	m_dynamicsWorld->setGravity(btVector3(grav.x, grav.y, grav.z));
	m_dynamicsWorld->setWorldUserInfo(this);
	m_dynamicsWorld->setInternalTickCallback(substepCallback, static_cast<void*>(this));

	enableDebugPhysics(gkEngine::getSingleton().getUserDefs().debugPhysics, gkEngine::getSingleton().getUserDefs().debugPhysicsAabb);

	if (gkEngine::getSingleton().getUserDefs().useBulletDbvt)
		m_dbvt = new gkDbvt();
}



void gkDynamicsWorld::destroyInstanceImpl(void)
{
	int i;
	for (i = m_dynamicsWorld->getNumConstraints() - 1; i >= 0; i--)
	{
		m_dynamicsWorld->removeConstraint(m_dynamicsWorld->getConstraint(i));
	}

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
	if (enable)
	{
		if (!m_debug)
		{
			m_debug = new gkPhysicsDebug(this);

			m_dynamicsWorld->setDebugDrawer(m_debug);
		}

		if (debugAabb)
			m_debug->setDebugMode(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawAabb);
		else
			m_debug->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	}
	else if (m_debug)
	{
		m_dynamicsWorld->setDebugDrawer(0);

		delete m_debug;

		m_debug = 0;
	}
}

btRigidBody* gkDynamicsWorld::getFixedBody()
{
	static btRigidBody s_fixed(0, 0,0);
    s_fixed.setMassProps(btScalar(0.),btVector3(btScalar(0.),btScalar(0.),btScalar(0.)));

    return &s_fixed;
}

btTypedConstraint* gkDynamicsWorld::createConstraint(btRigidBody* rbA, btRigidBody*rbB, const gkPhysicsConstraintProperties& props)
{
	btVector3 pivotInA(gkMathUtils::get(props.m_pivot));
	btVector3 pivotInB(0,0,0);
	
	pivotInB = rbB ? rbB->getCenterOfMassTransform().inverse()(rbA->getCenterOfMassTransform()(pivotInA)) :
		rbA->getCenterOfMassTransform() * pivotInA;	

	//localConstraintFrameBasis
	btMatrix3x3 localCFrame;
	localCFrame.setEulerZYX(props.m_axis.x, props.m_axis.y, props.m_axis.z);
	btVector3 axisInA = localCFrame.getColumn(0);
	btVector3 axis1 = localCFrame.getColumn(1);
	btVector3 axis2 = localCFrame.getColumn(2);
	bool angularOnly = false;


	btTypedConstraint* constraint = 0;

	if (props.m_type == GK_BALL_CONSTRAINT)
	{
		btPoint2PointConstraint* p2p = 0;

		if (rbB)
			p2p = new btPoint2PointConstraint(*rbA,*rbB,pivotInA,pivotInB);
		else			
			p2p = new btPoint2PointConstraint(*rbA,pivotInA);			

		constraint = p2p;
	}
	else if (props.m_type == GK_HINGE_CONSTRAINT)
	{
		btHingeConstraint* hinge = 0;
		if (rbB)
		{
			btVector3 axisInB = rbB->getCenterOfMassTransform().getBasis().inverse() * (rbA->getCenterOfMassTransform().getBasis() * axisInA);

			hinge = new btHingeConstraint(*rbA, *rbB, pivotInA, pivotInB, axisInA, axisInB);
		} 
		else
		{
			hinge = new btHingeConstraint(*rbA, pivotInA, axisInA);
		}
		hinge->setAngularOnly(angularOnly);

		constraint = hinge;

	}
	else if (props.m_type == GK_D6_CONSTRAINT)
	{
        btTransform frameInA;
        btTransform frameInB;
        if (axis1.length() == 0.0)
        {
			btPlaneSpace1(axisInA, axis1, axis2);
        }
        frameInA.getBasis().setValue(axisInA.x(), axis1.x(), axis2.x(),
									axisInA.y(), axis1.y(), axis2.y(),
									axisInA.z(), axis1.z(), axis2.z());
        frameInA.setOrigin( pivotInA );
                                                
        btTransform inv = rbB ? rbB->getCenterOfMassTransform().inverse() : btTransform::getIdentity();
        btTransform globalFrameA = rbA->getCenterOfMassTransform() * frameInA;
        frameInB = inv  * globalFrameA;
        bool useReferenceFrameA = true;
                                                
        if (!rbB)
			rbB = getFixedBody();
        btGeneric6DofSpringConstraint* genericConstraint = new btGeneric6DofSpringConstraint(*rbA, *rbB, frameInA, frameInB, useReferenceFrameA);

        //if it is a generic 6DOF constraint, set all the limits accordingly
        int dof, dofbit=1;
        for (dof=0;dof<6;dof++)
        {
			if (props.m_flag & dofbit)
			{
				///this access is a bloated, will probably make special cases for common arrays
				btScalar minLimit = props.m_minLimit[dof];
				btScalar maxLimit = props.m_maxLimit[dof];
				genericConstraint->setLimit(dof, minLimit, maxLimit);
			} 
			else
			{
				//minLimit > maxLimit means free(disabled limit) for this degree of freedom
				genericConstraint->setLimit(dof, 1, -1);
			}
			dofbit<<=1;
        }

		constraint = genericConstraint;
	}
	else if (props.m_type == GK_CONETWIST_CONSTRAINT)
	{
		//TODO: implement conetwist constraint
	}

	return constraint;
}

gkRigidBody* gkDynamicsWorld::createRigidBody(gkGameObject* state)
{
	GK_ASSERT(state);
	gkRigidBody* rb = new gkRigidBody(state, this);
	rb->create();
	m_objects.push_back(rb);
	return rb;
}



gkCharacter* gkDynamicsWorld::createCharacter(gkGameObject* state)
{
	GK_ASSERT(state);
	gkCharacter* character = new gkCharacter(state, this);
	character->create();
	m_objects.push_back(character);
	return character;
}



void gkDynamicsWorld::destroyObject(gkPhysicsController* cont)
{
	UTsize pos;
	if ((pos = m_objects.find(cont)) != UT_NPOS)
	{
		m_objects.erase(pos);

		cont->destroy();
		delete cont;
	}
}




void gkDynamicsWorld::localDrawObject(gkPhysicsController* phyCon)
{
	btCollisionObject* colObj = phyCon->getCollisionObject();

	GK_ASSERT(m_debug && m_dynamicsWorld && colObj);

	if (colObj->isStaticObject())
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

	//uncomment this for better simulation quality (but a little bit less performance)
	//	m_dynamicsWorld->stepSimulation(tick,10,1./240.);
	m_dynamicsWorld->stepSimulation(tick);

	m_dynamicsWorld->debugDrawWorld();

	// uncomment this to print bullet profiling information
	//CProfileManager::dumpAll();
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


void gkDynamicsWorld::presubstep(gkScalar tick)
{
	// update callbacks
	utArrayIterator<gkDynamicsWorld::Listeners> iter(m_listeners);
	while(iter.hasMoreElements())
		iter.getNext()->presubtick(tick);
}


void gkDynamicsWorld::substep(gkScalar tick)
{
	if (m_handleContacts)
	{
		int nr = m_dispatcher->getNumManifolds();

		for (int i = 0; i < nr; ++i)
		{
			btPersistentManifold* manifold = m_dispatcher->getManifoldByIndexInternal(i);

			gkPhysicsController* colA = gkPhysicsController::castController(manifold->getBody0());
			gkPhysicsController* colB = gkPhysicsController::castController(manifold->getBody1());


			colA->_resetContactInfo();
			colB->_resetContactInfo();
		}

		for (int i = 0; i < nr; ++i)
		{
			btPersistentManifold* manifold = m_dispatcher->getManifoldByIndexInternal(i);

			gkPhysicsController* colA = gkPhysicsController::castController(manifold->getBody0());
			gkPhysicsController* colB = gkPhysicsController::castController(manifold->getBody1());

			colA->_handleManifold(manifold);
			colB->_handleManifold(manifold);
		}
	}
	
	// update callbacks
	utArrayIterator<gkDynamicsWorld::Listeners> iter(m_listeners);
	while(iter.hasMoreElements())
		iter.getNext()->subtick(tick);
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
				gkPhysicsController* cont = iter.getNext();


				gkGameObject* ob = cont->getObject();
				if (ob->isInstanced())
					localDrawObject(cont);
			}
		}
	}
}



void gkDynamicsWorld::substepCallback(btDynamicsWorld* dyn, btScalar tick)
{
	gkDynamicsWorld* world = static_cast<gkDynamicsWorld*>(dyn->getWorldUserInfo());
	GK_ASSERT(world);
	world->substep(tick);
}



void gkDynamicsWorld::presubstepCallback(btDynamicsWorld *dyn, btScalar tick)
{
	gkDynamicsWorld *world = static_cast<gkDynamicsWorld *>(dyn->getWorldUserInfo());
	GK_ASSERT(world);
	world->presubstep(tick);
}

gkVariable* gkDynamicsWorld::getDBVTInfo(void)
{
	return m_dbvt ? m_dbvt->getInfo() : 0;
}




void gkDynamicsWorld::handleDbvt(gkCamera* cam)
{
	if (!m_dbvt)
		return;

	m_dbvt->mark(cam, (btDbvtBroadphase*)m_pairCache, m_objects);
}



void gkDynamicsWorld::exportBullet(const gkString& fileName)
{
	int maxSerializeBufferSize = 1024 * 1024 * 5;

	btDefaultSerializer*    serializer = new btDefaultSerializer(maxSerializeBufferSize);
	m_dynamicsWorld->serialize(serializer);

	FILE* file = fopen(fileName.c_str(), "wb");
	fwrite(serializer->getBufferPointer(), serializer->getCurrentBufferSize(), 1, file);

	fclose(file);
	delete serializer;
}

void gkDynamicsWorld::addListener(gkDynamicsWorld::Listener *listener)
{
	m_listeners.push_back(listener);
}





void gkDynamicsWorld::removeListener(gkDynamicsWorld::Listener *listener)
{
	if (m_listeners.find(listener))
		m_listeners.erase(listener);
}

