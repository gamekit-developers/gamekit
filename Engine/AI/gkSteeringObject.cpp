/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Nestor Silveira.

    Contributor(s): none yet.
    Notes: Major part of this file are based on OpenSteer -- Steering Behaviors for Autonomous Characters
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
#include "gkRigidBody.h"
#include "gkLogger.h"
#include "gkNavPath.h"
#include "PolylineSegmentedPathwaySegmentRadii.h"
#include "QueryPathAlike.h"
#include "QueryPathAlikeUtilities.h"
#include "SegmentedPathAlikeUtilities.h"
#include "Utilities.h"
#include "btBulletDynamicsCommon.h"

using namespace OpenSteer;

gkSteeringObject::OTHERS gkSteeringObject::m_others;

/////////////////////////////////

namespace OpenSteer
{
class PointToRadiusMapping : public OpenSteer::DontExtractPathDistance
{
public:
	PointToRadiusMapping(): radius( 0.0f ) {}

	void setPointOnPathCenterLine( OpenSteer::Vec3 const& ) {}
	void setPointOnPathBoundary( OpenSteer::Vec3 const&  ) {}
	void setRadius( float r ) { radius = r; }
	void setTangent( OpenSteer::Vec3 const& ) {}
	void setSegmentIndex( OpenSteer::size_t ) {}
	void setDistancePointToPath( float  ) {}
	void setDistancePointToPathCenterLine( float ) {}
	void setDistanceOnPath( float  ) {}
	void setDistanceOnSegment( float ) {}

	float radius;
};

class PointToTangentMapping : public OpenSteer::DontExtractPathDistance
{
public:
	PointToTangentMapping() : tangent( OpenSteer::Vec3( 0.0f, 0.0f, 0.0f ) ) {}

	void setPointOnPathCenterLine( OpenSteer::Vec3 const& ) {}
	void setPointOnPathBoundary( OpenSteer::Vec3 const&  ) {}
	void setRadius( float ) {}
	void setTangent( OpenSteer::Vec3 const& t ) { tangent = t; }
	void setSegmentIndex( OpenSteer::size_t ) {}
	void setDistancePointToPath( float  ) {}
	void setDistancePointToPathCenterLine( float ) {}
	void setDistanceOnPath( float  ) {}
	void setDistanceOnSegment( float ) {}

	OpenSteer::Vec3 tangent;
};

class PointToPointOnCenterLineAndOutsideMapping : public OpenSteer::DontExtractPathDistance
{
public:
	PointToPointOnCenterLineAndOutsideMapping() : pointOnPathCenterLine( OpenSteer::Vec3( 0.0f, 0.0f, 0.0f ) ), distancePointToPathBoundary( 0.0f ) {}

	void setPointOnPathCenterLine( OpenSteer::Vec3 const& point) { pointOnPathCenterLine = point; }
	void setPointOnPathBoundary( OpenSteer::Vec3 const& ) {}
	void setRadius( float ) {}
	void setTangent( OpenSteer::Vec3 const& ) {}
	void setSegmentIndex( OpenSteer::size_t ) {}
	void setDistancePointToPath( float d ) { distancePointToPathBoundary = d; }
	void setDistancePointToPathCenterLine( float ) {}
	void setDistanceOnPath( float  ) {}
	void setDistanceOnSegment( float ) {}

	OpenSteer::Vec3 pointOnPathCenterLine;
	float distancePointToPathBoundary;
};

class PointToOutsideMapping : public OpenSteer::DontExtractPathDistance
{
public:
	PointToOutsideMapping() : distancePointToPathBoundary( 0.0f ) {}

	void setPointOnPathCenterLine( OpenSteer::Vec3 const& ) {}
	void setPointOnPathBoundary( OpenSteer::Vec3 const&  ) {}
	void setRadius( float ) {}
	void setTangent( OpenSteer::Vec3 const& ) {}
	void setSegmentIndex( OpenSteer::size_t ) {}
	void setDistancePointToPath( float d ) { distancePointToPathBoundary = d; }
	void setDistancePointToPathCenterLine( float ) {}
	void setDistanceOnPath( float  ) {}
	void setDistanceOnSegment( float ) {}

