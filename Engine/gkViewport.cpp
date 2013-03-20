/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Xavier T.

    Contributor(s):  harkon.kr.
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

#include "gkCommon.h"
#include "gkViewport.h"
#include "gkWindow.h"
#include "gkLogger.h"

#include "OgreRenderWindow.h"
#include "OgreRoot.h"

#ifdef OGREKIT_USE_COMPOSITOR
#include "External/Ogre/gkOgreCompositorManager.h"
#endif

gkViewport::gkViewport(gkWindow* window, Ogre::Viewport* vp)
	:	m_window(window),
		m_framing(0),
		m_viewport(vp)
{
	GK_ASSERT(m_window && m_viewport);
}

gkViewport::~gkViewport()
{	
	if (m_viewport)
	{
#ifdef OGREKIT_USE_COMPOSITOR
		gkCompositorManager::getSingleton().removeCompositorChain(this);
#endif
		GK_ASSERT(m_window->getRenderWindow() != 0);
		m_window->getRenderWindow()->removeViewport(m_viewport->getZOrder());
	}
}


void gkViewport::_setDimension(int framing, gkScalar left, gkScalar top, gkScalar right, gkScalar bottom)
{
	m_framing = framing;

	float l = left;
	float r = right;
	float t = top;
	float b = bottom;
	int w = m_window->getWidth();
	int h = m_window->getHeight();

	int rw = m_window->getRequestedWidth();
	int rh = m_window->getRequestedHeight();

	if (w != rw || h != rh)
	{
		switch (m_framing)
		{
		case FRAMING_CROP:
			{
				l = (w - rw) / (2.0f * w);
				r = (w + rw) / (2.0f * w);
				t = (h - rh) / (2.0f * h);
				b = (h + rh) / (2.0f * h);
				break;
			}
		case FRAMING_LETTERBOX:
			{
				float hratio = (float)rw / (float)w;
				float vratio = (float)rh/ (float)h;

				if (hratio > vratio)
				{
					l = 0;
					r = 1;
					t = (1 - (vratio / hratio)) / 2.0f;
					b = t + (vratio / hratio);
				}
				else
				{

					t = 0;
					b = 1;
					l = (1 - (hratio / vratio)) / 2.0f;
					r = l + (hratio / vratio);
				}
				break;
			}
		}

	}

	m_viewport->setDimensions(l, t, r - l, b - t);
}
