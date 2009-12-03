/*
-------------------------------------------------------------------------------
	This file is part of the Ogre GameKit port.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#include "gkCameraObject.h"
#include "gkSceneObject.h"
#include "OgreSceneManager.h"
#include "OgreCamera.h"
#include "OgreSceneNode.h"



using namespace Ogre;


// ----------------------------------------------------------------------------
gkCameraObject::gkCameraObject(gkSceneObject *scene, const String& name, gkManualObjectLoader* loader) :
		gkGameObject(scene, name, GK_CAMERA, loader),
		mCameraProps(),
		mCamera(0)
{
}

// ----------------------------------------------------------------------------
void gkCameraObject::loadImpl(void)
{
	gkGameObject::loadImpl();

	if (mCamera != 0)
		return;

	/// sanity check
	GK_ASSERT(mScene);
	SceneManager *manager= mScene->getManager();
	GK_ASSERT(manager);

	mCamera= manager->createCamera(mName);

	mCamera->setNearClipDistance(mCameraProps.clipstart);
	mCamera->setFarClipDistance(mCameraProps.clipend);
	mCamera->setFOVy(mCameraProps.fov);

	mNode->attachObject(mCamera);

	if (mCameraProps.start)
		mScene->setMainCamera(this);
}

// ----------------------------------------------------------------------------
void gkCameraObject::unloadImpl(void)
{
	if (mCamera != 0)
	{
		/// sanity check
		GK_ASSERT(mScene);
		SceneManager *manager= mScene->getManager();
		GK_ASSERT(manager);

		mNode->detachObject(mCamera);
		manager->destroyCamera(mCamera);
		mCamera= 0;
	}

	gkGameObject::unloadImpl();
}


// ----------------------------------------------------------------------------
void gkCameraObject::makeCurrent(void)
{
	if (mCamera && mScene)
		mScene->setMainCamera(this);
}

// ----------------------------------------------------------------------------
void gkCameraObject::setClip(Real start, Real end)
{
	if (mCameraProps.clipstart != start)
	{
		mCameraProps.clipstart= start;
		if (mCamera) mCamera->setNearClipDistance(start);
	}

	if (mCameraProps.clipend != end)
	{
		mCameraProps.clipstart= start;
		if (mCamera) mCamera->setNearClipDistance(start);
	}
}

// ----------------------------------------------------------------------------
void gkCameraObject::setFov(const Radian& fov)
{
	if (mCameraProps.fov != fov)
	{
		mCameraProps.fov= fov;
		if (mCamera) mCamera->setFOVy(fov);
	}
}

// ----------------------------------------------------------------------------
void gkCameraObject::setFov(const Degree& fov)
{
	Radian valRad= Radian(fov);
	if (mCameraProps.fov != valRad)
	{
		mCameraProps.fov= valRad;
		if (mCamera) mCamera->setFOVy(valRad);
	}
}

// ----------------------------------------------------------------------------
void gkCameraObject::setMainCamera(bool v)
{
	mCameraProps.start= v;
	if (mCamera)
		mScene->setMainCamera(this);
}
