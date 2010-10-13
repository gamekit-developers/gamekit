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

#include "gkVehicle.h"

gkGearBox::gkGearBox(bool automatic, short numGears, gkScalar shiftTime, gkScalar reverseRatio)
	: m_isAutomatic(automatic), m_numGears(numGears), m_reverseRatio(reverseRatio),
	  m_gears(0), m_currentGear(0), m_shifTime(shiftTime), m_isShifting(false),
	  m_passedSinceShift(0)
{
	m_gears = new gkGear[m_numGears];
}

gkGearBox::~gkGearBox()
{
	delete []  m_gears;
}

void gkGearBox::setGearProperties(const short& numGear, const gkScalar& ratio, const gkScalar& rpmLow, const gkScalar& rpmHigh)
{
	if (numGear > 0 && numGear <= m_numGears)
	{
		m_gears[numGear-1].m_ratio = ratio;
		m_gears[numGear-1].m_rpmLow = rpmLow;
		m_gears[numGear-1].m_rpmHigh = rpmHigh;
	}
}

gkScalar gkGearBox::getCurrentRatio(void)
{
	if (m_isShifting)
		return 0;
	if (m_currentGear > 0 && m_currentGear <= m_numGears)
		return m_gears[m_currentGear-1].m_ratio;
	else if (m_currentGear == -1)
		return m_reverseRatio;
	else
		return 0;
}

void gkGearBox::setCurrentGear(short num)
{
	if (!m_isShifting && num >= -1 && num <= m_numGears)
	{
		m_isShifting = true;
		m_passedSinceShift = 0;

		m_currentGear = num;
	}
}

void gkGearBox::shiftUp(void)
{
	if (!m_isShifting)
	{
		m_isShifting = true;
		m_passedSinceShift = 0;

		if (m_currentGear < m_numGears) m_currentGear += 1;
	}
}

void gkGearBox::shiftDown(void)
{
	if (!m_isShifting)
	{
		m_isShifting = true;
		m_passedSinceShift = 0;

		if (m_currentGear > -1) m_currentGear -= 1;
	}
}

void gkGearBox::update(gkScalar rate, const gkScalar& rpm)
{
	if (m_isShifting)
	{
		m_passedSinceShift += rate;

		if (m_passedSinceShift > m_shifTime)
			m_isShifting = false;
	}
	else if (m_isAutomatic)
	{
		if (m_currentGear > 0)
		{
			if (rpm > m_gears[m_currentGear-1].m_rpmHigh)
			{
				shiftUp();
			}
			else if (rpm < m_gears[m_currentGear-1].m_rpmLow)
			{
				if (m_currentGear > 1)
					shiftDown();
			}
		}
		else if (m_currentGear == 0)
		{
			if (rpm > 2000)
				shiftUp();
		}
	}
}



gkVehicle::gkVehicle(gkScene* scene)
	: m_scene(scene), m_dynamicWorld(0), m_raycaster(0), m_vehicle(0), m_object(0), m_chassis(0),
	  m_gaz(0), m_brake(0), m_steer(0), m_handBrake(false), m_engineTorque(0), m_brakePower(0),
	  m_rearBrakeRatio(1.0f), m_maxSteering(0), m_gearBox(0), m_currentRpm(0), m_ruptorRpm(0),
	  m_driveTrain(DT_PROPULSION)
{

}

gkVehicle::~gkVehicle()
{
	if(m_dynamicWorld)
		m_dynamicWorld->removeListener(this);
		
	gkWheelProperties** ptr = m_wheels.ptr();
	for (UTsize i = 0; i < m_wheels.size(); ++i)
		delete ptr[i];
		
	if(m_gearBox)
		delete m_gearBox;
	if(m_vehicle)
		delete m_vehicle;
	if(m_raycaster)
		delete m_raycaster;
}

void gkVehicle::tick(gkScalar rate)
{
	updateVehicle(rate);
}

void gkVehicle::presubtick(gkScalar rate)
{

}

