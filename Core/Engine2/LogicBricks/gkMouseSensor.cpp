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
#include "gkMouseSensor.h"
#include "gkLogicManager.h"


gkMouseDispatch::gkMouseDispatch()
{
}

gkMouseDispatch::~gkMouseDispatch()
{
}

void gkMouseDispatch::dispatch(void)
{
    // TODO, do tests for change

    if (!m_sensors.empty())
    {
        utListIterator<SensorList> it(m_sensors);
        while (it.hasMoreElements())
            it.getNext()->tick();
    }
}


gkMouseSensor::gkMouseSensor(gkGameObject *object, const gkString &name)
:       gkLogicSensor(object, name), m_type(MOUSE_NILL)
{
    // connect to dispatcher
    gkLogicManager::getSingleton().getDispatcher(DIS_MOUSE).connect(this);
}


bool gkMouseSensor::query(void)
{
    if (m_type == MOUSE_NILL)
        return false;


    gkMouse *mse = gkWindowSystem::getSingleton().getMouse();
    switch (m_type)
    {
    case MOUSE_LEFT:
        return mse->isButtonDown(gkMouse::Left);
    case MOUSE_MIDDLE:
        return mse->isButtonDown(gkMouse::Middle);
    case MOUSE_RIGHT:
        return mse->isButtonDown(gkMouse::Right);
    case MOUSE_MOTION:
        return mse->moved;
    case MOUSE_WHEEL_UP:
        return mse->wheelDelta > 0;
    case MOUSE_WHEEL_DOWN:
        return mse->wheelDelta < 0;
    case MOUSE_MOUSE_OVER:
    case MOUSE_MOUSE_OVER_ANY:
        // use Ogre viewport to ray query
        break;
    }
    return false;
}
