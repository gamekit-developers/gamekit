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

#ifndef _luEdit_h_
#define _luEdit_h_

class luEdit : public wxStyledTextCtrl
{
	int m_marginLineNumber;
	int m_marginFolder;
	int m_marginBookmark;

	wxString m_fileName;

	void OnSetFocus(wxFocusEvent& event);
	void OnKillFocus(wxFocusEvent& event);
	void OnSize(wxSizeEvent &event );

    // edit
    void OnEditRedo(wxCommandEvent &event);
    void OnEditUndo(wxCommandEvent &event);
    void OnEditClear(wxCommandEvent &event);
    void OnEditCut(wxCommandEvent &event);
    void OnEditCopy(wxCommandEvent &event);
    void OnEditPaste(wxCommandEvent &event);
    // find
    void OnFind(wxCommandEvent &event);
    void OnFindNext(wxCommandEvent &event);
    void OnReplace(wxCommandEvent &event);
    void OnReplaceNext(wxCommandEvent &event);
    void OnBraceMatch(wxCommandEvent &event);
    void OnGoto(wxCommandEvent &event);
    void OnEditIndentIncrease(wxCommandEvent &event);
    void OnEditIndentReduce(wxCommandEvent &event);
    void OnEditSelectAll(wxCommandEvent &event);
    void OnEditSelectLine(wxCommandEvent &event);
    // view
    void OnHilightLang(wxCommandEvent &event);
    void OnDisplayEOL(wxCommandEvent &event);
    void OnIndentGuide(wxCommandEvent &event);
    void OnLineNumber(wxCommandEvent &event);
    void OnLongLineOn(wxCommandEvent &event);
    void OnWhiteSpace(wxCommandEvent &event);
    void OnFoldToggle(wxCommandEvent &event);
    void OnSetOverType(wxCommandEvent &event);
    void OnSetReadOnly(wxCommandEvent &event);
    void OnWrapmodeOn(wxCommandEvent &event);
    void OnUseCharset(wxCommandEvent &event);
	void OnBookmarkToggle(wxCommandEvent &event);
	void OnBookmarkClearAll(wxCommandEvent &event);
	void OnBookmarkNext(wxCommandEvent &event);
	void OnBookmarkPrev(wxCommandEvent &event);
    // extra
    void OnChangeCase(wxCommandEvent &event);
    void OnConvertEOL(wxCommandEvent &event);
    // stc
    void OnMarginClick(wxStyledTextEvent &event);
    void OnCharAdded(wxStyledTextEvent &event);

	bool hasBookmark(int line);
public:
	luEdit(wxWindow* parent);
	~luEdit();

	void gotoLineEnsureVisible(int line);

	void setStyleColor(int style, const wxColour& foreCol, const wxColour& backCol=*wxWHITE);
	void setStyleFont(int style, const wxString& fontName, int fontSize, bool fontBold=false);

	bool openFile(const wxString& fileName);
	const wxString& getFileName() { return m_fileName; }

	bool isModified () { return GetModify() && !GetReadOnly(); }

	wxString getSelectionText();

	DECLARE_EVENT_TABLE()
};

#endif //_luEdit_h_