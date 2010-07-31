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
#include "gkGameObject.h"
#include "gksGameObject.h"
#include "gksScene.h"
#include "gkScene.h"
#include "gkEntity.h"


// ----------------------------------------------------------------------------
gksVector3 gksGameObject::getPosition(void)
{
	if (m_object)
		return gksVector3(cast<gkGameObject>()->getPosition());
	return gksVector3();
}

// ----------------------------------------------------------------------------
// Math.gksVector3 gksGameObject:getRotation()
gksVector3 gksGameObject::getRotation(void)
{
	if (m_object)
		return gksVector3(cast<gkGameObject>()->getRotation().toVector3());
	return gksVector3();
}

// ----------------------------------------------------------------------------
// Math.gksQuaternion gksGameObject:getOrientation()
gksQuaternion gksGameObject::getOrientation(void)
{
	if (m_object)
		return gksQuaternion(cast<gkGameObject>()->getOrientation());
	return gksQuaternion();
}

// Math.gksVector3 gksGameObject:getScale()
gksVector3 gksGameObject::getScale(void)
{
	if (m_object)
		return gksVector3(cast<gkGameObject>()->getScale());
	return gksVector3();
}

// Math.gksVector3 gksGameObject:getWorldPosition()
gksVector3 gksGameObject::getWorldPosition(void)
{
	if (m_object)
		return gksVector3(cast<gkGameObject>()->getWorldPosition());
	return gksVector3();
}

// Math.gksVector3 gksGameObject:getWorldRotation()
gksVector3 gksGameObject::getWorldRotation(void)
{
	if (m_object)
		return gksVector3(cast<gkGameObject>()->getWorldRotation().toVector3());
	return gksVector3();
}

// Math.gksQuaternion gksGameObject:getWorldOrientation()
gksQuaternion gksGameObject::getWorldOrientation(void)
{
	if (m_object)
		return gksQuaternion(cast<gkGameObject>()->getWorldOrientation());
	return gksQuaternion();
}


// Math.gksVector3 gksGameObject:getLinearVelocity()
gksVector3 gksGameObject::getLinearVelocity(void)
{
	if (m_object)
		return gksVector3(cast<gkGameObject>()->getLinearVelocity());
	return gksVector3();
}

// Math.gksVector3 gksGameObject:getAngularVelocity()
gksVector3 gksGameObject::getAngularVelocity(void)
{
	if (m_object)
		return gksVector3(cast<gkGameObject>()->getAngularVelocity());
	return gksVector3();
}

// nil gksGameObject:setLinearVelocity(Math.gksVector3)
void gksGameObject::setLinearVelocity(const gksVector3 &v)
{
	if (m_object)
		cast<gkGameObject>()->setLinearVelocity(v);
}

void gksGameObject::setLinearVelocity(float x, float y, float z)
{
	if (m_object)
		cast<gkGameObject>()->setLinearVelocity(gkVector3(x,y,z));
}

// nil gksGameObject:setAngularVelocity(Math.gksVector3)
void gksGameObject::setAngularVelocity(const gksVector3 &v)
{
	if (m_object)
		cast<gkGameObject>()->setAngularVelocity(v);
}

void gksGameObject::setAngularVelocity(float x, float y, float z)
{
	if (m_object)
		cast<gkGameObject>()->setAngularVelocity(gkVector3(x,y,z));
}

// nil gksGameObject:setPosition(Math.gksVector3)
void gksGameObject::setPosition(const gksVector3 &v)
{
	if (m_object)
		cast<gkGameObject>()->setPosition(v);
}

void gksGameObject::setPosition(float x, float y, float z)
{
	if (m_object)
		cast<gkGameObject>()->setPosition(gkVector3(x,y,z));
}

// nil gksGameObject:setRotation(Math.gksVector3)
void gksGameObject::setRotation(const gksVector3 &v)
{
	if (m_object)
		cast<gkGameObject>()->setOrientation(gkEuler(v));
}

void gksGameObject::setRotation(float yaw, float pitch, float roll)
{
	if (m_object)
		cast<gkGameObject>()->setOrientation(gkEuler(yaw, pitch, roll));
}

// nil gksGameObject:setOrientation(Math.gksQuaternion)
void gksGameObject::setOrientation(const gksQuaternion &quat)
{
	if (m_object)
		cast<gkGameObject>()->setOrientation(quat);
}

void gksGameObject::setOrientation(float w, float x, float y, float z)
{
	if (m_object)
		cast<gkGameObject>()->setOrientation(gkQuaternion(w, x, y, z));
}

// Enum gksGameObject:getType()
gksGameObjectTypes gksGameObject::getType(void)
{
	if (m_object)
		return (gksGameObjectTypes)cast<gkGameObject>()->getType();
	return OB_UNKNOWN;
}


void gksGameObject::rotate(float dx, float dy, float dz)
{
	if (m_object)
		cast<gkGameObject>()->rotate(gkEuler(dx, dy, dz), TRANSFORM_PARENT);
}

