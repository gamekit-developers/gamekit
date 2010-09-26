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

class okWindow : public wxWindow
{
public:
	okWindow(wxWindow* parent=NULL);
	~okWindow();

	bool Init(okApp* app, const gkString& blend="", const gkString& cfg="", int winSizeX=800, int winSizeY=600);
	void Uninit();
	bool Load(const gkString& blend="", const gkString& cfg="");

	GK_INLINE okApp* getApp() { return m_okApp; }

	GK_INLINE bool isRunnigGameLoop() { return m_timer.IsRunning(); }
	void startGameLoop();
	GK_INLINE void stopGameLoop()  { if (m_timer.IsRunning()) m_timer.Stop();   }
private:
	DECLARE_EVENT_TABLE()

	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnTimer(wxTimerEvent& event);
	void OnEraseBackground(wxEraseEvent& event);
	void OnLButtonDown(wxMouseEvent& event);

	void UpdateRenderWindow();

	okApp* m_okApp;
	wxTimer m_timer;

	Ogre::RenderWindow *m_renderWindow;
	Ogre::Viewport* m_viewport;
	Ogre::Camera* m_camera;
};

#endif //_okWin_h_