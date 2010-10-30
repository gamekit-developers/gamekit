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

luPropsPanel::luPropsPanel(wxWindow* parent) :
	wxPanel(parent, ID_PROPS_PANEL, wxDefaultPosition, wxDefaultSize),
	m_prop(NULL)
{
	m_prop = new wxPropertyGrid(this,ID_PROPS_PROJ,wxDefaultPosition,wxSize(400,400),
                wxPG_SPLITTER_AUTO_CENTER |
                wxPG_BOLD_MODIFIED );
    

    m_prop->Append( new wxStringProperty(PNAME_PROJECT_NAME, wxPG_LABEL) );
    m_prop->Append( new wxIntProperty(PNAME_WINSIZE_X, wxPG_LABEL) );
	m_prop->Append( new wxIntProperty(PNAME_WINSIZE_Y, wxPG_LABEL) );
    m_prop->Append( new wxBoolProperty(PNAME_FULLSCREEN, wxPG_LABEL) );

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