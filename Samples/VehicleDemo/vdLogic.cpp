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

vdLogic::vdLogic(gkScene *scene)
		: m_scene(scene), m_tree(gkNodeManager::getSingleton().create()),
		m_camera(scene->getMainCamera()), m_upKeyNode(0), m_downKeyNode(0),
		m_leftKeyNode(0), m_rightKeyNode(0), m_spaceKeyNode(0), m_rKeyNode(0),
		m_cameraNode(0)
{
	createInput();
	createVehicle();
	createCamera();
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

	m_rKeyNode = m_tree->createNode<gkKeyNode>();
	m_rKeyNode->setKey(KC_RKEY);
}

void vdLogic::createVehicle()
{

	float power=2000.0f;
	float brake=60.0f;
	float brakeRatio=0.6f;
	float steer=0.3f;
	
	float connectionHeight = 0.4;
	float sideOffset = 0.73;
	float frontOffest = 1.211;
	float rearOffset = 1.282;
	float radius = 0.357;
	
	float suspensionRestLength = 0.53;
	float suspensionTravelCm = 16;
	float suspensionStiffness= 50.0;
	float suspensionRelax = 0.3 * 2.0 * btSqrt(suspensionStiffness);
	float suspensionCompression = 0.1 * 2.0 * btSqrt(suspensionStiffness);
	float friction = 1.3;
	float rollInfluence = 0.1;

	gkVector3 wheelDirection(0,0,-1);
	gkVector3 wheelAxle(1,0,0);
	gkVector3 connectionPoint(0,0,0);
	
	bool isFront = true;
	
	m_vehicle = new vdVehicle(m_scene, "MiniG", power, brake, brakeRatio, steer);
	
	connectionPoint = gkVector3(-sideOffset,frontOffest, connectionHeight);
	m_vehicle->addWheel("wheel_MiniG_FT.L", radius, connectionPoint, wheelDirection, wheelAxle, isFront,
						suspensionRestLength, suspensionStiffness, suspensionRelax, suspensionCompression,
						friction, rollInfluence, suspensionTravelCm);
	
	connectionPoint = gkVector3(sideOffset,frontOffest, connectionHeight);
	m_vehicle->addWheel("wheel_MiniG_FT.R", radius, connectionPoint, wheelDirection, wheelAxle, isFront,
						suspensionRestLength, suspensionStiffness, suspensionRelax, suspensionCompression,
						friction, rollInfluence, suspensionTravelCm);
	
	isFront=false;
	connectionPoint = gkVector3(-sideOffset,-rearOffset, connectionHeight);
	m_vehicle->addWheel("wheel_MiniG_RR.L", radius, connectionPoint, wheelDirection, wheelAxle, isFront,
						suspensionRestLength, suspensionStiffness, suspensionRelax, suspensionCompression,
						friction, rollInfluence, suspensionTravelCm);
	
	connectionPoint = gkVector3(sideOffset,-rearOffset, connectionHeight);
	m_vehicle->addWheel("wheel_MiniG_RR.R", radius, connectionPoint, wheelDirection, wheelAxle, isFront,
						suspensionRestLength, suspensionStiffness, suspensionRelax, suspensionCompression,
						friction, rollInfluence, suspensionTravelCm);
	
	//simple node setup for logic
	m_vehicleNode = m_tree->createNode<vdVehicleNode>();
	
	m_vehicleNode->setVehicle(m_vehicle);
	m_vehicleNode->getFRONT()->link(m_upKeyNode->getIS_DOWN());
	m_vehicleNode->getREAR()->link(m_downKeyNode->getIS_DOWN());
	m_vehicleNode->getLEFT()->link(m_leftKeyNode->getIS_DOWN());
	m_vehicleNode->getRIGHT()->link(m_rightKeyNode->getIS_DOWN());
}

void vdLogic::createCamera()
{
	gkObjectNode* centerObj = m_tree->createNode<gkObjectNode>();
	centerObj->setOtherObject("MiniG");
	
	m_cameraNode = m_tree->createNode<gkCameraNode>();
	
	gkMathNode<gkScalar, MTH_MULTIPLY> *mathNode1 = m_tree->createNode<gkMathNode<gkScalar, MTH_MULTIPLY> >();
	gkMathNode<gkScalar, MTH_SUBTRACT> *mathNode2 = m_tree->createNode<gkMathNode<gkScalar, MTH_SUBTRACT> >();
	gkMathNode<gkScalar, MTH_DIVIDE> *mathNode3 = m_tree->createNode<gkMathNode<gkScalar, MTH_DIVIDE> >();

	mathNode1->getA()->link(m_vehicleNode->getZROT());
	mathNode1->getB()->setValue(-1);
	
	mathNode2->getA()->link(m_cameraNode->getCURRENT_ROLL());
	mathNode2->getB()->link(mathNode1->getRESULT());
	
	// I would like to damp here for smoother camera movement
	// but it does not work properly with the camera node
	mathNode3->getA()->link(mathNode2->getRESULT());
	mathNode3->getB()->setValue(1);

	m_cameraNode->getCENTER_OBJ()->setValue(m_scene->getObject("MiniG"));
	m_cameraNode->getCENTER_POSITION()->link(centerObj->getOUT_POSITION());
	m_cameraNode->getREL_X()->link(mathNode3->getRESULT());
	m_cameraNode->getAVOID_BLOCKING()->setValue(true);
	m_cameraNode->getTARGET()->setValue(m_camera);
	m_cameraNode->getMIN_Z()->setValue(2.0f);
	m_cameraNode->getMAX_Z()->setValue(8);
	m_cameraNode->getINITIAL_ROLL()->setValue(0);
	m_cameraNode->getMAX_ROLL()->setValue(360);
	m_cameraNode->getMIN_ROLL()->setValue(-360);
	m_cameraNode->getINITIAL_PITCH()->setValue(75);
	m_cameraNode->getKEEP_DISTANCE()->setValue(true);
}

void vdLogic::tick(gkScalar rate)
{
	m_vehicle->tick(rate);
}

