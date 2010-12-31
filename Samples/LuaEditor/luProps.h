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

#ifndef _luProps_h_
#define _luProps_h_

WX_PG_DECLARE_VARIANT_DATA(gkVector2)

class luVector2Property : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(luVector2Property)
public:

	luVector2Property( const wxString& label = wxPG_LABEL,
		const wxString& name = wxPG_LABEL,
		const gkVector2& value = gkVector2::ZERO );

	wxVariant ChildChanged( wxVariant& thisValue,
		int childIndex, wxVariant& childValue ) const;
	virtual void RefreshChildren();
};


WX_PG_DECLARE_VARIANT_DATA(gkVector3)

class luVector3Property : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(luVector3Property)
public:

	luVector3Property( const wxString& label = wxPG_LABEL,
		const wxString& name = wxPG_LABEL,
		const gkVector3& value = gkVector3::ZERO );

	wxVariant ChildChanged( wxVariant& thisValue,
		int childIndex, wxVariant& childValue ) const;
	virtual void RefreshChildren();
};


WX_PG_DECLARE_VARIANT_DATA(gkVector4)

class luVector4Property : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(luVector3Property)
public:

	luVector4Property( const wxString& label = wxPG_LABEL,
		const wxString& name = wxPG_LABEL,
		const gkVector4& value = gkVector4::ZERO );

	wxVariant ChildChanged( wxVariant& thisValue,
		int childIndex, wxVariant& childValue ) const;
	virtual void RefreshChildren();
};

struct luPhysicsProps
{
	gkPhysicsProperties props;

	luPhysicsProps(gkGameObject* obj = NULL)
	{
		if (obj)
			props = obj->getProperties().m_physics;
	}
};

WX_PG_DECLARE_VARIANT_DATA(luPhysicsProps)

struct luGameObjProps
{
	gkString name;
	gkString type;
	gkVector3 pos;
	gkVector3 scale;
	gkVector3 front;
	gkVector3 up;
	gkVector3 dim;
	int flags;
	int state;
	bool hasParent;
	int numChilds;
	bool isGroupInstance;
	bool isInGroup;
	bool isStaticGeometry;
	bool isImmovable;
	bool hasCharactor;
	bool hasLogicBricks;
	wxArrayString animations;

	gkGameObjectProperties props;

	luGameObjProps(gkGameObject* obj = NULL);
};

WX_PG_DECLARE_VARIANT_DATA(luGameObjProps)

class luPhysicsProperty : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(luPhysicsProperty)

public:	
	enum PROP 
	{
		PROP_IS_CONTACTLISTENER,
		PROP_IS_DOSSER,
		PROP_IS_RIGIDORDYNAMIC,
		PROP_IS_RIGIDORSTATIC,
		PROP_IS_PHYSICSOBJECT,
		PROP_IS_STATIC,
		PROP_IS_DYNAMIC,
		PROP_IS_RIGID,
		PROP_IS_SOFT,
		PROP_IS_MESHSHAPE,

		PROP_TYPE,
		PROP_MODE,
		PROP_SHAPE, 
		PROP_MARGIN, 
		PROP_CPT,
		PROP_MASS, 
		PROP_RADIUS,
		PROP_LINEARDAMP,
		PROP_ANGULARDAMP, 
		PROP_FORMFACTOR,
		PROP_MINVEL,
		PROP_MAXVEL,
		PROP_RESTITUTION,
		PROP_FRICTION 

	};

	luPhysicsProperty(const wxString& label = wxPG_LABEL, const wxString& name = wxPG_LABEL);
	
	virtual void RefreshChildren();
};

class luObjectProperty : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(luObjectProperty)

