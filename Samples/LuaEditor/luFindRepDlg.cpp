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
#include "luDefs.h"
#include "luFindRepDlg.h"
#include "luEdit.h"
#include "luUtils.h"

luFindDlg::luFindDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) :
	wxDialog( parent, id, title, pos, size, style ),
	m_firstSearch(true)
{
	
}

luFindDlg::~luFindDlg()
{
}

BEGIN_EVENT_TABLE(luFindDlg, wxDialog)
	//EVT_KEY_DOWN(luFindDlg::OnKeyDown)
	EVT_BUTTON(ID_FIND_BUTTON, luFindDlg::OnFindClick)
END_EVENT_TABLE()

void luFindDlg::OnKeyDown(wxKeyEvent& event)
{
	int key = event.GetKeyCode();
	if (key == WXK_ESCAPE)
		EndModal(0);
}

void luFindDlg::OnFindClick( wxCommandEvent& event )
{
	wxString str = m_searchCombo->GetValue();
	if (str.IsEmpty()) return;

	luEdit* edit = dynamic_cast<luEdit*>(GetParent());
	if (!edit) return;

	int flags = 0;
	if (m_matchCase->IsChecked()) flags |= SCFIND_MATCHCASE;
	if (m_matchWholeWords->IsChecked()) flags |= SCFIND_WHOLEWORD;
	if (m_regularExp->IsChecked()) flags |= SCFIND_REGEXP;


	int start = 0;
	if (m_firstSearch)
		m_firstSearch = false;
	else
		start = edit->GetSelectionStart() + 1;

	int pos = edit->FindText(start, edit->GetLength(), str, flags);
	if (pos < 0)
	{
		alertDlg(wxString::Format("Can't find search for '%s'.", str));
		return;
	}

	edit->SetSelection(pos, pos + str.Length());
	edit->EnsureCaretVisible();

}

void luFindDlg::setupControls()
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxFlexGridSizer* findSizer;
	findSizer = new wxFlexGridSizer( 3, 1, 0, 0 );
	findSizer->AddGrowableCol( 0 );
	findSizer->AddGrowableRow( 1 );
	findSizer->SetFlexibleDirection( wxBOTH );
	findSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* topSizer;
	topSizer = new wxFlexGridSizer( 1, 2, 0, 10 );
	topSizer->AddGrowableCol( 1 );
	topSizer->SetFlexibleDirection( wxBOTH );
	topSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Search for:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	topSizer->Add( m_staticText1, 0, wxALL, 5 );

	m_searchCombo = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0, NULL, 0 );
	topSizer->Add( m_searchCombo, 0, wxEXPAND|wxRIGHT, 5 );

	findSizer->Add( topSizer, 1, wxEXPAND|wxTOP, 5 );

	wxFlexGridSizer* midSizer;
	midSizer = new wxFlexGridSizer( 2, 2, 0, 10 );
	midSizer->AddGrowableCol( 1 );
	midSizer->AddGrowableRow( 1 );
	midSizer->SetFlexibleDirection( wxBOTH );
	midSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxStaticBoxSizer* opt1Sizer;
	opt1Sizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("options") ), wxVERTICAL );

	m_matchCase = new wxCheckBox( this, wxID_ANY, wxT("Match Case"), wxDefaultPosition, wxDefaultSize, 0 );
	opt1Sizer->Add( m_matchCase, 0, wxALL, 5 );

	m_matchWholeWords = new wxCheckBox( this, wxID_ANY, wxT("Match whole words only"), wxDefaultPosition, wxDefaultSize, 0 );
	opt1Sizer->Add( m_matchWholeWords, 0, wxALL, 5 );

	m_regularExp = new wxCheckBox( this, wxID_ANY, wxT("Regular expressions"), wxDefaultPosition, wxDefaultSize, 0 );
	opt1Sizer->Add( m_regularExp, 0, wxALL, 5 );

	midSizer->Add( opt1Sizer, 1, wxEXPAND, 5 );

	wxStaticBoxSizer* opt2Sizer;
	opt2Sizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("direction") ), wxVERTICAL );

	m_dirFoward = new wxRadioButton( this, wxID_ANY, wxT("Foward"), wxDefaultPosition, wxDefaultSize, 0 );
	opt2Sizer->Add( m_dirFoward, 0, wxALL, 5 );

	m_dirBackward = new wxRadioButton( this, wxID_ANY, wxT("Backward"), wxDefaultPosition, wxDefaultSize, 0 );
	opt2Sizer->Add( m_dirBackward, 0, wxALL, 5 );

	midSizer->Add( opt2Sizer, 1, wxEXPAND, 5 );

	findSizer->Add( midSizer, 1, wxEXPAND, 5 );

	wxFlexGridSizer* btmSizer;
	btmSizer = new wxFlexGridSizer( 1, 4, 0, 0 );
	btmSizer->AddGrowableCol( 0 );
	btmSizer->AddGrowableRow( 0 );
	btmSizer->SetFlexibleDirection( wxBOTH );
	btmSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );


	btmSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	m_findBtn = new wxButton( this, ID_FIND_BUTTON, wxT("Find"), wxDefaultPosition, wxDefaultSize, 0 );
	m_findBtn->SetDefault();
	btmSizer->Add( m_findBtn, 0, wxALL, 5 );

	m_closeBtn = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	btmSizer->Add( m_closeBtn, 0, wxALL, 5 );



	btmSizer->Add( 20, 0, 1, wxEXPAND, 5 );

	findSizer->Add( btmSizer, 1, wxALL|wxEXPAND, 5 );

	this->SetSizer( findSizer );
	this->Layout();

	this->Centre( wxBOTH );
}

