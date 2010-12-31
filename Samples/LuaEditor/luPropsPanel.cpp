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
#include "luProps.h"
#include "luFile.h"

#define PANEL_NAME			"Properties"

BEGIN_EVENT_TABLE(luPropsPanel, wxPanel)
	EVT_PG_CHANGED( -1, luPropsPanel::OnPropertyGridChanged )
END_EVENT_TABLE()


luPropsPanel::luPropsPanel(wxWindow* parent) :
	wxPanel(parent, ID_PROPS_PANEL, wxDefaultPosition, wxDefaultSize),
	m_prop(NULL),
	m_obj(NULL),
	m_light(NULL),
	m_physics(NULL),
	m_entity(NULL),
	m_proj(NULL),
	m_camera(NULL)
{
	m_prop = new wxPropertyGrid(this,ID_PROPS_PROJ,wxDefaultPosition,wxSize(400,400),
		wxPG_SPLITTER_AUTO_CENTER |
		wxPG_BOLD_MODIFIED );
	
	m_prop->Append( m_proj = new luProjectProperty(wxT("Project"), wxPG_LABEL) );

	m_prop->Append( m_obj = new luObjectProperty("Object"));
	m_prop->Append( m_physics = new luPhysicsProperty("Physics"));
	m_prop->Append( m_entity = new luEntityObjProperty("Entity"));
	m_prop->Append( m_light = new luLightObjProperty("Light"));
	m_prop->Append( m_camera = new luCameraObjProperty("Camera"));

	addControlToPanel(this, m_prop, PANEL_NAME);

	m_obj->Hide(true);
	m_physics->Hide(true);
	m_entity->Hide(true);
	m_light->Hide(true);
	m_camera->Hide(true);
}

luPropsPanel::~luPropsPanel()
{
}

void luPropsPanel::OnPropertyGridChanged( wxPropertyGridEvent& event )
{
#if 0
	wxPGProperty* p = event.GetProperty();

	if (!p) return;

	if (p->GetName() == PNAME_PROJECT_NAME)
	{
		luMainFrame* frame = getLuMainFrame(); GK_ASSERT(frame);
		frame->setProjectName(p->GetValueAsString());
	}
#endif
}


void luPropsPanel::updateProject(luProjectFile *project)
{
	if (!m_prop) return;

	m_proj->Hide(!project);

	if (!project)
		return;
	
	wxVariant v;
	v << luProjectProps(project);
	m_proj->SetValue(v);
}


void luPropsPanel::selectObject(gkGameObject* obj)
{
	if (!obj)
	{
		m_obj->Hide(true);
		m_physics->Hide(true);
		m_entity->Hide(true);
		m_light->Hide(true);
		m_camera->Hide(true);
		return;
	}

	m_obj->Hide(false);
	m_physics->Hide(false);
	m_entity->Hide(obj->getEntity() == NULL);
	m_light->Hide(obj->getLight() == NULL);
	m_camera->Hide(obj->getCamera() == NULL);
	

	wxVariant v; 
	v << luGameObjProps(obj);
	m_obj->SetValue(v);

	v << luPhysicsProps(obj);
	m_physics->SetValue(v);

	if (obj->getEntity())
	{
		v << luEntityObjProps(obj);
		m_entity->SetValue(v);
	}

	if (obj->getLight())
	{
		v << luLightObjProps(obj);
		m_light->SetValue(v);
	}

	if (obj->getCamera())
	{
		v << luCameraObjProps(obj);
		m_camera->SetValue(v);
	}
}