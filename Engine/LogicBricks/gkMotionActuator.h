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

    struct MotionData
    {
        MotionData() :  vec(0,0,0), local(true), evaluate(false) {}

        gkVector3 vec;
        bool local, evaluate;
    };


protected:

    int m_type;
    MotionData m_loc, m_rot, m_force, m_torque, m_linv, m_angv;
    bool m_linvInc;
    gkScalar m_damping;
    gkQuaternion m_quat;

public:

    gkMotionActuator(gkGameObject *object, gkLogicLink *link, const gkString &name);
    virtual ~gkMotionActuator();

    gkLogicBrick* clone(gkLogicLink *link, gkGameObject *dest);

    // Handle incoming logic.
    void execute(void);


    GK_INLINE void setType(int v)                   {m_type = v;}
    GK_INLINE void setDamping(gkScalar v)           {m_damping = gkClampf(1.f - v, 0.f, 1.f);}
    GK_INLINE void setIncrementalVelocity(bool v)   {m_linvInc = v;}
    

    GK_INLINE void setTranslation(const gkVector3& v, bool local)   
    {
        m_loc.vec = v; 
        m_loc.local = local; 
        m_loc.evaluate = !gkFuzzyVec(m_loc.vec);
    }
 
    GK_INLINE void setRotation(const gkVector3& v, bool local)   
    {
        m_quat = gkMathUtils::getQuatFromEuler(v, false);
        m_rot.vec = v; 
        m_rot.local = local; 
        m_rot.evaluate = !gkFuzzyVec(m_rot.vec);

    }

    GK_INLINE void setForce(const gkVector3& v, bool local)   
    {
        m_force.vec = v; 
        m_force.local = local; 
        m_force.evaluate = !gkFuzzyVec(m_force.vec);
    }

    GK_INLINE void setTorque(const gkVector3& v, bool local)   
    {
        m_torque.vec = v; 
        m_torque.local = local; 
        m_torque.evaluate = !gkFuzzyVec(m_torque.vec);
    }

    GK_INLINE void setLinearVelocity(const gkVector3& v, bool local)   
    {
        m_linv.vec = v; 
        m_linv.local = local; 
        m_linv.evaluate = !gkFuzzyVec(m_linv.vec);
    }

    GK_INLINE void setAngularVelocity(const gkVector3& v, bool local)   
    {
        m_angv.vec = v; 
        m_angv.local = local; 
        m_angv.evaluate = !gkFuzzyVec(m_angv.vec);
    }
};


#endif//_gkMotionActuator_h_
