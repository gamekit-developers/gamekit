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

#include "gkLogicNode.h"
#include "LinearMath/btVector3.h"

class btPoint2PointConstraint;
class btRigidBody;

class gkGrabNode : public gkLogicNode
{
public:
	enum 
	{
		CREATE_GRAB,
		RELEASE_GRAB,
		TARGET,
		GRAB_DIRECTION,
		RELEASE_VEL,
		MAX_SOCKETS
	};

	gkGrabNode(gkLogicTree *parent, size_t id);

	~gkGrabNode();

	void update(Ogre::Real tick);

	bool evaluate(Ogre::Real tick);

	GK_INLINE gkLogicSocket* getCreateGrab() {return &m_sockets[CREATE_GRAB];}
	GK_INLINE gkLogicSocket* getReleaseGrab() {return &m_sockets[RELEASE_GRAB];}
	GK_INLINE gkLogicSocket* getTarget() {return &m_sockets[TARGET];}
	GK_INLINE gkLogicSocket* getGrabDirection() {return &m_sockets[GRAB_DIRECTION];}
	GK_INLINE gkLogicSocket* getReleaseVelocity() {return &m_sockets[RELEASE_VEL];}

private:

	void CreateGrab();
	void ReleaseGrab(bool applyVel);
	void UpdateGrab();

private:

	gkLogicSocket m_sockets[MAX_SOCKETS];

	gkGameObject* m_target;

	btPoint2PointConstraint* m_constraint;

	btRigidBody* m_pickedBody;
	btVector3 m_hitPos;
	btVector3 m_oldPickingPos;
	Ogre::Real m_oldPickingDist;
};

#endif//_gkGrabNode_h_