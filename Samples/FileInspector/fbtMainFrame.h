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
#ifndef _fbtMainFrame_h_
#define _fbtMainFrame_h_


#include <wx/frame.h>
#include <wx/treectrl.h>
#include "fbtTypes.h"
#include "fbtInspectorFile.h"


class fbtText;
class wxTextCtrl;
class wxTreeCtrl;
class wxGrid;
class wxAuiNotebook;
class wxAuiNotebookEvent;
class wxAuiManager;
class wxDataViewListCtrl;
class wxStyledTextCtrl;
class wxChoicebook;
class fbtProjectTree;
class fbtTextFile;
class fbtChunkCtrl;
class fbtStruct;

class fbtMainFrame : public wxFrame
{
public:
	fbtMainFrame();
	virtual ~fbtMainFrame();

	void openProject(const wxString& proj);
	void openTextFile(const wxString& path, fbtText* fp);

	void notifyFileModified(fbtTextFile* tf);

private:

	bool saveModified(void);

	void quitEvent(wxCommandEvent& evt);
	void saveFileEvent(wxCommandEvent& evt);
	void saveEvent(wxCommandEvent& evt);
	void saveAsEvent(wxCommandEvent& evt);
	void openEvent(wxCommandEvent& evt);
	void openFileEvent(wxCommandEvent& evt);
	void newEvent(wxCommandEvent& evt);
	void newFileEvent(wxCommandEvent& evt);
	void openFBTEvent(wxCommandEvent& evt);

	void fbtViewEvent(wxCommandEvent& evt);
	void projViewEvent(wxCommandEvent& evt);
	void outViewEvent(wxCommandEvent& evt);

	void buildEvent(wxCommandEvent& evt);
	void gtcEvent(wxCommandEvent& evt);

	void itemActivatedEvent(wxTreeEvent& evt);
	void editLabelBegin(wxTreeEvent& evt);
	void editLabelEnd(wxTreeEvent& evt);
	void openProjectMenu(wxTreeEvent& evt);
	void closeAllEvent(wxCommandEvent& evt);
	void resetLayoutEvent(wxCommandEvent& evt);
	void deleteSelectedEvent(wxCommandEvent& evt);
	void renameSelectedEvent(wxCommandEvent& evt);
	void editTextEvent(wxCommandEvent& evt);
	void exportTextEvent(wxCommandEvent& evt);
	void exportAllTextEvent(wxCommandEvent& evt);
	void newNote(wxCommandEvent& evt);
	void genCppHeader(wxCommandEvent& evt);
	void chunkActivated(wxTreeEvent& evt);

	void closeAllPages(void);

	void openNewFile(const wxString& name, fbtText* fp);

	// project tree settings
	void addProjectFile(fbtText* fp, bool open = false, bool store = true);
	void addProjectFiles(const wxArrayString& files);
	void setProjectText(const wxString& text);
	void createNewProjectTree(const wxString& name = wxEmptyString);



	void populateChunks(fbtList& chunks);
	void populate(fbtBinTables* table, int index, fbtBinTables* linkTable=0);
	void unpopulate(void);
    
	void chunkItemMenu(wxTreeEvent& evt);
	void chunkFind(wxCommandEvent& evt);
	void chunkShowOnly(wxCommandEvent& evt);
	void chunkHide(wxCommandEvent& evt);
	void chunkUnhideAll(wxCommandEvent& evt);
	void chunkFindOrDelete(const wxString& name, bool matchDelete, bool findOnly = false);

	void findMemTable(wxCommandEvent& evt);
	void findFileTable(wxCommandEvent& evt);
	void findTableItem(const wxString& text, int ictrl, int sel);

	void dumpMemTable(wxCommandEvent& evt);
	void dumpFileTable(wxCommandEvent& evt);
	void dumpTableItem(int ictrl, int sel);
	void dumpTableStruct(fbtStruct* strc, int depth, fbtBinTables* table, fbtBinTables* linkTable);

	void clearLog(wxCommandEvent& evt);

	void log(const wxString& msg);
	void clearLog();
	void alert(const wxString& msg);

	void saveProject(const wxString& path);
	void buildProject(void);
	void clearProject(void);


	void destroyProject(void);
	void createNewLayout(void);


	void loadDataView(wxChoicebook* parent, int index);


	void    loadMainFile(const wxString& path);
	void    loadWindows(void);
	void    loadMenus(void);

	wxAuiManager*           m_auiManager;
	wxAuiNotebook*          m_note;
	wxAuiNotebook*          m_projBook;
	wxAuiNotebook*          m_output;
	wxAuiNotebook*          m_fbtBook;


	wxTextCtrl*             m_logWindow;
	wxTreeCtrl*             m_chunkExplorer;
	wxTreeCtrl*             m_projectExplorer;
	wxString                m_startupPerspective;
	wxString                m_projPath;

	fbtInspectorFile*       m_curProject;
	fbtBinTables*           m_compiledTables;
	int                     m_unnamed;
	wxTreeItemId            m_fbtId, m_srcId, m_nteId;
	wxTreeItemId            m_client;


	wxChoicebook*           m_mpBook;
	wxChoicebook*           m_fpBook;
	wxChoicebook*           m_cpBook;

	wxDataViewListCtrl*     m_name[3];
	wxDataViewListCtrl*     m_type[3];
	wxDataViewListCtrl*     m_strc[3];

	wxTextCtrl*             m_mpTextCtrl;
	wxTextCtrl*             m_fpTextCtrl;

	fbtFile* m_file;
	fbtDebugger::Reporter m_hook;
	static void report(wxTextCtrl* ctrl, const char* message);

	DECLARE_EVENT_TABLE();
};



#endif//_fbtMainFrame_h_
