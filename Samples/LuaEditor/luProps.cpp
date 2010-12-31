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
#include "luProps.h"
#include "luFile.h"
#include "luMainFrame.h"
#include "okUtils.h"

WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(gkVector2)

WX_PG_IMPLEMENT_PROPERTY_CLASS(luVector2Property,wxPGProperty, gkVector2,const gkVector3&,TextCtrl)

luVector2Property::luVector2Property( const wxString& label, const wxString& name, const gkVector2& value )
	: wxPGProperty(label,name)
{
	SetValue( WXVARIANT(value) );
	AddPrivateChild( new wxFloatProperty(wxT("X"),wxPG_LABEL,value.x) );
	AddPrivateChild( new wxFloatProperty(wxT("Y"),wxPG_LABEL,value.y) );	
}

void luVector2Property::RefreshChildren()
{
	if ( !GetChildCount() ) return;
	const gkVector2& v = gkVector2RefFromVariant(m_value);
	Item(0)->SetValue( v.x );
	Item(1)->SetValue( v.y );
}

wxVariant  luVector2Property::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
	gkVector3 v;
	v << thisValue;
	switch ( childIndex )
	{
	case 0: v.x = childValue.GetDouble(); break;
	case 1: v.y = childValue.GetDouble(); break;
	}
	return thisValue << v;
}


WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(gkVector3)

WX_PG_IMPLEMENT_PROPERTY_CLASS(luVector3Property,wxPGProperty, gkVector3,const gkVector3&,TextCtrl)

luVector3Property::luVector3Property( const wxString& label, const wxString& name, const gkVector3& value )
	: wxPGProperty(label,name)
{
	SetValue( WXVARIANT(value) );
	AddPrivateChild( new wxFloatProperty(wxT("X"),wxPG_LABEL,value.x) );
	AddPrivateChild( new wxFloatProperty(wxT("Y"),wxPG_LABEL,value.y) );
	AddPrivateChild( new wxFloatProperty(wxT("Z"),wxPG_LABEL,value.z) );
}

void luVector3Property::RefreshChildren()
{
	if ( !GetChildCount() ) return;
	const gkVector3& v = gkVector3RefFromVariant(m_value);
	Item(0)->SetValue( v.x );
	Item(1)->SetValue( v.y );
	Item(2)->SetValue( v.z );
}

wxVariant  luVector3Property::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
	gkVector3 v;
	v << thisValue;
	switch ( childIndex )
	{
	case 0: v.x = childValue.GetDouble(); break;
	case 1: v.y = childValue.GetDouble(); break;
	case 2: v.z = childValue.GetDouble(); break;
	}
	return thisValue << v;
}


WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(gkVector4)

WX_PG_IMPLEMENT_PROPERTY_CLASS(luVector4Property,wxPGProperty, gkVector4,const gkVector4&,TextCtrl)

luVector4Property::luVector4Property( const wxString& label, const wxString& name, const gkVector4& value )
	: wxPGProperty(label,name)
{
	SetValue( WXVARIANT(value) );
	AddPrivateChild( new wxFloatProperty(wxT("X"),wxPG_LABEL,value.x) );
	AddPrivateChild( new wxFloatProperty(wxT("Y"),wxPG_LABEL,value.y) );
	AddPrivateChild( new wxFloatProperty(wxT("Z"),wxPG_LABEL,value.z) );
	AddPrivateChild( new wxFloatProperty(wxT("W"),wxPG_LABEL,value.w) );
}

void luVector4Property::RefreshChildren()
{
	if ( !GetChildCount() ) return;
	const gkVector4& v = gkVector4RefFromVariant(m_value);
	Item(0)->SetValue( v.x );
	Item(1)->SetValue( v.y );
	Item(2)->SetValue( v.z );
	Item(2)->SetValue( v.w );
}

