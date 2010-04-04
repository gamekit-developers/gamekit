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
#include "nsSocketEditors.h"


// ----------------------------------------------------------------------------
BEGIN_EVENT_TABLE( nsNodePropertyPage, wxPropertyGridPage )
    EVT_PG_CHANGED(wxID_ANY, nsNodePropertyPage::propertyChangeEvent)
END_EVENT_TABLE()



// ----------------------------------------------------------------------------
static void nsPropToVariable(wxPGProperty *p, nsVariable &var)
{
    wxVariant v = p->GetValue();
    if (v.IsNull())
    {
        var.makeNull();
        return;
    }

    switch (var.getType())
    {
    case nsVariable::VAR_ENUM:
    case nsVariable::VAR_INT:
        var.setValue(p->GetValue().GetInteger());
        break;
    case nsVariable::VAR_REAL:
        var.setValue((NSfloat)p->GetValue().GetDouble());
        break;
    case nsVariable::VAR_STRING:
        var.setValue(wxToSTDString(p->GetValue().GetString()));
        break;
    case nsVariable::VAR_VEC2:
        {
            NSvec2 vec;
            vec << p->GetValue();
            var.setValue(vec);
            break;
        }
    case nsVariable::VAR_VEC3:
        {
            NSvec3 vec;
            vec << p->GetValue();
            var.setValue(vec);
            break;
        }
    case nsVariable::VAR_VEC4:
        {
            NSvec4 vec;
            vec << p->GetValue();
            var.setValue(vec);
            break;
        }
    case nsVariable::VAR_QUAT:
        {
            NSquat vec;
            vec << p->GetValue();
            var.setValue(vec);
            break;
        }
    case nsVariable::VAR_BOOL:
    default:
        var.setValue(p->GetValue().GetBool());
        break;
    }
}

// ----------------------------------------------------------------------------
static void nsVariableToProp(wxPGProperty *p, nsVariable &var)
{
    switch (var.getType())
    {
    case nsVariable::VAR_ENUM:
    case nsVariable::VAR_INT:
        p->SetValue(var.getValueInt());
        break;
    case nsVariable::VAR_REAL:
        p->SetValue(var.getValueReal());
        break;
    case nsVariable::VAR_STRING:
        p->SetValue(var.getValueString().c_str());
        break;
    case nsVariable::VAR_VEC2:
        p->SetValue(WXVARIANT(var.getValueVector2()));
        break;
    case nsVariable::VAR_VEC3:
        p->SetValue(WXVARIANT(var.getValueVector3()));
        break;
    case nsVariable::VAR_VEC4:
        p->SetValue(WXVARIANT(var.getValueVector4()));
        break;
    case nsVariable::VAR_QUAT:
        p->SetValue(WXVARIANT(var.getValueQuaternion()));
        break;
    case nsVariable::VAR_BOOL:
    default:
        p->SetValue(var.getValueBool());
        break;
    }
}


// ----------------------------------------------------------------------------
nsNodePropertyPage::nsNodePropertyPage(nsPropertyManager *manager, nsNodeType *type)
    :   m_manager(manager),
        m_node(0),
        m_nodeType(type),
        m_type(0),
        m_inputs(0),
        m_vars(0),
        m_typename(0),
        m_groupname(0),
        m_id(0)
{
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

    if (!m_nodeType->m_variables.empty())
    {
        // variable category
        wxString lab = wxString::Format("%s Data", m_nodeType->m_typename.c_str());
        m_vars = new wxPropertyCategory(lab);
        m_vars->SetHelpString("Variables attached to this node.");
        Append(m_vars);
        createVariables();
    }

    if (!m_nodeType->m_inputs.empty())
    {
        // input category
        m_inputs = new wxPropertyCategory("Inputs");
        m_inputs->SetHelpString("Input sockets that can be connected. Each socket may contain only one input.");
        Append(m_inputs);
        createInputs();
    }

    DisableProperty(m_type);
}

// ----------------------------------------------------------------------------
void nsNodePropertyPage::propertyChangeEvent(wxPropertyGridEvent &evt)
{
    if (!m_node)
        return;


    wxPGProperty *prop = evt.GetProperty();
    if (m_inputs)
    {
        // Find property in inputs


        int index = m_inputs->Index(prop);
        if (index != wxNOT_FOUND)
        {
            void *client = prop->GetClientData();
            if (client)
            {
                nsSocket *sock = static_cast<nsSocket *>(client);
                nsVariable &var = sock->getValue();

                nsPropToVariable(prop, var);

                sock->setValue(var);
                evt.Skip();

                return;
            }
        }
    }


    if (m_vars)
    {
        // Find property in variables

        int index = m_vars->Index(prop);
        if (index != wxNOT_FOUND)
        {
            void *client = prop->GetClientData();
            if (client)
            {
                nsNodeData *data = static_cast<nsNodeData *>(client);

                nsVariable &var = data->getValue();
                nsPropToVariable(prop, var);

                data->setValue(var);
                evt.Skip();
            }
        }
    }
}


// ----------------------------------------------------------------------------
void nsNodePropertyPage::socketEvent(nsSocketEvent &evt)
{
    nsSocket *sock = evt.ptr();

    if (sock->isInput() && m_inputs)
    {
        unsigned int  nr = m_inputs->GetChildCount();
        for (unsigned int i=0; i<nr; i++)
        {

            wxPGProperty *prop = m_inputs->Item(i);
            if (prop && prop->GetClientData() == sock)
            {
                // Disable if it's linked to another
                EnableProperty(prop, evt.getId() == NS_SOCKET_UNLINK);
            }
        }
    }
}


