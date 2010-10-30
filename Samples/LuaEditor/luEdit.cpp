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
#include "luEdit.h"
#include "luMainFrame.h"
#include "luFindRepDlg.h"
#include "luConfig.h"

#define DEFAULT_LUA_KEYWORDS \
	"and break do else elseif " \
	"end false for function if " \
	"in local nil not or " \
	"repeat return then true until while "

#define DEFAULT_LUA_FONT_NAME	"Verdana"
#define DEFAULT_LUA_FONT_SIZE	11
#define LINE_MARGIN_ID			0
#define DIVIDER_MARGIN_ID		1
#define FOLDER_MARGIN_ID		2
#define DEFAULT_FOLDER_MARGIN	16


BEGIN_EVENT_TABLE (luEdit, wxStyledTextCtrl)
	// common

	EVT_SET_FOCUS(luEdit::OnSetFocus)
	EVT_KILL_FOCUS(luEdit::OnKillFocus)
	EVT_SIZE(luEdit::OnSize)

	// edit
	EVT_MENU(wxID_CLEAR,			luEdit::OnEditClear)
	EVT_MENU(wxID_CUT,				luEdit::OnEditCut)
	EVT_MENU(wxID_COPY,				luEdit::OnEditCopy)
	EVT_MENU(wxID_PASTE,			luEdit::OnEditPaste)
	EVT_MENU(ID_CODE_INDENTINC,		luEdit::OnEditIndentInc)
	EVT_MENU(ID_CODE_INDENTRED,		luEdit::OnEditIndentRed)
	EVT_MENU(wxID_SELECTALL,		luEdit::OnEditSelectAll)
	EVT_MENU(ID_CODE_SELECTLINE,	luEdit::OnEditSelectLine)
	EVT_MENU(wxID_REDO,				luEdit::OnEditRedo)
	EVT_MENU(wxID_UNDO,				luEdit::OnEditUndo)
	// find	 ID_CODE
	EVT_MENU(wxID_FIND,				luEdit::OnFind)
	EVT_MENU(ID_CODE_FINDNEXT,		luEdit::OnFindNext)
	EVT_MENU(ID_CODE_REPLACE,		luEdit::OnReplace)
	EVT_MENU(ID_CODE_REPLACENEXT,	luEdit::OnReplaceNext)
	EVT_MENU(ID_CODE_BRACEMATCH,	luEdit::OnBraceMatch)
	EVT_MENU(ID_CODE_GOTO,			luEdit::OnGoto)
	// view
	EVT_MENU_RANGE (ID_CODE_HILIGHTFIRST, ID_CODE_HILIGHTLAST, luEdit::OnHilightLang)
	EVT_MENU(ID_CODE_DISPLAYEOL,	luEdit::OnDisplayEOL)
	EVT_MENU(ID_CODE_INDENTGUIDE,	luEdit::OnIndentGuide)
	EVT_MENU(ID_CODE_LINENUMBER,	luEdit::OnLineNumber)
	EVT_MENU(ID_CODE_LONGLINEON,	luEdit::OnLongLineOn)
	EVT_MENU(ID_CODE_WHITESPACE,	luEdit::OnWhiteSpace)
	EVT_MENU(ID_CODE_FOLDTOGGLE,	luEdit::OnFoldToggle)
	EVT_MENU(ID_CODE_OVERTYPE,		luEdit::OnSetOverType)
	EVT_MENU(ID_CODE_READONLY,		luEdit::OnSetReadOnly)
	EVT_MENU(ID_CODE_WRAPMODEON,	luEdit::OnWrapmodeOn)
	EVT_MENU(ID_CODE_CHARSETANSI,	luEdit::OnUseCharset)
	EVT_MENU(ID_CODE_CHARSETMAC,	luEdit::OnUseCharset)
	// extra ID_CODE
	EVT_MENU(ID_CODE_CHANGELOWER,	luEdit::OnChangeCase)
	EVT_MENU(ID_CODE_CHANGEUPPER,	luEdit::OnChangeCase)
	EVT_MENU(ID_CODE_CONVERTCR,		luEdit::OnConvertEOL)
	EVT_MENU(ID_CODE_CONVERTCRLF,	luEdit::OnConvertEOL)
	EVT_MENU(ID_CODE_CONVERTLF,		luEdit::OnConvertEOL)
	// stc
	EVT_STC_MARGINCLICK (wxID_ANY,	luEdit::OnMarginClick)
	EVT_STC_CHARADDED (wxID_ANY,	luEdit::OnCharAdded)
END_EVENT_TABLE()

