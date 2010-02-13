/*
-------------------------------------------------------------------------------
	This file is part of OgreKit.
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
#ifndef _OgreTransformState_h_
#define _OgreTransformState_h_

#include "OgreUtilsCommon.h"
#include "OgreMathUtils.h"

namespace Ogre {

	
class TransformState
{
public:

	TransformState() {}
	TransformState(const Vector3 &oloc, const Quaternion &orot, const Vector3 &oscl)
	{
		rot.w= orot.w;
		rot.x= orot.x; rot.y= orot.y; rot.z= orot.z;
		loc.x= oloc.x; loc.y= oloc.y; loc.z= oloc.z;
		scl.x= oscl.x; scl.y= oscl.y; scl.z= oscl.z;
	}

	OGRE_INLINE const TransformState &operator= (const TransformState &o)
	{
		rot.w= o.rot.w;
		rot.x= o.rot.x; rot.y= o.rot.y; rot.z= o.rot.z;
		loc.x= o.loc.x; loc.y= o.loc.y; loc.z= o.loc.z;
		scl.x= o.scl.x; scl.y= o.scl.y; scl.z= o.scl.z;
		return *this;
	}

	OGRE_INLINE bool operator != (const TransformState &o) const
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

	OGRE_INLINE void setIdentity(void)
	{
		rot.w= Real(1.0);
		rot.x= Real(0.0); rot.y= Real(0.0); rot.z= Real(0.0);
		loc.x= Real(0.0); loc.y= Real(0.0); loc.z= Real(0.0);
		scl.x= Real(1.0); scl.y= Real(1.0); scl.z= Real(1.0);
	}

	OGRE_INLINE void	toMatrix(Matrix4 &m) const	{ m.makeTransform(loc, scl, rot); }
	OGRE_INLINE Matrix4 toMatrix(void) const		{ Matrix4 m; m.makeTransform(loc, scl, rot); return m;}


	Quaternion	rot;
	Vector3		loc;
	Vector3		scl;
};

}

#endif//_OgreTransformState_h_