wxVariant  luVector4Property::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
	gkVector4 v;
	v << thisValue;
	switch ( childIndex )
	{
	case 0: v.x = childValue.GetDouble(); break;
	case 1: v.y = childValue.GetDouble(); break;
	case 2: v.z = childValue.GetDouble(); break;
	case 3: v.w = childValue.GetDouble(); break;
	}
	return thisValue << v;
}

//--

luGameObjProps::luGameObjProps(gkGameObject* obj) 
	:	pos(gkVector3::ZERO),
		scale(gkVector3::ZERO),
		front(gkVector3::ZERO),
		up(gkVector3::ZERO),
		dim(gkVector3::ZERO),
		flags(0),
		state(0),
		hasParent(false),
		numChilds(0),
		isGroupInstance(false),
		isInGroup(false),
		isStaticGeometry(false),
		isImmovable(false),
		hasCharactor(false),
		hasLogicBricks(false)
{
	if (!obj) return;

	name = obj->getName();
	type = getLuObjectTypeName(obj->getType());
	pos = obj->getPosition();
	scale = obj->getScale();
	front = obj->getOrientation() * gkVector3::UNIT_Y;
	up = obj->getOrientation() * gkVector3::UNIT_Z;
	dim = obj->getAabb().getSize() * scale;
	flags = obj->getFlags();
	state = obj->getState();
	hasParent = obj->hasParent();
	numChilds = obj->getChildren().size();
	isGroupInstance = obj->isGroupInstance();
	isInGroup = obj->isInGroup();
	isStaticGeometry = obj->isStaticGeometry();
	isImmovable = obj->isImmovable();
	hasCharactor = obj->getAttachedCharacter() != NULL;
	hasLogicBricks = obj->getLogicBricks() != NULL;

	props = obj->getProperties();

	utArray<utHashedString> names;
	obj->getAnimationNames(names);

	for (UTsize i = 0; i < names.size(); i++)
	{
		animations.Add(GK2WX(names[i].str()));
	//	gkPrintf("%s", names[i].str().c_str());
	}
}

//--
WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(luPhysicsProps)

WX_PG_IMPLEMENT_PROPERTY_CLASS(luPhysicsProperty,wxPGProperty, luPhysicsProps,const luPhysicsProps&,TextCtrl)

luPhysicsProperty::luPhysicsProperty(const wxString& label, const wxString& name) 
	:	wxPGProperty(label, name)
{
	AddPrivateChild(new wxBoolProperty("IsContactListener"));
	AddPrivateChild(new wxBoolProperty("IsDosser"));
	AddPrivateChild(new wxBoolProperty("IsRigidOrDynamic"));
	AddPrivateChild(new wxBoolProperty("IsRigidOrStatic"));
	AddPrivateChild(new wxBoolProperty("IsPhysicsObject"));
	AddPrivateChild(new wxBoolProperty("IsStatic"));
	AddPrivateChild(new wxBoolProperty("IsDynamic"));
	AddPrivateChild(new wxBoolProperty("IsRigid"));
	AddPrivateChild(new wxBoolProperty("IsSoft"));
	AddPrivateChild(new wxBoolProperty("IsMeshShape"));

	AddPrivateChild(new wxIntProperty("Type"));
	AddPrivateChild(new wxIntProperty("Mode"));
	AddPrivateChild(new wxIntProperty("Shape"));

	AddPrivateChild(new wxFloatProperty("Margin"));
	AddPrivateChild(new wxFloatProperty("Cpt"));
	AddPrivateChild(new wxFloatProperty("Mass"));
	AddPrivateChild(new wxFloatProperty("Radius"));
	AddPrivateChild(new wxFloatProperty("LinearDamp"));
	AddPrivateChild(new wxFloatProperty("AngularDamp"));
	AddPrivateChild(new wxFloatProperty("FormFactor"));
	AddPrivateChild(new wxFloatProperty("MinVel"));
	AddPrivateChild(new wxFloatProperty("MaxVel"));
	AddPrivateChild(new wxFloatProperty("Restitution"));
	AddPrivateChild(new wxFloatProperty("Friction"));
}

