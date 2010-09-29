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

#include "gkVehicleNode.h"
//#include "OgreKit.h"

gkVehicleNode::gkVehicleNode(gkLogicTree* parent, size_t id)
	:    gkLogicNode(parent, id), m_vehicle(0), m_steer(0)
{
	ADD_ISOCK(UPDATE, true);
	ADD_ISOCK(FRONT, false);
	ADD_ISOCK(REAR, false);
	ADD_ISOCK(LEFT, false);
	ADD_ISOCK(RIGHT, false)
	ADD_ISOCK(STEER_TIME, 0);
	ADD_ISOCK(HAND_BRAKE, false);
	ADD_ISOCK(GEAR_UP, false);
	ADD_ISOCK(GEAR_DOWN, false);
	ADD_OSOCK(ZROT, 0);
	ADD_OSOCK(KMH, 0);
	ADD_OSOCK(GEAR, 0);
	ADD_OSOCK(RPM, 0);
}

bool gkVehicleNode::evaluate(gkScalar tick)
{
	float steerSpeed = GET_SOCKET_VALUE(STEER_TIME);
	bool front = GET_SOCKET_VALUE(FRONT);
	bool rear = GET_SOCKET_VALUE(REAR);
	bool left = GET_SOCKET_VALUE(LEFT);
	bool right = GET_SOCKET_VALUE(RIGHT);
	bool brake = false;

	if (left)
		m_steer += tick / steerSpeed;
	else if (right)
		m_steer -= tick / steerSpeed;
	else
	{
		if (m_steer > 0)
		{
			m_steer -= tick / steerSpeed;
			m_steer = gkMaxf(m_steer, 0);
		}
		else
		{
			m_steer += tick / steerSpeed;
			m_steer = gkMinf(m_steer, 0);
		}
	}
	m_steer = gkClampf(m_steer, -1.0f, 1.0f);

	if (m_vehicle)
	{
		if (GET_SOCKET_VALUE(GEAR_UP))
			m_vehicle->shiftUp();
		else if (GET_SOCKET_VALUE(GEAR_DOWN))
			m_vehicle->shiftDown();

		m_vehicle->setGaz(0.0);
		m_vehicle->setSteer(0.0);

		if (front)
		{
			if (gkAbs(m_vehicle->getCurrentSpeedKmHour()) < 1 && m_vehicle->getCurrentGear() == -1)
				m_vehicle->shiftUp();
			if (m_vehicle->getCurrentGear() != -1)
				m_vehicle->setGaz(1.0);
			else
				brake = true;
		}

		if (rear)
		{
			if (gkAbs(m_vehicle->getCurrentSpeedKmHour()) < 1 && m_vehicle->getCurrentGear() != -1)
				m_vehicle->shiftDown();
			if (m_vehicle->getCurrentGear() == -1)
				m_vehicle->setGaz(1.0);
			else
				brake = true;
		}

		if (brake)
			m_vehicle->setBrake(1.0);
		else
			m_vehicle->setBrake(0.0);

		m_vehicle->setSteer(m_steer);

		m_vehicle->setHandBrake(GET_SOCKET_VALUE(HAND_BRAKE));
		
		
		SET_SOCKET_VALUE(ZROT, m_vehicle->getVelocityEulerZ());
		SET_SOCKET_VALUE(KMH, (int)(m_vehicle->getCurrentSpeedKmHour() + 0.5));
		SET_SOCKET_VALUE(GEAR, m_vehicle->getCurrentGear());
		SET_SOCKET_VALUE(RPM, m_vehicle->getCurrentRpm());

	}

	bool update = GET_SOCKET_VALUE(UPDATE);

	return update && m_vehicle;
}