luEdit::luEdit(wxWindow *parent) :  
	wxStyledTextCtrl(parent),
	m_displayLineNumber(true),
	m_displayFolder(true)
{	
	gkString keywords = LuConfig.getEditKeywords(DEFAULT_LUA_KEYWORDS);
	SetLexer(SCLEX_LUA);
	SetKeyWords(0, keywords);

	setStyleColor(STYLE_DEFAULT, wxColour(0, 0, 0));

	gkString fontName = LuConfig.getEditFontName(DEFAULT_LUA_FONT_NAME);
	int fontSize = LuConfig.getEditFontSize(DEFAULT_LUA_FONT_SIZE);
	setStyleFont(STYLE_DEFAULT, fontName, fontSize);
	
	m_marginLineNumber = LuConfig.getEditLineNumberMargin(TextWidth(wxSTC_STYLE_LINENUMBER, wxT("_9999")));
	m_marginFolder = LuConfig.getEditFolderMargin(DEFAULT_FOLDER_MARGIN);


	StyleClearAll();

	setStyleColor(SCE_LUA_DEFAULT, wxColour(0, 0, 0));
	setStyleColor(SCE_LUA_COMMENT, wxColour(0, 0x80, 0));
	setStyleColor(SCE_LUA_COMMENTLINE, wxColour(0, 0x80, 0));
	setStyleColor(SCE_LUA_COMMENTDOC, wxColour(0, 0x80, 0));
	//setStyleColor(SCE_LUA_COMMENTLINEDOC, wxColour(0, 0x80, 0));
	//setStyleColor(SCE_LUA_COMMENTDOCKEYWORD, wxColour(0, 0x80, 0));
	//setStyleColor(SCE_LUA_COMMENTDOCKEYWORDERROR, wxColour(0, 0x80, 0));
	setStyleColor(SCE_LUA_NUMBER, wxColour(0, 0x80, 0x80));
	setStyleColor(SCE_LUA_WORD, wxColour(0, 0, 0x80));
	StyleSetBold(SCE_LUA_WORD, 1);	
	setStyleColor(SCE_LUA_STRING, wxColour(0x80, 0, 0x80));
	setStyleColor(SCE_LUA_IDENTIFIER, wxColour(0, 0, 0));
	setStyleColor(SCE_LUA_PREPROCESSOR, wxColour(0x80, 0, 0));
	setStyleColor(SCE_LUA_OPERATOR, wxColour(0x80, 0x80, 0));


	SetMarginType(LINE_MARGIN_ID, wxSTC_MARGIN_NUMBER);
	SetMarginType(FOLDER_MARGIN_ID, wxSTC_MARGIN_SYMBOL);
	SetMarginMask(FOLDER_MARGIN_ID, wxSTC_MASK_FOLDERS);

	SetMarginWidth(LINE_MARGIN_ID, m_marginLineNumber);
	SetMarginWidth(FOLDER_MARGIN_ID, m_marginFolder);

	// set margin as unused
	SetMarginType (DIVIDER_MARGIN_ID, wxSTC_MARGIN_SYMBOL);
	SetMarginWidth (DIVIDER_MARGIN_ID, 0);
	SetMarginSensitive (DIVIDER_MARGIN_ID, false);

	// markers
	MarkerDefine(wxSTC_MARKNUM_FOLDER,        SC_MARK_PLUS,		wxT("BLACK"), wxT("BLACK"));
	MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN,    SC_MARK_MINUS,	wxT("BLACK"), wxT("BLACK"));
	MarkerDefine(wxSTC_MARKNUM_FOLDERSUB,     wxSTC_MARK_EMPTY, wxT("BLACK"), wxT("BLACK"));
	MarkerDefine(wxSTC_MARKNUM_FOLDEREND,     wxSTC_MARK_EMPTY,	wxT("BLACK"), wxT("WHITE"));
	MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_EMPTY,	wxT("BLACK"), wxT("WHITE"));
	MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY, wxT("BLACK"), wxT("BLACK"));
	MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL,    wxSTC_MARK_EMPTY, wxT("BLACK"), wxT("BLACK"));



	SetMarginWidth (FOLDER_MARGIN_ID,m_marginFolder);
	SetMarginSensitive (FOLDER_MARGIN_ID, true);
	SetProperty(wxT("fold"), "1");
	SetProperty(wxT("fold.comment"), "1");
	SetProperty(wxT("fold.compact"), "1");
	SetProperty(wxT("fold.at.else"), "1");
	SetProperty(wxT("fold.flags"), "1");
	SetProperty(wxT("fold.preprocessor"), "1");
	SetProperty(wxT("fold.html"), "1");
	SetProperty(wxT("fold.html.preprocessor"), "1");
	SetProperty(wxT("styling.within.preprocessor"), "1");        



	SetFoldFlags (wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED |
		wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED);
}