void gkVehicle::subtick(gkScalar rate)
{
	for(UTsize i=0; i<m_wheels.size(); i++)
	{
		gkWheelProperties* wheel = m_wheels[i];
		btWheelInfo& btwheel = m_vehicle->getWheelInfo(i);
		
		// Block rear wheels when using handbrake
		if(m_handBrake && !btwheel.m_bIsFrontWheel)
			btwheel.m_deltaRotation = 0;
		
		
		// Rotate the wheel when not in contact
		if(!btwheel.m_raycastInfo.m_isInContact)
		{
			if(m_gaz > 0.05f && isWheelDriven(i) )
				btwheel.m_deltaRotation = m_gaz;
			else
				btwheel.m_deltaRotation *= 0.995;
		}
		
		btwheel.m_rotation += btwheel.m_deltaRotation;
	}
}

void gkVehicle::createVehicle()
{
	if(m_object)
	{
		if (!m_object->isInstanced())
			m_object->createInstance();

		m_dynamicWorld = m_scene->getDynamicsWorld();
		m_dynamicWorld->addListener(this);
		
		m_chassis = ((gkRigidBody*)m_object->getPhysicsController())->getBody();
		m_chassis->setActivationState(DISABLE_DEACTIVATION);
	
		m_raycaster = new btDefaultVehicleRaycaster(m_dynamicWorld->getBulletWorld());
	
		m_vehicle = new btRaycastVehicle(m_tuning, m_chassis, m_raycaster);
		m_vehicle->setCoordinateSystem(0, 2, 1);
	
		m_dynamicWorld->getBulletWorld()->addVehicle(m_vehicle);
		
		for(UTsize i=0; i<m_wheels.size(); i++)
		{
			gkWheelProperties* wheel = m_wheels[i];
			btVector3 cp(wheel->m_connectionPoint.x, wheel->m_connectionPoint.y, wheel->m_connectionPoint.z);
			btVector3 wd(wheel->m_wheelDirection.x, wheel->m_wheelDirection.y, wheel->m_wheelDirection.z);
			btVector3 wa(wheel->m_wheelAxle.x, wheel->m_wheelAxle.y, wheel->m_wheelAxle.z);
		
			btWheelInfo& btwheel = m_vehicle->addWheel(cp , wd, wa, wheel->m_restLength, wheel->m_radius, m_tuning, wheel->m_isFront);
		
			btwheel.m_suspensionStiffness = wheel->m_stiffness;
			btwheel.m_wheelsDampingRelaxation = wheel->m_dampingRelax;
			btwheel.m_wheelsDampingCompression = wheel->m_dampingComp;
			btwheel.m_frictionSlip = wheel->m_friction;
			btwheel.m_rollInfluence = wheel->m_rollInfluence;
			btwheel.m_maxSuspensionTravelCm = wheel->m_travelDistCm;
		}
	}
}

void gkVehicle::updateTransmition(gkScalar rate)
{
	if (m_gearBox)
	{
		float wheelRpm;
		
		if(m_vehicle->getNumWheels() >0)
			wheelRpm = 60 * getCurrentSpeedKmHour() / (3.6f * 2 * gkPi * m_vehicle->getWheelInfo(0).m_wheelsRadius);
		else
			wheelRpm = 0;

		float gearRatio = m_gearBox->getCurrentRatio();
		if (gearRatio == 0 )
			m_currentRpm = m_ruptorRpm * m_gaz;
		else
			m_currentRpm = wheelRpm * gearRatio;
		m_gearBox->update(rate, m_currentRpm);
	}
}

