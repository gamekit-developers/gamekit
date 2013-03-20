/*
-------------------------------------------------------------------------------
    This file is part of gkGUI.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Alberto Torres Ruiz

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

#ifndef _gkGUI_h_
#define _gkGUI_h_

#define DEFAULT_ROCKET_RESOURCE_GROUP "Rocket"

#include <Rocket/Core.h>
#include "utSingleton.h"
#include "gkCommon.h"

class RocketRenderListener;
class RocketEventListener;

class gkGUI
{
protected:

	RocketRenderListener*	m_rkRenderListener;
	RocketEventListener*	m_rkEventListener;	
	Rocket::Core::Context*	m_rkContext;
	gkWindow*		m_window;
	Rocket::Core::ElementDocument* m_document;

public:
	gkGUI(gkWindow* window);
	~gkGUI();
	
	GK_INLINE Rocket::Core::Context* getContext() {return m_rkContext;}
};

#endif // _gkGUI_h_

