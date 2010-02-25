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
#ifndef _gkPickNode_h_
#define _gkPickNode_h_

#include "gkLogicNode.h"
#include "LinearMath/btVector3.h"

class btPoint2PointConstraint;
class btRigidBody;

class gkPickNode : public gkLogicNode
{
public:
	enum 
	{
		ENABLE,
		CREATE_PICK,
		RELEASE_PICK,
		UPDATE_PICK,
		XPOS,
		YPOS,
		MAX_SOCKETS
	};

	gkPickNode(gkLogicTree *parent, size_t id);

	~gkPickNode();

	void update(Ogre::Real tick);

	bool evaluate(Ogre::Real tick);

	GK_INLINE gkLogicSocket* getEnable() {return &m_sockets[ENABLE];}
	GK_INLINE gkLogicSocket* getCreatePick() {return &m_sockets[CREATE_PICK];}
	GK_INLINE gkLogicSocket* getReleasePick() {return &m_sockets[RELEASE_PICK];}
	GK_INLINE gkLogicSocket* getUpdate() {return &m_sockets[UPDATE_PICK];}

    GK_INLINE gkLogicSocket* getX() {return &m_sockets[XPOS];}
    GK_INLINE gkLogicSocket* getY() {return &m_sockets[YPOS];}

private:

	void CreatePick();
	void ReleasePick();
	void UpdatePick();

private:

	gkLogicSocket m_sockets[MAX_SOCKETS];

	btPoint2PointConstraint* m_constraint;
	btRigidBody* m_pickedBody;
	btVector3 m_hitPos;
	btVector3 m_oldPickingPos;
	Ogre::Real m_oldPickingDist;
};

#endif//_gkPickNode_h_