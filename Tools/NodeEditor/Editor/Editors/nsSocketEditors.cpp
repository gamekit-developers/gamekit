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
#include <wx/validate.h>
#include "Editors/nsSocketEditors.h"


// ----------------------------------------------------------------------------
//
// BOOLEAN VALUES
//
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
nsBoolProperty::nsBoolProperty(const wxString &label, const wxString &name, nsSocketDef *def)
    :   wxBoolProperty(label, name, def ? def->getValue().get<bool>() : false), m_def(def), m_client(0)
{
}

// ----------------------------------------------------------------------------
void nsBoolProperty::OnSetValue(void)
{
    if (m_client)
        m_client->setValue(GetValue().GetBool());
}

// ----------------------------------------------------------------------------
void nsBoolProperty::attachClientObject(nsSocket *sock)
{
    m_client = sock;
    if (m_client)
        SetValue(m_client->getValue().get<bool>());
}



// ----------------------------------------------------------------------------
//
// INTEGER VALUES
//
// ----------------------------------------------------------------------------



// ----------------------------------------------------------------------------
nsIntProperty::nsIntProperty(const wxString &label, const wxString &name, nsSocketDef *def)
    :   wxIntProperty(label, name, def ? (long)def->getValue().get<int>() : 0), m_def(def), m_client(0), m_lock(false)
{
}

// ----------------------------------------------------------------------------
void nsIntProperty::OnSetValue(void)
{
    if (m_client && !m_lock)
    {
        long d =GetValue().GetInteger();

        if (d < (long)m_def->getMin())
        {
            m_lock = true;
            d = (long)m_def->getMin();
            SetValue(d);
            m_lock = false;
        }
        if (d > (long)m_def->getMax())
        {
            m_lock = true;
            d = (long)m_def->getMax();
            SetValue(d);
            m_lock = false;
        }
        m_client->setValue((int)d);
    }
}

// ----------------------------------------------------------------------------
void nsIntProperty::attachClientObject(nsSocket *sock)
{
    m_client = sock;
    if (m_client)
        SetValue((long)m_client->getValue().get<int>());
}



// ----------------------------------------------------------------------------
//
// REAL VALUES
//
// ----------------------------------------------------------------------------



// ----------------------------------------------------------------------------
nsFloatProperty::nsFloatProperty(const wxString &label, const wxString &name, nsSocketDef *def)
    :   wxFloatProperty(label, name, def ? def->getValue().get<float>() : 0.0), m_def(def), m_client(0), m_lock(false)
{
}

// ----------------------------------------------------------------------------
void nsFloatProperty::OnSetValue(void)
{
    if (m_client && !m_lock)
    {
        double d = GetValue().GetDouble();
        if (d < m_def->getMin())
        {
            m_lock = true;
            d = m_def->getMin();
            SetValue(d);
            m_lock = false;
        }
        if (d > m_def->getMax())
        {
            m_lock = true;
            d = m_def->getMax();
            SetValue(d);
            m_lock = false;
        }

        m_client->setValue((float)d);
    }
}

// ----------------------------------------------------------------------------
void nsFloatProperty::attachClientObject(nsSocket *sock)
{
    m_client = sock;
    if (m_client)
        SetValue((double)m_client->getValue().get<float>());
}


// ----------------------------------------------------------------------------
//
// STRING VALUES
//
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
nsStringProperty::nsStringProperty(const wxString &label, const wxString &name, nsSocketDef *def)
    :   wxStringProperty(label, name, def ? wxString(def->getValue().get<nsString>()) : wxString("")), m_def(def), m_client(0), m_lock(false)
{
}


// ----------------------------------------------------------------------------
void nsStringProperty::OnSetValue(void)
{
    if (m_client && !m_lock)
    {
        nsString str = wxToAscii(GetValue().GetString());
        if (str.size() > m_def->getMax())
        {
            m_lock = true;
            str = str.substr(0, (int)m_def->getMax());
            SetValue(wxString(str));
            m_lock = false;
        }

        m_client->setValue(str);
    }
}

// ----------------------------------------------------------------------------
void nsStringProperty::attachClientObject(nsSocket *sock)
{
    m_client = sock;
    if (m_client)
        SetValue(wxString(m_client->getValue().get<nsString>()));
}



// ----------------------------------------------------------------------------
//
// OBJECT VALUES
//
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
nsObjectProperty::nsObjectProperty(const wxString &label, const wxString &name, nsSocketDef *def)
    :   wxStringProperty(label, name, def ? wxString(def->getValue().get<nsObjectSocketData>().getObject()) : wxString("")),
        m_def(def), m_client(0), m_lock(false)
{
}

// ----------------------------------------------------------------------------
void nsObjectProperty::OnSetValue(void)
{
    if (m_client && !m_lock)
    {
        nsString str = nsString(GetValue().GetString());
        if (str.size() > m_def->getMax())
        {
            m_lock = true;
            str = str.substr(0, (int)m_def->getMax());
            SetValue(wxString(str));
            m_lock = false;
        }

        m_client->setValue(nsObjectSocketData(str));
    }
}

// ----------------------------------------------------------------------------
void nsObjectProperty::attachClientObject(nsSocket *sock)
{
    m_client = sock;
    if (m_client)
        SetValue(wxString(m_client->getValue().get<nsObjectSocketData>().getObject()));
}


NS_IMPLEMENT_SOCKET_DEF(nsBoolSocketDef,    nsBoolProperty);
NS_IMPLEMENT_SOCKET_DEF(nsIntSocketDef,     nsIntProperty);
NS_IMPLEMENT_SOCKET_DEF(nsFloatSocketDef,   nsFloatProperty);
NS_IMPLEMENT_SOCKET_DEF(nsStringSocketDef,  nsStringProperty);
NS_IMPLEMENT_SOCKET_DEF(nsObjectSocketDef,  nsObjectProperty);