void gkVehicle::updateVehicle(gkScalar rate)
{
	float gearRatio;
	float wheelTorque;
	float frontBrake;
	float rearBrake;
	float steering;

	updateTransmition(rate);
	
	gearRatio = (m_gearBox)? m_gearBox->getCurrentRatio():1;
	wheelTorque = m_gaz * m_engineTorque * gearRatio / getNumberOfDrivenWheel();
	if (m_currentRpm > m_ruptorRpm)
		wheelTorque = 0;

	frontBrake = m_brake * m_brakePower;
	if (m_handBrake)
		rearBrake = 100000;
	else
		rearBrake = m_brake * m_rearBrakeRatio * m_brakePower;

	steering = m_steer * m_maxSteering;

	btTransform trans;
	gkTransformState gtrans;

	int i;
	for (i = 0; i < m_vehicle->getNumWheels(); i++)
	{
		btWheelInfo& btwheel = m_vehicle->getWheelInfo(i);

		if(isWheelDriven(i))
			m_vehicle->applyEngineForce(wheelTorque, i);
		
		if (btwheel.m_bIsFrontWheel)
		{
			m_vehicle->setSteeringValue(steering, i);
			m_vehicle->setBrake(frontBrake, i);
		}
		else
		{
			m_vehicle->setBrake(rearBrake, i);
		}

		//synchronize the wheels with the (interpolated) chassis worldtransform
		m_vehicle->updateWheelTransform(i, true);

		//update wheels position
		trans = btwheel.m_worldTransform;
		gtrans.setIdentity();
		gtrans.loc = gkVector3(trans.getOrigin());
		gtrans.rot = gkQuaternion(trans.getRotation().w(), trans.getRotation().x(), trans.getRotation().y(), trans.getRotation().z());
		m_wheels[i]->m_object->setTransform(gtrans);

		//debugdraw
//        btCollisionShape *wheelShape = new btCylinderShapeX(btVector3(0.25, btwheel.m_wheelsRadius, btwheel.m_wheelsRadius));
//        m_dynamicWorld->debugDrawObject(trans, wheelShape, btVector3(0,1,0));
//        delete wheelShape;
	}
}

void gkVehicle::addWheel(gkGameObject* object, gkScalar radius, gkVector3 connectionPoint, gkVector3 wheelDirection,
                         gkVector3 wheelAxle, bool isFront, gkScalar restLength, gkScalar stiffness, gkScalar dampingRelax,
                         gkScalar dampingComp, gkScalar friction, gkScalar roll, gkScalar travelDist)
{
	gkWheelProperties* wheel = new gkWheelProperties();
	
	wheel->m_object = object;
	wheel->m_radius = radius;
	wheel->m_isFront = isFront;
	wheel->m_connectionPoint = connectionPoint;
	wheel->m_wheelDirection = wheelDirection;
	wheel->m_wheelAxle = wheelAxle;
	wheel->m_restLength = restLength;
	wheel->m_stiffness = stiffness;
	wheel->m_dampingRelax = dampingRelax;
	wheel->m_dampingComp = dampingComp;
	wheel->m_friction = friction;
	wheel->m_rollInfluence = roll;
	wheel->m_travelDistCm = travelDist;
	
	m_wheels.push_back(wheel);
}

gkScalar gkVehicle::getVelocityEulerZ(void)
{
	gkQuaternion rot;
	gkVector3 eul;

	if(!m_vehicle)
		return 0;

	gkVector3 dir = gkVector3(m_vehicle->getRigidBody()->getLinearVelocity());

	if ( gkAbs(dir.x) < 0.7 && gkAbs(dir.y) < 0.7)
	{
		rot = m_object->getWorldOrientation();
		eul = gkMathUtils::getEulerFromQuat(rot, true);

		return eul.z;
	}

	dir.z = 0;
	rot = dir.getRotationTo(gkVector3::UNIT_Y);
	eul = gkMathUtils::getEulerFromQuat(rot, true);

	return -eul.z;
}

bool gkVehicle::isWheelDriven(int i)
{
	if(i<0 || i>=(int)m_wheels.size())
		return false;
		
	switch(m_driveTrain)
	{
		case DT_ALLWHEEL: return true;
		case DT_PROPULSION: return !m_wheels[i]->m_isFront;
		case DT_TRACTION: return m_wheels[i]->m_isFront;
		default: return false;
	}
}

int gkVehicle::getNumberOfDrivenWheel(void)
{
	int count = 0;
	for(UTsize i=0; i<m_wheels.size(); i++)
	{
		if(isWheelDriven(i))
			count++;
	}
	return count;
}
