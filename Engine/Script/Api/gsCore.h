/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): Jonathan.
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
#ifndef _gsCore_h_
#define _gsCore_h_

#include "gsCommon.h"
#include "gsMath.h"
#include "gsUtils.h"



enum gsRenderSystem
{
	GS_RS_GL,
	GS_RS_GLES,
	GS_RS_D3D9,
	GS_RS_D3D10,
	GS_RS_D3D11,
};



enum gsPropertyType
{
	PROP_NULL,
	PROP_BOOL,
	PROP_NUMBER,
	PROP_STRING
};



enum gsTransformSpace
{
	TS_LOCAL,
	TS_PARENT,
	TS_WORLD,
};



enum gsGameObjectTypes
{
	OBT_UNKNOWN = 0,
	OBT_CAMERA,
	OBT_LIGHT,
	OBT_ENTITY,
	OBT_EMPTY,
	OBT_SKELETON,
};



enum gsEngineEvents
{
	EVT_TICK,
};



enum gsMouseButton
{
	LEFT = 0,
	RIGHT,
	MIDDLE,
};


enum gsCompositorOp 
{ 
	COP_ADD = 0, 
	COP_DEL, 
	COP_REPLACE, 
	COP_RESET 
};


class gsProperty
{
private:
	bool m_creator;

public:
	gsProperty();
	gsProperty(const gkString& name, bool value);
	gsProperty(const gkString& name, double value);
	gsProperty(const gkString& name, const gkString& value);
	gsProperty(const gsProperty& oth);
	~gsProperty();

	const gkString& getName(void) const;
	const gkString& getValue(void) const;
	void makeDebug(bool v);
	gsPropertyType getType(void) const;
	bool toBool(void) const;
	double  toNumber(void) const;
	gkString toString(void) const;
	void fromBool(bool v);
	void  fromNumber(double v);
	void fromString(const gkString& v);


	// internal wrap
	OGRE_KIT_WRAP_CLASS_COPY_CTOR(gsProperty, gkVariable, m_prop);
};


class gsUserDefs
{
private:
	typedef utHashTable<gkHashedString, gsProperty*> PropertyMap;
	PropertyMap m_properties;

	void setValueEvent(gkVariable& v);
public:
	~gsUserDefs();

	const gsProperty& getProperty(const gkString& name);
	void addProperty(const gsProperty& prop);
	bool hasProperty(const gkString& name);

	const gsProperty&    __getitem__(const char* name);
	void                __setitem__(const char* name, bool  v);
	void                __setitem__(const char* name, double v);
	void                __setitem__(const char* name, const gkString& v);

	// internal wrap
	OGRE_KIT_WRAP_CLASS_COPY_CTOR(gsUserDefs, gkUserDefs, m_defs);
};



class gsMouse
{
public:
	gsMouse();
	~gsMouse();

	float   xpos, ypos;
	float   xrel, yrel;
	float   winx, winy;
	float   wheel;
	bool    moved;

	void capture(void);
	bool isButtonDown(int btn);
};

extern int getNumJoysticks(void);

class gsJoystick
{
	gkJoystick m_joystick;
	int m_index;
	
public:
	gsJoystick(int i);
	~gsJoystick();
	
	void capture(void);
	
	int getNumAxes();
	int getAxis(int i);
	int getRelAxis(int i);

	gsVector3 getAccel();
	
	int getNumButtons();
	int getButtonCount();
	bool isButtonDown(int i);
	bool wasButtonPressed(int i);
	
	int getWinWidth(void);
	int getWinHeight(void);
};


class gsKeyboard
{
public:
	gsKeyboard();
	~gsKeyboard();
	void capture(void) {}

	bool isKeyDown(int sc);
};




class gsEngine
#ifndef SWIG
	: public gkEngine::Listener
