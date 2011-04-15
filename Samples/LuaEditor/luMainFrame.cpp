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


#include "okApp.h"
#include "okWin.h"
#include "okCamera.h"
#include "okUtils.h"
#include "luDefs.h"
#include "luFile.h"
#include "luMainApp.h"
#include "luMainFrame.h"
#include "luMainApp.h"
#include "luLog.h"
#include "luHelp.h"
#include "luProjPanel.h"
#include "luUtils.h"
#include "luPropsPanel.h"
#include "luOutputs.h"
#include "luEdit.h"
#include "Lib/liUtils.h"
#include "luProcess.h"
#include "luWizard.h"
#include "luFile.h"
#include "luConfig.h"
#include "luAnimDlg.h"

#include "Resource/app.xpm"

#if defined(_MSC_VER) && defined(_DEBUG)
#include <malloc.h>
#include <crtdbg.h>
#define _VC_DEBUG
#endif



#define RUNTIME_LOG_FILE "LuaRuntime.log"

#define DEFAULT_CAMERA_RADIUS 10

GK_INLINE bool IsSameFile(const wxString& file1, const wxString& file2)
{
	return wxFileName(file1).SameAs(file2);
}

//#define NOTEBOOK_STYLE (wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxNO_BORDER)

#define BOOK_MAIN_STYLE		wxAUI_NB_TOP                | wxAUI_NB_TAB_MOVE     |\
                            wxAUI_NB_SCROLL_BUTTONS     | wxAUI_NB_CLOSE_BUTTON |\
                            wxAUI_NB_WINDOWLIST_BUTTON  | wxTAB_TRAVERSAL       |\
                            wxAUI_NB_TAB_SPLIT

#define BOOK_PROJ_STYLE		wxAUI_NB_TOP            | wxAUI_NB_TAB_MOVE          | \
							wxAUI_NB_SCROLL_BUTTONS | wxAUI_NB_TAB_EXTERNAL_MOVE

#define BOOK_OUTPUT_STYLE	wxAUI_NB_BOTTOM         | wxAUI_NB_TAB_MOVE          | \
							wxAUI_NB_SCROLL_BUTTONS | wxAUI_NB_TAB_EXTERNAL_MOVE

#define MAIN_FRAME_SIZE		wxSize(1024,800)

BEGIN_EVENT_TABLE(luMainFrame, wxFrame)
	EVT_DROP_FILES(					luMainFrame::OnDropFiles)
	EVT_IDLE(						luMainFrame::OnIdle)
	EVT_CLOSE(						luMainFrame::OnClose)

	EVT_MENU(wxID_SAVE,				luMainFrame::OnSaveAll)
	
    EVT_MENU(wxID_CLOSE,			luMainFrame::OnExit)
	//EVT_MENU(wxID_OPEN,				luMainFrame::OnOpen)
	EVT_MENU(ID_LOG_CLEAR,			luMainFrame::OnClearLog)
	EVT_MENU(ID_LOG_SAVE,			luMainFrame::OnSaveLog)
	EVT_TIMER(ID_STATUS_TIMER,		luMainFrame::OnTimer)	
	EVT_MENU(ID_TIMER_PAUSE,		luMainFrame::OnPauseRun)
	
	EVT_MENU(ID_OPEN_PROJ_FILE,		luMainFrame::OnOpenProjFile)
	EVT_MENU(ID_OPEN_BLEND_FILE,	luMainFrame::OnOpenBlendFile)
	EVT_MENU(ID_OPEN_LUA_FILE,		luMainFrame::OnOpenLuaFile)

	EVT_MENU(ID_NEW_PROJ_FILE,		luMainFrame::OnNewProjFile)
	EVT_MENU(ID_NEW_BLEND_FILE,		luMainFrame::OnNewBlendFile)
	EVT_MENU(ID_NEW_LUA_FILE,		luMainFrame::OnNewLuaFile)

	EVT_MENU(ID_PROJ_STARTUP_FILE,	luMainFrame::OnProjStartupFile)
	EVT_MENU(ID_PROJ_REMOVE_FILE,	luMainFrame::OnProjRemoveFile)
	

	EVT_MENU(ID_RUN_RUNTIME,		luMainFrame::OnRunRuntime)
	EVT_MENU(ID_RUN_BLENDER,		luMainFrame::OnRunBlender)
	
	EVT_MENU(ID_SHOW_HELP,			luMainFrame::OnShowHelp)
	EVT_MENU(ID_SHOW_PROJ_WIN,		luMainFrame::OnShowProjWin)
	EVT_MENU(ID_SHOW_LOG_WIN,		luMainFrame::OnShowLogWin)
	EVT_MENU(ID_SHOW_PROPS_WIN,		luMainFrame::OnShowPropsWin)

	EVT_MENU(ID_TOOL_ANIM_MANAGER,	luMainFrame::OnToolAnimManager)
	EVT_MENU(ID_TOOL_STATE_MANAGER,	luMainFrame::OnToolStateManager)	
	
	EVT_MENU(ID_WIN_PAGE_PREV,		luMainFrame::OnWinPagePrev)
	EVT_MENU(ID_WIN_PAGE_NEXT,		luMainFrame::OnWinPageNext)

	EVT_MENU(ID_CAMERA_POINTS,		luMainFrame::OnChangeCameraPolyMode)
	EVT_MENU(ID_CAMERA_WIREFRAME,	luMainFrame::OnChangeCameraPolyMode)
	EVT_MENU(ID_CAMERA_SOLID,		luMainFrame::OnChangeCameraPolyMode)

	EVT_MENU(ID_CAMERA_POS_RESET,	luMainFrame::OnChangeCameraDirection)
	EVT_MENU(ID_CAMERA_POS_FRONT,	luMainFrame::OnChangeCameraDirection)
	EVT_MENU(ID_CAMERA_POS_RIGHT,	luMainFrame::OnChangeCameraDirection)
	EVT_MENU(ID_CAMERA_POS_TOP,		luMainFrame::OnChangeCameraDirection)

	EVT_COMMAND(ID_OKWIN_OBJECT_SELECTED, wxEVT_NULL, luMainFrame::OnGameObjectSelected)

	EVT_MENU(ID_SHOW_PHYSICS_DEBUG, luMainFrame::OnShowPhysicsDebug)
	EVT_MENU(ID_SHOW_BOUNDING_BOX,	luMainFrame::OnShowBoundingBox)
	EVT_MENU(ID_SHOW_AXIS,			luMainFrame::OnShowAxis)

	EVT_TREE_ITEM_ACTIVATED(ID_PROJ_TREE, luMainFrame::OnProjTreeItemActivated)

	//-- send event to focused edit
	// edit
    EVT_MENU(wxID_CLEAR,			luMainFrame::OnEdit)
    EVT_MENU(wxID_CUT,				luMainFrame::OnEdit)
    EVT_MENU(wxID_COPY,				luMainFrame::OnEdit)
    EVT_MENU(wxID_PASTE,			luMainFrame::OnEdit)
    EVT_MENU(ID_CODE_INDENTINC,		luMainFrame::OnEdit)
    EVT_MENU(ID_CODE_INDENTRED,		luMainFrame::OnEdit)
    EVT_MENU(wxID_SELECTALL,		luMainFrame::OnEdit)
    EVT_MENU(ID_CODE_SELECTLINE,	luMainFrame::OnEdit)
    EVT_MENU(wxID_REDO,				luMainFrame::OnEdit)
    EVT_MENU(wxID_UNDO,				luMainFrame::OnEdit)
    // find
    EVT_MENU(wxID_FIND,            luMainFrame::OnEdit)
    EVT_MENU(ID_CODE_FINDNEXT,     luMainFrame::OnEdit)
    EVT_MENU(ID_CODE_REPLACE,      luMainFrame::OnEdit)
    EVT_MENU(ID_CODE_REPLACENEXT,  luMainFrame::OnEdit)
    EVT_MENU(ID_CODE_BRACEMATCH,   luMainFrame::OnEdit)
    EVT_MENU(ID_CODE_GOTO,         luMainFrame::OnEdit)
    // view
    EVT_MENU_RANGE (ID_CODE_HILIGHTFIRST, ID_CODE_HILIGHTLAST, luMainFrame::OnEdit)
    EVT_MENU(ID_CODE_DISPLAYEOL,   luMainFrame::OnEdit)
    EVT_MENU(ID_CODE_INDENTGUIDE,  luMainFrame::OnEdit)
    EVT_MENU(ID_CODE_LINENUMBER,   luMainFrame::OnEdit)
    EVT_MENU(ID_CODE_LONGLINEON,   luMainFrame::OnEdit)
    EVT_MENU(ID_CODE_WHITESPACE,   luMainFrame::OnEdit)
    EVT_MENU(ID_CODE_FOLDTOGGLE,   luMainFrame::OnEdit)
    EVT_MENU(ID_CODE_OVERTYPE,     luMainFrame::OnEdit)
    EVT_MENU(ID_CODE_READONLY,     luMainFrame::OnEdit)
    EVT_MENU(ID_CODE_WRAPMODEON,   luMainFrame::OnEdit)
    // extra
    EVT_MENU(ID_CODE_CHANGELOWER,  luMainFrame::OnEdit)
    EVT_MENU(ID_CODE_CHANGEUPPER,  luMainFrame::OnEdit)
    EVT_MENU(ID_CODE_CONVERTCR,    luMainFrame::OnEdit)
    EVT_MENU(ID_CODE_CONVERTCRLF,  luMainFrame::OnEdit)
    EVT_MENU(ID_CODE_CONVERTLF,    luMainFrame::OnEdit)
    EVT_MENU(ID_CODE_CHARSETANSI,  luMainFrame::OnEdit)
    EVT_MENU(ID_CODE_CHARSETMAC,   luMainFrame::OnEdit)
	
	EVT_MENU(ID_CODE_BOOKMARK_TOGGLE,	luMainFrame::OnEdit)
	EVT_MENU(ID_CODE_BOOKMARK_CELAR,	luMainFrame::OnEdit)
	EVT_MENU(ID_CODE_BOOKMARK_NEXT,		luMainFrame::OnEdit)
	EVT_MENU(ID_CODE_BOOKMARK_PREV,		luMainFrame::OnEdit)

	EVT_AUINOTEBOOK_PAGE_CLOSE(ID_BOOK_MAIN, luMainFrame::OnClosePage)
	//EVT_TREE_KEY_DOWN(ID_PROJ_TREE, luProjTree::OnKeyChar)
