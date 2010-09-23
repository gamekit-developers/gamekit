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
    This is a replacement candidate for Engine/Graphics
    - For now it wraps Ogre::Overlays to gk usable objects.
    - Later it will be superseded by custom Ogre vertex buffer usage,
    - abstracted for external GameKit external rendering.
*/
#include "gkHUDManager.h"
#include "gkHUD.h"
#include "gkHUDElement.h"

#include "OgreOverlayManager.h"
#include "OgreOverlay.h"
#include "OgreOverlayElement.h"
#include "gkLogger.h"


gkHUDManager::gkHUDManager()
	:    gkResourceManager("HUDManager", "HUD")
{
}


gkHUDManager::~gkHUDManager()
{
	destroyAll();
}


gkHUD* gkHUDManager::getOrCreate(const gkHashedString& name)
{
	if (exists(name))
		return (gkHUD*)getByName(name);
	return (gkHUD*)gkResourceManager::create(name);
}


gkResource* gkHUDManager::createImpl(const gkResourceName& name, const gkResourceHandle& handle)
{
	return new gkHUD(this, name, handle);
}



UT_IMPLEMENT_SINGLETON(gkHUDManager);
