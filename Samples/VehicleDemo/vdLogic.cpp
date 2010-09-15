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

#include "vdLogic.h"
#include "vdBulletSerializeNode.h"

vdLogic::vdLogic(gkScene *scene)
		: m_scene(scene), m_tree(gkNodeManager::getSingleton().create()),
		m_camera(scene->getMainCamera()), m_upKeyNode(0), m_downKeyNode(0),
		m_leftKeyNode(0), m_rightKeyNode(0), m_spaceKeyNode(0), m_rKeyNode(0),
		m_cameraNode(0), m_dKeyNode(0), m_cKeyNode(0)
{
	createInput();
	createVehicle();
	createCamera();
	createDisplayProps();
	
	m_tree->solveOrder();
	m_camera->attachLogic(m_tree);
}

vdLogic::~vdLogic()
{
	delete m_vehicle;
}

void vdLogic::createInput()
{
	m_upKeyNode = m_tree->createNode<gkKeyNode>();
	m_upKeyNode->setKey(KC_UPARROWKEY);

	m_downKeyNode = m_tree->createNode<gkKeyNode>();
	m_downKeyNode->setKey(KC_DOWNARROWKEY);

	m_leftKeyNode = m_tree->createNode<gkKeyNode>();
	m_leftKeyNode->setKey(KC_LEFTARROWKEY);

	m_rightKeyNode = m_tree->createNode<gkKeyNode>();
	m_rightKeyNode->setKey(KC_RIGHTARROWKEY);

	m_spaceKeyNode = m_tree->createNode<gkKeyNode>();
	m_spaceKeyNode->setKey(KC_SPACEKEY);

	m_dKeyNode = m_tree->createNode<gkKeyNode>();
	m_dKeyNode->setKey(KC_DKEY);
	
	m_cKeyNode = m_tree->createNode<gkKeyNode>();
	m_cKeyNode->setKey(KC_CKEY);
	
	m_rKeyNode = m_tree->createNode<gkKeyNode>();
	m_rKeyNode->setKey(KC_RKEY);
}

