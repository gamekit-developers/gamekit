/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 harkon.kr.

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

#ifndef _gkParticleResource_h_
#define _gkParticleResource_h_

#include "gkSerialize.h"
#include "gkResource.h"

class gkParticleResource : public gkResource
{
public:
	gkParticleResource(gkResourceManager* creator, const gkResourceName& name, const gkResourceHandle& handle);
	virtual ~gkParticleResource();

	GK_INLINE gkParticleSettingsProperties& getParticleProperties() { return m_particleProps; }

	Ogre::ParticleSystem* createParticle(const gkParticleSettingsProperties& props);
	Ogre::ParticleSystem* createParticle(); //use .particle script template

	GK_INLINE Ogre::ParticleSystem* getParticleSystem() { return m_psys; }

	GK_INLINE bool isTemplateOwner() const { return m_isTemplateOwner; }
protected:
	Ogre::ParticleSystem* m_psys;
	gkParticleSettingsProperties m_particleProps;
	bool m_isTemplateOwner;
};

#endif//_gkParticleResource_h_