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
#include "Ogre.h"
#include "gkSteering.h"

gkSteering::gkSteering(const gkVector3& steerAxis)
	: m_steerAxis(steerAxis)
{
}

gkSteering::~gkSteering()
{
}

gkRadian gkSteering::getAngle(const gkVector3& from, const gkVector3& to)
{
	Ogre::Vector3 from1 = getProjectionOnPlane(from, m_steerAxis);

	Ogre::Vector3 to1 = getProjectionOnPlane(to, m_steerAxis);

	Ogre::Quaternion q = from1.getRotationTo(to1);

	Ogre::Radian angle;
	Ogre::Vector3 rAxis;

	q.ToAngleAxis(angle, rAxis);

	rAxis = angle.valueRadians() * (rAxis * m_steerAxis);

	angle = rAxis.x + rAxis.y + rAxis.z;

	return angle;
}

gkQuaternion gkSteering::getRotation(const gkVector3& from, const gkVector3& to)
{
	return gkQuaternion(getAngle(from, to), m_steerAxis);
}

gkVector3 gkSteering::getProjectionOnPlane(const gkVector3& V, const gkVector3& N)
{
	//U = V - (V dot N)N

	return  V - V.dotProduct(N) * N;
}
