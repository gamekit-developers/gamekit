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
#ifndef _gkCollisionSensor_h_
#define _gkCollisionSensor_h_

#include "gkLogicSensor.h"
#include "gkLogicDispatcher.h"
#include "Physics/gkDynamicsWorld.h"


class gkCollisionDispatch : public gkAbstractDispatcher
{
public:
    gkCollisionDispatch();
    virtual ~gkCollisionDispatch() {}

    void dispatch(void);
};


class gkCollisionSensor : public gkLogicSensor
{
protected:
    gkString m_material, m_prop;

public:

    gkCollisionSensor(gkGameObject *object, gkLogicLink *link, const gkString &name);
    virtual ~gkCollisionSensor() {}

    bool query(void);
    GK_INLINE void setMaterial(const gkString& material)    {m_material = material;}
    GK_INLINE void setProperty(const gkString& prop)        {m_prop = prop;}
};


#endif//_gkCollisionSensor_h_
