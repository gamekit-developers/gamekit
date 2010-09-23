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
		UPDATE,
		MATERIAL_NAME,
		X,
		Y,
		WIDTH,
		HEIGHT
	};

	DECLARE_SOCKET_TYPE(ENABLE, bool);
	DECLARE_SOCKET_TYPE(UPDATE, bool);
	DECLARE_SOCKET_TYPE(MATERIAL_NAME, gkString);
	DECLARE_SOCKET_TYPE(X, gkScalar);
	DECLARE_SOCKET_TYPE(Y, gkScalar);
	DECLARE_SOCKET_TYPE(WIDTH, gkScalar);
	DECLARE_SOCKET_TYPE(HEIGHT, gkScalar);

	gkCursorNode(gkLogicTree* parent, size_t id);

	~gkCursorNode();

	void initialize();
	void update(Ogre::Real tick);
	bool evaluate(Ogre::Real tick);

private:

	Ogre::Overlay* m_overlay;

	Ogre::OverlayContainer* m_panelContainer;

	Ogre::OverlayElement* m_panelElement;
};

#endif//_gkCursorNode_h_
