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
#include "nsNodeCanvas.h"
#include "nsWorkspace.h"
#include "nsRenderSystem.h"
#include "nsMainWindow.h"
#include "nsNodeManager.h"
#include "nsPropertyPage.h"
#include "nsClipboard.h"


#include <wx/dcclient.h>
#include <wx/menu.h>



BEGIN_EVENT_TABLE( nsNodeCanvas, wxGLCanvas )
	EVT_MOUSE_CAPTURE_LOST(nsNodeCanvas::focusLostEvent)
	EVT_PAINT(nsNodeCanvas::paintEvent)
	EVT_ERASE_BACKGROUND(nsNodeCanvas::eraseEvent)
	EVT_LEFT_DOWN(nsNodeCanvas::leftClickEvent)
	EVT_LEFT_UP(nsNodeCanvas::leftClickEvent)
	EVT_LEFT_DCLICK(nsNodeCanvas::leftDblClickEvent)
	EVT_MIDDLE_DOWN(nsNodeCanvas::middleClickEvent)
	EVT_MIDDLE_UP(nsNodeCanvas::middleClickEvent)
	EVT_RIGHT_UP(nsNodeCanvas::rightClickEvent)
	EVT_RIGHT_DOWN(nsNodeCanvas::rightClickEvent)
	EVT_SIZE(nsNodeCanvas::sizeEvent)
	EVT_MOTION(nsNodeCanvas::motionEvent)
	EVT_MOUSEWHEEL(nsNodeCanvas::wheelEvent)
	EVT_KEY_DOWN(nsNodeCanvas::keyPressEvent)
END_EVENT_TABLE()



nsNodeCanvas::nsNodeCanvas(wxWindow* parent, nsNodeTree* tree)
	:   wxGLCanvas(parent, NS_WID_CANVAS, NULL, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE | nsBorderNone)
{
	m_clickedSocket = 0;
	m_tree          = tree;
	m_viewport      = NSrect(0, 0, 0, 0);
	m_mousePos      = NSvec2(0, 0);
	m_lastPos       = NSvec2(0, 0);
	m_clickedPos    = NSvec2(0, 0);


	m_size          = NSvec2(GetClientSize().x, GetClientSize().y);
	m_projection    = NSrect(-(m_size.x * .5f), -(m_size.y * .5f), m_size.x, m_size.y);
	m_dragFrom      = NSvec2(0, 0);
	m_dragTo        = NSvec2(0, 0);
	m_viewportDirty = true;
	m_firstPaint    = true;
	m_grabCapture   = false;
	m_grabbed       = false;

	// build a drag / seclect rect
	m_dragSelection = false;

	// zoom min max
	m_zoomLimit.x = 0.75f;
	m_zoomLimit.y = 10.f;

	// copy of hidden
	m_context = nsWorkspace::getSingleton().getGLContext();

	m_tree->attachCanvas(this);
	nsNodeIterator it = tree->getNodeIterator();


	// re-create nodes
	while (it.hasMoreElements())
	{
		nsNode* nd = it.getNext();
		if (nd->getState() == NDST_ACTIVE)
		{
			nd->setState(NDST_INACTIVE);
			captureNode(nd);
		}
		m_renderList.push_back(nd);
	}
}


nsNodeCanvas::~nsNodeCanvas()
{
	if (m_tree)
	{
		m_tree->setSize(m_size);
		m_tree->setProjection(m_projection);
		m_tree = 0;
	}
}


void nsNodeCanvas::updateMouseCo(void)
{
	wxPoint pos = wxGetMousePosition();
	ScreenToClient(&pos.x, &pos.y);
	m_lastPos = m_mousePos = m_clickedPos = NSvec2(pos.x, pos.y);
}


void nsNodeCanvas::redraw(void)
{
	m_tree->setSize(m_size);
	m_tree->setProjection(m_projection);
	Refresh();
}


NSvec2 nsNodeCanvas::projectPoint(const NSvec2& v)
{
	// For hit testing with a cartesian coordinate system
	// m_projection being a variant of NSrect(-m_size.x/2, -m_size.y/2, m_size.x, m_size.y)
	// or std projection
	// m_projection being NSrect(0.f, 0.f, m_size.x, m_size.y)

	UT_ASSERT(m_size.valid() && !m_size.isZero());

	return NSvec2(
	           m_projection.x + m_projection.width * v.x / m_size.x,
	           m_projection.y + m_projection.height * v.y / m_size.y
	       );
}


