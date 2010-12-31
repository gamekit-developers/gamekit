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


#ifndef _LauUtils_h_
#define _LauUtils_h_

#include "luDefs.h"

class luMainApp;
class luMainFrame;

void addControlToPanel(wxPanel *panel, wxWindow *control, const wxString& title, int titleHeight = PANEL_TITLE_HEIGHT);

GK_INLINE luMainApp* getLuApp()
{
	return (luMainApp*)wxTheApp;
}

GK_INLINE luMainFrame* getLuMainFrame()
{
	GK_ASSERT(wxTheApp->GetTopWindow());

	return (luMainFrame*)wxTheApp->GetTopWindow();
}

void alertDlg(const wxString& msg, wxWindow* parent = NULL, const wxString& title = "Alert");

int selectPage(wxAuiNotebook* noteBook, wxWindow* page);

void setupDefaultAuiDockArt(wxAuiManager* aui);

#endif //_LauUtils_h_