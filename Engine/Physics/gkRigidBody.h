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
#ifndef _gkRigidBody_h_
#define _gkRigidBody_h_

#include "gkPhysicsController.h"
#include "LinearMath/btMotionState.h"


class btDynamicsWorld;
class btRigidBody;
class btTriangleMesh;
class btCollisionShape;
class gkDynamicsWorld;



// gkRigidBody handles:
// dynamic (no angular velocity), rigid, and static bodies
class gkRigidBody : public gkPhysicsController, public btMotionState
{
public:
	gkRigidBody(gkGameObject* object, gkDynamicsWorld* owner);
	virtual ~gkRigidBody();

	void        applyTorque(const gkVector3& t, int tspace = TRANSFORM_PARENT);
	void        applyForce(const gkVector3& f, int tspace = TRANSFORM_PARENT);


	void        setLinearVelocity(const gkVector3& linv, int tspace = TRANSFORM_PARENT);
	void        setAngularVelocity(const gkVector3& angv, int tspace = TRANSFORM_PARENT);
	gkVector3   getLinearVelocity(void);
	gkVector3   getAngularVelocity(void);

	GK_INLINE int setActivationState(int newActivationState)
	{
		int oldState = m_oldActivationState;

		m_oldActivationState = newActivationState;

		return oldState;
	}

	btRigidBody* getBody(void);

	void addConstraint(btTypedConstraint* constraint, bool disableLinkedCollision = false);
	void removeConstraint(btTypedConstraint* constraint);

	GK_INLINE UTsize getConstraintCount(void)				{ return m_constraints.size(); }
	GK_INLINE btTypedConstraint* getConstraint(UTsize i)	{ return m_constraints[i];     }


	void create(void);
	void destroy(void);

	void createConstraints(void);
	void removeConstaints(void);

	void recalLocalInertia(void);

private:

	void getWorldTransform(btTransform& worldTrans) const;
	void setWorldTransform(const btTransform& worldTrans);

	btRigidBody* m_body;
	utArray<btTypedConstraint*> m_constraints;

	int m_oldActivationState;
};

#endif//_gkRigidBody_h_
