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
#ifndef _gkLightObject_h_
#define _gkLightObject_h_

#include "gkGameObject.h"
#include "gkSerialize.h"


// Game light
class gkLight : public gkGameObject
{
public:
    gkLight(gkScene *scene, const gkString& name, gkObject::Loader* loader = 0);
    virtual ~gkLight() {}

    // Property access
    GK_INLINE gkLightProperties& getLightProperties(void)           {return m_lightProps;}
    GK_INLINE void setLightProperties(const gkLightProperties& v)   {m_lightProps = v; }
    GK_INLINE Ogre::Light* getLight(void)                           {return m_light;}

    void updateProperties(void);

private:
    gkObject    *clone(const gkString &name);

    gkLightProperties       m_lightProps;
    Ogre::Light             *m_light;

    virtual void loadImpl(void);
    virtual void unloadImpl(void);
};

#endif//_gkLightObject_h_
