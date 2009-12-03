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
#ifndef _gkConstraint_h_
#define _gkConstraint_h_

#include "gkCommon.h"
#include "OgreString.h"
#include "OgreStringVector.h"
#include "OgreStdHeaders.h"
#include "OgreIteratorWrappers.h"
#include "gkMathUtils.h"






// ----------------------------- gkConstraint Base ------------------------------
class gkConstraint
{
public:

	typedef enum ConstraintTypes
	{
		CONST_NONE= 0,
		CONST_LIMIT_ROT,
		CONST_LIMIT_LOC,
	}ConstraintTypes;
public:

	gkConstraint(ConstraintTypes type);
	virtual ~gkConstraint() {}

	void setInfluence(Ogre::Real v);
	void setLocalSpace();

	bool getLocal();
	Ogre::Real getInfluence();

	virtual bool update(gkGameObject *ob)= 0;

	void setMatrix(const Ogre::Matrix4 &m);
	Ogre::Matrix4 getMatrix();

protected:
	const ConstraintTypes mType;
	bool	mIsLocal;
	Ogre::Real	mInfluence;
	Ogre::Matrix4 mMatrix;
};


// ---------------------------------------------------------------------------
GK_INLINE void gkConstraint::setInfluence(Ogre::Real v)
{
	mInfluence= gkClamp(v, 0.0, 1.0);
}

// ---------------------------------------------------------------------------
GK_INLINE void gkConstraint::setLocalSpace()
{
	mIsLocal= true;
}

// ---------------------------------------------------------------------------
GK_INLINE bool gkConstraint::getLocal()
{
	return mIsLocal;
}

// ---------------------------------------------------------------------------
GK_INLINE Ogre::Real gkConstraint::getInfluence()
{
	return mInfluence;
}

// ---------------------------------------------------------------------------
GK_INLINE void gkConstraint::setMatrix(const Ogre::Matrix4 &m)
{
	mMatrix= m;
}

// ---------------------------------------------------------------------------
GK_INLINE Ogre::Matrix4 gkConstraint::getMatrix()
{
	return mMatrix;
}





#endif//_gkConstraint_h_
