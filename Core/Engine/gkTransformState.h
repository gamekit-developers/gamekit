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
#ifndef _gkTransformState_h_
#define _gkTransformState_h_

#include "gkCommon.h"
#include "gkMathUtils.h"





// ----------------------------------------------------------------------------
class gkTransformState
{
public:

	gkTransformState() {}

	gkTransformState(const Ogre::Vector3& loc, const Ogre::Quaternion& rot, const Ogre::Vector3 &scl);

	const gkTransformState &operator= (const gkTransformState &o);

	bool operator != (const gkTransformState &o);

	gkTransformState blendTransform(const gkTransformState& currentTransform, Ogre::Real fact);

	gkTransformState perdictTransform(const gkTransformState& currentTransform, Ogre::Real fact, Ogre::Real subStep);

	void setIdentity();

	Ogre::Quaternion	rot;
	Ogre::Vector3		loc;
	Ogre::Vector3		scl;
};


// ----------------------------------------------------------------------------
GK_INLINE gkTransformState::gkTransformState(const Ogre::Vector3& oloc, const Ogre::Quaternion& orot, const Ogre::Vector3 &oscl)
{
	rot.w= orot.w;
	rot.x= orot.x; rot.y= orot.y; rot.z= orot.z;
	loc.x= oloc.x; loc.y= oloc.y; loc.z= oloc.z;
	scl.x= oscl.x; scl.y= oscl.y; scl.z= oscl.z;
}

// ----------------------------------------------------------------------------
GK_INLINE const gkTransformState &gkTransformState::operator= (const gkTransformState &o)
{
	rot.w= o.rot.w;
	rot.x= o.rot.x; rot.y= o.rot.y; rot.z= o.rot.z;
	loc.x= o.loc.x; loc.y= o.loc.y; loc.z= o.loc.z;
	scl.x= o.scl.x; scl.y= o.scl.y; scl.z= o.scl.z;
	return *this;
}

// ----------------------------------------------------------------------------
GK_INLINE bool gkTransformState::operator != (const gkTransformState &o)
{
	return ((rot.w != o.rot.w) ||
			(rot.x != o.rot.x) ||
			(rot.y != o.rot.y) ||
			(rot.z != o.rot.z) ||
			(loc.x != o.loc.x) ||
			(loc.y != o.loc.y) ||
			(loc.z != o.loc.z) ||
			(scl.x != o.scl.x) ||
			(scl.y != o.scl.y) ||
			(scl.z != o.scl.z)
		   );
}


// ----------------------------------------------------------------------------
GK_INLINE gkTransformState gkTransformState::blendTransform(const gkTransformState& currentTransform, Ogre::Real fact)
{
	gkTransformState result;
	result.loc= gkMathUtils::interp(loc, currentTransform.loc, fact);
	result.rot= gkMathUtils::interp(rot, currentTransform.rot, fact);
	result.scl= gkMathUtils::interp(scl, currentTransform.scl, fact);
	return result;
}

// ----------------------------------------------------------------------------
GK_INLINE gkTransformState gkTransformState::perdictTransform(const gkTransformState& currentTransform, Ogre::Real intrp, Ogre::Real subStep)
{
	gkTransformState result;
	return result;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkTransformState::setIdentity()
{
	rot.w= Ogre::Real(1.0);
	rot.x= Ogre::Real(0.0); rot.y= Ogre::Real(0.0); rot.z= Ogre::Real(0.0);
	loc.x= Ogre::Real(0.0); loc.y= Ogre::Real(0.0); loc.z= Ogre::Real(0.0);
	scl.x= Ogre::Real(1.0); scl.y= Ogre::Real(1.0); scl.z= Ogre::Real(1.0);
}

#endif//_gkTransformState_h_