void nsNodeCanvas::addNode(nsNode* nd)
{
	if (nd)
	{
		const NSrect& rect = nd->getRect();

		// drop at the previous mouse coordinate
		NSvec2 dropPoint = projectPoint(m_mousePos);

		nd->setPosition(dropPoint.x, dropPoint.y);
		m_renderList.push_back(nd);

		sendEvent(NS_NODE_ADD, nd);
		redraw();
	}
}



void nsNodeCanvas::grabCanvas(bool doit, bool releaseSelection)
{
	// release temporary states
	if (m_grabCapture)
	{
		redraw();
		m_grabCapture   = false;
	}

	if (releaseSelection && m_dragSelection)
	{
		redraw();
		m_dragSelection = false;
		m_dragFrom      = NSvec2(0, 0);
		m_dragTo        = NSvec2(0, 0);
	}

	if (doit)
	{
		if (!HasCapture())
			CaptureMouse();
		SetFocus();
	}
	else
	{
		if (HasCapture())
			ReleaseMouse();
	}
}


nsNode* nsNodeCanvas::hitTestNode(const NSvec2& projClick)
{
	// do a front to back search, nodes on
	// top of the stack will be tested first

	nsReverseNodeIterator it(m_renderList);
	while (it.hasMoreElements())
	{
		nsNode* nd = it.getNext();

		// test for off screen nodes

		const NSrect& rct = nd->getRect();
		if (!m_projection.contains(rct))
			continue;

		// do the test
		if (rct.contains(projClick.x, projClick.y))
			return nd;
	}
	return 0;
}



nsSocket* nsNodeCanvas::hitTestSocket(const NSvec2& projClick)
{
	nsReverseNodeIterator it(m_renderList);
	while (it.hasMoreElements())
	{
		nsNode* nd = it.getNext();

		// test for off screen nodes
		NSrect rct = nd->getRect();
		if (!m_projection.contains(rct))
			continue;

		// add larger test border
		rct.expand(72, 72);
		if (rct.contains(projClick.x, projClick.y))
		{
			nsSocket* sock;

			sock = nd->getFirstInput();
			if (sock)
			{
				if ( sock->getDerrivedRect().containsX(projClick.x) )
				{
					while (sock)
					{
						if (sock->getDerrivedRect().containsY(projClick.y))
							return sock;
						sock = sock->getNext();
					}
				}
			}

			sock = nd->getFirstOutput();
			if (sock)
			{
				if ( sock->getDerrivedRect().containsX(projClick.x) )
				{
					while (sock)
					{
						if (sock->getDerrivedRect().containsY(projClick.y))
							return sock;
						sock = sock->getNext();
					}
				}
			}
		}
	}
	return 0;
}



void nsNodeCanvas::bringToFront(nsNode* node)
{
	// sync render list and tree list
	if (m_tree)
	{
		m_tree->bringToFront(node);
		m_tree->bringToFront(m_renderList, node);
	}
}


void nsNodeCanvas::captureNode(nsNode* node)
{

	UT_ASSERT(node);
	if (!node)
		return;

	// only if not captured
	if (node->getState() != NDST_ACTIVE)
	{
		node->setState(NDST_ACTIVE);
		m_captured.push_back(node);

		// send notifications
		sendEvent(NS_NODE_SELECT, node);

		nsMainWindow::getSingleton().setStatus(0, "Captured %s : %p", node->getType()->getName().c_str(), node);
	}
}


void nsNodeCanvas::captureNodes(const NSrect& rect)
{
	// hit test all

	if (m_renderList.empty())
		return;


	nsNode* lastCapture = 0;
	nsReverseNodeIterator it(m_renderList);
	while (it.hasMoreElements())
	{
		nsNode* node = it.getNext();

		// test for off screen nodes
		const NSrect& nodeRect = node->getRect();
		if (!m_projection.contains(nodeRect))
			continue;

		if (node->getState() != NDST_ACTIVE)
		{
			if (rect.contains(nodeRect))
			{
				// save last for the event
				lastCapture = node;

				node->setState(NDST_ACTIVE);
				m_captured.push_back(node);
			}
		}
	}

	if (lastCapture != 0)
	{
		m_captured.erase(lastCapture);
		lastCapture->setState(NDST_INACTIVE);
		// call capture event
		captureNode(lastCapture);
	}
}



