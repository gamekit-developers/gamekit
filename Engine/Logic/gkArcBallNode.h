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

class gkGameObject;
class gkRigidBody;

class gkArcBallNode : public gkLogicNode
{
public:

	enum 
	{
		UPDATE,
		CENTER_OBJ,
		CENTER_POSITION,
		RELX,
		RELY,
		RELZ,
		TARGET,
		MIN_PITCH,
		MAX_PITCH,
		MIN_ROLL,
		MAX_ROLL,
		KEEP_DISTANCE,
		CURRENT_ROLL,
		CURRENT_PITCH,
		MIN_Z,
		MAX_Z,
		MAX_SOCKETS
	};

	gkArcBallNode(gkLogicTree *parent, size_t id);

	~gkArcBallNode();

	bool evaluate(gkScalar tick);
	void update(gkScalar tick);

	GK_INLINE gkLogicSocket* getUpdate() {return &m_sockets[UPDATE];}

    GK_INLINE gkLogicSocket* getCenterObj() {return &m_sockets[CENTER_OBJ];}
	GK_INLINE gkLogicSocket* getCenterPosition() {return &m_sockets[CENTER_POSITION];}

    GK_INLINE gkLogicSocket* getRelX() {return &m_sockets[RELX];}
    GK_INLINE gkLogicSocket* getRelY() {return &m_sockets[RELY];}
    GK_INLINE gkLogicSocket* getRelZ() {return &m_sockets[RELZ];}

	GK_INLINE gkLogicSocket* getMinPitch() {return &m_sockets[MIN_PITCH];}
	GK_INLINE gkLogicSocket* getMaxPitch() {return &m_sockets[MAX_PITCH];}

	GK_INLINE gkLogicSocket* getMinRoll() {return &m_sockets[MIN_ROLL];}
	GK_INLINE gkLogicSocket* getMaxRoll() {return &m_sockets[MAX_ROLL];}

	GK_INLINE gkLogicSocket* getKeepDistance() {return &m_sockets[KEEP_DISTANCE];}

	GK_INLINE gkLogicSocket* getRoll() {return &m_sockets[CURRENT_ROLL];}
	GK_INLINE gkLogicSocket* getPitch() {return &m_sockets[CURRENT_PITCH];}

	GK_INLINE gkLogicSocket* getMinZ() {return &m_sockets[MIN_Z];}
	GK_INLINE gkLogicSocket* getMaxZ() {return &m_sockets[MAX_Z];}

	GK_INLINE gkLogicSocket* getTarget() {return &m_sockets[TARGET];}

private:

	gkLogicSocket m_sockets[MAX_SOCKETS];

	gkVector3 m_currentPosition;

	gkVector3 m_center;
	gkVector3 m_oldCenter;

	gkGameObject* m_target;
	gkGameObject* m_centerObj;

	gkQuaternion m_rollNode;
	gkQuaternion m_pitchNode;
};

#endif//_gkArcBallNode_h_