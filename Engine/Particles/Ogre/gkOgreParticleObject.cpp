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

#include "gkCommon.h"
#include "gkScene.h"
#include "gkOgreParticleObject.h"
#include "gkOgreParticleRenderer.h"
#include "gkOgreParticleResource.h"
#include "gkParticleManager.h"
#include "gkLogger.h"

#include "OgreSceneManager.h"
#include "OgreParticleSystemManager.h"


gkOgreParticleObject::gkOgreParticleObject(gkInstancedManager* creator, const gkResourceName& name, const gkResourceHandle& handle)
	:	gkParticleObject(creator, name, handle),
		m_psys(0)
{
}

gkOgreParticleObject::~gkOgreParticleObject()
{
}

void gkOgreParticleObject::createInstanceImpl()
{
	gkParticleObject::createInstanceImpl();

	GK_ASSERT(!m_psys);

	try
	{
		Ogre::SceneManager* manager = m_scene->getManager();

		const gkString& pname = m_particleProps.m_settings;
		
		m_psys = manager->createParticleSystem(getName(), m_particleProps.m_settings);
		if (m_psys)
		{
			m_psys->setUserAny(Ogre::Any(this));
			m_node->attachObject(m_psys);

			gkOgreParticleResource* resource = gkParticleManager::getSingleton().getByName<gkOgreParticleResource>(
				gkResourceName(pname, getGroupName()));

			if (resource && resource->isTemplateOwner())
			{
				m_psys->setRenderer(gkOgreParticleRenderer::NAME);
		
				if (resource->getParticleProperties().m_render == gkParticleSettingsProperties::R_HALO)
					m_psys->setMaterialName(gkParticleManager::getSingleton().createOrRetrieveHaloMaterial(m_particleProps.m_material));
				else
					m_psys->setMaterialName(m_particleProps.m_material);
			} //else create by .particle script
		}
	}
	catch (Ogre::Exception& e)
	{
		gkLogMessage("gkOgreParticleObject: " << e.getDescription());
	}
}

void gkOgreParticleObject::destroyInstanceImpl()
{
	if (m_psys)
	{
		Ogre::SceneManager* manager = m_scene->getManager();

		if (!m_scene->isBeingDestroyed())
		{
			if (m_node)
				m_node->detachObject(m_psys);

			manager->destroyParticleSystem(m_psys);
			m_psys = 0;
		}
	}

	gkParticleObject::destroyInstanceImpl();
}


gkGameObject* gkOgreParticleObject::clone(const gkString& name)
{
	gkOgreParticleObject* cl = new gkOgreParticleObject(getInstanceCreator(), name, -1);

	cl->m_particleProps = m_particleProps;

	gkParticleObject::cloneImpl(cl);
	return cl;
}

void gkOgreParticleObject::setMaterialName(const gkString& material)
{
	GK_ASSERT(m_psys);
	if (!m_psys) return;
	
	m_psys->setMaterialName(material);	
}