	float distancePointToPathBoundary;
};

class PointToSegmentIndexMapping : public OpenSteer::DontExtractPathDistance
{
public:
	PointToSegmentIndexMapping() : segmentIndex( 0 ) {}

	void setPointOnPathCenterLine( OpenSteer::Vec3 const& ) {}
	void setPointOnPathBoundary( OpenSteer::Vec3 const&  ) {}
	void setRadius( float ) {}
	void setTangent( OpenSteer::Vec3 const& ) {}
	void setSegmentIndex( OpenSteer::size_t i ) { segmentIndex = i; }
	void setDistancePointToPath( float  ) {}
	void setDistancePointToPathCenterLine( float ) {}
	void setDistanceOnPath( float  ) {}
	void setDistanceOnSegment( float ) {}

	OpenSteer::size_t segmentIndex;
};

/**
 * Maps @a point to @a pathway and extracts the radius at the mapping point.
 */
float mapPointToRadius( OpenSteer::PolylineSegmentedPathwaySegmentRadii const& pathway, OpenSteer::Vec3 const& point )
{
	PointToRadiusMapping mapping;
	OpenSteer::mapPointToPathAlike( pathway, point, mapping );
	return mapping.radius;
}

/**
 * Maps @a point to @a pathway and extracts the tangent at the mapping
 * point.
 */
OpenSteer::Vec3 mapPointToTangent( OpenSteer::PolylineSegmentedPathwaySegmentRadii const& pathway, OpenSteer::Vec3 const& point )
{
	PointToTangentMapping mapping;
	OpenSteer::mapPointToPathAlike( pathway, point, mapping );
	return mapping.tangent;
}

/**
 * Returns @c true if @a point is inside @a pathway segment @a segmentIndex.
 *
 * On point on the boundary isn't inside the pathway.
 */
bool isInsidePathSegment( OpenSteer::PolylineSegmentedPathwaySegmentRadii const& pathway,
                          OpenSteer::PolylineSegmentedPathwaySegmentRadii::size_type segmentIndex,
                          OpenSteer::Vec3 const& point )
{
	assert( pathway.isValid() && "pathway isn't valid." );
	assert( segmentIndex < pathway.segmentCount() && "segmentIndex out of range." );

	float const segmentDistance = pathway.mapPointToSegmentDistance( segmentIndex, point );
	OpenSteer::Vec3 const pointOnSegmentCenterLine = pathway.mapSegmentDistanceToPoint( segmentIndex, segmentDistance );
	float const segmentRadiusAtPoint = pathway.mapSegmentDistanceToRadius( segmentIndex, segmentDistance );

	float const distancePointToPointOnSegmentCenterLine = (point - pointOnSegmentCenterLine).length();

	return distancePointToPointOnSegmentCenterLine < segmentRadiusAtPoint;
}

/**
 * Maps the @a point to @a pathway and extracts the tangent at the mapping
 * point or of the next path segment as indicated by @a direction if the
 * mapping point is near a path defining point (waypoint).
 *
 * @param pathway Pathway to inspect.
 * @param point Point to map to @a pathway.
 * @param direction Follow the path in path direction (@c 1) or in reverse
 *                  direction ( @c -1 ).
 */
OpenSteer::Vec3 mapPointAndDirectionToTangent( OpenSteer::PolylineSegmentedPathwaySegmentRadii const& pathway, OpenSteer::Vec3 const& point, int direction )
{
	assert( ( ( 1 == direction ) || ( -1 == direction ) ) && "direction must be 1 or -1." );
	typedef OpenSteer::PolylineSegmentedPathwaySegmentRadii::size_type size_type;

	PointToSegmentIndexMapping mapping;
	OpenSteer::mapPointToPathAlike( pathway, point, mapping );
	size_type segmentIndex = mapping.segmentIndex;
	size_type nextSegmentIndex = segmentIndex;
	if ( 0 < direction )
	{
		nextSegmentIndex = OpenSteer::nextSegment( pathway, segmentIndex );
	}
	else
	{
		nextSegmentIndex = OpenSteer::previousSegment( pathway, segmentIndex );
	}

	if ( isInsidePathSegment( pathway, nextSegmentIndex, point ) )
	{
		segmentIndex = nextSegmentIndex;
	}

	// To save calculations to gather the tangent in a sound way the fact is
	// used that a polyline segmented pathway has the same tangent for a
	// whole segment.
	return pathway.mapSegmentDistanceToTangent( segmentIndex, 0.0f ) * static_cast< float >( direction );
}

/**
 * Returns @c true if @a point is near a waypoint of @a pathway.
 *
 * It is near if its distance to a waypoint of the path is lesser than the
 * radius of one of the segments that the waypoint belongs to.
 *
 * On point on the boundary isn't inside the pathway.
 */
bool isNearWaypoint( OpenSteer::PolylineSegmentedPathwaySegmentRadii const& pathway, OpenSteer::Vec3 const& point )
{
	assert( pathway.isValid() && "pathway must be valid." );

	typedef OpenSteer::PolylineSegmentedPathwaySegmentRadii::size_type size_type;

	size_type pointIndex = 0;

	// Test first waypoint.
	OpenSteer::Vec3 pointPathwayPointVector = point - pathway.point( pointIndex );
	float pointPathwayPointDistance = pointPathwayPointVector.dot( pointPathwayPointVector );
	if ( pointPathwayPointDistance < OpenSteer::square( pathway.segmentRadius( pointIndex ) ) )
	{
		return true;
	}

	// Test other waypoints.
	size_type const maxInnerPointIndex = pathway.pointCount() - 2;
	for ( pointIndex = 1; pointIndex <= maxInnerPointIndex; ++pointIndex )
	{
		pointPathwayPointVector = point - pathway.point( pointIndex );
		pointPathwayPointDistance = pointPathwayPointVector.dot( pointPathwayPointVector );
		if ( ( pointPathwayPointDistance < OpenSteer::square( pathway.segmentRadius( pointIndex ) ) ) ||
		        ( pointPathwayPointDistance < OpenSteer::square( pathway.segmentRadius( pointIndex - 1) ) ) )
		{
			return true;
		}
	}

	// Test last waypoint.
	pointPathwayPointVector = point - pathway.point( pointIndex );
	pointPathwayPointDistance = pointPathwayPointVector.dot( pointPathwayPointVector );
	if ( pointPathwayPointDistance < OpenSteer::square( pathway.segmentRadius( pointIndex - 1 ) ) )
	{
		return true;
	}

	return false;
}

/**
 * Maps @a point to @a pathway and returns the mapping point on the pathway
 * boundary and how far outside @a point is from the mapping point.
 */
OpenSteer::Vec3 mapPointToPointOnCenterLineAndOutside( OpenSteer::PolylineSegmentedPathwaySegmentRadii const& pathway, OpenSteer::Vec3 const& point, float& outside )
{
	PointToPointOnCenterLineAndOutsideMapping mapping;
	OpenSteer::mapPointToPathAlike( pathway, point, mapping );
	outside = mapping.distancePointToPathBoundary;
	return mapping.pointOnPathCenterLine;
}

/**
 * Maps @a point to @a pathway and returns how far outside @a point is from
 * the mapping point on the path boundary.
 */
float mapPointToOutside( OpenSteer::PolylineSegmentedPathwaySegmentRadii const& pathway, OpenSteer::Vec3 const& point )
{
	PointToOutsideMapping mapping;
	OpenSteer::mapPointToPathAlike( pathway, point, mapping);
	return mapping.distancePointToPathBoundary;
}

/**
 * Returns @c true if @a point is inside @a pathway, @c false otherwise.
 * A point on the boundary isn't inside the pathway.
 */
bool isInsidePathway( OpenSteer::PolylineSegmentedPathwaySegmentRadii const& pathway, OpenSteer::Vec3 const& point )
{
	return 0.0f > mapPointToOutside( pathway, point );
}

OpenSteer::PolylineSegmentedPathwaySegmentRadii::size_type mapPointToSegmentIndex(  OpenSteer::PolylineSegmentedPathwaySegmentRadii const& pathway,
        OpenSteer::Vec3 const& point )
{
	PointToSegmentIndexMapping mapping;
	OpenSteer::mapPointToPathAlike( pathway, point, mapping );
	return mapping.segmentIndex;
}
}

