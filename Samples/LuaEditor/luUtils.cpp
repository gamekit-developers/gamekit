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
#include "luUtils.h"

void addControlToPanel(wxPanel *panel, wxWindow *control, const wxString& title, int titleHeight)
{
	if (!panel || !control) return;

	wxSizer* size = new wxBoxSizer(wxVERTICAL);
	
	wxPanel* titlePanel = new wxPanel(panel);
	titlePanel->SetSize(0, titleHeight);

	size->Add(titlePanel, wxSizerFlags(0).Expand().Border(wxALL, 0));
	size->Add(control, wxSizerFlags(1).Expand().Border(wxALL, 0));

	panel->SetSizer(size);
	panel->Layout();
}


void alertDlg(const wxString& msg, wxWindow* parent, const wxString& title)
{
	wxMessageDialog dlg(parent, msg, title);
	dlg.ShowModal();
}

int selectPage(wxAuiNotebook* noteBook, wxWindow* page)
{
	if (!noteBook || !page) return -1;

	for (size_t i = 0; i < noteBook->GetPageCount(); i++)
	{
		if (page == noteBook->GetPage(i))
		{
			noteBook->SetSelection(i);
			return (int)i;
		}
	}

	return -1;
}

void setupDefaultAuiDockArt(wxAuiManager* aui)
{
	wxASSERT(aui);

	wxAuiDockArt* art = aui->GetArtProvider();

	art->SetMetric(wxAUI_DOCKART_PANE_BORDER_SIZE,  0);
	art->SetMetric(wxAUI_DOCKART_SASH_SIZE,         3);
	art->SetMetric(wxAUI_DOCKART_CAPTION_SIZE,      18);
}
