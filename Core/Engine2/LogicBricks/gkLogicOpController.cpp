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


gkLogicOpController::gkLogicOpController(gkGameObject *object, gkLogicLink *link, const gkString &name)
:       gkLogicController(object, link, name), m_op(OP_NILL)
{
}


void gkLogicOpController::relay(void)
{
    if (m_actuators.empty()) {
        // undefined
        return;
    }

    if ((m_stateMask & m_link->getState()) == 0)
        return;

    bool execAct = false;


    if (!m_sensors.empty()) {

        bool fsr = true, lsr = false;

        switch (m_op) {
        case OP_NOR:
        case OP_OR: {
                utListIterator<Sensors> it(m_sensors);
                while (it.hasMoreElements()) {

                    gkLogicSensor *sens = it.getNext();
                    if (fsr) {
                        fsr = false;
                        execAct = sens->isPositive();
                    } else
                        execAct = execAct || sens->isPositive();

                    if (m_op == OP_NOR)
                        sens->setNegative(true);

                    if (execAct) break;
                }
                if (m_op == OP_NOR)
                    execAct = !execAct;
            }
            break;
        case OP_XNOR:
        case OP_XOR: {
                utListIterator<Sensors> it(m_sensors);
                while (it.hasMoreElements()) {

                    gkLogicSensor *sens = it.getNext();
                    fsr = sens->isPositive();

                    if (fsr && lsr) {
                        execAct = false;
                        break;
                    } else if (fsr) execAct = true;

                    if (!lsr && fsr)
                        lsr = true;

                    if (m_op == OP_XNOR) 
                        sens->setNegative(true);
                }
                if (m_op == OP_XNOR)
                    execAct = !execAct;
            }
            break;
        case OP_NAND:
        case OP_AND: {
                utListIterator<Sensors> it(m_sensors);
                while (it.hasMoreElements()) {

                    gkLogicSensor *sens = it.getNext();
                    if (fsr) {
                        fsr = false;
                        execAct = sens->isPositive();
                    } else
                        execAct = execAct && sens->isPositive();

                    if (m_op == OP_NAND)
                        sens->setNegative(true);
                }
                if (m_op == OP_NAND)
                    execAct = !execAct;
            }
            break;
        }
    }

    if (execAct) {
        gkLogicManager &mgr = gkLogicManager::getSingleton();

        utListIterator<Actuators> it(m_actuators);
        while (it.hasMoreElements()) {

            gkLogicActuator *act = it.getNext();
            if (!act->isActive()) {
                act->setActive(true);
                mgr.push(act);
            }
        }
    }
}
