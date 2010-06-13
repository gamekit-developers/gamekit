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

gkSteeringPathFollowing::gkSteeringPathFollowing(gkGameObject* obj, gkScalar maxSpeed, const gkVector3& forward, const gkVector3& up, const gkVector3& side, const gkVector3& polyPickExt, int maxPathPolys) 
: gkSteeringObject(obj, maxSpeed, forward, up, side),
m_goalPosition(gkVector3::ZERO),
m_goalRadius(1),
m_polyPickExt(polyPickExt),
m_maxPathPolys(maxPathPolys),
m_pathIndex(0)
{
}

gkSteeringPathFollowing::~gkSteeringPathFollowing()
{
}

void gkSteeringPathFollowing::setGoal(const gkVector3& goalPosition, gkScalar goalRadius)
{
	m_goalPosition = goalPosition;
	m_goalRadius = goalRadius;
}

void gkSteeringPathFollowing::createPath()
{
	m_navPath.create(gkNavMeshData::getSingleton(), m_obj->getPosition(), m_goalPosition, m_polyPickExt, m_maxPathPolys); 
	m_navPath.setRadius(radius());
	m_pathIndex = 0;
}

void gkSteeringPathFollowing::update(gkScalar tick)
{
	if(m_navPath.empty())
	{
		reset();
		createPath();
		m_inGoal = false;
		return;
	}

	m_navPath.showPath();
	
	gkVector3 steer(0, 0, 0);

	float baseDistance = OpenSteer::Vec3::distance(position(), m_goalPosition);

	if (baseDistance > (radius() + m_goalRadius)) 
	{
		gkVector3 targetPosition = m_navPath.getPoint(m_pathIndex);
		
		baseDistance = OpenSteer::Vec3::distance(position(), targetPosition);
		
		if (baseDistance < radius())
		{
			++m_pathIndex;
			
			if(m_pathIndex >= m_navPath.getPoints())
				m_pathIndex = m_navPath.getPoints()-1;
		}

		steer = targetPosition - position();	
	}
	else
	{
		m_inGoal = true;

		setSpeed(0);

		m_navPath.clear();
	}

	
	applyDirectForce(steer, tick);
}

