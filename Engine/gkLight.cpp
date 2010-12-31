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
#include "gkLight.h"
#include "OgreLight.h"
#include "OgreSceneManager.h"
#include "OgreSceneNode.h"
#include "gkScene.h"




gkLight::gkLight(gkInstancedManager* creator, const gkResourceName& name, const gkResourceHandle& handle)
	:    gkGameObject(creator, name, handle, GK_LIGHT),
	     m_lightProps(), m_light(0)
{
}




void gkLight::updateProperties(void)
{
	if (!m_light)
		return;

	m_light->setCastShadows(m_lightProps.m_casts);
	m_light->setSpecularColour(m_lightProps.m_specular);
	m_light->setSpecularColour(m_lightProps.m_specular);
	m_light->setDiffuseColour(m_lightProps.m_diffuse * m_lightProps.m_power);
	m_light->setAttenuation(m_lightProps.m_range, m_lightProps.m_constant, m_lightProps.m_linear, m_lightProps.m_quadratic);

	m_light->setPowerScale(m_lightProps.m_power);
	if (m_lightProps.m_type == gkLightProperties::LI_SPOT || m_lightProps.m_type == gkLightProperties::LI_DIR)
	{
		m_light->setType(Ogre::Light::LT_DIRECTIONAL);
		m_light->setDirection(m_lightProps.m_direction);

		if (m_lightProps.m_type == gkLightProperties::LI_SPOT)
		{
			m_light->setType(Ogre::Light::LT_SPOTLIGHT);
			m_light->setSpotlightRange(gkDegree(m_lightProps.m_spot.x), gkDegree(m_lightProps.m_spot.y), m_lightProps.m_falloff);
		}
	}
	else
		m_light->setType(Ogre::Light::LT_POINT);
}




void gkLight::createInstanceImpl(void)
{
	gkGameObject::createInstanceImpl();

	GK_ASSERT(!m_light);

	Ogre::SceneManager* manager = m_scene->getManager();

	m_light = manager->createLight(m_name.getName());
	m_node->attachObject(m_light);

	updateProperties();
}



void gkLight::destroyInstanceImpl(void)
{
	GK_ASSERT(m_light);


	if (!m_scene->isBeingDestroyed())
	{
		Ogre::SceneManager* manager = m_scene->getManager();

		m_node->detachObject(m_light);
		manager->destroyLight(m_light);
	}

	m_light = 0;

	gkGameObject::destroyInstanceImpl();
}



gkGameObject* gkLight::clone(const gkString& name)
{
	gkLight* cl = new gkLight(getInstanceCreator(), name, -1);
	cl->m_lightProps = m_lightProps;

	gkGameObject::cloneImpl(cl);
	return cl;
}
