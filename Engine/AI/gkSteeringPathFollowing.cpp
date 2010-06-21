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

#include "gkSteeringPathFollowing.h"
#include "OgreRoot.h"
#include "gkGameObject.h"
#include "gkNavMeshData.h"
#include "gkLogger.h"

using namespace OpenSteer;

gkSteeringPathFollowing::gkSteeringPathFollowing(gkGameObject* obj, gkScalar maxSpeed, const gkVector3& forward, const gkVector3& up, const gkVector3& side, const gkVector3& polyPickExt, int maxPathPolys, gkScalar minimumTurningRadius) 
: gkSteeringObject(obj, maxSpeed, forward, up, side),
m_goalPosition(gkVector3::ZERO),
m_goalRadius(0),
m_polyPickExt(polyPickExt),
m_maxPathPolys(maxPathPolys),
m_minimumTurningRadius(minimumTurningRadius)
{
}

gkSteeringPathFollowing::~gkSteeringPathFollowing()
{
}

void gkSteeringPathFollowing::createPath()
{
	m_navPath.create(gkNavMeshData::getSingleton(), m_obj->getPosition(), m_goalPosition, m_polyPickExt, m_maxPathPolys, radius()); 
}

gkVector3 gkSteeringPathFollowing::steering(STATE& newState, const float elapsedTime)
{
	gkVector3 steering(gkVector3::ZERO);
	
	if(m_navPath.empty())
	{
		reset();

		createPath();
	}
	else 
	{
		m_navPath.showPath();
		
		newState = SEEKING_PATH;
		
		steering = steerForTargetSpeed(maxSpeedForCurvature(&m_navPath, m_minimumTurningRadius, 1));
		
		const Vec3 pf = steerToFollowPathLinear(1, combinedLookAheadTime(1, radius()/2.0f), m_navPath); 
		
		if (pf != Vec3::zero)
		{
			// steer to remain on path
			if (pf.dot (forward()) < 0)
				steering = pf;
			else
				steering = pf + steering;
		}
		else
		{
			// path aligment: when neither obstacle avoidance nor
			// path following is required, align with path segment
			const Vec3 pathHeading = mapPointAndDirectionToTangent(m_navPath, position(), 1);
		
			steering += steerTowardHeading(pathHeading) * (isNearWaypoint(m_navPath, position()) ? 0.5f : 0.1f);
		}

		//steering = adjustSteeringForMinimumTurningRadius(steering, m_minimumTurningRadius);
	}

	return steering;
}

void gkSteeringPathFollowing::applyForce(const gkVector3& force, const float elapsedTime)
{
	applySteeringForce(force, elapsedTime);
}

void gkSteeringPathFollowing::reset()
{
	m_navPath.clear();

	gkSteeringObject::reset();
}


void gkSteeringPathFollowing::notifyInGoal()
{
	setSpeed(0);

	m_navPath.clear();
}

