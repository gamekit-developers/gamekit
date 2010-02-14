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
#ifndef _gkLogicSensor_h_
#define _gkLogicSensor_h_

#include "gkLogicBrick.h"


class gkLogicSensor : public gkLogicBrick
{
public:
    enum Pulse
    {
        PULSE_NONE      = 0,
        PULSE_POSITIVE  = 1,
        PULSE_NEGATIVE  = 2,
    };

    typedef utList<gkLogicController*> Controllers;

protected:

    Controllers m_controllers;

    int     m_freq, m_tick, m_pulse;
    bool    m_invert, m_positive, m_suspend, m_tap;
    bool    m_sorted;

public:

    gkLogicSensor(gkGameObject *object, const gkString &name);
    virtual ~gkLogicSensor() {}


    GK_INLINE void link(gkLogicController *cont) {GK_ASSERT(cont); m_controllers.push_back(cont);}

    // Process pending events.
    void tick(void);

    // Do test on subclass to see if a pulse is wanted.
    virtual bool query(void) = 0;


    // Events have been queried, now pass to 
    // connected controllers for further processing.
    void dispatch(void);


    GK_INLINE void setFrequency(int v)      {m_freq = v;}
    GK_INLINE void setMode(int m)           {m_pulse = m;}
    GK_INLINE void invert(bool v)           {m_invert = v;}
    GK_INLINE bool isPositive(void)         {return m_positive;}
    GK_INLINE void suspend(bool v)          {m_suspend = v;}
    GK_INLINE bool isSuspended(void)        {return m_suspend;} 
    GK_INLINE void setTap(bool v)           {m_tap = v;}

};


#endif//_gkLogicSensor_h_
