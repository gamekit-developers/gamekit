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
#include "gkLightObject.h"
#include "OgreLight.h"
#include "OgreSceneManager.h"
#include "OgreSceneNode.h"
#include "gkSceneObject.h"


using namespace Ogre;


// ----------------------------------------------------------------------------
gkLightObject::gkLightObject(gkSceneObject *scene, const String& name, gkManualObjectLoader* loader):
		gkGameObject(scene, name, GK_LIGHT, loader),
		mLightProps(), mLight(0)
{
}

// ----------------------------------------------------------------------------
void gkLightObject::_update(void)
{
	if (!mLight)
		return;

	mLight->setCastShadows(mLightProps.casts);
	mLight->setType((Light::LightTypes)mLightProps.type);

	mLight->setSpecularColour(mLightProps.specular);
	mLight->setSpecularColour(mLightProps.specular);

	if (mLightProps.extra) /// HACK, four unique props to pass data to shaders
	{
		mLight->setAttenuation(mLightProps.param[0], mLightProps.param[1], mLightProps.param[2], mLightProps.param[3]);
		mLight->setDiffuseColour(mLightProps.diffuse);
	}
	else
	{
		mLight->setAttenuation(mLightProps.range, mLightProps.constant, mLightProps.linear, mLightProps.quadratic);
		mLight->setDiffuseColour(mLightProps.diffuse * mLightProps.power);
	}

	mLight->setPowerScale(mLightProps.power);
	if (mLightProps.type == Ogre::Light::LT_SPOTLIGHT || mLightProps.type == Ogre::Light::LT_DIRECTIONAL)
	{
		mLight->setDirection(mLightProps.direction);
		if (mLightProps.type == Ogre::Light::LT_SPOTLIGHT)
			mLight->setSpotlightRange(mLightProps.spot_inner, mLightProps.spot_outer, mLightProps.falloff);
	}
}


// ----------------------------------------------------------------------------
void gkLightObject::loadImpl(void)
{
	gkGameObject::loadImpl();

	if (mLight != 0)
		return;

	/// sanity check
	GK_ASSERT(mScene);
	SceneManager *manager= mScene->getManager();
	GK_ASSERT(manager);

	mLight= manager->createLight(mName);
	mNode->attachObject(mLight);

	_update();
}

// ----------------------------------------------------------------------------
void gkLightObject::unloadImpl(void)
{
	if (mLight != 0)
	{
		/// sanity check
		GK_ASSERT(mScene);
		SceneManager *manager= mScene->getManager();
		GK_ASSERT(manager);

		mNode->detachObject(mLight);
		manager->destroyLight(mLight);
		mLight= 0;
	}
	gkGameObject::unloadImpl();
}
