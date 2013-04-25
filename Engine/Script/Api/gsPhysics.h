/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Charlie C.

    Contributor(s): Thomas Trocha(dertom)
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
#ifndef _gsPhysics_h_
#define _gsPhysics_h_

#include "gsCommon.h"
#include "gsMath.h"
#include "gsUtils.h"
#include "Script/Lua/gkLuaUtils.h"


/** \addtogroup Physics
	@{
*/

class gsDynamicsWorld
{
private:
	gkDynamicsWorld* m_world;

public:
	/**
	*/
	gsDynamicsWorld(gkDynamicsWorld* dynamicsWorld);
	~gsDynamicsWorld();
	/**
	*/
	void exportBullet(const gkString& fileName);
};


class gsRayTest
{
private:
	gkRayTest* m_ray;

public:
	/**
		\LuaMethod{RayTest,constructor}

		Construct a new test.

		\code
		function RayTest:constructor()
		\endcode
	*/
	gsRayTest(gsScene* scene=0);
	~gsRayTest();

	/**
		\LuaMethod{RayTest,cast}

		Do the test.

		\code
		function RayTest:cast(ray)
		\endcode

		\param ray \LuaClassRef{Ray} as the starting position and heading.
		\returns bool True if the ray has hit.
	*/
	bool cast(gsRay& ray, const gkString& prop="", gsGameObject* excludeObject=0);

	/**
		\LuaMethod{RayTest,getHitPoint}

		Returns the hit point.

		\code
		function RayTest:getHitPoint()
		\endcode

		\returns \LuaClassRef{Vector3}
	*/
	gsVector3 getHitPoint(void);

	/**
		\LuaMethod{RayTest,getHitNormal}

		Returns the hit normal.

		\code
		function RayTest:getHitNormal()
		\endcode

		\returns \LuaClassRef{Vector3}
	*/
	gsVector3 getHitNormal(void);

	/**
		\LuaMethod{RayTest,getObject}

		Returns the hit object.

		\code
		function RayTest:getObject()
		\endcode

		\returns \LuaClassRef{GameObject}
	*/
	gkGameObject* getObject(void);

	/**
		\LuaMethod{RayTest,getHitFraction}

		Returns the hit fraction.

		\code
		function RayTest:getHitFraction()
		\endcode

		\returns number
	*/
	float getHitFraction(void);
};




class gsSweptTest
{
private:
	gkSweptTest* m_test;
	gkSweptTest::AVOID_LIST m_avoid;

public:
	/**
		\LuaMethod{SweptTest,constructor}

		Construct a new test.

		\code
		function SweptTest:constructor()
		\endcode
	*/
	gsSweptTest();
	/**
		\sectionseperator{Overload:}

		Construct a new test with a list of objects to avoid.

		\code
		function SweptTest:constructor(avoid)
		\endcode

		\param avoid \LuaClassRef{ObjectList}
	*/
	gsSweptTest(gsArray<gsGameObject, gkGameObject> &avoid);
	~gsSweptTest();

	/**
		\LuaMethod{SweptTest,collides}

		Do the test.

		\code
		function SweptTest:collides(ray, radius)
		\endcode

		\param ray \LuaClassRef{Ray} as the starting position and heading.
		\param radius Radius of the collision sphere.

		\returns bool True if the test has hit.
	*/
	bool collides(const gsRay& ray, float rayRadius);
	/**
		\LuaMethod{SweptTest,getHitPoint}

		Returns the hit point.

		\code
		function SweptTest:getHitPoint()
		\endcode

		\returns \LuaClassRef{Vector3}
	*/
	gsVector3 getHitPoint(void);
	/**
		\LuaMethod{SweptTest,getHitNormal}

		Returns the hit normal.

		\code
		function SweptTest:getHitNormal()
		\endcode

		\returns \LuaClassRef{Vector3}
	*/
	gsVector3 getHitNormal(void);
	/**
		\LuaMethod{SweptTest,getReflection}

		Returns the reflection after hit.

		\code
		function SweptTest:getReflection()
		\endcode

		\returns \LuaClassRef{Vector3}
	*/
	gsVector3 getReflection(void);
	/**
		\LuaMethod{SweptTest,getSliding}

		Returns sliding vector along the plane normal.


		\code
		function SweptTest:getSliding()
		\endcode

		\returns \LuaClassRef{Vector3}
	*/
	gsVector3 getSliding(void);
	/**
		\LuaMethod{SweptTest,getObject}

		Returns the hit object.

		\code
		function SweptTest:getObject()
		\endcode

		\returns \LuaClassRef{GameObject}
		*/
	gkGameObject* getObject(void);
};

class gsCharacter
{
private:
    gkCharacter* m_character;
		
public:
	/**

	*/
    gsCharacter(gkCharacter* object);
    ~gsCharacter(void);
	/**
		\LuaMethod{Character,getObject}

		Returns the object of the character is attached.

		\code
		function Character:getObject()
		\endcode

		\returns \LuaClassRef{GameObject}
	*/
    gkGameObject* getObject(void);
	/**
		\LuaMethod{Character,setLinearVelocity}

		Move the character.

		\code
		function Character:setLinearVelocity(forward, backward, rightward, leftward, timeInterval)
		\endcode

		\param forawd The speed in forward direction.
		\param backward The speed in backward direction.
		\param rightward The speed in rightward direction.
		\param leftward The speed in leftward direction.
		\param timeInterval The character should move for the given time period.
	*/
    void setLinearVelocity(float forward, float backward, float rightward, float leftward, float timeInterval);    
	/**
		\LuaMethod{Character,setGravity}

		Set acceleration of gravity.

		\code
		function Character:setGravity(gravity)
		\endcode

		\param gravity The acceleration of gravity.	
	*/
    void setGravity(float gravity);
	/**
		\LuaMethod{Character,setRotation}

		\code
		function Character:setRotation(axis, scalar)
		\endcode

		Ratate the character.

		\param axis The \LuaClassRef{Vector3} is the axis that the character will rotate.
		\param scalar The amount to ratation.		
	*/
    void setRotation(const gsVector3& axis, float scalar);
	/**
		\LuaMethod{Character,setJumpSpeed}

		Set the character jump speed.

		\code
		function Character:setJumpSpeed()
		\endcode

		\param scalar The speed of jump.
	*/
    void setJumpSpeed(float scalar);
	/**
		\LuaMethod{Character,jump}

		Make the character jump.

		\code
		function Character:jump()
		\endcode
	*/
    void jump(void);
	/**
		\LuaMethod{Character,isOnGround}

		Test the character is on gound.

		\code
		function Character:isOnGround()
		\endcode

		\returns bool True if the character is on ground.
	*/
    bool isOnGround(void);
}; 

/** @} */

#endif //_gsPhysics_h_
