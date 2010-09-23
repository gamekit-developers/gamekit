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
#include <wx/aui/aui.h>
#include <wx/dcclient.h>
#include <wx/settings.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/tooltip.h>
#include <wx/filedlg.h>

#include "nsMainWindow.h"
#include "nsNodeCanvas.h"
#include "nsSolutionBrowser.h"
#include "nsPropertyPage.h"
#include "nsWorkspace.h"
#include "nsRenderSystem.h"
#include "nsNodeTypeInfo.h"
#include "nsNodeManager.h"
#include "nsNodeWriter.h"
#include "nsNodeReader.h"
//#include "nsOgreKitPreview.h"

#include "Resource/Icon.xpm"


NS_IMPLEMENT_SINGLETON(nsMainWindow);


BEGIN_EVENT_TABLE( nsMainWindow, wxFrame )

	// file menu
	EVT_MENU(NSID_QUIT,         nsMainWindow::quitEvent)
	EVT_MENU(NS_ID_NEW_TREE,    nsMainWindow::treeAddEvent )
	EVT_MENU(NSID_SAVE,         nsMainWindow::saveEvent)
	EVT_MENU(NSID_SAVEAS,       nsMainWindow::saveAsEvent)
	EVT_MENU(NSID_OPEN,         nsMainWindow::loadEvent)


	// edit
	EVT_MENU(NS_ID_CUT,         nsMainWindow::cutEvent)
	EVT_MENU(NS_ID_COPY,        nsMainWindow::copyEvent)
	EVT_MENU(NS_ID_PASTE,       nsMainWindow::pasteEvent)
	EVT_MENU(NS_ID_DUPLICATE,   nsMainWindow::duplicateEvent)
	EVT_MENU(NS_ID_DELETE,      nsMainWindow::deleteCapturedEvent)
	EVT_MENU(NS_ID_SELECT_ALL,  nsMainWindow::selectAllEvent)

	// view menu
	EVT_MENU(NS_ID_SOLUTION,    nsMainWindow::solutionCheckEvent )
	EVT_MENU(NS_ID_PROPERTIES,  nsMainWindow::propertiesCheckEvent )
	EVT_MENU(NS_ID_FULLSCREEN,  nsMainWindow::showFullscreenEvent )

	// aui events
	EVT_AUI_PANE_CLOSE(nsMainWindow::paneCloseEvent)

	// node add events
	EVT_MENU_RANGE(NSID_NODE_MENU_START, NSID_NODE_MENU_END, nsMainWindow::nodeAddEvent )
END_EVENT_TABLE()




nsMainWindow::nsMainWindow()
	:   wxFrame(NULL, NS_WID_MAINWINDOW, "Node Editor", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE)
{
	m_addMenuCaller = 0;
	wxToolTip::SetDelay(5);
	SetIcon( Icon_xpm );

	// load objects pre show.
	m_auiManager = new wxAuiManager(this, wxAUI_MGR_ALLOW_ACTIVE_PANE | wxAUI_MGR_DEFAULT);

	wxAuiDockArt* art = m_auiManager->GetArtProvider();
	art->SetMetric( wxAUI_DOCKART_PANE_BORDER_SIZE,  0 );
	art->SetMetric( wxAUI_DOCKART_SASH_SIZE,         3 );
	art->SetMetric( wxAUI_DOCKART_CAPTION_SIZE,      18 );

	loadWindows();
	loadMenus();
	loadSettings();

	m_auiManager->Update();
	Show();

	m_currentFile = wxEmptyString;

	// post show setup
	nsWorkspace::getSingleton().initializeContext();



	// default start with a blank tree
	wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, NS_ID_NEW_TREE);
	ProcessWindowEvent(evt);


	setStatus(0, "Ready");
}



nsMainWindow::~nsMainWindow()
{
	//if (m_previewApp)
	//    delete m_previewApp;
	//m_previewApp = 0;

	if (IsShown())
		nsWorkspace::getSingleton().finalizeContext();

	saveSettings();
	m_auiManager->UnInit();
	delete m_auiManager;
}


