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
		SOURCE,
		TARGET_UP_DIRECTION,
		ORIGINAL_TARGET_DIRECTION,
		HIT_DISTANCE,
		FOUND_THRESHOLD,
		PATH,
		SHOW_PATH_OFFSET,
		HAS_REACHED_END,
		NOT_HAS_REACHED_END,
		IDLE,
		WALK,
		RUN
	};

	typedef gkFindPathNode::PathData PathData;

	DECLARE_SOCKET_TYPE(UPDATE, bool);
	DECLARE_SOCKET_TYPE(TARGET, gkGameObject*);
	DECLARE_SOCKET_TYPE(SOURCE, gkGameObject*);
	DECLARE_SOCKET_TYPE(TARGET_UP_DIRECTION, gkVector3);
	DECLARE_SOCKET_TYPE(ORIGINAL_TARGET_DIRECTION, gkVector3);
	DECLARE_SOCKET_TYPE(HIT_DISTANCE, gkScalar);
	DECLARE_SOCKET_TYPE(FOUND_THRESHOLD, gkScalar);
	DECLARE_SOCKET_TYPE(PATH, PathData*);
	DECLARE_SOCKET_TYPE(SHOW_PATH_OFFSET, gkVector3);
	DECLARE_SOCKET_TYPE(HAS_REACHED_END, bool);
	DECLARE_SOCKET_TYPE(NOT_HAS_REACHED_END, bool);
	DECLARE_SOCKET_TYPE(IDLE, bool);
	DECLARE_SOCKET_TYPE(WALK, bool);
	DECLARE_SOCKET_TYPE(RUN, bool);

    gkFollowPathNode(gkLogicTree *parent, size_t id);
	virtual ~gkFollowPathNode() {}

	bool evaluate(gkScalar tick);
	void update(gkScalar tick);

	void setWalkVelocity(gkScalar velocity) { m_walkVelocity = velocity; }
	void setRunVelocity(gkScalar velocity) { m_runVelocity = velocity; }

private:

	gkRadian GetRotationAngleForAxis(const gkVector3& from, const gkVector3& to, const gkVector3& axis);
	gkVector3 GetProjectionOnPlane(const gkVector3& V, const gkVector3& N);

	void setVelocity(gkScalar d, gkScalar tick);

	bool isTargetReached();

	void showPath();

private:

	PathData* m_path;

	gkGameObject* m_target;
	gkVector3 m_dir;
	gkVector3 m_up;
	gkVector3 m_upMask;
	gkScalar m_foundThreshold;

	gkScalar m_walkVelocity;
	gkScalar m_runVelocity;
};

#endif//_gkFollowPathNode_h_
