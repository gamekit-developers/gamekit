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
		REL_X,
		REL_Y,
		REL_Z,
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
		AVOID_BLOCKING
	};

	DECLARE_SOCKET_TYPE(UPDATE, bool);
	DECLARE_SOCKET_TYPE(CENTER_OBJ, gkGameObject*);
	DECLARE_SOCKET_TYPE(CENTER_POSITION, gkVector3);
	DECLARE_SOCKET_TYPE(REL_X, gkScalar);
	DECLARE_SOCKET_TYPE(REL_Y, gkScalar);
	DECLARE_SOCKET_TYPE(REL_Z, gkScalar);
	DECLARE_SOCKET_TYPE(TARGET, gkGameObject*);
	DECLARE_SOCKET_TYPE(MIN_PITCH, gkScalar);
	DECLARE_SOCKET_TYPE(MAX_PITCH, gkScalar);
	DECLARE_SOCKET_TYPE(MIN_ROLL, gkScalar);
	DECLARE_SOCKET_TYPE(MAX_ROLL, gkScalar);
	DECLARE_SOCKET_TYPE(KEEP_DISTANCE, bool);
	DECLARE_SOCKET_TYPE(CURRENT_ROLL, gkQuaternion);
	DECLARE_SOCKET_TYPE(CURRENT_PITCH, gkQuaternion);
	DECLARE_SOCKET_TYPE(MIN_Z, gkScalar);
	DECLARE_SOCKET_TYPE(MAX_Z, gkScalar);
	DECLARE_SOCKET_TYPE(AVOID_BLOCKING, bool);
	

	gkArcBallNode(gkLogicTree *parent, size_t id);

	~gkArcBallNode();

	bool evaluate(gkScalar tick);
	void update(gkScalar tick);

private:

	gkVector3 m_center;
	gkVector3 m_oldCenter;

	gkGameObject* m_target;
	gkGameObject* m_centerObj;

	gkQuaternion m_rollNode;
	gkQuaternion m_pitchNode;

	gkScalar m_radiusOffset;
};

#endif//_gkArcBallNode_h_