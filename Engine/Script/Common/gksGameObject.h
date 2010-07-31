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
#ifndef _gksGameObject_h_
#define _gksGameObject_h_

#include "gksConstants.h"
#include "gksObject.h"
#include "gksProperty.h"
#include "gksMath.h"

class gksEntity;
class gksScene;



// ----------------------------------------------------------------------------
class gksGameObject : public gksObject
{
#ifndef SWIG
public:

	gksGameObject(gkObject *oth) : gksObject(oth) {}
#endif

public:

	gksGameObject() : gksObject() {}

	gksVector3 getPosition(void);

	// Math.gksVector3 gksGameObject:getRotation()
	gksVector3 getRotation(void);

	// Math.gksQuaternion gksGameObject:getOrientation()
	gksQuaternion getOrientation(void);

	// Math.gksVector3 gksGameObject:getScale()
	gksVector3 getScale(void);

	// Math.gksVector3 gksGameObject:getWorldPosition()
	gksVector3 getWorldPosition(void);

	// Math.gksVector3 gksGameObject:getWorldRotation()
	gksVector3 getWorldRotation(void);

	// Math.gksQuaternion gksGameObject:getWorldOrientation()
	gksQuaternion getWorldOrientation(void);

	// Math.gksVector3 gksGameObject:getLinearVelocity()
	gksVector3 getLinearVelocity(void);

	// Math.gksVector3 gksGameObject:getAngularVelocity()
	gksVector3 getAngularVelocity(void);

	// nil gksGameObject:setLinearVelocity(Math.gksVector3)
	void setLinearVelocity(const gksVector3 &v);
	void setLinearVelocity(float x, float y, float z);

	// nil gksGameObject:setAngularVelocity(Math.gksVector3)
	void setAngularVelocity(const gksVector3 &v);
	void setAngularVelocity(float x, float y, float z);

	// nil gksGameObject:setPosition(Math.gksVector3)
	void setPosition(const gksVector3 &v);
	void setPosition(float x, float y, float z);

	// nil gksGameObject:setRotation(Math.gksVector3)
	void setRotation(const gksVector3 &v);
	void setRotation(float yaw, float pitch, float roll);

	// nil gksGameObject:setOrientation(Math.gksQuaternion)
	void setOrientation(const gksQuaternion &quat);
	void setOrientation(float w, float x, float y, float z);

	// gksGameObjectTypes gksGameObject:getType()
	gksGameObjectTypes getType(void);

	void rotate(float dx, float dy, float dz);
	void rotate(const gksVector3 &v);
	void rotate(const gksQuaternion &v);
	void rotate(float dx, float dy, float dz, gksTransformSpace ts);
	void rotate(const gksVector3 &v, gksTransformSpace ts);
	void rotate(const gksQuaternion &v, gksTransformSpace ts);


	void translate(float x, float y, float z);
	void translate(const gksVector3 &v);
	void translate(float x, float y, float z, gksTransformSpace ts);
	void translate(const gksVector3 &v, gksTransformSpace ts);


	// nil gksGameObject:scale(Math.gksVector3)
	void scale(const gksVector3 &v);
	void scale(float x, float y, float z);

	// nil gksGameObject:yaw(Number, bool=false)
	void yaw(float deg);
	void yaw(float deg, gksTransformSpace ts);

	// nil gksGameObject:pitch(Number, bool=false)
	void pitch(float deg);
	void pitch(float deg, gksTransformSpace ts);

	// nil gksGameObject:roll(Number, bool=false)
	void roll(float deg);
	void roll(float deg, gksTransformSpace ts);

	int getState(void);

	gksPointer<gksEntity> getEntity(void);


	// Scene gksGameObject:getScene()
	gksPointer<gksScene> getScene(void);

	// property access
	gksProperty __getitem__(const char *prop);
	void        __setitem__(const char *prop, bool  v);
	void        __setitem__(const char *prop, float v);
	void        __setitem__(const char *prop, const char *v);
};


class gksEntity : public gksGameObject
{
#ifndef SWIG
public:

	gksEntity(gkObject *oth) : gksGameObject(oth) {}
#endif

public:

	gksEntity() : gksGameObject() {}

	// todo expose gkAction classes
	void playAction(const char *name, float blend);

};

#endif//_gksGameObject_h_
