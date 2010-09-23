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
#ifndef _gkSteeringWander_h_
#define _gkSteeringWander_h_

#include "gkSteeringObject.h"
#include "gkGameObject.h"

class gkSceneObstacle;

class gkSteeringWander : public gkSteeringObject
{
public:

	gkSteeringWander(
	    gkGameObject* obj,
	    gkScalar maxSpeed,
	    const gkVector3& forward,
	    const gkVector3& up,
	    const gkVector3& side,
	    gkScalar minPredictionTime,
	    gkScalar maxPredictionTime
	);

	~gkSteeringWander();

	bool steering(STATE& newState, const float elapsedTime);

private:

	gkSceneObstacle* m_sceneObstable;

	OpenSteer::ObstacleGroup m_allObstacles;

	gkScalar m_minPredictionTime;

	gkScalar m_maxPredictionTime;
};

#endif//_gkSteeringWander_h_
