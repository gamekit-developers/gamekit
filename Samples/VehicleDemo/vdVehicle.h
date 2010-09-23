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

#ifndef VDVEHICLE_H
#define VDVEHICLE_H

#include "OgreKit.h"
#include "btBulletDynamicsCommon.h"

typedef struct vdGear
{
	gkScalar m_ratio;
	gkScalar m_rpmLow;
	gkScalar m_rpmHigh;

	vdGear(const gkScalar& ratio = 0.0f, const gkScalar& rpmLow = 1000.0f, const gkScalar& rpmHigh = 4000.0f)
		: m_ratio(ratio), m_rpmLow(rpmLow), m_rpmHigh(rpmHigh) {}

} vdGear;

class vdGearBox
{
private:
	bool      m_isAutomatic;
	short     m_currentGear; // -1=reverse 0=neutral
	gkScalar  m_reverseRatio;
	short     m_type;
	short     m_numGears;
	vdGear*   m_gears;
	gkScalar  m_shifTime;
	bool      m_isShifting;
	gkScalar  m_passedSinceShift;

public:
	vdGearBox(bool automatic, short numGears, gkScalar shiftTime = 1.0f, gkScalar reverseRatio = 0.0f);
	~vdGearBox();

	gkScalar getCurrentRatio(void);
	void setGear(const short& numGear, const gkScalar& ratio, const gkScalar& rpmLow, const gkScalar& rpmHigh);

	int getCurrentGear(void) { return m_currentGear; }
	void setCurrentGear(short num);

	void shiftUp(void);
	void shiftDown(void);

	void update(gkScalar rate, const gkScalar& rpm);
};

class vdVehicle
{
private:
	gkScene*                           m_scene;
	btDynamicsWorld*                   m_dynamicWorld;
	btDefaultVehicleRaycaster*         m_raycaster;
	btRaycastVehicle*                  m_vehicle;
	btRaycastVehicle::btVehicleTuning  m_tuning;
	gkGameObject*                      m_object;
	utArray<gkGameObject*>             m_wheelObjects;
	btRigidBody*                       m_chassis;

	vdGearBox*                         m_gearBox;

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

	void createVehicle(void);
	void updateVehicle(gkScalar rate);

public:
	vdVehicle(gkScene* scene, const gkString& chassis, const gkScalar& power, const gkScalar& brakes, const gkScalar& rearBrakeRatio, const gkScalar& maxSteering, const gkScalar& ruptor = 6000);
	~vdVehicle();

	void addWheel(const gkString& name, gkScalar radius, gkVector3 connectionPoint, gkVector3 wheelDirection,
	              gkVector3 wheelAxle, bool isFront, gkScalar restLength, gkScalar stiffness, gkScalar dampingRelax,
	              gkScalar dampingComp, gkScalar friction, gkScalar roll, gkScalar travelDist);

	void tick(gkScalar rate);

	void setTransfrom(const gkTransformState& v) { m_object->setTransform(v); }
	void setGaz(gkScalar ratio)                  { m_gaz = ratio; }
	void setBrake(gkScalar ratio)                { m_brake = ratio; }
	void setSteer(gkScalar ratio)                { m_steer = ratio; }
	void setHandBrake(bool v)                    { m_handBrake = v; }
	void setGearBox(vdGearBox* box)              { m_gearBox = box; }

	gkScalar getCurrentSpeedKmHour(void)         { return m_vehicle->getCurrentSpeedKmHour(); }
	int getCurrentGear(void)                     { return m_gearBox ? m_gearBox->getCurrentGear() : 0; }
	void setCurrentGear(int num)                 { if (m_gearBox) m_gearBox->setCurrentGear(num); }
	gkScalar getVelocityEulerZ(void);
	gkScalar getCurrentRpm(void)                 { return m_currentRpm; }

	void shiftUp(void)   {if (m_gearBox) m_gearBox->shiftUp();}
	void shiftDown(void) {if (m_gearBox) m_gearBox->shiftDown();}
};

#endif // VDVEHICLE_H
