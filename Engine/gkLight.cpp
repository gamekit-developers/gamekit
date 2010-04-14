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


using namespace Ogre;


gkLight::gkLight(gkScene *scene, const gkString& name, gkObject::Loader* loader)
:       gkGameObject(scene, name, GK_LIGHT, loader),
        m_lightProps(), m_light(0)
{
}


void gkLight::updateProperties(void)
{
    if (!m_light)
        return;

    m_light->setCastShadows(m_lightProps.casts);
    m_light->setType((Light::LightTypes)m_lightProps.type);

    m_light->setSpecularColour(m_lightProps.specular);
    m_light->setSpecularColour(m_lightProps.specular);


    if (m_lightProps.extra)
    {
        m_light->setAttenuation(m_lightProps.param[0], m_lightProps.param[1], m_lightProps.param[2], m_lightProps.param[3]);
        m_light->setDiffuseColour(m_lightProps.diffuse);
    }
    else
    {
        m_light->setAttenuation(m_lightProps.range, m_lightProps.constant, m_lightProps.linear, m_lightProps.quadratic);
        m_light->setDiffuseColour(m_lightProps.diffuse * m_lightProps.power);
    }


    m_light->setPowerScale(m_lightProps.power);
    if (m_lightProps.type == Ogre::Light::LT_SPOTLIGHT || m_lightProps.type == Ogre::Light::LT_DIRECTIONAL)
    {
        m_light->setDirection(m_lightProps.direction);
        if (m_lightProps.type == Ogre::Light::LT_SPOTLIGHT)
            m_light->setSpotlightRange(gkDegree(m_lightProps.spot_inner), gkDegree(m_lightProps.spot_outer), m_lightProps.falloff);
    }
}



void gkLight::loadImpl(void)
{
    gkGameObject::loadImpl();

    if (m_light != 0)
        return;

    SceneManager *manager = m_scene->getManager();
    m_light = manager->createLight(m_name);
    m_node->attachObject(m_light);

    updateProperties();
}


void gkLight::unloadImpl(void)
{
    if (m_light != 0)
    {
        SceneManager *manager = m_scene->getManager();
        m_node->detachObject(m_light);
        manager->destroyLight(m_light);
        m_light = 0;
    }

    gkGameObject::unloadImpl();
}

gkObject *gkLight::clone(const gkString &name)
{
    // will need to set other properties in a bit!
    return new gkLight(m_scene, name, m_manual ? m_manual->clone() : 0);
}


