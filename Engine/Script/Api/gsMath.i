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

%rename(Quaternion)   gsQuaternion;
%rename(Ray)          gsRay;
%rename(Vector3)      gsVector3;

%rename(Vec3Negate)   gsVec3Negate;
%rename(Vec3AddVec3)  gsVec3AddVec3;
%rename(Vec3SubVec3)  gsVec3SubVec3;
%rename(Vec3MulVec3)  gsVec3MulVec3;
%rename(Vec3Mulf)     gsVec3Mulf;
%rename(Vec3DivVec3)  gsVec3DivVec3;
%rename(Vec3Divf)     gsVec3Divf;
%rename(QuatNegate)   gsQuatNegate;
%rename(QuatAddQuat)  gsQuatAddQuat;
%rename(QuatSubQuat)  gsQuatSubQuat;
%rename(QuatMulQuat)  gsQuatMulQuat;
%rename(QuatMulf)     gsQuatMulf;
%rename(QuatMulVec3)  gsQuatMulVec3;


%include "gsMath.h"

