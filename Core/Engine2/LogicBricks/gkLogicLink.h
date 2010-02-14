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
#ifndef _gkLogicLink_h_
#define _gkLogicLink_h_

#include "gkCommon.h"
#include "gkString.h"

class gkLogicSensor;
class gkLogicController;
class gkLogicActuator;



class gkLogicLink : public utListClass<gkLogicLink>::Link
{
public:

    typedef utList<gkLogicSensor*>      SensorList;
    typedef utList<gkLogicController*>  ControllerList;
    typedef utList<gkLogicActuator*>    ActuatorList;


protected:

    SensorList      m_sensors;
    ControllerList  m_controllers;
    ActuatorList    m_actuators;
    int             m_state;


public:

    gkLogicLink();
    ~gkLogicLink();

    gkLogicActuator* findActuator(const gkString& name);
    gkLogicController* findController(const gkString& name);


    GK_INLINE void push(gkLogicSensor *v)       {GK_ASSERT(v); m_sensors.push_back(v);}
    GK_INLINE void push(gkLogicController *v)   {GK_ASSERT(v); m_controllers.push_back(v);}
    GK_INLINE void push(gkLogicActuator *v)     {GK_ASSERT(v); m_actuators.push_back(v);}
    GK_INLINE void setState(int v)              {m_state = v;}
    GK_INLINE int getState(void)                {return m_state;}
};

#endif//_gkLogicLink_h_
