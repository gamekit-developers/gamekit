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

class vdVehicle
{
private:
	gkScene                           *m_scene;
	btDynamicsWorld                   *m_dynamicWorld;
	btDefaultVehicleRaycaster         *m_raycaster;
	btRaycastVehicle                  *m_vehicle;
	btRaycastVehicle::btVehicleTuning  m_tuning;
	gkGameObject                      *m_object;
	utArray<gkGameObject*>             m_wheelObjects;
	btRigidBody                       *m_chassis;
	
	gkScalar m_enginePower;
	gkScalar m_brakePower;
	gkScalar m_rearBrakeRatio;
	gkScalar m_maxSteering;
	
	gkScalar m_gaz;
	gkScalar m_brake;
	gkScalar m_steer;
	
	void createVehicle(void);
	void updateVehicle(void);
	
public:
	vdVehicle(gkScene *scene, const gkString &chassis, const gkScalar &power, const gkScalar &brakes, const gkScalar &rearBrakeRatio, const gkScalar & maxSteering);
	~vdVehicle();
	
	void addWheel(const gkString &name, gkScalar radius, gkVector3 connectionPoint, gkVector3 wheelDirection, 
						 gkVector3 wheelAxle, bool isFront, gkScalar restLength, gkScalar stiffness, gkScalar dampingRelax,
						 gkScalar dampingComp, gkScalar friction, gkScalar roll, gkScalar travelDist);

	void tick(gkScalar rate);
	
	void setTransfrom(const gkTransformState &v);
	
	void setGaz(gkScalar ratio);
	void setBrake(gkScalar ratio);
	void setSteer(gkScalar ratio);
	
	gkScalar getCurrentSpeedKmHour(void);
	gkScalar getVelocityEulerZ(void);
	
};

#endif // VDVEHICLE_H