void nsNodeCanvas::releaseCapture(void)
{
	if (!m_captured.empty())
	{
		nsNodeIterator it(m_captured);
		while (it.hasMoreElements())
		{
			nsNode* nd = it.getNext();

			// inactivate it
			nd->setState(NDST_INACTIVE);

			// send notifications
			sendEvent(NS_NODE_DESELECT, nd);
		}
		m_captured.clear();
		nsMainWindow::getSingleton().setStatus(0, "");
	}
	m_dragSelection = false;
	m_grabCapture = false;
}


void nsNodeCanvas::grabCapturedEvent(wxCommandEvent& evt)
{
	if (!m_captured.empty())
	{
		// notify that we want to grab all
		grabCanvas(true);

		m_dragSelection = false;
		m_grabbed = true;
		m_hasCapture = true;
		m_grabCapture = true;

		evt.Skip();
		redraw();
	}
}


void nsNodeCanvas::deleteCapturedEvent(wxCommandEvent& evt)
{
	if (!m_captured.empty())
	{
		if (m_captured.size() == m_tree->getNodeCount())
		{
			// send notifications
			sendEvent(NS_TREE_CLEAR);
			nsNodeIterator it(m_captured);
			while (it.hasMoreElements())
				sendEvent(NS_NODE_REMOVE, it.getNext());

			m_tree->clear();
			m_renderList.clear();

		}
		else
		{
			nsNodeIterator it(m_captured);
			while (it.hasMoreElements())
			{
				// TODO: tell all about this event
				nsNode* nd = it.getNext();

				// send notifications
				sendEvent(NS_NODE_DESELECT, nd);

				m_renderList.erase(nd);
				m_tree->deleteNode(nd);
			}
		}

		m_captured.clear();
		evt.Skip();
		redraw();
	}
}



void nsNodeCanvas::selectAllEvent(wxCommandEvent& evt)
{
	// release all
	releaseCapture();

	if (!m_renderList.empty())
	{
		// re-capture all
		nsNode* lastCapture = 0;
		nsReverseNodeIterator it(m_renderList);
		while (it.hasMoreElements())
		{
			nsNode* node = it.getNext();

			// save last for the event
			lastCapture = node;

			node->setState(NDST_ACTIVE);
			m_captured.push_back(node);
		}


		if (lastCapture != 0)
		{
			m_captured.erase(lastCapture);
			lastCapture->setState(NDST_INACTIVE);

			// call capture event
			captureNode(lastCapture);

			// redraw canvas
			evt.Skip();
			redraw();
		}
	}
}



void nsNodeCanvas::cutEvent(wxCommandEvent& evt)
{
	if (!m_captured.empty() && !m_renderList.empty())
	{
		// clone all nodes
		nsWorkspace::getSingleton().getClipboard().copy(m_captured);

		// remove from the canvas & owning tree
		nsNodeIterator it(m_captured);
		while (it.hasMoreElements())
		{
			nsNode* node = it.getNext();

			// send notifications
			sendEvent(NS_NODE_REMOVE, node);

			m_tree->deleteNode(node);
			m_renderList.erase(node);
		}

		if (m_renderList.empty())
			m_renderList.clear();

		m_captured.clear();
		evt.Skip();
		redraw();
	}
}



void nsNodeCanvas::copyEvent(wxCommandEvent& evt)
{
	if (!m_captured.empty())
	{
		// clone all nodes
		nsWorkspace::getSingleton().getClipboard().copy(m_captured);

		evt.Skip();
		redraw();
	}
}


void nsNodeCanvas::pasteEvent(wxCommandEvent& evt)
{
	nsClipboard& clp = nsWorkspace::getSingleton().getClipboard();
	if (!clp.isEmpty())
	{
		updateMouseCo();

		// insert clipboard into scene
		clp.paste(m_tree, m_renderList, projectPoint(m_mousePos));

		if (!m_renderList.empty())
		{
			// send notifications
			sendEvent(NS_NODE_ADD, m_renderList.back());
		}

		evt.Skip();
		redraw();
	}
}


