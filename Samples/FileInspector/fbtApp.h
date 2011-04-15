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
#ifndef _fbtApp_h_
#define _fbtApp_h_

#include <wx/defs.h>

enum FBTEventID
{


	// File
	FBT_QUIT = wxID_HIGHEST + 1,
	FBT_NEW,
	FBT_NEW_FILE,
	FBT_OPEN,
	FBT_OPEN_FBT,
	FBT_OPEN_FILE,
	FBT_SAVE,
	FBT_SAVE_AS,


	// view
	FBT_WINDOW_PRJ_VIEW,
	FBT_WINDOW_FBT_VIEW,
	FBT_WINDOW_OUT_VIEW,


	// Tools
	FBT_TOOL_GENERATE_TYPE_CAST_REPORT,
	FBT_TOOL_GENERATE_CPP_HEADER,

	// Would be cool...
	//FBT_TOOL_GENERATE_XML_PARSER,
	//FBT_TOOL_GENERATE_ASCII_PARSER,
	//FBT_TOOL_GENERATE_XML_SCHEMA,
	//FBT_TOOL_GENERATE_ASCII_TABLES,

	// Build
	FBT_BUILD,

	// Window
	FBT_CLOSE_ALL,
	FBT_EDIT_LAYOUT,
	FBT_RESET_LAYOUT,

	// Help
	FBT_ABOUT,

	// Project Menu
	FBT_NEW_NOTE,
	FBT_EXPORT,
	FBT_EXPORT_ALL,
	FBT_RENAME,
	FBT_DELETE_FILE,


	// Edit Menu
	FBT_CLEAR_LOG,

	// Windows
	FBT_WINDOW_WRK,
	FBT_WINDOW_PRJ,
	FBT_WINDOW_OUT,
	FBT_WINDOW_FBT,

	// Work Windows
	FBT_WINDOW_TXT,
	FBT_WINDOW_VIZ,
	FBT_WINDOW_CHK,

	// Insp Popup Menu
	FBT_CHUNK_FIND,
	FBT_CHUNK_SHOW_ONLY,
	FBT_CHUNK_HIDE,
	FBT_CHUNK_UNHIDE_ALL,

	FBT_MEM_TABLE_FIND,
	FBT_MEM_TABLE_DUMP,

	FBT_FILE_TABLE_FIND,
	FBT_FILE_TABLE_DUMP
};

#define fbtNoteStyle wxAUI_NB_TOP            | wxAUI_NB_TAB_MOVE          | \
                     wxAUI_NB_SCROLL_BUTTONS | wxAUI_NB_WINDOWLIST_BUTTON | \
                     wxAUI_NB_TAB_SPLIT      | \
                     wxAUI_NB_CLOSE_BUTTON 

#define fbtNoteStyleProjB wxAUI_NB_BOTTOM         | wxAUI_NB_TAB_MOVE          | \
                          wxAUI_NB_SCROLL_BUTTONS
#define fbtNoteStyleProjT wxAUI_NB_TOP            | wxAUI_NB_TAB_MOVE          | \
                          wxAUI_NB_SCROLL_BUTTONS


#define fbtDefaultTitle			wxString::Format("File Inspector - v%i.%i.%i", INSP_VERSION_MAJOR, INSP_VERSION_MINOR, INSP_VERSION_SUB)
#define fbtDataViewText         wxDATAVIEW_CELL_INERT,  128, wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE
#define fbtDataViewSmall        wxDATAVIEW_CELL_INERT, -1, wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE | wxCOL_SORTABLE | wxDATAVIEW_COL_REORDERABLE


#ifdef __WXMSW__
# define fbtTreeStyle wxTR_EDIT_LABELS | wxTR_ROW_LINES | wxTR_DEFAULT_STYLE
#else
# define fbtTreeStyle wxTR_EDIT_LABELS | wxTR_DEFAULT_STYLE
#endif


#define FBT_WRK_LAYER 0
#define FBT_OUT_LAYER 1
#define FBT_PRJ_LAYER 2
#define FBT_ISP_LAYER 2


#endif//_fbtApp_h_
