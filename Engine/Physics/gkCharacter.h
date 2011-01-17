/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Nestor Silveira.

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
#ifndef _gkCharacter_h_
#define _gkCharacter_h_


#include "gkPhysicsController.h"


class btDynamicsWorld;
class btPairCachingGhostObject;
class btKinematicCharacterController;
class btTriangleMesh;
class gkDynamicsWorld;


class gkCharacter : public gkPhysicsController, public btActionInterface
{
public:

	gkCharacter(gkGameObject* object, gkDynamicsWorld* owner);
	virtual ~gkCharacter();


	void setVelocity(const gkVector3& v, gkScalar timeInterval);
	void setLinearVelocity(gkScalar forward, gkScalar backward, gkScalar rightward, gkScalar leftward, gkScalar timeInterval);


	btPairCachingGhostObject* getGhostObject() const;
	btKinematicCharacterController* getCharacterController() const { return m_character; }

	void updateAction( btCollisionWorld* collisionWorld, btScalar deltaTime);

	void debugDraw(btIDebugDraw* debugDrawer) {}

	void setGravity(gkScalar gravity);

	void setRotation(const gkVector3& axis, gkScalar scalar);

	void setJumpSpeed(gkScalar scalar);
	void jump(void);

	bool isOnGround(void);

	void create(void);
	void destroy(void);

protected:

	void setWorldTransform(const btTransform& worldTrans);

	btKinematicCharacterController* m_character;
};

#endif//_gkCharacter_h_
