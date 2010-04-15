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
#ifndef _gkFollowPathNode_h_
#define _gkFollowPathNode_h_

#include "gkLogicNode.h"
#include "gkFindPathNode.h"

class gkGameObject;
class gkFollowPathNode : public gkLogicNode
{
public:

	enum
	{
		UPDATE,
		TARGET,
		TARGET_UP_DIRECTION,
		ORIGINAL_TARGET_DIRECTION,
		FOUND_THRESHOLD,
		PATH,
		HAS_REACHED_END,
		NOT_HAS_REACHED_END,
		CURRENT_STATE
	};

	typedef gkFindPathNode::PathData PathData;

	DECLARE_SOCKET_TYPE(UPDATE, bool);
	DECLARE_SOCKET_TYPE(TARGET, gkGameObject*);
	DECLARE_SOCKET_TYPE(TARGET_UP_DIRECTION, gkVector3);
	DECLARE_SOCKET_TYPE(ORIGINAL_TARGET_DIRECTION, gkVector3);
	DECLARE_SOCKET_TYPE(FOUND_THRESHOLD, gkScalar);
	DECLARE_SOCKET_TYPE(PATH, PathData*);
	DECLARE_SOCKET_TYPE(HAS_REACHED_END, bool);
	DECLARE_SOCKET_TYPE(NOT_HAS_REACHED_END, bool);
	DECLARE_SOCKET_TYPE(CURRENT_STATE, int);

    gkFollowPathNode(gkLogicTree *parent, size_t id);
	virtual ~gkFollowPathNode() {}

	bool evaluate(gkScalar tick);
	void update(gkScalar tick);

	void setIdleState(int state) { m_idleState = state;}
	void setWalkState(int state, gkScalar velocity) { m_walkState = state; m_walkVelocity = velocity; }
	void setRunState(int state, gkScalar velocity) { m_runState = state; m_runVelocity = velocity; }

private:

	gkRadian GetRotationAngleForAxis(const gkVector3& from, const gkVector3& to, const gkVector3& axis);
	gkVector3 GetProjectionOnPlane(const gkVector3& V, const gkVector3& N);

	void setVelocity(gkScalar d, gkScalar tick);
	bool animationHasBeenSet() const { return m_idleState!= -1 && m_walkVelocity && m_runVelocity; }

private:

	PathData* m_path;

	gkGameObject* m_target;
	gkVector3 m_dir;
	gkVector3 m_up;
	gkVector3 m_upMask;
	gkScalar m_foundThreshold;

	int m_idleState;
	int m_walkState;
	int m_runState;

	gkScalar m_walkVelocity;
	gkScalar m_runVelocity;
};

#endif//_gkFollowPathNode_h_