public:	
	enum PROP 
	{
		PROP_NAME,
		PROP_TYPE,
		PROP_POS,
		PROP_SCALE,
		PROP_FRONT,
		PROP_UP,
		PROP_DIM,
		PROP_FLAGS,
		PROP_STATE,
		PROP_HAS_PARENT,
		PROP_NUM_CHILDS,
		PROP_IS_GROUP_INSTANCE,
		PROP_IS_IN_GROPUP,
		PROP_IS_STATIC_GEOMETRY,
		PROP_IS_IMMOVABLE,

		PROP_IS_ACTOR,
		PROP_IS_INVISIBLE,
		PROP_IS_OCCLUDER,
		PROP_IS_GHOST,

		PROP_HAS_CHARACTOR,
		PROP_HAS_LOGICBRICKS,

		PROP_ANIMATIONS
	};

	luObjectProperty(const wxString& label = wxPG_LABEL, const wxString& name = wxPG_LABEL);

	wxVariant ChildChanged( wxVariant& thisValue, int childIndex, wxVariant& childValue ) const;
	virtual void RefreshChildren();
};


//--


struct luEntityObjProps
{
	gkEntityProperties props;

	luEntityObjProps(gkGameObject* obj = NULL)
	{
		if (obj && obj->getEntity())
			props = obj->getEntity()->getEntityProperties();
	}
};

WX_PG_DECLARE_VARIANT_DATA(luEntityObjProps)

class luEntityObjProperty : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(luEntityObjProperty)

public:	
	enum PROP 
	{
		PROP_MESH,
		PROP_CASTS,
		PROP_SOURCE,
		PROP_STARTPOSE
	};

	luEntityObjProperty(const wxString& label = wxPG_LABEL, const wxString& name = wxPG_LABEL);

	virtual void RefreshChildren();
};


//--


struct luCameraObjProps
{
	gkCameraProperties props;

	luCameraObjProps(gkGameObject* obj = NULL)
	{
		if (obj && obj->getCamera())
			props = obj->getCamera()->getCameraProperties();
	}
};

WX_PG_DECLARE_VARIANT_DATA(luCameraObjProps)

class luCameraObjProperty : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(luCameraObjProperty)

public:	
	enum PROP 
	{
		PROP_CLIPSTART,
		PROP_CLIPEND,
		PROP_FOV, 
		PROP_ORTHOSCALE, 
		PROP_START,
		PROP_TYPE
	};

	luCameraObjProperty(const wxString& label = wxPG_LABEL, const wxString& name = wxPG_LABEL);

	virtual void RefreshChildren();
};


//--

struct luLightObjProps
{
	gkLightProperties props;

	luLightObjProps(gkGameObject* obj = NULL)
	{
		if (obj && obj->getLight())
			props = obj->getLight()->getLightProperties();
	}
};

WX_PG_DECLARE_VARIANT_DATA(luLightObjProps)

class luLightObjProperty : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(luLightObjProperty)

public:	
	enum PROP 
	{
		PROP_DIFFUSE,
		PROP_SPECULAR,
		PROP_TYPE, 
		PROP_SPOT, 
		PROP_DIRECTION,
		PROP_POWER,
		PROP_FALLOFF,
		PROP_RANGE,
		PROP_CONSTANT,
		PROP_LINEAR,
		PROP_QUADRATIC,
		PROP_CASTS,
		PROP_EXTRA,
		PROP_PARAM0,
		PROP_PARAM1,
		PROP_PARAM2,
		PROP_PARAM3
	};

	luLightObjProperty(const wxString& label = wxPG_LABEL, const wxString& name = wxPG_LABEL);

	virtual void RefreshChildren();
};

//--

class luProjectFile;
struct luProjectProps 
{
	gkString projName;
	gkString projDir;
	int winSizeX;
	int winSizeY;
	bool fullScreen;

	luProjectFile* project;

	luProjectProps(luProjectFile *inProject = NULL);
};

WX_PG_DECLARE_VARIANT_DATA(luProjectProps)

class luProjectProperty : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(luProjectProperty)

public:
	enum PROP 
	{
		PROP_PROJECT_NAME,
		PROP_PROJECT_DIR,
		PROP_WINSIZE_X, 
		PROP_WINSIZE_Y, 
		PROP_FULLSCREEN
	};

	luProjectProperty( const wxString& label = wxPG_LABEL,
		const wxString& name = wxPG_LABEL);

	wxVariant ChildChanged( wxVariant& thisValue,
		int childIndex, wxVariant& childValue ) const;
	virtual void RefreshChildren();
};

#endif //_luProps_h_