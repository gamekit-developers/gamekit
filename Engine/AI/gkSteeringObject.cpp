/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Nestor Silveira.

    Contributor(s): none yet.
	Notes: Some parts of this file are based on OpenSteer -- Steering Behaviors for Autonomous Characters
	(the work of Craig Reynolds <craig_reynolds@playstation.sony.com>)
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

#include "gkMathUtils.h"
#include "gkSteeringObject.h"
#include "gkGameObject.h"
#include "gkSteering.h"
#include "gkRigidBody.h"
#include "gkLogger.h"
#include "btBulletDynamicsCommon.h"

using namespace OpenSteer;

gkSteeringObject::OTHERS gkSteeringObject::m_others;

gkSteeringObject::gkSteeringObject(gkGameObject* obj, gkScalar maxSpeed, const gkVector3& forward, const gkVector3& up, const gkVector3& side) 
: m_obj(obj), 
m_mass(1), 
m_radius(1), 
m_speed(maxSpeed),  
m_maxForce(maxSpeed), 
m_maxSpeed(maxSpeed),
m_avoidancePredictTimeMin(0.9f),
m_avoidancePredictTimeMax(2),
m_avoidancePredictTime(0.9f),
m_evading(false),
m_avoiding(false),
m_curvature(0),
m_lastForward(Vec3::zero),
m_lastPosition(Vec3::zero),
m_smoothedPosition(Vec3::zero),
m_smoothedCurvature(0),
m_smoothedAcceleration(Vec3::zero),
m_forward(forward),
m_up(up),
m_side(side),
m_inGoal(false)
{
	if(m_obj->getAttachedBody())
	{
		setMass(1/m_obj->getAttachedBody()->getBody()->getInvMass());

		GK_ASSERT(m_mass);
	}

	if(m_obj->getAttachedBody() || m_obj->getAttachedCharacter())
	{
		btVector3 center;
		btScalar radius;

		m_obj->getCollisionObject()->getCollisionShape()->getBoundingSphere(center, radius);

		m_radius = radius;

		GK_ASSERT(m_radius && "m_radius cannot be zero!!!");
	}

	reset();
	
	gkSteeringObject::m_others.insert(this);
}

gkSteeringObject::~gkSteeringObject() 
{
	gkSteeringObject::m_others.erase(this);
}

void gkSteeringObject::reset()
{
	m_avoidancePredictTime = 0.9f;
	m_evading = false;
	m_avoiding = false;
	m_curvature = 0;
	m_lastForward = Vec3::zero;
	m_lastPosition = Vec3::zero;
	m_smoothedPosition = Vec3::zero;
	m_smoothedCurvature = 0;
	m_smoothedAcceleration = Vec3::zero;
	m_speed = m_maxSpeed;
	
	SimpleSteering::reset();
}

Vec3 gkSteeringObject::side() const
{
	return m_obj->getOrientation() * m_side;
}

Vec3 gkSteeringObject::up() const
{
	return m_obj->getOrientation() * m_up;
}

Vec3 gkSteeringObject::forward() const
{
	return m_obj->getOrientation() * m_forward;
}

Vec3 gkSteeringObject::position() const
{
	return m_obj->getPosition();
}

