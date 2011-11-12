/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

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
#include "gkNearSensor.h"
#include "gkLogicManager.h"
#include "gkLogicDispatcher.h"
#include "gkDynamicsWorld.h"
#include "gkGameObject.h"
#include "gkPhysicsController.h"
#include "gkScene.h"
#include "gkContactTest.h"
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"


gkNearSensor::gkNearSensor(gkGameObject* object, gkLogicLink* link, const gkString& name)
	:        gkLogicSensor(object, link, name), m_range(0.01), m_resetrange(0.01), m_previous(false), m_material(""), m_prop("")
{
	m_dispatchType = DIS_CONSTANT;
	connect();
}


gkLogicBrick* gkNearSensor::clone(gkLogicLink* link, gkGameObject* dest)
{
	gkNearSensor* sens = new gkNearSensor(*this);
	sens->cloneImpl(link, dest);
	return sens;
}



bool gkNearSensor::query(void)
{
	m_nearObjList.clear();
	gkScene* scene = m_object->getOwner();
	gkDynamicsWorld* dyn = scene->getDynamicsWorld();

	btDynamicsWorld* btw = dyn->getBulletWorld();

	gkVector3 vec = m_object->getWorldPosition();

	gkAllContactResultCallback exec;

	btTransform btt;
	btt.setIdentity();
	btt.setOrigin(btVector3(vec.x, vec.y, vec.z));

	btSphereShape btss(m_range);
	if (m_previous)
		btss.setUnscaledRadius(m_resetrange);

	btCollisionObject btco = btCollisionObject();
	btco.setCollisionShape(&btss);
	btco.setWorldTransform(btt);

	btw->contactTest( &btco, exec);

	if (btw->getDebugDrawer())
		btw->debugDrawObject(btt, &btss, btVector3(0, 1, 0));

	if (!exec.hasHit())
		return m_previous = false;
	if (exec.m_contactObjects.empty())
		return m_previous = false;

//	if (m_material.empty() && m_prop.empty())
//		return m_previous = true;

	utArray<const btCollisionObject*> contacts = exec.m_contactObjects;
	utArrayIterator< utArray<const btCollisionObject*> > iter(contacts);

	while (iter.hasMoreElements())
	{
		gkGameObject* ob = gkPhysicsController::castObject(iter.peekNext());
		if (ob!=m_object && gkPhysicsController::sensorTest(ob, m_prop, m_material)){
			if (m_nearObjList.find(ob)==UT_NPOS){
				m_nearObjList.push_back(ob);
			}
		}

		iter.getNext();
	}
	if (m_nearObjList.empty())
		return m_previous = false;
	else
		return m_previous = true;
}
