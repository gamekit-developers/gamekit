/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Charlie C.

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
#include "gkRaySensor.h"
#include "gkLogicManager.h"
#include "gkLogicDispatcher.h"
#include "gkGameObject.h"
#include "gkPhysicsController.h"
#include "gkScene.h"
#include "gkRayTest.h"




gkRaySensor::gkRaySensor(gkGameObject* object, gkLogicLink* link, const gkString& name)
	:       gkLogicSensor(object, link, name), m_range(0.01), m_axis(-1),
                m_material(""), m_prop(""), m_xray(false)
{
	m_dispatchType = DIS_CONSTANT;
	connect();
}



gkLogicBrick* gkRaySensor::clone(gkLogicLink* link, gkGameObject* dest)
{
	gkRaySensor* sens = new gkRaySensor(*this);
	sens->cloneImpl(link, dest);
	return sens;
}



bool gkRaySensor::query(void)
{

	gkVector3 from, to, dir;
	bool result;
	gkRayTest test;
	
	from = m_object->getWorldPosition();
	
	switch (m_axis)
	{
	case RA_XPOS: {dir = gkVector3(m_range, 0, 0);  break;}
	case RA_YPOS: {dir = gkVector3(0, m_range, 0);  break;}
	case RA_ZPOS: {dir = gkVector3(0, 0, m_range);  break;}
	case RA_XNEG: {dir = gkVector3(-m_range, 0, 0); break;}
	case RA_YNEG: {dir = gkVector3(0, -m_range, 0); break;}
	case RA_ZNEG: {dir = gkVector3(0, 0, -m_range); break;}
	}
	
	dir = m_object->getWorldOrientation() * dir;
	to = from + dir;
	
	if(m_xray){
		xrayFilter xrf(m_object, m_prop, m_material);
		result = test.collides(from, to, xrf);
	}
	else
	{
		gkRayTest::gkRayTestFilter nmf;
		result = test.collides(from, to, nmf);
	}
	
	bool onlyActorTODO = false;
	// if x-ray, m_prop and m_material were already tested
	if (!m_xray && result){
		gkGameObject* hit = gkPhysicsController::castObject(test.getCollisionObject());
		result = hit && gkPhysicsController::sensorTest(hit, m_prop, m_material, onlyActorTODO);
	}
	
	return result;
}



