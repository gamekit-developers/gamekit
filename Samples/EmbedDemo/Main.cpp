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
#include "okUtils.h"

#define WIN_SIZE_X		640
#define WIN_SIZE_Y		480
#define LOG_BOX_HEIGHT  100
#define ID_STATUS_TIMER	wxID_HIGHEST + 100
#define ID_LOG_BOX		wxID_HIGHEST + 101
#define ID_LOG_CLEAR	wxID_HIGHEST + 102
#define ID_LOG_SAVE		wxID_HIGHEST + 103
#define ID_TIMER_PAUSE	wxID_HIGHEST + 104
#define STATUS_FPS      4
#define DEMO_BLEND		"logo_text.blend"
#define APP_TITLE		"OgreKit Embedding Demo"


class EmbedLog : public wxLog, public Ogre::LogListener
{
	wxListBox *m_logBox;
public:
	EmbedLog(wxListBox *lb) : m_logBox(lb) {}

	void log(const wxString &msg)
	{
		if (!m_logBox) return;
		m_logBox->Select(m_logBox->Append(msg));
	}

	virtual void messageLogged (const Ogre::String &message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName)
	{
		log(message.c_str());
	}

	virtual void  DoLogString (const wxString &msg, time_t timestamp) //wxlog
	{		
		log(wxDateTime(timestamp).FormatTime() + " : " + msg);
	}
};


class EmbedApp : public wxApp
{
	okApp*   m_okApp;
	wxString m_blend;

public:
    EmbedApp() : m_okApp(NULL) { }

    virtual bool OnInit();
    virtual int  OnExit();

	virtual void OnInitCmdLine(wxCmdLineParser&  parser);
	virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

	GK_INLINE okApp*   GetOkApp()           { return m_okApp; }
	GK_INLINE wxString GetBlendFile() const { return m_blend; }

	virtual void OnFatalException()
	{
		Exit();
	}

	virtual bool OnExceptionInMainLoop()  
	{
		return false;
	}
};


class EmbedFrame : public wxFrame
{
public:
    EmbedFrame();
	virtual ~EmbedFrame();

	GK_INLINE void SetAppTitle(const wxString &blend) { SetTitle(wxString::Format("%s - %s", APP_TITLE, blend)); }
	
private:
	void OnOpen(wxCommandEvent& event);
    void OnClose(wxCommandEvent& event);

	void OnSize(wxSizeEvent& event);
	void OnTimer(wxTimerEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnDropFiles(wxDropFilesEvent& event);

	void OnClearLog(wxCommandEvent& event);
	void OnSaveLog(wxCommandEvent& event);

	void OnPauseRun(wxCommandEvent& event);

	bool OpenBlendFile(const wxString& file);

    DECLARE_EVENT_TABLE()

	okWindow*    m_okWin;
	wxListBox*   m_logBox;	
	wxStatusBar* m_statusBar;
	EmbedLog*    m_logListener;
	wxTimer	     m_timer;
};


IMPLEMENT_APP(EmbedApp)

void EmbedApp::OnInitCmdLine(wxCmdLineParser&  parser)
{
	static const wxCmdLineEntryDesc cmdLineDesc[] =
	{
		{ wxCMD_LINE_OPTION, "f", "", "open the blend file.", wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },

		{ wxCMD_LINE_NONE }
	};

	parser.SetDesc(cmdLineDesc);
	parser.SetSwitchChars("-");
}

bool EmbedApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
	wxString file;
	if (parser.Found("f", &file))
		m_blend = file;		

	return true;
}

bool EmbedApp::OnInit()
{
    if (!wxApp::OnInit())
        return false;
	
	m_okApp = new okApp;

    new EmbedFrame();

    return true;
}

int EmbedApp::OnExit()
{
	if (m_okApp) 
	{
		m_okApp->uninit();
		delete m_okApp; 
		m_okApp = NULL;
	}

    return wxApp::OnExit();
}

