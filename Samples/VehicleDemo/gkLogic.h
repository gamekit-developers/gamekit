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

#ifndef GKLOGIC_H
#define GKLOGIC_H

#include "OgreKit.h"
#include "gkVehicle.h"
#include "gkVehicleNode.h"

class gkLogic
{
public:
	gkLogic(gkScene* scene);
	~gkLogic();

	void tick(gkScalar rate);

private:

	void createInput();
	void createVehicle();
	void createCamera();
	void createDisplayProps();

public:

	gkScene* m_scene;

	gkGameObject* m_camera;

	gkLogicTree* m_tree;

	gkKeyNode* m_upKeyNode;
	gkKeyNode* m_downKeyNode;
	gkKeyNode* m_leftKeyNode;
	gkKeyNode* m_rightKeyNode;
	gkKeyNode* m_spaceKeyNode;
	gkKeyNode* m_dKeyNode;
	gkKeyNode* m_cKeyNode;
	gkKeyNode* m_rKeyNode;

	gkCameraNode* m_cameraNode;

	gkVehicleNode* m_vehicleNode;

private:
	gkVehicle* m_vehicle;
};

#endif // GKLOGIC_H
