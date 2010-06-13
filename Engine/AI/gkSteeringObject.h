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
#ifndef _gkSteeringObject_h_
#define _gkSteeringObject_h_

#include "AbstractVehicle.h"
#include "SteerLibrary.h"

typedef OpenSteer::LocalSpaceMixin<OpenSteer::AbstractVehicle> SimpleSteering1;
typedef OpenSteer::SteerLibraryMixin<SimpleSteering1> SimpleSteering;

class gkGameObject;

class gkSteeringObject : public SimpleSteering
{
public:

	gkSteeringObject(gkGameObject* obj, gkScalar maxSpeed, const gkVector3& forward, const gkVector3& up, const gkVector3& side);

	virtual ~gkSteeringObject();

    OpenSteer::Vec3 side (void) const;
    OpenSteer::Vec3 up (void) const;
	OpenSteer::Vec3 forward (void) const;
	OpenSteer::Vec3 position (void) const;

    float mass() const {return m_mass;}
    float setMass(float m) {return m_mass = m;}

	OpenSteer::Vec3 velocity() const {return forward() * m_speed;}

    float speed() const {return m_speed;}
    float setSpeed(float s) {return m_speed = s;}

    float radius() const {return m_radius;}
    float setRadius(float m) {return m_radius = m;}

    float maxForce() const {return m_maxForce;}
    float setMaxForce(float mf) {return m_maxForce = mf;}

    float maxSpeed(void) const {return m_maxSpeed;}
    float setMaxSpeed(float ms) {return m_maxSpeed = ms;}

	OpenSteer::Vec3 predictFuturePosition(const float predictionTime) const
	{
		return position() + (velocity() * predictionTime);
	}

	virtual void update(gkScalar tick) = 0;
	void update(const float currentTime, const float elapsedTime){};

	bool inGoal() const { return m_inGoal; }

protected:

	void reset();
	void applySteeringForce(const OpenSteer::Vec3& force, const float elapsedTime);
	void applyDirectForce(const OpenSteer::Vec3& force, const float elapsedTime);
	void applyBrakingForce(const float rate, const float deltaTime);
	OpenSteer::Vec3 adjustRawSteeringForce(const OpenSteer::Vec3& force, const float /* deltaTime */);
	void measurePathCurvature(const float elapsedTime);
	bool clearPathToGoal(const gkVector3& goalPosition, gkGameObject* target = 0);
	void adjustObstacleAvoidanceLookAhead(bool clearPath, const gkVector3& goalPosition);
	OpenSteer::Vec3 steerToEvadeOthers(const gkGameObject* target = 0);

protected:

	gkGameObject* m_obj;

	typedef std::set<gkSteeringObject*> OTHERS;
	static OTHERS m_others;

    float m_mass;       // mass (defaults to unity so acceleration=force)

    float m_radius;     // size of bounding sphere, for obstacle avoidance, etc.

    float m_speed;      // speed along Forward direction.  Because local space
                       // is velocity-aligned, velocity = Forward * Speed

    float m_maxForce;   // the maximum steering force this vehicle can apply
                       // (steering force is clipped to this magnitude)

    float m_maxSpeed;   // the maximum speed this vehicle is allowed to move
                       // (velocity is clipped to this magnitude)

    float m_avoidancePredictTimeMin;
    float m_avoidancePredictTimeMax;
    float m_avoidancePredictTime; 

	bool m_evading;
	bool m_avoiding;

	float m_curvature;
    OpenSteer::Vec3 m_lastForward;
    OpenSteer::Vec3 m_lastPosition;
    OpenSteer::Vec3 m_smoothedPosition;
    float m_smoothedCurvature;
	OpenSteer::Vec3 m_smoothedAcceleration;

	bool m_inGoal;

private:

	gkVector3 m_forward;
	gkVector3 m_up;
	gkVector3 m_side;
};

#endif//_gkSteeringObject_h_
