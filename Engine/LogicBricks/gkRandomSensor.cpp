/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

    Contributor(s): nonw yet.
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

#include "gkRandomSensor.h"
#include "gkLogicManager.h"
#include "gkLogicDispatcher.h"


gkRandomSensor::gkRandomSensor(gkGameObject* object, gkLogicLink* link, const gkString& name)
	:	gkLogicSensor(object, link, name),
		m_seed(0),
		m_current(0),
		m_count(0)
{
	m_randGen = new utRandomNumberGenerator(0);
	m_dispatchType = DIS_CONSTANT;
	connect();
}


gkRandomSensor::~gkRandomSensor()
{
	delete m_randGen;
}


gkLogicBrick* gkRandomSensor::clone(gkLogicLink* link, gkGameObject* dest)
{
	gkRandomSensor* sens = new gkRandomSensor(*this);
	sens->cloneImpl(link, dest);
	sens->m_randGen = new utRandomNumberGenerator(m_seed);
	return sens;
}


void gkRandomSensor::setSeed(UTuint32 v)
{
	m_seed = v;
	m_randGen->setSeed(m_seed);
	m_current = m_randGen->rand32();
	m_count = 0;
}


bool gkRandomSensor::query(void)
{
	m_count = m_count + 1;
	if (m_count > 32)
	{
		m_current = m_randGen->rand32();
		m_count = 1;
	}
	return (m_current >> (m_count - 1)) & 0x1;
}
