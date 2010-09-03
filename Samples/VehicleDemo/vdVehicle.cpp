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

#include "vdVehicle.h"

vdVehicle::vdVehicle(gkScene *scene, const gkString &chassis, const gkScalar &power, const gkScalar &brakes, const gkScalar &rearBrakeRatio, const gkScalar & maxSteering)
		: m_scene(scene), m_dynamicWorld(0), m_raycaster(0), m_vehicle(0), m_object(0), m_chassis(0), 
		m_gaz(0), m_brake(0), m_steer(0), m_enginePower(power), m_brakePower(brakes), m_rearBrakeRatio(rearBrakeRatio), m_maxSteering(maxSteering)
{

	m_object = scene->getObject(chassis);
	m_dynamicWorld = scene->getDynamicsWorld()->getBulletWorld();
	
	createVehicle();
}

vdVehicle::~vdVehicle()
{
	delete m_vehicle;
	delete m_raycaster;
}

void vdVehicle::tick(gkScalar rate)
{
	updateVehicle();
}

void vdVehicle::createVehicle()
{
	if(!m_object->isInstanced())
		m_object->createInstance();
	
	m_chassis = ((gkRigidBody*)m_object->getPhysicsController())->getBody();
	m_chassis->setActivationState(DISABLE_DEACTIVATION);
	
	m_raycaster = new btDefaultVehicleRaycaster(m_dynamicWorld);
	
	m_vehicle = new btRaycastVehicle(m_tuning,m_chassis,m_raycaster);
	m_vehicle->setCoordinateSystem(0,2,1);
	
	m_dynamicWorld->addVehicle(m_vehicle);
}

void vdVehicle::updateVehicle()
{
	btTransform trans;
	gkTransformState gtrans;
	
	int i;
	for (i=0;i<m_vehicle->getNumWheels();i++)
	{
		btWheelInfo &btwheel = m_vehicle->getWheelInfo(i);
		
		if (btwheel.m_bIsFrontWheel)
		{
			m_vehicle->applyEngineForce(m_gaz * m_enginePower, i);
			m_vehicle->setSteeringValue(m_steer * m_maxSteering, i);
			m_vehicle->setBrake(m_brake * m_brakePower, i);
		}
		else
		{
			m_vehicle->setBrake(m_brake * m_rearBrakeRatio * m_brakePower, i);
		}
	
		//synchronize the wheels with the (interpolated) chassis worldtransform
		m_vehicle->updateWheelTransform(i,true);
		
		//update wheels position
		trans = m_vehicle->getWheelInfo(i).m_worldTransform;
		gtrans.setIdentity();
		gtrans.loc = gkVector3(trans.getOrigin());
		gtrans.rot = gkQuaternion(trans.getRotation().w(), trans.getRotation().x(), trans.getRotation().y(), trans.getRotation().z());
		m_wheelObjects[i]->setTransform(gtrans);
	}
}

void vdVehicle::addWheel(const gkString &name, gkScalar radius, gkVector3 connectionPoint, gkVector3 wheelDirection, 
						 gkVector3 wheelAxle, bool isFront, gkScalar restLength, gkScalar stiffness, gkScalar dampingRelax,
						 gkScalar dampingComp, gkScalar friction, gkScalar roll, gkScalar travelDist)
{
	gkGameObject *wheel = m_scene->getObject(name);
	
	if(!wheel->isInstanced())
		wheel->createInstance();
		
	m_wheelObjects.push_back(wheel);
	
	btVector3 cp(connectionPoint.x, connectionPoint.y, connectionPoint.z);
	btVector3 wd(wheelDirection.x, wheelDirection.y, wheelDirection.z);
	btVector3 wa(wheelAxle.x, wheelAxle.y, wheelAxle.z);
	
	btWheelInfo & btwheel = m_vehicle->addWheel(cp ,wd, wa, restLength, radius, m_tuning, isFront);
	
	btwheel.m_suspensionStiffness = stiffness;
	btwheel.m_wheelsDampingRelaxation = dampingRelax;
	btwheel.m_wheelsDampingCompression = dampingComp;
	btwheel.m_frictionSlip = friction;
	btwheel.m_rollInfluence = roll;
	btwheel.m_maxSuspensionTravelCm = travelDist;
}

void vdVehicle::setTransfrom(const gkTransformState &v)
{
	m_object->setTransform(v);
}

void vdVehicle::setGaz(gkScalar ratio)
{
	m_gaz = ratio;
}

void vdVehicle::setBrake(gkScalar ratio)
{
	m_brake = ratio;
}

void vdVehicle::setSteer(gkScalar ratio)
{
	m_steer = ratio;
}

gkScalar vdVehicle::getCurrentSpeedKmHour(void)
{
	return m_vehicle->getCurrentSpeedKmHour();
}

gkScalar vdVehicle::getVelocityEulerZ(void)
{
	gkQuaternion rot;
	gkVector3 eul;
	
	gkVector3 dir = gkVector3(m_vehicle->getRigidBody()->getLinearVelocity());
	
	if( gkAbs(dir.x)<0.7 && gkAbs(dir.y)<0.7)
	{
		rot = m_object->getWorldOrientation();
		eul = gkMathUtils::getEulerFromQuat(rot, true);
	
		return eul.z;
	}
	
	dir.z=0;
	rot = dir.getRotationTo(gkVector3::UNIT_Y);
	eul = gkMathUtils::getEulerFromQuat(rot, true);
	
	return -eul.z;
}

