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

vdVehicle::vdVehicle(gkScene *scene)
		: m_dynamicWorld(0), m_raycaster(0), m_vehicle(0), m_object(0), m_chassis(0), m_wheelShape(0), m_gaz(0), m_brake(0), m_steer(0)
{
	btVector3 wheelDirectionCS0(0,0,-1);
	btVector3 wheelAxleCS(1,0,0); 
	float connectionHeight = 0.4;
	float sideOffset = 0.73;
	float frontOffest = 1.211;
	float rearOffset = 1.282;
	float wheelRadius = 0.357;
	float wheelWidth = 0.2;
	
	float suspensionRestLength = 0.58;
	float suspensionTravelCm = 12; //centimeters
	float suspensionStiffness= 80.0;
	float suspensionDamping = 0.3;
	float suspensionCompression = 0.5;
	float wheelFriction = 10;
	float rollInfluence = 0.1;
	
	
	m_object = scene->getObject("MiniG");
	
	if(!m_object->isInstanced())
		m_object->createInstance();
	
	m_chassis = ((gkRigidBody*)m_object->getPhysicsController())->getBody();
	m_chassis->setActivationState(DISABLE_DEACTIVATION);
	
	m_dynamicWorld = scene->getDynamicsWorld()->getBulletWorld();
	
	m_raycaster = new btDefaultVehicleRaycaster(m_dynamicWorld);
	m_vehicle = new btRaycastVehicle(m_tuning,m_chassis,m_raycaster);
	m_vehicle->setCoordinateSystem(0,2,1); //right,up,front
	
	m_dynamicWorld->addVehicle(m_vehicle);
	
	m_wheelShape = new btCylinderShapeX(btVector3(wheelWidth,wheelRadius,wheelRadius));
	bool isFront = true;
	btVector3 connectionPointCS0(sideOffset,frontOffest, connectionHeight);
	m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFront);
	
	connectionPointCS0 = btVector3(-sideOffset,frontOffest, connectionHeight);
	m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFront);
	
	isFront = false;
	connectionPointCS0 = btVector3(sideOffset,-rearOffset, connectionHeight);
	m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFront);
	
	connectionPointCS0 = btVector3(-sideOffset,-rearOffset, connectionHeight);
	m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFront);
	
	for (int i=0;i<m_vehicle->getNumWheels();i++)
	{
		btWheelInfo& wheel = m_vehicle->getWheelInfo(i);
		wheel.m_suspensionStiffness = suspensionStiffness;
		wheel.m_wheelsDampingRelaxation = suspensionDamping;
		wheel.m_wheelsDampingCompression = suspensionCompression;
		wheel.m_frictionSlip = wheelFriction;
		wheel.m_rollInfluence = rollInfluence;
		wheel.m_maxSuspensionTravelCm = suspensionTravelCm;
	}
}

vdVehicle::~vdVehicle()
{
	delete m_vehicle;
	delete m_raycaster;
	delete m_wheelShape;
}

void vdVehicle::tick(gkScalar rate)
{
	float power=2000.0f;
	float brake=300.0f;
	float steer=0.2f;
	
	m_vehicle->applyEngineForce(m_gaz*power, 2);
	m_vehicle->setBrake(m_brake*brake, 2);
	m_vehicle->applyEngineForce(m_gaz*power, 3);
	m_vehicle->setBrake(m_brake*brake, 3);
	
	m_vehicle->setSteeringValue(m_steer*steer, 0);
	m_vehicle->setSteeringValue(m_steer*steer, 1);
	
	btTransform trans;
	int i;
	for (i=0;i<m_vehicle->getNumWheels();i++)
	{
		//synchronize the wheels with the (interpolated) chassis worldtransform
		m_vehicle->updateWheelTransform(i,true);
		//draw wheels (cylinders)
		trans = m_vehicle->getWheelInfo(i).m_worldTransform;
		
		if (m_dynamicWorld->getDebugDrawer())
			m_dynamicWorld->debugDrawObject(trans, m_wheelShape, btVector3(0,1,0));
	}
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

gkScalar vdVehicle::getCurrentSpeedKmHour()
{
	return m_vehicle->getCurrentSpeedKmHour();
}
