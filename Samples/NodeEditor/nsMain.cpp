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
#include <wx/app.h>
#include <wx/frame.h>
#include "nsMainWindow.h"

#include "nsRenderSystem.h"
#include "nsNodeTypeInfo.h"
#include "nsNodeManager.h"



class nsApp : public wxApp
{
private:

	nsRenderSystem*  m_renderSystem;
	nsNodeManager*   m_nodeManager;
	nsNodeTypeInfo*  m_typeInfo;


public:
	bool OnInit( void )
	{
		// load node types
		m_typeInfo = new nsNodeTypeInfo();
		m_typeInfo->registerTypes();

		m_renderSystem = new nsRenderSystem();

		// main node manager
		m_nodeManager = new nsNodeManager();

		// load the app
		nsMainWindow* win = new nsMainWindow();
		SetTopWindow(win);
		return true;
	}


	int OnExit(void)
	{
		// remove all
		delete m_nodeManager;
		m_nodeManager = 0;

		delete m_renderSystem;
		m_renderSystem = 0;

		delete m_typeInfo;
		m_typeInfo = 0;

		return 0;
	}

};


IMPLEMENT_APP( nsApp );
