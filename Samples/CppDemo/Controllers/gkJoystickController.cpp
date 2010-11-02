/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

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
#include "gkGameController.h"
#include "gkJoystickController.h"
#include "gkWindowSystem.h"
#include "gkGamePlayer.h"
#include "gkGameObject.h"
#include "gkEntity.h"


gkJoystickController::gkJoystickController(gkGamePlayer* player)
	:    m_player(player),
	     m_btn1Cache(0),
	     m_btn2Cache(0),
	     m_btn3Cache(0),
	     m_isBtn1(false),
	     m_isBtn2(false),
	     m_isBtn3(false)
{
	m_joystick = gkWindowSystem::getSingleton().getJoystick(0);
	GK_ASSERT(m_joystick);
}


gkJoystickController::~gkJoystickController()
{
}


bool gkJoystickController::hasInput(const gkGameController::InputCode& ic)
{
	switch (ic)
	{
	case IC_RUN:
		return !m_movement.isDead() && !m_movement.isInFactor(gkAppData::gkJoyWalkToRunTol);
	case IC_WALK:
		return !m_movement.isDead() && m_movement.isInFactor(gkAppData::gkJoyWalkToRunTol);
	case IC_BUTTON_0:
		return m_isBtn1;
	case IC_BUTTON_1:
		return m_isBtn2;
	case IC_BUTTON_2:
		return m_isBtn3;
	case IC_STOP:
		return m_movement.isDead();
	}
	return false;
}

void gkJoystickController::moveCamera(void)
{
	gkGamePlayer::Data& data = m_player->getData();

	gkScalar crAxUD = (m_camRot.m_normal.x * gkPih) * gkAppData::gkFixedTickDelta2;
	gkScalar crAxLR = (m_camRot.m_normal.y * gkPih) * gkAppData::gkFixedTickDelta2;


	data.m_xRot->pitch(gkRadian(crAxUD));

	if (m_camRot.isDeadUD())
	{
		// Auto rebound pitch.
		gkScalar cPitch = data.m_xRot->getRotation().x.valueRadians();

		cPitch = -gkAppData::gkJoyReboundFac * cPitch;
		data.m_xRot->pitch(gkDegree(cPitch));
	}

	if (!m_camRot.isDeadLR())
		data.m_zRot->roll(gkRadian(crAxLR));

	data.m_zRot->translate(
	    (data.m_physics->getPosition() - (data.m_zRot->getPosition() +
	                                      gkVector3(0, 0, -gkAppData::gkPlayerHeadZ))
	    ) * gkAppData::gkCameraTol
	);
}



void gkJoystickController::movePlayer(void)
{
	gkGamePlayer::Data& data = m_player->getData();


	gkScalar axRotLR = (m_movement.m_normal.y * gkPi);
	gkScalar axRotUD = (-m_movement.m_normal.x * gkPi);
	gkScalar axTan = -gkMath::ATan2(axRotLR, axRotUD).valueDegrees();


	gkScalar speed = 3.f;
	if (m_movement.isInFactor(gkAppData::gkJoyWalkToRunTol))
		speed /= 4.f;

	gkScalar axLinUD = speed * -m_movement.m_normal.x;
	gkScalar axLinLR = speed * m_movement.m_normal.y;
	gkVector3 linvel   = data.m_physics->getLinearVelocity();


	gkQuaternion curZRot = data.m_zRot->getOrientation();

	gkQuaternion aOri = gkEuler(0.f, 0.f, axTan).toQuaternion();

	aOri.normalise();

	gkVector3 m = curZRot * gkVector3(axLinLR, axLinUD, linvel.z);

	data.m_physics->setLinearVelocity(m);
	data.m_entity->setOrientation(aOri * curZRot);

}


bool gkJoystickController::isButtonDownCache(int btn, int& cache)
{
	bool result = m_joystick->isButtonDown(btn);
	if (result && cache == 0)
		cache = 1;
	else if (cache == 1 && result)
		result = false;
	else if (cache == 1 && !result)
		cache = 0;
	return result;
}



void gkJoystickController::updateInputState(void)
{
	m_camRot.m_absolute   = gkVector2(m_joystick->getAxisValue(0), m_joystick->getAxisValue(1));
	m_camRot.normalize();

	m_movement.m_absolute = gkVector2(m_joystick->getAxisValue(2), m_joystick->getAxisValue(3));
	m_movement.normalize();

	m_isBtn1 = isButtonDownCache(GK_JOY_BUTTON_1, m_btn1Cache);
	m_isBtn2 = isButtonDownCache(GK_JOY_BUTTON_2, m_btn2Cache);
	m_isBtn3 = isButtonDownCache(GK_JOY_BUTTON_3, m_btn3Cache);
}