/////////////////////////////////


gkSteeringObject::gkSteeringObject(gkGameObject* obj, gkScalar maxSpeed, const gkVector3& forward, const gkVector3& up, const gkVector3& side)
	: m_obj(obj),
	  m_mass(0),
	  m_radius(0),
	  m_speed(0),
	  m_maxForce(maxSpeed),
	  m_maxSpeed(maxSpeed),
	  m_state(UNKNOWN),
	  m_smoothedAcceleration(Vec3::zero),
	  m_forward(forward),
	  m_up(up),
	  m_side(side),
	  m_curvature(0),
	  m_lastForward(Vec3::zero),
	  m_lastPosition(Vec3::zero),
	  m_smoothedCurvature(0),
	  m_lastFuturePosition(gkVector3::ZERO),
	  m_steerUtility(up)
{
	const gkGameObjectProperties& props = m_obj->getProperties();
	const gkPhysicsProperties& phy = props.m_physics;

	m_mass = phy.m_mass;
	m_radius = phy.m_radius;

	GK_ASSERT(m_radius && "m_radius cannot be zero!!!");
	GK_ASSERT(m_mass && "m_mass cannot be zero!!!");

	reset();

	gkSteeringObject::m_others.insert(this);
}

gkSteeringObject::~gkSteeringObject()
{
	gkSteeringObject::m_others.erase(this);
}

