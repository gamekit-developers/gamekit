/*
-------------------------------------------------------------------------------
    This file is part of FBT (File Binary Tables).
    http://gamekit.googlecode.com/

    Copyright (c) 2010 Charlie C & Erwin Coumans.

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
#include "fbtApp.h"
#include "fbtTextFile.h"
#include "fbtMainFrame.h"

#define COMMENT_COLOR       wxColor(0, 128, 0)
#define NUMBER_COLOR        wxColor(0, 0, 0)
#define OPERATOR_COLOR      wxColor(0, 0, 0)
#define WORD_COLOR          wxColor(0, 0, 255)


const char* WORDS =
    "bool break case catch char class const  "
    "continue default delete do double else enum explicit "
    "export extern false float for friend goto if inline int long "
    "mutable namespace new operator private protected public register "
    " return short signed sizeof static "
    "struct switch template this throw true try typedef typeid "
    "typename union unsigned using virtual void volatile "
    "while privsec";



BEGIN_EVENT_TABLE( fbtTextFile, wxStyledTextCtrl )

	EVT_MENU(wxID_UNDO,         fbtTextFile::undoEvent)
	EVT_MENU(wxID_REDO,         fbtTextFile::redoEvent)
	EVT_MENU(wxID_CUT,          fbtTextFile::cutEvent)
	EVT_MENU(wxID_COPY,         fbtTextFile::copyEvent)
	EVT_MENU(wxID_PASTE,        fbtTextFile::pasteEvent)
	EVT_MENU(wxID_CLEAR,        fbtTextFile::deleteEvent)
	EVT_MENU(wxID_SELECTALL,    fbtTextFile::selectAllEvent)


	EVT_STC_SAVEPOINTLEFT(FBT_WINDOW_TXT,       fbtTextFile::savePointLeftEvent)
	EVT_STC_SAVEPOINTREACHED(FBT_WINDOW_TXT,    fbtTextFile::savePointReachedEvent)
	EVT_STC_ZOOM(FBT_WINDOW_TXT,                fbtTextFile::zoomEvent)

END_EVENT_TABLE()



fbtTextFile::fbtTextFile(fbtMainFrame* parent, fbtText* fp, int id)
	:   wxStyledTextCtrl(parent, FBT_WINDOW_TXT),
	    m_file(fp), m_parent(parent)
{

	SetViewEOL(false);
	SetIndentationGuides(false);
	SetEdgeMode(wxSTC_EDGE_NONE);
	SetViewWhiteSpace(wxSTC_WS_INVISIBLE);

	SetReadOnly(false);
	SetTabWidth(4);
	SetWrapMode(wxSTC_WRAP_NONE);

	wxFont font (10, wxMODERN, wxNORMAL, wxNORMAL);
	StyleSetFont(wxSTC_STYLE_DEFAULT, font);


	StyleSetForeground(wxSTC_STYLE_DEFAULT,     *wxBLACK);
	StyleSetBackground(wxSTC_STYLE_DEFAULT,     *wxWHITE);
	StyleSetForeground(wxSTC_STYLE_LINENUMBER,  wxColour (wxT("DARK GREY")));
	StyleSetBackground(wxSTC_STYLE_LINENUMBER,  *wxWHITE);
	StyleSetForeground(wxSTC_STYLE_INDENTGUIDE, wxColour (wxT("DARK GREY")));


	SetMarginType(0, wxSTC_MARGIN_NUMBER);
	SetMarginWidth(0, TextWidth (wxSTC_STYLE_LINENUMBER, wxT("_99999")));

	SetVisiblePolicy(wxSTC_VISIBLE_STRICT | wxSTC_VISIBLE_SLOP, 1);
	SetXCaretPolicy(wxSTC_CARET_EVEN | wxSTC_VISIBLE_STRICT | wxSTC_CARET_SLOP, 1);
	SetYCaretPolicy(wxSTC_CARET_EVEN | wxSTC_VISIBLE_STRICT | wxSTC_CARET_SLOP, 1);


	SetLexer(wxSTC_LEX_CPP);

	StyleSetForeground(wxSTC_C_COMMENT,         COMMENT_COLOR);
	StyleSetForeground(wxSTC_C_COMMENTLINEDOC,  COMMENT_COLOR);
	StyleSetForeground(wxSTC_C_COMMENTLINE,     COMMENT_COLOR);
	StyleSetForeground(wxSTC_C_COMMENTDOC,      COMMENT_COLOR);
	StyleSetForeground(wxSTC_C_NUMBER,          NUMBER_COLOR);
	StyleSetForeground(wxSTC_C_OPERATOR,        OPERATOR_COLOR);
	StyleSetForeground(wxSTC_C_WORD,            WORD_COLOR);
	StyleSetForeground(wxSTC_C_PREPROCESSOR,    WORD_COLOR);

	SetKeyWords(0, WORDS);

	SetSelBackground(true, wxColor(51, 94, 168));
	SetSelForeground(true, wxColor(255, 255, 255));

	m_file->m_textFile = this;
	m_file->m_flag |= fbtText::FILE_IS_OPEN;
}




fbtTextFile::~fbtTextFile()
{
	if (m_file)
	{
		m_file->m_flag &= ~(fbtText::FILE_IS_OPEN);
		m_file->m_textFile = 0;
	}
}


void fbtTextFile::undoEvent(wxCommandEvent& evt)
{
	if (!CanUndo())
		return;
	Undo();
}


void fbtTextFile::redoEvent(wxCommandEvent& evt)
{
	if (!CanRedo())
		return;
	Redo();
}

void fbtTextFile::cutEvent(wxCommandEvent& evt)
{
	if (GetReadOnly()  || GetSelectionEnd() - GetSelectionStart() <= 0)
		return;
	Cut();

}

void fbtTextFile::copyEvent(wxCommandEvent& evt)
{
	if (GetSelectionEnd() - GetSelectionStart() <= 0)
		return;
	Copy();
}

void fbtTextFile::pasteEvent(wxCommandEvent& evt)
{
	if (!CanPaste())
		return;
	Paste();
}


void fbtTextFile::deleteEvent(wxCommandEvent& evt)
{
	if (GetReadOnly())
		return;
	Clear();

}
void fbtTextFile::selectAllEvent(wxCommandEvent& evt)
{
	SetSelection(0, GetTextLength());
}


void fbtTextFile::savePointLeftEvent(wxStyledTextEvent& evt)
{
	if (m_file)
	{
		if (!m_file->isModified())
		{
			m_file->modify(true);
			m_parent->notifyFileModified(this);
		}
	}
}


void fbtTextFile::savePointReachedEvent(wxStyledTextEvent& evt)
{
	if (m_file)
	{

		if (m_file->isModified())
		{
			m_file->modify(false);
			m_parent->notifyFileModified(this);
		}
	}
}

void fbtTextFile::zoomEvent(wxStyledTextEvent& evt)
{
	evt.Skip();

	// dsiable zoom
	if (GetZoom() != 0) SetZoom(0);
}