void gksGameObject::rotate(const gksVector3 &v)
{
	if (m_object)
		cast<gkGameObject>()->rotate(gkEuler(v), TRANSFORM_PARENT);
}

void gksGameObject::rotate(const gksQuaternion &v)
{
	if (m_object)
		cast<gkGameObject>()->rotate(v, TRANSFORM_PARENT);
}

void gksGameObject::rotate(float dx, float dy, float dz, gksTransformSpace ts)
{
	if (m_object)
		cast<gkGameObject>()->rotate(gkEuler(dx, dy, dz), ts);
}

void gksGameObject::rotate(const gksVector3 &v, gksTransformSpace ts)
{
	if (m_object)
		cast<gkGameObject>()->rotate(gkEuler(v), ts);
}

void gksGameObject::rotate(const gksQuaternion &v, gksTransformSpace ts)
{
	if (m_object)
		cast<gkGameObject>()->rotate(v, ts);
}


void gksGameObject::translate(const gksVector3 &v)
{
	if (m_object)
		cast<gkGameObject>()->translate(v, TRANSFORM_PARENT);
}

void gksGameObject::translate(float x, float y, float z)
{
	if (m_object)
		cast<gkGameObject>()->translate(gkVector3(x,y,z), TRANSFORM_PARENT);
}

void gksGameObject::translate(const gksVector3 &v, gksTransformSpace ts)
{
	if (m_object)
		cast<gkGameObject>()->translate(v, ts);
}

void gksGameObject::translate(float x, float y, float z, gksTransformSpace ts)
{
	if (m_object)
		cast<gkGameObject>()->translate(gkVector3(x,y,z), ts);
}


// nil gksGameObject:scale(Math.gksVector3)
void gksGameObject::scale(const gksVector3 &v)
{
	if (m_object)
		cast<gkGameObject>()->scale(v);
}

void gksGameObject::scale(float x, float y, float z)
{
	if (m_object)
		cast<gkGameObject>()->scale(gkVector3(x,y,z));
}

// nil gksGameObject:yaw(Number, bool=false)
void gksGameObject::yaw(float deg)
{
	if (m_object)
		cast<gkGameObject>()->yaw(gkDegree(deg), TRANSFORM_PARENT);
}

void gksGameObject::yaw(float deg, gksTransformSpace ts)
{
	if (m_object)
		cast<gkGameObject>()->yaw(gkRadian(deg), ts);
}


// nil gksGameObject:pitch(Number, bool=false)
void gksGameObject::pitch(float deg)
{
	if (m_object)
		cast<gkGameObject>()->pitch(gkRadian(deg), TRANSFORM_PARENT);
}

void gksGameObject::pitch(float deg, gksTransformSpace ts)
{
	if (m_object)
		cast<gkGameObject>()->pitch(gkRadian(deg), ts);
}

void gksGameObject::roll(float deg)
{
	if (m_object)
		cast<gkGameObject>()->roll(gkRadian(deg), TRANSFORM_PARENT);
}

void gksGameObject::roll(float deg, gksTransformSpace ts)
{
	if (m_object)
		cast<gkGameObject>()->roll(gkRadian(deg), ts);
}


gksProperty gksGameObject::__getitem__(const char *prop)
{
	if (m_object)
	{
		gkVariable *var = cast<gkGameObject>()->getVariable(prop);
		if (var)
			return gksProperty(var);
	}
	return gksProperty();
}


void gksGameObject::__setitem__(const char *prop, bool  v)
{
	if (m_object)
	{
		gkVariable *var = cast<gkGameObject>()->getVariable(prop);
		if (var)
			(*var).setValue(v);
	}
}

void gksGameObject::__setitem__(const char *prop, float v)
{
	if (m_object)
	{
		gkVariable *var = cast<gkGameObject>()->getVariable(prop);
		if (var)
			(*var).setValue(v);
	}
}


void gksGameObject::__setitem__(const char *prop, const char *v)
{
	if (m_object)
	{
		gkVariable *var = cast<gkGameObject>()->getVariable(prop);
		if (var)
			(*var).setValue(gkString(v));
	}
}


// gksScene gksGameObject:getScene()
gksPointer<gksScene> gksGameObject::getScene(void)
{
	if (m_object)
		return gksPointer<gksScene>(new gksScene(cast<gkGameObject>()->getOwner()));
	return gksPointer<gksScene>();
}

int gksGameObject::getState(void)
{
	if (m_object)
		return cast<gkGameObject>()->getState();
	return -1;
}



gksPointer<gksEntity> gksGameObject::getEntity(void)
{
	if (m_object && cast<gkGameObject>()->getType() == GK_ENTITY)
		return gksPointer<gksEntity>(new gksEntity(cast<gkGameObject>()));
	return gksPointer<gksEntity>();
}


void gksEntity::playAction(const char *name, float blend)
{
	if (m_object)
		cast<gkEntity>()->playAction(name, blend);
}
