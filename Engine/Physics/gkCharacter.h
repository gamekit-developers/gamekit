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


#include "btBulletDynamicsCommon.h"
#include "gkObject.h"
#include "gkTransformState.h"
#include "LinearMath/btMotionState.h"


class btDynamicsWorld;
class btPairCachingGhostObject;
class btKinematicCharacterController;
class btTriangleMesh;
class gkDynamicsWorld;

class gkCharacter : public gkObject, public btActionInterface
{
public:

    gkCharacter(const gkString& name, gkGameObject *object, gkDynamicsWorld *owner, gkObject::Loader *manual=0);

    virtual ~gkCharacter();

    void _reinstanceCharacter(btPairCachingGhostObject *ghostObject);

    void setTransformState(const gkTransformState& state);

    // update state based on the objects transform 
    void updateTransform(void);

	void setVelocity(const gkVector3& v, gkScalar timeInterval);

    // Gain access to the bullet body
    //btKinematicCharacterController* getCharacter()      {return m_character;}
    // Gain access to the world
    gkDynamicsWorld* getWorld(void) {GK_ASSERT(m_owner); return m_owner;}
    // Gain access to the game object
    gkGameObject* getObject(void)   {GK_ASSERT(m_object); return m_object;}

	btCollisionObject* getCollisionObject();

	Ogre::AxisAlignedBox getAabb() const;

	btPairCachingGhostObject* getGhostObject() const { return m_ghostObject; }
	btKinematicCharacterController* getCharacterController() const { return m_character; }

	void updateAction( btCollisionWorld* collisionWorld, btScalar deltaTime);

	void debugDraw(btIDebugDraw* debugDrawer){}


protected:

    void setWorldTransform(const btTransform& worldTrans);

    void loadImpl(void);
    void unloadImpl(void);

protected:

    // Parent world
    gkDynamicsWorld* m_owner;

    // modifier object
    gkGameObject* m_object;

	btPairCachingGhostObject* m_ghostObject;
	btKinematicCharacterController* m_character;
	
};

#endif//_gkCharacter_h_

