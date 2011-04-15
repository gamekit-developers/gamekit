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