void gkSteeringObject::reset()
{
	m_smoothedAcceleration = Vec3::zero;
	m_speed = 0;
	m_curvature = 0;

	m_lastForward = Vec3::zero;
	m_lastPosition = Vec3::zero;
	m_smoothedCurvature = 0;

	m_lastFuturePosition = position();

	SimpleSteering::reset();
}

void gkSteeringObject::notifyInGoal()
{
	reset();
}


// measure path curvature (1/turning-radius), maintain smoothed version
void gkSteeringObject::measurePathCurvature (const float elapsedTime)
{
	if (elapsedTime > 0)
	{
		const Vec3 dP = m_lastPosition - position ();

		gkScalar length = dP.length ();

		if (length > std::numeric_limits<gkScalar>::epsilon())
		{
			const Vec3 dF = (m_lastForward - forward ()) / length;
			const Vec3 lateral = dF.perpendicularComponent (forward ());
			const float sign = (lateral.dot (side ()) < 0) ? 1.0f : -1.0f;
			m_curvature = lateral.length() * sign;

			blendIntoAccumulator (elapsedTime * 4.0f,
			                      m_curvature,
			                      m_smoothedCurvature);

			GK_ASSERT(!Ogre::Math::isNaN(m_curvature));

			m_lastForward = forward ();
			m_lastPosition = position ();
		}
	}
}

bool gkSteeringObject::update(gkScalar tick)
{
	if (!inGoal())
	{
		STATE newState = UNKNOWN;

		if (!steering(newState, tick))
			return false;

		gkVector3 futurePosition = predictFuturePosition(tick);

		if (speed() > 0 && m_lastFuturePosition.positionEquals(futurePosition, 0))
		{
			m_state = STUCK;
		}
		else
		{
			m_state = newState;
		}

		m_lastFuturePosition = futurePosition;
	}
	else if (m_state != IN_GOAL)
	{
		m_state = IN_GOAL;

		notifyInGoal();
	}
	else
	{
		return false;
	}

	return true;
}

