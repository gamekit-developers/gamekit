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
#include "nsSocketEditors.h"


// ----------------------------------------------------------------------------
WX_PG_IMPLEMENT_PROPERTY_CLASS(nsVector2Property,wxPGProperty,
                               NSvec2,const NSvec2&,TextCtrl);

WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(NSvec2);

// ----------------------------------------------------------------------------
nsVector2Property::nsVector2Property(const wxString &label,
                                       const wxString &name,
                                       const NSvec2 &data)
    : wxPGProperty(label, name)
{
    SetValue(WXVARIANT(data));
    AddPrivateChild( new wxFloatProperty("X", wxPG_LABEL,data.x));
    AddPrivateChild( new wxFloatProperty("Y", wxPG_LABEL,data.y));
}


// ----------------------------------------------------------------------------
nsVector2Property::~nsVector2Property()
{
}

// ----------------------------------------------------------------------------
void nsVector2Property::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
    NSvec2 vector;
    vector << thisValue;
    switch ( childIndex )
    {
        case 0: vector.x = childValue.GetDouble(); break;
        case 1: vector.y = childValue.GetDouble(); break;
    }
    thisValue << vector;
}

// ----------------------------------------------------------------------------
void nsVector2Property::RefreshChildren(void)
{
    if ( !GetChildCount() ) return;
    const NSvec2& vector = NSvec2RefFromVariant(m_value);
    Item(0)->SetValue( vector.x );
    Item(1)->SetValue( vector.y );
}



// ----------------------------------------------------------------------------
WX_PG_IMPLEMENT_PROPERTY_CLASS(nsVector3Property,wxPGProperty,
                               NSvec3,const NSvec3&,TextCtrl);

WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(NSvec3);

// ----------------------------------------------------------------------------
nsVector3Property::nsVector3Property(const wxString &label,
                                       const wxString &name,
                                       const NSvec3 &data)
    : wxPGProperty(label, name)
{
    SetValue(WXVARIANT(data));
    AddPrivateChild( new wxFloatProperty("X", wxPG_LABEL,data.x));
    AddPrivateChild( new wxFloatProperty("Y", wxPG_LABEL,data.y));
    AddPrivateChild( new wxFloatProperty("Z", wxPG_LABEL,data.y));
}


// ----------------------------------------------------------------------------
nsVector3Property::~nsVector3Property()
{
}

// ----------------------------------------------------------------------------
void nsVector3Property::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
    NSvec3 vector;
    vector << thisValue;
    switch ( childIndex )
    {
        case 0: vector.x = childValue.GetDouble(); break;
        case 1: vector.y = childValue.GetDouble(); break;
        case 2: vector.z = childValue.GetDouble(); break;
    }
    thisValue << vector;
}

// ----------------------------------------------------------------------------
void nsVector3Property::RefreshChildren(void)
{
    if ( !GetChildCount() ) return;
    const NSvec3& vector = NSvec3RefFromVariant(m_value);
    Item(0)->SetValue( vector.x );
    Item(1)->SetValue( vector.y );
    Item(2)->SetValue( vector.z );
}





// ----------------------------------------------------------------------------
WX_PG_IMPLEMENT_PROPERTY_CLASS(nsVector4Property,wxPGProperty,
                               NSvec4,const NSvec4&,TextCtrl);

WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(NSvec4);

// ----------------------------------------------------------------------------
nsVector4Property::nsVector4Property(const wxString &label,
                                       const wxString &name,
                                       const NSvec4 &data)
    : wxPGProperty(label, name)
{
    SetValue(WXVARIANT(data));
    AddPrivateChild( new wxFloatProperty("X", wxPG_LABEL,data.x));
    AddPrivateChild( new wxFloatProperty("Y", wxPG_LABEL,data.y));
    AddPrivateChild( new wxFloatProperty("Z", wxPG_LABEL,data.z));
    AddPrivateChild( new wxFloatProperty("W", wxPG_LABEL,data.w));
}


// ----------------------------------------------------------------------------
nsVector4Property::~nsVector4Property()
{
}

// ----------------------------------------------------------------------------
void nsVector4Property::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
    NSvec4 vector;
    vector << thisValue;
    switch ( childIndex )
    {
        case 0: vector.x = childValue.GetDouble(); break;
        case 1: vector.y = childValue.GetDouble(); break;
        case 2: vector.z = childValue.GetDouble(); break;
        case 3: vector.w = childValue.GetDouble(); break;
    }
    thisValue << vector;
}

// ----------------------------------------------------------------------------
void nsVector4Property::RefreshChildren(void)
{
    if ( !GetChildCount() ) return;
    const NSvec4& vector = NSvec4RefFromVariant(m_value);
    Item(0)->SetValue( vector.x );
    Item(1)->SetValue( vector.y );
    Item(2)->SetValue( vector.z );
    Item(3)->SetValue( vector.w );
}



// ----------------------------------------------------------------------------
WX_PG_IMPLEMENT_PROPERTY_CLASS(nsQuaternionProperty,wxPGProperty,
                               NSquat,const NSquat&,TextCtrl);

WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(NSquat);

// ----------------------------------------------------------------------------
nsQuaternionProperty::nsQuaternionProperty(const wxString &label,
                                       const wxString &name,
                                       const NSquat &data)
    : wxPGProperty(label, name)
{
    SetValue(WXVARIANT(data));
    AddPrivateChild( new wxFloatProperty("W", wxPG_LABEL,data.w));
    AddPrivateChild( new wxFloatProperty("X", wxPG_LABEL,data.x));
    AddPrivateChild( new wxFloatProperty("Y", wxPG_LABEL,data.y));
    AddPrivateChild( new wxFloatProperty("Z", wxPG_LABEL,data.z));
}


// ----------------------------------------------------------------------------
nsQuaternionProperty::~nsQuaternionProperty()
{
}

// ----------------------------------------------------------------------------
void nsQuaternionProperty::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
    NSquat vector;
    vector << thisValue;
    switch ( childIndex )
    {
        case 0: vector.w = childValue.GetDouble(); break;
        case 1: vector.x = childValue.GetDouble(); break;
        case 2: vector.y = childValue.GetDouble(); break;
        case 3: vector.z = childValue.GetDouble(); break;
    }
    thisValue << vector;
}

// ----------------------------------------------------------------------------
void nsQuaternionProperty::RefreshChildren(void)
{
    if ( !GetChildCount() ) return;
    const NSquat& vector = NSquatRefFromVariant(m_value);
    Item(0)->SetValue( vector.w );
    Item(1)->SetValue( vector.x );
    Item(2)->SetValue( vector.y );
    Item(3)->SetValue( vector.z );
}