void vdLogic::createVehicle()
{

	float power=170.0f; //Engine torque N/m
	float brake=30.0f;
	float brakeRatio=0.6f;
	float steer=0.69f;
	float steerTime=0.21f;
	
	float connectionHeight = 0.267;
	float sideOffsetF = 0.764;
	float sideOffsetR = 0.848;
	float frontOffest = 0.988;
	float rearOffset = 0.965;
	float radius = 0.370;
	
	float suspensionTravelCm = 49;
	float suspensionRestLength = .538;
	float suspensionStiffness = 22.0;
	float suspensionRelax = 0.1 * 2.0 * btSqrt(suspensionStiffness);
	float suspensionCompression = 0.3 * 2.0 * btSqrt(suspensionStiffness);
	float friction = 0.95f;
	float rollInfluence = 0.1;

	float diferencial = 3.5;
	float gearReverse = -3.38 * diferencial;
	float gear1 = 2.66 * diferencial;
	float gear2 = 2.14 * diferencial;
	float gear3 = 1.68 * diferencial;
	float gear4 = 1.37 * diferencial;
	float gear5 = 1.12 * diferencial;
	float shiftTime = 0.4;
	
	gkVector3 wheelDirection(0,0,-1);
	gkVector3 wheelAxle(1,0,0);
	gkVector3 connectionPoint(0,0,0);
	
	bool isFront = true;
	
	
	
	
	gkBlendFile* pBlendFileCar = gkBlendLoader::getSingleton().loadFile(gkUtils::getFile("buggy.blend"));
	
	gkGroupManager& groups = gkGroupManager::getSingleton();
	gkGameObjectGroup* carGroup = (gkGameObjectGroup*)groups.getByName("CarGroup");
	
	groups.attachGroupToScene(m_scene, carGroup);
	
	gkGameObjectGroup::Objects::Iterator iter =  carGroup->getObjects().iterator();
	while (iter.hasMoreElements())
	{
		gkGameObject *gobj = iter.getNext().second;

		m_scene->addObject(gobj);

		gobj->createInstance();
	}
	
	gkGameObject* objCol = m_scene->getObject("ChassisCollision");
	gkGameObject* objChassis = m_scene->getObject("Chassis");
	objCol->setTransform(gkTransformState(gkVector3(-66.5, 295, -8.5), gkEuler(0,0,180).toQuaternion()));
	objChassis->setParent(objCol);
	m_scene->getObject("WishBone.F.R")->setParent(objChassis);
	m_scene->getObject("WishBone.F.L")->setParent(objChassis);
	m_scene->getObject("WishBone.R.R")->setParent(objChassis);
	m_scene->getObject("WishBone.R.L")->setParent(objChassis);
	
	m_vehicle = new vdVehicle(m_scene, "ChassisCollision", power, brake, brakeRatio, steer);
	
	// wheels
	connectionPoint = gkVector3(-sideOffsetF,frontOffest, connectionHeight);
	m_vehicle->addWheel("TireFL", radius, connectionPoint, wheelDirection, wheelAxle, isFront,
						suspensionRestLength, suspensionStiffness, suspensionRelax, suspensionCompression,
						friction, rollInfluence, suspensionTravelCm);
	
	connectionPoint = gkVector3(sideOffsetF,frontOffest, connectionHeight);
	m_vehicle->addWheel("TireFR", radius, connectionPoint, wheelDirection, wheelAxle, isFront,
						suspensionRestLength, suspensionStiffness, suspensionRelax, suspensionCompression,
						friction, rollInfluence, suspensionTravelCm);
	
	isFront=false;
	connectionPoint = gkVector3(-sideOffsetR,-rearOffset, connectionHeight);
	m_vehicle->addWheel("TireRL", radius, connectionPoint, wheelDirection, wheelAxle, isFront,
						suspensionRestLength, suspensionStiffness, suspensionRelax, suspensionCompression,
						friction, rollInfluence, suspensionTravelCm);
	
	connectionPoint = gkVector3(sideOffsetR,-rearOffset, connectionHeight);
	m_vehicle->addWheel("TireRR", radius, connectionPoint, wheelDirection, wheelAxle, isFront,
						suspensionRestLength, suspensionStiffness, suspensionRelax, suspensionCompression,
						friction, rollInfluence, suspensionTravelCm);
	
	// gear box
	vdGearBox *box = new vdGearBox(true, 5, shiftTime, gearReverse);
	box->setGear(1, gear1, 2000, 3000);
	box->setGear(2, gear2, 2000, 3800);
	box->setGear(3, gear3, 2500, 4000);
	box->setGear(4, gear4, 2600, 4100);
	box->setGear(5, gear5, 2600, 4200);
	
	m_vehicle->setGearBox(box);
	
	
	//simple node setup for logic
	m_vehicleNode = m_tree->createNode<vdVehicleNode>();
	
	m_vehicleNode->setVehicle(m_vehicle);
	m_vehicleNode->getFRONT()->link(m_upKeyNode->getIS_DOWN());
	m_vehicleNode->getREAR()->link(m_downKeyNode->getIS_DOWN());
	m_vehicleNode->getLEFT()->link(m_leftKeyNode->getIS_DOWN());
	m_vehicleNode->getRIGHT()->link(m_rightKeyNode->getIS_DOWN());
	m_vehicleNode->getSTEER_TIME()->setValue(steerTime);
	m_vehicleNode->getHAND_BRAKE()->link(m_spaceKeyNode->getIS_DOWN());
	m_vehicleNode->getGEAR_UP()->link(m_dKeyNode->getIS_DOWN());
	m_vehicleNode->getGEAR_DOWN()->link(m_cKeyNode->getIS_DOWN());
}

