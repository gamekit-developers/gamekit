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
#ifndef _gkLogicController_h_
#define _gkLogicController_h_

#include "gkLogicBrick.h"


class gkLogicController : public gkLogicBrick
{
public:

    typedef utList<gkLogicSensor*>      Sensors;
    typedef utList<gkLogicActuator*>    Actuators;

protected:
    Sensors     m_sensors;
    Actuators   m_actuators;
    bool        m_isGate;

public:

    gkLogicController(gkGameObject *object, const gkString &name);
    virtual ~gkLogicController() {}

    GK_INLINE void link(gkLogicSensor *v)       {GK_ASSERT(v); m_sensors.push_back(v);}
    GK_INLINE void link(gkLogicActuator *v)     {GK_ASSERT(v); m_actuators.push_back(v);}

    // Process incoming events, handle then pass to actuators.
    virtual void relay(void) = 0;

    GK_INLINE void setGate(bool v)  {m_isGate = v;}
    GK_INLINE bool isGate(void)     {return m_isGate;}
};


#endif//_gkLogicController_h_