void nsNodeCanvas::duplicateEvent(wxCommandEvent& evt)
{
	if (!m_captured.empty())
	{
		// clone all nodes

		nsNodes dupli;
		nsWorkspace::getSingleton().getClipboard().duplicate(m_tree, m_captured, dupli);

		releaseCapture();
		nsNode* lastCapture = 0;

		nsNodeIterator it(dupli);
		while (it.hasMoreElements())
		{
			nsNode* node = it.getNext();

			lastCapture = node;

			node->setState(NDST_ACTIVE);
			m_captured.push_back(node);
			m_renderList.push_back(node);
		}

		if (lastCapture != 0)
		{
			m_captured.erase(lastCapture);
			lastCapture->setState(NDST_INACTIVE);

			// call capture event
			captureNode(lastCapture);
		}

		// notify that we want to grab all
		grabCanvas(true);

		m_dragSelection = false;
		m_grabbed = true;
		m_hasCapture = true;
		m_grabCapture = true;


		evt.Skip();
		redraw();
	}
}

void nsNodeCanvas::sendEvent(int id, nsNode* node)
{
	nsNodeEvent evt((nsNodifierID)id, this, node);

	// push to property window
	nsPropertyPage::getSingleton().nodeEvent(evt);
	// TODO: maybe add solution
}


void nsNodeCanvas::sendEvent(int id, nsSocket* sock)
{
	nsSocketEvent evt((nsNodifierID)id, this, sock);

	// push to property window
	//nsPropertyPage::getSingleton().socketEvent(evt);
}


void nsNodeCanvas::sendEvent(int id)
{
	nsTreeEvent evt((nsNodifierID)id, this, m_tree);

	// push to property window
	nsPropertyPage::getSingleton().treeEvent(evt);
}



void nsNodeCanvas::focusLostEvent(wxMouseCaptureLostEvent&)
{
	grabCanvas(false);
}


void nsNodeCanvas::leftClickEvent(wxMouseEvent& evt)
{
	m_lastPos = m_mousePos = m_clickedPos = NSvec2(evt.m_x, evt.m_y);
	m_hasCapture = evt.LeftDown();
	grabCanvas(m_hasCapture, false);

	// left was clicked
	if (m_hasCapture)
	{
		// hit test nodes
		nsNode* nodeAtPoint = hitTestNode(projectPoint(m_mousePos));
		if (nodeAtPoint != 0)
		{
			bringToFront(nodeAtPoint);

			if (nodeAtPoint->getState() == NDST_INACTIVE)
				releaseCapture();

			// set status
			captureNode(nodeAtPoint);
			evt.Skip();
			redraw();
		}
		else
		{
			// hit test sockets
			nsSocket* sockAtPoint = hitTestSocket(projectPoint(m_mousePos));

			if (sockAtPoint)
			{

				m_clickedSocket = sockAtPoint;
				if (m_clickedSocket->isConnected() && m_clickedSocket->isInput())
				{
					// disconnect & swap active socket.
					nsSocket* sock = m_clickedSocket->getSocketLink();
					sendEvent(NS_SOCKET_UNLINK, sock);

					m_clickedSocket->connect(0);
					m_clickedSocket = sock;
				}

				evt.Skip();
				redraw();
			}

			else
			{
				// release all.
				releaseCapture();

				// enable dragging
				m_dragSelection = true;
				m_dragFrom      = projectPoint(m_mousePos);
				m_dragTo        = NSvec2(0, 0);


				evt.Skip();
				redraw();
			}
		}
	}
	else
	{
		// left was released
		if (m_dragSelection)
		{
			// capture nodes found within the drag rect
			NSrect drag(m_dragFrom.x, m_dragFrom.y, m_dragTo.x, m_dragTo.y);
			drag.makeMinMax();

			captureNodes(drag);

			evt.Skip();
			redraw();
		}
		else if (m_clickedSocket)
		{
			// find droped socket.
			nsSocket* sockAtPoint = hitTestSocket(projectPoint(m_mousePos));

			if (sockAtPoint && sockAtPoint != m_clickedSocket)
			{
				// droped on another.

				if (m_clickedSocket->isInput() && sockAtPoint->isOutput())
				{
					if (m_clickedSocket->canConnect(sockAtPoint))
					{
						sendEvent(NS_SOCKET_LINK, m_clickedSocket);
						m_clickedSocket->connect(sockAtPoint);
					}
				}
				else if (sockAtPoint->isInput() && m_clickedSocket->isOutput())
				{
					if (sockAtPoint->canConnect(m_clickedSocket))
					{
						sendEvent(NS_SOCKET_LINK, sockAtPoint);
						sockAtPoint->connect(m_clickedSocket);
					}
				}
			}
			// nullify temp
			m_clickedSocket = 0;

			evt.Skip();
			redraw();
		}
	}
	grabCanvas(m_hasCapture, !m_hasCapture);
}



