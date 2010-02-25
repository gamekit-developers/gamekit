/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Nestor Silveira.

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
#ifndef _gkCursorNode_h_
#define _gkCursorNode_h_

#include "gkLogicNode.h"

class gkCursorNode : public gkLogicNode
{
public:
	enum 
	{
		ENABLE,
		UPDATED,
		MATERIAL_NAME,
		XPOS,
		YPOS,
		WIDTH,
		HEIGHT,
		MAX_SOCKETS
	};

	gkCursorNode(gkLogicTree *parent, size_t id);

	~gkCursorNode();

	void update(Ogre::Real tick);

	bool evaluate(Ogre::Real tick);

	GK_INLINE gkLogicSocket* getEnable() {return &m_sockets[ENABLE];}
	GK_INLINE gkLogicSocket* getUpdate() {return &m_sockets[UPDATED];}

	GK_INLINE gkLogicSocket* getMaterialName() {return &m_sockets[MATERIAL_NAME];}
    GK_INLINE gkLogicSocket* getX() {return &m_sockets[XPOS];}
    GK_INLINE gkLogicSocket* getY() {return &m_sockets[YPOS];}
    GK_INLINE gkLogicSocket* getWidth() {return &m_sockets[WIDTH];}
	GK_INLINE gkLogicSocket* getHeight() {return &m_sockets[HEIGHT];}


private:

	Ogre::Overlay* m_overlay;

	Ogre::OverlayContainer* m_panelContainer;

	Ogre::OverlayElement* m_panelElement;

	gkLogicSocket m_sockets[MAX_SOCKETS];
};

#endif//_gkCursorNode_h_