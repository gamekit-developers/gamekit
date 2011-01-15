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

#ifndef _okWin_h_
#define _okWin_h_

class okApp;
class okCamera;

enum OKWIN_COMMAND_ID
{
	ID_OKWIN_REFESH_TIMER = wxID_HIGHEST + 1,
	ID_OKWIN_OBJECT_SELECTED
};

class okWindow : public wxWindow
{
public:
	okWindow(wxWindow* parent=NULL, const wxSize& size = wxDefaultSize);
	~okWindow();

	gkString getNativeHandle();

	bool init(okApp* app, gkWindow* win);
	void uninit();	
	void resize();

	okApp* getApp() { return m_okApp; }
	okCamera* getCamera() { return m_okCam; }

	bool isRunnigGameLoop() { return m_timer.IsRunning(); }
	void startGameLoop();
	void stopGameLoop()  { if (m_timer.IsRunning()) m_timer.Stop();   }

	void alert(const wxString& msg);

	void setRenderOnly(bool renderOnly);
	void setEanbleCameraControl(bool enable) { m_enableCameraControl = enable; }
	void setPlayAnimSelObj(bool enable) { m_playAnimSelObj = enable; }

	void setCameraPolyMode(Ogre::PolygonMode polyMode);

	bool getShowBoundingBox();
	void showBoundingBox(bool show);
	void toggleShowBoundingBox() { showBoundingBox(!getShowBoundingBox()); }

	bool getShowAxis();
	void showAxis(bool show);
	void toggleShowAxis() { showAxis(!getShowAxis()); }

	gkScene* getScene() { return m_scene; }
	bool loadScene(const gkString& blend="", const gkString& scene="", bool ignoreCache=false);
	bool mergeScene(const gkString& blend="", const gkString& scene="", bool ignoreCache=false);
	void unloadScene();

	void resetScene(); //unload & create empty scene
	bool changeScene(const wxString& sceneName);

	gkGameObject* getSelectedObject() { return m_selObj; }
	gkGameObject* selectObject(const wxString& objName);
	gkGameObject* pickObject(int x, int y);

private:
	DECLARE_EVENT_TABLE()
	DECLARE_DYNAMIC_CLASS_NO_COPY(okWindow)

	void OnExit(wxCloseEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnTimer(wxTimerEvent& event);
	void OnEraseBackground(wxEraseEvent& event);

	void OnLButtonDown(wxMouseEvent& event);
	void OnLButtonUp(wxMouseEvent& event);
	void OnRButtonDown(wxMouseEvent& event);
	void OnRButtonUp(wxMouseEvent& event);
	void OnMButtonDown(wxMouseEvent& event);
	void OnMButtonUp(wxMouseEvent& event);
	void OnMouseWheel(wxMouseEvent& event);
	void OnMouseMove(wxMouseEvent& event);
	void OnSetFocus(wxFocusEvent& event);
	void OnKillFocus(wxFocusEvent& event);
	void OnEnterWindow(wxMouseEvent& event);
	void OnLeaveWindow(wxMouseEvent& event);

	void setupRenderWindow();
	void drawRenderWindow();

	okApp* m_okApp;
	wxTimer m_timer;

	wxPoint m_posMouse;
	bool m_renderOnly; //don't call engine update()
	bool m_enableCameraControl;
	bool m_playAnimSelObj;

	Ogre::RenderWindow *m_renderWindow;
	Ogre::Camera* m_camera;
	Ogre::SceneManager* m_sceneMgr;
	gkGameObject* m_selObj;
	okCamera* m_okCam;
	gkScene* m_scene;
	gkWindow* m_win;

	bool m_LClick;
	bool m_MClick;
	bool m_RClick;

	void selectObject(gkGameObject* obj);
};

#endif //_okWin_h_