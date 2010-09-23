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
#ifndef _nsNodePropertyPage_h_
#define _nsNodePropertyPage_h_

#include "nsCommon.h"
#include "nsSingleton.h"
#include "nsEventHandler.h"
#include "nsNodeTypeInfo.h"

#include <wx/panel.h>
#include <wx/propgrid/manager.h>



class nsNodePropertyPage : public wxPropertyGridPage
{
protected:
	nsPropertyManager*   m_manager;
	nsNode*              m_node;


	wxPropertyCategory*  m_type, *m_data;
	wxStringProperty*    m_typename;
	wxStringProperty*    m_groupname;
	wxStringProperty*    m_id;
	wxStringProperty*    m_object;
	virtual bool propertyChanged(wxPGProperty* prop)    {return false;}

public:

	nsNodePropertyPage(nsPropertyManager* manager);
	virtual ~nsNodePropertyPage() {}


	void createProperties(void);
	void selectRoot(void);

	// events
	void propertyChangeEvent(wxPropertyGridEvent& evt);

	// current node this page will operate on
	void setNode(nsNode* node);

	DECLARE_EVENT_TABLE();
};


#endif//_nsNodePropertyPage_h_