void vdLogic::createCamera()
{
	gkObjectNode* centerObj = m_tree->createNode<gkObjectNode>();
	centerObj->setOtherObject("ChassisCollision");
	
	m_cameraNode = m_tree->createNode<gkCameraNode>();
	
	gkQuaternionToEulerNode *quatNode = m_tree->createNode<gkQuaternionToEulerNode>();
	gkVectorDecomposeNode *vecNode = m_tree->createNode<gkVectorDecomposeNode>();
	
	gkMathNode<gkScalar, MTH_SUBTRACT> *mathNode1 = m_tree->createNode<gkMathNode<gkScalar, MTH_SUBTRACT> >();
	gkMathNode<gkScalar, MTH_DIVIDE> *mathNode2 = m_tree->createNode<gkMathNode<gkScalar, MTH_DIVIDE> >();
	
	gkIfNode<gkScalar, CMP_GREATER> *ifNode1 = m_tree->createNode<gkIfNode<gkScalar, CMP_GREATER> >();
	gkIfNode<gkScalar, CMP_LESS> *ifNode2 = m_tree->createNode<gkIfNode<gkScalar, CMP_LESS> >();
	
	gkMathNode<gkScalar, MTH_ADD> *mathNode3 = m_tree->createNode<gkMathNode<gkScalar, MTH_ADD> >();
	gkMathNode<gkScalar, MTH_ADD> *mathNode4 = m_tree->createNode<gkMathNode<gkScalar, MTH_ADD> >();
	
	gkMultiplexerNode<gkScalar> *mpxNode1 = m_tree->createNode<gkMultiplexerNode<gkScalar> >();
	gkMultiplexerNode<gkScalar> *mpxNode2 = m_tree->createNode<gkMultiplexerNode<gkScalar> >();
	
	quatNode->getQUAT()->link(m_cameraNode->getCURRENT_ROLL());
	
	vecNode->getVEC()->link(quatNode->getEUL());
	
	// Diff between the actual roll and the one we want
	mathNode1->getA()->link(vecNode->getZ());
	mathNode1->getB()->link(m_vehicleNode->getZROT());
	
	// Always go the shortest way
	
	ifNode1->getA()->link(mathNode1->getRESULT());
	ifNode1->getB()->setValue(180);
	
	mathNode3->getA()->link(mathNode1->getRESULT());
	mathNode3->getB()->setValue(-360);
	
	mpxNode1->getSEL()->link(ifNode1->getIS_TRUE());
	mpxNode1->getINPUT_TRUE()->link(mathNode3->getRESULT());
	mpxNode1->getINPUT_FALSE()->link(mathNode1->getRESULT());
	
	ifNode2->getA()->link(mathNode1->getRESULT());
	ifNode2->getB()->setValue(-180);
	
	mathNode4->getA()->link(mathNode1->getRESULT());
	mathNode4->getB()->setValue(360);
	
	mpxNode2->getSEL()->link(ifNode2->getIS_TRUE());
	mpxNode2->getINPUT_TRUE()->link(mathNode4->getRESULT());
	mpxNode2->getINPUT_FALSE()->link(mpxNode1->getOUTPUT());
	
	// Damping
	// Ideally this should be proportional to time step
	mathNode2->getA()->link(mpxNode2->getOUTPUT());
	mathNode2->getB()->setValue(50);

	m_cameraNode->getCENTER_OBJ()->setValue(m_scene->getObject("ChassisCollision"));
	m_cameraNode->getCENTER_POSITION()->link(centerObj->getOUT_POSITION());
	m_cameraNode->getREL_X()->link(mathNode2->getRESULT());
	m_cameraNode->getAVOID_BLOCKING()->setValue(true);
	m_cameraNode->getTARGET()->setValue(m_camera);
	m_cameraNode->getMIN_Z()->setValue(4.5f);
	m_cameraNode->getMAX_Z()->setValue(8);
	m_cameraNode->getINITIAL_ROLL()->setValue(0);
	m_cameraNode->getMAX_ROLL()->setValue(360);
	m_cameraNode->getMIN_ROLL()->setValue(-360);
	m_cameraNode->getINITIAL_PITCH()->setValue(76);
	m_cameraNode->getKEEP_DISTANCE()->setValue(true);
}

void vdLogic::createDisplayProps(void)
{
	gkVariableGetSetNode<int> *kmhVarNode = m_tree->createNode<gkVariableGetSetNode<int> >();
	kmhVarNode->setName("Km/h");
	kmhVarNode->setDebug(true);
	kmhVarNode->getSET()->link(m_vehicleNode->getKMH());
	
	gkVariableGetSetNode<int> *rpmVarNode = m_tree->createNode<gkVariableGetSetNode<int> >();
	rpmVarNode->setName("Rpm");
	rpmVarNode->setDebug(true);
	rpmVarNode->getSET()->link(m_vehicleNode->getRPM());

	gkVariableGetSetNode<int> *gearVarNode = m_tree->createNode<gkVariableGetSetNode<int> >();
	gearVarNode->setName("Gear");
	gearVarNode->setDebug(true);
	gearVarNode->getSET()->link(m_vehicleNode->getGEAR());
	
	vdBulletSerializeNode *bulletNode = m_tree->createNode<vdBulletSerializeNode>();
	bulletNode->setFileName("vehicle_demo.bullet");
	bulletNode->getSERIALIZE()->link(m_rKeyNode->getIS_DOWN());

}

void vdLogic::tick(gkScalar rate)
{
	m_vehicle->tick(rate);
}

