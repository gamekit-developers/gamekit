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
#include "OgreSceneManager.h"
#include "OgreSceneNode.h"


#include "gkCamera.h"
#include "gkScene.h"


// TODO: Should never overide properties created from blender.


// ----------------------------------------------------------------------------
gkCamera::gkCamera(gkScene *scene, const gkString &name)
	:	gkGameObject(scene, name, GK_CAMERA),
	    m_cameraProps(),
	    m_camera(0)
{
}


// ----------------------------------------------------------------------------
void gkCamera::initializeImpl(void)
{
	gkGameObject::initializeImpl();

	GK_ASSERT(!m_camera);



	Ogre::SceneManager *manager = m_scene->getManager();
	m_camera = manager->createCamera(m_name);

	m_camera->setNearClipDistance(m_cameraProps.m_clipstart);
	m_camera->setFarClipDistance(m_cameraProps.m_clipend);
	m_camera->setFOVy(gkDegree(m_cameraProps.m_fov));

	m_node->attachObject(m_camera);


	if (m_cameraProps.m_start)
		m_scene->setMainCamera(this);
}


// ----------------------------------------------------------------------------
void gkCamera::finalizeImpl(void)
{
	GK_ASSERT(m_camera);


	if (!m_scene->isFinalizing())
	{
		Ogre::SceneManager *manager = m_scene->getManager();


		m_node->detachObject(m_camera);
		manager->destroyCamera(m_camera);
	}

	m_camera = 0;
	gkGameObject::finalizeImpl();
}

// ----------------------------------------------------------------------------
void gkCamera::makeCurrent(void)
{
	if (m_camera && m_scene)
		m_scene->setMainCamera(this);
}


// ----------------------------------------------------------------------------
void gkCamera::setClip(gkScalar start, gkScalar end)
{

	if (m_cameraProps.m_clipstart != start)
	{
		m_cameraProps.m_clipstart = start;
		if (m_camera) m_camera->setNearClipDistance(start);
	}

	if (m_cameraProps.m_clipend != end)
	{
		m_cameraProps.m_clipend = end;
		if (m_camera) m_camera->setFarClipDistance(end);
	}
}


// ----------------------------------------------------------------------------
void gkCamera::setFov(const gkRadian &fov)
{

	gkScalar val = fov.valueRadians();

	if (m_cameraProps.m_fov != val)
	{
		m_cameraProps.m_fov = val;
		if (m_camera) m_camera->setFOVy(gkRadian(fov));
	}
}


// ----------------------------------------------------------------------------
void gkCamera::setFov(const gkDegree &fov)
{
	gkScalar val = fov.valueRadians();
	if (m_cameraProps.m_fov != val)
	{
		m_cameraProps.m_fov = val;
		if (m_camera)
			m_camera->setFOVy(gkRadian(fov));
	}
}


// ----------------------------------------------------------------------------
void gkCamera::setMainCamera(bool v)
{
	m_cameraProps.m_start = v;
	if (m_camera)
		m_scene->setMainCamera(this);
}



// ----------------------------------------------------------------------------
gkGameObject *gkCamera::clone(const gkString &name)
{
	gkCamera *cl = new gkCamera(m_scene, name);
	memcpy(&cl->m_cameraProps, &m_cameraProps, sizeof(gkCameraProperties));

	gkGameObject::cloneImpl(cl);
	return cl;
}
