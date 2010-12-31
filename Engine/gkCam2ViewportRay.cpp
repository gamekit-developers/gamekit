/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) Nestor Silveira.

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
#include "gkCam2ViewportRay.h"
#include "gkEngine.h"
#include "gkCamera.h"
#include "gkScene.h"
#include "gkWindowSystem.h"
#include "gkWindow.h"
#include "gkDynamicsWorld.h"
#include "OgreRenderWindow.h"

gkCam2ViewportRay::gkCam2ViewportRay(gkScalar x, gkScalar y, gkScalar rayLength)
{
	gkScene* pScene = gkEngine::getSingleton().getActiveScene();

	GK_ASSERT(pScene);

	gkCamera* pCamera = pScene->getMainCamera();

	GK_ASSERT(pCamera);

	gkVector2 pos(x, y);
	
	gkWindow* pWindow = pScene->getDisplayWindow();
	if (pWindow == 0)
		pWindow = gkWindowSystem::getSingleton().getMainWindow();

	GK_ASSERT(pWindow);

	gkScalar width = pWindow->getWidth();

	gkScalar height = pWindow->getHeight();

	GK_ASSERT(width && height);

	Ogre::Ray ray = pCamera->getCamera()->getCameraToViewportRay(pos.x / width, pos.y / height);

	gkVector3 p0 = ray.getOrigin();

	gkVector3 p1 = p0 + ray.getDirection() * rayLength;

	setOrigin(p0);
	setDirection(p1 - p0);
}

gkCam2ViewportRay::~gkCam2ViewportRay()
{
}