void nsNodeCanvas::middleClickEvent(wxMouseEvent& evt)
{
	m_lastPos = m_mousePos = m_clickedPos = NSvec2(evt.m_x, evt.m_y);
	m_hasCapture = evt.MiddleDown();
	grabCanvas(m_hasCapture);
}


void nsNodeCanvas::rightClickEvent(wxMouseEvent& evt)
{
	m_lastPos = m_mousePos = m_clickedPos = NSvec2(evt.m_x, evt.m_y);
	m_hasCapture = evt.RightDown();
	grabCanvas(m_hasCapture);

	if (m_hasCapture)
	{
		// hit test for node under the menu
		// and capture it

		nsNode* nodeAtPoint = hitTestNode(projectPoint(m_mousePos));
		if (nodeAtPoint != 0)
		{
			bringToFront(nodeAtPoint);

			if (nodeAtPoint->getState() == NDST_INACTIVE)
				releaseCapture();

			// set status
			captureNode(nodeAtPoint);
			evt.Skip();
			redraw();
		}
	}

	// show add menu on right click up event
	if (!m_hasCapture)
	{
		wxMenu menu;
		nsMainWindow::getSingleton().makeNodeMenu(this, &menu);


		// append edit as well
		menu.AppendSeparator();
		nsMainWindow::getSingleton().makeEditMenu(&menu);

		PopupMenu(&menu);
	}
}



void nsNodeCanvas::motionEvent(wxMouseEvent& evt)
{
	if (!m_hasCapture)
		return;

	m_mousePos = NSvec2(evt.m_x, evt.m_y);
	NSvec2 relPos = m_mousePos - m_lastPos;
	m_lastPos = m_mousePos;

	if (relPos.isZero())
		return;

	if (m_dragSelection)
	{
		// update current selection rectangle
		NSvec2 z = getZoom();

		m_dragTo.x += relPos.x * z.x;
		m_dragTo.y += relPos.y * z.y;

		redraw();
		evt.Skip();
		return;
	}


	if (m_grabCapture && m_grabbed)
	{
		// make sure offest is the same,
		// zero the first time round.
		relPos.x = 0.f;
		relPos.y = 0.f;
		m_grabbed = false;

		redraw();
		evt.Skip();
		return;
	}

	if (m_clickedSocket && evt.LeftIsDown())
	{
		// dragging a socket link.
		redraw();
		evt.Skip();
		return;
	}

	if (!m_captured.empty() && (m_grabCapture  || evt.LeftIsDown()) )
	{
		// transform nodes
		NSvec2 z = getZoom();


		nsNodeIterator it(m_captured);
		while (it.hasMoreElements())
			it.getNext()->translate(relPos.x * z.x, relPos.y * z.y);


		// update status bar
		const NSrect& newTrans = m_captured.front()->getRect();
		nsMainWindow::getSingleton().setStatus(1, "Moved [%i, %i]", ((int)newTrans.x), ((int)newTrans.y));

		redraw();
		evt.Skip();
		return;
	}


	if (evt.MiddleIsDown())
	{
		// move or zoom
		if (evt.ControlDown())
		{
			NSfloat diff = 1.f / (2.f * -relPos.y * (1.f / m_size.y));
			setZoom(diff);
		}
		else
		{
			NSvec2 z = getZoom();
			m_projection.x -= relPos.x * z.x;
			m_projection.y -= relPos.y * z.y;
		}
		redraw();
		evt.Skip();
	}
}



