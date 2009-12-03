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
#ifndef _gkLightObjectTypes_h_
#define _gkLightObjectTypes_h_

#include "gkCommon.h"
#include "gkMathUtils.h"



// ---------------------------------------------------------------------------
class gkLightProperties
{
public:

	gkLightProperties();

	Ogre::ColourValue diffuse, specular;
	int type;
	Ogre::Degree spot_inner, spot_outer;
	Ogre::Vector3 direction;

	Ogre::Real power, falloff, range;
	Ogre::Real constant, linear, quadratic;
	bool casts, extra;

	/* extra data for shaders */
	Ogre::Real param[4];
};


// ---------------------------------------------------------------------------
GK_INLINE gkLightProperties::gkLightProperties() :
		diffuse(Ogre::ColourValue::White), specular(Ogre::ColourValue::White), type(0),
		spot_inner(30), spot_outer(40), direction(Ogre::Vector3::NEGATIVE_UNIT_Z),
		power(1.0), falloff(1.0), range(10000.0), constant(1.0), linear(0.0),
		quadratic(0.0), casts(0.0), extra(false)

{
	param[0]= 0.0; param[1]= 0.0;
	param[2]= 0.0; param[3]= 0.0;
}





#endif//_gkLightObjectTypes_h_
