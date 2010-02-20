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
#ifndef _gkRigidBody_h_
#define _gkRigidBody_h_


#include "gkObject.h"
#include "gkTransformState.h"
#include "LinearMath/btMotionState.h"
#include "BulletCollision/NarrowPhaseCollision/btManifoldPoint.h"

class btDynamicsWorld;
class btRigidBody;
class btTriangleMesh;
class gkDynamicsWorld;


// Game body
class gkRigidBody : public gkObject, public btMotionState, public utListClass<gkRigidBody>::Link
{
public:

    enum Flags
    {
        RBF_LIMIT_LVEL_X = 1,
        RBF_LIMIT_LVEL_Y = 2,
        RBF_LIMIT_LVEL_Z = 4,
        RBF_LIMIT_AVEL_X = 8,
        RBF_LIMIT_AVEL_Y = 16,
        RBF_LIMIT_AVEL_Z = 32,
        RBF_CONTACT_INFO = 64, 
    };


    struct ContactInfo
    {
        gkRigidBody*        collider;
        btManifoldPoint     point;
    };

    typedef utArray<ContactInfo> ContactArray;

protected:

    // Parent world
    gkDynamicsWorld*    m_owner;

    // modifier object
    gkGameObject*       m_object;


    // Bullet body
    btRigidBody*        m_rigidBody;

    // information about collisions
    ContactArray        m_contacts;

    // misc flags
    int                 m_flags;

    // material info for sensors
    gkString            m_sensorMaterial;


    // transform callbacks

    void getWorldTransform(btTransform& worldTrans) const;
    void setWorldTransform(const btTransform& worldTrans);

    void loadImpl(void);
    void unloadImpl(void);

public:

    gkRigidBody(const gkString& name, gkGameObject *object, gkDynamicsWorld *owner, gkObject::Loader *manual=0);
    virtual ~gkRigidBody();

    void _reinstanceBody(btRigidBody *body);

    void setTransformState(const gkTransformState& state);

    // collision contact information
    GK_INLINE ContactArray& getContacts(void) {return m_contacts;}

    GK_INLINE bool wantsContactInfo(void) 
    {return (m_flags & RBF_CONTACT_INFO) != 0;}

    GK_INLINE void setFlags(int flags)  {m_flags = flags;}
    GK_INLINE int  getFlags(void)       {return m_flags;}

    GK_INLINE void setSensorMaterial(const gkString& v) {m_sensorMaterial = v;}
    GK_INLINE const gkString& getSensorMaterial(void)   {return m_sensorMaterial;}


    // update state based on the objects transform 
    void        updateTransform(void);


    void        applyTorque(const gkVector3 &t, int tspace = TRANSFORM_PARENT);
    void        applyForce(const gkVector3 &f, int tspace = TRANSFORM_PARENT);
    void        setLinearVelocity(const gkVector3 &linv, int tspace = TRANSFORM_PARENT);
    void        setAngularVelocity(const gkVector3 &angv, int tspace = TRANSFORM_PARENT);
    gkVector3   getLinearVelocity(void);
    gkVector3   getAngularVelocity(void);



    // Gain access to the bullet body
    btRigidBody* getBody(void)      {GK_ASSERT(m_rigidBody); return m_rigidBody;}
    // Gain access to the world
    gkDynamicsWorld* getWorld(void) {GK_ASSERT(m_owner); return m_owner;}
    // Gain access to the game object
    gkGameObject* getObject(void)   {GK_ASSERT(m_object); return m_object;}

};

#endif//_gkRigidBody_h_
