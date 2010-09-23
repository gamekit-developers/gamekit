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
#ifndef _nsNodeCanvas_h_
#define _nsNodeCanvas_h_

#include "nsCommon.h"
#include "nsSingleton.h"
#include "nsMath.h"
#include <wx/glcanvas.h>


class nsNodeCanvas : public wxGLCanvas
{
protected:


	wxGLContext*     m_context;
	NSvec2          m_size;
	NSvec2          m_mousePos, m_lastPos, m_clickedPos, m_zoomLimit;
	NSrect          m_viewport, m_projection;
	bool            m_viewportDirty, m_firstPaint, m_hasCapture;
	bool            m_grabCapture, m_grabbed, m_dragSelection;
	NSvec2          m_dragFrom, m_dragTo;
	nsNodeTree*      m_tree;
	nsNodes         m_renderList, m_captured;
	nsSocket*        m_clickedSocket;

	void grabCanvas(bool doit, bool releaseSelection = true);


	// events

	void            eraseEvent(wxEraseEvent& evt);
	void            paintEvent(wxPaintEvent& evt);
	void            sizeEvent(wxSizeEvent& evt);
	void            focusLostEvent(wxMouseCaptureLostEvent& evt);

	// key events
	void            keyPressEvent(wxKeyEvent& evt);

	// click events
	void            leftClickEvent(wxMouseEvent& evt);
	void            middleClickEvent(wxMouseEvent& evt);
	void            rightClickEvent(wxMouseEvent& evt);
	void            leftDblClickEvent(wxMouseEvent& evt);


	// movements
	void            motionEvent(wxMouseEvent& evt);
	void            wheelEvent(wxMouseEvent& evt);

	// hit testing
	nsNode*          hitTestNode(const NSvec2& projClick);
	nsSocket*        hitTestSocket(const NSvec2& projClick);
	void            bringToFront(nsNode* node);

	// capturing
	void            captureNode(nsNode* node);
	void            captureNodes(const NSrect& rect);
	void            releaseCapture(void);

	// update mouse coords outside of mouse events
	void            updateMouseCo(void);


	// for sending events to other windows
	void            sendEvent(int id, nsNode* node);
	void            sendEvent(int id, nsSocket* sock);
	void            sendEvent(int id);

	// update window state
	void            redraw(void);


public:
	nsNodeCanvas(wxWindow* parent, nsNodeTree* tree);
	virtual ~nsNodeCanvas();

	// copied down from the workspace
	void grabCapturedEvent(wxCommandEvent& evt);
	void deleteCapturedEvent(wxCommandEvent& evt);
	void selectAllEvent(wxCommandEvent& evt);
	void copyEvent(wxCommandEvent& evt);
	void pasteEvent(wxCommandEvent& evt);
	void cutEvent(wxCommandEvent& evt);
	void duplicateEvent(wxCommandEvent& evt);


	NSvec2          getZoom(void);
	void            setZoom(NSfloat zd);
	void            addNode(nsNode* nd);
	NSvec2          projectPoint(const NSvec2& v);

	nsNodeTree*      getTree(void)  {return m_tree;}


	// static tables
	DECLARE_EVENT_TABLE();

};

#endif//_nsNodeCanvas_h_
