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
#include "nsNodeManager.h"


BEGIN_EVENT_TABLE( nsTreePropertyPage, wxPropertyGridPage )
	EVT_PG_CHANGED(NS_WID_PROPERTY_DATA, nsTreePropertyPage::propertyChangeEvent)
END_EVENT_TABLE()



nsTreePropertyPage::nsTreePropertyPage(nsPropertyManager* manager)
	:   m_manager(manager), m_tree(0),
	    m_name(0), m_groupname(0), m_id(0), m_isGroup(0), m_info(0), m_group(0), m_object(0)
{
}


void nsTreePropertyPage::setTree(nsNodeTree* tree)
{
	m_tree = tree;
	if (m_tree)
	{
		m_name->SetValue(wxString(m_tree->getName().c_str()));

		wxString id = wxString::Format("%p", m_tree);
		m_id->SetValue(id);

		if (m_object)
			m_object->SetValue(wxString(m_tree->getAttachedName()));


		EnableProperty(m_group);
		if (m_tree->isGroup())
		{
			m_isGroup->SetValue(true);
			m_groupname->SetValue(wxString(m_tree->getGroupName().c_str()));
		}
		else
		{
			DisableProperty(m_groupname);
			m_isGroup->SetValue(false);
			m_groupname->SetValue(wxEmptyString);
		}
	}
	else
	{
		DisableProperty(m_group);
	}
}



void nsTreePropertyPage::createProperties(void)
{
	// create default / initial properties


	m_info = new wxPropertyCategory("Node Tree");
	m_info->SetHelpString("Single node tree that will execute per frame.");
	Append(m_info);

	m_name = new wxStringProperty("Tree Name", wxPG_LABEL, "");
	m_info->AppendChild(m_name);

	m_id = new wxStringProperty("Id", wxPG_LABEL, "");
	m_info->AppendChild(m_id);


	m_object = new wxStringProperty("Attached Object", wxPG_LABEL, "");
	m_info->AppendChild(m_object);


	// group settings

	m_group = new wxPropertyCategory("Grouping");
	Append(m_group);

	m_groupname = new wxStringProperty("Group Name", wxPG_LABEL, "");
	m_group->AppendChild(m_groupname);


	m_isGroup = new wxBoolProperty("IsGroup", wxPG_LABEL, "");
	m_group->AppendChild(m_isGroup);

	// read only
	DisableProperty(m_group);
	DisableProperty(m_info);
	EnableProperty(m_object);
}



void nsTreePropertyPage::propertyChangeEvent(wxPropertyGridEvent& evt)
{
	// data change notification
	if (!m_tree)
		return;

	wxPGProperty* prop = evt.GetProperty();


	if (prop == m_isGroup)
	{
		bool group = prop->GetValue().GetBool();
		m_tree->setGroup(group);
		if (group)
			EnableProperty(m_groupname);
		else
		{
			m_groupname->SetValue(wxEmptyString);
			DisableProperty(m_groupname);
		}
		evt.Skip();
	}
	else if (prop == m_groupname)
	{
		wxString str = prop->GetValue().GetString();
		if (!str.empty() && m_isGroup->GetValue().GetBool())
			m_tree->setGroupName(nsString(str));
		evt.Skip();
	}
	else if (prop == m_object)
	{
		wxString str = prop->GetValue().GetString();
		m_tree->setAttachedName(nsString(str));
	}
}
