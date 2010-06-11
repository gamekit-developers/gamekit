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
#ifndef _gkPropertySensor_h_
#define _gkPropertySensor_h_

#include "gkLogicSensor.h"
#include "gkVariable.h"


class gkPropertySensor : public gkLogicSensor
{
public:

    enum Type
    {
        PS_EQUAL,
        PS_NEQUAL,
        PS_INTERVAL,
        PS_CHANGED,
    };

protected:

    gkVariable  m_old;
    gkVariable  *m_cur;
    int         m_type;
    gkString    m_propName, m_propVal;
    bool        m_init;

public:

    gkPropertySensor(gkGameObject *object, gkLogicLink *link, const gkString &name);
    virtual ~gkPropertySensor() {}

    gkLogicBrick* clone(gkLogicLink *link, gkGameObject *dest);

    bool query(void);


    GK_INLINE void  setType(int type)               {m_type = type;}
    GK_INLINE int   getType(void)                   {return m_type;}

    GK_INLINE void  setProperty(const gkString& v)  {m_propName = v;}
    GK_INLINE void  setValue(const gkString& v)     {m_propVal = v;}
};


#endif//_gkPropertySensor_h_