#endif
{
private:

	bool m_ctxOwner, m_running;
	gsUserDefs* m_defs;
	typedef utArray<gkLuaEvent*> Connectors;

	Connectors m_ticks;

	void tick(gkScalar rate);

public:

	gsEngine();
	~gsEngine();

	void initialize(void);
	void run(void);
	void requestExit(void);
	void saveTimestampedScreenShot(const gkString& filenamePrefix, const gkString& filenameSuffix);

	// connect global function
	void connect(int evt, gsFunction func);

	// connect self:function
	void connect(int evt, gsSelf self, gsFunction method);


	gkScene* loadBlendFile(const gkString& name);
	gkScene* getActiveScene(void);

	void unloadBlendFile(const gkString& name);
	void unloadAllBlendFiles();

	gsUserDefs& getUserDefs(void);

	// internal wrap
	OGRE_KIT_WRAP_CLASS(gkEngine, m_engine);
};




class gsObject
{
public:
	gsObject();

	void createInstance(void);
	void destroyInstance(void);
	void reinstance(void);

	gkString getName(void);

	// internal wrap
	OGRE_KIT_WRAP_CLASS_COPY_CTOR(gsObject, gkInstancedObject, m_object);
	OGRE_KIT_INTERNAL_CAST(m_object);
};

class gsHUD
{
public:
	gsHUD();

	void show(bool v);

	gkHUDElement* getChild(const gkString& child);

	// internal wrap
	OGRE_KIT_WRAP_CLASS_COPY_CTOR(gsHUD, gkHUD, m_object);
	OGRE_KIT_INTERNAL_CAST(m_object);
	//OGRE_KIT_TEMPLATE_NEW_INLINE(gsHUD, gkHUD);
};

class gsHUDElement
{
public:
	gsHUDElement();

	void show(bool v);

	gkString getValue();
	void setValue(const gkString& value);

	void setUvCoords(float u1, float v1, float u2, float v2);
	
	gkString getParameter(const gkString& name);
	void setParameter(const gkString& name, const gkString& value);

	gkString getMaterialName();
	void setMaterialName(const gkString& material);

	float getMaterialAlpha();
	void setMaterialAlpha(float factor);

	int getMaterialAlphaRejectValue();
	void setMaterialAlphaRejectValue(int val, bool isGreater);

	// internal wrap
	OGRE_KIT_WRAP_CLASS_COPY_CTOR(gsHUDElement, gkHUDElement, m_object);
	OGRE_KIT_INTERNAL_CAST(m_object);
	//OGRE_KIT_TEMPLATE_NEW_INLINE(gsHUDElement, gkHUDElement);
};

extern gkHUD* getHUD(const gkString& name);

class gsScene : public gsObject
{
private:
	gsArray<gsGameObject, gkGameObject> m_objectCache;

public:
	gsScene();
	~gsScene();


	bool hasObject(const gkString& name);

	gkGameObject* getObject(const gkString& name);
	gkGameObject* createEmpty(const gkString& name);
    gkGameObject* cloneObject(gsGameObject* obj, int lifeSpan, bool instantiate);

	gsArray<gsGameObject, gkGameObject> &getObjectList(void);

	gsDynamicsWorld* getDynamicsWorld(void);

	// internal
	OGRE_KIT_WRAP_BASE_COPY_CTOR(gsScene, gkInstancedObject);
};

extern gkScene* getActiveScene(void);

class gsGameObject : public gsObject
{
public:
	gsGameObject();
	~gsGameObject() {}

	gsVector3 getPosition(void);
	gsVector3 getRotation(void);
	gsQuaternion getOrientation(void);
	gsVector3 getScale(void);
	gsVector3 getWorldPosition(void);
	gsVector3 getWorldRotation(void);
	gsQuaternion getWorldOrientation(void);
	gsVector3 getLinearVelocity(void);
	gsVector3 getAngularVelocity(void);

	void setLinearVelocity(const gsVector3& v);
	void setLinearVelocity(float x, float y, float z);

	void setAngularVelocity(const gsVector3& v);
	void setAngularVelocity(float x, float y, float z);

	void setPosition(const gsVector3& v);
	void setPosition(float x, float y, float z);

	void setRotation(const gsVector3& v);
	void setRotation(float pitch, float yaw, float roll);

	void setOrientation(const gsQuaternion& quat);
	void setOrientation(float w, float x, float y, float z);

	gsGameObjectTypes getType(void);

