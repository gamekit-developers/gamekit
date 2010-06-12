/*
-------------------------------------------------------------------------------
--                                                                           --
--  This file is part of GameKit.                                            --
--  http://gamekit.googlecode.com/                                           --
--                                                                           --
--                                                                           --
--  Released under the zlib license.                                         --
--  http://www.gzip.org/zlib/zlib_license.html                               --
--                                                                           --
--                                                                           --
-------------------------------------------------------------------------------
*/
#ifndef _gkLuaLogicBrick_h_
#define _gkLuaLogicBrick_h_

#include "gkLogicBrick.h"


class LogicBrick
{
protected:

    gkLogicBrick *m_brick;

public:


#ifndef SWIG

    LogicBrick(gkLogicBrick *brick) 
        :   m_brick(brick)
    {
    }

    template<typename T> T* cast(void) { return static_cast<T*>(m_brick); }

#endif


    LogicBrick()
        :   m_brick(0)
    {
    }

    ~LogicBrick()
    {
    }

    const char *getName(void)
    {
        static char buf[32];
        if (m_brick)
            sprintf(buf, "%s", m_brick->getName().c_str());
        else buf[0] = 0;
        return buf;
    }


    bool isPulseOn(void)     
    { 
        if (m_brick)
            return m_brick->isPulseOn(); 
        return false;
    }
    bool isPulseOff(void)    
    { 
        if (m_brick)
            return m_brick->isPulseOff(); 
        return false;
    }

};


#endif//_gkLuaLogicBrick_h_
