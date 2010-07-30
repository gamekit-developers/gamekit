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
-- OgreKit Physics Bindings
*/
#include "OgreKitCommon.h"
#include "OgreKitMath.h"
#include "OgreKitUtils.h"
#include "Script/Lua/gkLuaUtils.h"


namespace OgreKit
{
class GameObject;


class RayTest
{
private:
    gkRayTest *m_ray;

public:
    RayTest();
    ~RayTest();

    bool cast(const Vector3 &origin, const Vector3 &dir);

    bool cast(float ox, float oy, float oz, float dx, float dy, float dz)
    { return cast(Vector3(ox, oy, oz), Vector3(dx, dy, dz)); }

    Vector3                 getHitPoint(void);
    Vector3                 getHitNormal(void);
	Pointer<GameObject>     getObject(void);
    float                   getHitFraction(void);
};



class SweptTest
{
private:
    gkSweptTest *m_test;
    gkSweptTest::AVOID_LIST empty_fixme;

public:
    SweptTest(const Pointer<GameObject> &avoid);
    ~SweptTest();

    bool collides(const Ray &ray, float rayRadius);

    Vector3 getHitPoint(void);
    Vector3 getHitNormal(void);
    Vector3 getReflection(void);
    Vector3 getSliding(void);

    Pointer<GameObject> getObject(void);

};

}