void gkSteeringObject::applySteeringForce(const Vec3& force, const float elapsedTime)
{
	const Vec3 adjustedForce = adjustRawSteeringForce (force);

	// enforce limit on magnitude of steering force
	const Vec3 clippedForce = adjustedForce.truncateLength (maxForce ());

	// compute acceleration and velocity
	gkVector3 newAcceleration = (clippedForce / mass());

	// damp out abrupt changes and oscillations in steering acceleration
	// (rate is proportional to time step, then clipped into useful range)
	if (elapsedTime > 0)
	{
		const float smoothRate = clip (9 * elapsedTime, 0.15f, 0.4f);

		blendIntoAccumulator (smoothRate,
		                      newAcceleration,
		                      m_smoothedAcceleration);

		GK_ASSERT(!m_smoothedAcceleration.isNaN());

	}

	// Euler integrate (per frame) acceleration into velocity
	Vec3 newVelocity = velocity() + m_smoothedAcceleration * elapsedTime;

	// enforce speed limit
	newVelocity = newVelocity.truncateLength (maxSpeed ());

	// update Speed
	setSpeed (newVelocity.length());

	m_obj->rotate(m_steerUtility.getRotation(forward(), newVelocity.normalize()), TRANSFORM_LOCAL);

	m_obj->setLinearVelocity(m_forward * m_speed);

	measurePathCurvature(elapsedTime);
}

void gkSteeringObject::applyBrakingForce(const float rate, const float deltaTime)
{
	const float rawBraking = speed () * rate;
	const float clipBraking = ((rawBraking < maxForce()) ?
	                           rawBraking :
	                           maxForce());

	setSpeed (speed () - (clipBraking * deltaTime));
}

Vec3 gkSteeringObject::adjustRawSteeringForce(const Vec3& force)
{
	const float maxAdjustedSpeed = 0.2f * maxSpeed ();

	if ((speed () > maxAdjustedSpeed) || (force == Vec3::zero))
	{
		return force;
	}
	else
	{
		const float range = speed() / maxAdjustedSpeed;
		const float cosine = interpolate (pow (range, 20), 1.0f, -1.0f);
		return limitMaxDeviationAngle (force, cosine, forward());
	}
}

