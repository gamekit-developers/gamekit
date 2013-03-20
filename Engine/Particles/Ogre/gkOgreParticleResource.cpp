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

#include "gkOgreParticleResource.h"
#include "gkOgreParticleRenderer.h"
#include "gkOgreParticleEmitter.h"
#include "gkOgreParticleAffector.h"
#include "gkLogger.h"

#include "OgreParticleSystemManager.h"

gkOgreParticleResource::gkOgreParticleResource(gkResourceManager *creator, const gkResourceName &name, const gkResourceHandle &handle)
	:	gkParticleResource(creator, name, handle),
		m_psys(0),
		m_isTemplateOwner(false)
{

}

gkOgreParticleResource::~gkOgreParticleResource()
{
	if (m_psys)
	{
		if (m_isTemplateOwner)
			Ogre::ParticleSystemManager::getSingleton().removeTemplate(m_name.getName());
		m_psys = 0;
	}
}

void gkOgreParticleResource::createParticle(void)
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
		gkLogMessage("gkOgreParticleResource: " << e.getDescription());
	}
}


void gkOgreParticleResource::createParticle(const gkParticleSettingsProperties& props)
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

			gkOgreParticleAffector* affector = static_cast<gkOgreParticleAffector*>(psys->addAffector(gkOgreParticleAffector::NAME));
			affector->setCreator(this);

			gkOgreParticleEmitter*  emitter  = static_cast<gkOgreParticleEmitter*>(psys->addEmitter(gkOgreParticleEmitter::NAME));
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
		gkLogMessage("gkOgreParticleResource: " << e.getDescription());
	}
}


gkScalar gkOgreParticleResource::getMaxTimeToLiveOfEmitters(void)
{
	GK_ASSERT(m_psys);
	if (!m_psys) return 0;

	gkScalar timeToLive = 0;
	unsigned short i;

	for (i = 0; i < m_psys->getNumEmitters(); i++)
	{
		timeToLive = gkMax(timeToLive, m_psys->getEmitter(i)->getTimeToLive());
	}

	return timeToLive;
}