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
#include "okCamera.h"


#define ROT_SPEED			0.01
#define ZOOM_SPEED			0.01
#define MOV_SPEED			0.01
#define MAX_CAM_RADIUS		100
#define DEFAULT_CAM_RADIUS	10

#define RAY_LENGTH			100

#define FPS					60


IMPLEMENT_DYNAMIC_CLASS(okWindow, wxWindow)

okWindow::okWindow(wxWindow* parent, const wxSize& size) 
	:	wxWindow(parent, -1, wxDefaultPosition, size),
		m_timer(this, 	ID_OKWIN_REFESH_TIMER),
		m_okApp(NULL),	
		m_okCam(NULL),
		m_renderWindow(NULL),
		m_camera(NULL),
		m_sceneMgr(NULL),
		m_renderOnly(false),
		m_enableCameraControl(false),
		m_LClick(false),
		m_MClick(false),
		m_RClick(false),
		m_posMouse(-1,-1),
		m_selObj(NULL),
		m_scene(NULL),
		m_win(NULL),
		m_playAnimSelObj(false)
{ 
}

okWindow::~okWindow()
{
	uninit();
}

BEGIN_EVENT_TABLE(okWindow, wxWindow)
	EVT_SIZE(okWindow::OnSize)
	EVT_PAINT(okWindow::OnPaint)
	EVT_ERASE_BACKGROUND(okWindow::OnEraseBackground)
	EVT_TIMER(ID_OKWIN_REFESH_TIMER, okWindow::OnTimer)
	
	EVT_LEFT_DOWN(okWindow::OnLButtonDown)
	EVT_LEFT_UP(okWindow::OnLButtonUp)
	EVT_MIDDLE_DOWN(okWindow::OnMButtonDown)
	EVT_MIDDLE_UP(okWindow::OnMButtonUp)
	EVT_RIGHT_DOWN(okWindow::OnRButtonDown)
	EVT_RIGHT_UP(okWindow::OnRButtonUp)
	EVT_MOUSEWHEEL(okWindow::OnMouseWheel)
	EVT_MOTION(okWindow::OnMouseMove)

	EVT_SET_FOCUS(okWindow::OnSetFocus)
	EVT_KILL_FOCUS(okWindow::OnKillFocus)
	EVT_ENTER_WINDOW(okWindow::OnEnterWindow)
	EVT_LEAVE_WINDOW(okWindow::OnLeaveWindow)
END_EVENT_TABLE()

void okWindow::OnSetFocus(wxFocusEvent& event)
{
}

void okWindow::OnKillFocus(wxFocusEvent& event)
{
}


void okWindow::OnEnterWindow(wxMouseEvent& event)
{
	SetFocus(); //catch key event
}

void okWindow::OnLeaveWindow(wxMouseEvent& event)
{
}


void okWindow::OnLButtonDown(wxMouseEvent& event)
{
	m_LClick = true;
	m_posMouse = event.GetLogicalPosition(wxClientDC(this));
}


void okWindow::OnLButtonUp(wxMouseEvent& event)
{	
	m_LClick = false;
}

void okWindow::OnRButtonDown(wxMouseEvent& event)
{	
	m_RClick = true;
	m_posMouse = event.GetLogicalPosition(wxClientDC(this));

	if (m_enableCameraControl)
	{
		wxPoint pos = event.GetPosition();
		gkGameObject* obj = pickObject(pos.x, pos.y);
		if (obj)
			selectObject(obj);
	}
}

void okWindow::OnRButtonUp(wxMouseEvent& event)
{	
	m_RClick = false;
}

void okWindow::OnMButtonDown(wxMouseEvent& event)
{
	m_MClick = true;
	m_posMouse = event.GetLogicalPosition(wxClientDC(this));
}

void okWindow::OnMButtonUp(wxMouseEvent& event)
{	
	m_MClick = false;
}

void okWindow::OnMouseWheel(wxMouseEvent& event)
{	
	int dir = event.GetWheelRotation();

	if (m_enableCameraControl && m_okCam)
	{
		m_okCam->zoom(ZOOM_SPEED * -dir);
	}
}



