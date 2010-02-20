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
#include "gkMotionActuator.h"
#include "gkGameObject.h"
#include "gkRigidBody.h"

gkMotionActuator::gkMotionActuator(gkGameObject *object, gkLogicLink *link, const gkString &name)
:       gkLogicActuator(object, link, name),
        m_type(0), m_linvInc(false), m_damping(1.f)
{
}

gkMotionActuator::~gkMotionActuator()
{
}

void gkMotionActuator::execute(void)
{
    GK_ASSERT(m_object);
    if (!m_object->isLoaded())
        return;

    gkRigidBody *body = m_object->getAttachedBody();

    if (m_type == MT_SIMPLE) {
        if (m_loc.evaluate)
            m_object->translate(m_loc.vec , m_loc.local ? TRANSFORM_LOCAL : TRANSFORM_PARENT);
        if (m_rot.evaluate)
            m_object->rotate(m_quat, m_rot.local ? TRANSFORM_LOCAL : TRANSFORM_PARENT);

        if (body)
        {
            if (m_force.evaluate)
                body->applyForce(m_force.vec* m_damping, m_force.local ? TRANSFORM_LOCAL : TRANSFORM_PARENT);
            if (m_torque.evaluate)
                body->applyTorque(m_torque.vec* m_damping, m_torque.local ? TRANSFORM_LOCAL : TRANSFORM_PARENT);

            if (m_linv.evaluate)
            {
                gkVector3 extra(0,0,0);
                if (m_linvInc)
                    extra = body->getLinearVelocity();
                body->setLinearVelocity((m_linv.vec + extra) *m_damping , m_linv.local ? TRANSFORM_LOCAL : TRANSFORM_PARENT);
            }
            if (m_angv.evaluate)
                body->setAngularVelocity(m_angv.vec *m_damping , m_angv.local ? TRANSFORM_LOCAL : TRANSFORM_PARENT);
        }
    }
}
