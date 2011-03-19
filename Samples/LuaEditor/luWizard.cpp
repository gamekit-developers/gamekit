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
#include "luWizard.h"
#include "luFile.h"
#include "luMainApp.h"
#include "luMainFrame.h"
#include "Lib/liUtils.h"

#include "Resource/app_wiz.xpm"


class luNewProjNamePage : public wxWizardPageSimple
{


	wxStaticText* m_staticText4;
	wxFilePickerCtrl* m_filePicker;
	wxTextCtrl* m_textName;
	wxStaticText* m_staticText6;

public:
	wxString getProjName() { return m_textName ? m_textName->GetLabelText() : ""; }
	wxString getProjFileName() { return m_filePicker ? m_filePicker->GetPath() : ""; }


	luNewProjNamePage(wxWizard* parent) :
		wxWizardPageSimple(parent)

	{
		//-- find a project file name
		wxString cwd = wxFileName::GetCwd();
		wxString fileName;
		for (int i = 0; i < 100; i++)
		{
			fileName = wxString::Format("%s/Project%d.okproj", cwd, i);
			if (!wxFile::Exists(fileName)) break;
		}

		this->SetSizeHints( wxDefaultSize, wxDefaultSize );

		wxFlexGridSizer* fgSizer10;
		fgSizer10 = new wxFlexGridSizer( 2, 2, 0, 10 );
		fgSizer10->AddGrowableCol( 1 );
		fgSizer10->SetFlexibleDirection( wxBOTH );
		fgSizer10->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

		m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("Project Name"), wxDefaultPosition, wxDefaultSize, 0 );
		m_staticText6->Wrap( -1 );
		fgSizer10->Add( m_staticText6, 0, wxALL, 5 );

		m_textName= new wxTextCtrl( this, wxID_ANY, wxFileName(fileName).GetName(), wxDefaultPosition, wxDefaultSize, 0);
		fgSizer10->Add( m_textName , 0, wxALL|wxEXPAND, 5 );

		m_staticText4 = new wxStaticText( this, wxID_ANY, wxT("Project File Name"), wxDefaultPosition, wxDefaultSize, 0 );
		m_staticText4->Wrap( -1 );
		fgSizer10->Add( m_staticText4, 0, wxALL, 5 );

		m_filePicker = new wxFilePickerCtrl( this, wxID_ANY, fileName,
			wxT("Create Project File"), wxT("Project Files (*.okproj)|*.okproj"),
			wxDefaultPosition, wxDefaultSize, wxFLP_USE_TEXTCTRL | wxFLP_SAVE);
		fgSizer10->Add( m_filePicker, 0, wxALL|wxEXPAND, 5 );

		this->SetSizer( fgSizer10 );
		this->Layout();

		this->Centre( wxBOTH );

		SetSizerAndFit(fgSizer10);
	}


	virtual bool TransferDataFromWindow()
    {
		wxFileName fileName = m_filePicker->GetFileName();
		if (fileName.FileExists())
		{
			alertDlg("Project file is already exist.\nSelect other filename.", this);
			m_filePicker->SetFocus();
			return false;
		}

		if (!fileName.IsOk())
		{
			alertDlg("Project file name is empty or invalid.", this);
			m_filePicker->SetFocus();
			return false;
		}

		if (m_textName->GetLabelText().IsEmpty())
		{
			alertDlg("Project name is empty.", this);
			m_textName->SetFocus();
			return false;
		}

		return true;
	}

};



class luNewProjTemplatePage : public wxWizardPageSimple
{
	luProjTempList* m_projTempList;

	wxStaticText* m_staticText5;

	wxListBox* m_listTemplate;
	wxStaticBitmap* m_imgPreview;
	wxTextCtrl* m_textTitle;
	wxStaticText* m_staticText6;
	wxTextCtrl* m_textDesc;

	int m_curSel;
public:

	int getSelectedTemplate() { return m_curSel; }

	luNewProjTemplatePage(wxWizard* parent) : wxWizardPageSimple(parent)
	{
		this->SetSizeHints( wxDefaultSize, wxDefaultSize );

		wxFlexGridSizer* fgSizer10;
		fgSizer10 = new wxFlexGridSizer( 3, 2, 0, 10 );
		fgSizer10->AddGrowableCol( 1 );
		fgSizer10->SetFlexibleDirection( wxBOTH );
		fgSizer10->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

		m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("Project Templates"), wxDefaultPosition, wxDefaultSize, 0 );
		m_staticText5->Wrap( -1 );
		fgSizer10->Add( m_staticText5, 0, wxALL, 5 );

		m_textTitle = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
		fgSizer10->Add( m_textTitle, 0, wxALL, 5 );