luEdit::~luEdit()
{
	luMainFrame* frame = getLuMainFrame();
	if (frame) frame->setFocusEdit(this, false);
}



void luEdit::setStyleColor(int style, const wxColour& foreCol, const wxColour& backCol)
{
	StyleSetForeground(style, foreCol);
	StyleSetBackground(style, backCol);
}

void luEdit::setStyleFont(int style, const wxString& fontName, int fontSize)
{
	wxFont font(fontSize, wxMODERN, wxNORMAL, wxNORMAL, false, fontName);
	StyleSetFont(style, font);
}

bool luEdit::openFile(const wxString& fileName)
{
	//	ClearAll();

	if (!LoadFile(fileName))
		return false;

	SetSelection(0, 0);
	ClearSelections();
	EmptyUndoBuffer();

	SetModified(false);

	m_fileName = fileName;

	return true;
}

wxString luEdit::getSelectionText()
{
	return GetTextRange(GetSelectionStart(), GetSelectionEnd());
}

void luEdit::OnSetFocus(wxFocusEvent& event)
{
	wxStyledTextCtrl::OnGainFocus(event);

	luMainFrame* frame = getLuMainFrame();
	if (frame) frame->setFocusEdit(this);
}

void luEdit::OnKillFocus(wxFocusEvent& event)
{
	wxStyledTextCtrl::OnLoseFocus(event);

	luMainFrame* frame = getLuMainFrame();
	if (frame) frame->setFocusEdit(this, false);
}


//-- from wxWidget samples/stctest

void luEdit::OnSize( wxSizeEvent& event ) 
{
	int x = GetClientSize().x + 
		(m_displayLineNumber ? m_marginLineNumber : 0) +
		(m_displayFolder    ? m_marginFolder : 0);

	if (x > 0) SetScrollWidth (x);

	event.Skip();
}

// edit event handlers
void luEdit::OnEditRedo(wxCommandEvent &WXUNUSED(event)) 
{
	if (!CanRedo()) 
		return;
	Redo ();
}

void luEdit::OnEditUndo(wxCommandEvent &WXUNUSED(event)) 
{
	if (!CanUndo()) 
		return;
	Undo ();
}

void luEdit::OnEditClear(wxCommandEvent &WXUNUSED(event)) 
{
	if (GetReadOnly()) 
		return;
	Clear ();
}

void luEdit::OnEditCut(wxCommandEvent &WXUNUSED(event)) 
{
	if (GetReadOnly() || (GetSelectionEnd()-GetSelectionStart() <= 0)) 
		return;
	Cut ();
}

void luEdit::OnEditCopy(wxCommandEvent &WXUNUSED(event)) 
{
	if (GetSelectionEnd()-GetSelectionStart() <= 0) return;
	Copy ();
}

void luEdit::OnEditPaste(wxCommandEvent &WXUNUSED(event)) 
{
	if (!CanPaste()) return;
	Paste ();
}

void luEdit::OnFind(wxCommandEvent &WXUNUSED(event)) 
{
	luFindDlg dlg(this);
	dlg.ShowModal();
}

void luEdit::OnFindNext(wxCommandEvent &WXUNUSED(event)) 
{
}

void luEdit::OnReplace(wxCommandEvent &WXUNUSED(event)) 
{
	luReplaceDlg dlg(this);
	dlg.ShowModal();
}

void luEdit::OnReplaceNext(wxCommandEvent &WXUNUSED(event)) 
{
}

void luEdit::OnBraceMatch(wxCommandEvent &WXUNUSED(event)) 
{
	int min = GetCurrentPos ();
	int max = BraceMatch (min);
	if (max > (min+1)) 
	{
		BraceHighlight (min+1, max);
		SetSelection (min+1, max);
	}
	else
	{
		BraceBadLight (min);
	}
}

void luEdit::OnGoto(wxCommandEvent &WXUNUSED(event)) 
{	
	wxString str = wxGetTextFromUser("Enter Line Number: ", "Goto Line", "");
	unsigned long line= 0;
	if (str.ToULong(&line))
		GotoLine(line);
}

void luEdit::OnEditIndentInc(wxCommandEvent &WXUNUSED(event)) 
{
	CmdKeyExecute (wxSTC_CMD_TAB);
}

void luEdit::OnEditIndentRed(wxCommandEvent &WXUNUSED(event)) 
{
	CmdKeyExecute (wxSTC_CMD_DELETEBACK);
}

void luEdit::OnEditSelectAll(wxCommandEvent &WXUNUSED(event)) 
{
	SetSelection (0, GetTextLength ());
}

