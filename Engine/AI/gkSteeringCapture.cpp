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

#include "OgreRoot.h"
#include "gkMathUtils.h"
#include "gkSteeringCapture.h"
#include "gkRayTest.h"
#include "Obstacle.h"
#include "gkGameObject.h"
#include "gkLogger.h"

using namespace OpenSteer;

class SceneObstacle : public Obstacle
{
public:
    
	SceneObstacle(gkScalar howFarCanSee) : m_howFarCanSee(howFarCanSee) {}

    ~SceneObstacle() {}

	void findIntersectionWithVehiclePath (const AbstractVehicle& vehicle, PathIntersection& pi) const;

private:

	gkScalar m_howFarCanSee;
};

void SceneObstacle::findIntersectionWithVehiclePath(const AbstractVehicle& vehicle, PathIntersection& pi) const
{
	pi.intersect = false;

	Ogre::Ray ray(vehicle.position(), vehicle.forward() * m_howFarCanSee);

	gkRayTest rayTest;

	if(rayTest.collides(ray))
	{
		if(rayTest.getObject()->getAttachedCharacter() == 0)
		{
			pi.intersect = true;
			pi.obstacle = this;
			pi.distance = rayTest.getHitPoint().distance(vehicle.position());
			pi.steerHint = rayTest.getHitNormal();
			pi.steerHint.z = 0;
			pi.steerHint = pi.steerHint.normalize();
			gkScalar a = pi.steerHint.dot(vehicle.side());
			pi.steerHint = a > 0 ? vehicle.side() : -vehicle.side();

			pi.surfacePoint = rayTest.getHitPoint();
			pi.surfaceNormal = rayTest.getHitNormal();
			pi.vehicleOutside = true;
		}
	}
}

////////////////////////////////////////////

gkSteeringCapture::gkSteeringCapture(gkGameObject* obj, gkScalar maxSpeed, const gkVector3& forward, const gkVector3& up, const gkVector3& side, gkGameObject* target, gkScalar minPredictionTime, gkScalar maxPredictionTime) 
: gkSteeringObject(obj, maxSpeed, forward, up, side),
m_target(target),
m_sceneObstable(new SceneObstacle(30)),
m_minPredictionTime(minPredictionTime),
m_maxPredictionTime(maxPredictionTime)
{
	m_allObstacles.push_back(m_sceneObstable);
}

gkSteeringCapture::~gkSteeringCapture()
{
	delete m_sceneObstable;
}

const gkVector3& gkSteeringCapture::getGoalPosition() const
{
	return m_target->getPosition();
}

gkScalar gkSteeringCapture::getGoalRadius() const
{
    const gkGameObjectProperties &props = m_target->getProperties();
    
	const gkPhysicsProperties &phy = props.m_physics;

	return phy.m_radius;
}

gkVector3 gkSteeringCapture::steering(STATE& newState, const float elapsedTime)
{
	bool clearPath = clearPathToGoal(m_target->getPosition(), m_target);

	gkScalar avoidancePredictTime = adjustObstacleAvoidanceLookAhead(clearPath, m_target->getPosition(), m_minPredictionTime, m_maxPredictionTime);

	gkVector3 obstacleAvoidance = steerToAvoidObstacles(avoidancePredictTime, m_allObstacles);

    if (obstacleAvoidance != Vec3::zero)
    {
		newState = AVOIDING;

        return obstacleAvoidance;
    }
    else
    {
        const Vec3 seek = xxxsteerForSeek(m_target->getPosition());

        if (clearPath)
        {
			newState = SEEKING_GOAL;

            Vec3 s = limitMaxDeviationAngle (seek, 0.707f, forward());

            return s;
        }
        else
        {
			newState = EVADING_AND_SEEKING_GOAL;
			
            const Vec3 evade = steerToEvadeOthers(m_target);

			const Vec3 steer = limitMaxDeviationAngle (seek + evade, 0.707f, forward());

			return steer;
        }
    }
}

void gkSteeringCapture::applyForce(const gkVector3& force, const float elapsedTime)
{
	applySteeringForce(force, elapsedTime);
}

void gkSteeringCapture::reset()
{
	gkSteeringObject::reset();
}

void gkSteeringCapture::notifyInGoal()
{
	setSpeed(0);
}
