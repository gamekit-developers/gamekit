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


class luVector3 : public luClass, public gkVector3
{
    luClassHeader
public:
    ~luVector3();

    static int create(luObject &L, const gkVector3 &vec);

    static UT_INLINE luVector3& getArg(luObject &L, int v)
    {
        return L.toclassRefT<luVector3>(v);
    }

    // gkVector3 meta functions 
    int _normalize(luClass *self, luObject &L);
    int _dot(luClass *self, luObject &L);
    int _cross(luClass *self, luObject &L);
    int _length(luClass *self, luObject &L);
    int _length2(luClass *self, luObject &L);
    int _dist(luClass *self, luObject &L);
    int _dist2(luClass *self, luObject &L);

    int _get(luClass *self, luObject &L);
    int _set(luClass *self, luObject &L);
    int _add(luClass *self, luObject &L);
    int _sub(luClass *self, luObject &L);
    int _mul(luClass *self, luObject &L);
    int _div(luClass *self, luObject &L);
    int _neg(luClass *self, luObject &L);
    int _tostring(luClass *self, luObject &L);
};

class luQuat : public luClass, public gkQuaternion
{
    luClassHeader
public:
    ~luQuat();

    static int create(luObject &L, const gkQuaternion &vec);

    static UT_INLINE luQuat& getArg(luObject &L, int v)
    {
        return L.toclassRefT<luQuat>(v);
    }

    int _normalize(luClass *self, luObject &L);
    int _inverse(luClass *self, luObject &L);
    int _dot(luClass *self, luObject &L);

    int _get(luClass *self, luObject &L);
    int _set(luClass *self, luObject &L);
    int _mul(luClass *self, luObject &L);

    int _tostring(luClass *self, luObject &L);
};


extern void luMath_Open(ltState *L);

#endif//_luMath_h_
