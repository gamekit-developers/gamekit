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
#include "gkScene.h"
#include "gkParticleObject.h"
#include "gkParticleRenderer.h"
#include "gkParticleResource.h"
#include "gkParticleManager.h"
#include "gkLogger.h"

#include "OgreSceneManager.h"
#include "OgreParticleSystemManager.h"


gkParticleObject::gkParticleObject(gkInstancedManager* creator, const gkResourceName& name, const gkResourceHandle& handle)
	:	gkGameObject(creator, name, handle, GK_PARTICLES),
		m_psys(0)
{
}

gkParticleObject::~gkParticleObject()
{
}

void gkParticleObject::createInstanceImpl()
{
	gkGameObject::createInstanceImpl();

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

			gkParticleResource* resource = gkParticleManager::getSingleton().getByName<gkParticleResource>(
				gkResourceName(pname, getGroupName()));

			if (resource && resource->isTemplateOwner())
			{
				m_psys->setRenderer(gkParticleRenderer::NAME);
		
				if (resource->getParticleProperties().m_render == gkParticleSettingsProperties::R_HALO)
					m_psys->setMaterialName(gkParticleManager::getSingleton().createOrRetrieveHaloMaterial(m_particleProps.m_material));
				else
					m_psys->setMaterialName(m_particleProps.m_material);
			} //else create by .particle script
		}
	}
	catch (Ogre::Exception& e)
	{
		gkLogMessage("gkParticleObject: " << e.getDescription());
	}
}

void gkParticleObject::destroyInstanceImpl()
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

	gkGameObject::destroyInstanceImpl();
}


gkGameObject* gkParticleObject::clone(const gkString& name)
{
	gkParticleObject* cl = new gkParticleObject(getInstanceCreator(), name, -1);

	cl->m_particleProps = m_particleProps;

	gkGameObject::cloneImpl(cl);
	return cl;
}

void gkParticleObject::setMaterialName(const gkString& material)
{
	GK_ASSERT(m_psys);
	if (!m_psys) return;
	
	m_psys->setMaterialName(material);	
}