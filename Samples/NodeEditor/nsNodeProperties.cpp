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
#include "nsPropertyPage.h"
#include "nsTreeProperties.h"
#include "nsNodeProperties.h"
#include "nsNodeManager.h"


BEGIN_EVENT_TABLE( nsNodePropertyPage, wxPropertyGridPage )
	EVT_PG_CHANGED(wxID_ANY, nsNodePropertyPage::propertyChangeEvent)
END_EVENT_TABLE()



nsNodePropertyPage::nsNodePropertyPage(nsPropertyManager* manager)
	:   m_manager(manager),
	    m_node(0),
	    m_type(0),
	    m_data(0),
	    m_typename(0),
	    m_groupname(0),
	    m_id(0),
	    m_object(0)
{
}


void nsNodePropertyPage::createProperties(void)
{
	// type info

	m_type = new wxPropertyCategory("Information");
	Append(m_type);


	m_typename = new wxStringProperty("Type", wxPG_LABEL, "");
	m_typename->SetHelpString("The type of data this node will operate on.");
	m_type->AppendChild(m_typename);


	m_groupname = new wxStringProperty("Group", wxPG_LABEL, "");
	m_groupname->SetHelpString("The containing group for menus.");
	m_type->AppendChild(m_groupname);


	m_id = new wxStringProperty("Node Id", wxPG_LABEL, "");
	m_id->SetHelpString("Unique node identifier.");
	m_type->AppendChild(m_id);


	m_object =  new wxStringProperty("Attached Object", wxPG_LABEL, "");
	m_object->SetHelpString("Attach a game object to this node. Blank to inherent from owning tree.");
	m_type->AppendChild(m_object);

	DisableProperty(m_type);
	EnableProperty(m_object);
}


void nsNodePropertyPage::propertyChangeEvent(wxPropertyGridEvent& evt)
{
	if (!m_node)
		return;


	wxPGProperty* prop = evt.GetProperty();
	if (propertyChanged(prop))
		evt.Skip();
	else if (m_object == prop)
	{
		wxString str = prop->GetValue().GetString();
		m_node->setAttachedName(wxToSTDString(str));
		evt.Skip();
	}
}




void nsNodePropertyPage::selectRoot(void)
{
	// select the information property	
	DoSetSelection(m_type);
}


void nsNodePropertyPage::setNode(nsNode* node)
{
	if (m_node && m_node == node)
		return;

	m_node = node;

	if (m_node != 0)
	{
		// setup the ID
		m_id->SetValue(wxString::Format("%p", m_node));


		m_typename->SetValue(wxString::Format("%s", m_node->getType()->getName().c_str()));
		m_groupname->SetValue(wxString::Format("%s",
		                                       nsNodeTypeInfo::getSingleton().getGroupName(m_node->getType()->getGroup()).c_str()
		                                      ));

		// apply object name
		m_object->SetValue(wxString(m_node->getAttachedName()));
	}
}
