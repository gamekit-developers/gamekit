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
#ifndef _gkLimitLocConstraint_h_
#define _gkLimitLocConstraint_h_

#include "gkConstraint.h"



// ----------------------------- Limit Location ------------------------------
class gkLimitLocConstraint : public gkConstraint
{
public:

	gkLimitLocConstraint();
	virtual ~gkLimitLocConstraint() {}

	bool update(gkGameObject *ob);

	void setMinX(Ogre::Real v);
	void setMaxX(Ogre::Real v);

	void setMinY(Ogre::Real v);
	void setMaxY(Ogre::Real v);

	void setMinZ(Ogre::Real v);
	void setMaxZ(Ogre::Real v);

private:
	short mFlag[2];

	Ogre::Real x[2];
	Ogre::Real y[2];
	Ogre::Real z[2];
};


// ---------------------------------------------------------------------------
GK_INLINE void gkLimitLocConstraint::setMinX(Ogre::Real v)
{
	mFlag[0] |= 1;
	x[0]= v;
}

// ---------------------------------------------------------------------------
GK_INLINE void gkLimitLocConstraint::setMaxX(Ogre::Real v)
{
	mFlag[1] |= 1;
	x[1]= v;
}

// ---------------------------------------------------------------------------
GK_INLINE void gkLimitLocConstraint::setMinY(Ogre::Real v)
{
	mFlag[0] |= 2;
	y[0]= v;
}

// ---------------------------------------------------------------------------
GK_INLINE void gkLimitLocConstraint::setMaxY(Ogre::Real v)
{
	mFlag[1] |= 2;
	y[1]= v;
}

// ---------------------------------------------------------------------------
GK_INLINE void gkLimitLocConstraint::setMinZ(Ogre::Real v)
{
	mFlag[0] |= 4;
	z[0]= v;
}

// ---------------------------------------------------------------------------
GK_INLINE void gkLimitLocConstraint::setMaxZ(Ogre::Real v)
{
	mFlag[1] |= 4;
	z[1]= v;
}






#endif//_gkLimitLocConstraint_h_
