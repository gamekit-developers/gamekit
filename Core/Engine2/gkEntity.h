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
#ifndef _gkEntityObject_h_
#define _gkEntityObject_h_

#include "gkGameObject.h"
#include "gkSerialize.h"
#include "gkActionManager.h"


// Game entity
class gkEntity : public gkGameObject
{
public:
    gkEntity(gkScene *scene, const gkString& name, gkObject::Loader* loader = 0);
    virtual ~gkEntity() {}


    GK_INLINE Ogre::Entity* getEntity(void)                             {return m_entity;}

    // Property access
    GK_INLINE gkEntityProperties& getEntityProperties(void)             {return m_entityProps;}
    GK_INLINE void setEntityProperties(const gkEntityProperties& v)     {m_entityProps = v;}
    GK_INLINE gkAnimProperties& getAnimationProperties(void)            {return m_animProps;}
    GK_INLINE void setAnimationProperties(const gkAnimProperties& v)    {m_animProps = v;}



    void playAction(const gkString& act, gkScalar blend);


    void _resetPose(void);
    void updateAnimations(void);

protected:
    gkEntityProperties      m_entityProps;
    Ogre::Entity*           m_entity;
    gkAnimProperties        m_animProps;
    gkActionManager         m_actionMgr;
    gkAction*               m_active, *m_default;
    gkScalar                m_animTime, m_blendTime;
    gkString                m_activeName;
    gkSkeleton*             m_skeleton;

    virtual void loadImpl();
    virtual void unloadImpl();
};


#endif//_gkEntityObject_h_
