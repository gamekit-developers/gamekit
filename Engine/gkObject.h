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
#ifndef _gkObject_h_
#define _gkObject_h_

#include "gkCommon.h"
#include "gkHashedString.h"
#include "BulletCollision/NarrowPhaseCollision/btManifoldPoint.h"
#include "OgreAxisAlignedBox.h"


class gkGameObject;
class btCollisionObject;
class btPersistentManifold;

// Base class representing a loadable object
class gkObject : public utListClass<gkObject>::Link
{
protected:
    const gkString      m_name;
    bool                m_loaded;

    virtual void preLoadImpl(void) {}
    virtual void preUnloadImpl(void) {}
    virtual void loadImpl(void) {}
    virtual void unloadImpl(void) {}
    virtual void postLoadImpl(void) {}
    virtual void postUnloadImpl(void) {}

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
        gkObject        *collider;
        btManifoldPoint     point;
    };

    typedef utArray<ContactInfo> ContactArray;

    gkObject(const gkString &name);
    virtual ~gkObject();

    // duplication 
    virtual gkObject                *clone(const gkString &name)    {return 0;}
    virtual gkGameObject            *getObject(void)                {return 0;}
    virtual Ogre::AxisAlignedBox     getAabb() const                {return Ogre::AxisAlignedBox::BOX_NULL;}
    virtual btCollisionObject       *getCollisionObject()           {return 0;}


    void load(void);
    void unload(void);
    void reload(void);


    GK_INLINE const gkString    &getName(void)                          {return m_name;}
    GK_INLINE bool              isLoaded(void) const                    {return m_loaded;}

    // collision contact information
    GK_INLINE ContactArray      &getContacts(void)                      {return m_contacts;}
    GK_INLINE bool              wantsContactInfo(void)                  {return (m_flags & RBF_CONTACT_INFO) != 0;}

    // misc flags
    GK_INLINE void              setFlags(int flags)                     {m_flags = flags;}
    GK_INLINE int               getFlags(void)                          {return m_flags;}


    // attached material for physics
    GK_INLINE void              setSensorMaterial(const gkString &v)    {m_sensorMaterial = v;}
    GK_INLINE const gkString    &getSensorMaterial(void)                {return m_sensorMaterial;}

    void handleManifold(btPersistentManifold *manifold);

    void resetContactInfo();

protected:

    // information about collisions
    ContactArray m_contacts;

    // misc flags
    int m_flags;

    // material info for sensors
    gkString m_sensorMaterial;
};


#endif//_gkObject_h_