END_EVENT_TABLE()



luMainFrame::luMainFrame() : 
	wxFrame(NULL, ID_MAIN_FRAME, APP_TITLE, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE | wxSUNKEN_BORDER),
	m_okWin(NULL),
	m_timer(this, ID_STATUS_TIMER),
	m_statusBar(NULL),
	m_logListener(NULL),
	m_logBox(NULL),
	m_runtimeLogFile(NULL),
	m_consoleEdit(NULL),
	m_logRuntime(NULL),
	m_pidHelp(0),
	m_pidRuntime(0),
	m_projPanel(NULL),
	m_propsPanel(NULL),
	m_aui(NULL),	
	m_projFile(NULL),
	m_runtimeProcess(NULL),
	m_bookMain(NULL),
	m_bookOutput(NULL),
	m_bookProps(NULL),
	m_focusEdit(NULL),
	m_isLoadingProjFile(false),
	m_isLoadedBlendFile(false),
	m_helpTopic(NULL),
	m_animDlg(NULL)
{    
	SetIcon(app_icon_xpm);

#ifdef _VC_DEBUG
	//_CrtSetBreakAlloc(15528); 
#endif



	//-- setup aui
	m_aui = new wxAuiManager(this, wxAUI_MGR_ALLOW_ACTIVE_PANE | wxAUI_MGR_DEFAULT);
	setupDefaultAuiDockArt(m_aui);

	//-- setup menu
	setupMenu();

	//-- setup statusbar
	m_statusBar = CreateStatusBar();

	setupWindows();

	//-- toolbar
	setupToolbar();	

	SetSize(MAIN_FRAME_SIZE);

	//Layouts();
	m_aui->Update();
    Show();

	DragAcceptFiles(true);

	wxString projFile = getLuApp()->getProjFileToOpen();
	if (!projFile.IsEmpty())
		openFileByType(projFile);

	m_helpTopic = new luHhkFile;
	if (!m_helpTopic->load(LuConfig.getHelpTopicFile()))
		alert(wxString("Can't open help topic file: ") + LuConfig.getHelpTopicFile());

	wxString runtimeLogFile = wxFileName::GetCwd() + "/" + RUNTIME_LOG_FILE;
	m_runtimeLogFile = new luLogFile(runtimeLogFile);	
}

luMainFrame::~luMainFrame()
{

	if (m_pidRuntime > 0)
	{
		wxProcess::Kill(m_pidRuntime);
		m_pidRuntime = 0;
	}

	if (m_pidHelp > 0)
	{
		wxProcess::Kill(m_pidHelp);
		m_pidHelp = 0;
	}
	
	SAFE_DELETE(m_animDlg);

	SAFE_DELETE(m_helpTopic);
	SAFE_DELETE(m_runtimeLogFile);

	SAFE_DELETE(m_runtimeProcess);
	SAFE_DELETE(m_projFile);

	if (Ogre::LogManager::getSingletonPtr())
	{
		Ogre::Log *log = Ogre::LogManager::getSingleton().getDefaultLog();
		if (log) log->removeListener(m_logListener);		
	}
	delete wxLog::SetActiveTarget(NULL); //delete m_logListener

	if (m_aui) 
	{
		m_aui->UnInit();
		SAFE_DELETE(m_aui);
	}
}

void luMainFrame::setAppTitle(const wxString &blend) 
{ 
	SetTitle(wxString::Format("%s - %s", APP_TITLE, blend)); 
}


