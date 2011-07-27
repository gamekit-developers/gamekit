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
#include "fbtMainFrame.h"
#include "fbtInspectorFile.h"
#include "fbtStreams.h"
#include "fbtTextFile.h"
#include "fbtApp.h"
#include "fbtTables.h"
#include "fbtBuilder.h"
#include "fbtBlend.h"
#include "fbtBullet.h"
#include "fbtIcons.h"
#include "fbtDataView.h"

#include <wx/aui/aui.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/filedlg.h>
#include <wx/dirdlg.h>
#include <wx/textctrl.h>
#include <wx/treectrl.h>
#include <wx/choicebk.h>
#include <wx/dataview.h>
#include <wx/filename.h>
#include <wx/stc/stc.h>
#include <wx/textdlg.h>
#include <wx/toolbar.h> 
#include <wx/button.h>
#include <wx/msgdlg.h>
#include <wx/artprov.h>

#include "Resource/system-search.xpm"


BEGIN_EVENT_TABLE( fbtMainFrame, wxFrame )


	EVT_MENU(FBT_NEW,       fbtMainFrame::newEvent)
	EVT_MENU(FBT_NEW_FILE,  fbtMainFrame::newFileEvent)
	EVT_MENU(FBT_OPEN,      fbtMainFrame::openEvent)
	EVT_MENU(FBT_OPEN_FBT,  fbtMainFrame::openFBTEvent)
	EVT_MENU(FBT_OPEN_FILE, fbtMainFrame::openFileEvent)
	EVT_MENU(FBT_SAVE,      fbtMainFrame::saveEvent)
	EVT_MENU(FBT_SAVE_AS,   fbtMainFrame::saveAsEvent)
	EVT_MENU(FBT_QUIT,      fbtMainFrame::quitEvent)

	EVT_MENU(wxID_UNDO,         fbtMainFrame::editTextEvent)
	EVT_MENU(wxID_REDO,         fbtMainFrame::editTextEvent)
	EVT_MENU(wxID_CUT,          fbtMainFrame::editTextEvent)
	EVT_MENU(wxID_COPY,         fbtMainFrame::editTextEvent)
	EVT_MENU(wxID_PASTE,        fbtMainFrame::editTextEvent)
	EVT_MENU(wxID_CLEAR,        fbtMainFrame::editTextEvent)
	EVT_MENU(wxID_SELECTALL,    fbtMainFrame::editTextEvent)
	EVT_MENU(FBT_CLEAR_LOG,		fbtMainFrame::clearLog)


	EVT_MENU(FBT_WINDOW_PRJ_VIEW,   fbtMainFrame::projViewEvent)
	EVT_MENU(FBT_WINDOW_OUT_VIEW,   fbtMainFrame::outViewEvent)
	EVT_MENU(FBT_WINDOW_FBT_VIEW,   fbtMainFrame::fbtViewEvent)


	EVT_MENU(FBT_BUILD,  fbtMainFrame::buildEvent)

	EVT_MENU(FBT_TOOL_GENERATE_TYPE_CAST_REPORT, fbtMainFrame::gtcEvent)
	EVT_MENU(FBT_TOOL_GENERATE_CPP_HEADER, fbtMainFrame::genCppHeader)




	EVT_MENU(FBT_CLOSE_ALL,     fbtMainFrame::closeAllEvent)
	EVT_MENU(FBT_RESET_LAYOUT,  fbtMainFrame::resetLayoutEvent)
	EVT_MENU(FBT_EDIT_LAYOUT,   fbtMainFrame::resetLayoutEvent)


	EVT_TREE_ITEM_ACTIVATED(FBT_WINDOW_PRJ,     fbtMainFrame::itemActivatedEvent)
	EVT_TREE_BEGIN_LABEL_EDIT(FBT_WINDOW_PRJ,   fbtMainFrame::editLabelBegin)
	EVT_TREE_END_LABEL_EDIT(FBT_WINDOW_PRJ,     fbtMainFrame::editLabelEnd)
	EVT_TREE_ITEM_MENU(FBT_WINDOW_PRJ,          fbtMainFrame::openProjectMenu)
	
	EVT_MENU(FBT_DELETE_FILE,                   fbtMainFrame::deleteSelectedEvent)
	EVT_MENU(FBT_RENAME,                        fbtMainFrame::renameSelectedEvent)
	EVT_MENU(FBT_EXPORT,                        fbtMainFrame::exportTextEvent)
	EVT_MENU(FBT_EXPORT_ALL,                    fbtMainFrame::exportAllTextEvent)
	EVT_MENU(FBT_NEW_NOTE,                      fbtMainFrame::newNote)

	EVT_TREE_ITEM_ACTIVATED(FBT_WINDOW_FBT,     fbtMainFrame::chunkActivated)

	EVT_TREE_ITEM_MENU(FBT_WINDOW_FBT,			fbtMainFrame::chunkItemMenu)
	EVT_MENU(FBT_CHUNK_FIND,					fbtMainFrame::chunkFind)
	EVT_MENU(FBT_CHUNK_SHOW_ONLY,				fbtMainFrame::chunkShowOnly)
	EVT_MENU(FBT_CHUNK_HIDE,					fbtMainFrame::chunkHide)
	EVT_MENU(FBT_CHUNK_UNHIDE_ALL,				fbtMainFrame::chunkUnhideAll)

	EVT_MENU(FBT_MEM_TABLE_FIND,				fbtMainFrame::findMemTable)
	EVT_MENU(FBT_FILE_TABLE_FIND,				fbtMainFrame::findFileTable)
	EVT_MENU(FBT_MEM_TABLE_DUMP,				fbtMainFrame::dumpMemTable)
	EVT_MENU(FBT_FILE_TABLE_DUMP,				fbtMainFrame::dumpFileTable)
END_EVENT_TABLE()




class fbtTextFileData : public wxTreeItemData
{
public:
	fbtTextFileData(fbtText* fp) : m_file(fp){}
	fbtText* m_file;
};



class fbtChunkData : public wxTreeItemData
{
public:
	fbtChunkData(fbtFile::MemoryChunk* mch, int index = 0) : m_chunk(mch), m_index(index) {}
	fbtFile::MemoryChunk* m_chunk;
	int m_index;
};





fbtMainFrame::fbtMainFrame()
	:   wxFrame(NULL, -1, fbtDefaultTitle, wxDefaultPosition, wxSize(800, 600)),
	    m_auiManager(0),
	    m_note(0),
	    m_projBook(0),
	    m_output(0),
	    m_fbtBook(0),
	    m_logWindow(0),
	    m_chunkExplorer(0),
	    m_projectExplorer(0),
	    m_startupPerspective(wxEmptyString),
		m_projPath(wxEmptyString),
	    m_curProject(0),
	    m_compiledTables(0),
	    m_unnamed(0),
	    m_client(0),
	    m_mpBook(0),
	    m_fpBook(0),
	    m_cpBook(0),
	    m_file(0),
		m_mpTextCtrl(0),
		m_fpTextCtrl(0)
{
	m_name[0] = m_name[1] = m_name[2] = 0;
	m_type[0] = m_type[1] = m_type[2] = 0;
	m_strc[0] = m_strc[1] = m_strc[2] = 0;

	SetIcon( system_search_xpm );

	m_auiManager = new wxAuiManager(this, wxAUI_MGR_ALLOW_ACTIVE_PANE | wxAUI_MGR_DEFAULT);
	m_curProject = new fbtInspectorFile();

	wxAuiDockArt* art = m_auiManager->GetArtProvider();
	art->SetMetric(wxAUI_DOCKART_PANE_BORDER_SIZE,  1);
	art->SetMetric(wxAUI_DOCKART_SASH_SIZE,         2);
	art->SetMetric(wxAUI_DOCKART_CAPTION_SIZE,      18);

	art->SetColor(wxAUI_DOCKART_INACTIVE_CAPTION_COLOUR,            art->GetColor(wxAUI_DOCKART_ACTIVE_CAPTION_GRADIENT_COLOUR));
	art->SetColor(wxAUI_DOCKART_INACTIVE_CAPTION_GRADIENT_COLOUR,   art->GetColor(wxAUI_DOCKART_ACTIVE_CAPTION_COLOUR));
	art->SetColor(wxAUI_DOCKART_INACTIVE_CAPTION_TEXT_COLOUR,       *wxWHITE);


	loadWindows();
	loadMenus();


	//Maximize();
}



