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
		:	m_scene(scene), m_tree(gkNodeManager::getSingleton().create()), m_camera(scene->getMainCamera())
{
	createInput();
	createVehicle();
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
	m_vehicle = new vdVehicle(m_scene);
	
	m_vehicleNode = m_tree->createNode<vdVehicleNode>();
	m_vehicleNode->setVehicle(m_vehicle);
	m_vehicleNode->getFRONT()->link(m_upKeyNode->getIS_DOWN());
	m_vehicleNode->getREAR()->link(m_downKeyNode->getIS_DOWN());
	m_vehicleNode->getLEFT()->link(m_leftKeyNode->getIS_DOWN());
	m_vehicleNode->getRIGHT()->link(m_rightKeyNode->getIS_DOWN());
}

void vdLogic::tick(gkScalar rate)
{
	m_vehicle->tick(rate);
}

