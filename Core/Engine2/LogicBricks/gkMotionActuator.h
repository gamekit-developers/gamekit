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
#ifndef _gkMotionActuator_h_
#define _gkMotionActuator_h_

#include "gkLogicActuator.h"


class gkMotionActuator : public gkLogicActuator
{
public:

    enum Type
    {
        MT_SIMPLE,
        MT_SERVO,
    };

protected:

    int m_type;
    gkVector3   m_loc, m_rot;
    bool        m_locL, m_rotL;

public:

    gkMotionActuator(gkGameObject *object, gkLogicLink *link, const gkString &name);
    virtual ~gkMotionActuator();


    // Handle incoming logic.
    void execute(void);


    GK_INLINE void setType(int v)                           {m_type = v;}
    GK_INLINE void setLoc(const gkVector3& v, bool local)   {m_loc = v; m_locL = local;}
    GK_INLINE void setRot(const gkVector3& v, bool local)   {m_rot = v; m_rotL = local;}
};


#endif//_gkMotionActuator_h_