void gkSteeringObject::applySteeringForce(const Vec3& force, const float elapsedTime)
{
    const Vec3 adjustedForce = adjustRawSteeringForce (force, elapsedTime);
	
    // enforce limit on magnitude of steering force
    const Vec3 clippedForce = adjustedForce.truncateLength (maxForce ());
	
    // compute acceleration and velocity
    Vec3 newAcceleration = (clippedForce / mass());
    Vec3 newVelocity = velocity();
	
    // damp out abrupt changes and oscillations in steering acceleration
    // (rate is proportional to time step, then clipped into useful range)
    if (elapsedTime > 0)
    {
		
        const float smoothRate = clip (9 * elapsedTime, 0.15f, 0.4f);
		
        blendIntoAccumulator (smoothRate,
                              newAcceleration,
                              m_smoothedAcceleration);
		
		if(gkVector3(m_smoothedAcceleration).isNaN())
		{
			m_smoothedAcceleration = Vec3::zero;
		}
    }
	
    // Euler integrate (per frame) acceleration into velocity
    newVelocity += m_smoothedAcceleration * elapsedTime;
	
    // enforce speed limit
    newVelocity = newVelocity.truncateLength (maxSpeed ());
	
	
    // update Speed
    setSpeed (newVelocity.length());
	
    // Euler integrate (per frame) velocity into position
	//setPosition (position() + (newVelocity * elapsedTime));
	
	gkSteering steer(up());
	m_obj->rotate(steer.getRotation(forward(), newVelocity.normalize()), TRANSFORM_LOCAL);
	m_obj->setLinearVelocity(m_forward * m_speed);
	
    // maintain path curvature information
    measurePathCurvature (elapsedTime);
	
    // running average of recent positions
    blendIntoAccumulator (elapsedTime * 0.06f, // QQQ
                          position (),
                          m_smoothedPosition);
}


void gkSteeringObject::applyDirectForce(const Vec3& force, const float elapsedTime)
{
    const Vec3 adjustedForce = adjustRawSteeringForce (force, elapsedTime);

    // enforce limit on magnitude of steering force
    const Vec3 clippedForce = adjustedForce.truncateLength (maxForce ());
	
	gkSteering steer(up());
	m_obj->rotate(steer.getRotation(forward(), adjustedForce.normalize()), TRANSFORM_LOCAL);

    Vec3 newVelocity = velocity();

    // enforce speed limit
    newVelocity = newVelocity.truncateLength (maxSpeed ());
	
    // update Speed
    setSpeed (newVelocity.length());

	m_obj->setLinearVelocity(m_forward * m_speed);
 }

void gkSteeringObject::applyBrakingForce(const float rate, const float deltaTime)
{
    const float rawBraking = speed () * rate;
    const float clipBraking = ((rawBraking < maxForce()) ?
                               rawBraking :
                               maxForce());

    setSpeed (speed () - (clipBraking * deltaTime));
}

Vec3 gkSteeringObject::adjustRawSteeringForce(const Vec3& force, const float /* deltaTime */)
{
    const float maxAdjustedSpeed = 0.2f * maxSpeed ();

    if ((speed () > maxAdjustedSpeed) || (force == Vec3::zero))
    {
        return force;
    }
    else
    {
        const float range = speed() / maxAdjustedSpeed;
        // const float cosine = interpolate (pow (range, 6), 1.0f, -1.0f);
        // const float cosine = interpolate (pow (range, 10), 1.0f, -1.0f);
        // const float cosine = interpolate (pow (range, 20), 1.0f, -1.0f);
        // const float cosine = interpolate (pow (range, 100), 1.0f, -1.0f);
        // const float cosine = interpolate (pow (range, 50), 1.0f, -1.0f);
        const float cosine = interpolate (pow (range, 20), 1.0f, -1.0f);
        return limitMaxDeviationAngle (force, cosine, forward());
    }
}

// ----------------------------------------------------------------------------
// measure path curvature (1/turning-radius), maintain smoothed version

void gkSteeringObject::measurePathCurvature(const float elapsedTime)
{
    if (elapsedTime > 0)
    {
        const Vec3 dP = m_lastPosition - position ();
        const Vec3 dF = (m_lastForward - forward ()) / dP.length ();
        const Vec3 lateral = dF.perpendicularComponent (forward ());
        const float sign = (lateral.dot (side ()) < 0) ? 1.0f : -1.0f;
        m_curvature = lateral.length() * sign;
        blendIntoAccumulator (elapsedTime * 4.0f,
                              m_curvature,
                              m_smoothedCurvature);
        m_lastForward = forward ();
        m_lastPosition = position ();
    }
}

