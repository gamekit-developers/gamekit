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
#ifndef _nsPropertyPage_h_
#define _nsPropertyPage_h_

#include "nsCommon.h"
#include "nsSingleton.h"
#include "nsEventHandler.h"
#include "utTypes.h"

#include <wx/panel.h>
#include <wx/propgrid/manager.h>

class nsPropertyManager;
class nsToolBar;
class nsTreePropertyPage;
class nsNodePropertyPage;


class nsPropertyPage :  public wxPanel,
	public nsNotifierBase,
	public nsSingleton<nsPropertyPage>

{
protected:

	nsToolBar*               m_tool;
	nsPropertyManager*       m_manager;

	wxPropertyGridPage*      m_default;
	nsTreePropertyPage*      m_tree;
	nsNodePropertyPage*      m_nodeTypes;


public:
	nsPropertyPage(wxWindow* parent);
	virtual ~nsPropertyPage();

	// one time init
	void initialize(void);

	// tree notifications
	void treeEvent(nsTreeEvent& evt);

	// node notifications
	void nodeEvent(nsNodeEvent& evt);

	NS_DECLARE_SINGLETON(nsPropertyPage)

};

#endif//_nsSolutionBrowser_h_
