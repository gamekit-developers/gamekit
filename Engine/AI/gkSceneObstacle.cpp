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
#include "gkSceneObstacle.h"
#include "gkGameObject.h"
#include "gkSteeringObject.h"
#include "gkMathUtils.h"
#include "gkRayTest.h"
#include "gkLogger.h"

using namespace OpenSteer;

void gkSceneObstacle::findIntersectionWithVehiclePath(const AbstractVehicle& vehicle, PathIntersection& pi) const
{
	pi.intersect = false;

	Ogre::Ray ray(vehicle.position(), vehicle.forward() * m_howFarCanSee);

	gkRayTest rayTest;

	if (rayTest.collides(ray))
	{
		gkGameObjectProperties& props = rayTest.getObject()->getProperties();

		if (!props.isDynamic())
		{
			pi.intersect = true;
			pi.obstacle = this;
			pi.distance = rayTest.getHitPoint().distance(vehicle.position());

			pi.steerHint = rayTest.getHitNormal();

			if (vehicle.getState() != gkSteeringObject::AVOIDING)
			{
				m_lastDirection = pi.steerHint.dot(vehicle.side());
			}

			if (m_lastDirection > 0)
			{
				pi.steerHint += vehicle.side();
			}
			else if (m_lastDirection < 0)
			{
				pi.steerHint -= vehicle.side();
			}

			pi.steerHint = pi.steerHint.normalize();

			pi.surfacePoint = rayTest.getHitPoint();
			pi.surfaceNormal = rayTest.getHitNormal();
			pi.vehicleOutside = true;
		}
	}
}
