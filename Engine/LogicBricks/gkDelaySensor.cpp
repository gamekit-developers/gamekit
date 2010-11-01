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

#include "gkDelaySensor.h"
#include "gkLogicManager.h"


gkDelaySensor::gkDelaySensor(gkGameObject* object, gkLogicLink* link, const gkString& name)
	:   gkLogicSensor(object, link, name), 
		m_delay(0),
		m_duration(0),
		m_count(0),
		m_repeat(false)
{
	m_dispatchType = DIS_CONSTANT;
	connect();
}


gkLogicBrick* gkDelaySensor::clone(gkLogicLink* link, gkGameObject* dest)
{
	gkDelaySensor* sens = new gkDelaySensor(*this);
	sens->cloneImpl(link, dest);
	return sens;
}


bool gkDelaySensor::query(void)
{
	m_count += 1;
	if (m_count > m_delay + m_duration)
		if (m_repeat) m_count = 0;
		else return false;
	if (m_count <= m_delay)
		return false;
	if (m_duration == 0)
		return false;
	return true;
}
