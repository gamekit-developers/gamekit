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


// ----------------------------------------------------------------------------
BEGIN_EVENT_TABLE( nsNodePropertyPage, wxPropertyGridPage )
    EVT_PG_CHANGED(wxID_ANY, nsNodePropertyPage::propertyChangeEvent)
END_EVENT_TABLE()


// ----------------------------------------------------------------------------
nsNodePropertyPage::nsNodePropertyPage(nsPropertyManager *manager, nsNodeType *type)
    :   m_manager(manager), 
        m_node(0), 
        m_nodeType(type),
        m_type(0), 
        m_inputs(0), 
        m_outputs(0), 
        m_typename(0), 
        m_groupname(0), 
        m_id(0),
        m_isEdit(0)
{
}


// ----------------------------------------------------------------------------
void nsNodePropertyPage::propertyChangeEvent(wxPropertyGridEvent &evt)
{
    if (!m_node)
        return;


    wxPGProperty *prop = evt.GetProperty();
    if (prop == m_isEdit)
    {
        if (m_node->getEditOutputs())
            DisableProperty(m_outputs);
        else
            EnableProperty(m_outputs);
        evt.Skip();
        return;
    }

    UTsize pos;
    if ((pos = m_socketMap.find(prop)) != UT_NPOS)
    {
        m_socketMap.at(pos)->setValue(nsVariable((const char *)prop->GetValue().GetString().mb_str()));
        evt.Skip();
    }
}

// ----------------------------------------------------------------------------
void nsNodePropertyPage::createProperties(void)
{
    if (!m_nodeType)
        return;

    // type info 

    m_type = new wxPropertyCategory("Information");
    m_type->SetHelpString(m_nodeType->m_briefHelp.c_str());
    Append(m_type);


    m_typename = new wxStringProperty("Type", wxPG_LABEL, m_nodeType->m_typename.c_str());
    m_typename->SetHelpString("The type of data this node operates on.");
    m_type->AppendChild(m_typename);

    m_groupname = new wxStringProperty("Group", wxPG_LABEL, m_nodeType->m_groupname.c_str());
    m_groupname->SetHelpString("The containing group for menus.");
    m_type->AppendChild(m_groupname);


    m_id = new wxStringProperty("Node Id", wxPG_LABEL, "");
    m_id->SetHelpString("Unique node identifier.");
    m_type->AppendChild(m_id);


    // input category

    m_inputs = new wxPropertyCategory("Inputs");
    m_inputs->SetHelpString("Input sockets that can be connected. Each socket may contain only one input.");
    Append(m_inputs);
    createInputs();


    m_isEdit = new wxBoolProperty("Edit Outputs", wxPG_LABEL, false);
    m_isEdit->SetHelpString("Enable or disable setting of output sockets default values.");
    m_type->AppendChild(m_isEdit);


    // output category

    m_outputs = new wxPropertyCategory("Outputs");
    m_outputs->SetHelpString("Output values for sockets. "
        "Not generaly needed unless a different default output value is wanted.");
    Append(m_outputs);
    createOutputs();


    DisableProperty(m_type);
    DisableProperty(m_outputs);
    DisableProperty(m_inputs);
}


// ----------------------------------------------------------------------------
void nsNodePropertyPage::createGenericSocket(nsSocket *sock, wxPropertyCategory *root)
{
}


// ----------------------------------------------------------------------------
void nsNodePropertyPage::createInputs(void)
{ 
}


// ----------------------------------------------------------------------------
void nsNodePropertyPage::createOutputs(void)     
{ 
}


// ----------------------------------------------------------------------------
wxPGProperty *nsNodePropertyPage::createProperty(nsSocketType *type)
{
    // the main socket property
    // TODO custom property editors

    wxPGProperty *prop = 0;
    prop = new wxBoolProperty(type->m_name.c_str(), wxPG_LABEL,  nsVariable(type->m_default).getValueBool());
    prop->SetHelpString(type->m_briefHelp.c_str());
    return prop;
}
