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
#include "gkLogicDispatcher.h"
#include "gkLogicSensor.h"
#include "gkGameObject.h"





void gkAbstractDispatcher::doDispatch(SensorList& senslist)
{
	if (!senslist.empty())
	{
		SensorList::Iterator it = senslist.iterator();
		while (it.hasMoreElements())
		{
			gkLogicSensor*   sens = it.getNext();
			gkGameObject*    obj = sens->getObject();

			if (obj && obj->isInstanced())
				sens->execute();
		}
	}
}



void gkAbstractDispatcher::reset(void)
{
	if (!m_sensors.empty())
	{
		gkAbstractDispatcher::SensorList::Iterator it = m_sensors.iterator();
		while (it.hasMoreElements())
			it.getNext()->reset();
	}
}


void gkAbstractDispatcher::sort(void)
{
	if (!m_sensors.empty())
	{
		gkAbstractDispatcher::SensorList::Iterator it = m_sensors.iterator();
		while (it.hasMoreElements())
			it.getNext()->sort();
	}
}


void gkConstantDispatch::dispatch(void)
{
	doDispatch(m_sensors);
}