void luMainFrame::setupWindows()
{
	wxBitmap pageBmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16));


	//-- output
	m_logBox = new luLogBox(this, ID_LOG_BOX);

	m_bookOutput = new wxAuiNotebook(this, ID_BOOK_OUTPUT, wxDefaultPosition, wxDefaultSize, BOOK_OUTPUT_STYLE);
	m_bookOutput->AddPage(m_logBox, "Log", false, pageBmp);

	m_logRuntime = new luLogEdit(this, ID_LOG_RUNTIME_EDIT);
	m_bookOutput->AddPage(m_logRuntime, "Run", false, pageBmp);

	m_consoleEdit = new luConsoleEdit(this, ID_CONSOLE_EDIT);
	m_bookOutput->AddPage(m_consoleEdit, "Console", false, pageBmp);

	//m_bookOutput->SetSelection(2);

	//-- props

	m_propsPanel = new luPropsPanel(this);

	m_bookProps = new wxAuiNotebook(this, ID_BOOK_PROP, wxDefaultPosition, wxDefaultSize, BOOK_OUTPUT_STYLE);
	m_bookProps->AddPage(m_propsPanel, "Properties", false, pageBmp);


	//-- main
	m_bookMain = new wxAuiNotebook(this, ID_BOOK_MAIN, wxDefaultPosition, wxDefaultSize, BOOK_MAIN_STYLE);

	
	//-- project
	m_projPanel = new luProjPanel(this);

	//-- okWin
	luMainApp* app = (luMainApp*)wxTheApp;
	okWindow* win = new okWindow(this, wxSize(WIN_SIZE_X, WIN_SIZE_Y));
	win->setRenderOnly(true);
	win->setEanbleCameraControl(true);

	luLog *logListener = new luLog(m_logBox);
	delete wxLog::SetActiveTarget(logListener);

	wxLog::EnableLogging(true);

	Ogre::Log *log = Ogre::LogManager::getSingleton().getDefaultLog();
	if (log) log->addListener(logListener);

	m_logListener = logListener;

	wxLogMessage("Initing...");
	Ogre::Timer watch;
	okApp* okapp = app->getOkApp();
	bool ok = okapp->init("", win->getNativeHandle(), WIN_SIZE_X, WIN_SIZE_Y); GK_ASSERT(ok);
	if (win->init(okapp, okapp->getMainWindow())) 
	{		
		wxLogMessage("Elapsed time for initing: %.3f seconds", watch.getMilliseconds()/1000.0f);
		m_timer.Start(1000/STATUS_FPS);
		m_okWin = win;
		m_bookMain->AddPage(m_okWin, "Blend", false, pageBmp);
	}
	else
	{
		SAFE_DELETE(win);
		wxLogMessage("Error - Can't init the Ogrekit.");
	}

	//-- size

	int sizeX = WIN_SIZE_X, sizeY = WIN_SIZE_Y + LOG_BOX_HEIGHT;
	if (m_statusBar)
		sizeY += m_statusBar->GetSize().GetHeight();

	SetClientSize(sizeX, sizeY);


	//-- aui
	
	wxAuiPaneInfo info;
	info.DestroyOnClose(false).MaximizeButton(true).MinimizeButton(true);
	
	m_aui->AddPane(m_projPanel, 
		wxAuiPaneInfo(info).Caption("Project").Left().Layer(1).Name("Project"));

	m_aui->AddPane(m_bookProps,
		wxAuiPaneInfo(info).Caption("Properties").Right().Layer(2).Name("Properties").MinSize(200,100));

	
	m_aui->AddPane(m_bookOutput, 
		wxAuiPaneInfo(info).Caption("Output").Bottom().Layer(3).Name("Output").MinSize(100,LOG_BOX_HEIGHT));


	m_aui->AddPane(m_bookMain, wxAuiPaneInfo().Name("bookMain").Layer(0).
                  CenterPane().PaneBorder(false));


}

