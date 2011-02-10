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

#include "gkLogic.h"
#include "gkBulletSerializeNode.h"
#include "gkBuggy.h"

gkLogic::gkLogic(gkScene* scene)
	: m_scene(scene), m_tree(0),
	  m_camera(scene->getMainCamera()), m_upKeyNode(0), m_downKeyNode(0),
	  m_leftKeyNode(0), m_rightKeyNode(0), m_spaceKeyNode(0), m_rKeyNode(0),
	  m_cameraNode(0), m_dKeyNode(0), m_cKeyNode(0)
{
	GK_ASSERT(m_scene);
	m_tree = gkNodeManager::getSingleton().createLogicTree(m_scene->getGroupName());

	createInput();
	createVehicle();
	createCamera();
	createDisplayProps();

	m_tree->solveOrder();
	m_camera->attachLogic(m_tree);
}

gkLogic::~gkLogic()
{
	delete m_vehicle;
}

void gkLogic::createInput()
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

void gkLogic::createVehicle()
{

	m_vehicle = new gkBuggy(m_scene);
	m_vehicle->load();
	m_vehicle->createVehicle();
	// Move to start line
	m_vehicle->setTransform(gkTransformState(gkVector3(-66.5, 295, -8.5), gkEuler(0, 0, 180).toQuaternion()));


	float steerTime = 0.15f;

	//simple node setup for logic
	m_vehicleNode = m_tree->createNode<gkVehicleNode>();

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

void gkLogic::createCamera()
{
	gkObjectNode* centerObj = m_tree->createNode<gkObjectNode>();
	centerObj->setOtherObject("ChassisCollision");

	m_cameraNode = m_tree->createNode<gkCameraNode>();

	gkQuaternionToEulerNode* quatNode = m_tree->createNode<gkQuaternionToEulerNode>();
	gkVectorDecomposeNode* vecNode = m_tree->createNode<gkVectorDecomposeNode>();

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

void gkLogic::createDisplayProps(void)
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

	gkBulletSerializeNode* bulletNode = m_tree->createNode<gkBulletSerializeNode>();
	bulletNode->setFileName("vehicle_demo.bullet");
	bulletNode->getSERIALIZE()->link(m_rKeyNode->getIS_DOWN());

}

void gkLogic::tick(gkScalar rate)
{
	m_vehicle->tick(rate);
}
