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
#define BOOKMARK_MARGIN_ID		1
#define FOLDER_MARGIN_ID		2
#define DEFAULT_FOLDER_MARGIN	16
#define DEFAULT_BOOKMARK_MARGIN 16


#define BOOKMARK_ID				1

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
	EVT_MENU(ID_CODE_INDENTINC,		luEdit::OnEditIndentIncrease)
	EVT_MENU(ID_CODE_INDENTRED,		luEdit::OnEditIndentReduce)
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
	EVT_MENU(ID_CODE_BOOKMARK_TOGGLE, luEdit::OnBookmarkToggle)
	EVT_MENU(ID_CODE_BOOKMARK_CELAR, luEdit::OnBookmarkClearAll)
	EVT_MENU(ID_CODE_BOOKMARK_NEXT, luEdit::OnBookmarkNext)
	EVT_MENU(ID_CODE_BOOKMARK_PREV, luEdit::OnBookmarkPrev)
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
	m_marginLineNumber(0),
	m_marginFolder(0),
	m_marginBookmark(0)
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
	m_marginBookmark = LuConfig.getEditFolderMargin(DEFAULT_BOOKMARK_MARGIN);

	SetTabWidth(LuConfig.getEditTabSize());
	SetIndent(LuConfig.getEditIndentSize());

	StyleClearAll();

	setStyleColor(SCE_LUA_DEFAULT,		LuConfig.getEditDefaultFontColor(),			LuConfig.getEditDefaultFontBgColor());
	setStyleColor(SCE_LUA_COMMENT,		LuConfig.getEditCommentFontColor(),			LuConfig.getEditCommentFontBgColor());
	setStyleColor(SCE_LUA_COMMENTLINE,	LuConfig.getEditCommentLineFontColor(),		LuConfig.getEditCommentLineFontBgColor());
	setStyleColor(SCE_LUA_COMMENTDOC,	LuConfig.getEditCommentDocFontColor(),		LuConfig.getEditCommentDocFontBgColor());
	setStyleColor(SCE_LUA_NUMBER,		LuConfig.getEditNumberFontColor(),			LuConfig.getEditNumberFontBgColor());
	setStyleColor(SCE_LUA_WORD,			LuConfig.getEditWordFontColor(),			LuConfig.getEditWordFontBgColor());
	setStyleColor(SCE_LUA_STRING,		LuConfig.getEditStringFontColor(),			LuConfig.getEditStringFontBgColor());
	setStyleColor(SCE_LUA_IDENTIFIER,	LuConfig.getEditIdentifierFontColor(),		LuConfig.getEditIdentifierFontBgColor());
	setStyleColor(SCE_LUA_PREPROCESSOR,	LuConfig.getEditPreprocessorFontColor(),	LuConfig.getEditPreprocessorFontBgColor());
	setStyleColor(SCE_LUA_OPERATOR,		LuConfig.getEditOperatorFontColor(),		LuConfig.getEditOperatorFontBgColor());
													
	StyleSetBold(SCE_LUA_DEFAULT,		LuConfig.getEditDefaultFontBold());		
	StyleSetBold(SCE_LUA_COMMENT,		LuConfig.getEditCommentFontBold());		
	StyleSetBold(SCE_LUA_COMMENTLINE,	LuConfig.getEditCommentLineFontBold());	
	StyleSetBold(SCE_LUA_COMMENTDOC,	LuConfig.getEditCommentDocFontBold());	
	StyleSetBold(SCE_LUA_NUMBER,		LuConfig.getEditNumberFontBold());		
	StyleSetBold(SCE_LUA_WORD,			LuConfig.getEditWordFontBold());			
	StyleSetBold(SCE_LUA_STRING,		LuConfig.getEditStringFontBold());		
	StyleSetBold(SCE_LUA_IDENTIFIER,	LuConfig.getEditIdentifierFontBold());	
	StyleSetBold(SCE_LUA_PREPROCESSOR,	LuConfig.getEditPreprocessorFontBold());	
	StyleSetBold(SCE_LUA_OPERATOR,		LuConfig.getEditOperatorFontBold());		

	SetMarginType(LINE_MARGIN_ID, wxSTC_MARGIN_NUMBER);
	SetMarginType(FOLDER_MARGIN_ID, wxSTC_MARGIN_SYMBOL);
	SetMarginMask(FOLDER_MARGIN_ID, wxSTC_MASK_FOLDERS);

	SetMarginWidth(LINE_MARGIN_ID, LuConfig.getEditViewLineNumber() ? m_marginLineNumber : 0);
	SetMarginWidth(FOLDER_MARGIN_ID, LuConfig.getEditViewFolder() ? m_marginFolder : 0);
	SetMarginWidth(BOOKMARK_MARGIN_ID, LuConfig.getEditViewBookmark() ? m_marginBookmark : 0);

	SetViewEOL(LuConfig.getEditViewEOL());
	SetViewWhiteSpace(LuConfig.getEditViewWhiteSpace() ? wxSTC_WS_VISIBLEALWAYS: wxSTC_WS_INVISIBLE);
	SetIndentationGuides(LuConfig.getEditViewIndentGuide());
	SetEdgeMode(LuConfig.getEditViewLongLineMark() ? wxSTC_EDGE_LINE: wxSTC_EDGE_NONE);


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

	SetFoldFlags (wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED | wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED);
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

