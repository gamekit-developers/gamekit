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
#ifndef _gkGrabNode_h_
#define _gkGrabNode_h_

#include "gkPickNode.h"

class gkGrabNode : public gkPickNode
{
public:
	enum 
	{
		THROW_OBJECT,
		TARGET,
		GRAB_DIRECTION,
		THROW_VEL,
		RELATED_OFFSET_POSITION,
		THROWED,
		MAX_SOCKETS
	};

	gkGrabNode(gkLogicTree *parent, size_t id);

	~gkGrabNode();

	void update(Ogre::Real tick);

	bool evaluate(Ogre::Real tick);

	GK_INLINE gkLogicSocket* getThrowObject() {return &m_sockets[THROW_OBJECT];}
	GK_INLINE gkLogicSocket* getTarget() {return &m_sockets[TARGET];}
	GK_INLINE gkLogicSocket* getGrabDirection() {return &m_sockets[GRAB_DIRECTION];}
	GK_INLINE gkLogicSocket* getThrowVelocity() {return &m_sockets[THROW_VEL];}
	GK_INLINE gkLogicSocket* getOffsetPosition() {return &m_sockets[RELATED_OFFSET_POSITION];}
	GK_INLINE gkLogicSocket* getThrowed() {return &m_sockets[THROWED];}

private:

	void ThrowObject();
	Ogre::Ray GetRay();
	gkVector3 GetPivotPosition();

private:

	gkLogicSocket m_sockets[MAX_SOCKETS];

	gkGameObject* m_target;
};

#endif//_gkGrabNode_h_