bool gkSteeringObject::clearPathToGoal(const gkVector3& goalPosition, gkGameObject* target)
{
	bool result = true;

	const Vec3 goalOffset = goalPosition - position();
	const float goalDistance = goalOffset.length ();

	if (goalDistance > std::numeric_limits<float>::epsilon())
	{

		const float sideThreshold = radius() * 8.0f;
		const float behindThreshold = radius() * 2.0f;

		const Vec3 goalDirection = goalOffset / goalDistance;

		const bool goalIsAside = isAside(goalPosition, 0.5);

		OTHERS::iterator it = gkSteeringObject::m_others.begin();

		while (it != gkSteeringObject::m_others.end())
		{
			gkSteeringObject& e = **it;

			if (this->m_obj != e.m_obj && target != e.m_obj && e.speed() > std::numeric_limits<gkScalar>::epsilon())
			{
				const float eDistance = Vec3::distance (position(), e.position());
				const float timeEstimate = 0.3f * eDistance / e.speed(); //xxx
				const Vec3 eFuture = e.predictFuturePosition (timeEstimate);
				const Vec3 eOffset = eFuture - position();
				const float alongCorridor = goalDirection.dot (eOffset);
				const bool inCorridor = ((alongCorridor > -behindThreshold) &&
				                         (alongCorridor < goalDistance));
				const float eForwardDistance = forward().dotProduct(eOffset);

				// consider as potential blocker if within the corridor
				if (inCorridor)
				{
					const Vec3 perp = eOffset - (goalDirection * alongCorridor);
					const float acrossCorridor = perp.length();
					if (acrossCorridor < sideThreshold)
					{
						// not a blocker if behind us and we are perp to corridor
						const float eFront = eForwardDistance + (e.radius () * 0.9f);

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
	}

	return result;
}


// adjust obstacle avoidance look ahead time: make it large when we are far
// from the goal and heading directly towards it, make it small otherwise.
gkScalar gkSteeringObject::adjustObstacleAvoidanceLookAhead(bool clearPath, const gkVector3& goalPosition, gkScalar minPredictTime, gkScalar maxPredictTime)
{
	gkScalar avoidancePredictTime = 0;

	if (clearPath)
	{
		const float goalDistance = Vec3::distance(goalPosition, position());
		const bool headingTowardGoal = isAhead(goalPosition, 0.98f);
		const bool isNear = speed() > std::numeric_limits<gkScalar>::epsilon() ? (goalDistance / speed()) < maxPredictTime : false;
		const bool useMax = headingTowardGoal && !isNear;
		avoidancePredictTime = (useMax ? maxPredictTime : minPredictTime);
	}
	else
	{
		avoidancePredictTime = minPredictTime;
	}

	GK_ASSERT(avoidancePredictTime);

	return avoidancePredictTime;
}

Vec3 gkSteeringObject::steerToEvadeOthers(const gkGameObject* target)
{
	// sum up weighted evasion
	Vec3 evade (0, 0, 0);

	OTHERS::iterator it = gkSteeringObject::m_others.begin();

	while (it != gkSteeringObject::m_others.end())
	{
		gkSteeringObject& e = **it;

		if (this->m_obj != e.m_obj && target != e.m_obj && e.speed() > std::numeric_limits<float>::epsilon())
		{
			const Vec3 eOffset = e.position() - position();
			const float eDistance = eOffset.length();

			GK_ASSERT(eDistance >= std::numeric_limits<float>::epsilon());

			// xxx maybe this should take into account e's heading? xxx
			const float timeEstimate = 0.5f * eDistance / e.speed(); //xxx
			const Vec3 eFuture = e.predictFuturePosition (timeEstimate);

			// steering to flee from eFuture (enemy's future position)
			const Vec3 flee = xxxsteerForFlee(eFuture);

			const float eForwardDistance = forward().dotProduct(eOffset);
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

// QQQ ad hoc speed limitation based on path orientation...
// QQQ should be renamed since it is based on more than curvature
float gkSteeringObject::maxSpeedForCurvature(gkScalar minimumTurningRadius) const
{
	GK_ASSERT(minimumTurningRadius > std::numeric_limits<gkScalar>::epsilon());

	// compute an ad hoc "relative curvature"
	const float absC = absXXX (curvature ());
	const float maxC = 1 / minimumTurningRadius;
	const float relativeCurvature = sqrtXXX (clip (absC / maxC, 0, 1));

	// map from full throttle when straight to 10% at max curvature
	float maxRelativeSpeed = interpolate (relativeCurvature, 1.0f, 0.1f);

	return maxSpeed () * maxRelativeSpeed;
}

// xxx library candidate
// xxx assumes (but does not check or enforce) heading is unit length
//
Vec3 gkSteeringObject::steerTowardHeading (const Vec3& desiredGlobalHeading) const
{
	const Vec3 headingError = desiredGlobalHeading - forward ();
	return headingError.normalize () * maxForce ();
}

// QQQ ad hoc speed limitation based on path orientation...
// QQQ should be renamed since it is based on more than curvature
float gkSteeringObject::maxSpeedForCurvature(const gkNavPath* path, gkScalar minimumTurningRadius, int pathFollowDirection) const
{
	GK_ASSERT(minimumTurningRadius > std::numeric_limits<gkScalar>::epsilon());

	// compute an ad hoc "relative curvature"
	const float absC = absXXX (curvature ());
	const float maxC = 1 / minimumTurningRadius;
	const float relativeCurvature = sqrtXXX (clip (absC / maxC, 0, 1));

	// map from full throttle when straight to 10% at max curvature
	const float curveSpeed = interpolate (relativeCurvature, 1.0f, 0.1f);

	// heading (unit tangent) of the path segment of interest
	const Vec3 pathHeading =  mapPointAndDirectionToTangent( *path, position(), pathFollowDirection );
	// measure how parallel we are to the path
	const float parallelness = pathHeading.dot (forward ());

	// determine relative speed for this heading
	const float mw = 0.2f;
	const float headingSpeed = ((parallelness < 0) ? mw :
	                            interpolate (parallelness,
	                                    mw, 1.0f));

	float maxRelativeSpeed = minXXX (curveSpeed, headingSpeed);

	return maxSpeed () * maxRelativeSpeed;
}

// compute a "look ahead time" with two components, one based on
// minimum time to (say) a collision and one based on minimum distance
// arg 1 is "seconds into the future", arg 2 is "meters ahead"
gkScalar gkSteeringObject::combinedLookAheadTime (float minTime, gkScalar minDistance) const
{
	if (speed () < std::numeric_limits<gkScalar>::epsilon()) return 0;
	return maxXXX (minTime, minDistance / speed ());
}

gkVector3 gkSteeringObject::steerToFollowPathLinear (const int direction, const float predictionTime, const gkNavPath& path)
{
	// our goal will be offset from our path distance by this amount
	const float pathDistanceOffset = direction * predictionTime * speed();

	// predict our future position
	const Vec3 futurePosition = predictFuturePosition (predictionTime);

	// measure distance along path of our current and predicted positions
	const float nowPathDistance =
	    path.mapPointToPathDistance (position ());

	// are we facing in the correction direction?
	const Vec3 pathHeading = mapPointToTangent( path, position() ) * static_cast< float >( direction );
	const bool correctDirection = pathHeading.dot (forward ()) > 0;

	// find the point on the path nearest the predicted future position
	// XXX need to improve calling sequence, maybe change to return a
	// XXX special path-defined object which includes two Vec3s and a
	// XXX bool (onPath,tangent (ignored), withinPath)
	float futureOutside;
	const Vec3 onPath = mapPointToPointOnCenterLineAndOutside( path, futurePosition, futureOutside );

	// determine if we are currently inside the path tube
	float nowOutside;
	const Vec3 nowOnPath = mapPointToPointOnCenterLineAndOutside( path, position(), nowOutside );

	// no steering is required if our present and future positions are
	// inside the path tube and we are facing in the correct direction
	const float m = radius ();
	const bool whollyInside = (futureOutside < m) && (nowOutside < m);

	if (whollyInside && correctDirection)
	{
		// all is well, return zero steering
		return Vec3::zero;
	}
	else
	{
		// otherwise we need to steer towards a target point obtained
		// by adding pathDistanceOffset to our current path position
		// (reduce the offset if facing in the wrong direction)
		const float targetPathDistance = (nowPathDistance +
		                                  (pathDistanceOffset *
		                                   (correctDirection ? 1 : 0.1f)));
		Vec3 target = path.mapPathDistanceToPoint (targetPathDistance);


		// if we are on one segment and target is on the next segment and
		// the dot of the tangents of the two segments is negative --
		// increase the target offset to compensate the fold back
		const int ip =  static_cast< int >( mapPointToSegmentIndex( path, position() ) );
		const int it =  static_cast< int >( mapPointToSegmentIndex( path, target ) );
		// Because polyline paths have a constant tangent along a segment
		// just set the distance along the segment to @c 0.0f.
		Vec3 const ipTangent = path.mapSegmentDistanceToTangent( ip, 0.0f );
		// Because polyline paths have a constant tangent along a segment
		// just set the distance along the segment to @c 0.0f.
		Vec3 const itTangent = path.mapSegmentDistanceToTangent( it, 0.0f );
		if (((ip + direction) == it) &&
		        ( itTangent.dot( ipTangent ) < -0.1f ) )
		{
			const float newTargetPathDistance =
			    nowPathDistance + (pathDistanceOffset * 2);
			target = path.mapPathDistanceToPoint (newTargetPathDistance);
		}

		// if we are currently outside head directly in
		// (QQQ new, experimental, makes it turn in more sharply)
		if (nowOutside > 0) return steerForSeek (nowOnPath);

		// steering to seek target on path
		const Vec3 seek = steerForSeek (target).truncateLength(maxForce());

		// return that seek steering -- except when we are heading off
		// the path (currently on path and future position is off path)
		// in which case we put on the brakes.
		if ((nowOutside < 0) && (futureOutside > 0))
			return (seek.perpendicularComponent (forward ()) -
			        (forward () * maxForce ()));
		else
			return seek;
	}
}

// QQQ This is to work around the bug that scanObstacleMap's current
// QQQ arguments preclude the driving straight [curvature()==0] case.
// QQQ This routine returns the current vehicle path curvature, unless it
// QQQ is *very* close to zero, in which case a small positive number is
// QQQ returned (corresponding to a radius of 100,000 meters).
// QQQ
// QQQ Presumably it would be better to get rid of this routine and
// QQQ redesign the arguments of scanObstacleMap
//
float gkSteeringObject::nonZeroCurvatureQQQ() const
{
	const float c = curvature ();
	const float minCurvature = 1.0f / 100000.0f; // 100,000 meter radius
	const bool tooSmall = (c < minCurvature) && (c > -minCurvature);
	return (tooSmall ? minCurvature : c);
}


Vec3 gkSteeringObject::adjustSteeringForMinimumTurningRadius (const Vec3& steering, gkScalar minimumTurningRadius)
{
	const float maxCurvature = 1 / (minimumTurningRadius * 1.1f);

	// are we turning more sharply than the minimum turning radius?
	if (absXXX (curvature ()) > maxCurvature)
	{
		// remove the tangential (non-thrust) component of the steering
		// force, replace it with a force pointing away from the center
		// of curvature, causing us to "widen out" easing off from the
		// minimum turing radius
		const float signedRadius = 1 / nonZeroCurvatureQQQ ();
		const float sign = signedRadius < 0 ? 1.0f : -1.0f;
		const Vec3 thrust = steering.parallelComponent (forward ());
		const Vec3 trimmed = thrust.truncateLength (maxForce ());
		const Vec3 widenOut = side () * maxForce () * sign;

		return trimmed + widenOut;
	}

	// otherwise just return unmodified input
	return steering;
}


OpenSteer::Vec3 gkSteeringObject::mapPointAndDirectionToTangent(gkNavPath const& pathway, OpenSteer::Vec3 const& point, int direction) const
{
	return OpenSteer::mapPointAndDirectionToTangent(pathway, point, direction);
}

bool gkSteeringObject::isNearWaypoint(gkNavPath const& pathway, OpenSteer::Vec3 const& point) const
{
	return OpenSteer::isNearWaypoint(pathway, point);
}

gkString gkSteeringObject::getDebugStringState() const
{
	gkString str(m_obj->getName() + ":");

	switch (m_state)
	{
	case UNKNOWN:
		str += "UNKNOWN";
		break;

	case AVOIDING:
		str += "AVOIDING";
		break;

	case EVADING:
		str += "EVADING";
		break;

	case IN_GOAL:
		str += "IN_GOAL";
		break;

	case SEEKING:
		str += "SEEKING";
		break;

	case FOLLOWING_PATH:
		str += "FOLLOWING_PATH";
		break;

	case STUCK:
		str += "STUCK";
		break;

	default:
		GK_ASSERT(false);
	}

	return str;
}