		m_listTemplate = new wxListBox( this, ID_WIZARD_TEMPL_LIST, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
		fgSizer10->Add( m_listTemplate, 0, wxALL, 5 );

		m_imgPreview = new wxStaticBitmap( this, wxID_ANY, wxBitmap("Data/Empty.xpm", wxBITMAP_TYPE_ANY),
			wxDefaultPosition, wxDefaultSize, 0 );
		fgSizer10->Add( m_imgPreview, 0, wxALL, 5 );

		m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("Description"), wxDefaultPosition, wxDefaultSize, 0 );
		m_staticText6->Wrap( -1 );
		fgSizer10->Add( m_staticText6, 0, wxALL, 5 );

		m_textDesc = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY );
		fgSizer10->Add( m_textDesc, 0, wxALL|wxEXPAND, 5 );


		this->SetSizer( fgSizer10 );
		this->Layout();

		this->Centre( wxBOTH );

		//--

		luProjTempList *ptlist = getLuApp()->getProjTempList(); GK_ASSERT(ptlist);
		for (size_t i = 0; i < ptlist->size(); i++)
		{
			const luProjTemplate& templ = (*ptlist)[i];
			m_listTemplate->Append(templ.name);
		}

		m_projTempList = ptlist;

		m_listTemplate->Connect(  wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( luNewProjTemplatePage::OnListItemSelected ), NULL, this );

		m_listTemplate->SetSelection(0);

        wxCommandEvent evt(wxEVT_NULL, wxEVT_COMMAND_LISTBOX_SELECTED);
		OnListItemSelected(evt);


		SetSizerAndFit(fgSizer10);
	}

	void OnListItemSelected(wxCommandEvent& event)
	{
		int sel = m_listTemplate->GetSelection();
		if (sel < 0 || sel >= (int)m_projTempList->size())
		{
			m_curSel = -1;
			return;
		}

		m_curSel = sel;

		const luProjTemplate& templ = (*m_projTempList)[sel];

		m_imgPreview->SetBitmap(wxBitmap(templ.preview, wxBITMAP_TYPE_ANY));
		m_textDesc->SetLabelText(templ.desc);
		m_textTitle->SetLabelText(templ.title);

		//gkPrintf("%d", sel);
	}
};

class luNewProjFinishPage : public wxWizardPageSimple
{
public:
	luNewProjFinishPage(wxWizard* parent) : wxWizardPageSimple(parent)
	{
		new wxStaticText( this, wxID_ANY, wxT("Click finish button to create New project"), wxDefaultPosition, wxDefaultSize, 0 );
	}
};


luNewProjWizard::luNewProjWizard(wxFrame* parent, bool useSizer) :
	 wxWizard(parent, ID_NEW_PROJ_WIZARD, _T("Create New Project Wizard"),
                   wxBitmap(wiz_xpm),wxDefaultPosition,
                   wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
	SetBitmapPlacement(wxWIZARD_VALIGN_CENTRE);

    SetLayoutAdaptationMode(wxDIALOG_ADAPTATION_MODE_ENABLED);

    m_page1 = new wxWizardPageSimple(this);

	new wxStaticText(m_page1, wxID_ANY,
		_T("Create New Project for LuaEditor"),
		wxPoint(5,5)
	);

	m_page2 = new luNewProjNamePage(this);
	m_page3 = new luNewProjTemplatePage(this);
	m_page4 = new luNewProjFinishPage(this);

	wxWizardPageSimple::Chain(m_page1, m_page2);
	wxWizardPageSimple::Chain(m_page2, m_page3);
	wxWizardPageSimple::Chain(m_page3, m_page4);


	if (useSizer)
		GetPageAreaSizer()->Add(m_page1);

}


BEGIN_EVENT_TABLE(luNewProjWizard, wxWizard)
	EVT_WIZARD_CANCEL(ID_NEW_PROJ_WIZARD,   luNewProjWizard::OnWizardCancel)
    EVT_WIZARD_FINISHED(ID_NEW_PROJ_WIZARD, luNewProjWizard::OnWizardFinished)
END_EVENT_TABLE()


void luNewProjWizard::OnWizardCancel(wxWizardEvent& event)
{
	alertDlg("New create project wizard is cancelled.", this);
}

void luNewProjWizard::OnWizardFinished(wxWizardEvent& event)
{
	luMainFrame* frame = getLuMainFrame();
	if (!frame) return;
	wxString projFileName = m_page2->getProjFileName();
	if (frame->createNewProject(m_page2->getProjName(), projFileName, m_page3->getSelectedTemplate()))
	{
		alertDlg("New projct is created.");
		frame->openProjFile(projFileName);
	}
	else
	{
		alertDlg("Error - Can't create new project to \nSee error message in LogWindow." + m_page2->getProjFileName());
	}
}
