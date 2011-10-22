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
#include "gkLogicOpController.h"
#include "gkLogicActuator.h"
#include "gkLogicSensor.h"
#include "gkLogicManager.h"
#include "gkLogicLink.h"



gkLogicOpController::gkLogicOpController(gkGameObject* object, gkLogicLink* link, const gkString& name)
	:       gkLogicController(object, link, name), m_op(OP_NILL), m_isInverter(false)
{
}




gkLogicBrick* gkLogicOpController::clone(gkLogicLink* link, gkGameObject* dest)
{
	gkLogicOpController* cont = new gkLogicOpController(*this);
	cont->cloneImpl(link, dest);
	return cont;
}



void gkLogicOpController::execute(void)
{
	if (m_actuators.empty() || m_sensors.empty())
	{
		// undefined
		return;
	}

	bool doUpdate = false;
	bool seed = true, last = false, pos = false;

	switch (m_op)
	{
	case OP_NOR:
	case OP_OR:
		{
			gkSensorIterator it(m_sensors);
			while (it.hasMoreElements())
			{
				gkLogicSensor* sens = it.getNext();

				pos = sens->isPositive();
				if (pos)
					doUpdate = true;
				if (m_op == OP_NOR)
					m_isInverter = true;

				if (doUpdate)
					break;
			}

			if (m_isInverter)
				doUpdate = !doUpdate;
		}
		break;
	case OP_XNOR:
	case OP_XOR:
		{
			gkSensorIterator it(m_sensors);
			while (it.hasMoreElements())
			{

				gkLogicSensor* sens = it.getNext();
				seed = sens->isPositive();

				if (seed && last)
				{
					doUpdate = false;
					break;
				}
				else if (seed) doUpdate = true;

				if (!last && seed)
					last = true;

				if (m_op == OP_XNOR && seed)
					m_isInverter = true;
			}
			if (m_isInverter)
				doUpdate = !doUpdate;
		}
		break;
	case OP_NAND:
	case OP_AND:
		{
			gkSensorIterator it(m_sensors);
			while (it.hasMoreElements())
			{
				gkLogicSensor* sens = it.getNext();

				pos = sens->isPositive();
				if (seed)
				{
					seed = false;
					doUpdate = pos;
				}
				else
					doUpdate = doUpdate && pos;

				if (m_op == OP_NAND)
					m_isInverter = true;

			}
			if (m_isInverter)
				doUpdate = !doUpdate;
		}
		break;
	}

	gkLogicManager& mgr = gkLogicManager::getSingleton();
	gkActuatorIterator it(m_actuators);
	while (it.hasMoreElements())
	{
		gkLogicActuator* act = it.getNext();
		mgr.push(this, act, doUpdate);
	}
}
