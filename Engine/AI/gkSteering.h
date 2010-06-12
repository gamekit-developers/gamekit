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
#ifndef _gkSteering_h_
#define _gkSteering_h_

#include "gkCommon.h"
#include "gkMathUtils.h"

class gkSteering
{
public:

	gkSteering(const gkVector3& steerAxis);
	~gkSteering();

	gkRadian getAngle(const gkVector3& from, const gkVector3& to);
	gkQuaternion getRotation(const gkVector3& from, const gkVector3& to);

private:

	gkVector3 getProjectionOnPlane(const gkVector3& V, const gkVector3& N);

private:

	gkVector3 m_steerAxis;
};

#endif//_gkSteering_h_
