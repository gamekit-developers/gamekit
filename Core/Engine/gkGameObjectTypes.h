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
#ifndef _gkGameObjectTypes_h_
#define _gkGameObjectTypes_h_

#include "gkCommon.h"
#include "OgrePrerequisites.h"
#include "gkMathUtils.h"




// ---------------------------------------------------------------------------
typedef enum gkGameObjectTypes
{
	GK_OB_NULL= 0,
	GK_CAMERA,
	GK_LIGHT,
	GK_ENTITY,
	GK_OBJECT,
	GK_ARMATURE,
}gkGameObjectTypes;

// ---------------------------------------------------------------------------
enum gkObjectTransformSpace
{
	TRANSFORM_LOCAL,
	TRANSFORM_PARENT,
	TRANSFORM_WORLD,
};


// ---------------------------------------------------------------------------
class gkGameObjectProperties
{
public:

	GK_INLINE gkGameObjectProperties() :
			position(Ogre::Vector3::ZERO),
			scale(Ogre::Vector3::UNIT_SCALE),
			orientation(Ogre::Quaternion::IDENTITY),
			isStatic(false), showAABB(false)
	{
	}

	Ogre::Vector3 position, scale;
	Ogre::Quaternion orientation;
	bool isStatic, showAABB;
};


// ---------------------------------------------------------------------------
class gkPhysicsProperties
{
public:

	GK_INLINE gkPhysicsProperties() :
			enable(false), rigid(false), dyn(false), nosleep(false),
			size(Ogre::Vector3::ZERO), radius(1.0), mass(1.0), restitution(0.0),
			friction(0.5), adamp(0.1), ldamp(0.04),
			inertia(0.4), margin(0.06), type(SH_SPH)
	{
	}

	bool enable, rigid, dyn, nosleep;
	Ogre::Vector3 size;
	Ogre::Real radius, mass;
	Ogre::Real restitution, friction;
	Ogre::Real adamp, ldamp;
	Ogre::Real inertia, margin;

	unsigned char type;


	enum Shape
	{
		SH_NONE= 0,
		SH_BOX,
		SH_CONE,
		SH_CYL,
		SH_SPH,
		SH_SMESH,
	};
};





#endif//_gkGameObjectTypes_h_