BEGIN_EVENT_TABLE(EmbedFrame, wxFrame)
    EVT_MENU(wxID_CLOSE, EmbedFrame::OnClose)
	EVT_MENU(wxID_OPEN, EmbedFrame::OnOpen)
	EVT_MENU(ID_LOG_CLEAR, EmbedFrame::OnClearLog)
	EVT_MENU(ID_LOG_SAVE, EmbedFrame::OnSaveLog)
	EVT_TIMER(ID_STATUS_TIMER, EmbedFrame::OnTimer)	
	EVT_MENU(ID_TIMER_PAUSE, EmbedFrame::OnPauseRun)
	EVT_KEY_DOWN(EmbedFrame::OnKeyDown)
	EVT_DROP_FILES(EmbedFrame::OnDropFiles)
END_EVENT_TABLE()

EmbedFrame::EmbedFrame() : 
	wxFrame(NULL, wxID_ANY, APP_TITLE),
	m_okWin(NULL),
	m_timer(this, ID_STATUS_TIMER),
	m_statusBar(NULL),
	m_logListener(NULL),
	m_logBox(NULL)
{
//    SetIcon(wxICON(sample));

    wxMenu *menu = new wxMenu;
	menu->Append(wxID_OPEN);
	menu->AppendSeparator();
    menu->Append(wxID_CLOSE);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menu, "&File");

	menu = new wxMenu;
	menu->Append(ID_LOG_CLEAR, "&Clear Log");
	menu->Append(ID_LOG_SAVE,  "&Save Log");
	menuBar->Append(menu, "&Log");

	menu = new wxMenu;
	menu->Append(ID_TIMER_PAUSE, "&Pause");
	menuBar->Append(menu, "&Game");

    SetMenuBar(menuBar);

    wxStatusBar *sbar = CreateStatusBar();
	m_statusBar = sbar;

	
	m_logBox = new wxListBox(this, ID_LOG_BOX, wxPoint(0, WIN_SIZE_Y), wxSize(WIN_SIZE_X, LOG_BOX_HEIGHT));

	EmbedApp* app = (EmbedApp*)wxTheApp;
	okWindow* win = new okWindow(this);

	EmbedLog *logListener = new EmbedLog(m_logBox);
	delete wxLog::SetActiveTarget(logListener);

	Ogre::Log *log = Ogre::LogManager::getSingleton().getDefaultLog();
	if (log) log->addListener(logListener);

	m_logListener = logListener;
	wxString blend = app->GetBlendFile();
	if (blend.empty()) blend = DEMO_BLEND;

	wxLogMessage("Initing... %s ", blend);
	Ogre::Timer watch;
	if (win->Init(app->GetOkApp(), WX2GK(blend), "", WIN_SIZE_X, WIN_SIZE_Y)) 
	{		
		wxLogMessage("Elapsed time for initing: %.3f seconds", watch.getMilliseconds()/1000.0f);
		m_timer.Start(1000/STATUS_FPS);
		m_okWin = win;
	}
	else
	{
		delete win; win = NULL;
		wxLogMessage("Error - Can't init the Ogrekit.");
	}

	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

	if (m_okWin) 
	{
		vbox->Add(m_okWin, wxSizerFlags(1).Expand());
		m_okWin->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(EmbedFrame::OnKeyDown), NULL, this);
	}

	if (m_logBox) 
	{
		vbox->Add(m_logBox, wxSizerFlags().Expand());
		m_logBox->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(EmbedFrame::OnKeyDown), NULL, this);
	}

	SetClientSize(WIN_SIZE_X, WIN_SIZE_Y + sbar->GetSize().GetHeight() + LOG_BOX_HEIGHT);

	m_okWin->SetCanFocus(true);
	m_okWin->SetFocus();

	DragAcceptFiles(true);
	SetSizerAndFit(vbox);

    Show();       
}

EmbedFrame::~EmbedFrame()
{
	Ogre::Log *log = Ogre::LogManager::getSingleton().getDefaultLog();
	if (log) log->removeListener(m_logListener);
	delete wxLog::SetActiveTarget(NULL); //delete m_logListener
}

void EmbedFrame::OnKeyDown(wxKeyEvent& event)
{
	if (event.GetKeyCode() == WXK_TAB && m_okWin)
		m_okWin->SetFocus();
	else
		event.Skip();
}

void EmbedFrame::OnDropFiles(wxDropFilesEvent& event)
{
	if (event.GetNumberOfFiles() < 0) return;
	wxString file = *event.GetFiles();
	if (wxFileName(file).GetExt().Lower() != "blend") return;
	
	OpenBlendFile(file);
}

