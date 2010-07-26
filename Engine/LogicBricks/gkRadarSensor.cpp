/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): xat
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
#include "gkRadarSensor.h"
#include "gkLogicManager.h"
#include "gkLogicDispatcher.h"
#include "gkDynamicsWorld.h"
#include "gkGameObject.h"
#include "gkScene.h"
#include "btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
gkRadarSensor::gkRadarSensor(gkGameObject *object, gkLogicLink *link, const gkString &name)
:        gkRaySensor(object, link, name), m_angle(0.1)
{
}

// ----------------------------------------------------------------------------
gkLogicBrick* gkRadarSensor::clone(gkLogicLink *link, gkGameObject *dest)
{
    gkRadarSensor *sens = new gkRadarSensor(*this);
    sens->cloneImpl(link, dest);
    return sens;
}


// ----------------------------------------------------------------------------
bool gkRadarSensor::query(void)
{
    gkScene *scene = m_object->getOwner();
    gkDynamicsWorld *dyn = scene->getDynamicsWorld();

    btDynamicsWorld *btw = dyn->getBulletWorld();

    const gkScalar offs = m_range / 2.f;
    gkEuler ori;
    gkVector3 dir;
    switch (m_axis)
    {
    case RA_XPOS: {dir = gkVector3(offs,0,0);  break;}
    case RA_YPOS: {dir = gkVector3(0,offs,0);  break;}
    case RA_ZPOS: {dir = gkVector3(0,0,offs);  break;}
    case RA_XNEG: {dir = gkVector3(-offs,0,0); break;}
    case RA_YNEG: {dir = gkVector3(0,-offs,0); break;}
    case RA_ZNEG: {dir = gkVector3(0,0,-offs); break;}
    }

    switch (m_axis)
    {
    case RA_XPOS: {ori = gkEuler(0,  -90,   0);     break;}
    case RA_YPOS: {ori = gkEuler(90,  0,    0);     break;}
    case RA_ZPOS: {ori = gkEuler(0,   180,  0);     break;}
    case RA_XNEG: {ori = gkEuler(0,  -90, -180);    break;}
    case RA_YNEG: {ori = gkEuler(90,  0,  -180);    break;}
    case RA_ZNEG: {ori = gkEuler(0,   0,    0);     break;}
    }


    gkVector3 vec = m_object->getWorldPosition();
	dir = m_object->getWorldOrientation() * dir;
	btQuaternion btr = gkMathUtils::get(m_object->getWorldOrientation() * ori.toQuaternion());
    
    btCollisionWorld::ClosestConvexResultCallback exec(btVector3(vec.x, vec.y, vec.z), 
                                                    btVector3(vec.x + dir.x, vec.y + dir.y, vec.z + dir.z));

    btTransform start, end;
    start.setIdentity();
    end.setIdentity();
    start.setOrigin(exec.m_convexFromWorld);
    end.setOrigin(exec.m_convexToWorld);
	start.setRotation(btr);
	end.setRotation(btr);

	
	btConeShapeZ btcs(m_range*tan(m_angle/2), m_range);
    btw->convexSweepTest( &btcs, start, end, exec);

	if (btw->getDebugDrawer())
		btw->debugDrawObject(end, &btcs, btVector3(0,1,0));
	

    if (!exec.hasHit())
        return false;


    if (!exec.m_hitCollisionObject)
        return false;

    gkGameObject *object = ((gkGameObject*)exec.m_hitCollisionObject->getUserPointer())->getObject();


    if (!m_material.empty() || !m_prop.empty())
    {
        if (!m_prop.empty())
        {
            if (object->hasVariable(m_prop))
                return true;
        }
        else if (!m_material.empty())
        {
            if (object->getSensorMaterial() == m_material)
                return true;
        }

        return false;
    }

    return true;
}