	void rotate(float dx, float dy, float dz);
	void rotate(const gsVector3& v);
	void rotate(const gsQuaternion& v);
	void rotate(float dx, float dy, float dz, gsTransformSpace ts);
	void rotate(const gsVector3& v, gsTransformSpace ts);
	void rotate(const gsQuaternion& v, gsTransformSpace ts);


	void translate(float x, float y, float z);
	void translate(const gsVector3& v);
	void translate(float x, float y, float z, gsTransformSpace ts);
	void translate(const gsVector3& v, gsTransformSpace ts);

	void scale(const gsVector3& v);
	void scale(float x, float y, float z);


	void yaw(float deg);
	void yaw(float deg, gsTransformSpace ts);


	void pitch(float deg);
	void pitch(float deg, gsTransformSpace ts);


	void roll(float deg);
	void roll(float deg, gsTransformSpace ts);

	int getState(void);
	void changeState(int v);

	bool hasParent();
	void setParent(gsGameObject* par);
	void setParentInPlace(gsGameObject* par);
	void clearParent(void);
	void clearParentInPlace(void);
	void addChild(gsGameObject* chi);
	void removeChild(gsGameObject* chi);


	gkGameObject* getParent(void);

	void enableContacts(bool v);
	bool hasContacts();
	bool hasContact(const gkString& object);

	gkScene* getScene(void);
	
	void playAnimation(const gkString& name, float blend);

	// actually just calling not lua-enabled method!
	gsProperty getProperty(const gkString& name) {return __getitem__(name);}
	void        setProperty(const gkString& prop, bool  v) {__setitem__(prop,v);}
	void       setProperty(const gkString& prop, float v){__setitem__(prop,v);}
	void        setProperty(const gkString& prop, gkString v){__setitem__(prop,v.c_str());}

	gsProperty  __getitem__(const gkString& prop);
	void        __setitem__(const gkString& prop, bool  v);
	void        __setitem__(const gkString& prop, float v);
	void        __setitem__(const gkString& prop, const char* v);


	// internal
	OGRE_KIT_WRAP_BASE_COPY_CTOR(gsGameObject, gkInstancedObject);
	OGRE_KIT_TEMPLATE_CAST(gkGameObject, m_object);
//	OGRE_KIT_TEMPLATE_NEW(gsGameObject, gkGameObject);
};



class gsLight : public gsGameObject
{
public:
	gsLight();
	~gsLight() {}
	// internal
	OGRE_KIT_WRAP_BASE_COPY_CTOR(gsLight, gkInstancedObject);
};



class gsCamera : public gsGameObject
{
public:
	gsCamera();
	~gsCamera() {}

	void setClipping(float start, float end);
	float getClipStart();
	float getClipEnd();

	void setFov(float fov);
	float getFov();

	void makeCurrent();

	// internal
	OGRE_KIT_WRAP_BASE_COPY_CTOR(gsCamera, gkInstancedObject);
};

class gsCharacter;

class gsEntity : public gsGameObject
{
public:

	gsEntity();
	~gsEntity() {}

	bool hasCharacter(void);
	gkCharacter* getCharacter(void);

	// internal
	OGRE_KIT_WRAP_BASE_COPY_CTOR(gsEntity, gkInstancedObject);
};



class gsSkeleton : public gsGameObject
{
public:
	gsSkeleton();
	~gsSkeleton() {}


	// internal
	OGRE_KIT_WRAP_BASE_COPY_CTOR(gsSkeleton, gkInstancedObject);
};

class gsParticles : public gsGameObject
{
public:
	gsParticles();
	~gsParticles() {}


	// internal
	OGRE_KIT_WRAP_BASE_COPY_CTOR(gsParticles, gkInstancedObject);
};



class gsDebugger
{
protected:
	class gkDebugger* m_debugger;

public:

	gsDebugger(gsScene* sc);
	~gsDebugger() {}

	void drawLine(const gsVector3& from, const gsVector3& to, const gsVector3& color);
	void drawObjectAxis(gsGameObject* ptr, float size);

	void clear(void);
};

extern void sendMessage(const char* from,const char* to,const char* subject,const char* body);

extern void gsDebugPrint(const char* str);

extern bool gsSetCompositorChain(gsCompositorOp op, const gkString& compositorName);

#endif//_OgreKitApi_h_
