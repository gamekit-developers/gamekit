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
#include "nsSingleton.h"
#include "nsEventHandler.h"
#include "nsVariable.h"
#include "Utils/utTypes.h"
#include <wx/panel.h>
#include <wx/propgrid/manager.h>



// Editor for NSvec2 properties
WX_PG_DECLARE_VARIANT_DATA(NSvec2);

class nsVector2Property : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(nsVector2Property)
private:

    // op data
    NSvec2 m_data;

public:
    nsVector2Property(const wxString& label = wxPG_LABEL,
                       const wxString& name = wxPG_LABEL,
                       const NSvec2 &data = NSvec2());
    virtual ~nsVector2Property();

    // virtuals
    virtual void ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const;
    virtual void RefreshChildren(void);
};


WX_PG_DECLARE_VARIANT_DATA(NSvec3);

class nsVector3Property : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(nsVector3Property)
private:

    // op data
    NSvec3 m_data;

public:
    nsVector3Property(const wxString& label = wxPG_LABEL,
                       const wxString& name = wxPG_LABEL,
                       const NSvec3 &data = NSvec3());
    virtual ~nsVector3Property();

    // virtuals
    virtual void ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const;
    virtual void RefreshChildren(void);
};




WX_PG_DECLARE_VARIANT_DATA(NSvec4);

class nsVector4Property : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(nsVector4Property)
private:

    // op data
    NSvec4 m_data;

public:
    nsVector4Property(const wxString& label = wxPG_LABEL,
                       const wxString& name = wxPG_LABEL,
                       const NSvec4 &data = NSvec4());
    virtual ~nsVector4Property();

    // virtuals
    virtual void ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const;
    virtual void RefreshChildren(void);
};




WX_PG_DECLARE_VARIANT_DATA(NSquat);

class nsQuaternionProperty : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(nsQuaternionProperty)
private:

    // op data
    NSquat m_data;

public:
    nsQuaternionProperty(const wxString& label = wxPG_LABEL,
                       const wxString& name = wxPG_LABEL,
                       const NSquat &data = NSquat());
    virtual ~nsQuaternionProperty();

    // virtuals
    virtual void ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const;
    virtual void RefreshChildren(void);
};


#endif//_nsSocketEditors_h_
