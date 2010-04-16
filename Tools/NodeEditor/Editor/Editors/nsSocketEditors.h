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
#ifndef _nsSocketEditors_h_
#define _nsSocketEditors_h_

#include "nsCommon.h"
#include "nsSocket.h"
#include <wx/propgrid/manager.h>

// ----------------------------------------------------------------------------
class nsBoolProperty : public wxBoolProperty
{
protected:
    nsSocket        *m_client;
    nsSocketDef     *m_def;

    void OnSetValue(void);

public:
    nsBoolProperty(const wxString &label = wxPG_LABEL, const wxString &name = wxPG_LABEL, nsSocketDef *def=0);
    virtual ~nsBoolProperty() {}

    void attachClientObject(nsSocket *sock);
};


// ----------------------------------------------------------------------------
class nsIntProperty : public wxIntProperty
{
protected:
    nsSocket        *m_client;
    nsSocketDef     *m_def;
    bool            m_lock;

    void OnSetValue(void);

public:
    nsIntProperty(const wxString &label = wxPG_LABEL, const wxString &name = wxPG_LABEL, nsSocketDef *def=0);
    virtual ~nsIntProperty() {}

    void attachClientObject(nsSocket *sock);
};


// ----------------------------------------------------------------------------
class nsFloatProperty : public wxFloatProperty
{
protected:
    nsSocket        *m_client;
    nsSocketDef     *m_def;
    bool            m_lock;

    void OnSetValue(void);

public:
    nsFloatProperty(const wxString &label = wxPG_LABEL, const wxString &name = wxPG_LABEL, nsSocketDef *def=0);
    virtual ~nsFloatProperty() {}

    void attachClientObject(nsSocket *sock);
};


// ----------------------------------------------------------------------------
class nsStringProperty : public wxStringProperty
{
protected:
    nsSocket        *m_client;
    nsSocketDef     *m_def;
    bool            m_lock;

    void OnSetValue(void);

public:
    nsStringProperty(const wxString &label = wxPG_LABEL, const wxString &name = wxPG_LABEL, nsSocketDef *def=0);
    virtual ~nsStringProperty() {}

    void attachClientObject(nsSocket *sock);
};



// ----------------------------------------------------------------------------
class nsObjectProperty : public wxStringProperty
{
    // will have sub types later
protected:
    nsSocket        *m_client;
    nsSocketDef     *m_def;
    bool            m_lock;

    void OnSetValue(void);

public:
    nsObjectProperty(const wxString &label = wxPG_LABEL, const wxString &name = wxPG_LABEL, nsSocketDef *def=0);
    virtual ~nsObjectProperty() {}

    void attachClientObject(nsSocket *sock);
};

#endif//_nsSocketEditors_h_
