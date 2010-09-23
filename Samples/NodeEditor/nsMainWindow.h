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
#ifndef _nsMainWindow_h_
#define _nsMainWindow_h_

#include <wx/frame.h>
#include <wx/config.h>

#include "nsCommon.h"
#include "nsSingleton.h"
class wxAuiManagerEvent;
class nsOgreKitPreview;

class nsMainWindow :    public wxFrame,
	public nsSingleton<nsMainWindow>
{
protected:
	wxAuiManager*        m_auiManager;

	// menu entries
	wxMenuItem*          m_viewSolution;
	wxMenuItem*          m_viewProperties;
	wxWindow*            m_addMenuCaller;
	wxString            m_currentFile;


private:

	void    loadSettings(void);
	void    saveSettings(void);
	void    loadWindows(void);
	void    loadMenus(void);



	// events
	void quitEvent(wxCommandEvent& evt);
	void saveEvent(wxCommandEvent& evt);
	void saveAsEvent(wxCommandEvent& evt);
	void loadEvent(wxCommandEvent& evt);


	// show / hide panels
	void solutionCheckEvent(wxCommandEvent& evt);
	void propertiesCheckEvent(wxCommandEvent& evt);
	void paneCloseEvent(wxAuiManagerEvent& evt);
	void showFullscreenEvent(wxCommandEvent& evt);


	// node creation
	void nodeAddEvent(wxCommandEvent& evt);
	// tree creation
	void treeAddEvent(wxCommandEvent& evt);


	// global node space
	void deleteCapturedEvent(wxCommandEvent& evt);
	void selectAllEvent(wxCommandEvent& evt);


	// copy, paste & cut
	void copyEvent(wxCommandEvent& evt);
	void pasteEvent(wxCommandEvent& evt);
	void cutEvent(wxCommandEvent& evt);
	void duplicateEvent(wxCommandEvent& evt);


public:
	nsMainWindow();
	virtual ~nsMainWindow();


	// the base add toolbox
	void    makeNodeMenu(wxWindow* caller, wxMenu* menu);

	// the base edit toolbox
	void    makeEditMenu(wxMenu* menu);

	// status text nsilities
	void    setStatus(int id, const char* fmt, ...);

	// singleton access
	NS_DECLARE_SINGLETON(nsMainWindow);

	// wx tables
	DECLARE_EVENT_TABLE();
};


#endif//_nsMainWindow_h_
