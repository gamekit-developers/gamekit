/*
-------------------------------------------------------------------------------
	This file is part of the Ogre GameKit port.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#ifndef _gkCameraObjectTypes_h_
#define _gkCameraObjectTypes_h_

#include "gkCommon.h"
#include "OgrePrerequisites.h"
#include "gkMathUtils.h"




// ----------------------------------------------------------------------------
class gkCameraProperties
{
public:

	gkCameraProperties();

	Ogre::Real clipstart, clipend;
	Ogre::Radian fov;
	bool start;

};

// ----------------------------------------------------------------------------
GK_INLINE gkCameraProperties::gkCameraProperties() :
		clipstart(0.1), clipend(100.0),
		fov(Ogre::Degree(49.13)), start(false)
{
}





#endif//_gkCameraObjectTypes_h_
