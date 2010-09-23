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
#include "gkKeyboardSensor.h"
#include "gkLogicManager.h"
#include "gkScene.h"



gkKeyDispatch::gkKeyDispatch()
{
}



gkKeyDispatch::~gkKeyDispatch()
{
}



void gkKeyDispatch::dispatch(void)
{
	// TODO need to sort.
	doDispatch(m_sensors);
}


gkKeyboardSensor::gkKeyboardSensor(gkGameObject* object, gkLogicLink* link, const gkString& name)
	:       gkLogicSensor(object, link, name), m_key(KC_NONE), m_mod0(KC_NONE), m_mod1(KC_NONE), m_allKeys(false)
{
	m_dispatchType = DIS_KEY;
	connect();
}


gkLogicBrick* gkKeyboardSensor::clone(gkLogicLink* link, gkGameObject* dest)
{
	gkKeyboardSensor* sens = new gkKeyboardSensor(*this);
	sens->cloneImpl(link, dest);
	return sens;
}


bool gkKeyboardSensor::query(void)
{
	gkKeyboard* key = gkWindowSystem::getSingleton().getKeyboard();
	if (m_allKeys)
		return key->key_count > 0;

	bool qres = key->isKeyDown((gkScanCode)m_key);
	if (m_mod0 != KC_NONE)
		qres = qres && key->isKeyDown((gkScanCode)m_mod0);
	if (m_mod1 != KC_NONE)
		qres = qres && key->isKeyDown((gkScanCode)m_mod1);

	return qres;
}
