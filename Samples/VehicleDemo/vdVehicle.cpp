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

vdGearBox::vdGearBox(bool automatic, short numGears, gkScalar shiftTime, gkScalar reverseRatio)
		: m_isAutomatic(automatic), m_numGears(numGears), m_reverseRatio(reverseRatio),
		m_gears(0), m_currentGear(0), m_shifTime(shiftTime), m_isShifting(false), 
		m_passedSinceShift(0)
{
	m_gears = new vdGear[m_numGears];
}

vdGearBox::~vdGearBox()
{
	delete []  m_gears;
}

void vdGearBox::setGear(const short &numGear, const gkScalar &ratio, const gkScalar &rpmLow, const gkScalar &rpmHigh)
{
	if(numGear>0 && numGear<=m_numGears)
	{
		m_gears[numGear-1].m_ratio = ratio;
		m_gears[numGear-1].m_rpmLow = rpmLow;
		m_gears[numGear-1].m_rpmHigh = rpmHigh;
	}
}

gkScalar vdGearBox::getCurrentRatio(void)
{
	if(m_isShifting)
		return 0;
	if(m_currentGear>0 && m_currentGear<=m_numGears)
		return m_gears[m_currentGear-1].m_ratio;
	else if (m_currentGear==-1)
		return m_reverseRatio;
	else 
		return 0;
}

void vdGearBox::setCurrentGear(short num)
{
	if(!m_isShifting && num >=-1 && num <=m_numGears)
	{
		m_isShifting = true;
		m_passedSinceShift = 0;
		
		m_currentGear = num;
	}
}

void vdGearBox::shiftUp(void)
{
	if(!m_isShifting)
	{
		m_isShifting = true;
		m_passedSinceShift = 0;
		
		if(m_currentGear<m_numGears) m_currentGear+=1;
	}
}

void vdGearBox::shiftDown(void)
{
	if(!m_isShifting)
	{
		m_isShifting = true;
		m_passedSinceShift = 0;
		
		if(m_currentGear>-1) m_currentGear-=1;
	}
}

void vdGearBox::update(gkScalar rate, const gkScalar &rpm)
{
	if(m_isShifting)
	{
		m_passedSinceShift += rate;
		
		if(m_passedSinceShift>m_shifTime)
			m_isShifting = false;
	}
	else if(m_isAutomatic)
	{
		if(m_currentGear>0)
		{
			if(rpm>m_gears[m_currentGear-1].m_rpmHigh)
			{
				shiftUp();
			}
			else if(rpm<m_gears[m_currentGear-1].m_rpmLow)
			{
				if(m_currentGear>1)
					shiftDown();
			}
		}
		else if(m_currentGear==0)
		{
			if(rpm>2000)
				shiftUp();
		}
	}
}



vdVehicle::vdVehicle(gkScene *scene, const gkString &chassis, const gkScalar &power, const gkScalar &brakes, const gkScalar &rearBrakeRatio, const gkScalar & maxSteering, const gkScalar &ruptor)
		: m_scene(scene), m_dynamicWorld(0), m_raycaster(0), m_vehicle(0), m_object(0), m_chassis(0), 
		m_gaz(0), m_brake(0), m_steer(0), m_handBrake(false), m_engineTorque(power), m_brakePower(brakes), 
		m_rearBrakeRatio(rearBrakeRatio), m_maxSteering(maxSteering), m_gearBox(0), m_currentRpm(0), m_ruptorRpm(ruptor)
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
	updateVehicle(rate);
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

void vdVehicle::updateVehicle(gkScalar rate)
{
	float gearRatio=1;
	float wheelTorque;
	float frontBrake;
	float rearBrake;
	float steering;
	float wheelRpm;
	
	
	wheelRpm = 60 * getCurrentSpeedKmHour() / (3.6f * 2 * gkPi * m_vehicle->getWheelInfo(0).m_wheelsRadius);

	if(m_gearBox)
	{
		gearRatio = m_gearBox->getCurrentRatio();
		if(gearRatio == 0 )
			m_currentRpm = m_ruptorRpm * m_gaz;
		else
			m_currentRpm = wheelRpm * gearRatio;
		m_gearBox->update(rate, m_currentRpm);
	}
	
	wheelTorque = m_gaz * m_engineTorque *gearRatio;
	if(m_currentRpm > m_ruptorRpm)
		wheelTorque = 0;
	
	frontBrake = m_brake * m_brakePower;
	if(m_handBrake)
		rearBrake = 100000;
	else
		rearBrake = m_brake * m_rearBrakeRatio * m_brakePower;
	
	steering = m_steer * m_maxSteering;
	
	btTransform trans;
	gkTransformState gtrans;
	
	int i;
	for (i=0;i<m_vehicle->getNumWheels();i++)
	{
		btWheelInfo &btwheel = m_vehicle->getWheelInfo(i);
		
		if (btwheel.m_bIsFrontWheel)
		{
			m_vehicle->setSteeringValue(steering, i);
			m_vehicle->setBrake(frontBrake, i);
		}
		else
		{
			m_vehicle->applyEngineForce(wheelTorque, i);
			m_vehicle->setBrake(rearBrake, i);
			//btwheel.m_deltaRotation = 0;
		}
	
		//synchronize the wheels with the (interpolated) chassis worldtransform
		m_vehicle->updateWheelTransform(i,true);
		
		//update wheels position
		trans = btwheel.m_worldTransform;
		gtrans.setIdentity();
		gtrans.loc = gkVector3(trans.getOrigin());
		gtrans.rot = gkQuaternion(trans.getRotation().w(), trans.getRotation().x(), trans.getRotation().y(), trans.getRotation().z());
		m_wheelObjects[i]->setTransform(gtrans);
		
		//debugdraw
//		btCollisionShape *wheelShape = new btCylinderShapeX(btVector3(0.25, btwheel.m_wheelsRadius, btwheel.m_wheelsRadius));
//		m_dynamicWorld->debugDrawObject(trans, wheelShape, btVector3(0,1,0));
//		delete wheelShape;
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

