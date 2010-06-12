/*
-------------------------------------------------------------------------------
--                                                                           --
--  This file is part of GameKit.                                            --
--  http://gamekit.googlecode.com/                                           --
--                                                                           --
--                                                                           --
--  Released under the zlib license.                                         --
--  http://www.gzip.org/zlib/zlib_license.html                               --
--                                                                           --
--                                                                           --
-------------------------------------------------------------------------------
*/
#ifndef _gkLuaRayTest_h_
#define _gkLuaRayTest_h_


#include "gkLuaGameObject.h"


class RayTest
{
private:

    class gkRayTest *m_ray;
    
public:
    RayTest();
    ~RayTest();


    bool cast(const Vector3 &from, const Vector3 &dir);


    bool cast(float fx, float fy, float fz, float tx, float ty, float tz)
    {
        return cast(Vector3(fx, fy, fz), Vector3(tx, ty, tz));
    }

    Vector3         getHitPoint(void);
	GameObjectPtr   getObject(void);
    float           getHitFraction(void);
};

extern RayTestPtr newRayTest(void);


#endif//_gkLuaRayTest_h_