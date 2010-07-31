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