void nsMainWindow::loadSettings(void)
{
	// window settings
	bool maximize = false;
	wxSize size(800, 600);
	wxPoint pos(10, 10);
	wxString layout;

	wxConfig* def = new wxConfig("NodeEditor");

	if (def->HasEntry("/WindowSettings/sx"))
		def->Read("/WindowSettings/sx", &size.x);
	if (def->HasEntry("/WindowSettings/sy"))
		def->Read("/WindowSettings/sy", &size.y);
	if (def->HasEntry("/WindowSettings/px"))
		def->Read("/WindowSettings/px", &pos.x);
	if (def->HasEntry("/WindowSettings/py"))
		def->Read("/WindowSettings/py", &pos.y);
	if (def->HasEntry("/WindowSettings/maximize"))
		def->Read("/WindowSettings/maximize", &maximize);
	if (def->HasEntry("/WindowSettings/layout"))
		def->Read("/WindowSettings/layout", &layout);

	if (!maximize)
	{
		SetSize(size);
		SetPosition(pos);
	}
	else
	{
		SetPosition(wxPoint(10, 10));
		SetSize(wxSize(800, 600));
		Maximize();
	}

	if (!layout.empty())
	{
		m_auiManager->LoadPerspective(layout, true);

		// update menus
		wxAuiPaneInfo inf = m_auiManager->GetPane(nsPropertyPage::getSingletonPtr());
		m_viewProperties->Check(inf.IsShown());

		inf = m_auiManager->GetPane(nsSolutionBrowser::getSingletonPtr());
		m_viewSolution->Check(inf.IsShown());
	}

	delete def;
}


void nsMainWindow::saveSettings(void)
{
	// window settings
	wxConfig* def = new wxConfig("NodeEditor");

	wxSize size = GetSize();
	wxPoint pos = GetPosition();

	def->Write("/WindowSettings/sx",          size.x);
	def->Write("/WindowSettings/sy",          size.y);
	def->Write("/WindowSettings/px",          pos.x);
	def->Write("/WindowSettings/py",          pos.y);
	def->Write("/WindowSettings/maximize",    IsMaximized());
	def->Write("/WindowSettings/layout",      m_auiManager->SavePerspective());
	delete def;
}



void nsMainWindow::loadWindows(void)
{
	wxAuiPaneInfo inf;
	inf.DestroyOnClose(false).MaximizeButton(true).MinimizeButton(true);


	nsSolutionBrowser* browser = new nsSolutionBrowser(this);
	inf.Caption("Solution Explorer").Left().Layer(0).Name("Solution");
	inf.TopDockable(false);
	m_auiManager->AddPane(browser, inf);

	nsPropertyPage* page = new nsPropertyPage(this);
	page->initialize();

	inf.Caption("Property Browser").Right().Layer(0).Name("Property");
	inf.TopDockable(false);
	m_auiManager->AddPane(page, inf);

	nsWorkspace* work = new nsWorkspace(this);
	inf = wxAuiPaneInfo().CenterPane().Name("Workspace");
	m_auiManager->AddPane(work, inf);

}



void nsMainWindow::loadMenus(void)
{
	wxMenuBar* menubar = new wxMenuBar();
	wxMenuItem* item;
	wxMenu* file, *edit, *view;

	// --- File Menu ---

	file = new wxMenu();
	item = file->Append(NSID_NEW, "New Project");
	item->SetHelp("Create a new node tree solution.");

	file->AppendSeparator();

	item = file->Append(NSID_OPEN, "Open Project\tCtrl-O");
	item->SetHelp("Open node tree solution.");

	item = file->Append(NSID_SAVE, "Save\tCtrl-S");
	item->SetHelp("Save existing solution.");

	item = file->Append(NSID_SAVEAS, "Save As\tCtrl-Shift-S");
	item->SetHelp("Save solution to another file.");

	file->AppendSeparator();
	item = file->Append(NS_ID_NEW_TREE, "New Tree\tCtrl-N");
	item->SetHelp("Create a new node tree.");

	file->AppendSeparator();
	item = file->Append(NSID_QUIT, "Exit");
	item->SetHelp("Exit application.");

	menubar->Append(file, "File");


	// --- Edit Menu ---

	edit = new wxMenu();
	edit->Append(NS_ID_UNDO, "Undo\tCtrl-Z");
	edit->Append(NS_ID_REDO, "Redo\tCtrl-Shift-Z");
	edit->AppendSeparator();
	makeEditMenu(edit);
	menubar->Append(edit, wxT("Edit"));

	// --- View Menu ---

	view = new wxMenu();
	item = view->AppendCheckItem(NS_ID_SOLUTION, wxT("Solution Explorer\tF2"));
	item->SetCheckable(true); item->Check(m_auiManager->GetPane(nsSolutionBrowser::getSingletonPtr()).IsShown());
	item->SetHelp(wxT("Show Solution Explorer."));
	m_viewSolution = item;

	item = view->AppendCheckItem(NS_ID_PROPERTIES, wxT("Properties\tF3"));
	item->SetCheckable(true); item->Check(m_auiManager->GetPane(nsPropertyPage::getSingletonPtr()).IsShown());
	item->SetHelp(wxT("Show Properties Window."));
	m_viewProperties = item;

	item = view->AppendCheckItem(NS_ID_VARIABLES, wxT("Variable Editor\tF4"));
	item->SetCheckable(true); item->Check(false);
	item->SetHelp(wxT("Show Variable Editor."));
	view->AppendSeparator();

	item = view->Append(NS_ID_FULLSCREEN, wxT("Full screen\tF12"));
	item->SetHelp(wxT("Switch to full screen mode."));
	menubar->Append(view, wxT("View"));

	SetMenuBar(menubar);

	// status

	wxStatusBar* stat = new wxStatusBar(this);
	stat->SetFieldsCount(2);
	int widths[] = { -1, 150};
	stat->SetStatusWidths(2, widths);

	SetStatusBar(stat);
}



