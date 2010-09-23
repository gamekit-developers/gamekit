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
#ifndef _nsRenderSystem_h_
#define _nsRenderSystem_h_


#include "nsCommon.h"
#include "nsSingleton.h"
#include "nsMath.h"



// Simple OpenGL 2D render tools
class nsRenderSystem  : public nsSingleton<nsRenderSystem>
{
private:

	nsRenderSystemPrivate*   m_context;
	NSrect                  m_projection, m_clip;

	void paintSocket(nsNode* parent, nsSocket* sock);

public:

	nsRenderSystem();
	~nsRenderSystem();

	// one time setup
	void initializeContextData(void);

	// cleanup
	void finalizeContextData(void);

	// setup viewport rendering
	void loadViewport(const NSrect& rect);

	// projection
	void loadProjection(const NSrect& rect);

	// clear buffers
	void clear(void);

	// back drop
	void drawBackground(void);

	// node
	void drawNode(nsNode* nd);

	// socket linking
	void drawConnectingSocket(nsSocket* sock, const NSvec2& pos, const NSvec2& mouse);

	// all links
	void drawSocketLinks(nsNodes& nodes);

	// selection rect
	void drawDragRect(const NSrect& rect);


	NS_DECLARE_SINGLETON(nsRenderSystem);
};

#endif//_nsRenderSystem_h_