// ----------------------------------------------------------------------------
void nsNodePropertyPage::createInputs(void)
{
    if (!m_nodeType->m_inputs.empty() && m_inputs)
    {
        nsNodeType::SocketIterator iter = nsNodeType::SocketIterator(m_nodeType->m_inputs);
        while (iter.hasMoreElements())
        {
            nsSocketType *type = iter.getNext();
            wxPGProperty *prop = createProperty(type->m_type, type->m_name, type->m_default, type->m_briefHelp);

            prop->SetHelpString(type->m_briefHelp.c_str());
            m_inputs->AppendChild(prop);
        }
    }
}

// ----------------------------------------------------------------------------
void nsNodePropertyPage::createVariables(void)
{
    if (!m_nodeType->m_variables.empty() && m_vars)
    {
        nsVariableIterator vars(m_nodeType->m_variables);
        while (vars.hasMoreElements())
        {
            nsVariableType *varType = vars.getNext();

            wxPGProperty *prop = createProperty(varType->m_type,
                                                varType->m_name,
                                                varType->m_value,
                                                varType->m_briefHelp,
                                                &varType->m_enum);
            m_vars->AppendChild(prop);
        }
    }

}

// ----------------------------------------------------------------------------
void nsNodePropertyPage::selectRoot(void)
{
    // select the information property
    SetSelection(m_type);
}

// ----------------------------------------------------------------------------
void nsNodePropertyPage::setNode(nsNode *node)
{
    if (m_node && m_node == node)
        return;


    m_node = node;
    if (m_node && m_inputs)
    {
        EnableProperty(m_inputs, node != 0);

        // setup the ID
        m_id->SetValue(wxString::Format("%p", m_node));


        nsSocket *sock = m_node->getFirstInput();
        while (sock)
        {
            nsSocketType *type = sock->getType();

            // apply client information
            wxPGProperty *prop = m_inputs->GetPropertyByName(type->m_name.c_str());
            prop->SetClientData(sock);


            nsVariable &var = sock->getValue();
            nsVariableToProp(prop, var);

            if (!sock->isConnected())
            {
                if (!IsPropertyEnabled(prop))
                    EnableProperty(prop);
            }
            else
            {
                if (IsPropertyEnabled(prop))
                    DisableProperty(prop);
            }
            sock = sock->getNext();
        }
    }
    if (node && m_vars)
    {
        EnableProperty(m_vars, node != 0);

        nsNodeData *dt = m_node->getFirstVariable();
        while (dt)
        {
            nsVariableType *vt = dt->getType();

            // apply client information
            wxPGProperty *prop = m_vars->GetPropertyByName(vt->m_name.c_str());
            prop->SetClientData(dt);

            nsVariable &var = dt->getValue();
            nsVariableToProp(prop, var);

            dt = dt->getNext();
        }
    }
}


// ----------------------------------------------------------------------------
wxPGProperty *nsNodePropertyPage::createProperty(
    nsVariable::PropertyTypes type,
    const utString &name,
    const utString &value,
    const utString &help,
    void *enumValue)
{
    wxPGProperty *prop = 0;
    switch (type)
    {
    case nsVariable::VAR_INT:
        prop = new wxIntProperty(name.c_str(), wxPG_LABEL, nsVariable(value).getValueInt());
        break;
    case nsVariable::VAR_REAL:
        prop = new wxFloatProperty(name.c_str(), wxPG_LABEL, nsVariable(value).getValueReal());
        break;
    case nsVariable::VAR_STRING:
        prop = new wxStringProperty(name.c_str(), wxPG_LABEL, value.c_str());
        break;
    case nsVariable::VAR_VEC2:
        prop = new nsVector2Property(name.c_str(), wxPG_LABEL, nsVariable(value).getValueVector2());
        break;
    case nsVariable::VAR_VEC3:
        prop = new nsVector3Property(name.c_str(), wxPG_LABEL, nsVariable(value).getValueVector3());
        break;
    case nsVariable::VAR_VEC4:
        prop = new nsVector4Property(name.c_str(), wxPG_LABEL, nsVariable(value).getValueVector4());
        break;
    case nsVariable::VAR_QUAT:
        prop = new nsQuaternionProperty(name.c_str(), wxPG_LABEL, nsVariable(value).getValueQuaternion());
        break;
    case nsVariable::VAR_ENUM:
        {
            if (enumValue)
            {
                wxArrayString lab;
                wxArrayInt val;

                nsEnumItemIterator iter(*static_cast<nsEnumItems *>(enumValue));
                while (iter.hasMoreElements())
                {
                    // extract labels and the id
                    nsEnumItem &item = iter.getNext();

                    lab.push_back(item.m_name.c_str());
                    val.push_back(item.m_value);
                }

                prop = new wxEnumProperty(name.c_str(), wxPG_LABEL, lab, val);
            }
        } break;
    case nsVariable::VAR_BOOL:
    default:
        prop = new wxBoolProperty(name.c_str(), wxPG_LABEL,  nsVariable(value).getValueBool());
        break;
    }

    prop->SetHelpString(help.c_str());
    return prop;
}
