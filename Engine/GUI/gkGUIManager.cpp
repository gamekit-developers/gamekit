/*
-------------------------------------------------------------------------------
    This file is part of gkGUIManager.
    http://gamekit.googlecode.com/

    Copyright (c) 2012 Alberto Torres Ruiz

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

#include "gkGUIManager.h"

#include <gkFont.h>
#include <gkFontManager.h>
#include <Rocket/Core.h>
#include <Rocket/Controls.h>
#include <Rocket/Debugger.h>

#include "rocket/SystemInterfaceOgre3D.h"
#include "rocket/FileInterfaceOgre3D.h"

UT_IMPLEMENT_SINGLETON(gkGUIManager)

gkGUIManager::gkGUIManager()
{
	// Rocket initialisation.

	Ogre::ResourceGroupManager::getSingleton().createResourceGroup(DEFAULT_ROCKET_RESOURCE_GROUP);

	m_rkOgreSystem = new SystemInterfaceOgre3D();
	Rocket::Core::SetSystemInterface(m_rkOgreSystem);

	Rocket::Core::Initialise();
	Rocket::Controls::Initialise();

	m_rkFileInterface = new FileInterfaceOgre3D();
	Rocket::Core::SetFileInterface(m_rkFileInterface);
}


gkGUIManager::~gkGUIManager()
{
	Rocket::Core::Shutdown();

	delete m_rkOgreSystem;
	delete m_rkFileInterface;

	Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(DEFAULT_ROCKET_RESOURCE_GROUP);
}

void gkGUIManager::loadFont(const gkString& name)
{
	gkFont *fnt = gkFontManager::getSingleton().getByName<gkFont>(name);
	
	if (fnt)
	{
		Rocket::Core::FontDatabase::LoadFontFace((const unsigned char*)fnt->getData(), fnt->getSize());
	}
	else
	{
		Rocket::Core::FontDatabase::LoadFontFace(name.c_str());
	}
}

void gkGUIManager::setDebug(gkGUI* gui)
{
	if (gui)
	{
		if (!Rocket::Debugger::SetContext(gui->getContext()))
			Rocket::Debugger::Initialise(gui->getContext());
	}
	else
		Rocket::Debugger::SetContext(NULL);
}