void luPhysicsProperty::RefreshChildren()
{
	if ( !GetChildCount() ) return;

	const luPhysicsProps& obj = luPhysicsPropsRefFromVariant(m_value);

	Item(PROP_IS_CONTACTLISTENER)->SetValue( WXVARIANT(obj.props.isContactListener()));
	Item(PROP_IS_DOSSER)->SetValue( WXVARIANT(obj.props.isDosser()));
	Item(PROP_IS_RIGIDORDYNAMIC)->SetValue( WXVARIANT(obj.props.isRigidOrDynamic()));
	Item(PROP_IS_RIGIDORSTATIC)->SetValue( WXVARIANT(obj.props.isRigidOrStatic()));
	Item(PROP_IS_PHYSICSOBJECT)->SetValue( WXVARIANT(obj.props.isPhysicsObject()));
	Item(PROP_IS_STATIC)->SetValue( WXVARIANT(obj.props.isStatic()));
	Item(PROP_IS_DYNAMIC)->SetValue( WXVARIANT(obj.props.isDynamic()));
	Item(PROP_IS_RIGID)->SetValue( WXVARIANT(obj.props.isRigid()));
	Item(PROP_IS_SOFT)->SetValue( WXVARIANT(obj.props.isSoft()));
	Item(PROP_IS_MESHSHAPE)->SetValue( WXVARIANT(obj.props.isMeshShape()));	

	Item(PROP_TYPE)->SetValue( WXVARIANT(obj.props.m_type));
	Item(PROP_MODE)->SetValue( WXVARIANT(obj.props.m_mode));
	Item(PROP_SHAPE)->SetValue( WXVARIANT(obj.props.m_shape));

	Item(PROP_MARGIN)->SetValue( double(obj.props.m_margin));
	Item(PROP_CPT)->SetValue( double(obj.props.m_cpt));
	Item(PROP_MASS)->SetValue( double(obj.props.m_mass));
	Item(PROP_RADIUS)->SetValue( double(obj.props.m_radius));
	Item(PROP_LINEARDAMP)->SetValue( double(obj.props.m_linearDamp));
	Item(PROP_ANGULARDAMP)->SetValue( double(obj.props.m_angularDamp));
	Item(PROP_FORMFACTOR)->SetValue( double(obj.props.m_formFactor));
	Item(PROP_MINVEL)->SetValue( double(obj.props.m_minVel));
	Item(PROP_MAXVEL)->SetValue( double(obj.props.m_maxVel));
	Item(PROP_RESTITUTION)->SetValue( double(obj.props.m_restitution));
	Item(PROP_FRICTION)->SetValue( double(obj.props.m_friction));
}

//--


WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(luGameObjProps)

WX_PG_IMPLEMENT_PROPERTY_CLASS(luObjectProperty,wxPGProperty, luGameObjProps,const luGameObjProps&,TextCtrl)

luObjectProperty::luObjectProperty(const wxString& label, const wxString& name) 
	:	wxPGProperty(label, name)
{
	//keep order match to OBJPROP
	AddPrivateChild(new wxStringProperty("Name", wxPG_LABEL));
	AddPrivateChild(new wxStringProperty("Type", wxPG_LABEL));
	AddPrivateChild(new luVector3Property("Position"));
	AddPrivateChild(new luVector3Property("Scale"));
	AddPrivateChild(new luVector3Property("Front"));
	AddPrivateChild(new luVector3Property("Up"));
	AddPrivateChild(new luVector3Property("Dimentions"));
	AddPrivateChild(new wxIntProperty("Flags"));
	AddPrivateChild(new wxIntProperty("State"));
	AddPrivateChild(new wxBoolProperty("HasParent"));
	AddPrivateChild(new wxIntProperty("NumChilds"));
	AddPrivateChild(new wxBoolProperty("IsGroupInstance"));
	AddPrivateChild(new wxBoolProperty("IsInGroup"));
	AddPrivateChild(new wxBoolProperty("IsStaticGeometry"));
	AddPrivateChild(new wxBoolProperty("IsImmovable"));
	 
	
	AddPrivateChild(new wxBoolProperty("IsActor"));
	AddPrivateChild(new wxBoolProperty("IsInvisible"));
	AddPrivateChild(new wxBoolProperty("IsOccluder"));
	AddPrivateChild(new wxBoolProperty("IsGhost"));

	AddPrivateChild(new wxBoolProperty("HasCharactor"));
	AddPrivateChild(new wxBoolProperty("HasLogicBricks"));

	AddPrivateChild(new wxArrayStringProperty("Animations"));
}