void nsNodeCanvas::wheelEvent(wxMouseEvent& evt)
{
	setZoom(evt.m_wheelRotation > 0 ? 16.f : -16.f);
	redraw();
	evt.Skip();
}


void nsNodeCanvas::keyPressEvent(wxKeyEvent& evt)
{
	int code = evt.GetKeyCode();

	// release grab explicitly
	if (m_grabCapture && code == WXK_ESCAPE)
	{
		m_grabCapture = false;
		m_grabbed = false;

		redraw();
		evt.Skip();
	}
	else evt.Skip();
}



void nsNodeCanvas::leftDblClickEvent(wxMouseEvent& evt)
{

	NSvec2 dbl(evt.m_x, evt.m_y);

	if (!m_captured.empty())
	{
		nsNode* top = m_captured.front();

		if (top->getRect().contains(projectPoint(dbl)))
		{
			top->getType()->edit(top);
			redraw();
		}
	}
}



void nsNodeCanvas::sizeEvent(wxSizeEvent& evt)
{
	NSvec2 newSize(evt.m_size.x, evt.m_size.y);

	// prevent zero size
	newSize.x = NSMax(newSize.x, 5.f);
	newSize.y = NSMax(newSize.y, 5.f);


	// only after first paint
	if (!m_firstPaint)
	{
		NSvec2 cur = getZoom();
		cur.x *= newSize.x;
		cur.y *= newSize.y;
		m_projection.width  = cur.x;
		m_projection.height = cur.y;
	}

	m_size.x = newSize.x;
	m_size.y = newSize.y;
	m_viewportDirty = true;

	m_tree->setSize(m_size);
	m_tree->setProjection(m_projection);
}



NSvec2 nsNodeCanvas::getZoom(void)
{
	UT_ASSERT(m_size.valid() && !m_size.isZero());
	// current zoom
	return NSvec2(m_projection.width / m_size.x, m_projection.height / m_size.y);
}


void nsNodeCanvas::setZoom(NSfloat zd)
{
	if (NSFuzzy(zd))
		return;

	// align zd to zoom min/max
	NSfloat fac = 1.f / (m_zoomLimit.y - m_zoomLimit.x);
	zd += fac;


	NSrect o = m_projection;
	NSvec2 v = m_projection.getSize();
	NSvec2 d = v / zd;
	m_projection.contract(d.x, d.y);


	NSvec2 z = getZoom();
	if (z.x < m_zoomLimit.x || z.y > m_zoomLimit.y)
		m_projection = o;
}



void nsNodeCanvas::paintEvent(wxPaintEvent& evt)
{
	wxPaintDC dc(this);
	if (!IsShown() || IsFrozen())
		return;

	SetCurrent(*m_context);

	// Apply state to rendersystem.
	nsRenderSystem& paint = nsRenderSystem::getSingleton();
	paint.clear();


	m_viewportDirty = true;
	if (m_viewportDirty)
	{
		paint.loadViewport(NSrect(0, 0, m_size.x, m_size.y));
		m_viewportDirty = false;
	}

	if (m_firstPaint)
	{
		// reset
		m_projection = NSrect(-(m_size.x * .5f), -(m_size.y * .5f), m_size.x, m_size.y);
		m_firstPaint = false;
		m_tree->setProjection(m_projection);
	}


	paint.loadProjection(m_projection);
	paint.drawBackground();

	// draw link if connecting a socket.
	if (m_clickedSocket)
		paint.drawConnectingSocket(m_clickedSocket, m_clickedSocket->getDerrivedRect().center(), projectPoint(m_mousePos));


	// Fill links in the background.
	paint.drawSocketLinks(m_renderList);


	// Fill all nodes
	nsNodeIterator it(m_renderList);
	while (it.hasMoreElements())
	{
		nsNode* nd = it.getNext();
		paint.drawNode(nd);
	}

	if (m_dragSelection)
	{
		// selection on top
		NSrect drag(m_dragFrom.x, m_dragFrom.y, m_dragTo.x, m_dragTo.y);

		// make rect aligned
		drag.makeMinMax();


		paint.drawDragRect(drag);
	}

	// swap
	SwapBuffers();
}


void nsNodeCanvas::eraseEvent(wxEraseEvent& evt)
{
}