void luMainFrame::setupMenu()
{
	if (GetMenuBar()) return;

    wxMenu* menu = NULL, * submenu = NULL;
	
	//-- file
	menu = new wxMenu;

	submenu = new wxMenu;
	submenu->Append(ID_NEW_PROJ_FILE,	"New &Projct");
	submenu->Append(ID_NEW_LUA_FILE,	"New &Lua File");
	submenu->Append(ID_NEW_BLEND_FILE,	"New &Blend File");
	menu->AppendSubMenu(submenu, "&New");

	submenu = new wxMenu;
	submenu->Append(ID_OPEN_PROJ_FILE,	"Open &Projct");
	submenu->Append(ID_OPEN_LUA_FILE,	"Open &Lua File");
	submenu->Append(ID_OPEN_BLEND_FILE,	"Open &Blend File");
	menu->AppendSubMenu(submenu, "&Open");
	menu->Append(wxID_SAVE, "&Save All\tCtrl-S");

	//menu->Append(wxID_OPEN);
	menu->AppendSeparator();
	menu->Append(ID_EXPORT_DISTRIBUTION,	"Export for Distribution");
	menu->AppendSeparator();

	menu->Append(ID_LOG_CLEAR, "&Clear Log");
	menu->Append(ID_LOG_SAVE,  "&Save Log");

	menu->AppendSeparator();
    menu->Append(wxID_CLOSE);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menu, "&File");

	
	//-- code

    // Edit menu
    wxMenu *menuEdit = new wxMenu;
    menuEdit->Append(wxID_UNDO, wxT("&Undo\tCtrl+Z"));
    menuEdit->Append(wxID_REDO, wxT("&Redo\tCtrl+Shift+Z"));
    menuEdit->AppendSeparator();
    menuEdit->Append(wxID_CUT, wxT("Cu&t\tCtrl+X"));
    menuEdit->Append(wxID_COPY, wxT("&Copy\tCtrl+C"));
    menuEdit->Append(wxID_PASTE, wxT("&Paste\tCtrl+V"));
    menuEdit->Append(wxID_CLEAR, wxT("&Delete\tDel"));
    menuEdit->AppendSeparator();
    menuEdit->Append(wxID_FIND, wxT("&Find\tCtrl+F"));
    menuEdit->Append(ID_CODE_FINDNEXT, wxT("Find &next\tF3"));
    menuEdit->Enable(ID_CODE_FINDNEXT, false);
    menuEdit->Append(ID_CODE_REPLACE, wxT("&Replace\tCtrl+R"));
    menuEdit->Append(ID_CODE_REPLACENEXT, wxT("Replace &again\tShift+F4"));
    menuEdit->Enable(ID_CODE_REPLACENEXT, false);
    menuEdit->AppendSeparator();
    menuEdit->Append(ID_CODE_BRACEMATCH, wxT("&Match brace\tCtrl+M"));
    menuEdit->Append(ID_CODE_GOTO, wxT("&Goto\tCtrl+G"));
    menuEdit->AppendSeparator();
    menuEdit->Append(ID_CODE_INDENTINC, wxT("&Indent increase\tTab"));
    menuEdit->Append(ID_CODE_INDENTRED, wxT("I&ndent reduce\tShift+Tab"));
    menuEdit->AppendSeparator();
    menuEdit->Append(wxID_SELECTALL, wxT("&Select all\tCtrl+A"));
    menuEdit->Append(ID_CODE_SELECTLINE, wxT("Select &line\tCtrl+L"));
	menuEdit->AppendSeparator();
	menuEdit->AppendCheckItem(ID_CODE_FOLDTOGGLE, wxT("&Toggle current fold\tCtrl+T"));
	menuEdit->AppendCheckItem(ID_CODE_OVERTYPE, wxT("&Overwrite mode\tIns"));
	menuEdit->AppendCheckItem(ID_CODE_WRAPMODEON, wxT("&Wrap mode"));


	wxMenu* menuBookmark = new wxMenu;
	menuBookmark->AppendCheckItem(ID_CODE_BOOKMARK_TOGGLE, wxT("&Toggle bookmark\tCtrl+F2"));
	menuBookmark->AppendCheckItem(ID_CODE_BOOKMARK_CELAR, wxT("ClearAll bookmark\tCtrl+Shift+F2"));
	menuBookmark->AppendCheckItem(ID_CODE_BOOKMARK_NEXT, wxT("Goto next bookmark\tF2"));
	menuBookmark->AppendCheckItem(ID_CODE_BOOKMARK_PREV, wxT("Goto prev bookmark\tShift+F2"));
	menuEdit->AppendSeparator();
	menuEdit->AppendSubMenu(menuBookmark, wxT("Bookmark .."));
	
    // charset menu
    wxMenu* menuCharset = new wxMenu;
    menuCharset->Append(ID_CODE_CHARSETANSI, wxT("&ANSI (Windows)"));
    menuCharset->Append(ID_CODE_CHARSETMAC, wxT("&MAC (Macintosh)"));
	menuEdit->AppendSeparator();
	menuEdit->Append (ID_CODE_USECHARSET, wxT("Use &code page of .."), menuCharset);

    // View menu
    wxMenu* menuView = new wxMenu;
	menuView->AppendCheckItem(ID_CODE_DISPLAYEOL,  wxT("Show line &endings"));
	menuView->AppendCheckItem(ID_CODE_INDENTGUIDE, wxT("Show &indent guides"));
	menuView->AppendCheckItem(ID_CODE_LINENUMBER,  wxT("Show line &numbers"));
	menuView->AppendCheckItem(ID_CODE_LONGLINEON,  wxT("Show &long line marker"));
	menuView->AppendCheckItem(ID_CODE_WHITESPACE,  wxT("Show white&space"));
	menuView->AppendSeparator();
		

    // change case submenu
    wxMenu *menuChangeCase = new wxMenu;
    menuChangeCase->Append(ID_CODE_CHANGEUPPER, wxT("&Upper case\tCtrl+Shift+U"));
    menuChangeCase->Append(ID_CODE_CHANGELOWER, wxT("&Lower case\tCtrl+U"));

    // convert EOL submenu
    wxMenu *menuConvertEOL = new wxMenu;
    menuConvertEOL->Append(ID_CODE_CONVERTCR, wxT("CR (&Linux)"));
    menuConvertEOL->Append(ID_CODE_CONVERTCRLF, wxT("CR+LF (&Windows)"));
    menuConvertEOL->Append(ID_CODE_CONVERTLF, wxT("LF (&Macintosh)"));

    // Extra menu
	menuEdit->AppendSeparator();
    menuEdit->Append(ID_CODE_CHANGECASE, wxT("Change &case to .."), menuChangeCase);
    menuEdit->Append(ID_CODE_CONVERTEOL, wxT("Convert line &endings to .."), menuConvertEOL);

	menuEdit->AppendSubMenu(menuView, "View");

	menuBar->Append(menuEdit, wxT("&Edit"));

	//-- render
	menu = new wxMenu;
	menu->Append(ID_TIMER_PAUSE, "&Pause");
	
	submenu = new wxMenu;
	submenu->Append(ID_CAMERA_POINTS, "&Points");
	submenu->Append(ID_CAMERA_WIREFRAME, "&Wireframe");
	submenu->Append(ID_CAMERA_SOLID, "&Solid");
	menu->AppendSubMenu(submenu, "Polygon &Mode");
	
	submenu = new wxMenu;
	submenu->Append(ID_CAMERA_POS_RESET, "&Reset\tCtrl-Enter");
	submenu->Append(ID_CAMERA_POS_TOP, "&Top");
	submenu->Append(ID_CAMERA_POS_FRONT, "&Front");
	submenu->Append(ID_CAMERA_POS_RIGHT, "Ri&ght");
	
	menu->AppendSubMenu(submenu, "Camera Direction");

	menu->Append(ID_SHOW_PHYSICS_DEBUG, "Show &Physics Debug");
	menu->Append(ID_SHOW_BOUNDING_BOX, "Show &Bounding Box");
	menu->Append(ID_SHOW_AXIS, "Show &Axis");

	menuBar->Append(menu, "&Render");

	//-- run
	menu = new wxMenu;
	menu->Append(ID_RUN_RUNTIME, "&Run Runtime");
	menu->Append(ID_RUN_BLENDER, "Run &Blender");
	menuBar->Append(menu, "&Run");

	//-- window
	menu = new wxMenu;
	menu->Append(ID_SHOW_PROJ_WIN, "Show Project\tF9");
	menu->Append(ID_SHOW_LOG_WIN, "Show Log\tF10");
	menu->Append(ID_SHOW_PROPS_WIN, "Show Properties\tF11");
	menu->AppendSeparator();
    menu->Append(ID_WIN_PAGE_PREV, wxT("&Previous\tCtrl+Shift+Tab"));
    menu->Append(ID_WIN_PAGE_NEXT, wxT("&Next\tCtrl+Tab"));

	menuBar->Append(menu, "&Window");

	//-- tools
	menu = new wxMenu;
	menu->Append(ID_TOOL_ANIM_MANAGER, "Animation Manger");
	menu->Append(ID_TOOL_STATE_MANAGER, "State Manager");

	menuBar->Append(menu, "&Tools");

	//-- help
	menu = new wxMenu;
	menu->Append(ID_SHOW_HELP, "&View Help\tF1");
	menuBar->Append(menu, "&Help");

	//-- end

	SetMenuBar(menuBar);
}


void luMainFrame::setFocusEdit(luEdit* edit, bool setFocus)
{
	if (!setFocus)
	{
		if (edit == m_focusEdit)
			m_focusEdit = NULL;
		
		return;
	}
	
	m_focusEdit = edit;
}

void luMainFrame::setupToolbar()
{
	if (GetToolBar()) return;

	GK_ASSERT(m_aui); if (!m_aui) return;

	wxAuiToolBar* tb = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                         wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW);

	tb->SetToolBitmapSize(wxSize(16,16));
	
	tb->AddTool(ID_NEW_PROJ_FILE, "NewProject",  wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16)));
	tb->AddTool(ID_OPEN_PROJ_FILE, "OpenProject",  wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_OTHER, wxSize(16,16)));	
	tb->AddTool(wxID_SAVE, "SaveAll",	 wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_OTHER, wxSize(16,16)));
	tb->AddTool(ID_RUN_RUNTIME,  "Run",  wxArtProvider::GetBitmap(wxART_INFORMATION, wxART_OTHER, wxSize(16,16)));
	tb->AddTool(ID_TOOL_ANIM_MANAGER,  "Animation",  wxArtProvider::GetBitmap(wxART_TICK_MARK, wxART_OTHER, wxSize(16,16)));
	
    tb->AddSeparator();

    tb->AddTool(ID_SHOW_HELP, "Help", wxArtProvider::GetBitmap(wxART_QUESTION, wxART_OTHER, wxSize(16,16)));

	tb->Realize();

	m_aui->AddPane(tb, wxAuiPaneInfo().
		Name(wxT("tb")).Caption("Toolbar").
		ToolbarPane().Top().
		LeftDockable(false).RightDockable(false)
	);
}

