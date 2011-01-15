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

#include "gkBuggy.h"

gkBuggy::gkBuggy(gkScene* scene)
		: gkVehicle(scene)
{

}

gkBuggy::~gkBuggy()
{

}

void gkBuggy::load(void)
{
	gkBlendFile* pBlendFileCar = gkBlendLoader::getSingleton().loadFile(gkUtils::getFile(GK_RESOURCE_BUGGY_FILE), "", VEHICLE_RESOURCE_GROUP);

	gkScene* carScene = pBlendFileCar->getMainScene();
	gkSceneManager::getSingleton().copyObjects(carScene, m_scene);
	gkGameObject* objCol = m_scene->getObject(GK_RESOURCE_BUGGY_PHYSOBJ);

#ifdef _DEBUG
	gkGameObject* objChassis = m_scene->getObject(GK_RESOURCE_BUGGY_CHASSIS);
	gkGameObject* o = m_scene->getObject("Circle.022");
	GK_ASSERT(o && o->getParent() && o->getParent() == objChassis);
#endif
	
	// general
	setDriveTrain(gkVehicle::DT_ALLWHEEL);
	setChassisObject(objCol);
	setEngineTorque(330);
	setBrakePower(30);
	setRearBrakeRatio(0.6);
	setMaxSteeringAngle(0.23);
	setRuptorRpm(6000);
	
	// wheels
	float connectionHeight = 0.267;
	float sideOffsetF = 0.764;
	float sideOffsetR = 0.848;
	float frontOffest = 0.988;
	float rearOffset = 0.965;
	float radius = 0.370;

	float suspensionTravelCm = 40;
	float suspensionRestLength = .4;
	float suspensionStiffness = 22.0;
	float suspensionRelax = 0.6 * 2.0 * btSqrt(suspensionStiffness);
	float suspensionCompression = 0.6 * 2.0 * btSqrt(suspensionStiffness);
	float friction = 2.0f;
	float rollInfluence = 0.1;


	gkVector3 wheelDirection(0, 0, -1);
	gkVector3 wheelAxle(1, 0, 0);
	gkVector3 connectionPoint(0, 0, 0);

	bool isFront = true;
	gkGameObject* wheel;

	wheel = m_scene->getObject(GK_RESOURCE_BUGGY_WHELLFL);
	connectionPoint = gkVector3(-sideOffsetF, frontOffest, connectionHeight);
	addWheel(wheel, radius, connectionPoint, wheelDirection, wheelAxle, isFront,
	                    suspensionRestLength, suspensionStiffness, suspensionRelax, suspensionCompression,
	                    friction, rollInfluence, suspensionTravelCm);

	wheel = m_scene->getObject(GK_RESOURCE_BUGGY_WHELLFR);
	connectionPoint = gkVector3(sideOffsetF, frontOffest, connectionHeight);
	addWheel(wheel, radius, connectionPoint, wheelDirection, wheelAxle, isFront,
	                    suspensionRestLength, suspensionStiffness, suspensionRelax, suspensionCompression,
	                    friction, rollInfluence, suspensionTravelCm);

	isFront = false;
	wheel = m_scene->getObject(GK_RESOURCE_BUGGY_WHELLRL);
	connectionPoint = gkVector3(-sideOffsetR, -rearOffset, connectionHeight);
	addWheel(wheel, radius, connectionPoint, wheelDirection, wheelAxle, isFront,
	                    suspensionRestLength, suspensionStiffness, suspensionRelax, suspensionCompression,
	                    friction, rollInfluence, suspensionTravelCm);

	wheel = m_scene->getObject(GK_RESOURCE_BUGGY_WHELLRR);
	connectionPoint = gkVector3(sideOffsetR, -rearOffset, connectionHeight);
	addWheel(wheel, radius, connectionPoint, wheelDirection, wheelAxle, isFront,
	                    suspensionRestLength, suspensionStiffness, suspensionRelax, suspensionCompression,
	                    friction, rollInfluence, suspensionTravelCm);

	// gear box
	float diferencial = 3.5;
	float gearReverse = -3.38 * diferencial;
	float gear1 = 2.66 * diferencial;
	float gear2 = 2.14 * diferencial;
	float gear3 = 1.68 * diferencial;
	float gear4 = 1.37 * diferencial;
	float gear5 = 1.12 * diferencial;
	float shiftTime = 0.21;
	
	gkGearBox* box = new gkGearBox(true, 5, shiftTime, gearReverse);
	box->setGearProperties(1, gear1, 2000, 3000);
	box->setGearProperties(2, gear2, 2000, 3800);
	box->setGearProperties(3, gear3, 2500, 4000);
	box->setGearProperties(4, gear4, 2600, 4100);
	box->setGearProperties(5, gear5, 2600, 4200);

	setGearBox(box);
}

