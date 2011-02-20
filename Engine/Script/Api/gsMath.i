/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

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
%{
#include "gsMath.h"
%}
%include "gsMacros.i"

GS_SCRIPT_NAME(Quaternion)      
GS_SCRIPT_NAME(Ray)             
GS_SCRIPT_NAME(Vector3)         
		     
GS_SCRIPT_NAME(Vec3Negate)      
GS_SCRIPT_NAME(Vec3AddVec3)     
GS_SCRIPT_NAME(Vec3SubVec3)     
GS_SCRIPT_NAME(Vec3MulVec3)     
GS_SCRIPT_NAME(Vec3Mulf)        
GS_SCRIPT_NAME(Vec3DivVec3)     
GS_SCRIPT_NAME(Vec3Divf)        
GS_SCRIPT_NAME(QuatNegate)      
GS_SCRIPT_NAME(QuatAddQuat)     
GS_SCRIPT_NAME(QuatSubQuat)     
GS_SCRIPT_NAME(QuatMulQuat)     
GS_SCRIPT_NAME(QuatMulf)        
GS_SCRIPT_NAME(QuatMulVec3)     
GS_SCRIPT_NAME(Vec3RotationTo) 

%include "gsMath.h"