void luEdit::OnEditSelectLine(wxCommandEvent &WXUNUSED(event)) 
{
	int lineStart = PositionFromLine (GetCurrentLine());
	int lineEnd = PositionFromLine (GetCurrentLine() + 1);
	SetSelection (lineStart, lineEnd);
}

void luEdit::OnHilightLang(wxCommandEvent &event) 
{
}

void luEdit::OnDisplayEOL(wxCommandEvent &WXUNUSED(event)) 
{
	SetViewEOL (!GetViewEOL());
}

void luEdit::OnIndentGuide(wxCommandEvent &WXUNUSED(event)) 
{
	SetIndentationGuides (!GetIndentationGuides());
}

void luEdit::OnLineNumber(wxCommandEvent &WXUNUSED(event)) 
{
	SetMarginWidth(m_marginLineNumber,
		GetMarginWidth (0) == 0 ? m_marginLineNumber : 0);
}

void luEdit::OnLongLineOn(wxCommandEvent &WXUNUSED(event)) 
{
	SetEdgeMode(GetEdgeMode() == 0? wxSTC_EDGE_LINE: wxSTC_EDGE_NONE);
}

void luEdit::OnWhiteSpace(wxCommandEvent &WXUNUSED(event)) 
{
	SetViewWhiteSpace(
		GetViewWhiteSpace() == 0 ? wxSTC_WS_VISIBLEALWAYS: wxSTC_WS_INVISIBLE);
}

void luEdit::OnFoldToggle(wxCommandEvent &WXUNUSED(event)) 
{
	ToggleFold (GetFoldParent(GetCurrentLine()));
}

void luEdit::OnSetOverType(wxCommandEvent &WXUNUSED(event)) 
{
	SetOvertype (!GetOvertype());
}

void luEdit::OnSetReadOnly(wxCommandEvent &WXUNUSED(event)) 
{
	SetReadOnly (!GetReadOnly());
}

void luEdit::OnWrapmodeOn(wxCommandEvent &WXUNUSED(event)) 
{
	SetWrapMode (GetWrapMode() == 0? wxSTC_WRAP_WORD: wxSTC_WRAP_NONE);
}

void luEdit::OnUseCharset(wxCommandEvent &event) 
{    
	int charset = GetCodePage();

	switch (event.GetId()) 
	{
	case ID_CODE_CHARSETANSI: 
		charset = wxSTC_CHARSET_ANSI; 
		break;
	case ID_CODE_CHARSETMAC:
		charset = wxSTC_CHARSET_ANSI; 
		break;
	}

	for (int i = 0; i < wxSTC_STYLE_LASTPREDEFINED; i++) 
	{
		StyleSetCharacterSet (i, charset);
	}
	SetCodePage (charset);
}


void luEdit::OnChangeCase(wxCommandEvent &event) 
{
	switch (event.GetId()) 
	{
	case ID_CODE_CHANGELOWER: 
		CmdKeyExecute (wxSTC_CMD_LOWERCASE);
		break;

	case ID_CODE_CHANGEUPPER: 
		CmdKeyExecute (wxSTC_CMD_UPPERCASE);
		break;

	}
}

void luEdit::OnConvertEOL(wxCommandEvent &event) 
{
	int eolMode = GetEOLMode();
	switch (event.GetId()) 
	{
	case ID_CODE_CONVERTCR: 
		eolMode = wxSTC_EOL_CR; 
		break;
	case ID_CODE_CONVERTCRLF: 
		eolMode = wxSTC_EOL_CRLF; 
		break;
	case ID_CODE_CONVERTLF: 
		eolMode = wxSTC_EOL_LF; 
		break;
	}
	ConvertEOLs (eolMode);
	SetEOLMode (eolMode);
}

void luEdit::OnMarginClick (wxStyledTextEvent &event) 
{
	if (event.GetMargin() == 2) 
	{
		int lineClick = LineFromPosition (event.GetPosition());
		int levelClick = GetFoldLevel (lineClick);

		if ((levelClick & wxSTC_FOLDLEVELHEADERFLAG) > 0) 
			ToggleFold (lineClick);
	}
}

void luEdit::OnCharAdded (wxStyledTextEvent &event) 
{
	char chr = (char)event.GetKey();
	int currentLine = GetCurrentLine();
	// Change this if support for mac files with \r is needed
	if (chr == '\n') 
	{
		int lineInd = 0;

		if (currentLine > 0) 
			lineInd = GetLineIndentation(currentLine - 1);

		if (lineInd == 0) 
			return;

		SetLineIndentation (currentLine, lineInd);
		GotoPos(PositionFromLine (currentLine) + lineInd);
	}
}