fbtMainFrame::~fbtMainFrame()
{
	destroyProject();

	m_auiManager->UnInit();
	delete m_auiManager;
}

void fbtMainFrame::loadDataView(wxChoicebook* parent, int i)
{
	m_name[i] = new wxDataViewListCtrl(parent, -1);
	m_name[i]->AppendTextColumn(wxT("Id"), fbtDataViewSmall);
	m_name[i]->AppendTextColumn(wxT("Name"), fbtDataViewText);
	m_name[i]->AppendTextColumn(wxT("Pointer Count"), fbtDataViewSmall);
	m_name[i]->AppendTextColumn(wxT("Array Size"), fbtDataViewSmall);
	m_name[i]->AppendTextColumn(wxT("Name Id"), fbtDataViewSmall);
	m_name[i]->AppendTextColumn(wxT("Base Id"), fbtDataViewSmall);

	parent->AddPage(m_name[i], wxT("Name"));

	//--

	m_type[i] = new wxDataViewListCtrl(parent, -1);
	m_type[i]->AppendTextColumn(wxT("Id"), fbtDataViewSmall);
	m_type[i]->AppendTextColumn(wxT("Name"), fbtDataViewText);
	m_type[i]->AppendTextColumn(wxT("Struct Id"), fbtDataViewSmall);
	m_type[i]->AppendTextColumn(wxT("Sizeof"), fbtDataViewSmall);
	m_type[i]->AppendTextColumn(wxT("Type Id"), fbtDataViewSmall);

	parent->AddPage(m_type[i], wxT("Type"));

	//--

	m_strc[i] = new wxDataViewListCtrl(parent, -1);
	m_strc[i]->AppendTextColumn(wxT("Id"),              fbtDataViewSmall);
	m_strc[i]->AppendTextColumn(wxT("Name"),            fbtDataViewText);
	m_strc[i]->AppendTextColumn(wxT("Member Count"),    fbtDataViewSmall);
	m_strc[i]->AppendTextColumn(wxT("Sizeof"),          fbtDataViewSmall);
	m_strc[i]->AppendTextColumn(wxT("Linked"),			fbtDataViewSmall);
	m_strc[i]->AppendTextColumn(wxT("Unlinked Member"),	fbtDataViewSmall);

	parent->AddPage(m_strc[i], wxT("Structure"));
}