void luObjectProperty::RefreshChildren()
{
	if ( !GetChildCount() ) return;

	const luGameObjProps& obj = luGameObjPropsRefFromVariant(m_value);

	Item(PROP_NAME)->SetValue( obj.name );
	Item(PROP_TYPE)->SetValue( obj.type );
	Item(PROP_POS)->SetValue( WXVARIANT(obj.pos) );
	Item(PROP_SCALE)->SetValue( WXVARIANT(obj.scale) );
	Item(PROP_FRONT)->SetValue( WXVARIANT(obj.front) );
	Item(PROP_UP)->SetValue( WXVARIANT(obj.up) );
	Item(PROP_DIM)->SetValue( WXVARIANT(obj.dim) );
	Item(PROP_FLAGS)->SetValue( WXVARIANT(obj.flags) );
	Item(PROP_STATE)->SetValue( WXVARIANT(obj.state) );
	Item(PROP_HAS_PARENT)->SetValue( WXVARIANT(obj.hasParent) );
	Item(PROP_NUM_CHILDS)->SetValue( WXVARIANT(obj.numChilds) );
	Item(PROP_IS_GROUP_INSTANCE)->SetValue( WXVARIANT(obj.isGroupInstance) );
	Item(PROP_IS_IN_GROPUP)->SetValue( WXVARIANT(obj.isInGroup) );
	Item(PROP_IS_STATIC_GEOMETRY)->SetValue( WXVARIANT(obj.isStaticGeometry) );
	Item(PROP_IS_IMMOVABLE)->SetValue( WXVARIANT(obj.isImmovable) );

	Item(PROP_IS_ACTOR)->SetValue( WXVARIANT(obj.props.isActor()));
	Item(PROP_IS_INVISIBLE)->SetValue( WXVARIANT(obj.props.isInvisible()));
	Item(PROP_IS_OCCLUDER)->SetValue( WXVARIANT(obj.props.isOccluder()));
	Item(PROP_IS_GHOST)->SetValue( WXVARIANT(obj.props.isGhost()));

	Item(PROP_HAS_CHARACTOR)->SetValue(obj.hasCharactor);
	Item(PROP_HAS_LOGICBRICKS)->SetValue(obj.hasLogicBricks);

	Item(PROP_ANIMATIONS)->SetValue( WXVARIANT(obj.animations));
}

wxVariant  luObjectProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
	luGameObjProps obj;
	obj << thisValue;
	switch ( childIndex )
	{
	case PROP_NAME:	{ obj.name = childValue.GetString(); break; }
	//case PROP_TYPE:	{ obj.type = childValue.GetString(); break; }
	case PROP_POS:	{ obj.pos = gkVector3RefFromVariant(childValue); break; }
	case PROP_SCALE:	{ obj.scale = gkVector3RefFromVariant(childValue); break; }
	case PROP_FRONT:	{ obj.front = gkVector3RefFromVariant(childValue); break; }
	case PROP_UP:	{ obj.up = gkVector3RefFromVariant(childValue); break; }
	case PROP_DIM:	{ obj.dim = gkVector3RefFromVariant(childValue); break; }
	case PROP_FLAGS:	{ obj.flags = childValue.GetInteger(); break; }
	case PROP_STATE:	{ obj.state = childValue.GetInteger(); break; }
	}
	return thisValue << obj;
}

//--

WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(luEntityObjProps)

