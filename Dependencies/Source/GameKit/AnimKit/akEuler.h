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

#ifndef AKEULER_H
#define AKEULER_H

#include "utCommon.h"

#include "akMathUtils.h"

class akEuler
{
public:
	// Angles in radians
	akScalar x, y, z;

public:
	UT_INLINE akEuler()
		:       x(0), y(0), z(0)
	{
	}

	UT_INLINE akEuler(const Vector3& v)
		:       x(v.getX()), y(v.getY()), z(v.getZ())
	{
	}

	UT_INLINE akEuler(const akScalar& rx, const akScalar& ry, const akScalar& rz)
		:       x(rx), y(ry), z(rz)
	{
	}

	UT_INLINE akEuler(const Quat& q)
	{
		Matrix3 matr(q);		
		fromMatrix3(matr);
	}

	UT_INLINE akEuler(const Matrix3& v)
	{
		fromMatrix3(v);
	}
	
	UT_INLINE Vector3 toVector3(void) const
	{
		return Vector3(x, y, z);
	}
	
	UT_INLINE Quat toQuaternion(void) const
	{
		return Quat(toMatrix3());
	}

	UT_INLINE Matrix3 toMatrix3(void) const
	{		
		return Matrix3::rotationZYX(Vector3(x,y,z));
	}
	
	UT_INLINE void fromMatrix3(const Matrix3& v)
	{
		// rot =  cy*cz           cz*sx*sy-cx*sz  cx*cz*sy+sx*sz
		//        cy*sz           cx*cz+sx*sy*sz -cz*sx+cx*sy*sz
		//       -sy              cy*sx           cx*cy
		
		akScalar rfYAngle;
		akScalar rfPAngle;
		akScalar rfRAngle;
		
		rfPAngle = sin(-v[2][0]);
		if ( rfPAngle < akPih )
		{
			if ( rfPAngle > -akPih )
			{
				rfYAngle = atan2(v[1][0],v[0][0]);
				rfRAngle = atan2(v[2][1],v[2][2]);
				return;
			}
			else
			{
				// WARNING.  Not a unique solution.
				akScalar fRmY = atan2(-v[0][1],v[0][2]);
				rfRAngle = 0.0;  // any angle works
				rfYAngle = rfRAngle - fRmY;
				return;
			}
		}
		else
		{
			// WARNING.  Not a unique solution.
			akScalar fRpY = atan2(-v[0][1],v[0][2]);
			rfRAngle = 0.0;  // any angle works
			rfYAngle = fRpY - rfRAngle;
			return;
		}
	}

};

#endif // AKEULER_H