void luEdit::setStyleFont(int style, const wxString& fontName, int fontSize, bool fontBold)
{
	wxFont font(fontSize, wxMODERN, wxNORMAL, wxNORMAL, false, fontName);
	StyleSetFont(style, font);
	StyleSetBold(style, fontBold);
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
		(GetMarginWidth(LINE_MARGIN_ID)		? m_marginLineNumber : 0) +
		(GetMarginWidth(FOLDER_MARGIN_ID)	? m_marginFolder : 0) +
		(GetMarginWidth(BOOKMARK_MARGIN_ID)	? m_marginBookmark : 0);

	if (x > 0) SetScrollWidth(x);

	event.Skip();
}

// edit event handlers
void luEdit::OnEditRedo(wxCommandEvent &WXUNUSED(event)) 
{
	if (!CanRedo()) 
		return;
	Redo();
}

void luEdit::OnEditUndo(wxCommandEvent &WXUNUSED(event)) 
{
	if (!CanUndo()) 
		return;
	Undo();
}

void luEdit::OnEditClear(wxCommandEvent &WXUNUSED(event)) 
{
	if (GetReadOnly()) 
		return;
	Clear();
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
	Copy();
}

void luEdit::OnEditPaste(wxCommandEvent &WXUNUSED(event)) 
{
	if (!CanPaste()) return;
	Paste();
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
	int min = GetCurrentPos();
	int max = BraceMatch(min);
	if (max > (min+1)) 
	{
		BraceHighlight(min+1, max);
		SetSelection(min+1, max);
	}
	else
	{
		BraceBadLight(min);
	}
}

void luEdit::OnGoto(wxCommandEvent &WXUNUSED(event)) 
{	
	wxString str = wxGetTextFromUser("Enter Line Number: ", "Goto Line", "");
	unsigned long line = 0;
	if (str.ToULong(&line))
		GotoLine(line);
}

void luEdit::OnEditIndentIncrease(wxCommandEvent &WXUNUSED(event)) 
{
	CmdKeyExecute(wxSTC_CMD_TAB);
}

void luEdit::OnEditIndentReduce(wxCommandEvent &WXUNUSED(event)) 
{
	CmdKeyExecute(wxSTC_CMD_BACKTAB);
}

void luEdit::OnEditSelectAll(wxCommandEvent &WXUNUSED(event)) 
{
	SetSelection(0, GetTextLength());
}

void luEdit::OnEditSelectLine(wxCommandEvent &WXUNUSED(event)) 
{
	int lineStart = PositionFromLine(GetCurrentLine());
	int lineEnd = PositionFromLine(GetCurrentLine() + 1);
	SetSelection(lineStart, lineEnd);
}

void luEdit::OnHilightLang(wxCommandEvent &event) 
{
}

void luEdit::OnDisplayEOL(wxCommandEvent &WXUNUSED(event)) 
{
	SetViewEOL(!GetViewEOL());
}

void luEdit::OnIndentGuide(wxCommandEvent &WXUNUSED(event)) 
{
	SetIndentationGuides(!GetIndentationGuides());
}

