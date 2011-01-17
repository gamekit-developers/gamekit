/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): Nestor Silveira.
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



gkMotionActuator::gkMotionActuator(gkGameObject* object, gkLogicLink* link, const gkString& name)
	:       gkLogicActuator(object, link, name),
	        m_type(0), m_linvInc(false), m_damping(1.f), m_dampIncr(0.f)
{
}


gkMotionActuator::~gkMotionActuator()
{
}



gkLogicBrick* gkMotionActuator::clone(gkLogicLink* link, gkGameObject* dest)
{
	gkMotionActuator* act = new gkMotionActuator(*this);
	act->cloneImpl(link, dest);
	return act;
}


void gkMotionActuator::execute(void)
{
	if (isPulseOff())
	{
		// reset damping
		m_dampIncr = 0.f;
		return;
	}

	if (m_type == MT_SIMPLE)
	{
		if (m_loc.evaluate)
			m_object->translate(m_loc.vec , m_loc.local ? TRANSFORM_LOCAL : TRANSFORM_PARENT);
		if (m_rot.evaluate)
			m_object->rotate(m_quat, m_rot.local ? TRANSFORM_LOCAL : TRANSFORM_PARENT);

		gkPhysicsController* object = m_object->getPhysicsController();

		if (object)
		{
			gkRigidBody* body = dynamic_cast<gkRigidBody*>(object);

			if (body)
			{
				// Tooltip, states it's the number of frames to reach the target.
				gkScalar val = 1.f;
				if (m_damping > 0.f)
				{
					m_dampIncr += 1.f;
					if (m_dampIncr > m_damping)
						m_dampIncr = m_damping;

					val = m_dampIncr / m_damping;
				}

				if (m_force.evaluate)
					body->applyForce(m_force.vec * val, m_force.local ? TRANSFORM_LOCAL : TRANSFORM_PARENT);
				if (m_torque.evaluate)
				{
					if (body->getProperties().isDynamic())
					{
						//workaround for incompatibility between 'DYNAMIC' game object, and angular factor
						//a DYNAMIC object has some inconsistency: it has no angular effect due to collisions, but still has torque
						btRigidBody* btbody = body->getBody();
						const btVector3 old = btbody->getAngularFactor();
						btbody->setAngularFactor(btVector3(1,1,1));
						body->applyTorque(m_torque.vec * val, m_torque.local ? TRANSFORM_LOCAL : TRANSFORM_PARENT);
						btbody->setAngularFactor(old);
					}
					else
						body->applyTorque(m_torque.vec * val, m_torque.local ? TRANSFORM_LOCAL : TRANSFORM_PARENT);
				}

				if (m_linv.evaluate)
				{
					gkVector3 extra = m_linv.vec;
					if (m_linvInc)
						extra += body->getLinearVelocity();
					body->setLinearVelocity(extra * val, m_linv.local ? TRANSFORM_LOCAL : TRANSFORM_PARENT);
				}
				if (m_angv.evaluate)
					body->setAngularVelocity(m_angv.vec * val , m_angv.local ? TRANSFORM_LOCAL : TRANSFORM_PARENT);
			}
		}
	}
}