void fbtMainFrame::loadWindows(void)
{
	wxAuiPaneInfo inf;

	m_fbtBook = new wxAuiNotebook(this, -1, wxDefaultPosition, wxSize(250, 200), fbtNoteStyleProjT);
	m_chunkExplorer = new wxTreeCtrl(this, FBT_WINDOW_FBT, wxDefaultPosition, wxSize(200, 200));

	m_chunkExplorer->AssignImageList(fbtMakeInspectorImageList());

	//--
	wxPanel* mpPanel = new wxPanel(this, -1, wxDefaultPosition, wxSize(250, 200));
	wxBoxSizer* mpSizer = new wxBoxSizer(wxVERTICAL);

	wxToolBar* mpToolbar = new wxToolBar(mpPanel, -1 , wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL); 
	//mpToolbar->AddSeparator();
	m_mpTextCtrl = new wxTextCtrl(mpToolbar, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	mpToolbar->AddControl(m_mpTextCtrl);
	mpToolbar->AddTool(FBT_MEM_TABLE_FIND, wxT("Search"),  wxArtProvider::GetBitmap(wxART_FIND, wxART_OTHER, wxSize(16,16)));
	mpToolbar->AddTool(FBT_MEM_TABLE_DUMP, wxT("Dump"),  wxArtProvider::GetBitmap(wxART_INFORMATION, wxART_OTHER, wxSize(16,16)));
	mpToolbar->Realize();

	mpSizer->Add( mpToolbar, 0, wxEXPAND, 0);
	mpSizer->AddSpacer(4);

	m_mpBook = new wxChoicebook(mpPanel, -1, wxDefaultPosition, wxSize(250, 200));
	loadDataView(m_mpBook, 0);
	mpSizer->Add(m_mpBook, 1, wxEXPAND | wxALL, 0);
	mpPanel->SetSizer(mpSizer);
	mpPanel->Layout();

	//--
	wxPanel* fpPanel = new wxPanel(this, -1, wxDefaultPosition, wxSize(250, 200));
	wxBoxSizer* fpSizer = new wxBoxSizer(wxVERTICAL);

	wxToolBar* fpToolbar = new wxToolBar(fpPanel, -1 , wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL); 
	m_fpTextCtrl = new wxTextCtrl(fpToolbar, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	fpToolbar->AddControl(m_fpTextCtrl);
	fpToolbar->AddTool(FBT_FILE_TABLE_FIND, wxT("Search"),  wxArtProvider::GetBitmap(wxART_FIND, wxART_OTHER, wxSize(16,16)));
	fpToolbar->AddTool(FBT_FILE_TABLE_DUMP, wxT("Dump"),  wxArtProvider::GetBitmap(wxART_INFORMATION, wxART_OTHER, wxSize(16,16)));
	fpToolbar->Realize();

	fpSizer->Add(fpToolbar, 0, wxEXPAND, 0);
	fpSizer->AddSpacer(4);

	m_fpBook = new wxChoicebook(fpPanel, -1, wxDefaultPosition, wxSize(250, 200));
	loadDataView(m_fpBook, 1);
	fpSizer->Add(m_fpBook, 1, wxEXPAND | wxALL, 0);
	fpPanel->SetSizer(fpSizer);
	fpPanel->Layout();

	//--

	inf = wxAuiPaneInfo().Caption(wxT("Inspector")).Right().Layer(FBT_ISP_LAYER).Name(wxT("FBTE")).Hide();
	m_fbtBook->AddPage(m_chunkExplorer, wxT("Explorer"));	
	m_fbtBook->AddPage(mpPanel, wxT("Memory Tables"));
	m_fbtBook->AddPage(fpPanel, wxT("File Tables"));
	m_auiManager->AddPane(m_fbtBook, inf);



	m_projBook = new wxAuiNotebook(this, -1, wxDefaultPosition, wxSize(250, 200), fbtNoteStyleProjT);
	m_projectExplorer = new wxTreeCtrl(this, FBT_WINDOW_PRJ,
	                                   wxDefaultPosition, wxSize(200, 200),
	                                   fbtTreeStyle);


	m_projectExplorer->AssignImageList(fbtMakeProjectImageList());

	inf = wxAuiPaneInfo().Caption(wxT("Project")).Left().Layer(FBT_PRJ_LAYER).Name(wxT("ProjB"));
	m_projBook->AddPage(m_projectExplorer,  wxT("Explorer"));
	createNewProjectTree();


	m_auiManager->AddPane(m_projBook, inf);


	m_output    = new wxAuiNotebook(this, FBT_WINDOW_OUT, wxDefaultPosition, wxSize(250, 200), fbtNoteStyleProjB);
	m_logWindow = new wxTextCtrl(this, -1, wxT(""), wxDefaultPosition, wxSize(200, 200), wxTE_MULTILINE | wxTE_READONLY);
	wxFont font (8, wxMODERN, wxNORMAL, wxNORMAL);
	m_logWindow->SetFont(font);

	m_hook.m_client = (FBTuintPtr)m_logWindow;
	m_hook.m_hook   = (fbtDebugger::ReportHook)fbtMainFrame::report;
	fbtDebugger::setReportHook(m_hook);

	m_cpBook = new wxChoicebook(this, -1, wxDefaultPosition, wxSize(250, 200));
	loadDataView(m_cpBook, 2);

	inf = wxAuiPaneInfo().Caption(wxT("Output")).Bottom().Layer(FBT_OUT_LAYER).Name(wxT("Log"));
	m_output->AddPage(m_logWindow, wxT("Log"));
	m_output->AddPage(m_cpBook,    wxT("Compiled Tables"));
	m_auiManager->AddPane(m_output, inf);


	m_note = new wxAuiNotebook(this, FBT_WINDOW_WRK, wxDefaultPosition, wxDefaultSize, fbtNoteStyle);
	inf = wxAuiPaneInfo().CenterPane().Layer(FBT_WRK_LAYER).Name(wxT("Workspace"));
	m_auiManager->AddPane(m_note, inf);
	m_auiManager->Update();

	m_startupPerspective = m_auiManager->SavePerspective();

}


void fbtMainFrame::loadMenus(void)
{
	wxMenuBar* menubar = new wxMenuBar();
	wxMenuItem* item;
	wxMenu* iter, *iter2;


	iter = new wxMenu();
	iter2 = new wxMenu();
	item = iter2->Append(FBT_NEW,       wxT("New Project\tCtrl+Shift+N"));
	item = iter2->Append(FBT_NEW_FILE,  wxT("New File\tCtrl+N"));
	iter->AppendSubMenu(iter2, wxT("New"));
	iter2 = new wxMenu();
	item = iter2->Append(FBT_OPEN,      wxT("Project\tCtrl+Shift+O"));
	item = iter2->Append(FBT_OPEN_FILE, wxT("File\tCtrl+O"));
	iter->AppendSubMenu(iter2, wxT("Open"));


	iter->AppendSeparator();
	item = iter->Append(FBT_OPEN_FBT,  wxT("Inspect File"));

	iter->AppendSeparator();
	item = iter->Append(FBT_SAVE, wxT("Save\tCtrl+S"));
	item->SetHelp(wxT("Save file"));

	item = iter->Append(FBT_SAVE_AS, wxT("Save As\tCtrl+Shift+S"));
	item->SetHelp(wxT("Save file as"));

	iter->AppendSeparator();
	item = iter->Append(FBT_QUIT, wxT("Exit"));
	item->SetHelp(wxT("Exit application."));
	menubar->Append(iter, wxT("File"));


	iter = new wxMenu();
	item = iter->Append(wxID_UNDO,  wxT("Undo\tCtrl+Z"));
	item = iter->Append(wxID_REDO,  wxT("Redo\tCtrl+Y"));
	iter->AppendSeparator();
	item = iter->Append(wxID_CUT,   wxT("Cut\tCtrl+X"));
	item = iter->Append(wxID_COPY,  wxT("Copy\tCtrl+C"));
	item = iter->Append(wxID_PASTE, wxT("Paste\tCtrl+V"));
	item = iter->Append(wxID_CLEAR, wxT("Delete\tDel"));
	iter->AppendSeparator();
	item = iter->Append(wxID_SELECTALL, wxT("Select All\tCtrl+A"));
	iter->AppendSeparator();
	item = iter->Append(FBT_CLEAR_LOG, wxT("Clear Log"));
	menubar->Append(iter, wxT("Edit"));


	iter = new wxMenu();
	item = iter->Append(FBT_WINDOW_FBT_VIEW, wxT("Inspector"));
	item = iter->Append(FBT_WINDOW_PRJ_VIEW, wxT("Project"));
	item = iter->Append(FBT_WINDOW_OUT_VIEW, wxT("Output"));
	menubar->Append(iter, wxT("View"));


	iter = new wxMenu();
	item = iter->Append(FBT_BUILD, wxT("Build Tables\tF7"));
	menubar->Append(iter, wxT("Build"));



	iter = new wxMenu();
	item = iter->Append(FBT_TOOL_GENERATE_TYPE_CAST_REPORT, wxT("Generate type cast log"));
	item->SetHelp(wxT("Generate log based on file to memory casts."));

	iter->AppendSeparator();

	item = iter->Append(FBT_TOOL_GENERATE_CPP_HEADER, wxT("Generate C++ header"));
	item->SetHelp(wxT("Compile C++ header from file tables"));
	menubar->Append(iter, wxT("Tools"));




	iter = new wxMenu();
	item = iter->Append(FBT_CLOSE_ALL, wxT("Close All Documents"));
	iter->AppendSeparator();
	item = iter->Append(FBT_EDIT_LAYOUT, wxT("Edit layout"));

	iter->AppendSeparator();
	item = iter->Append(FBT_RESET_LAYOUT, wxT("Reset Layout"));
	menubar->Append(iter, wxT("Window"));



	iter = new wxMenu();
	item = iter->Append(FBT_ABOUT, wxT("About"));
	menubar->Append(iter, wxT("Help"));

	SetMenuBar(menubar);

	wxStatusBar* stat = new wxStatusBar(this);
	stat->SetFieldsCount(2);
	int widths[] = { -1, 150};
	stat->SetStatusWidths(2, widths);
	SetStatusBar(stat);
}


void fbtMainFrame::quitEvent(wxCommandEvent& evt)
{
	// send close signal
	Close(true);
}


void fbtMainFrame::saveProject(const wxString& path)
{
	if (m_curProject)
	{
		saveModified();

		fbtProjectFile *proj = m_curProject->getFileGlobal();

		m_auiManager->Update();
		wxString str = m_auiManager->SavePerspective();
		proj->setLayout(str.c_str(), str.size());
		m_curProject->save(path.c_str());

		wxFileName fname(path);
		setProjectText(fname.GetName());
		SetTitle(wxString(fbtDefaultTitle) + wxT(" - ") + path);

	}
}


void fbtMainFrame::editTextEvent(wxCommandEvent& evt)
{
	int pos;
	if ((pos = m_note->GetSelection()) >= 0)
	{
		m_note->GetPage(pos)->GetEventHandler()->ProcessEvent(evt);
	}

}


void fbtMainFrame::saveEvent(wxCommandEvent& evt)
{
	if (m_curProject)
	{
		fbtProjectFile* proj = m_curProject->getFileGlobal();

		if (m_projPath.IsEmpty())
		{
			wxFileDialog dlg(this,
			                 "Save Inspector Project",
			                 wxEmptyString,
			                 wxEmptyString,
			                 "Inspector files (*.insp)|*.insp",
			                 wxFD_SAVE);


			if (dlg.ShowModal() == wxID_OK)
			{
				m_projPath = dlg.GetPath();
				saveProject(dlg.GetPath());

			}
		}
		else
			saveProject(m_projPath);
	}
}



void fbtMainFrame::saveAsEvent(wxCommandEvent& evt)
{
	wxFileDialog dlg(this,
	                 "Save Inspector Project",
	                 wxEmptyString,
	                 wxEmptyString,
	                 "Inspector files (*.insp)|*.insp",
	                 wxFD_SAVE);


	if (dlg.ShowModal() == wxID_OK)
		saveProject(dlg.GetPath());
}

void fbtMainFrame::openProject(const wxString& projPath)
{
	if (!m_curProject)
		return;

	fbtProjectFile* proj = m_curProject->getFileGlobal();


	if (proj->m_flags & fbtProjectFile::FF_DIRTY)
	{
		// eg add save dialog yes | no | cancel ...
	}


	fbtInspectorFile* newFile = new fbtInspectorFile();
	if (newFile->parse(projPath, fbtFile::PM_UNCOMPRESSED) == fbtFile::FS_OK)
	{
		clearProject();
		m_curProject = newFile;
		m_projPath = projPath;

		buildProject();

	}
	else
	{
		// loading failed....
		delete newFile;
	}

}


void fbtMainFrame::openEvent(wxCommandEvent& evt)
{
	wxFileDialog dlg(this,
	                 "Open Inspector Project",
	                 wxEmptyString,
	                 wxEmptyString,
	                 "Inspector files (*.insp)|*.insp",
	                 wxFD_OPEN);



	if (dlg.ShowModal() == wxID_OK)
		openProject(dlg.GetPath());
}


void fbtMainFrame::openFBTEvent(wxCommandEvent& evt)
{
	wxFileDialog dlg(this,
	                 "Open FBT compatible file",
	                 wxEmptyString,
	                 wxEmptyString,
	                 "FBT compatible (*.blend;*.bullet;*.ntree;*.insp;*.strip)|*.blend;*.bullet;*.ntree;*.insp;*.strip",
	                 wxFD_OPEN);


	if (dlg.ShowModal() == wxID_OK)
	{
		// Replace main file

		loadMainFile(dlg.GetPath());
	}
}




void fbtMainFrame::openFileEvent(wxCommandEvent& evt)
{
	wxFileDialog dlg(this,
	                 "Open C/C++ header files.",
	                 wxEmptyString,
	                 wxEmptyString,
	                 "C/C++ headers (*.h;*.hh;*.hpp;*.hxx)|*.h;*.hh;*.hpp;*.hxx",
	                 wxFD_OPEN | wxFD_MULTIPLE | wxFD_FILE_MUST_EXIST);

	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString paths;
		dlg.GetPaths(paths);
		addProjectFiles(paths);
	}
}


void fbtMainFrame::addProjectFile(fbtText* fp, bool open, bool store)
{
	if (!fp || !m_curProject)
		return;


	wxTreeItemId root = m_projectExplorer->GetRootItem();

	if (root.IsOk() && m_srcId.IsOk() && m_nteId.IsOk())
	{
		if (store)
		{
			fbtProjectFile* proj = m_curProject->getFileGlobal();
			proj->addFile(fp);
		}

		if (fp->isFBT())
		{
			open = false;
			fp->m_flag &= ~fbtText::FILE_IS_OPEN;
			m_projectExplorer->AppendItem(m_fbtId, fp->m_name, ICO_PRJ_TEXT, ICO_PRJ_TEXT, new fbtTextFileData(fp));
		}
		else if (fp->isNote())
		{
			m_projectExplorer->AppendItem(m_nteId, fp->m_name, ICO_PRJ_TEXT, ICO_PRJ_TEXT, new fbtTextFileData(fp));
			if (open)
				m_projectExplorer->Expand(m_nteId);
		}
		else
		{
			m_projectExplorer->AppendItem(m_srcId, fp->m_name, ICO_PRJ_TEXT, ICO_PRJ_TEXT, new fbtTextFileData(fp));
			if (open)
				m_projectExplorer->Expand(m_srcId);
		}

		if (open)
			openTextFile(fp->m_name, fp);

		if (!m_projectExplorer->IsExpanded(root))
			m_projectExplorer->Expand(root);
	}
	else
	{
		fbtPrintf("Invalid tree root!\n");
	}
}




void fbtMainFrame::addProjectFiles(const wxArrayString& files)
{
	if (files.IsEmpty())
		return;

	fbtProjectFile* proj = m_curProject->getFileGlobal();

	bool doOpen = files.size() == 1;
	bool treeMod = false;



	for (size_t f = 0; f < files.size(); ++f)
	{
		const char* str = (const char*)files[f].c_str();

		if (proj->hasFile(str))
			continue;

		fbtFileStream fs;
		fs.open(str, fbtStream::SM_READ);


		if (!fs.isOpen())
			continue;


		fbtText* file = new fbtText();
		file->m_flag |= fbtText::FILE_OWN;


		file->m_size = fs.size();
		file->m_data = new char[file->m_size + 1];


		FBTsize br;
		if (( br = fs.read(file->m_data, file->m_size)) > 0 )
		{
			file->m_size = br;
			file->m_data[file->m_size] = 0;



			wxFileName base(files[f]);
			strncpy(file->m_name, base.GetFullName(), 32);

			addProjectFile(file, doOpen);

			treeMod = true;
		}
		else
			delete file;
	}


	if (treeMod)
	{
		wxTreeItemId root = m_projectExplorer->GetRootItem();

		if (root.IsOk())
			m_projectExplorer->Expand(root);
	}


}




void fbtMainFrame::setProjectText(const wxString& text)
{
	wxTreeItemId id = m_projectExplorer->GetRootItem();
	if (id.IsOk())
		m_projectExplorer->SetItemText(id, text);
}




void fbtMainFrame::createNewProjectTree(const wxString& name)
{
	// Add a fresh layout
	m_projectExplorer->DeleteAllItems();


	wxTreeItemId id = m_projectExplorer->AddRoot(name.IsEmpty() ? wxT("Untitled") : name, ICO_PRJ_SYSTEM, ICO_PRJ_SYSTEM);
	m_fbtId = m_projectExplorer->AppendItem(id, wxT("FBT"),     ICO_PRJ_PACKAGE,        ICO_PRJ_PACKAGE);
	m_nteId = m_projectExplorer->AppendItem(id, wxT("Notes"),   ICO_PRJ_TEXT_NEW,       ICO_PRJ_TEXT_NEW);
	m_srcId = m_projectExplorer->AppendItem(id, wxT("Source"),  ICO_PRJ_FOLDER_CLOSE,   ICO_PRJ_FOLDER_OPEN);


	m_projectExplorer->ExpandAll();
}



void fbtMainFrame::newEvent(wxCommandEvent& evt)
{
	fbtProjectFile* proj = m_curProject->getFileGlobal();

	if (proj->m_flags & fbtProjectFile::FF_DIRTY)
	{
		// eg add save dialog yes | no | cancel ...
	}

	clearProject();

	m_curProject = new fbtInspectorFile();
	m_projPath  = wxEmptyString;

	m_auiManager->LoadPerspective(m_startupPerspective, true);
}


void fbtMainFrame::newFileEvent(wxCommandEvent& evt)
{
	if (!m_srcId.IsOk())
		return;


	fbtProjectFile* proj = m_curProject->getFileGlobal();

	fbtText* text = new fbtText();
	text->own();

	wxString fn = wxString::Format(wxT("NewFile%i"), ++m_unnamed);
	strcpy(text->m_name, (const char*)fn.c_str());

	proj->addFile(text);

	m_projectExplorer->AppendItem(m_srcId, fn, ICO_PRJ_TEXT, ICO_PRJ_TEXT, new fbtTextFileData(text));
	m_projectExplorer->Expand(m_srcId);

	openTextFile(text->m_name, text);
	proj->mark(true);
}



void fbtMainFrame::projViewEvent(wxCommandEvent& evt)
{
	wxAuiPaneInfo& inf = m_auiManager->GetPane(m_projBook);
	inf.IsShown() ? inf.Hide() : inf.Show();
	m_auiManager->Update();
}


void fbtMainFrame::outViewEvent(wxCommandEvent& evt)
{
	wxAuiPaneInfo& inf = m_auiManager->GetPane(m_output);
	inf.IsShown() ? inf.Hide() : inf.Show();
	m_auiManager->Update();
}




void fbtMainFrame::fbtViewEvent(wxCommandEvent& evt)
{
	wxAuiPaneInfo& inf = m_auiManager->GetPane(m_fbtBook);
	inf.IsShown() ? inf.Hide() : inf.Show();
	m_auiManager->Update();
}


void fbtMainFrame::openProjectMenu(wxTreeEvent& evt)
{

	if (evt.GetItem() == m_nteId)
	{
		wxMenu menu;
		menu.Append(FBT_NEW_NOTE, wxString(wxT("New Note")));
		PopupMenu(&menu);
	}
	else if (evt.GetItem() == m_srcId)
	{
		wxMenu menu;
		menu.Append(FBT_NEW_FILE,   wxString(wxT("New File")));
		menu.Append(FBT_OPEN_FILE,  wxString(wxT("Add Files")));
		menu.AppendSeparator();
		menu.Append(FBT_EXPORT_ALL, wxString(wxT("Export Files")));
		PopupMenu(&menu);
	}
	else
	{
		fbtTextFileData* item = (fbtTextFileData*)m_projectExplorer->GetItemData(evt.GetItem());

		if (item && item->m_file)
		{
			m_client = evt.GetItem();

			wxMenu menu;
			menu.Append(FBT_EXPORT, wxString(wxT("Save As")));
			menu.Append(FBT_RENAME, wxString(wxT("Rename")));
			menu.AppendSeparator();
			menu.Append(FBT_DELETE_FILE, wxString(wxT("Delete")));
			PopupMenu(&menu);

			m_client.Unset();
		}
	}
}


void fbtMainFrame::renameSelectedEvent(wxCommandEvent& evt)
{
	if (m_client.IsOk())
		m_projectExplorer->EditLabel(m_client);
}


void fbtMainFrame::exportTextEvent(wxCommandEvent& evt)
{
	if (m_client.IsOk())
	{
		fbtTextFileData* item = (fbtTextFileData*)m_projectExplorer->GetItemData(m_client);
		if (item && item->m_file)
		{
			wxString path = wxEmptyString;

			if (item->m_file->isFBT())
			{
				wxFileDialog dlg(this,
				                 "Save C/C++ Source",
				                 wxEmptyString,
				                 wxEmptyString,
				                 "Save C/C++ Source (*.c;*.cc;*.cpp;*.cxx)|*.c;*.cc;*.cpp;*.cxx",
				                 wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

				if (dlg.ShowModal() == wxID_OK)
					path = dlg.GetPath();
			}
			else
			{
				wxFileDialog dlg(this,
				                 "Save C/C++ Header",
				                 wxEmptyString,
				                 wxEmptyString,
				                 "Save C/C++ Header (*.h;*.hh;*.hpp;*.hxx)|*.h;*.hh;*.hpp;*.hxx",
				                 wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

				if (dlg.ShowModal() == wxID_OK)
					path = dlg.GetPath();
			}

			if (!path.IsEmpty())
			{
				fbtFileStream fs;
				fs.open(path.c_str(), fbtStream::SM_WRITE);

				if (item->m_file->m_data)
					fs.write(item->m_file->m_data, item->m_file->m_size);
			}
		}
	}
}


void fbtMainFrame::exportAllTextEvent(wxCommandEvent& evt)
{
	fbtProjectFile* proj = m_curProject->getFileGlobal();

	if (!proj->m_projectFiles.m_first)
		return;


	wxDirDialog dlg(this, wxT("Select Export Directory"), wxEmptyString, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxString path = dlg.GetPath();

		for (fbtText* txt = proj->m_projectFiles.m_first; txt; txt = txt->m_next)
		{
			if (!txt->isSource()) continue;

			wxFileName fname(txt->m_name);
			wxString out = path + wxT("/") + fname.GetName() + wxT(".h");





			fbtFileStream fs;
			fs.open(out.c_str(), fbtStream::SM_WRITE);

			if (txt->m_data)
				fs.write(txt->m_data, txt->m_size);
		}
	}

}


void fbtMainFrame::newNote(wxCommandEvent& evt)
{
	if (!m_nteId.IsOk())
		return;


	fbtProjectFile* proj = m_curProject->getFileGlobal();

	fbtText* text = new fbtText();
	text->own();
	text->makeNote();

	wxString fn = wxString::Format(wxT("Note%i"), ++m_unnamed);
	strcpy(text->m_name, (const char*)fn.c_str());

	proj->addFile(text);

	m_projectExplorer->AppendItem(m_nteId, fn, ICO_PRJ_TEXT, ICO_PRJ_TEXT, new fbtTextFileData(text));
	m_projectExplorer->Expand(m_nteId);

	openTextFile(text->m_name, text);
	proj->mark(true);
}



void fbtMainFrame::deleteSelectedEvent(wxCommandEvent& evt)
{

	if (m_client.IsOk())
	{
		fbtTextFileData* item = (fbtTextFileData*)m_projectExplorer->GetItemData(m_client);
		if (item && item->m_file)
		{
			fbtText* text = item->m_file;

			fbtProjectFile* proj = m_curProject->getFileGlobal();

			if (text->m_textFile)
			{
				int idx = m_note->GetPageIndex((wxWindow*)text->m_textFile);
				if (idx >= 0 && idx < (int)m_note->GetPageCount())
					m_note->DeletePage(idx);
			}

			if (text->isFBT())
				proj->mark(false);


			m_projectExplorer->Delete(m_client);
			proj->removeFile(text);
		}

		m_client.Unset();
	}
}


void fbtMainFrame::itemActivatedEvent(wxTreeEvent& evt)
{

	// Item double clicked


	fbtTextFileData* item = (fbtTextFileData*)m_projectExplorer->GetItemData(evt.GetItem());


	if (item && item->m_file)
	{
		fbtText* tf = item->m_file;

		if (!tf->isOpen())
		{
			openTextFile(tf->m_name, tf);
		}
	}
}


void fbtMainFrame::chunkActivated(wxTreeEvent& evt)
{
	if (!m_file)
		return;

	fbtChunkData* item = (fbtChunkData*)m_chunkExplorer->GetItemData(evt.GetItem());
	if (item && item->m_chunk)
	{
		fbtChunkCtrl *ctrl = new fbtChunkCtrl(this, FBT_WINDOW_CHK, wxDefaultPosition, wxSize(450, 200), wxDV_HORIZ_RULES | wxDV_VERT_RULES);
		ctrl->LoadChunk(m_file, item->m_chunk, item->m_index);


		m_note->AddPage(ctrl, m_chunkExplorer->GetItemText(evt.GetItem()), true);

		m_auiManager->Update();


		ctrl->FitParent();
	}
}




void fbtMainFrame::closeAllPages(void)
{
	size_t i = 0, p = 0;
	while (m_note->GetPageCount())
	{
		if (!m_note->DeletePage(0))
			break;
	}

	m_auiManager->Update();
}


bool fbtMainFrame::saveModified(void)
{
	bool hasMod = false;

	fbtProjectFile* proj = m_curProject->getFileGlobal();
	if (proj->m_projectFiles.m_first)
	{
		for (fbtText* fp = proj->m_projectFiles.m_first; fp; fp = fp->m_next)
		{
			if (fp->m_flag & fbtText::FILE_MODIFIED)
			{
				hasMod = true;
				proj->mark(false);

				if (fp->m_textFile)
				{
					fbtTextFile* tf = (fbtTextFile*)fp->m_textFile;
					wxString str = tf->GetText();
					tf->SetSavePoint();


					fp->m_flag &= ~fbtText::FILE_MODIFIED;
					notifyFileModified(tf);


					fbtFree(fp->m_data);
					fp->m_data = 0;
					fp->m_size = str.size();

					if (fp->m_size)
					{
						fp->m_data = (char*)fbtMalloc(fp->m_size + 1);
						fbtMemcpy(fp->m_data, ((const char*)str.c_str()), fp->m_size);
						fp->m_data[fp->m_size] = 0;
					}
				}
			}
		}
	}

	return hasMod;
}




void fbtMainFrame::openTextFile(const wxString& path, fbtText* fp)
{
	if (!fp)
		return;

	fbtTextFile* stc = new fbtTextFile(this, fp);
	if (fp->m_data)
	{
		stc->AddText(fp->m_data);

		stc->SetSelection(0, 0);
		stc->ClearSelections();
		stc->EmptyUndoBuffer();
		stc->SetModified(false);
	}


	m_note->AddPage(stc, path, true);
}

void fbtMainFrame::closeAllEvent(wxCommandEvent& evt)
{
	closeAllPages();
	m_auiManager->Update();
}


void fbtMainFrame::resetLayoutEvent(wxCommandEvent& evt)
{
	m_auiManager->LoadPerspective(m_startupPerspective, true);
}

#define ChunkCodeSub(n) (n->m_chunk.m_code != data && n->m_chunk.m_code != aray) 

void fbtMainFrame::populateChunks(fbtList& chunks)
{
	wxTreeItemId root = m_chunkExplorer->AddRoot(m_file->getHeader().c_str(), ICO_INS_SYSTEM, ICO_INS_SYSTEM);

	wxFileName fname(m_file->getPath());
	m_chunkExplorer->AppendItem(root, fname.GetName(), ICO_INS_FBT, ICO_INS_FBT);

	
	fbtBinTables *mem = m_file->getMemoryTable();



	FBTuint32 i;
	const FBTuint32 data = FBT_ID('D', 'A', 'T', 'A');
	const FBTuint32 aray = FBT_ID('A', 'R', 'A', 'Y');


	FBTuint32 skipLinks = fbtCharHashKey("Link").hash();

	wxTreeItemId prevData = root;


	for (fbtFile::MemoryChunk* node = (fbtFile::MemoryChunk*)chunks.first; node; node = node->m_next)
	{
		if (mem->m_type[mem->m_strc[node->m_newTypeId][0]].m_typeId == skipLinks)
			continue;
		if (!node->m_newBlock)
			continue;

		bool addToRoot = ChunkCodeSub(node);


		wxString name = wxString::Format("%s", mem->m_type[mem->m_strc[node->m_newTypeId][0]].m_name);
		wxTreeItemId id;

		if (node->m_chunk.m_nr > 1)

		{
			id = m_chunkExplorer->AppendItem(root, name, ICO_INS_CHUNK, ICO_INS_CHUNK);
			for (i=0; i<node->m_chunk.m_nr; i++)
				m_chunkExplorer->AppendItem(id, name, ICO_INS_CHUNK, ICO_INS_CHUNK, new fbtChunkData(node, i));
		}
		else
			id = m_chunkExplorer->AppendItem(root, name, ICO_INS_CHUNK, ICO_INS_CHUNK, new fbtChunkData(node));




	}

	m_chunkExplorer->Expand(root);
}


void fbtMainFrame::buildEvent(wxCommandEvent& evt)
{
	fbtProjectFile* proj = m_curProject->getFileGlobal();
	if (!proj->m_projectFiles.m_first)
		return;

	bool hasMods = saveModified();
	if (hasMods) proj->mark(false);

	if (proj->isBuilt())
		return;

	m_logWindow->Clear();
	wxAuiPaneInfo& inf = m_auiManager->GetPane(m_output);
	if (!inf.IsShown())
		inf.Show();

	m_auiManager->Update();


	fbtBuilder builder;
	fbtText* fp = proj->m_projectFiles.m_first;
	while (fp)
	{
		if (fp->m_data && fp->isSource())
		{
			fbtPrintf("Building ==> %s\n", fp->m_name);

			if (fp->m_size > 0)
			{
				if (builder.parseBuffer(fp->m_name, fp->m_data) < 0)
				{
					fbtPrintf("parsing failed\n");
					return;
				}
			}
		}



		fp = fp->m_next;
	}




	int status;
	if ((status = builder.buildTypes()) != LNK_OK)
	{
		fbtPrintf("Build failed %i\n", status);
		return;
	}





	if (m_compiledTables)
		delete m_compiledTables;



	m_compiledTables = builder.write();
	if (m_compiledTables)
	{
		if (m_compiledTables->read(false))
		{
			m_name[2]->DeleteAllItems();
			m_type[2]->DeleteAllItems();
			m_strc[2]->DeleteAllItems();

			populate(m_compiledTables, 2);
		}




		int idx = m_output->GetPageIndex(m_cpBook);
		if (idx >= 0 && idx < (int)m_output->GetPageCount())
			m_output->SetSelection(idx);

		if (m_fbtId.IsOk())
		{
			// open compiled file
			wxFileName fn(m_projPath.IsEmpty() ?  "Untitled" : m_projPath);


			fbtMemoryStream ms;
			ms.open(fbtStream::SM_WRITE);
			builder.writeFile((const char*)fn.GetName().c_str(), &ms);


			fbtText* text = new fbtText();
			text->own();
			text->m_flag |= fbtText::FILE_IS_FBT;
			text->copy((const char*)ms.ptr(), ms.size());
			strcpy(text->m_name, (const char*)fn.GetName().c_str());


			if (proj->hasFile(text->m_name))
				proj->removeFile(proj->getFile(text->m_name));
			proj->addFile(text);


			m_projectExplorer->DeleteChildren(m_fbtId);
			m_projectExplorer->AppendItem(m_fbtId, fn.GetName(), ICO_PRJ_TEXT, ICO_PRJ_TEXT, new fbtTextFileData(text));
			m_projectExplorer->Expand(m_fbtId);

			proj->mark(true);
		}
	}
	else
		fbtPrintf("Build failed!\n");

}


void fbtMainFrame::loadMainFile(const wxString& path)
{
	if (m_file)
		unpopulate();

	if (path.find(".blend") != -1 || path.Find(".strip") != -1)
		m_file = new fbtBlend();
	else if (path.find(".insp") != -1) // inspect inspector :p
		m_file = new fbtInspectorFile();
	else
		m_file = new btBulletFile();

	m_auiManager->GetPane(m_fbtBook).Show();
	m_auiManager->Update();



	if (m_file->parse(path.c_str(), fbtFile::PM_COMPRESSED) == fbtFile::FS_OK)
	{
		populate(m_file->getMemoryTable(), 0, m_file->getFileTable());
		populate(m_file->getFileTable(), 1, m_file->getMemoryTable());
		// build chunk window
		populateChunks(m_file->getChunks());

	}
	else
	{
		fbtPrintf("File %s loading failed\n", (const char*)path.c_str());
	}
}



void fbtMainFrame::notifyFileModified(fbtTextFile* tf)
{
	if (tf)
	{
		fbtText* dtf = tf->getFile();
		if (dtf)
		{
			int idx = m_note->GetPageIndex(tf);
			if (idx >= 0 && idx < (int)m_note->GetPageCount())
			{
				if (dtf->m_flag & fbtText::FILE_MODIFIED)
					m_note->SetPageText(idx, wxString(dtf->m_name) + wxT("*"));
				else
					m_note->SetPageText(idx, wxString(dtf->m_name));
			}
		}
	}
}



void fbtMainFrame::populate(fbtBinTables* table, int index, fbtBinTables* linkTable)
{
	FBTtype i;

	if (!table)
		return;


	for (i = 0; i < table->m_nameNr; i++)
	{
		fbtName& name = table->m_name[i];

		wxVector<wxVariant> data;
		data.push_back( wxVariant((int)i).MakeString() );
		data.push_back( wxVariant(wxString(name.m_name)) );
		data.push_back( wxVariant(name.m_ptrCount).MakeString() );
		data.push_back( wxVariant(name.m_arraySize).MakeString() );
		data.push_back( wxVariant((wxULongLong)name.m_nameId).MakeString() );
		data.push_back( wxVariant((wxULongLong)table->m_base[i]).MakeString() );
		m_name[index]->AppendItem( data );

	}

	for (i = 0; i < table->m_typeNr; i++)
	{
		fbtType& type = table->m_type[i];

		wxVector<wxVariant> data;
		data.push_back( wxVariant((int)i).MakeString() );
		data.push_back( wxVariant(wxString(type.m_name)) );
		data.push_back( wxVariant((int)type.m_strcId).MakeString() );
		data.push_back( wxVariant((int)table->m_tlen[i]).MakeString() );
		data.push_back( wxVariant((wxULongLong)type.m_typeId).MakeString() );
		m_type[index]->AppendItem( data );

	}


	for (i = 0; i < table->m_strcNr; i++)
	{
		FBTtype* type = table->m_strc[i];

		fbtStruct* strc = table->m_offs[i];
		wxASSERT(strc);


		wxVector<wxVariant> data;
		data.push_back( wxVariant((int)i).MakeString() );
		data.push_back( wxVariant(wxString(table->m_type[type[0]].m_name)));
		data.push_back( wxVariant((int)type[1]).MakeString() );
		data.push_back( wxVariant((int)table->m_tlen[type[0]]).MakeString() );
		
		if (linkTable && strc->m_link)
		{
			data.push_back( wxVariant(wxString::Format("%d (%s)",
				strc->m_link->m_strcId, linkTable->getStructType(strc->m_link))));
		}
		else
			data.push_back( wxVariant(""));

		data.push_back ( wxVariant((int)strc->getUnlinkedMemberCount()).MakeString() );

		m_strc[index]->AppendItem( data );

	}
}


void fbtMainFrame::buildProject(void)
{
	if (!m_curProject)
		return;



	fbtProjectFile* proj = m_curProject->getFileGlobal();


	if (proj->m_windowLayout && proj->m_version >= INSPECTOR_VERSION_FILE)
		m_auiManager->LoadPerspective(proj->m_windowLayout, true);


	wxFileName projPath(m_projPath);
	createNewProjectTree(projPath.GetName());
	SetTitle(wxString(fbtDefaultTitle) + wxT(" - ") + projPath.GetFullPath());


	if (proj->m_projectFiles.m_first)
	{
		fbtText* fp = proj->m_projectFiles.m_first;

		while (fp)
		{
			addProjectFile(fp, (fp->m_flag & fbtText::FILE_IS_OPEN) != 0, false);
			fp = fp->m_next;
		}
	}
}


void fbtMainFrame::destroyProject(void)
{

	closeAllPages();
	m_chunkExplorer->DeleteAllItems();

	m_projectExplorer->DeleteAllItems();
	m_logWindow->Clear();

	for (int i = 0;  i < 3; i++)
	{
		m_name[i]->DeleteAllItems();
		m_type[i]->DeleteAllItems();
		m_strc[i]->DeleteAllItems();
	}

	delete m_compiledTables;
	m_compiledTables = 0;

	delete m_file;
	m_file = 0;

	delete m_curProject;
	m_curProject = 0;


	m_projBook->SetSelection(0);
	m_output->SetSelection(0);
	m_fbtBook->SetSelection(0);
	m_mpBook->SetSelection(0);
	m_fpBook->SetSelection(0);
	m_cpBook->SetSelection(0);
}



void fbtMainFrame::clearProject(void)
{
	destroyProject();
	createNewProjectTree();
}


void fbtMainFrame::unpopulate(void)
{
	closeAllPages();
	m_chunkExplorer->DeleteAllItems();
	m_logWindow->Clear();

	for (int i = 0;  i < 3; i++)
	{
		m_name[i]->DeleteAllItems();
		m_type[i]->DeleteAllItems();
		m_strc[i]->DeleteAllItems();
	}

	if (m_file)
		delete m_file;
	m_file = 0;

	m_projBook->SetSelection(0);
	m_output->SetSelection(0);
	m_fbtBook->SetSelection(0);
	m_mpBook->SetSelection(0);
	m_fpBook->SetSelection(0);
	m_cpBook->SetSelection(0);

}


void fbtMainFrame::report(wxTextCtrl* ctrl, const char* message)
{
	if (ctrl)
		ctrl->AppendText(message);
}


void fbtMainFrame::editLabelBegin(wxTreeEvent& evt)
{
	fbtTextFileData* item = (fbtTextFileData*)m_projectExplorer->GetItemData(evt.GetItem());
	if (!item || !item->m_file)
		evt.Veto();
}


void fbtMainFrame::editLabelEnd(wxTreeEvent& evt)
{
	fbtTextFileData* item = (fbtTextFileData*)m_projectExplorer->GetItemData(evt.GetItem());
	if (item || item->m_file)
	{
		fbtText* text = item->m_file;

		const wxString& label = evt.GetLabel();
		if (!label.IsEmpty())
		{
			strcpy(text->m_name, ((const char*)label.c_str()));

			if (text->m_textFile)
			{
				int idx = m_note->GetPageIndex((wxWindow*)text->m_textFile);
				if (idx >= 0 && idx < (int)m_note->GetPageCount())
					m_note->SetPageText(idx, label);
			}
		}

	}
}




void fbtMainFrame::genCppHeader(wxCommandEvent& evt)
{
	if (!m_file) return;

	fbtBinTables* table = m_file->getFileTable();


	clearLog();
	for (size_t i = 0; i < table->m_offs.size(); i++)
	{
		fbtStruct* strc = table->m_offs[i];
		log(wxString::Format(
			"struct %s\n" 
			"{\n"
			,
			table->getStructType(strc)
			)
		);

		for (size_t k = 0; k < strc->m_members.size(); k++)
		{
			fbtStruct* member = &strc->m_members[k];
			log(wxString::Format(
				"\t%s %s;\n"
				,
				table->getStructType(member),
				table->getStructName(member)
				)
			);
		}

		log(wxString::Format(
			"};\n\n"
			));
	}

}



void fbtMainFrame::gtcEvent(wxCommandEvent& evt)
{
	wxFileDialog dlg(this,
	                 "Save Report",
	                 wxEmptyString,
	                 wxEmptyString,
	                 "HTML files(*.html)|*.html",
	                 wxFD_SAVE);


	if (dlg.ShowModal() != wxID_OK)
		return;

	fbtFileStream dest;
	dest.open(dlg.GetPath().c_str(), fbtStream::SM_WRITE);

	if (!m_file)
		return;

	fbtBinTables* fp, *mp;
	mp = m_file->getMemoryTable();
	fp = m_file->getFileTable();

	if (!mp)
	{
		fbtPrintf("Missing memory table!\n");
		return;
	}

	if (!fp)
	{
		fbtPrintf("Missing file table!\n");
		return;
	}

	fbtBinTables::OffsM::Pointer md = mp->m_offs.ptr();
	fbtBinTables::OffsM::Pointer fd = fp->m_offs.ptr();
	FBTsizeType i, s = mp->m_offs.size();

	fbtPrintf("Compiling report....\n");

	dest.writef("<html><head><title>Type cast report for %s</title></head><body><ul>", m_file->getPath());

	for (i = 0; i < s; ++i)
	{
		fbtStruct* strc = md[i];

		fbtStruct* a, *b;
		a = strc;
		b = strc->m_link;
		if (!b)
			continue;

		char* cp0 = mp->m_type[a->m_key.k16[0]].m_name;
		dest.writef("<li><a href=\"#%s\">%s</a> (%d)</li>\n", cp0, cp0, i);
	}
	dest.writef("</ul>\n");

	for (i = 0; i < s; ++i)
	{
		fbtStruct* strc = md[i];


		fbtStruct* a, *b, *c, *d;
		a = strc;
		b = strc->m_link;
		if (!b)
			continue;

		const char* cp0 = mp->getStructType(a);//mp->m_type[a->m_key.k16[0]].m_name;
		const char* cp1 = fp->getStructType(b);//fp->m_type[b->m_key.k16[0]].m_name;

		dest.writef("<center><h2>%s (%d)</h2></center>\n", cp0, i);
		dest.writef("<a id=\"%s\"/>\n", cp0);

		dest.writef("<table><tr><td>File</td><td>(<b>%s</b>)</td><td><i>To</i></td><td>Memory</td><td>(<b>%s</b>)</td></tr></table>\n", cp1, cp0);
		dest.writef("<table>\n");

		fbtStruct::Members::Pointer mbp = a->m_members.ptr();
		
		int ml = 0,  fl = 0;

		for (FBTsizeType i = 0; i < a->m_members.size(); i++)
		{
			c = &mbp[i];
			d = c->m_link;
			//char* cpMN = mp->m_name[c->m_key.k16[1]].m_name;
			const char* cpMN = mp->getStructName(c);

			//cp0 = mp->m_type[mp->m_strc[c->m_strcId][0]].m_name;
			cp0 = mp->getOwnerStructName(c);

			if (!d)
			{
				ml += c->m_len;


				dest.writef("<tr><td>%d</td><td>(M(&nbsp;&nbsp;%s&nbsp;&nbsp;)</td><td>+</td><td>%i)"
				            "</td><td><i>&nbsp;&nbsp;%s&nbsp;&nbsp;</i></td><td>=</td><td>&nbsp;&nbsp;0&nbsp;&nbsp;"
				            "</td><td></td><td>Not in file tables.</td></tr>\n", i, cp0, c->m_off, cpMN);
				continue;
			}


			//char* cpFN = fp->m_name[d->m_key.k16[1]].m_name;
			const char* cpFN = fp->getStructName(d);

			dest.writef("<tr><td>%d</td><td>(M(&nbsp;&nbsp;%s&nbsp;&nbsp;)</td><td>+</td><td>%i)</td><td>"
			            "<i>&nbsp;&nbsp;%s&nbsp;&nbsp;</i></td><td>=</td><td>F(&nbsp;&nbsp;%s&nbsp;&nbsp;)"
			            "</td><td>+</td><td>%i</td></tr>\n", i, cp0, c->m_off, cpMN, cp1, d->m_off);
			ml += c->m_len;
		}


		dest.writef("</table>\n");
	}

	dest.writef("</body></html>\n");

	fbtPrintf("\tDone..\n");

}


void fbtMainFrame::alert(const wxString& msg)
{	
	//wxLogMessage(msg);
	wxMessageDialog dlg(this, msg, "Error", wxOK | wxICON_ERROR);
	dlg.ShowModal();
}

void fbtMainFrame::log(const wxString& msg)
{
	if (m_logWindow)
		m_logWindow->AppendText(msg);
}

void fbtMainFrame::clearLog()
{
	if (m_logWindow)
		m_logWindow->Clear();
}


void fbtMainFrame::chunkItemMenu(wxTreeEvent& evt)
{
	if (!m_file) return;

    wxTreeItemId itemId = evt.GetItem();

	wxMenu menu;
	menu.Append(FBT_CHUNK_FIND, "&Find");
    menu.Append(FBT_CHUNK_SHOW_ONLY, "&Show Only");
    menu.Append(FBT_CHUNK_HIDE, "&Hide");
	menu.Append(FBT_CHUNK_UNHIDE_ALL, "&UnhideAll");

    m_chunkExplorer->PopupMenu(&menu, evt.GetPoint()); 
}

void fbtMainFrame::chunkFind(wxCommandEvent& evt)
{
	wxString name = wxGetTextFromUser("Enter a chunk name to find", "Find Chunk", "", this);
	if (!name.IsEmpty())
		chunkFindOrDelete(name, false, true);
}

void fbtMainFrame::chunkShowOnly(wxCommandEvent& evt)
{
	chunkFindOrDelete("", false);
}

void fbtMainFrame::chunkFindOrDelete(const wxString& name, bool matchDelete, bool findOnly)
{
	wxTreeCtrl* tree = m_chunkExplorer;
	wxTreeItemId root = tree->GetRootItem();

	wxString text = name;
	if (text.IsEmpty())
	{
		wxTreeItemId item = tree->GetFocusedItem();
		if (!item.IsOk() || item == root) return;

		text = tree->GetItemText(item);
	}
	
	wxTreeItemIdValue cookie;
	wxTreeItemId child = tree->GetFirstChild(root, cookie);
	while (child.IsOk())
	{
		wxTreeItemId prev = child;
		child = tree->GetNextChild(root, cookie);		
		bool match = tree->GetItemText(prev).IsSameAs(text, false);

		if (findOnly)
		{
			if (match)
			{
				tree->SelectItem(prev);
				return;
			}
		}
		else
		{
			if (matchDelete)
			{
				if (match) tree->Delete(prev);	
			}
			else
			{	
				if (!match) tree->Delete(prev);
			}
		}
	}
}

void fbtMainFrame::chunkHide(wxCommandEvent& evt)
{	
	chunkFindOrDelete("", true);
}

void fbtMainFrame::chunkUnhideAll(wxCommandEvent& evt)
{
	m_chunkExplorer->DeleteAllItems();
	populateChunks(m_file->getChunks());
}


void fbtMainFrame::findMemTable(wxCommandEvent& evt)
{	
	wxString text = m_mpTextCtrl->GetValue();
		
	findTableItem(text, 0, m_mpBook->GetSelection());
}

void fbtMainFrame::findFileTable(wxCommandEvent& evt)
{
	wxString text = m_fpTextCtrl->GetValue();
		
	findTableItem(text, 1, m_fpBook->GetSelection());
}

void fbtMainFrame::findTableItem(const wxString& text, int ictrl, int sel)
{
	if (text.IsEmpty() || ictrl > 2) return;

	wxDataViewListCtrl* ctrl = m_name[ictrl];
	if (sel == 1) ctrl = m_type[ictrl];
	else if (sel == 2) ctrl = m_strc[ictrl];

	wxDataViewItem sitem = ctrl->GetSelection();
	const wxDataViewListStore* store = ctrl->GetStore();

	unsigned int start = 0, end = store->GetCount();
	if (sitem.IsOk())
	{
		start = store->GetRow(sitem) + 1;
		ctrl->Unselect(sitem);
	}

	
	for (int t = 0; t < 2; t++)
	{
		for (unsigned int i = start; i < end; i++)
		{		
			if (ctrl->GetTextValue(i, 1).StartsWith(text))
			{
				wxDataViewItem item = store->GetItem(i);
				ctrl->Select(item);
				ctrl->EnsureVisible(item);
				ctrl->SetFocus();
				return;
			}
		}

		end = start+1;
		start = 0;
	}

	alert("Not found");
}

void fbtMainFrame::dumpTableItem(int ictrl, int sel)
{
	if (ictrl > 2 && !m_file) return;

	wxDataViewListCtrl* ctrl = m_name[ictrl];
	if (sel == 1) ctrl = m_type[ictrl];
	else if (sel == 2) ctrl = m_strc[ictrl];

	wxDataViewItem sitem = ctrl->GetSelection();
	const wxDataViewListStore* store = ctrl->GetStore();

	if (!sitem.IsOk()) return;

	unsigned long id = 0;
	wxString strId = ctrl->GetTextValue(store->GetRow(sitem), 0);
	strId.ToULong(&id);

	fbtBinTables* table  = (ictrl == 0) ? m_file->getMemoryTable() : m_file->getFileTable();
	fbtBinTables* linkTable = (ictrl == 1) ? m_file->getMemoryTable() : m_file->getFileTable();

	m_logWindow->Clear();
	if (sel == 0)
	{
		if (id >= table->m_nameNr)
			return;
		
		fbtName* name = &table->m_name[id];
		log(wxString::Format(
			"id: %d\n"
			"name: %s\n"
			"loc: %d\n"
			"nameId: %u\n"
			"ptrCount: %d\n"
			"numSlots: %d\n"
			"isFptr: %d\n"
			"arraySize: %d\n",
			id,
			name->m_name,
			name->m_loc,
			name->m_nameId,
			name->m_ptrCount,
			name->m_numSlots,
			name->m_isFptr,
			name->m_arraySize
			)
		);

		for (int i = 0; i < name->m_arraySize; i++)
			m_logWindow->AppendText(wxString::Format(" [%2d] %d\n", i, name->m_slots[i]));
	}
	else if (sel == 2)
	{
		if (id >= table->m_strcNr)
			return;

		fbtStruct* strc = table->m_offs[id];
		
		dumpTableStruct(strc, 0, table, linkTable);
	}
}

void fbtMainFrame::dumpTableStruct(fbtStruct* strc, int depth, fbtBinTables* table, fbtBinTables* linkTable)
{
	wxASSERT(strc && table && linkTable);
	fbtName& name = depth == 0 ? table->m_name[strc->m_key.k16[0]] : table->m_name[strc->m_key.k16[1]];
	log(wxString::Format("========= %d ==========\n", depth));
	log(wxString::Format(
			//"id: %d\n"
			"type: %s name: %s\n"
			"key: %u  k16(%d, %d)  //(type,name)\n"
			"val: %llu k32(%u, %u) //(type hash,basename hash)\n"
			"pointer:%d array:%d\n"
			"off: %u\n"
			"len: %u\n"
			"nr: %u\n"
			"dp: %u\n"
			"strcId: %u\n"
			"flag: 0x%x\n"
			"members: %d\n",
			//id,
			table->getStructType(strc), depth == 0 ? "" : table->getStructName(strc),
			strc->m_key.k32, strc->m_key.k16[0], strc->m_key.k16[1], 
			strc->m_val.k64, strc->m_val.k32[0], strc->m_val.k32[1], 
			name.m_ptrCount, name.m_arraySize,
			strc->m_off,
			strc->m_len,
			strc->m_nr,
			strc->m_dp,
			strc->m_strcId,
			strc->m_flag,
			strc->m_members.size()
		)
	);

	if (strc->m_link)
	{
		log(wxString::Format("--------- link --------\n"));
		fbtStruct* org = strc;
		strc = strc->m_link;
		fbtName& name = depth == 0 ? linkTable->m_name[strc->m_key.k16[0]] : linkTable->m_name[strc->m_key.k16[1]];
		log(wxString::Format(
			//"id: %d\n"
			"  type: %s name: %s\n"			
			"  key: %u  k16(%d, %d)\n"
			"  val: %llu k32(%u, %u)\n"
			"  pointer:%d array:%d\n"
			"  off: %u\n"
			"  len: %u\n"
			"  nr: %u\n"
			"  dp: %u\n"
			"  strcId: %u\n"
			"  flag: 0x%x\n"
			"  members: %d\n",
			//id,
			linkTable->getStructType(strc), depth == 0 ? "" : linkTable->getStructName(strc),
			strc->m_key.k32, strc->m_key.k16[0], strc->m_key.k16[1], 
			strc->m_val.k64, strc->m_val.k32[0], strc->m_val.k32[1], 
			name.m_ptrCount, name.m_arraySize,
			strc->m_off,
			strc->m_len,
			strc->m_nr,
			strc->m_dp,
			strc->m_strcId,
			strc->m_flag,
			strc->m_members.size()
			)
		);
		strc = org;
	}
	else
	{
		log("unlinked structure\n");
	}

	for (size_t i = 0; i < strc->m_members.size(); i++)
	{
		dumpTableStruct(&strc->m_members[i], depth+1, table, linkTable);
	}
}


void fbtMainFrame::dumpMemTable(wxCommandEvent& evt)
{
	dumpTableItem(0, m_mpBook->GetSelection());
}

void fbtMainFrame::dumpFileTable(wxCommandEvent& evt)
{
	dumpTableItem(1, m_fpBook->GetSelection());
}

void fbtMainFrame::clearLog(wxCommandEvent& evt)
{
	m_logWindow->Clear();
}
