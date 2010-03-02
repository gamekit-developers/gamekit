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
#ifndef _gkArcBallNode_h_
#define _gkArcBallNode_h_

#include "gkLogicNode.h"

class SceneNode;

class gkArcBallNode : public gkLogicNode
{
public:

	enum 
	{
		ENABLE,
		CENTER_UPDATED,
		POS_UPDATED,
		XPOS,
		YPOS,
		RELX,
		RELY,
		RELZ,
		MAX_SOCKETS
	};

	gkArcBallNode(gkLogicTree *parent, size_t id);

	~gkArcBallNode();

	void update(Ogre::Real tick);

	bool evaluate(Ogre::Real tick);

	GK_INLINE gkLogicSocket* getEnable() {return &m_sockets[ENABLE];}
    GK_INLINE gkLogicSocket* getUpdateCenter() {return &m_sockets[CENTER_UPDATED];}
	GK_INLINE gkLogicSocket* getUpdatePosition() {return &m_sockets[POS_UPDATED];}

    GK_INLINE gkLogicSocket* getX() {return &m_sockets[XPOS];}
    GK_INLINE gkLogicSocket* getY() {return &m_sockets[YPOS];}

    GK_INLINE gkLogicSocket* getRelX() {return &m_sockets[RELX];}
    GK_INLINE gkLogicSocket* getRelY() {return &m_sockets[RELY];}
    GK_INLINE gkLogicSocket* getRelZ() {return &m_sockets[RELZ];}


private:

	void SetNewPosition();
	void GetNewCenter();

private:

	gkLogicSocket m_sockets[MAX_SOCKETS];

	Ogre::RaySceneQuery* m_rayQuery;
	Ogre::SceneNode* m_RotationNode;

	gkGameObject* m_gameObj;
	gkScene* m_scene;

	Ogre::Vector3 m_center;

	bool m_centerUpdated;
	bool m_positionUpdated;
};

#endif//_gkArcBallNode_h_