void okWindow::OnMouseMove(wxMouseEvent& event)
{	
	if (m_enableCameraControl && m_okCam)
	{
		wxPoint pos = event.GetLogicalPosition(wxClientDC(this));
		
		wxPoint posRel = pos - m_posMouse;

		m_posMouse = pos;

		if (m_LClick)		
			m_okCam->rotate(gkRadian(-posRel.x * ROT_SPEED), gkRadian(-posRel.y * ROT_SPEED));		
		else if (m_MClick)
			m_okCam->setTargetPostion(m_okCam->getOrientation() * gkVector3(-posRel.x * MOV_SPEED, posRel.y * MOV_SPEED, 0), true);
	}
}

void okWindow::OnSize(wxSizeEvent& event)
{	
	resize();
}


void okWindow::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	wxPaintDC dc(this);

	drawRenderWindow();
}

void okWindow::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{

}

void okWindow::startGameLoop()
{ 
	if (!m_timer.IsRunning()) 
		m_timer.Start(1000/FPS); 
}

gkString okWindow::getNativeHandle()
{
	return getNativeWinHandleFromWxWin(this);
}

bool okWindow::init(okApp* app, gkWindow* win)
{
	if (m_okApp || !app) return false;

	const wxSize& size = GetClientSize(); 

	if (!win)
	{		
		gkWindow* win = app->createWindow(getNativeHandle(), size.GetWidth(), size.GetHeight());
		if (!win) return false;

		m_win = win;
	}
	else 
	{
		m_win = win;
		m_scene = m_win->getRenderScene();
		if (m_scene)
		{
			setupRenderWindow();			
			startGameLoop();
		}
	}

	GK_ASSERT(m_win);

	m_okApp = app;

	return true;
}

void okWindow::uninit()
{
	if (!m_okApp) return;

	unloadScene();

	m_okApp->destroyWindow(m_win);
	m_win = NULL;
	m_okApp = NULL;
}

void okWindow::setRenderOnly(bool renderOnly) 
{ 
	m_renderOnly = renderOnly;
	if (m_scene)
		m_scene->setUpdateFlags(0);
}


void okWindow::setupRenderWindow()
{
	GK_ASSERT(m_win && m_scene);	

	m_renderWindow = m_win->getRenderWindow();
		
	if (!m_renderWindow) return;
	
	
	//if (!m_scene) m_scene = m_okApp->getFirstScene(); GK_ASSERT(m_scene);
	gkCamera* cam = m_scene->getMainCamera(); GK_ASSERT(cam);
	m_camera = cam->getCamera(); GK_ASSERT(m_camera);
	m_sceneMgr = m_camera->getSceneManager(); GK_ASSERT(m_sceneMgr);

	if (m_enableCameraControl)
	{
		if (!m_okCam) 
			m_okCam = new okCamera(m_camera->getSceneManager(), NULL, okCamera::MODE_TARGET);
		
		m_okCam->attachCamera(m_camera);
		m_okCam->reset(gkVector3(0,0,DEFAULT_CAM_RADIUS));		
	}
}

void okWindow::unloadScene()
{
	stopGameLoop();

	delete m_okCam; m_okCam = NULL;

	if (m_okApp && m_scene)
	{
		m_okApp->unloadScene(m_scene);
		m_scene = NULL;
	}

	m_renderWindow = NULL;
	m_camera = NULL;	
	m_sceneMgr = NULL;
	m_selObj = NULL;
}


void okWindow::resize()
{
	if (!m_renderWindow)  return;
	wxSize size = GetClientSize();

	m_renderWindow->resize(size.GetWidth(), size.GetHeight());
	m_renderWindow->windowMovedOrResized();
	if (m_camera && size.GetHeight() != 0)
		m_camera->setAspectRatio(gkScalar(size.GetWidth())/size.GetHeight());

	if (!isRunnigGameLoop())
		m_renderWindow->update();	
}

bool okWindow::loadScene(const gkString& blend, const gkString& scene, bool ignoreCache)
{
	if (!m_okApp || blend.empty()) return false;

	unloadScene();

	GK_ASSERT(m_win);	
	
	m_scene = m_okApp->loadScene(m_win, blend, scene, ignoreCache);
	
	if (!m_scene)
		return false;

	if (m_renderOnly)
		m_scene->setUpdateFlags(0);

	setupRenderWindow();
	startGameLoop();

	return true;
}