WX_PG_IMPLEMENT_PROPERTY_CLASS(luEntityObjProperty, wxPGProperty, luEntityObjProps,const luEntityObjProps&,TextCtrl)

luEntityObjProperty::luEntityObjProperty(const wxString& label, const wxString& name)
	:	wxPGProperty(label, name)
{
	AddPrivateChild(new wxStringProperty("Mesh"));
	AddPrivateChild(new wxBoolProperty("Casts"));
	AddPrivateChild(new wxStringProperty("Source"));
	AddPrivateChild(new wxStringProperty("StartPose"));
}

void luEntityObjProperty::RefreshChildren()
{
	if ( !GetChildCount() ) return;

	const luEntityObjProps& obj = luEntityObjPropsRefFromVariant(m_value);

	Item(PROP_MESH)->SetValue( GK2WX(obj.props.m_mesh->getName()) );
	Item(PROP_CASTS)->SetValue( obj.props.m_casts );
	Item(PROP_SOURCE)->SetValue( GK2WX(obj.props.m_source));
	Item(PROP_STARTPOSE)->SetValue( GK2WX(obj.props.m_startPose));
}

//--


//--

WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(luCameraObjProps)

WX_PG_IMPLEMENT_PROPERTY_CLASS(luCameraObjProperty, wxPGProperty, luCameraObjProps,const luCameraObjProps&,TextCtrl)

luCameraObjProperty::luCameraObjProperty(const wxString& label, const wxString& name)
	:	wxPGProperty(label, name)
{
	AddPrivateChild(new wxFloatProperty("ClipStart"));
	AddPrivateChild(new wxFloatProperty("ClipEnd"));
	AddPrivateChild(new wxFloatProperty("Fov"));
	AddPrivateChild(new wxFloatProperty("OrthoScale"));
	AddPrivateChild(new wxBoolProperty("Start"));
	AddPrivateChild(new wxIntProperty("Type"));	
}


void luCameraObjProperty::RefreshChildren()
{
	if ( !GetChildCount() ) return;

	const luCameraObjProps& obj = luCameraObjPropsRefFromVariant(m_value);

	Item(PROP_CLIPSTART)->SetValue(double(obj.props.m_clipstart));
	Item(PROP_CLIPEND)->SetValue(double(obj.props.m_clipend));
	Item(PROP_FOV)->SetValue(double(obj.props.m_fov));
	Item(PROP_ORTHOSCALE)->SetValue(double(obj.props.m_orthoscale));
	Item(PROP_START)->SetValue(obj.props.m_start);
	Item(PROP_TYPE)->SetValue(obj.props.m_type);
}

//--

WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(luLightObjProps)

WX_PG_IMPLEMENT_PROPERTY_CLASS(luLightObjProperty, wxPGProperty, luLightObjProps,const luLightObjProps&,TextCtrl)

luLightObjProperty::luLightObjProperty(const wxString& label, const wxString& name)
	:	wxPGProperty(label, name)
{
	static const wxChar* lightNames[] = {wxT("Point"), wxT("Spot"), wxT("Directional") };
	static long lightValues[] = { 0, 1, 2 };

	AddPrivateChild(new wxColourProperty("Diffuse"));
	AddPrivateChild(new wxColourProperty("Specular"));
	AddPrivateChild(new wxEnumProperty("Type", wxPG_LABEL, lightNames, lightValues));
	AddPrivateChild(new luVector2Property("Spot"));
	AddPrivateChild(new luVector3Property("Direction"));
	AddPrivateChild(new wxFloatProperty("Power"));
	AddPrivateChild(new wxFloatProperty("Falloff"));
	AddPrivateChild(new wxFloatProperty("Range"));
	AddPrivateChild(new wxFloatProperty("Constant"));
	AddPrivateChild(new wxFloatProperty("Linear"));
	AddPrivateChild(new wxFloatProperty("Quadratic"));
	AddPrivateChild(new wxBoolProperty("Casts"));
	AddPrivateChild(new wxBoolProperty("Extra"));
	AddPrivateChild(new wxFloatProperty("Param0"));
	AddPrivateChild(new wxFloatProperty("Param1"));
	AddPrivateChild(new wxFloatProperty("Param2"));
	AddPrivateChild(new wxFloatProperty("Param3"));
}




