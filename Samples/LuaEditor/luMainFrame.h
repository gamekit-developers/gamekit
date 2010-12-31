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


#ifndef _luMainFrame_h_
#define _luMainFrame_h_

class okApp;
class okWindow;
class luLog;
class luLogFile;
class luProjPanel;
class luPropsPanel;
class luProjectFile;
class luProcess;
class luEdit;
class luHhkFile;
class luLogEdit;
class luLogBox;
class luConsoleEdit;
class luAnimDlg;

class luMainFrame : public wxFrame
{
public:
    luMainFrame();
	virtual ~luMainFrame();

	void setAppTitle(const wxString &blend);
	void setFocusEdit(luEdit* edit, bool setFocus=true);

	bool openFileByType(const wxString& file, bool exceptProj=false);
	bool openProjFile(const wxString& file);
	bool openBlendFile(const wxString& file, bool addToProject = true);
	bool openLuaFile(const wxString& file, bool addToProject = true);

	bool createNewProject(const wxString& name, const wxString& projFileName, int projTemplate);
	bool saveProject(bool closeOpendFile=false);

	void alert(const wxString& msg);

	luEdit* getFocusEdit() { return m_focusEdit; }
	luEdit* findLuEdit(const wxString& fileName);
	luProjectFile* getProjectFile() { return m_projFile; }
	wxString getPathInProject(const wxString& pathName, bool relPath);
	bool saveAllModifiedFilesInProject();

	bool changeStartupFileInProject(const wxString& pathName);
	bool removeFileInProject(const wxString& pathName);

	bool isProjectCreated() {return m_projFile != NULL; }

	bool changeScene(const wxString& sceneName);

	okApp* getOkApp();
	okWindow* getOkWin() { return m_okWin; }

	bool setProjectName(const wxString& name);

	bool gotoLuaSource(const wxString& luaFile, int lineNo);
	bool selectGameObject(const wxString& name);

	void onSceneChanged();
	void showAnimDlg();
	void onAnimDlgClosed() { m_animDlg = NULL; }
private:

	void OnOpen(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
	void OnEdit(wxCommandEvent& event);
	void OnSaveAll(wxCommandEvent& event);

	void OnClose(wxCloseEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnTimer(wxTimerEvent& event);
	void OnIdle(wxIdleEvent& event);
	void OnDropFiles(wxDropFilesEvent& event);	
	void OnClosePage(wxAuiNotebookEvent& event);

	void OnClearLog(wxCommandEvent& event);
	void OnSaveLog(wxCommandEvent& event);

	void OnPauseRun(wxCommandEvent& event);
	void OnShowHelp(wxCommandEvent& event);

	void OnOpenProjFile(wxCommandEvent& event);
	void OnOpenBlendFile(wxCommandEvent& event);
	void OnOpenLuaFile(wxCommandEvent& event);

	void OnProjStartupFile(wxCommandEvent& event);
	void OnProjRemoveFile(wxCommandEvent& event);

	void OnNewProjFile(wxCommandEvent& event);
	void OnNewBlendFile(wxCommandEvent& event);
	void OnNewLuaFile(wxCommandEvent& event);

	void OnRunRuntime(wxCommandEvent& event);
	void OnRunBlender(wxCommandEvent& event); //open with Blender

	void OnShowProjWin(wxCommandEvent& event);
	void OnShowLogWin(wxCommandEvent& event);
	void OnShowPropsWin(wxCommandEvent& event);

	void OnToolAnimManager(wxCommandEvent& event);
	void OnToolStateManager(wxCommandEvent& event);

	void OnWinPagePrev(wxCommandEvent& event);
	void OnWinPageNext(wxCommandEvent& event);

	void OnChangeCameraPolyMode(wxCommandEvent& event);
	void OnChangeCameraDirection(wxCommandEvent& event);
	void OnShowPhysicsDebug(wxCommandEvent& event);
	void OnShowBoundingBox(wxCommandEvent& event);
	void OnShowAxis(wxCommandEvent& event);   

	void OnProjTreeItemActivated(wxTreeEvent& event);

	void OnGameObjectSelected(wxCommandEvent& event);


	wxAuiManager*		m_aui;
	okWindow*			m_okWin;
	luLogBox*			m_logBox;
	luLogEdit*			m_logRuntime;
	luConsoleEdit*		m_consoleEdit;
	wxStatusBar*		m_statusBar;
	luLog*				m_logListener;
	wxTimer				m_timer;
	

	long				m_pidHelp;
	long				m_pidRuntime;

	luProjPanel*		m_projPanel;
	luPropsPanel*		m_propsPanel;
	luEdit*				m_focusEdit;
	luHhkFile*			m_helpTopic;
	luAnimDlg*			m_animDlg;
	
	wxAuiNotebook*		m_bookMain;
	wxAuiNotebook*		m_bookOutput;
	wxAuiNotebook*		m_bookProps;
	
	luProjectFile*		m_projFile;
	bool				m_isLoadingProjFile;
	bool				m_isLoadedBlendFile;

	//wxHtmlHelpController m_htmlHelp;
	bool addFileToProject(const wxString& pathName, int data);

	void setupMenu();
	void setupToolbar();
	void setupWindows();
	void showHidePanel(wxWindow* panel);

	bool saveProjFile();

	void refreshInspPanel();
	
	luProcess* m_runtimeProcess;

	luLogFile* m_runtimeLogFile;

	wxString m_blendFile;
public:
	void addPipedOutput(const wxString& str);

	void OnAsyncTermination(luProcess* process, int exitCode);

	DECLARE_EVENT_TABLE()

};

#endif //_luMainFrame_h_