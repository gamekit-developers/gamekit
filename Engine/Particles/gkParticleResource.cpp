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

#include "gkParticleResource.h"
#include "gkParticleRenderer.h"
#include "gkParticleEmitter.h"
#include "gkParticleAffector.h"
#include "gkLogger.h"

#include "OgreParticleSystemManager.h"

gkParticleResource::gkParticleResource(gkResourceManager *creator, const gkResourceName &name, const gkResourceHandle &handle)
	:	gkResource(creator, name, handle),
		m_psys(0),
		m_isTemplateOwner(false)
{

}

gkParticleResource::~gkParticleResource()
{
	if (m_psys)
	{
		if (m_isTemplateOwner)
			Ogre::ParticleSystemManager::getSingleton().removeTemplate(m_name.getName());
		m_psys = 0;
	}
}

Ogre::ParticleSystem* gkParticleResource::createParticle()
{
	GK_ASSERT(m_psys == 0);

	try
	{
		Ogre::ParticleSystemManager& mgr = Ogre::ParticleSystemManager::getSingleton();
		Ogre::ParticleSystem* psys = mgr.getTemplate(getName());

		m_particleProps.m_name = getName();
		m_isTemplateOwner = false;
		m_psys = psys;
	}
	catch (Ogre::Exception& e)
	{
		gkLogMessage("gkParticleResource: " << e.getDescription());
	}

	return m_psys;
}


Ogre::ParticleSystem* gkParticleResource::createParticle(const gkParticleSettingsProperties& props)
{
	GK_ASSERT(m_psys == 0);

	try
	{
		Ogre::ParticleSystemManager& mgr = Ogre::ParticleSystemManager::getSingleton();
		Ogre::ParticleSystem* psys = mgr.getTemplate(getName());
		if (!psys)
		{
			psys = mgr.createTemplate(getName(), getGroupName());			

			m_isTemplateOwner = true;

			gkParticleAffector* affector = static_cast<gkParticleAffector*>(psys->addAffector(gkParticleAffector::NAME));
			affector->setCreator(this);

			gkParticleEmitter*  emitter  = static_cast<gkParticleEmitter*>(psys->addEmitter(gkParticleEmitter::NAME));
			emitter->setCreator(this);

			psys->setCullIndividually(false);
			psys->setDefaultDimensions(props.m_size, props.m_size);

			psys->setParticleQuota(props.m_amount);
			//emitter->setStartTime(props.m_start);
			
			//emitter->setTimeToLive(props.m_lifetime);
			//emitter->setDirection(props.m_velocity);
			//emitter->setParticleVelocity(props.m_velocity.length());

			psys->setParameter("billboard_type", "oriented_self");
			//emitter->setEmissionRate(props.m_velNormal);
		}

		m_particleProps = props;
		m_psys = psys;
	}
	catch (Ogre::Exception& e)
	{
		gkLogMessage("gkParticleResource: " << e.getDescription());
	}

	return m_psys;
}
