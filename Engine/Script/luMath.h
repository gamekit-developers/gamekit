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
#ifndef _luMath_h_
#define _luMath_h_

#include "luUtils.h"
#include "gkMathUtils.h"

class luVector3 : public luClass
{
public:
    static luMethodDef Methods[];
    static luTypeDef Type;

    gkVector3 vec;

public:
    luVector3(const gkVector3 &vec);
    ~luVector3();
    luTypeDef *getType(void) { return &Type; }
};

class luQuat : public luClass
{
public:
    static luMethodDef Methods[];
    static luTypeDef Type;

    gkQuaternion quat;

public:
    luQuat(const gkQuaternion &_q);
    ~luQuat();

    luTypeDef *getType(void) { return &Type; }
};

UT_INLINE bool          LU_IsVec3(luObject &L, int n)                   { return L.typecheck(n, &luVector3::Type); }
UT_INLINE bool          LU_IsQuat(luObject &L, int n)                   { return L.typecheck(n, &luQuat::Type); }
UT_INLINE gkVector3&    LU_GetVec3Arg(luObject &L, int n)               { return L.toclassT<luVector3>(n)->vec; }
UT_INLINE gkQuaternion& LU_GetQuatArg(luObject &L, int n)               { return L.toclassT<luQuat>(n)->quat; }
UT_INLINE int           LU_Vec3New(luObject &L, const gkVector3 &v)     { new(&luVector3::Type, L) luVector3(v); return 1;}
UT_INLINE int           LU_QuatNew(luObject &L, const gkQuaternion &v)  { new(&luQuat::Type, L) luQuat(v); return 1; }




extern void luMath_Open(ltState *L);

#endif//_luMath_h_
