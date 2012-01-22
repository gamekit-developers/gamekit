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
#include "gkDefaultController.h"
#include "gkWindowSystem.h"
#include "gkGamePlayer.h"
#include "gkGameObject.h"
#include "gkEntity.h"

gkDefaultController::gkDefaultController(gkGamePlayer* player) 
{
	m_player = player;
	m_keyboard = gkWindowSystem::getSingleton().getKeyboard();
	m_mouse = gkWindowSystem::getSingleton().getMouse();


	bindScanCode(IC_RUN,      KC_WKEY);
	bindScanCode(IC_WALK,     KC_NONE);
	bindScanCode(IC_BUTTON_0, gkMouse::Left);
	bindScanCode(IC_BUTTON_1, KC_SPACEKEY);
	bindScanCode(IC_BUTTON_2, gkMouse::Right);
	bindScanCode(IC_STOP,     KC_NONE);
}



gkDefaultController::~gkDefaultController()
{
}

bool gkDefaultController::hasInput(const gkGameController::InputCode& ic)
{
	switch (ic)
	{
	case IC_WALK:
		return false;
	case IC_BUTTON_1:
	case IC_RUN:
		return m_keyboard->isKeyDown((gkScanCode)m_bindings[ic]);
	case IC_BUTTON_0:
	case IC_BUTTON_2:
		return m_mouse->isButtonDown(m_bindings[ic]);
	case IC_STOP:
		return !m_keyboard->isKeyDown((gkScanCode)m_bindings[IC_RUN]);
	}
	return false;
}

void gkDefaultController::moveCamera(void)
{
	gkGamePlayer::Data& data = m_player->getData();


	if (m_mouse->mouseMoved())
	{
		const gkScalar tick = gkAppData::gkFixedTickDelta * .25f;

		if (m_mouse->relative.x!= 0.f)
			data.m_zRot->roll(-gkRadian(m_mouse->relative.x * tick));
		if (m_mouse->relative.y!= 0.f)
			data.m_xRot->pitch(-gkRadian(m_mouse->relative.y * tick));
	}
	else
	{
		// Auto rebound pitch.
		gkScalar cPitch = data.m_xRot->getRotation().x.valueRadians();

		cPitch = -gkAppData::gkJoyReboundFac * cPitch;
		data.m_xRot->pitch(gkDegree(cPitch));

	}

	data.m_zRot->translate(
	    (data.m_physics->getPosition() - (data.m_zRot->getPosition() +
	                                      gkVector3(0, 0, -gkAppData::gkPlayerHeadZ))
	    ) * gkAppData::gkCameraTol
	);

}
void gkDefaultController::movePlayer(void)
{
	gkGamePlayer::Data& data = m_player->getData();


	gkScalar speed = 3.f;
	
	if(data.m_zRot)
	{
		gkVector3 movement = data.m_zRot->getOrientation()  * gkVector3(0, speed, 0);
		
		data.m_entity->setOrientation(gkEuler(0, 0, data.m_zRot->getRotation().z.valueDegrees()));
		
		
		movement.z = data.m_physics->getLinearVelocity().z;
		data.m_physics->setLinearVelocity(movement);
	}
}




void gkDefaultController::bindScanCode(const InputCode& ic, const int &code)
{
	int iic = (int)ic;
	if (iic >= 0 && iic < IC_MAX)
		m_bindings[ic] = code;
}
