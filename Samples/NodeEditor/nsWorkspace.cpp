/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

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
#include "nsWorkspace.h"
#include "nsSolutionBrowser.h"
#include "nsPropertyPage.h"
#include "nsNodeCanvas.h"
#include "nsRenderSystem.h"
#include "nsNodeManager.h"
#include "nsClipboard.h"
#include <wx/aui/aui.h>
#include <wx/sizer.h>
#include <wx/treectrl.h>
#include <wx/dcclient.h>
#include <wx/settings.h>
#include <wx/glcanvas.h>



NS_IMPLEMENT_SINGLETON(nsWorkspace);

#define nsWorkspaceStyle    wxAUI_NB_TOP                | wxAUI_NB_TAB_MOVE     |\
                            wxAUI_NB_SCROLL_BUTTONS     | wxAUI_NB_CLOSE_BUTTON |\
                            wxAUI_NB_WINDOWLIST_BUTTON  | wxTAB_TRAVERSAL       |\
                            wxAUI_NB_TAB_SPLIT          | nsBorderNone




// Private Context window in order to share GL context across multiple windows
class nsGlHiddenContext : public wxGLCanvas
{
protected:

	wxGLContext* m_context;

public:
	nsGlHiddenContext(wxWindow* parent) : wxGLCanvas(parent, -1, NULL)
	{
		m_context = new wxGLContext(this);
	}

	virtual ~nsGlHiddenContext()
	{
		// free up
		delete m_context;
	}

	wxGLContext* getContext(void) {return m_context;}

};



// Event tables
BEGIN_EVENT_TABLE( nsWorkspace, wxPanel )
	EVT_AUINOTEBOOK_PAGE_CLOSE(NS_WID_WORKSPACE_DATA,       nsWorkspace::pageClosedEvent)
	EVT_AUINOTEBOOK_PAGE_CHANGED(NS_WID_WORKSPACE_DATA,     nsWorkspace::pageChangedEvent)

END_EVENT_TABLE()



nsWorkspace::nsWorkspace(wxWindow* parent)
	:   wxPanel(parent, NS_WID_WORKSPACE, wxDefaultPosition, wxSize(200, 200), wxTAB_TRAVERSAL | nsBorderDefault)

{
	m_activeCanvas = 0;
	// setup hidden context
	m_hidenContext = new nsGlHiddenContext(this);
	m_hidenContext->Hide();


	// the clipboard is shared across all trees
	// copy in one window & paste in another.
	m_clipboard = new nsClipboard();


	// setup main content
	wxSizer* size = new wxBoxSizer(wxVERTICAL);
	m_book = new wxAuiNotebook(this, NS_WID_WORKSPACE_DATA, wxDefaultPosition, wxDefaultSize, nsWorkspaceStyle);

	size->Add(m_book, wxSizerFlags(1).Expand().Border(wxALL, 0));
	SetSizer(size);
	Layout();
}



nsWorkspace::~nsWorkspace()
{
}



void nsWorkspace::pageChangedEvent(wxAuiNotebookEvent& evt)
{
	if (m_book)
	{
		nsNodeCanvas* canvas = (nsNodeCanvas*)m_book->GetPage(evt.GetSelection());

		if (m_activeCanvas && m_activeCanvas != canvas)
		{
			// notify old
			nsTreeEvent deselect(NS_TREE_DESELECT, m_activeCanvas, m_activeCanvas->getTree());
			nsSolutionBrowser::getSingleton().treeEvent(deselect);
			nsPropertyPage::getSingleton().treeEvent(deselect);

		}

		m_activeCanvas = canvas;

		// notify pages
		nsTreeEvent select(NS_TREE_SELECT, m_activeCanvas, m_activeCanvas->getTree());
		nsSolutionBrowser::getSingleton().treeEvent(select);
		nsPropertyPage::getSingleton().treeEvent(select);

		evt.Skip();
	}
}


void nsWorkspace::pageClosedEvent(wxAuiNotebookEvent& evt)
{
	// only notify tree deslect here
	// trees have persistant state

	nsNodeCanvas* canvas = (nsNodeCanvas*)m_book->GetPage(evt.GetSelection());
	if (canvas)
	{
		// notify old
		nsTreeEvent deselect(NS_TREE_DESELECT, canvas, canvas->getTree());
		nsPropertyPage::getSingleton().treeEvent(deselect);
		canvas->getTree()->setOpen(false);
		m_activeCanvas = 0;
	}
}