bool okWindow::mergeScene(const gkString& blend, const gkString& scene, bool ignoreCache)
{
	if (!m_okApp || !m_scene) return false;

	GK_ASSERT(m_win);
	
	if (!m_okApp->mergeScene(m_scene, blend, scene, ignoreCache))
		return false;

	if (m_renderOnly)
		m_scene->setUpdateFlags(0);

	setupRenderWindow();
	startGameLoop();

	return true;
}


bool okWindow::changeScene(const wxString& sceneName)
{
	if (!m_scene) return false;

	if (m_scene->getName() == sceneName)
		return true;

	unloadScene();

	if (!m_okApp->changeScene(m_scene, WX2GK(sceneName)))
		return false;

	if (m_renderOnly)
		m_scene->setUpdateFlags(0);

	setupRenderWindow();
	startGameLoop();
	resize();

	return true;
}

void okWindow::drawRenderWindow()
{
	if (!m_renderWindow) return;
	
	Ogre::Root::getSingleton()._fireFrameStarted();
	m_renderWindow->update();
	Ogre::Root::getSingleton()._fireFrameEnded();
}

void okWindow::OnTimer(wxTimerEvent& event)
{
	if (!m_okApp) return;
	
	try 
	{		
		m_okApp->step();

		if (m_okCam) m_okCam->updateHelper();
	
	} 
	catch (Ogre::Exception& e) 
	{
		stopGameLoop();
		alert(wxString::Format("%s", e.getFullDescription().c_str()));
	} 
	catch (std::exception& e) 
	{
		stopGameLoop();
		alert(e.what());
	} 
	catch (...) 
	{
		stopGameLoop();
		alert("Unknown exception raised.");
	}

#ifndef WIN32
	Refresh();
#endif
}

void okWindow::alert(const wxString& msg)
{
	wxMessageDialog dlg(this, msg, "Alert");
	dlg.ShowModal();
}

void okWindow::setCameraPolyMode(Ogre::PolygonMode polyMode)
{
	if (m_okCam)
		m_okCam->setPolygonMode(polyMode);
}

bool okWindow::getShowBoundingBox()
{
	return m_sceneMgr ? m_sceneMgr->getShowBoundingBoxes() : false;
}

void okWindow::showBoundingBox(bool show)
{
	if (m_sceneMgr) 
		m_sceneMgr->showBoundingBoxes(show);
}

bool okWindow::getShowAxis()
{
	return m_okCam ? m_okCam->getShowAxis() : false;
}

void okWindow::showAxis(bool show)
{
	if (m_okCam)
		m_okCam->showAxis(show);
}

void okWindow::resetScene()
{
	if (!m_okApp) return;

	delete m_okCam; m_okCam = NULL;

	m_okApp->unloadAllScenes();

	m_scene = m_okApp->createEmptyScene();

	setupRenderWindow();
	startGameLoop();

}

void okWindow::selectObject(gkGameObject* obj)
{
	if (m_selObj && m_selObj->getNode())
	{
		m_selObj->getNode()->showBoundingBox(false);
		m_selObj = NULL;
	}

	if (!obj) return;

	Ogre::SceneNode* node = obj->getNode();
	if (node) node->showBoundingBox(true);

	m_selObj = obj;

	wxCommandEvent evt(wxEVT_NULL, ID_OKWIN_OBJECT_SELECTED);
	wxTheApp->GetTopWindow()->GetEventHandler()->ProcessEvent(evt);
}

gkGameObject* okWindow::selectObject(const wxString& objName)
{	
	if (!m_scene) return NULL;

	gkGameObject* obj = m_scene->getObject(WX2GK(objName));	

	selectObject(obj);
	
	return obj;
}


gkGameObject* okWindow::pickObject(int x, int y)
{
	if (!m_camera) return NULL;

	wxSize size = GetClientSize();
	float px = float(x)/size.GetWidth(), py = float(y)/size.GetHeight();

	Ogre::Ray ray = m_camera->getCameraToViewportRay(px, py);
	ray.setDirection(ray.getDirection()*RAY_LENGTH);
	gkRayTest rayTest;

	if (rayTest.collides(ray))
	{
		btCollisionObject* pCol = rayTest.getCollisionObject();
		gkPhysicsController* pObj = static_cast<gkPhysicsController*>(pCol->getUserPointer());
		gkRigidBody* pickedBody = dynamic_cast<gkRigidBody*>(pObj);

		if (pickedBody)
		{
			return pickedBody->getObject();
		}
	}

	return NULL;
}