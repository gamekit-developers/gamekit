/*
-------------------------------------------------------------------------------
    This file is part of gkGUI.
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

#include "gkWindow.h"
#include "gkGUI.h"

#include <gkFont.h>
#include <gkFontManager.h>
#include <Rocket/Core.h>
#include <Rocket/Controls.h>
#include <Rocket/Debugger.h>

#include "rocket/RenderInterfaceOgre3D.h"
#include "rocket/RocketRenderListener.h"
#include "rocket/RocketEventListener.h"

gkGUI::gkGUI(gkWindow *window):
	m_window(window),
	m_document(0)
{
	
	RenderInterfaceOgre3D *rkOgreRenderer = new RenderInterfaceOgre3D(window->getWidth(), window->getHeight());

	m_rkContext = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(window->getWidth(), window->getHeight()), rkOgreRenderer);

	m_rkEventListener  = new RocketEventListener(window, m_rkContext);
	m_rkRenderListener = new RocketRenderListener(window->getRenderWindow(), m_rkContext);
}

gkGUI::~gkGUI()
{
	Rocket::Core::RenderInterface *ri;

	if (m_rkContext)
	{
		ri = m_rkContext->GetRenderInterface();
		m_rkContext->RemoveReference();
		delete ri;
	}
	delete m_rkEventListener;
	delete m_rkRenderListener;

}
