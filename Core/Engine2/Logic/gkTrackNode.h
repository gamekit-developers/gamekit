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
#ifndef _gkTrackNode_h_
#define _gkTrackNode_h_

#include "gkLogicNode.h"

class SceneNode;

class gkTrackNode : public gkLogicNode
{
public:

	enum 
	{
		ENABLE,
		OFFSET,
		STIFFNESS,
		DAMPING,
		MAX_SOCKETS
	};

	gkTrackNode(gkLogicTree *parent, size_t id);

	~gkTrackNode();

	void update(Ogre::Real tick);

	bool evaluate(Ogre::Real tick);

	GK_INLINE gkLogicSocket* getEnable() {return &m_sockets[ENABLE];}
	GK_INLINE gkLogicSocket* getOffset() {return &m_sockets[OFFSET];}

	// How hard the spring is to extend.
	//A high value here is as if the spring was a stick (or fixed lenght)
	GK_INLINE gkLogicSocket* getStiffness() {return &m_sockets[STIFFNESS];}

	// Controls how the spring resists the spring  boingggg effect.
	//A higher value makes for a smoother ride, lower values and the more boing you'll get.
	GK_INLINE gkLogicSocket* getDamping() {return &m_sockets[DAMPING];}

	void SetTarget(gkGameObject* target) { m_target = target; }

private:

	gkLogicSocket m_sockets[MAX_SOCKETS];

	gkGameObject* m_target;

	Ogre::SceneNode* m_RotationNode;

	gkVector3 m_oldPosition;

	bool m_oldPositionSet;
};

#endif//_gkTrackNode_h_