void luMainFrame::showHidePanel(wxWindow* panel)
{
	if (!m_aui || !panel) return;

	wxAuiPaneInfo& info = m_aui->GetPane(panel);
	
	if (info.IsShown())
		info.Hide();
	else
		info.Show();

	m_aui->Update();
}


void luMainFrame::OnClosePage(wxAuiNotebookEvent& event)
{	
	if (m_bookMain && m_bookMain->GetPage(event.GetSelection()) == m_okWin)
		event.Veto();
	else
		event.Skip();
}

void luMainFrame::OnEdit(wxCommandEvent &event) 
{
	luEdit* edit = getFocusEdit();
	if (edit) edit->GetEventHandler()->ProcessEvent(event);
}

void luMainFrame::OnChangeCameraPolyMode(wxCommandEvent& event)
{
	if (!m_okWin) return;

	int id = event.GetId();
	if (id == ID_CAMERA_POINTS)
		m_okWin->setCameraPolyMode(Ogre::PM_POINTS);
	else if (id == ID_CAMERA_WIREFRAME)
		m_okWin->setCameraPolyMode(Ogre::PM_WIREFRAME);
	else if (id == ID_CAMERA_SOLID)
		m_okWin->setCameraPolyMode(Ogre::PM_SOLID);
}

void luMainFrame::OnChangeCameraDirection(wxCommandEvent& event)
{
	if (!m_okWin || !m_okWin->getCamera()) return;

	okCamera* camera = m_okWin->getCamera();

	gkVector3 pos(0,0, DEFAULT_CAMERA_RADIUS);

	int id = event.GetId();
	okCamera::CAMERA_DIR dir = okCamera::DIR_RESET;
	if (id == ID_CAMERA_POS_FRONT)
		dir = okCamera::DIR_FRONT;
	else if (id == ID_CAMERA_POS_TOP)
		dir = okCamera::DIR_TOP;
	else if (id == ID_CAMERA_POS_RIGHT)
		dir = okCamera::DIR_RIGHT;

	camera->reset(pos, dir);
}

void luMainFrame::OnShowPhysicsDebug(wxCommandEvent& event)
{
	if (!m_okWin) return;

	okApp* app = m_okWin->getApp();
	bool show = app->getShowPhysicsDebug();
	app->setShowPhysicsDebug(!show);
	//if (!show) app->step();
}

void luMainFrame::OnShowBoundingBox(wxCommandEvent& event)
{
	if (!m_okWin) return;
	
	m_okWin->toggleShowBoundingBox();
}

void luMainFrame::OnShowAxis(wxCommandEvent& event)
{
	if (!m_okWin) return;
	
	m_okWin->toggleShowAxis();
}


void luMainFrame::OnShowProjWin(wxCommandEvent& event)
{
	showHidePanel(m_projPanel);
}

void luMainFrame::OnShowLogWin(wxCommandEvent& event)
{
	showHidePanel(m_bookOutput);
}

void luMainFrame::OnShowPropsWin(wxCommandEvent& event)
{
	showHidePanel(m_bookProps);
}


void luMainFrame::OnWinPagePrev(wxCommandEvent& event)
{
	if (!m_bookMain) return;

	int i = m_bookMain->GetSelection() - 1;
	if (i < 0)
		i = m_bookMain->GetPageCount() - 1;
	m_bookMain->SetSelection(i);
}

void luMainFrame::OnWinPageNext(wxCommandEvent& event)
{
	if (!m_bookMain) return;

	int i = m_bookMain->GetSelection() - 1;
	if (i >= (int)m_bookMain->GetPageCount())
		i =  0;
	m_bookMain->SetSelection(i);
}

void luMainFrame::OnOpenProjFile(wxCommandEvent& WXUNUSED(event))
{
	wxFileDialog *dlg = new wxFileDialog(this, "Open Project File", "","", "Project Files (*.okproj)|*.okproj", wxFD_OPEN);
	if (dlg->ShowModal() == wxID_OK)
	{
		 wxString fileName = dlg->GetPath();
		 openProjFile(fileName);
	}
}

void luMainFrame::OnOpenLuaFile(wxCommandEvent& WXUNUSED(event))
{
	wxFileDialog *dlg = new wxFileDialog(this, "Open Lua File", "","", "Lua Files (*.lua)|*.lua", wxFD_OPEN);
	if (dlg->ShowModal() == wxID_OK)
	{
		 wxString fileName = dlg->GetPath();
		 openLuaFile(fileName);
	}
}

void luMainFrame::OnOpenBlendFile(wxCommandEvent& WXUNUSED(event))
{
	wxFileDialog *dlg = new wxFileDialog(this, "Open Blend File", "","", "Blend Files (*.blend)|*.blend", wxFD_OPEN);
	if (dlg->ShowModal() == wxID_OK)
	{
		 wxString fileName = dlg->GetPath();
		 openBlendFile(fileName);
	}
}

void luMainFrame::OnProjRemoveFile(wxCommandEvent& event)
{
	if (!m_projPanel) return;

	wxString fileName = m_projPanel->getSelectedFile();
	if (!fileName.IsEmpty())
		removeFileInProject(fileName);
}

void luMainFrame::OnProjStartupFile(wxCommandEvent& event)
{
	if (!m_projPanel) return;

	wxString fileName = m_projPanel->getSelectedFile();
	if (!fileName.IsEmpty())
		changeStartupFileInProject(fileName);
}

void luMainFrame::OnNewProjFile(wxCommandEvent& event)
{
	luNewProjWizard wizard(this);

	wizard.RunWizard(wizard.getFirstPage());
}

void luMainFrame::OnNewBlendFile(wxCommandEvent& event)
{
	if (!m_projFile)
	{
		alert("Project isn't created.");
		return;
	}

	wxString blendFile = LuConfig.getBaseBlendFile();
	if (!wxFile::Exists(blendFile))
	{
		alert("Base blend file isn't exist: " + blendFile);
		return;
	}

	wxFileDialog *dlg = new wxFileDialog(this, "New Blend File", m_projFile->getProjDir(),"", "Blend Files (*.blend)|*.blend", wxFD_SAVE);
	if (dlg->ShowModal() == wxID_OK)
	{
		 wxString fileName = dlg->GetPath();
		 		 
		 if (!wxCopyFile(blendFile, fileName))
		 {
			 alert("Can't create the blend file: " + fileName);
			 return;
		 }

		 openBlendFile(fileName);
	}
}

void luMainFrame::OnSaveAll(wxCommandEvent& event)
{
	if (!m_projFile) return;

	bool ok = saveProjFile() && saveAllModifiedFilesInProject();

	gkPrintf("Project is saved: %d", ok);
}

