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

#ifndef GKVEHICLE_H
#define GKVEHICLE_H

#include "OgreKit.h"
#include "btBulletDynamicsCommon.h"

typedef struct gkWheelProperties
{
	gkGameObject* m_object;
	gkScalar      m_radius;
	bool          m_isFront;
	gkVector3     m_connectionPoint;
	gkVector3     m_wheelDirection;
	gkVector3     m_wheelAxle;
	gkScalar      m_restLength;
	gkScalar      m_stiffness;
	gkScalar      m_dampingRelax;
	gkScalar      m_dampingComp;
	gkScalar      m_friction;
	gkScalar      m_rollInfluence;
	gkScalar      m_travelDistCm;
	
} gkWheelProperties;

typedef struct gkGear
{
	gkScalar m_ratio;
	gkScalar m_rpmLow;
	gkScalar m_rpmHigh;

	gkGear(const gkScalar& ratio = 0.0f, const gkScalar& rpmLow = 1000.0f, const gkScalar& rpmHigh = 4000.0f)
		: m_ratio(ratio), m_rpmLow(rpmLow), m_rpmHigh(rpmHigh) {}

} gkGear;

class gkGearBox
{
private:
	bool      m_isAutomatic;
	short     m_currentGear; // -1=reverse 0=neutral
	gkScalar  m_reverseRatio;
	short     m_type;
	short     m_numGears;
	gkGear*   m_gears;
	gkScalar  m_shifTime;
	bool      m_isShifting;
	gkScalar  m_passedSinceShift;

public:
	gkGearBox(bool automatic, short numGears, gkScalar shiftTime = 1.0f, gkScalar reverseRatio = 0.0f);
	~gkGearBox();

	gkScalar getCurrentRatio(void);
	void setGearProperties(const short& numGear, const gkScalar& ratio, const gkScalar& rpmLow, const gkScalar& rpmHigh);

	int getCurrentGear(void) { return m_currentGear; }
	void setCurrentGear(short num);

	void shiftUp(void);
	void shiftDown(void);

	void update(gkScalar rate, const gkScalar& rpm);
};

class gkVehicle : public gkDynamicsWorld::Listener
{
public:
	enum
	{
		DT_PROPULSION,
		DT_TRACTION,
		DT_ALLWHEEL
	};
	
protected:
	gkScene*                           m_scene;
	gkDynamicsWorld*                   m_dynamicWorld;
	btDefaultVehicleRaycaster*         m_raycaster;
	btRaycastVehicle*                  m_vehicle;
	btRaycastVehicle::btVehicleTuning  m_tuning;
	gkGameObject*                      m_object;
	utArray<gkWheelProperties*>        m_wheels;
	btRigidBody*                       m_chassis;

	gkGearBox*                         m_gearBox;

	short int m_driveTrain;
	gkScalar m_engineTorque;
	gkScalar m_brakePower;
	gkScalar m_rearBrakeRatio;
	gkScalar m_maxSteering;
	gkScalar m_currentRpm;
	gkScalar m_ruptorRpm;

	gkScalar m_gaz;
	gkScalar m_brake;
	gkScalar m_steer;
	bool m_handBrake;

	void updateTransmition(gkScalar rate);
	void updateWheels(gkScalar rate);
	
public:
	gkVehicle(gkScene* scene);
	~gkVehicle();

	virtual void load() = 0;

	void createVehicle(void);
	void updateVehicle(gkScalar rate);
	
	void addWheel(gkGameObject* object, gkScalar radius, gkVector3 connectionPoint, gkVector3 wheelDirection,
	              gkVector3 wheelAxle, bool isFront, gkScalar restLength, gkScalar stiffness, gkScalar dampingRelax,
	              gkScalar dampingComp, gkScalar friction, gkScalar roll, gkScalar travelDist);

	void tick(gkScalar rate);
	void presubtick(gkScalar rate);
	void subtick(gkScalar rate);

	void setTransform(const gkTransformState& v) { m_object->setTransform(v); }

	void setDriveTrain(short int v)              { m_driveTrain = v; }
	void setEngineTorque(gkScalar v)             { m_engineTorque = v; }
	void setBrakePower(gkScalar v)               { m_brakePower = v; }
	void setRearBrakeRatio(gkScalar v)           { m_rearBrakeRatio = v; }
	void setMaxSteeringAngle(gkScalar v)         { m_maxSteering = v; }
	void setRuptorRpm(gkScalar v)                { m_ruptorRpm = v; }
	void setChassisObject(gkGameObject* v)       { m_object = v; }
	
	void setGaz(gkScalar ratio)                  { m_gaz = ratio; }
	void setBrake(gkScalar ratio)                { m_brake = ratio; }
	void setSteer(gkScalar ratio)                { m_steer = ratio; }
	void setHandBrake(bool v)                    { m_handBrake = v; }
	void setGearBox(gkGearBox* box)              { m_gearBox = box; }

	gkScalar getCurrentSpeedKmHour(void)         { return m_vehicle->getCurrentSpeedKmHour(); }
	int getCurrentGear(void)                     { return m_gearBox ? m_gearBox->getCurrentGear() : 0; }
	void setCurrentGear(int num)                 { if (m_gearBox) m_gearBox->setCurrentGear(num); }
	gkScalar getVelocityEulerZ(void);
	gkScalar getCurrentRpm(void)                 { return m_currentRpm; }

	void shiftUp(void)   {if (m_gearBox) m_gearBox->shiftUp();}
	void shiftDown(void) {if (m_gearBox) m_gearBox->shiftDown();}
	
	bool isWheelDriven(int i);
	int getNumberOfDrivenWheel(void);
};

#define VEHICLE_RESOURCE_GROUP "VehicleDemo"

#endif // GKVEHICLE_H