//--

luReplaceDlg::luReplaceDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	setupControls();
}

luReplaceDlg::~luReplaceDlg()
{
}

BEGIN_EVENT_TABLE(luReplaceDlg, wxDialog)
	//EVT_KEY_DOWN(luReplaceDlg::OnKeyDown)
	EVT_BUTTON(ID_FIND_BUTTON, luReplaceDlg::OnFindClick)
	EVT_BUTTON(ID_REPLACE_BUTTON, luReplaceDlg::OnReplaceClick)
	EVT_BUTTON(ID_REPLACE_ALL_BUTTON, luReplaceDlg::OnReplaceAllClick)
END_EVENT_TABLE()

void luReplaceDlg::OnFindClick( wxCommandEvent& event )
{
	alertDlg("find");
}

void luReplaceDlg::OnReplaceClick( wxCommandEvent& event )
{
	alertDlg("replace");
}

void luReplaceDlg::OnReplaceAllClick( wxCommandEvent& event )
{
	alertDlg("replaceall");
}


void luReplaceDlg::setupControls()
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxFlexGridSizer* replaceSizer;
	replaceSizer = new wxFlexGridSizer( 4, 1, 0, 0 );
	replaceSizer->AddGrowableCol( 0 );
	replaceSizer->AddGrowableRow( 2 );
	replaceSizer->SetFlexibleDirection( wxBOTH );
	replaceSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* topSizer;
	topSizer = new wxFlexGridSizer( 1, 2, 0, 10 );
	topSizer->AddGrowableCol( 1 );
	topSizer->SetFlexibleDirection( wxBOTH );
	topSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Search for:"), wxDefaultPosition, wxSize( 80,-1 ), 0 );
	m_staticText1->Wrap( -1 );
	topSizer->Add( m_staticText1, 0, wxALL, 5 );

	m_searchCombo = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0, NULL, 0 );
	topSizer->Add( m_searchCombo, 0, wxEXPAND|wxRIGHT, 5 );

	replaceSizer->Add( topSizer, 1, wxEXPAND|wxTOP, 5 );

	wxFlexGridSizer* topSizer1;
	topSizer1 = new wxFlexGridSizer( 1, 2, 0, 10 );
	topSizer1->AddGrowableCol( 1 );
	topSizer1->SetFlexibleDirection( wxBOTH );
	topSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText11 = new wxStaticText( this, wxID_ANY, wxT("Replace with:"), wxDefaultPosition, wxSize( 80,-1 ), 0 );
	m_staticText11->Wrap( -1 );
	topSizer1->Add( m_staticText11, 0, wxALL, 5 );

	m_replaceCombo = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0, NULL, 0 );
	topSizer1->Add( m_replaceCombo, 0, wxEXPAND|wxRIGHT, 5 );

	replaceSizer->Add( topSizer1, 1, wxEXPAND, 5 );

	wxFlexGridSizer* midSizer;
	midSizer = new wxFlexGridSizer( 2, 2, 0, 10 );
	midSizer->AddGrowableCol( 1 );
	midSizer->AddGrowableRow( 1 );
	midSizer->SetFlexibleDirection( wxBOTH );
	midSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxStaticBoxSizer* opt1Sizer;
	opt1Sizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("options") ), wxVERTICAL );

	m_matchCase = new wxCheckBox( this, wxID_ANY, wxT("Match Case"), wxDefaultPosition, wxDefaultSize, 0 );
	opt1Sizer->Add( m_matchCase, 0, wxALL, 5 );

	m_matchWholeWords = new wxCheckBox( this, wxID_ANY, wxT("Match whole words only"), wxDefaultPosition, wxDefaultSize, 0 );
	opt1Sizer->Add( m_matchWholeWords, 0, wxALL, 5 );

	m_regularExp = new wxCheckBox( this, wxID_ANY, wxT("Regular expressions"), wxDefaultPosition, wxDefaultSize, 0 );
	opt1Sizer->Add( m_regularExp, 0, wxALL, 5 );

	m_selectionOnly = new wxCheckBox( this, wxID_ANY, wxT("Replace in selection only"), wxDefaultPosition, wxDefaultSize, 0 );
	opt1Sizer->Add( m_selectionOnly, 0, wxALL, 5 );

	midSizer->Add( opt1Sizer, 1, wxEXPAND, 5 );

	wxStaticBoxSizer* opt2Sizer;
	opt2Sizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("direction") ), wxVERTICAL );

	m_dirFoward = new wxRadioButton( this, wxID_ANY, wxT("Foward"), wxDefaultPosition, wxDefaultSize, 0 );
	opt2Sizer->Add( m_dirFoward, 0, wxALL, 5 );

	m_dirBackward = new wxRadioButton( this, wxID_ANY, wxT("Backward"), wxDefaultPosition, wxDefaultSize, 0 );
	opt2Sizer->Add( m_dirBackward, 0, wxALL, 5 );

	midSizer->Add( opt2Sizer, 1, wxEXPAND, 5 );

	replaceSizer->Add( midSizer, 1, wxEXPAND, 5 );

	wxFlexGridSizer* btmSizer;
	btmSizer = new wxFlexGridSizer( 1, 6, 0, 0 );
	btmSizer->AddGrowableCol( 0 );
	btmSizer->SetFlexibleDirection( wxBOTH );
	btmSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );


	btmSizer->Add( 0, 0, 1, 0, 5 );

	m_findBtn = new wxButton( this, ID_FIND_BUTTON, wxT("Next Match"), wxDefaultPosition, wxDefaultSize, 0 );
	m_findBtn->SetDefault();
	btmSizer->Add( m_findBtn, 0, wxALL, 5 );

	m_replaceBtn = new wxButton( this, ID_REPLACE_BUTTON, wxT("Replace"), wxDefaultPosition, wxDefaultSize, 0 );
	btmSizer->Add( m_replaceBtn, 0, wxALL, 5 );

	m_repAllBtn = new wxButton( this, ID_REPLACE_ALL_BUTTON, wxT("Replace All"), wxDefaultPosition, wxDefaultSize, 0 );
	btmSizer->Add( m_repAllBtn, 0, wxALL, 5 );

	m_closeBtn = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	btmSizer->Add( m_closeBtn, 0, wxALL, 5 );


	btmSizer->Add( 20, 0, 1, wxEXPAND, 5 );

	replaceSizer->Add( btmSizer, 1, wxEXPAND, 5 );

	this->SetSizer( replaceSizer );
	this->Layout();

	this->Centre( wxBOTH );
}