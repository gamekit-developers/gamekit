/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

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

#ifndef AKTRANSFORMSTATE_H
#define AKTRANSFORMSTATE_H

#include "utCommon.h"
#include "akMathUtils.h"

class akTransformState
{
public:
	Quat    rot;
	Vector3 loc;
	Vector3 scale;
	
public:
	akTransformState(const Vector3& oloc = Vector3(0,0,0),
					 const Quat&    orot = Quat::identity(),
					 const Vector3& oscl = Vector3(1,1,1))
	{
		rot = orot;
		loc = oloc;
		scale = oscl;
	}
	
	akTransformState(const Matrix4& mat)
	{
		loc = mat.getTranslation();
		
		Matrix3 m3 = mat.getUpper3x3();
		
		rot = Quat(m3);
		if(norm(rot)==0)
			rot = Quat::identity();
		
		Matrix4 mrot = Matrix4(rot, Vector3(0,0,0));
		Matrix4 T = mat*mrot;
		
		scale = Vector3(T[0][0], T[1][1], T[2][2]);
	}
	
	UT_INLINE Matrix4 toMatrix(void) const
	{
		Transform3 t3(rot, loc);
		appendScale(t3, scale);
		return Matrix4(t3);
	}
	
	UT_INLINE static akTransformState identity()
	{
		return akTransformState(Vector3(0,0,0), Quat::identity(), Vector3(1,1,1));
	}
	
	UT_INLINE const akTransformState& operator= (const akTransformState& o)
	{
		rot = o.rot;
		loc = o.loc;
		scale = o.scale;
		return *this;
	}
	
};

#endif // AKTRANSFORMSTATE_H