void nsWorkspace::treeEvent(nsTreeEvent& evt)
{
	if (evt.getId() == NS_TREE_CHANGED)
	{
		if (m_book)
		{
			nsNodeTree* tree = evt.ptr();
			int nr = (int)m_book->GetPageCount(), foundPage = -1, i;

			for (i = 0; i < nr; ++i)
			{
				nsNodeCanvas* canvas = (nsNodeCanvas*)m_book->GetPage(i);
				if (canvas->getTree() == tree)
				{
					m_book->SetPageText(i, tree->getName().c_str());
					break;
				}
			}
		}
	}
	else if (evt.getId() == NS_TREE_ADD)
	{
		if (m_book)
		{
			nsNodeTree* tree = evt.ptr();
			int nr = (int)m_book->GetPageCount(), foundPage = -1, i;

			for (i = 0; i < nr; ++i)
			{
				nsNodeCanvas* canvas = (nsNodeCanvas*)m_book->GetPage(i);

				if (canvas->getTree() == tree)
				{
					foundPage = i;
					break;
				}
			}

			if (foundPage == -1)
			{
				// create & activate it

				nsNodeCanvas* canvas = new nsNodeCanvas(this, tree);
				m_book->AddPage(canvas, tree->getName(), true);
				tree->setOpen(true);
			}
			else
			{
				// select & activate it
				m_book->SetSelection(foundPage);
			}
		}
	}

	else if (evt.getId() == NS_TREE_REMOVE)
	{
		if (m_book)
		{
			nsNodeTree* tree = evt.ptr();
			int nr = (int)m_book->GetPageCount(), foundPage = -1, i;

			for (i = 0; i < nr; ++i)
			{
				nsNodeCanvas* canvas = (nsNodeCanvas*)m_book->GetPage(i);

				if (canvas->getTree() == tree)
				{
					foundPage = i;
					break;
				}
			}

			if (foundPage != -1)
			{
				tree->setOpen(false);
				m_book->DeletePage(foundPage);
			}
		}
	}

}


void nsWorkspace::grabCapturedEvent(wxCommandEvent& evt)
{
	// pass to active canvas
	if (m_activeCanvas)
		m_activeCanvas->grabCapturedEvent(evt);
	else evt.Skip();
}



void nsWorkspace::deleteCapturedEvent(wxCommandEvent& evt)
{
	// pass to active canvas
	if (m_activeCanvas)
		m_activeCanvas->deleteCapturedEvent(evt);
}


void nsWorkspace::selectAllEvent(wxCommandEvent& evt)
{
	// pass to active canvas
	if (m_activeCanvas)
		m_activeCanvas->selectAllEvent(evt);
}


void nsWorkspace::cutEvent(wxCommandEvent& evt)
{
	// pass to active canvas
	if (m_activeCanvas)
		m_activeCanvas->cutEvent(evt);
}


void nsWorkspace::copyEvent(wxCommandEvent& evt)
{
	// pass to active canvas
	if (m_activeCanvas)
		m_activeCanvas->copyEvent(evt);
}


void nsWorkspace::pasteEvent(wxCommandEvent& evt)
{
	// pass to active canvas
	if (m_activeCanvas)
		m_activeCanvas->pasteEvent(evt);
}


void nsWorkspace::duplicateEvent(wxCommandEvent& evt)
{
	// pass to active canvas
	if (m_activeCanvas)
		m_activeCanvas->duplicateEvent(evt);
}


// remove shared context
void nsWorkspace::finalizeContext(void)
{
	if (IsShown() && m_hidenContext)
	{
		m_book->Freeze();

		wxGLContext* ctx = m_hidenContext->getContext();
		m_hidenContext->Show();
		m_hidenContext->SetCurrent(*ctx);
		nsRenderSystem::getSingleton().finalizeContextData();
		m_hidenContext->Hide();
		m_hidenContext->Destroy();
		m_hidenContext = 0;

		// close active
		m_activeCanvas = 0;

		// remove the book
		m_book->Destroy();
		m_book = 0;

		delete m_clipboard;
		m_clipboard = 0;
	}
}



// setup shared context
void nsWorkspace::initializeContext(void)
{
	if (IsShown())
	{
		wxGLContext* ctx = m_hidenContext->getContext();
		m_hidenContext->Show();

		bool status = m_hidenContext->SetCurrent(*ctx);
		if (status)
			nsRenderSystem::getSingleton().initializeContextData();
		m_hidenContext->Hide();
	}
}



wxGLContext* nsWorkspace::getGLContext(void)
{
	// access to the main context
	return m_hidenContext->getContext();
}
