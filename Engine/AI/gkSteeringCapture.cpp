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
#include "gkSceneObstacle.h"
#include "gkLogger.h"

using namespace OpenSteer;

gkSteeringCapture::gkSteeringCapture(gkGameObject* obj, gkScalar maxSpeed, const gkVector3& forward, const gkVector3& up, const gkVector3& side, gkGameObject* target, gkScalar minPredictionTime, gkScalar maxPredictionTime)
	: gkSteeringObject(obj, maxSpeed, forward, up, side),
	  m_target(target),
	  m_sceneObstable(new gkSceneObstacle(30)),
	  m_minPredictionTime(minPredictionTime),
	  m_maxPredictionTime(maxPredictionTime)
{
	m_allObstacles.push_back(m_sceneObstable);
}

gkSteeringCapture::~gkSteeringCapture()
{
	delete m_sceneObstable;
}

bool gkSteeringCapture::inGoal() const
{
	gkScalar baseDistance = gkVector2(position().x, position().y).distance(gkVector2(getGoalPosition().x, getGoalPosition().y));

	return baseDistance < radius() + getGoalRadius();
}

bool gkSteeringCapture::steering(STATE& newState, const float elapsedTime)
{
	bool clearPath = clearPathToGoal(m_target->getPosition(), m_target);

	gkScalar avoidancePredictTime = adjustObstacleAvoidanceLookAhead(clearPath, m_target->getPosition(), m_minPredictionTime, m_maxPredictionTime);

	gkVector3 steer = steerToAvoidObstacles(avoidancePredictTime, m_allObstacles);

	if (steer != Vec3::zero)
	{
		newState = AVOIDING;
	}
	else
	{
		const Vec3 seek = xxxsteerForSeek(m_target->getPosition());

		if (clearPath)
		{
			newState = SEEKING;

			steer = limitMaxDeviationAngle (seek, 0.707f, forward());
		}
		else
		{
			newState = EVADING;

			const Vec3 evade = steerToEvadeOthers(m_target);

			steer = limitMaxDeviationAngle (seek + evade, 0.707f, forward());
		}
	}

	applySteeringForce(steer, elapsedTime);

	return true;
}
