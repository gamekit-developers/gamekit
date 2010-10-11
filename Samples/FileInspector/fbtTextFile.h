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
#ifndef _fbtTextFile_h_
#define _fbtTextFile_h_


#include <wx/stc/stc.h>
#include "Serialize/fbtDataApi.h"
class fbtMainFrame;


class fbtTextFile : public wxStyledTextCtrl
{

public:
	fbtTextFile(fbtMainFrame* parent, fbtText* fp, int id = wxID_ANY);
	~fbtTextFile();


	fbtText* getFile(void) {return m_file;}


private:

	void undoEvent(wxCommandEvent& evt);
	void redoEvent(wxCommandEvent& evt);
	void cutEvent(wxCommandEvent& evt);
	void copyEvent(wxCommandEvent& evt);
	void pasteEvent(wxCommandEvent& evt);
	void selectAllEvent(wxCommandEvent& evt);
	void deleteEvent(wxCommandEvent& evt);
	void savePointLeftEvent(wxStyledTextEvent& evt);
	void savePointReachedEvent(wxStyledTextEvent& evt);
	void zoomEvent(wxStyledTextEvent& evt);


	fbtMainFrame* m_parent;
	fbtText* m_file;

	DECLARE_EVENT_TABLE();

};


#endif//_fbtTextFile_h_
