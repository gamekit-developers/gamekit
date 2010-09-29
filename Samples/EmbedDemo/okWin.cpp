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


#define FPS 60
#define ID_REFESH_TIMER wxID_HIGHEST + 1

okWindow::okWindow(wxWindow* parent) : 
	wxWindow(parent, -1),
	m_timer(this, ID_REFESH_TIMER),
	m_okApp(NULL),	
	m_renderWindow(NULL),	
	m_viewport(NULL),
	m_camera(NULL)
{
	
}

okWindow::~okWindow()
{
	stopGameLoop();
}

BEGIN_EVENT_TABLE(okWindow, wxWindow)
	EVT_SIZE(okWindow::OnSize)
	EVT_PAINT(okWindow::OnPaint)
	EVT_ERASE_BACKGROUND(okWindow::OnEraseBackground)
	EVT_TIMER(ID_REFESH_TIMER, okWindow::OnTimer)
	EVT_RIGHT_DOWN(okWindow::OnLButtonDown)
END_EVENT_TABLE()

void okWindow::OnLButtonDown(wxMouseEvent& event)
{
	SetFocus();
}

void okWindow::OnSize(wxSizeEvent& event)
{
	wxSize size = event.GetSize();
	if (m_renderWindow) 
	{
		m_renderWindow->resize(size.GetWidth(), size.GetHeight());
		m_renderWindow->windowMovedOrResized();
		if (size.GetHeight() != 0)
			m_camera->setAspectRatio(gkScalar(size.GetWidth())/size.GetHeight());

		if (!isRunnigGameLoop())
			m_renderWindow->update();
	}
}

void okWindow::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	wxPaintDC dc(this);

	if (!m_renderWindow) return;
	
	m_renderWindow->update();
}

void okWindow::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{

}

void okWindow::startGameLoop()
{ 
	if (!m_timer.IsRunning()) 
		m_timer.Start(1000/FPS); 
}

bool okWindow::Init(okApp* app, const gkString& blend, const gkString& cfg, int winSizeX, int winSizeY)
{
	if (m_okApp || !app) return false;


	gkString handle = getNativeWinHandleFromWxWin(this);
	if (handle.empty()) 
		return false;

	if (!app->init(blend, cfg, handle, winSizeX, winSizeY))
		return false;

	m_okApp = app;
	SetSize(wxSize(winSizeX, winSizeY));

	UpdateRenderWindow();
			
	startGameLoop();

	return true;
}

void okWindow::UpdateRenderWindow()
{
	GK_ASSERT(m_okApp);

	m_renderWindow = gkWindowSystem::getSingleton().getMainWindow(); 
	if (!m_renderWindow) return;
	m_viewport = m_renderWindow->getViewport(0);	
	m_camera = m_viewport->getCamera();
}

void okWindow::Uninit()
{
	if (!m_okApp) return;

	stopGameLoop();
	
	m_renderWindow = NULL;
	m_viewport = NULL;
	m_camera = NULL;
}

bool okWindow::Load(const gkString& blend, const gkString& cfg)
{
	if (!m_okApp) return false;

	Uninit();

	if (!m_okApp->load(blend))
		return false;

	UpdateRenderWindow();
	startGameLoop();

	return true;
}

void okWindow::OnTimer(wxTimerEvent& event)
{
	if (!m_okApp) return;
	
	m_okApp->step();
	Refresh();
}
