/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 harkon.kr.

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

#ifndef _gkOgreParticleEmitter_h_
#define _gkOgreParticleEmitter_h_

#include "OgreParticleEmitter.h"
#include "OgreParticleEmitterFactory.h"

class gkParticleResource;

class gkOgreParticleEmitter : public Ogre::ParticleEmitter
{
protected:
	gkParticleResource* m_creator;

public:
	static const Ogre::String NAME;
	
	gkOgreParticleEmitter(Ogre::ParticleSystem* psys);
	virtual ~gkOgreParticleEmitter();

	virtual void copyParametersTo(Ogre::StringInterface* dest) const;
	virtual unsigned short _getEmissionCount(Ogre::Real timeElapsed);
	virtual void _initParticle(Ogre::Particle* pParticle);


	GK_INLINE void setCreator(gkParticleResource* creator) { m_creator = creator; }
};


class gkOgreEmitterFactory : public Ogre::ParticleEmitterFactory
{
public:	
	GK_INLINE Ogre::String getName() const { return gkOgreParticleEmitter::NAME; }
	Ogre::ParticleEmitter* createEmitter(Ogre::ParticleSystem* psys);
};

#endif//_gkOgreParticleEmitter_h_