void nsMainWindow::makeEditMenu(wxMenu* menu)
{
	// the base edit toolbox

	wxMenuItem* item;

	item = menu->Append(NS_ID_CUT, "Cut\tCtrl-X");
	item->SetHelp("Cut selected nodes.");

	item = menu->Append(NS_ID_COPY, "Copy\tCtrl-C");
	item->SetHelp("Copy selected nodes.");

	item = menu->Append(NS_ID_PASTE, "Paste\tCtrl-V");
	item->SetHelp("Paste clipboard.");

	menu->AppendSeparator();
	item = menu->Append(NS_ID_DUPLICATE, "Duplicate\tCtrl-D");
	item->SetHelp("Duplicate selected nodes.");

	item = menu->Append(NS_ID_SELECT_ALL, "Select All\tCtrl-A");
	item->SetHelp("Select All nodes.");

	menu->AppendSeparator();
	item = menu->Append(NS_ID_DELETE, "Delete\tDel");
	item->SetHelp("Delete selected nodes.");
}


void nsMainWindow::makeNodeMenu(wxWindow* caller, wxMenu* menu)
{
	m_addMenuCaller = caller;

	// the base add toolbox
	nsNodeTypeInfo& types = nsNodeTypeInfo::getSingleton();


	nsNodeTypeInfo::GroupIterator it = types.getGroupIterator();
	while (it.hasMoreElements())
	{
		nsNodeTypeInfo::GroupIterator::Pair kv = it.getNext();
		wxString groupName = types.getGroupName(kv.first.key());

		// current group

		wxMenu* sub = new wxMenu();
		wxMenuItem* item;

		nsNodeTypeInfo::GroupListIterator typeIt = nsNodeTypeInfo::GroupListIterator(kv.second);
		while (typeIt.hasMoreElements())
		{
			nsNodeDef* nt = typeIt.getNext();
			wxString nodeTypeName = nt->getName();
			wxString nodeTypeHelp = nt->getDocStr();


			item = sub->Append( NSID_NODE_MENU_START + nt->getId(), nodeTypeName);


			// help text
			if (nodeTypeHelp.empty())
				item->SetHelp(wxString::Format("Create new %s type", nodeTypeName));
			else
				item->SetHelp(nodeTypeHelp);
		}

		// append the group
		item = menu->AppendSubMenu(sub, groupName);
		item->SetHelp(wxString::Format("Node group %s", groupName));
	}
}



void nsMainWindow::quitEvent(wxCommandEvent& evt)
{
	// send close signal
	Close(true);
}


void nsMainWindow::saveEvent(wxCommandEvent& evt)
{
	// if modified
	if (m_currentFile == wxEmptyString)
	{
		wxFileDialog dlg(this,
		                 "Save Node Logic",
		                 wxEmptyString,
		                 wxEmptyString,
		                 "Node logic files (*.ntree)|*.ntree",
		                 wxFD_SAVE);

		if (dlg.ShowModal() == wxID_OK)
			m_currentFile = dlg.GetPath();
	}

	if (m_currentFile != wxEmptyString)
	{
		nsNodeWriter writer;
		writer.writeToFile(wxToAscii(m_currentFile));

		SetTitle(wxString::Format("Node Editor [%s]", m_currentFile));
	}

}



void nsMainWindow::saveAsEvent(wxCommandEvent& evt)
{
	wxFileDialog dlg(this,
	                 "Save Node Logic",
	                 wxEmptyString,
	                 wxEmptyString,
	                 "Node logic files (*.ntree)|*.ntree",
	                 wxFD_SAVE);

	if (dlg.ShowModal() == wxID_OK)
		m_currentFile = dlg.GetPath();
}


void nsMainWindow::loadEvent(wxCommandEvent& evt)
{
	wxFileDialog dlg(this,
	                 "Load Node Logic",
	                 wxEmptyString,
	                 wxEmptyString,
	                 "Node logic files (*.ntree)|*.ntree",
	                 wxFD_OPEN);

	if (dlg.ShowModal() == wxID_OK)
	{
		m_currentFile = dlg.GetPath();


		nsNodeReader reader;
		reader.load(m_currentFile);
		SetTitle(wxString::Format("Node Editor [%s]", m_currentFile));
	}
}