bool EmbedFrame::OpenBlendFile(const wxString &file)
{
	if (!m_okWin) return false;

	EmbedApp* app = (EmbedApp*)wxTheApp;
	wxSize size = m_okWin->GetSize();

	if (file.CmpNoCase(m_okWin->getApp()->getBlendFileName().c_str()) == 0)
	{
		wxLogMessage("Already loaded.");
		return true;
	}

	wxLogMessage("Loading... %s ", file);

	Ogre::Timer watch;
	if (!m_okWin->Load(WX2GK(file)))
	{
		wxString msg = wxString::Format("Error - Can't load the blend file: %s", file);
		wxLogMessage(msg);
		wxMessageDialog *dlg= new wxMessageDialog(NULL, 
			msg, "Error", wxOK | wxICON_ERROR);
		dlg->ShowModal();

		return false;
	}

	wxLogMessage("Elapsed time for loading: %.3f seconds", watch.getMilliseconds()/1000.0f);

	SetAppTitle(wxFileName::FileName(file).GetFullName());

	m_okWin->SetFocus();
	m_okWin->SendSizeEvent();

	GetMenuBar()->SetLabel(ID_TIMER_PAUSE, m_okWin->isRunnigGameLoop() ? "&Pause" : "&Resume");

	return true;
}

void EmbedFrame::OnPauseRun(wxCommandEvent& event)
{
	if (!m_okWin) return;

	if (m_okWin->isRunnigGameLoop())
		m_okWin->stopGameLoop();
	else
		m_okWin->startGameLoop();

	GetMenuBar()->SetLabel(ID_TIMER_PAUSE, m_okWin->isRunnigGameLoop() ? "&Pause" : "&Resume");

	wxLogMessage("%s", m_okWin->isRunnigGameLoop() ? "Running" :  "Paused");
}

void EmbedFrame::OnClearLog(wxCommandEvent& WXUNUSED(event))
{
	if (!m_logBox) return;

	m_logBox->Clear();
}


void EmbedFrame::OnSaveLog(wxCommandEvent& WXUNUSED(event))
{
	if (!m_logBox) return;

	wxFileDialog *dlg = new wxFileDialog(this, wxFileSelectorPromptStr,  "",  "", "Text File (*.txt)|*.txt", wxFD_SAVE);
	if (dlg->ShowModal() == wxID_OK)
	{
		 wxString fileName = dlg->GetPath();
		 wxTextFile file(fileName);
		 
		 bool ok = wxFile::Exists(fileName) ? file.Open() : file.Create();

		 if (ok)
		 {
			 file.Clear();
			 wxArrayString lines = m_logBox->GetStrings();
			 for (size_t i = 0; i < lines.GetCount(); i++)
			 {
				 file.AddLine(lines[i]);
			 }
			 file.Write();
			 wxLogMessage("Log file is saved: %s", fileName);
		 }
		 else
		 {
			 wxLogMessage("Can't open log file: %s", fileName);
		 }		 
	}
}


void EmbedFrame::OnOpen(wxCommandEvent& WXUNUSED(event))
{
	if (m_timer.IsRunning())
		m_timer.Stop();

	wxFileDialog *dlg = new wxFileDialog(this);
	if (dlg->ShowModal() == wxID_OK)
	{
		 wxString fileName = dlg->GetPath();
		 OpenBlendFile(fileName);
	}
}

void EmbedFrame::OnClose(wxCommandEvent& WXUNUSED(event))
{
	m_timer.Stop();
    Close(true);
}

void EmbedFrame::OnTimer(wxTimerEvent& WXUNUSED(event))
{
	if (!m_okWin || !m_statusBar) return;

	const Ogre::RenderTarget::FrameStats& stats = gkWindowSystem::getSingleton().getMainWindow()->getStatistics();

	m_statusBar->SetStatusText(wxString::Format("FPS: %.3f (%.3f/%.3f/%.3f) NumTris: %d NumBatches: %d",  
		stats.lastFPS, stats.avgFPS, stats.bestFPS, stats.worstFPS, stats.triangleCount, stats.batchCount));

	if (wxTheApp->IsActive() && m_okWin->isRunnigGameLoop() && !m_okWin->HasFocus())
		m_okWin->SetFocusFromKbd ();
}
