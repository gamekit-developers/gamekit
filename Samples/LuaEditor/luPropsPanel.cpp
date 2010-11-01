/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 harkon.kr

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


#include "StdAfx.h"
#include "luUtils.h"
#include "luPropsPanel.h"
#include "luMainFrame.h"

#define PANEL_NAME			"Properties"

#define PNAME_PROJECT_NAME	"ProjectName"
#define PNAME_WINSIZE_X		"WinSizeX"
#define PNAME_WINSIZE_Y		"WinSizeY"
#define PNAME_FULLSCREEN	"FullScreen"

BEGIN_EVENT_TABLE(luPropsPanel, wxPanel)
	EVT_PG_CHANGED( -1, luPropsPanel::OnPropertyGridChanged )
	END_EVENT_TABLE()

	WX_PG_DECLARE_VARIANT_DATA(gkVector3)

	WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(gkVector3)


class luVector3Property : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(luVector3Property)
public:

	luVector3Property( const wxString& label = wxPG_LABEL,
		const wxString& name = wxPG_LABEL,
		const gkVector3& value = gkVector3() );
	virtual ~luVector3Property();

	wxVariant ChildChanged( wxVariant& thisValue,
		int childIndex, wxVariant& childValue ) const;
	virtual void RefreshChildren();

protected:
};

WX_PG_IMPLEMENT_PROPERTY_CLASS(luVector3Property,wxPGProperty, gkVector3,const gkVector3&,TextCtrl)


	luVector3Property::luVector3Property( const wxString& label, const wxString& name, const gkVector3& value )
	: wxPGProperty(label,name)
{
	SetValue( WXVARIANT(value) );
	AddPrivateChild( new wxFloatProperty(wxT("X"),wxPG_LABEL,value.x) );
	AddPrivateChild( new wxFloatProperty(wxT("Y"),wxPG_LABEL,value.y) );
	AddPrivateChild( new wxFloatProperty(wxT("Z"),wxPG_LABEL,value.z) );
}

luVector3Property::~luVector3Property() { }

void luVector3Property::RefreshChildren()
{
	if ( !GetChildCount() ) return;
	const gkVector3& vector = gkVector3RefFromVariant(m_value);
	Item(0)->SetValue( vector.x );
	Item(1)->SetValue( vector.y );
	Item(2)->SetValue( vector.z );
}

wxVariant  luVector3Property::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
	gkVector3 vector;
	vector << thisValue;
	switch ( childIndex )
	{
	case 0: vector.x = childValue.GetDouble(); break;
	case 1: vector.y = childValue.GetDouble(); break;
	case 2: vector.z = childValue.GetDouble(); break;
	}
	return thisValue << vector;
}


//--

struct luGameObjProps
{
	gkString name;
	gkString type;
	gkVector3 pos;

	luGameObjProps(gkGameObject* obj = NULL) : pos(gkVector3::ZERO)
	{
		if (!obj) return;
		name = obj->getName();
		type = getLuObjectTypeName(obj->getType());
		pos = obj->getPosition();
	}
};

WX_PG_DECLARE_VARIANT_DATA(luGameObjProps)

WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(luGameObjProps)


class luObjectProperty : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(luObjectProperty)
public:
	luObjectProperty(const wxString& label = wxPG_LABEL, const wxString& name = wxPG_LABEL) :
	wxPGProperty(label, name)
	{
		AddPrivateChild( new wxStringProperty("Name", wxPG_LABEL) );
		AddPrivateChild( new wxStringProperty("Type", wxPG_LABEL) );
		AddPrivateChild( new luVector3Property("Pos"));
	}

	wxVariant ChildChanged( wxVariant& thisValue,
		int childIndex, wxVariant& childValue ) const;
	virtual void RefreshChildren();
};

WX_PG_IMPLEMENT_PROPERTY_CLASS(luObjectProperty,wxPGProperty, luGameObjProps,const luGameObjProps&,TextCtrl)

void luObjectProperty::RefreshChildren()
{
	if ( !GetChildCount() ) return;

	const luGameObjProps& obj = luGameObjPropsRefFromVariant(m_value);
	Item(0)->SetValue( obj.name );
	Item(1)->SetValue( obj.type );
	Item(2)->SetValue( WXVARIANT(obj.pos) );
}

wxVariant  luObjectProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
	luGameObjProps obj;
	obj << thisValue;
	switch ( childIndex )
	{
	case 0: { obj.name = childValue.GetString(); break; }
	case 1: { obj.type = childValue.GetString(); break; }
	case 2: 
		{
			const gkVector3& vector = gkVector3RefFromVariant(childValue);
			obj.pos = vector; break;
		}
	}
	return thisValue << obj;
}


luPropsPanel::luPropsPanel(wxWindow* parent) :
	wxPanel(parent, ID_PROPS_PANEL, wxDefaultPosition, wxDefaultSize),
	m_prop(NULL),
	m_obj(NULL)
{
	m_prop = new wxPropertyGrid(this,ID_PROPS_PROJ,wxDefaultPosition,wxSize(400,400),
		wxPG_SPLITTER_AUTO_CENTER |
		wxPG_BOLD_MODIFIED );


	m_prop->Append( new wxStringProperty(PNAME_PROJECT_NAME, wxPG_LABEL) );
	m_prop->Append( new wxIntProperty(PNAME_WINSIZE_X, wxPG_LABEL) );
	m_prop->Append( new wxIntProperty(PNAME_WINSIZE_Y, wxPG_LABEL) );
	m_prop->Append( new wxBoolProperty(PNAME_FULLSCREEN, wxPG_LABEL) );

	m_prop->Append( m_obj = new luObjectProperty("Object"));

	addControlToPanel(this, m_prop, PANEL_NAME);
}

luPropsPanel::~luPropsPanel()
{
}

void luPropsPanel::OnPropertyGridChanged( wxPropertyGridEvent& event )
{
	wxPGProperty* p = event.GetProperty();

	if (!p) return;

	if (p->GetName() == PNAME_PROJECT_NAME)
	{
		luMainFrame* frame = getLuMainFrame(); GK_ASSERT(frame);
		frame->setProjectName(p->GetValueAsString());
	}
}


void luPropsPanel::setProjectName(const wxString& name)
{
	if (!m_prop) return;

	wxPGProperty* p = m_prop->GetProperty(PNAME_PROJECT_NAME);
	if (!p) return;
	p->SetValue(name);
}

void luPropsPanel::selectObject(gkGameObject* obj)
{
	if (!m_obj) return;

	luGameObjProps p(obj);
	wxVariant v; v << p;
	m_obj->SetValue(v);

}