void luMainFrame::OnNewLuaFile(wxCommandEvent& event)
{
	if (!m_projFile)
	{
		alert("Project isn't created.");
		return;
	}

	wxFileDialog *dlg = new wxFileDialog(this, "New Lua File", m_projFile->getProjDir(),"", "Lua Files (*.lua)|*.lua", wxFD_SAVE);
	if (dlg->ShowModal() == wxID_OK)
	{
		 wxString fileName = dlg->GetPath();

		 wxFile file;
		 if (!file.Create(fileName))
		 {
			 alert("Can't create the lua file: " + fileName);
			 return;
		 }

		 file.Close();

		 openLuaFile(fileName);
	}
}

void luMainFrame::OnDropFiles(wxDropFilesEvent& event)
{
	if (event.GetNumberOfFiles() < 0) return;
	wxString file = *event.GetFiles();
			
	openFileByType(file);
}


bool luMainFrame::openFileByType(const wxString& file, bool exceptProj)
{
	bool ok = false;

	ITEM ftype = getLuFileType(file);

	if (ftype == ITEM_BLEND)
		ok = openBlendFile(file);
	else if (ftype == ITEM_LUA)
		ok = openLuaFile(file);
	else if (ftype == ITEM_PROJ && !exceptProj)
		ok = openProjFile(file);
	else
		alert(wxString::Format("Unsupported file type: %s", file));

	return ok;
}



bool luMainFrame::saveAllModifiedFilesInProject()
{
	for (size_t i = 0; i < m_bookMain->GetPageCount(); i++)
	{
		luEdit* edit = dynamic_cast<luEdit*>(m_bookMain->GetPage(i));
		if (!edit) continue;

		if (edit->isModified())
		{
			wxString fileName = edit->getFileName();
			edit->SaveFile(fileName);
			if (edit->isModified())
			{
				alert("Can't save file: " + fileName);
				return false;
			}
		}
	}

	return true;
}

bool luMainFrame::removeFileInProject(const wxString& pathName)
{
	if (!m_projPanel || !m_projFile) return false;

	if (!m_projFile->removeFile(WX2GK(pathName)))
		return false;

	m_projPanel->removeFile(pathName);

	return true;
}


bool luMainFrame::changeStartupFileInProject(const wxString& pathName)
{
	if (!m_projPanel || !m_projFile) return false;
	wxString oldFile = m_projFile->getStartLuaFile();

	if (oldFile == pathName) return true;
	
	m_projPanel->setStartupFile(oldFile, false);
	m_projPanel->setStartupFile(pathName, true);

	m_projFile->setStartLuaFile(WX2GK(pathName));

	return true;
}

okApp* luMainFrame::getOkApp()
{
	return m_okWin ? m_okWin->getApp() : NULL;
}

bool luMainFrame::createNewProject(const wxString& name, const wxString& projFileName, int projTemplate)
{
	luProjTempList *ptlist = getLuApp()->getProjTempList();
	if (projTemplate < 0 || projTemplate >= (int)ptlist->size())
	{
		gkPrintf("Error - Project template isn't exist. (%d)", projTemplate);
		return false;
	}
	luProjTemplate& templ = (*ptlist)[projTemplate];

	luProjectFile projFile;
	if (!projFile.create(WX2GK(projFileName), WX2GK(name)))
	{
		gkPrintf("Error - Can't create project file: %s", (const char*)projFileName);
		return false;
	}

	wxString projDir = projFile.getProjDir();

	wxString luaFile;

	if (!templ.startLuaFile.empty())
		luaFile = projDir + "/" + wxFileName(templ.startLuaFile).GetFullName();

	for (size_t i = 0; i < templ.files.size(); i++)
	{
		wxString srcFile = templ.files[i];
		wxString destFile = projDir + "/" + wxFileName(srcFile).GetFullName();
		if (!wxCopyFile(srcFile, destFile, false))
		{
			gkPrintf("Error - Can't copy %s to %s.", (const char*)srcFile, (const char*)destFile);
			return false;
		}

		if (!projFile.addFile(WX2GK(destFile)))
		{
			gkPrintf("Error - Can't add % to project.",  (const char*)srcFile);
			return false;
		}

		if (luaFile.IsEmpty() && ITEM_LUA == getLuFileType(destFile))
		{
			luaFile = destFile;
		}
	}


	projFile.setStartLuaFile(WX2GK(luaFile));

	if (!projFile.save())
	{
		gkPrintf("Error - Can't save project file: %s", (const char*)projFileName);
		return false;
	}

	return true;
}


bool luMainFrame::saveProject(bool closeOpendFile)
{
	bool ok = saveProjFile() && saveAllModifiedFilesInProject();

	if (ok && closeOpendFile)
	{
		GK_ASSERT(m_bookMain);

		int nonEditPage = -1;
		utArray<size_t> pages;
		for (int i = (int)m_bookMain->GetPageCount() - 1; i >= 0; i--)
		{
			if (dynamic_cast<luEdit*>(m_bookMain->GetPage(i)) != NULL)
			{
				pages.push_back(i);
			}
		}

		for (size_t i = 0; i < pages.size(); i++)
		{
			m_bookMain->DeletePage(pages[i]);
		}

		m_focusEdit = NULL;
		if (m_okWin) m_okWin->resetScene();
	}

	return ok;
}


bool luMainFrame::openProjFile(const wxString& fileName)
{
	if (m_projFile && !saveProject(true)) return false;
	

	luProjectFile* proj = new luProjectFile;	
			
	if (!proj->load(WX2GK(fileName)))
	{
		SAFE_DELETE(proj);
		return false;
	}

	luProjectFile* oldProj = m_projFile;
	m_projFile = proj;
	m_isLoadingProjFile = true;
	m_isLoadedBlendFile = false;
	m_blendFile = "";

	gkString dir = proj->getProjDir();

	
	wxString failedFiles;

	if (m_projPanel) 
	{
		m_projPanel->clearAll();
	}

	for (size_t i = 0; i < proj->getFileSize(); i++)	
	{
		gkString path = proj->getFile(i, false);
		gkPrintf("open file: %s", path.c_str());
		if (!openFileByType(path, true))
		{
			gkPrintf("Can't open file: %s", path.c_str());			
			failedFiles += path + "\n";
		}
	}

	if (m_projPanel) 
	{
		if (!proj->getStartLuaFile().empty())
			m_projPanel->setStartupFile(proj->getStartLuaFile(), true);			

		m_projPanel->setRootItemText(proj->getProjName());
		m_projPanel->expandRoot();		
	}

	if (m_propsPanel)
	{
		m_propsPanel->updateProject(proj);
	}

	SAFE_DELETE(oldProj);

	m_isLoadingProjFile = false;
	m_isLoadedBlendFile = false;

	if (!failedFiles.IsEmpty())
		alert(wxString::Format("One more file opening is failed: %s", failedFiles));

	return true;
}

