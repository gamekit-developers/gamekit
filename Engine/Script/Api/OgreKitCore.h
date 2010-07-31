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
#ifndef _OgreKitCore_h_
#define _OgreKitCore_h_

#include "OgreKitCommon.h"
#include "OgreKitMath.h"
#include "OgreKitUtils.h"


namespace OgreKit
{

// Class decls
class Property;
class UserDefs;
class Engine;
class Entity;
class Camera;
class Light;
class Scene;
class GameObject;
class EngineEventObject;
class Mouse;
class Keyboard;


enum RenderSystem
{
	OGRE_RS_GL,
	OGRE_RS_GLES,
	OGRE_RS_D3D9,
	OGRE_RS_D3D10,
	OGRE_RS_D3D11,
};

enum PropertyType
{
	PROP_NULL,
	PROP_BOOL,
	PROP_NUMBER,
	PROP_STRING
};

enum TransformSpace
{
	TS_LOCAL,
	TS_PARENT,
	TS_WORLD,
};

enum GameObjectTypes
{
	OB_UNKNOWN=0,
	OB_CAMERA,
	OB_LIGHT,
	OB_ENTITY,
	OB_EMPTY,
	OB_SKELETON,
};

enum EngineEvents
{
	EVT_TICK,
};

enum MouseButton
{
	LEFT = 0,
	RIGHT,
	MIDDLE,
};


// Class OgreKit.Property
class Property
{
public:
	Property();

	// Class OgreKit.Property.__constructor(string, bool)
	Property(const String &name, bool value);

	// Class OgreKit.Property.__constructor(string, number)
	Property(const String &name, double value);

	// Class OgreKit.Property.__constructor(string, string)
	Property(const String &name, const String &value);

	// Class OgreKit.Property.__constructor(OgreKit.Property)
	Property(const Property &oth);

	// OgreKit.Property.__gc()
	~Property();

	// string OgreKit.Property:getName()
	const String &getName(void) const;

	// string OgreKit.Property:getValue()
	const String &getValue(void) const;

	// nil OgreKit.Property:makeDebug()
	void makeDebug(bool v);


	// OgreKit.PropertyType OgreKit.Property:getType()
	PropertyType getType(void) const;

	// bool OgreKit.Property:toBool()
	bool toBool(void) const;

	// number OgreKit.Property:toNumber()
	double  toNumber(void) const;

	// string OgreKit.Property:toString()
	String toString(void) const;

	// nil OgreKit.Property:fromNumber(bool)
	void fromBool(bool v);

	// nil OgreKit.Property:fromNumber(number)
	void  fromNumber(double v);

	// nil OgreKit.Property:fromString(string)
	void fromString(const String &v);


	// internal wrap
	OGRE_KIT_WRAP_CLASS_COPY_CTOR(Property, gkVariable, m_prop);
};


// Class OgreKit.UserDefs
class UserDefs
{
private:
	typedef utHashTable<gkHashedString, Property *> PropertyMap;
	PropertyMap m_properties;


	void setValueEvent(gkVariable &v);


public:

	// OgreKit.UserDefs.__gc()
	~UserDefs();

	// OgreKit.Property OgreKit.UserDefs:getProperty(string)
	const Property &getProperty(const String &name);

	// nil OgreKit.UserDefs:addProperty(OgreKit.Property)
	void addProperty(const Property &prop);

	// bool OgreKit.UserDefs:hasProperty(string)
	bool hasProperty(const String &name);

	// operators []
	// local v = OgreKit.UserDefs._Prop
	// OgreKit.UserDefs._Prop = v

	const Property     &__getitem__(const char *name);
	void                __setitem__(const char *name, bool  v);
	void                __setitem__(const char *name, double v);
	void                __setitem__(const char *name, const String &v);

	// internal wrap
	OGRE_KIT_WRAP_CLASS_COPY_CTOR(UserDefs, gkUserDefs, m_defs);
};



class Mouse
{
public:
	Mouse();
	~Mouse();

	float   xpos, ypos;
	float   xrel, yrel;
	float   winx, winy;
	float   wheel;
	bool    moved;

	void capture(void);
	bool isButtonDown(MouseButton btn);
};

class Keyboard
{
public:
	Keyboard();
	~Keyboard();
	void capture(void) {}

	bool isKeyDown(int sc);
};



// Class OgreKit.Engine
class Engine
#ifndef SWIG
	: public gkEngine::Listener
#endif
{
private:

	bool m_ctxOwner, m_running;
	UserDefs *m_defs;
	typedef utArray<EngineEventObject *> Connectors;

	Connectors m_ticks;

	void tick(gkScalar rate);

public:

	Engine();
	~Engine();

	void initialize(void);
	void run(void);
	void requestExit(void);

	// connect global function
	void connect(int evt, Function func);

	// connect self:function
	void connect(int evt, Self self, Function method);


	Pointer<Scene> loadBlendFile(const String &name);


	UserDefs &getUserDefs(void);


	// internal wrap
	OGRE_KIT_WRAP_CLASS(gkEngine, m_engine);
};


class Loadable
{
public:
	Loadable();


	// nil Loadable:load()
	void load(void);

	// nil Loadable:unload()
	void unload(void);

	// nil Loadable:reload()
	void reload(void);

	// String Loadable:getName()
	String getName(void);