void luEdit::OnLineNumber(wxCommandEvent &WXUNUSED(event)) 
{
	SetMarginWidth(LINE_MARGIN_ID,
		GetMarginWidth(LINE_MARGIN_ID) == 0 ? m_marginLineNumber : 0);
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
	ToggleFold(GetFoldParent(GetCurrentLine()));
}



bool luEdit::hasBookmark(int line)
{
	return (MarkerGet(line) & (1<<BOOKMARK_ID)) != 0;
}


void luEdit::gotoLineEnsureVisible(int line)
{
	EnsureVisible(line);
	GotoLine(line);
}

void luEdit::OnBookmarkToggle(wxCommandEvent &WXUNUSED(event)) 
{
	int line = GetCurrentLine();
	if (hasBookmark(line))
		MarkerDelete(line, BOOKMARK_ID);
	else
		MarkerAdd(line, BOOKMARK_ID);
}

void luEdit::OnBookmarkClearAll(wxCommandEvent &WXUNUSED(event)) 
{
	MarkerDeleteAll(BOOKMARK_ID);
}

void luEdit::OnBookmarkNext(wxCommandEvent &WXUNUSED(event)) 
{	
	int line = GetCurrentLine();
	int nline = MarkerNext(line+1, 1<<BOOKMARK_ID);
	if (nline >= 0 && nline != line)	
		gotoLineEnsureVisible(nline);	
	else
	{
		nline = MarkerNext(0, 1<<BOOKMARK_ID);
		if (nline >= 0 && nline != line)
			gotoLineEnsureVisible(nline);
	}
}

void luEdit::OnBookmarkPrev(wxCommandEvent &WXUNUSED(event)) 
{	
	int line = GetCurrentLine();
	int nline = MarkerPrevious(line-1, 1<<BOOKMARK_ID);
	if (nline >= 0 && nline != line)	
		gotoLineEnsureVisible(nline);	
	else
	{
		nline = MarkerPrevious(GetLineCount()-1, 1<<BOOKMARK_ID);
		if (nline >= 0 && nline != line)
			gotoLineEnsureVisible(nline);
	}
}

void luEdit::OnSetOverType(wxCommandEvent &WXUNUSED(event)) 
{
	SetOvertype(!GetOvertype());
}

void luEdit::OnSetReadOnly(wxCommandEvent &WXUNUSED(event)) 
{
	SetReadOnly(!GetReadOnly());
}

void luEdit::OnWrapmodeOn(wxCommandEvent &WXUNUSED(event)) 
{
	SetWrapMode(GetWrapMode() == 0? wxSTC_WRAP_WORD: wxSTC_WRAP_NONE);
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
	SetCodePage(charset);
}


void luEdit::OnChangeCase(wxCommandEvent &event) 
{
	switch (event.GetId()) 
	{
	case ID_CODE_CHANGELOWER: 
		CmdKeyExecute(wxSTC_CMD_LOWERCASE);
		break;

	case ID_CODE_CHANGEUPPER: 
		CmdKeyExecute(wxSTC_CMD_UPPERCASE);
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
	ConvertEOLs(eolMode);
	SetEOLMode(eolMode);
}

void luEdit::OnMarginClick(wxStyledTextEvent &event) 
{
	if (event.GetMargin() == 2) 
	{
		int lineClick = LineFromPosition(event.GetPosition());
		int levelClick = GetFoldLevel(lineClick);

		if ((levelClick & wxSTC_FOLDLEVELHEADERFLAG) > 0) 
			ToggleFold(lineClick);
	}
}

void luEdit::OnCharAdded(wxStyledTextEvent &event) 
{
	char chr = (char)event.GetKey();

#ifndef __WXMAC__
	if (chr == '\n') 
#else
	if (chr == '\r')
#endif
	{
		int currentLine = GetCurrentLine();

		int lineInd = 0;

		if (currentLine > 0) 
			lineInd = GetLineIndentation(currentLine - 1);

		if (lineInd == 0) 
			return;

		SetLineIndentation(currentLine, lineInd);
		GotoPos(PositionFromLine(currentLine) + lineInd/GetTabWidth());
	}
}


