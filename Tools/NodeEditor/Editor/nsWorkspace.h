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
#ifndef _nsWorkSpace_h_
#define _nsWorkSpace_h_

#include "nsCommon.h"
#include "nsSingleton.h"
#include "nsEventHandler.h"
#include <wx/panel.h>
#include <wx/aui/aui.h>

class wxAuiNotebook;
class wxGLContext;

class nsWorkspace :     public wxPanel,
	public nsNotifierBase,
	public nsSingleton<nsWorkspace>

{
protected:

	nsGlHiddenContext*       m_hidenContext;
	wxAuiNotebook*           m_book;
	nsNodeCanvas*            m_activeCanvas;
	nsClipboard*             m_clipboard;

	void pageChangedEvent(wxAuiNotebookEvent& evt);
	void pageClosedEvent(wxAuiNotebookEvent& evt);


public:
	nsWorkspace(wxWindow* parent);
	virtual ~nsWorkspace();



	void            finalizeContext(void);
	void            initializeContext(void);
	wxGLContext*     getGLContext(void);

	// shared access to the clipboard
	nsClipboard&    getClipboard(void) {UT_ASSERT(m_clipboard); return *m_clipboard; }

	// access to the current canvas page.
	nsNodeCanvas*    getActiveCanvas(void) {return m_activeCanvas;}

	// notifiers
	void treeEvent(nsTreeEvent& evt);

	// copied down from the main window
	void grabCapturedEvent(wxCommandEvent& evt);
	void deleteCapturedEvent(wxCommandEvent& evt);
	void selectAllEvent(wxCommandEvent& evt);
	void copyEvent(wxCommandEvent& evt);
	void pasteEvent(wxCommandEvent& evt);
	void cutEvent(wxCommandEvent& evt);
	void duplicateEvent(wxCommandEvent& evt);

	NS_DECLARE_SINGLETON(nsWorkspace);

	// static tables
	DECLARE_EVENT_TABLE();

};

#endif//_nsSolutionBrowser_h_
