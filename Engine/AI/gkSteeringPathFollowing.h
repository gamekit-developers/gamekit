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
#ifndef _gkSteeringPathFollowing_h_
#define _gkSteeringPathFollowing_h_

#include "gkSteeringObject.h"
#include "gkNavPath.h"

class gkSceneObstacle;
class dtNavMesh;

class gkSteeringPathFollowing : public gkSteeringObject
{
public:

	gkSteeringPathFollowing(
	    gkGameObject* obj,
	    gkScalar maxSpeed,
	    const gkVector3& forward,
	    const gkVector3& up,
	    const gkVector3& side,
	    const gkVector3& polyPickExt,
	    int maxPathPolys,
	    gkScalar minimumTurningRadius
	);

	~gkSteeringPathFollowing();

	bool inGoal() const;

	GK_INLINE void setGoalPosition(const gkVector3& position) { m_goalPosition = position; }
	GK_INLINE void setGoalRadius(gkScalar radius) { m_goalRadius = radius; }

	GK_INLINE const gkVector3& getGoalPosition() const { return m_goalPosition; }
	GK_INLINE gkScalar getGoalRadius() const { return m_goalRadius; }
	bool steering(STATE& newState, const float elapsedTime);
	void reset();
	void setNavMesh(PDT_NAV_MESH navMesh) { m_navMesh = navMesh; }
	PDT_NAV_MESH getNavMesh() const { return m_navMesh; }

private:

	bool createPath();

private:

	PDT_NAV_MESH m_navMesh;

	gkVector3 m_goalPosition;

	gkScalar m_goalRadius;

	gkNavPath m_navPath;

	gkVector3 m_polyPickExt;

	int m_maxPathPolys;

	gkScalar m_minimumTurningRadius;
};

#endif//_gkSteeringPathFollowing_h_
