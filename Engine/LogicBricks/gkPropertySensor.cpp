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
#include "gkPropertySensor.h"
#include "gkLogicManager.h"
#include "gkLogicDispatcher.h"
#include "gkGameObject.h"



gkPropertySensor::gkPropertySensor(gkGameObject* object, gkLogicLink* link, const gkString& name)
	:   gkLogicSensor(object, link, name), m_old(), m_cur(0), m_type(-1), m_propName(""), m_propVal(), m_propMax(),
	    m_init(false), m_change(false)

{
	m_dispatchType = DIS_CONSTANT;
	connect();
}


gkLogicBrick* gkPropertySensor::clone(gkLogicLink* link, gkGameObject* dest)
{
	gkPropertySensor* sens = new gkPropertySensor(*this);
	sens->m_cur      = 0;
	sens->m_init     = false;
	sens->cloneImpl(link, dest);
	return sens;
}



bool gkPropertySensor::query(void)
{
	if (!m_init)
	{
		m_init = true;


		if (m_object->hasVariable(m_propName))
		{
			m_cur = m_object->getVariable(m_propName);
			m_old.setValue(m_cur->getType(), m_propVal);
			m_change = m_change != ((*m_cur) != (m_old));

			if (m_type == PS_INTERVAL)
				m_test.setValue(m_cur->getType(), m_propMax);
		}
		else
		{
			m_suspend = true;
			return false;
		}
	}

	if (m_cur)
	{
		switch (m_type)
		{
		case PS_EQUAL:
			return (*m_cur) == (m_old);
		case PS_NEQUAL:
			return (*m_cur) != (m_old);
		case PS_CHANGED:
			if (m_change != ((*m_cur) != (m_old)))
			{
				m_old.assign(*m_cur);
				m_change = !m_change;
				return true;
			}
			break;
		case PS_INTERVAL:
			return (*m_cur) >= (m_old) && (*m_cur) <= (m_test);
		}
	}
	return false;
}
