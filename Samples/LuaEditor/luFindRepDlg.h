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


#ifndef  _luFindRepDlg_h_
#define  _luFindRepDlg_h_

class luEdit;

class luFindDlg : public wxDialog 
{
	void setupControls();
protected:
	wxStaticText* m_staticText1;
	wxComboBox* m_searchCombo;
	wxCheckBox* m_matchCase;
	wxCheckBox* m_matchWholeWords;
	wxCheckBox* m_regularExp;
	wxRadioButton* m_dirFoward;
	wxRadioButton* m_dirBackward;
		
	wxButton* m_findBtn;
	wxButton* m_closeBtn;

	bool m_firstSearch;
	
	void OnKeyDown(wxKeyEvent& evnet);
	virtual void OnFindClick( wxCommandEvent& event );
public:
		
	luFindDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Search"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 430,200 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
	~luFindDlg();
	
	DECLARE_EVENT_TABLE()
};


class luReplaceDlg : public wxDialog 
{
	void setupControls();
protected:
	wxStaticText* m_staticText1;
	wxComboBox* m_searchCombo;
	wxStaticText* m_staticText11;
	wxComboBox* m_replaceCombo;
	wxCheckBox* m_matchCase;
	wxCheckBox* m_matchWholeWords;
	wxCheckBox* m_regularExp;
	wxCheckBox* m_selectionOnly;
	wxRadioButton* m_dirFoward;
	wxRadioButton* m_dirBackward;
		
	wxButton* m_findBtn;
	wxButton* m_replaceBtn;
	wxButton* m_repAllBtn;
	wxButton* m_closeBtn;

	void OnFindClick( wxCommandEvent& event );
	void OnReplaceClick( wxCommandEvent& event );
	void OnReplaceAllClick( wxCommandEvent& event );
	
public:
		
	luReplaceDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Find and Replace"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 430,247 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
	~luReplaceDlg();
	
	DECLARE_EVENT_TABLE()
};


#endif //_luFindRepDlg_h_