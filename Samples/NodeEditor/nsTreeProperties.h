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
#ifndef _nsTreePropertyPage_h_
#define _nsTreePropertyPage_h_

#include "nsCommon.h"
#include "nsSingleton.h"
#include "nsEventHandler.h"
#include "utTypes.h"
#include <wx/panel.h>
#include <wx/propgrid/manager.h>



class nsTreePropertyPage : public wxPropertyGridPage
{
protected:
	nsPropertyManager*   m_manager;
	nsNodeTree*          m_tree;

	wxStringProperty*    m_name;
	wxStringProperty*    m_groupname;
	wxStringProperty*    m_id;
	wxBoolProperty*      m_isGroup;
	wxStringProperty*    m_object;

	wxPropertyCategory*  m_info;
	wxPropertyCategory*  m_group;

public:
	nsTreePropertyPage(nsPropertyManager* manager);
	virtual ~nsTreePropertyPage() {}

	// create default / initial properties
	void createProperties(void);

	// data change notification
	void propertyChangeEvent(wxPropertyGridEvent& evt);

	void setTree(nsNodeTree* tree);

	// tree table
	DECLARE_EVENT_TABLE();
};

#endif//_nsTreePropertyPage_h_