void nsMainWindow::solutionCheckEvent(wxCommandEvent& evt)
{
	// show / hide solution
	wxAuiPaneInfo& inf = m_auiManager->GetPane(nsSolutionBrowser::getSingletonPtr());
	inf.IsShown() ? inf.Hide() : inf.Show();
	m_viewSolution->Check(inf.IsShown());
	m_auiManager->Update();
}



void nsMainWindow::propertiesCheckEvent( wxCommandEvent& evt )
{
	// show / hide properties
	wxAuiPaneInfo& inf = m_auiManager->GetPane(nsPropertyPage::getSingletonPtr());
	inf.IsShown() ? inf.Hide() : inf.Show();
	m_viewProperties->Check(inf.IsShown());
	m_auiManager->Update();
}


void nsMainWindow::paneCloseEvent(wxAuiManagerEvent& evt)
{
	int id = evt.GetPane()->window->GetId();
	if (id == NS_WID_SOLUTION)
		m_viewSolution->Check(false);
	else if (id == NS_WID_PROPERTY)
		m_viewProperties->Check(false);
}



void nsMainWindow::showFullscreenEvent(wxCommandEvent& evt)
{
	// full screen window
	ShowFullScreen(!IsFullScreen(), wxFULLSCREEN_NOCAPTION | wxFULLSCREEN_NOBORDER | wxFULLSCREEN_NOSTATUSBAR);
}


void nsMainWindow::nodeAddEvent(wxCommandEvent& evt)
{
	int id = evt.GetId() - NSID_NODE_MENU_START;

	// find menu identifier
	nsNodeDef* nt = nsNodeTypeInfo::getSingleton().findTypeInfo( id );
	if (!nt)
	{
		m_addMenuCaller = 0;
		return;
	}


	// find the calling object & tree
	nsNodeTree* tree = 0;
	if (m_addMenuCaller && m_addMenuCaller->GetId() == NS_WID_CANVAS)
		tree = static_cast<nsNodeCanvas*>(m_addMenuCaller)->getTree();
	else if (m_addMenuCaller && m_addMenuCaller->GetId() == NS_WID_SOLUTION)
		tree = static_cast<nsSolutionBrowser*>(m_addMenuCaller)->getSelectedTree();


	if (!tree || !tree->getAttachedCanvas())
	{
		m_addMenuCaller = 0;
		return;
	}



	nsNodeCanvas* canvas = tree->getAttachedCanvas();
	if (canvas)
	{
		// drop node
		nsNode* nd = tree->createNode(nt);
		canvas->addNode(nd);
	}

	m_addMenuCaller = 0;
}



void nsMainWindow::treeAddEvent(wxCommandEvent& evt)
{
	// create new tree
	nsNodeTree* tree = nsNodeManager::getSingleton().createTree();

	nsTreeEvent treeEvent(NS_TREE_ADD, this, tree);

	// send events
	nsSolutionBrowser::getSingleton().treeEvent(treeEvent);
	nsWorkspace::getSingleton().treeEvent(treeEvent);
}


void nsMainWindow::deleteCapturedEvent(wxCommandEvent& evt)
{
	// pass down to workspace
	nsWorkspace::getSingleton().deleteCapturedEvent(evt);
}


void nsMainWindow::selectAllEvent(wxCommandEvent& evt)
{
	// pass down to workspace
	nsWorkspace::getSingleton().selectAllEvent(evt);
}


void nsMainWindow::cutEvent(wxCommandEvent& evt)
{
	// pass down to workspace
	nsWorkspace::getSingleton().cutEvent(evt);
}


void nsMainWindow::copyEvent(wxCommandEvent& evt)
{
	// pass down to workspace
	nsWorkspace::getSingleton().copyEvent(evt);
}


void nsMainWindow::pasteEvent(wxCommandEvent& evt)
{
	// pass down to workspace
	nsWorkspace::getSingleton().pasteEvent(evt);
}


void nsMainWindow::duplicateEvent(wxCommandEvent& evt)
{
	// pass down to workspace
	nsWorkspace::getSingleton().duplicateEvent(evt);
}


void nsMainWindow::setStatus(int i, const char* fmt, ...)
{
#ifdef _MSC_VER
# define ns_vsnprintf _vsnprintf
#else
# define ns_vsnprintf vsnprintf
#endif

	if (i < 0 && i >= 2 || !GetStatusBar())
		return;

	char buf[256];
	va_list lst;
	va_start(lst, fmt);
	int size = ns_vsnprintf(buf, 256, fmt, lst);
	va_end(lst);

	if (size < 0)
	{
		size = 256;
		buf[size] = 0;
	}

	GetStatusBar()->SetStatusText(wxString(buf), i);
}
