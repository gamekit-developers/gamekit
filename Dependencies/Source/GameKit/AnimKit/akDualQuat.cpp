/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

    Contributor(s): none yet.
-------------------------------------------------------------------------------
    Written by Xavier T. based on origninal code under the followind lisence:
    (note that this is a complete rewrite and not an altered copy of Kavan's code.)
    
    Version 1.0.0, February 7th, 2007

    Copyright (C) 2006-2007 University of Dublin, Trinity College, All Rights 
    Reserved

    This software is provided 'as-is', without any express or implied
    warranty.  In no event will the author(s) be held liable for any damages
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

    Author: Ladislav Kavan, kavanl@cs.tcd.ie

-------------------------------------------------------------------------------
*/

#include "akDualQuat.h"

akDualQuat::akDualQuat() : nondual(akQuat::identity()), dual(0,0,0,0)
{
}

akDualQuat::akDualQuat(akQuat rot, akVector3 trans)
{
	nondual = rot;
	dual.setX(0.5*( trans[0]*rot[3] + trans[1]*rot[2] - trans[2]*rot[1]));
	dual.setY(0.5*(-trans[0]*rot[2] + trans[1]*rot[3] + trans[2]*rot[0]));
	dual.setZ(0.5*( trans[0]*rot[1] - trans[1]*rot[0] + trans[2]*rot[3]));
	dual.setW(-0.5*(trans[0]*rot[0] + trans[1]*rot[1] + trans[2]*rot[2]));
}

akTransform3 akDualQuat::toTransform3(void) const
{
	akTransform3 t;
//	float len2 = dot(nondual, nondual);
//	float w = nondual.getW(), x = nondual.getX(), y = nondual.getY(), z = nondual.getZ();
//	float t0 = dual.getX(), t1 = dual., t2 = Qd.z, t3 = Qd.w;
		
//	t[0][0] = w*w + x*x - y*y - z*z; t[0][1] = 2*x*y - 2*w*z; t[0][2] = 2*x*z + 2*w*y;
//	t[1][0] = 2*x*y + 2*w*z; t[1][1] = w*w + y*y - x*x - z*z; t[1][2] = 2*y*z - 2*w*x; 
//	t[2][0] = 2*x*z - 2*w*y; t[2][1] = 2*y*z + 2*w*x; t[2][2] = w*w + z*z - x*x - y*y;
	
//	t[0][3] = -2*t0*x + 2*w*t1 - 2*t2*z + 2*y*t3;
//	t[1][3] = -2*t0*y + 2*t1*z - 2*x*t3 + 2*w*t2;
//	t[2][3] = -2*t0*z + 2*x*t2 + 2*w*t3 - 2*t1*y;
	
//	t /= len2;
	
	return t;	
}