bool luMainFrame::openBlendFile(const wxString& fileName, bool addToProject)
{
	if (!m_okWin || !m_bookMain) return false;

	if (!m_isLoadingProjFile || !m_isLoadedBlendFile)
	{
		if (m_timer.IsRunning())
			m_timer.Stop();

		wxSize size = m_okWin->GetSize();
	
		for (size_t i = 0; i < m_bookMain->GetPageCount(); i++)
		{
			if (m_bookMain->GetPage(i) == m_okWin)
			{
				m_bookMain->SetSelection(i);
				break;
			}
		}

		if (IsSameFile(fileName, m_blendFile))
		{
			wxLogMessage("Already loaded.");
			return true;
		}

		wxLogMessage("Loading... %s ", fileName);

		Ogre::Timer watch;
		if (!m_okWin->loadScene(WX2GK(fileName)))
		{
			wxString msg = wxString::Format("Error - Can't load the blend file: %s", fileName);
			wxLogMessage(msg);
			alert(msg);

			return false;
		}

		m_blendFile = fileName;

		wxLogMessage("Elapsed time for loading: %.3f seconds", watch.getMilliseconds()/1000.0f);

		m_okWin->SendSizeEvent();

		GetMenuBar()->SetLabel(ID_TIMER_PAUSE, m_okWin->isRunnigGameLoop() ? "&Pause" : "&Resume");
		if (!m_timer.IsRunning())
			m_timer.Start(1000/STATUS_FPS);

		if (m_isLoadingProjFile)
			m_isLoadedBlendFile = true;

		onSceneChanged();
	}

	if (addToProject && isProjectCreated())
	{
		addFileToProject(fileName, ITEM_BLEND);			
	}

	return true;
}

void luMainFrame::onSceneChanged()
{
	SAFE_DELETE(m_animDlg);
	refreshInspPanel();
}

void luMainFrame::refreshInspPanel()
{
	m_projPanel->clearListItems();
	gkScene *scene = getOkApp()->getFirstScene();
	if (scene)
	{
		gkGameObjectHashMap& objects = scene->getObjects();
		for (size_t i = 0; i < objects.size(); i++)
		{
			gkGameObject* obj = objects[i];
			m_projPanel->addListItem(obj->getName(), getLuObjectTypeName(obj->getType()));
		}
	}


}


bool luMainFrame::openLuaFile(const wxString& fileName, bool addToProject)
{
	if (!m_bookMain)
		return false;

	for (size_t i = 0; i < m_bookMain->GetPageCount(); i++)
	{
		luEdit* edit = dynamic_cast<luEdit*>(m_bookMain->GetPage(i));
		if (!edit) continue;		

		if (IsSameFile(edit->getFileName(), fileName))
		{
			m_bookMain->SetSelection(i);
			return true;
		}
	}

	wxBitmap pageBmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16));

	luEdit *edit= new luEdit(this);
	
	if (!edit->openFile(fileName))
	{
		alert("Can't open Lua file: " + fileName);
		SAFE_DELETE(edit);
		return false;
	}

	m_bookMain->AddPage(edit, wxFileName::FileName(fileName).GetFullName(), false, pageBmp);
	m_bookMain->SetSelection(m_bookMain->GetPageCount()-1);

	if (addToProject && isProjectCreated())
		addFileToProject(fileName, ITEM_LUA);

	return true;
}

void luMainFrame::OnPauseRun(wxCommandEvent& event)
{
	if (!m_okWin) return;

	if (m_okWin->isRunnigGameLoop())
		m_okWin->stopGameLoop();
	else
		m_okWin->startGameLoop();

	GetMenuBar()->SetLabel(ID_TIMER_PAUSE, m_okWin->isRunnigGameLoop() ? "&Pause" : "&Resume");

	wxLogMessage("%s", m_okWin->isRunnigGameLoop() ? "Running" :  "Paused");
}

void luMainFrame::OnClearLog(wxCommandEvent& WXUNUSED(event))
{
	if (!m_logBox) return;

	m_logBox->Clear();
}


void luMainFrame::OnSaveLog(wxCommandEvent& WXUNUSED(event))
{
	if (!m_logBox) return;

	wxFileDialog *dlg = new wxFileDialog(this, wxFileSelectorPromptStr,  "",  "", "Text File (*.txt)|*.txt", wxFD_SAVE);
	if (dlg->ShowModal() == wxID_OK)
	{
		wxString fileName = dlg->GetPath();
		m_logBox->save(fileName);		
	}
}

void luMainFrame::OnOpen(wxCommandEvent& WXUNUSED(event))
{
}


void luMainFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
	m_timer.Stop();
    Close(true);
}

void luMainFrame::OnClose(wxCloseEvent& event)
{	
	if (event.CanVeto() && (m_projFile && !saveProject()))
	{
		if ( wxMessageBox("Can't save the project file... continue closing?",
							"Please confirm",
							wxICON_QUESTION | wxYES_NO) != wxYES )
		{
			event.Veto();
			return;
		}
	}

	Destroy();
}

bool luMainFrame::saveProjFile()
{
	if (!m_projFile) return false;

	if (!m_projFile->isModified())
		return true;

	return m_projFile->save();
}

void luMainFrame::OnTimer(wxTimerEvent& event)
{
	if (event.GetId() == ID_WAKEUP_TIMER)
	{
		wxWakeUpIdle();
		return;
	}

	if (!m_okWin || !m_statusBar || !gkWindowSystem::getSingletonPtr()) return;

	if (m_animDlg)
		m_animDlg->update();

	const Ogre::RenderTarget::FrameStats& stats = gkWindowSystem::getSingleton().getMainWindow()->getRenderWindow()->getStatistics();

	m_statusBar->SetStatusText(wxString::Format("FPS: %.3f (%.3f/%.3f/%.3f) NumTris: %d NumBatches: %d",  
		stats.lastFPS, stats.avgFPS, stats.bestFPS, stats.worstFPS, (int)stats.triangleCount, (int)stats.batchCount));
}


void luMainFrame::OnIdle(wxIdleEvent& event)
{
	if (m_runtimeProcess)
	{
		if (m_runtimeLogFile->isModified())
		{
			m_logRuntime->AppendText(m_runtimeLogFile->readLog());
		}
	}
}

void luMainFrame::OnShowHelp(wxCommandEvent& WXUNUSED(event))
{
	//if (!m_helpWin) return;
	//if (!m_helpWin->IsVisible())
	//	m_helpWin->Show();
	//m_htmlHelp.DisplayContents();

	gkString helpFile = LuConfig.getHelpFilePath();
	if (!luFile::isFileExist(helpFile))
	{
		alert(wxString::Format("Can't found help file: %s", helpFile.c_str()));
		return;
	}

	wxString topic;
	if (m_bookMain && m_helpTopic)
	{
		luEdit* edit = dynamic_cast<luEdit*>(m_bookMain->GetPage(m_bookMain->GetSelection()));
		if (edit)
		{
			wxString sel = edit->getSelectionText();
			if (!sel.IsEmpty())
				topic = m_helpTopic->getHtmlFile(WX2GK(sel));
		}		
	}

	//TODO: change to wxHTML help
#ifdef WIN32
	if (!topic.IsEmpty())
		helpFile = WX2GK(wxString::Format("mk:@MSITStore:%s::/%s", helpFile.c_str(), topic.c_str()));

	if (!wxProcess::Exists(m_pidHelp))
	{
		m_pidHelp = wxExecute(wxString::Format("hh.exe %s", helpFile.c_str()));
		if (m_pidHelp < 0)
			alert("Can't execute hh.exe");
	}
#else
	alert("Not supprot yet!");
#endif
}


