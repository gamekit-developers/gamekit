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
#include "gkRaySensor.h"
#include "gkLogicManager.h"
#include "gkLogicDispatcher.h"
#include "gkDynamicsWorld.h"
#include "gkGameObject.h"
#include "gkScene.h"
#include "btBulletDynamicsCommon.h"


// ----------------------------------------------------------------------------
gkRaySensor::gkRaySensor(gkGameObject *object, gkLogicLink *link, const gkString &name)
	:       gkLogicSensor(object, link, name), m_range(0.01), m_axis(-1), m_material(""), m_prop("")
{
	m_dispatchType = DIS_CONSTANT;
	connect();
}


// ----------------------------------------------------------------------------
gkLogicBrick *gkRaySensor::clone(gkLogicLink *link, gkGameObject *dest)
{
	gkRaySensor *sens = new gkRaySensor(*this);
	sens->cloneImpl(link, dest);
	return sens;
}


// ----------------------------------------------------------------------------
bool gkRaySensor::query(void)
{
	gkScene *scene = m_object->getOwner();
	gkDynamicsWorld *dyn = scene->getDynamicsWorld();

	btDynamicsWorld *btw = dyn->getBulletWorld();

	gkVector3 dir;
	switch (m_axis)
	{
	case RA_XPOS: {dir = gkVector3(m_range,0,0);  break;}
	case RA_YPOS: {dir = gkVector3(0,m_range,0);  break;}
	case RA_ZPOS: {dir = gkVector3(0,0,m_range);  break;}
	case RA_XNEG: {dir = gkVector3(-m_range,0,0); break;}
	case RA_YNEG: {dir = gkVector3(0,-m_range,0); break;}
	case RA_ZNEG: {dir = gkVector3(0,0,-m_range); break;}
	}

	gkVector3 vec = m_object->getWorldPosition();
	btCollisionWorld::ClosestRayResultCallback exec(btVector3(vec.x, vec.y, vec.z),
	        btVector3(vec.x + dir.x, vec.y + dir.y, vec.z + dir.z));
	btw->rayTest(exec.m_rayFromWorld, exec.m_rayToWorld, exec);

	if (!exec.hasHit())
		return false;


	if (!exec.m_collisionObject)
		return false;

	gkGameObject *object = ((gkGameObject *)exec.m_collisionObject->getUserPointer())->getObject();


	if (!m_material.empty() || !m_prop.empty())
	{
		if (!m_prop.empty())
		{
			if (object->hasVariable(m_prop))
				return true;
		}
		else if (!m_material.empty())
		{
			if (object->hasSensorMaterial(m_material))
				return true;
		}

		return false;
	}

	return true;
}
