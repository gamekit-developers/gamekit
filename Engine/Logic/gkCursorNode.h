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
		HEIGHT
	};

	DECLARE_SOCKET_TYPE(ENABLE, bool);
	DECLARE_SOCKET_TYPE(UPDATED, bool);
	DECLARE_SOCKET_TYPE(MATERIAL_NAME, gkString);
	DECLARE_SOCKET_TYPE(XPOS, gkScalar);
	DECLARE_SOCKET_TYPE(YPOS, gkScalar);
	DECLARE_SOCKET_TYPE(WIDTH, gkScalar);
	DECLARE_SOCKET_TYPE(HEIGHT, gkScalar);

	gkCursorNode(gkLogicTree *parent, size_t id);

	~gkCursorNode();

	void update(Ogre::Real tick);
	bool evaluate(Ogre::Real tick);

	GK_INLINE gkLogicSocket<bool>* getEnable() {return GET_SOCKET(ENABLE);}
	GK_INLINE gkLogicSocket<bool>* getUpdate() {return GET_SOCKET(UPDATED);}
	GK_INLINE gkLogicSocket<gkString>* getMaterialName() {return GET_SOCKET(MATERIAL_NAME);}
    GK_INLINE gkLogicSocket<gkScalar>* getX() {return GET_SOCKET(XPOS);}
    GK_INLINE gkLogicSocket<gkScalar>* getY() {return GET_SOCKET(YPOS);}
    GK_INLINE gkLogicSocket<gkScalar>* getWidth() {return GET_SOCKET(WIDTH);}
	GK_INLINE gkLogicSocket<gkScalar>* getHeight() {return GET_SOCKET(HEIGHT);}


private:

	Ogre::Overlay* m_overlay;

	Ogre::OverlayContainer* m_panelContainer;

	Ogre::OverlayElement* m_panelElement;
};

#endif//_gkCursorNode_h_