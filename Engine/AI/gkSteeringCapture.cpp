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

#include "gkSteeringCapture.h"
#include "gkRayTest.h"
#include "Obstacle.h"
#include "OgreRoot.h"
#include "gkGameObject.h"
#include "gkLogger.h"

using namespace OpenSteer;

class SceneObstacle : public Obstacle
{
public:
    
	SceneObstacle() {}

    ~SceneObstacle() {}

	void findIntersectionWithVehiclePath (const AbstractVehicle& vehicle, PathIntersection& pi) const
	{
		pi.intersect = false;

		Ogre::Ray ray(vehicle.position(), vehicle.forward() * 1000);

		gkRayTest rayTest;

		if(rayTest.collides(ray))
		{
			if(rayTest.getObject()->getAttachedCharacter() == 0)
			{
				pi.intersect = true;
				pi.obstacle = this;
				pi.distance = rayTest.getHitPoint().distance(vehicle.position());
				pi.steerHint = rayTest.getHitNormal(); 
				pi.surfacePoint = rayTest.getHitPoint();
				pi.surfaceNormal = rayTest.getHitNormal();
				pi.vehicleOutside = true;
			}
		}
	}
};

////////////////////////////////////////////

gkSteeringCapture::gkSteeringCapture(gkGameObject* obj, gkScalar maxSpeed, const gkVector3& forward, const gkVector3& up, const gkVector3& side, gkGameObject* target) 
: gkSteeringObject(obj, maxSpeed, forward, up, side),
m_target(target),
m_sceneObstable(new SceneObstacle)
{
	m_allObstacles.push_back(m_sceneObstable);
}

gkSteeringCapture::~gkSteeringCapture()
{
	delete m_sceneObstable;
}

void gkSteeringCapture::update(gkScalar tick)
{
	if(m_inGoal) 
	{
		m_inGoal = false;
		
		reset();
	}

	gkVector3 steer(0, 0, 0);

	float baseDistance = Vec3::distance(position(), m_target->getPosition());

	if (baseDistance > (radius() + m_target->getRadius())) 
	{
		steer = steering();
	}
	else
	{
		m_inGoal = true;

		setSpeed(0);
	}

	applySteeringForce(steer, tick);
}

Vec3 gkSteeringCapture::steering()
{
	bool clearPath = clearPathToGoal(m_target->getPosition(), m_target);

	adjustObstacleAvoidanceLookAhead(clearPath, m_target->getPosition());

	gkVector3 obstacleAvoidance = steerToAvoidObstacles(m_avoidancePredictTime, m_allObstacles);

    m_avoiding = (obstacleAvoidance != Vec3::zero);

    if (m_avoiding)
    {
        return obstacleAvoidance;
    }
    else
    {
        const Vec3 seek = xxxsteerForSeek(m_target->getPosition());

        if (clearPath)
        {
            Vec3 s = limitMaxDeviationAngle (seek, 0.707f, forward());

            return s;
        }
        else
        {
            const Vec3 evade = steerToEvadeOthers(m_target);

			const Vec3 steer = limitMaxDeviationAngle (seek + evade, 0.707f, forward());

			return steer;
        }
    }
}

