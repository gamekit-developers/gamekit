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

#include "vdVehicleNode.h"
//#include "OgreKit.h"

vdVehicleNode::vdVehicleNode(gkLogicTree *parent, size_t id)
		:	gkLogicNode(parent, id), m_vehicle(0)
{
	ADD_ISOCK(UPDATE, true);
	ADD_ISOCK(FRONT, false);
	ADD_ISOCK(REAR, false);
	ADD_ISOCK(LEFT, false);
	ADD_ISOCK(RIGHT, false);
	ADD_OSOCK(ZROT, 0);
}

bool vdVehicleNode::evaluate(gkScalar tick)
{
	bool front = GET_SOCKET_VALUE(FRONT);
	bool rear = GET_SOCKET_VALUE(REAR);
	bool left = GET_SOCKET_VALUE(LEFT);
	bool right = GET_SOCKET_VALUE(RIGHT);
	bool brake = false;
	
	if(m_vehicle)
	{
		m_vehicle->setGaz(0.0);
		m_vehicle->setSteer(0.0);
		
		if(front)
			if(m_vehicle->getCurrentSpeedKmHour() > -FLT_MIN)
				m_vehicle->setGaz(1.0);
			else
				brake = true;
		if(rear)
			if(m_vehicle->getCurrentSpeedKmHour() < FLT_MIN)
				m_vehicle->setGaz(-0.5);
			else
				brake = true;
		if(brake)
			m_vehicle->setBrake(1.0);
		else
			m_vehicle->setBrake(0.0);
		if(left)
			m_vehicle->setSteer(1.0);
		else if(right)
			m_vehicle->setSteer(-1.0);
	}
	
	SET_SOCKET_VALUE(ZROT, m_vehicle->getVelocityEulerZ());
	
	bool update = GET_SOCKET_VALUE(UPDATE);
	
	return update && m_vehicle;
}
