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
#include "gkCamera.h"
#include "gkScene.h"
#include "OgreSceneManager.h"
#include "OgreSceneNode.h"




gkCamera::gkCamera(gkScene *scene, const gkString& name, gkObject::Loader* loader)
:       gkGameObject(scene, name, GK_CAMERA, loader),
        m_cameraProps(),
        m_camera(0)
{
}



void gkCamera::loadImpl(void)
{
    gkGameObject::loadImpl();

    if (m_camera != 0)
        return;

    Ogre::SceneManager *manager = m_scene->getManager();
    m_camera = manager->createCamera(m_name);

    m_camera->setNearClipDistance(m_cameraProps.clipstart);
    m_camera->setFarClipDistance(m_cameraProps.clipend);
    m_camera->setFOVy(gkDegree(m_cameraProps.fov));

    m_node->attachObject(m_camera);

    if (m_cameraProps.start)
        m_scene->setMainCamera(this);
}


void gkCamera::unloadImpl(void)
{
    if (m_camera != 0)
    {
        // sanity check
        GK_ASSERT(m_scene);
        Ogre::SceneManager *manager = m_scene->getManager();
        GK_ASSERT(manager);

        m_node->detachObject(m_camera);
        manager->destroyCamera(m_camera);
        m_camera = 0;
    }

    gkGameObject::unloadImpl();
}


gkObject *gkCamera::clone(const gkString &name)
{
    // will need to set other properties in a bit!
    return new gkCamera(m_scene, name, m_manual ? m_manual->clone() : 0);
}


void gkCamera::makeCurrent(void)
{
    if (m_camera && m_scene)
        m_scene->setMainCamera(this);
}


void gkCamera::setClip(gkScalar start, gkScalar end)
{
    if (m_cameraProps.clipstart != start)
    {
        m_cameraProps.clipstart = start;
        if (m_camera) m_camera->setNearClipDistance(start);
    }

    if (m_cameraProps.clipend != end)
    {
        m_cameraProps.clipstart = start;
        if (m_camera) m_camera->setNearClipDistance(start);
    }
}


void gkCamera::setFov(const gkRadian& fov)
{
    gkScalar val = fov.valueRadians();

    if (m_cameraProps.fov != val)
    {
        m_cameraProps.fov = val;
        if (m_camera) m_camera->setFOVy(gkRadian(fov));
    }
}


void gkCamera::setFov(const gkDegree& fov)
{
    gkScalar val = fov.valueRadians();
    if (m_cameraProps.fov != val)
    {
        m_cameraProps.fov = val;
        if (m_camera) m_camera->setFOVy(gkRadian(fov));
    }
}


void gkCamera::setMainCamera(bool v)
{
    m_cameraProps.start = v;
    if (m_camera)
        m_scene->setMainCamera(this);
}