void luMainFrame::OnAsyncTermination(luProcess* process, int exitCode)
{
	wxASSERT(m_runtimeProcess == process);

	gkPrintf("Runtime process is terminated. pid: %ld exit: %d", process->GetPid(), exitCode);
	
	SAFE_DELETE(m_runtimeProcess);
	m_pidRuntime = 0;
	
}

void luMainFrame::OnRunRuntime(wxCommandEvent& WXUNUSED(event))
{
	if (wxProcess::Exists(m_pidRuntime))
	{
		alert("One more Runtime is still alive.");
		return;
	}

	wxASSERT(!m_runtimeProcess && "Runtime process is still alive?");

	if (!m_projFile)
	{
		alert("No project file to run.");
		return;
	}

	wxString luaFile = m_projFile->getStartLuaFile();
	if (luaFile.IsEmpty())
	{
		alert("First, Set the startup Lua file.");
		return;
	}

	wxString runtimePath = LuConfig.getRuntimePath();
	if (runtimePath.IsEmpty())
	{
		alert("Runtime path isn't set.");
		return;
	}

	if (!saveAllModifiedFilesInProject())
	{
		alert("Can't save modified file.");
		return;
	}
	
	wxString dirName = m_projFile->getProjDir();	
	gkPrintf("Working Directory: %s", WX2GK(dirName).c_str());
	wxString cmd = wxString::Format("\"%s\" -s %s -d \"%s\" -l %s", runtimePath, luaFile, dirName, m_runtimeLogFile->getFileName());

	wxString args;
	args = wxGetTextFromUser(cmd + "\nEnter arguments to launch: ", "Execute Runtime", "");
	cmd += " " + args;

	m_runtimeProcess = new luProcess(this, cmd);
		
	m_pidRuntime = wxExecute(cmd, wxEXEC_ASYNC, m_runtimeProcess);
	if (m_pidRuntime <= 0)
	{
		alert("Can't execute the Runtime Process.");
		
		SAFE_DELETE(m_runtimeProcess);
	}
	else
	{	
		m_logRuntime->Clear();
		m_runtimeLogFile->reset();
		selectPage(m_bookOutput, m_logRuntime);
	}
}

void luMainFrame::OnProjTreeItemActivated(wxTreeEvent& event)
{
	luProjTree* tree= m_projPanel->getTreeCtrl();
	wxTreeItemId itemId = event.GetItem();
    luProjTree::ItemData* item = (luProjTree::ItemData*)tree->GetItemData(itemId);	

	if (!item) return;
	int data = item->GetData();

	wxString name = item->GetName();	
	
	if (data == ITEM_BLEND || data == ITEM_LUA)
	{
		wxString path = getPathInProject(name, false);

		if (data == ITEM_BLEND)
			openBlendFile(path, false);
		else if (data == ITEM_LUA)
			openLuaFile(path, false);
	}
	else if (data == ITEM_SCENE)
	{
		changeScene(name);
	}
}

void luMainFrame::addPipedOutput(const wxString& str)
{
	if (m_logListener)
		m_logListener->log(str);
}

void luMainFrame::OnRunBlender(wxCommandEvent& WXUNUSED(event)) //TODO:open with Blender
{
	alert("Execute the Blender");
}

void luMainFrame::alert(const wxString& msg)
{	
	wxLogMessage(msg);
	wxMessageDialog dlg(this, msg, "Error", wxOK | wxICON_ERROR);
	dlg.ShowModal();
}

bool luMainFrame::addFileToProject(const wxString& pathName, int data)
{
	if (!m_projPanel || pathName.IsEmpty() || !m_projFile)
		return false;

	gkString fn = WX2GK(pathName);
	if (!m_isLoadingProjFile && !m_projFile->addFile(fn))
		return false;

	wxString relPath = m_projFile->getRelativePath(WX2GK(pathName));
	
	wxTreeItemId item;

	if (data == ITEM_BLEND)
	{
		gkBlendFile* blend = getOkApp()->getFirstBlendFile();
		item = m_projPanel->addBlendFile(relPath, blend);
	}
	else
	{
		item = m_projPanel->addFile(relPath, data);
	}

	return item.IsOk();
}

wxString luMainFrame::getPathInProject(const wxString& pathName, bool relPath)
{
	if (!m_projFile) return pathName;

	return relPath ? m_projFile->getRelativePath(WX2GK(pathName)) : m_projFile->getAbsolutePath(WX2GK(pathName));
}


bool luMainFrame::changeScene(const wxString& sceneName)
{
	if (!m_okWin) return false;

	bool ok = m_okWin->changeScene(sceneName);
	onSceneChanged();
	return ok;
}

bool luMainFrame::setProjectName(const wxString& name)
{
	if (!m_projFile) return false;

	if (name.IsEmpty() || name == m_projFile->getProjName()) return false;	

	if (!m_projPanel) return false;

	m_projFile->setProjName(WX2GK(name));
		
	m_projPanel->setProjectName(name);

	return true;
}

luEdit* luMainFrame::findLuEdit(const wxString& fileName)
{
	for (size_t i = 0; i < m_bookMain->GetPageCount(); i++)
	{
		luEdit* edit = dynamic_cast<luEdit*>(m_bookMain->GetPage(i));
		if (!edit) continue;

		if (wxFileName(edit->getFileName()).GetFullName() == fileName)
			return edit;
	}

	return NULL;
}

bool luMainFrame::gotoLuaSource(const wxString& luaFile, int lineNo)
{
	luEdit* edit = findLuEdit(luaFile);
	if (!edit) return false; //TODO: check unopened lua file in project
	
	edit->GotoLine(lineNo-1);
	edit->SetFocus();
	return true;
}

bool luMainFrame::selectGameObject(const wxString& name)
{
	if (!m_okWin) return false;

	gkGameObject *obj = m_okWin->selectObject(name);

	return obj != NULL;
}

void luMainFrame::OnGameObjectSelected(wxCommandEvent& event)
{
	if (!m_okWin) return;

	gkGameObject *obj = m_okWin->getSelectedObject();

	if (m_propsPanel) m_propsPanel->selectObject(obj);
}


void luMainFrame::showAnimDlg()
{
	if (!m_okWin) return;

	gkGameObject *obj = m_okWin->getSelectedObject();
	if (!obj) 
	{
		alert("First, Select an object for animation.");
		return;
	}

	if (!m_animDlg)
		m_animDlg = new luAnimDlg(this);
	
	m_animDlg->selectObject(obj, m_okWin->getScene());	
}

void luMainFrame::OnToolAnimManager(wxCommandEvent& event)
{
	showAnimDlg();
}

void luMainFrame::OnToolStateManager(wxCommandEvent& event)
{
	alert("StateManager");
}