void luLightObjProperty::RefreshChildren()
{
	if ( !GetChildCount() ) return;

	const luLightObjProps& obj = luLightObjPropsRefFromVariant(m_value);


	Item(PROP_DIFFUSE)->SetValue( WXVARIANT(GK2WX(obj.props.m_diffuse)) );
	Item(PROP_SPECULAR)->SetValue( WXVARIANT(GK2WX(obj.props.m_specular)) );
	Item(PROP_TYPE)->SetValue( obj.props.m_type );
	Item(PROP_SPOT)->SetValue( WXVARIANT(obj.props.m_spot) );
	Item(PROP_DIRECTION)->SetValue( WXVARIANT(obj.props.m_direction) );
	Item(PROP_POWER)->SetValue( double(obj.props.m_power) );

	Item(PROP_FALLOFF)->SetValue( double(obj.props.m_falloff) );
	Item(PROP_RANGE)->SetValue( double(obj.props.m_range) );
	Item(PROP_CONSTANT)->SetValue( double(obj.props.m_constant) );
	Item(PROP_LINEAR)->SetValue( double(obj.props.m_linear) );
	Item(PROP_QUADRATIC)->SetValue( double(obj.props.m_quadratic) );
	Item(PROP_CASTS)->SetValue( WXVARIANT(obj.props.m_casts) );
	Item(PROP_EXTRA)->SetValue( WXVARIANT(obj.props.m_extra) );
	Item(PROP_PARAM0)->SetValue( double(obj.props.m_param[0]) );
	Item(PROP_PARAM1)->SetValue( double(obj.props.m_param[1]) );
	Item(PROP_PARAM2)->SetValue( double(obj.props.m_param[2]) );
	Item(PROP_PARAM3)->SetValue( double(obj.props.m_param[3]) );

}

//--

luProjectProps::luProjectProps(luProjectFile *inProject)
	:	winSizeX(0),
		winSizeY(0),
		project(inProject),
		fullScreen(false)
{
	if (project)
	{
		projName = project->getProjName();
		projDir = project->getProjDir();
	}
}

WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(luProjectProps)

WX_PG_IMPLEMENT_PROPERTY_CLASS(luProjectProperty, wxPGProperty, luProjectProps,const luProjectProps&,TextCtrl)

luProjectProperty::luProjectProperty(const wxString& label, const wxString& name) 
	:	wxPGProperty(label, name)
{
	AddPrivateChild(new wxStringProperty("ProjectName"));
	AddPrivateChild(new wxStringProperty("Directroy"));

	AddPrivateChild(new wxIntProperty("WinSizeX"));
	AddPrivateChild(new wxIntProperty("WinSizeY"));

	AddPrivateChild(new wxBoolProperty("FullScreen"));
}

void luProjectProperty::RefreshChildren()
{
	if ( !GetChildCount() ) return;

	const luProjectProps& obj = luProjectPropsRefFromVariant(m_value);

	Item(PROP_PROJECT_NAME)->SetValue(obj.projName);
	Item(PROP_PROJECT_DIR)->SetValue(obj.projDir);
	Item(PROP_WINSIZE_X)->SetValue(obj.winSizeX);
	Item(PROP_WINSIZE_Y)->SetValue(obj.winSizeY);
	Item(PROP_FULLSCREEN)->SetValue(obj.fullScreen);
}


wxVariant luProjectProperty::ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const
{
	luProjectProps obj;
	obj << thisValue;
	switch ( childIndex )
	{
	case PROP_PROJECT_NAME:
		{ 
			obj.projName = childValue.GetString(); 
			luMainFrame* frame = getLuMainFrame(); GK_ASSERT(frame);			
			frame->setProjectName(obj.projName);
			break; 
		}
	}
	return thisValue << obj;
}