	// internal wrap
	OGRE_KIT_WRAP_CLASS_COPY_CTOR(Loadable, gkObject, m_object);
	OGRE_KIT_INTERNAL_CAST(m_object);
};



class Scene : public Loadable
{
public:
	Scene();

	bool hasObject(const String &name);

	Pointer<GameObject> getObject(const String &name);
	Pointer<Entity>     getEntity(const String &name);
	Pointer<Camera>     getCamera(const String &name);
	Pointer<Light>      getLight(const String &name);



	Pointer<GameObject> createEmpty(const String &name);
	// \todo all the rest



	// internal
	OGRE_KIT_WRAP_BASE_COPY_CTOR(Scene, gkObject);

};


class GameObject : public Loadable
{
public:
	GameObject();

	Vector3 getPosition(void);

	// Math.Vector3 GameObject:getRotation()
	Vector3 getRotation(void);

	// Math.Quaternion GameObject:getOrientation()
	Quaternion getOrientation(void);

	// Math.Vector3 GameObject:getScale()
	Vector3 getScale(void);

	// Math.Vector3 GameObject:getWorldPosition()
	Vector3 getWorldPosition(void);

	// Math.Vector3 GameObject:getWorldRotation()
	Vector3 getWorldRotation(void);

	// Math.Quaternion GameObject:getWorldOrientation()
	Quaternion getWorldOrientation(void);

	// Math.Vector3 GameObject:getLinearVelocity()
	Vector3 getLinearVelocity(void);

	// Math.Vector3 GameObject:getAngularVelocity()
	Vector3 getAngularVelocity(void);

	// nil GameObject:setLinearVelocity(Math.Vector3)
	void setLinearVelocity(const Vector3 &v);
	void setLinearVelocity(float x, float y, float z);

	// nil GameObject:setAngularVelocity(Math.Vector3)
	void setAngularVelocity(const Vector3 &v);
	void setAngularVelocity(float x, float y, float z);

	// nil GameObject:setPosition(Math.Vector3)
	void setPosition(const Vector3 &v);
	void setPosition(float x, float y, float z);

	// nil GameObject:setRotation(Math.Vector3)
	void setRotation(const Vector3 &v);
	void setRotation(float yaw, float pitch, float roll);

	// nil GameObject:setOrientation(Math.Quaternion)
	void setOrientation(const Quaternion &quat);
	void setOrientation(float w, float x, float y, float z);

	// GameObjectTypes GameObject:getType()
	GameObjectTypes getType(void);

	void rotate(float dx, float dy, float dz);
	void rotate(const Vector3 &v);
	void rotate(const Quaternion &v);
	void rotate(float dx, float dy, float dz, TransformSpace ts);
	void rotate(const Vector3 &v, TransformSpace ts);
	void rotate(const Quaternion &v, TransformSpace ts);


	void translate(float x, float y, float z);
	void translate(const Vector3 &v);
	void translate(float x, float y, float z, TransformSpace ts);
	void translate(const Vector3 &v, TransformSpace ts);


	// nil GameObject:scale(Math.Vector3)
	void scale(const Vector3 &v);
	void scale(float x, float y, float z);

	// nil GameObject:yaw(Number, bool=false)
	void yaw(float deg);
	void yaw(float deg, TransformSpace ts);

	// nil GameObject:pitch(Number, bool=false)
	void pitch(float deg);
	void pitch(float deg, TransformSpace ts);

	// nil GameObject:roll(Number, bool=false)
	void roll(float deg);
	void roll(float deg, TransformSpace ts);

	int getState(void);


	/// entity data
	Pointer<Entity> getEntity(void);
	/// camera data
	Pointer<Camera> getCamera(void);
	/// camera data
	Pointer<Light> getLight(void);

	bool hasParent();
	void setParent(const Pointer<GameObject>& par);
	Pointer<GameObject> getParent(void);

	void enableContacts(bool v);
	bool hasContacts();
	bool hasContact(const String &object);


	// Scene GameObject:getScene()
	Pointer<Scene> getScene(void);

	// property access
	Property    __getitem__(const String &prop);
	void        __setitem__(const String &prop, bool  v);
	void        __setitem__(const String &prop, float v);
	void        __setitem__(const String &prop, const char *v);

	// internal
	OGRE_KIT_WRAP_BASE_COPY_CTOR(GameObject, gkObject);
};


class Light : public GameObject
{
public:
	Light();
	// internal
	OGRE_KIT_WRAP_BASE_COPY_CTOR(Light, gkObject);
};


class Camera : public GameObject
{
public:
	Camera();

	void setClipping(float start, float end);
	float getClipStart();
	float getClipEnd();

	void setFov(float fov);
	float getFov();


	void makeCurrent();

	// internal
	OGRE_KIT_WRAP_BASE_COPY_CTOR(Camera, gkObject);
};



class Entity : public GameObject
{
public:

	Entity();

	void playAction(const String &name, float blend);

	// internal
	OGRE_KIT_WRAP_BASE_COPY_CTOR(Entity, gkObject);
};



// Debug utilities
class Debugger
{
protected:
	class gkDebugger *m_debugger;

public:

	Debugger(const Pointer<Scene>& sc);
	~Debugger() {}

	void drawLine(const Vector3 &from, const Vector3 &to, const Vector3 &color);
	void drawObjectAxis(const Pointer<GameObject> &ptr, float size);

	void clear(void);
};


extern void DebugPrint(const char *str);



}
#endif//_OgreKitApi_h_