bool gkSteeringObject::clearPathToGoal(const gkVector3& goalPosition, gkGameObject* target)
{
    bool result = true;

	const float sideThreshold = radius() * 8.0f;
	const float behindThreshold = radius() * 2.0f;

	const Vec3 goalOffset = goalPosition - position();
	const float goalDistance = goalOffset.length ();
	const Vec3 goalDirection = goalOffset / goalDistance;

	const bool goalIsAside = isAside(goalPosition, 0.5);

	OTHERS::iterator it = gkSteeringObject::m_others.begin();

	while(it != gkSteeringObject::m_others.end())
	{
		gkSteeringObject& e = **it;

		if(this != &e && target != e.m_obj)
		{
			const float eDistance = Vec3::distance (position(), e.position());
			const float timeEstimate = 0.3f * eDistance / e.speed(); //xxx
			const Vec3 eFuture = e.predictFuturePosition (timeEstimate);
			const Vec3 eOffset = eFuture - position();
			const float alongCorridor = goalDirection.dot (eOffset);
			const bool inCorridor = ((alongCorridor > -behindThreshold) && 
									 (alongCorridor < goalDistance));
			const float eForwardDistance = forward().dot (eOffset);

			// consider as potential blocker if within the corridor
			if (inCorridor)
			{
				const Vec3 perp = eOffset - (goalDirection * alongCorridor);
				const float acrossCorridor = perp.length();
				if (acrossCorridor < sideThreshold)
				{
					// not a blocker if behind us and we are perp to corridor
					const float eFront = eForwardDistance + e.radius ();

					const bool eIsBehind = eFront < -behindThreshold;
					const bool eIsWayBehind = eFront < (-2 * behindThreshold);
					const bool safeToTurnTowardsGoal =
						((eIsBehind && goalIsAside) || eIsWayBehind);

					if (!safeToTurnTowardsGoal)
					{
						// this enemy blocks the path to the goal, so return false
						result = false;

						break;
					}
				}
			}
		}

		++it;
	}

	return result;
}

// ----------------------------------------------------------------------------
// adjust obstacle avoidance look ahead time: make it large when we are far
// from the goal and heading directly towards it, make it small otherwise.
void gkSteeringObject::adjustObstacleAvoidanceLookAhead(bool clearPath, const gkVector3& goalPosition)
{
    if(clearPath)
    {
        m_evading = false;
        const float goalDistance = Vec3::distance(goalPosition, position());
        const bool headingTowardGoal = isAhead(goalPosition, 0.98f);
        const bool isNear = (goalDistance/speed()) < m_avoidancePredictTimeMax;
        const bool useMax = headingTowardGoal && !isNear;
        m_avoidancePredictTime =
            (useMax ? m_avoidancePredictTimeMax : m_avoidancePredictTimeMin);
    }
    else
    {
		m_evading = true;
        m_avoidancePredictTime = m_avoidancePredictTimeMin;
    }
}

Vec3 gkSteeringObject::steerToEvadeOthers(const gkGameObject* target)
{
    // sum up weighted evasion
    Vec3 evade (0, 0, 0);

	OTHERS::iterator it = gkSteeringObject::m_others.begin();

	while(it != gkSteeringObject::m_others.end())
	{
		gkSteeringObject& e = **it;

		if(this != &e && target != e.m_obj)
		{
			const Vec3 eOffset = e.position() - position();
			const float eDistance = eOffset.length();

			// xxx maybe this should take into account e's heading? xxx
			const float timeEstimate = 0.5f * eDistance / e.speed(); //xxx
			const Vec3 eFuture = e.predictFuturePosition (timeEstimate);

			// steering to flee from eFuture (enemy's future position)
			const Vec3 flee = xxxsteerForFlee(eFuture);

			const float eForwardDistance = forward().dot (eOffset);
			const float behindThreshold = radius() * -2;

			const float distanceWeight = 4 / eDistance;
			const float forwardWeight = ((eForwardDistance > behindThreshold) ?
										 1.0f : 0.5f);

			const Vec3 adjustedFlee = flee * distanceWeight * forwardWeight;

			evade += adjustedFlee;
		}

		++it;
    }
    return evade;
}

