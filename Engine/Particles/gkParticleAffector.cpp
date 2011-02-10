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
#include "gkCommon.h"
#include "gkParticleAffector.h"
#include "gkParticleResource.h"
#include "gkParticleRenderer.h"
#include "Ogre.h"

const Ogre::String gkParticleAffector::NAME = "gkAffector";

gkParticleAffector::gkParticleAffector(Ogre::ParticleSystem* psys)
	:	Ogre::ParticleAffector(psys),
		m_creator(0)
{
	mType = NAME;
}

gkParticleAffector::~gkParticleAffector()
{
}

void gkParticleAffector::_affectParticles(Ogre::ParticleSystem* psys, Ogre::Real timeElapsed)
{
	GK_ASSERT(m_creator);
	gkParticleSettingsProperties& props = m_creator->getParticleProperties();
	if (props.m_gravity != 0)
	{
		Ogre::ParticleIterator pi = psys->_getIterator();	 
		while (!pi.end())
		{
			Ogre::Particle* p = pi.getNext();
			//float size = Ogre::Math::RangeRandom(props.m_size - props.m_sizeRandom, props.m_size + props.m_sizeRandom);
			//p->setDimensions(size, size);


			gkParticleVisualData* data = static_cast<gkParticleVisualData*>(p->getVisualData());
			if (data)
			{
				p->direction = data->m_initDir + props.m_gravity * (p->totalTimeToLive - p->timeToLive) * gkVector3(0,0,-9.8f);
			}
		}
	}
}

void gkParticleAffector::copyParametersTo(Ogre::StringInterface* dest) const
{
	Ogre::ParticleAffector::copyParametersTo(dest);
	static_cast<gkParticleAffector*>(dest)->m_creator = m_creator;
}

//--

Ogre::ParticleAffector* gkAffectorFactory::createAffector(Ogre::ParticleSystem* psys)
{
	Ogre::ParticleAffector* p = new gkParticleAffector(psys);
	mAffectors.push_back(p);
	return p;
}
