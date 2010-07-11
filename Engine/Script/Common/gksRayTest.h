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
-- Generic Script API Binding                                                --
-------------------------------------------------------------------------------
*/
#ifndef _gksRayTest_h_
#define _gksRayTest_h_

#include "gksGameObject.h"



class gksRayTest
{
private:

    class gkRayTest *m_ray;
    
public:
    gksRayTest();
    ~gksRayTest();


    bool cast(const gksVector3 &from, const gksVector3 &dir);

    bool cast(float fx, float fy, float fz, float tx, float ty, float tz)
    {
        return cast(gksVector3(fx, fy, fz), gksVector3(tx, ty, tz));
    }

    gksVector3                  getHitPoint(void);
	gksPointer<gksGameObject>   getObject(void);
    float                       getHitFraction(void);
};



extern gksPointer<gksRayTest> newRayTest(void);


#endif//_gksRayTest_h_