/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Charlie C.

    Contributor(s): Jonathan.
    				Thomas Trocha(dertom)
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
#include "gkMesh.h"
#include "gkSerialize.h"

#ifdef OGREKIT_USE_PROCESSMANAGER
# include "gsProcess.h"
#endif

/** \addtogroup Core
	@{
*/


/**
	RenderSystem Options

	\code
	OgreKit.OGRE_RS_GL,     OpenGL RenderSystem
	OgreKit.OGRE_RS_GLES,   OpenGL ES RenderSystem (TODO)
	OgreKit.OGRE_RS_D3D9,   Direct3D 9 RenderSystem
	OgreKit.OGRE_RS_D3D10,  Direct3D 10 RenderSystem
	OgreKit.OGRE_RS_D3D11,  Direct3D 11 RenderSystem
	\endcode
*/
enum gsRenderSystem
{
	GS_RS_GL,
	GS_RS_GLES,
	GS_RS_D3D9,
	GS_RS_D3D10,
	GS_RS_D3D11,
};

/**

*/
enum gsShaderProgrammType {
	ST_VERTEX,
	ST_FRAGMENT
};

/**
	Property identifiers.

	\code
	OgreKit.PROP_NULL
	OgreKit.PROP_BOOL
	OgreKit.PROP_NUMBER
	OgreKit.PROP_STRING
	\endcode
*/
enum gsPropertyType
{
	PROP_NULL,
	PROP_BOOL,
	PROP_NUMBER,
	PROP_STRING
};


/**
	GameObject transform space.

	\code
	OgreKit.TS_LOCAL
	OgreKit.TS_PARENT
	OgreKit.TS_WORLD
	\endcode
*/
enum gsTransformSpace
{
	TS_LOCAL,
	TS_PARENT,
	TS_WORLD,
};


/**

*/
enum gsGameObjectTypes
{
	OBT_UNKNOWN = 0,
	OBT_CAMERA,
	OBT_LIGHT,
	OBT_ENTITY,
	OBT_EMPTY,
	OBT_SKELETON,
	OBT_CURVE
};


/**
	Engine callback codes.

	\code
	OgreKit.EVT_TICK
	\endcode

	\LuaSee{Engine,connect}
*/
enum gsEngineEvents
{
	EVT_TICK,
};


/**
	Mouse button codes.

	\code
	OgreKit.LEFT
	OgreKit.RIGHT
	OgreKit.MIDDLE
	\endcode
*/
enum gsMouseButton
{
	LEFT = 0,
	RIGHT,
	MIDDLE,
};

/**
	Compositor chain operation.

	\code
	OgreKit.COP_ADD
	OgreKit.COP_DEL
	OgreKit.COP_REPLACE
	OgreKit.COP_RESET 
	\endcode
*/
enum gsCompositorOp 
{ 
	COP_ADD = 0, 
	COP_DEL, 
	COP_REPLACE, 
	COP_RESET 
};
/**

*/
enum gsSceneUpdateFlags
{
	UPDATE_NONE			= 0,
	UPDATE_PHYSICS		= 1 << 0,
	UPDATE_LOGIC_BRICKS	= 1 << 1,
	UPDATE_NODE_TREES	= 1 << 2,
	UPDATE_ANIMATIONS	= 1 << 3,
	UPDATE_SOUNDS		= 1 << 4,
	UPDATE_DBVT			= 1 << 5,
	UPDATE_DEBUG		= 1 << 6,
	UPDATE_ALL			= 0xFFFFFFFF
};


class gsProperty
{
private:
	bool m_creator;

public:
	/**
		\LuaMethod{Property,constructor}

		Creates a nil property.

		\code
		function Property:constructor()
		\endcode
	*/
	gsProperty();
	/**
		\sectionseperator{Overload 1:}

		Creates a new bool property.

		\code
		function Property:constructor(name, bool)
		\endcode

		\param name The property identifier.
		\param bool The boolean value.
	*/
	gsProperty(const gkString& name, bool value);
	/**
		\sectionseperator{Overload 2:}

		Creates a new numerical property.

		\code
		function Property:constructor(name, number)
		\endcode

		\param name The property identifier.
		\param number The numerical value.
	*/
	gsProperty(const gkString& name, double value);
	/**
		\sectionseperator{Overload 3:}

		Creates a new string property.

		\code
		function Property:constructor(name, string)
		\endcode

		\param name The property identifier.
		\param string The string value.
	*/
	gsProperty(const gkString& name, const gkString& value);
	/**

	*/
	gsProperty(const gsProperty& oth);
	~gsProperty();
	/**
		\LuaMethod{Property,getName}

		Returns the property identifier.

		\code
		function Property:getName()
		\endcode

		\returns string.
	*/
	const gkString& getName(void) const;
	/**
		\LuaMethod{Property,getValue}

		Returns the internal string value.

		\code
		function Property:getValue()
		\endcode

		\returns string.
	*/
	const gkString& getValue(void) const;
	/**
		\LuaMethod{Property,makeDebug}

		Adds this property to the render window.  

		\code
		function Property:makeDebug(val)
		\endcode

		\param val Add or remove.
	*/
	void makeDebug(bool v);
	/**
		\LuaMethod{Property,getType}

		Returns the internal type. 

		\code
		function Property:getType()
		\endcode

		\returns \LuaClassRef{PropertyType}.
	*/
	gsPropertyType getType(void) const;
	/**
		\LuaMethod{Property,toBool}

		Return value in boolean form.

		\code
		function Property:toBool()
		\endcode

		\returns bool The new value.
	*/
	bool toBool(void) const;
	/**
		\LuaMethod{Property,toNumber}

		Return value in numerical form.

		\code
		function Property:toNumber()
		\endcode

		\returns number The new value.
	*/
	double  toNumber(void) const;
	/**
		\LuaMethod{Property,toString}

		Return value in string form.

		\code
		function Property:toString()
		\endcode

		\returns string The new value.
	*/
	gkString toString(void) const;
	/**
		\LuaMethod{Property,fromBool}

		Applies value from a boolean value.

		\code
		function Property:fromBool(bool)
		\endcode

		\param bool The new value.
	*/
	void fromBool(bool v);
	/**
		\LuaMethod{Property,fromNumber}

		Applies value from a number.

		\code
		function Property:fromNumber(number)
		\endcode

		\param number The new value.
	*/
	void  fromNumber(double v);
	/**
		\LuaMethod{Property,fromString}

		Applies value from string.

		\code
		function Property:fromString(string)
		\endcode

		\param string The new value.
	*/
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

	/**
		\LuaMethod{UserDefs,getProperty}

		Finds a preexisting property.

		\code
		function UserDefs:getProperty(name)
		\endcode

		\param name lookup.
		\returns \LuaClassRef{Property}.
	*/
	const gsProperty& getProperty(const gkString& name);
	/**
		\LuaMethod{UserDefs,addProperty}

		Adds a new user defined property.

		\code
		function UserDefs:addProperty(property)
		\endcode

		\param property \LuaClassRef{Property}.	
	*/
	void addProperty(const gsProperty& prop);
	/**
		\LuaMethod{UserDefs,hasProperty}

		Looks for a preexisting property.

		\code
		function UserDefs:hasProperty(name)
		\endcode

		\param name lookup.
		\returns bool
	*/
	bool hasProperty(const gkString& name);

	/**
		\LuaMethod{UserDefs,__index}

		Index meta method.

		\code
		function UserDefs:__index(name)
		\endcode

		\param name lookup.
		\returns string, bool, or number. Depending on the type of name.


		\code
		-- Is equal to
		local val = UserDefs.someValue
		\endcode
	*/
	const gsProperty&    __getitem__(const char* name);
	/**
		\LuaMethod{UserDefs,__newindex}

		New index meta method.

		\code
		function UserDefs:__newindex(name, bool)
		\endcode

		\param name lookup.
		\param bool The boolean value to set.


		\code
		-- Is equal to
		UserDefs.someValue = true
		\endcode
	*/
	void                __setitem__(const char* name, bool  v);
	/**
		\sectionseperator{Overload 1:}

		\code
		function UserDefs:__newindex(name, number)
		\endcode

		\param name lookup.
		\param number The numerical value to set.

		\code
		-- Is equal to
		UserDefs.someValue = 123
		\endcode
	*/
	void                __setitem__(const char* name, double v);
	/**
		\sectionseperator{Overload 2:}

		\code
		function UserDefs:__newindex(name, string)
		\endcode

		\param name lookup.
		\param string The string value to set.

		\code
		-- Is equal to
		UserDefs.someValue = 'abc'
		\endcode
	*/
	void                __setitem__(const char* name, const gkString& v);

	// internal wrap
	OGRE_KIT_WRAP_CLASS_COPY_CTOR(gsUserDefs, gkUserDefs, m_defs);
};

class gsMouse
{
public:
	/**
		\LuaMethod{Mouse,constructor}

		Creates a mouse to test mouse events.

		\code
		function Mouse:constructor()
		\endcode
	*/
	gsMouse();
	~gsMouse();

	/**
		\LuaMethod{Mouse,xpos}

		Absolute X position of the mouse within the window.

		\code
		number Mouse.xpos
		\endcode
	*/
	float   xpos;
	/**
		\LuaMethod{Mouse,ypos}
		
		Absolute Y position of the mouse within the window.

		\code
		number Mouse.ypos
		\endcode
	*/
	float	ypos;
	/**
		\LuaMethod{Mouse,yrel}
		
		Relitve Y direction of the mouse within the window.

		\code
		number Mouse.xrel		
		\endcode
	*/
	float   xrel;
	/**	
		\LuaMethod{Mouse,yrel}
		
		Relitve Y direction of the mouse within the window.

		\code
		number Mouse.yrel
		\endcode
	*/
	float	yrel;
	/**
		\LuaMethod{Mouse,winx}		

		Window widht.		

		\code
		number Mouse.winx		
		\endcode
	*/
	float   winx;
	/**		
		\LuaMethod{Mouse,winy}
		
		Window height.

		\code
		number Mouse.winy
		\endcode
	*/
	float	winy;
	/**
		\LuaMethod{Mouse,wheel}

		Mouse wheel delta 

		\code
		number Mouse.wheel
		\endcode	
	*/
	float   wheel;
	/**
		\LuaMethod{Mouse,moved}

		True if the mouse has changed position.

		\code
		bool Mouse.moved
		\endcode	

	*/
	bool    moved;

	/**
		\LuaMethod{Mouse,capture}

		Updates the current state of the device.

		\code
		function Mouse:capture()
		\endcode
	*/
	void capture(void);
	/**
		\LuaMethod{Mouse,isButtonDown}

		Returns true if the specified code is active.

		\code
		function Mouse:isButtonDown(code)
		\endcode

		\param code \LuaClassRef{MouseButton}.
		\returns bool.
	*/
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
	
	/**

	*/
	void capture(void);
	
	/**

	*/
	int getNumAxes();
	/**

	*/
	int getAxis(int i);
	/**

	*/
	int getRelAxis(int i);
	/**

	*/
	gsVector3 getAccel();
	
	/**

	*/
	int getNumButtons();
	/**

	*/
	int getButtonCount();
	/**

	*/
	bool isButtonDown(int i);
	/**

	*/
	bool wasButtonPressed(int i);
	
	/**

	*/
	int getWinWidth(void);
	/**

	*/
	int getWinHeight(void);
};


class gsKeyboard
{
public:
	/**
		Creates a keyboard to test key press events.

		\code
		function Keyboard:constructor()
		\endcode
	*/
	gsKeyboard();
	~gsKeyboard();
	/**

	*/
	void capture(void) {}
	/**

	*/
	void clearKey(int sc);

	/**
		\LuaMethod{Keyboard,isKeyDown}

		Returns true if the specified scan code is active.

		\code
		function Keyboard:isKeyDown(code)
		\endcode

		\param code \LuaClassRef{ScanCode} key code.
		\returns bool.
	*/
	bool isKeyDown(int sc);
	/**

	*/
	bool isKeyUp(int sc);
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

	/**
		\LuaMethod{Engine,constructor}

		Create or get an instance of the engine. 

		\code
		function Engine:constructor()
		\endcode
	*/
	gsEngine();
	~gsEngine();

	/**
		\LuaMethod{Engine,initialize}

		Initialize the engine.

		\code
		function Engine:initialize()
		\endcode
	*/
	void initialize(void);
	/**
		\LuaMethod{Engine,run}

		Run the engine loop.

		\code
		function Engine:run()
		\endcode
	*/
	void run(void);
	/**
		\LuaMethod{Engine,requestExit}

		Sends an exit signal.

		\code
		function Engine:requestExit()
		\endcode
	*/
	void requestExit(void);
	/**
		\LuaMethod{Engine,saveTimestampedScreenShot}

		Save a time stamped screenshot file.

		\code
		function Engine:saveTimestampedScreenShot(filenamePrefix,filenameSuffix)
		\endcode

		\param filenamePrefix Prefix of the screenshot file name.
		\param filenameSuffix Suffix of the screenshot file name with file extention. (ex, '.png')
		\returns 
	*/
	void saveTimestampedScreenShot(const gkString& filenamePrefix, const gkString& filenameSuffix);

	// connect global function
	/**
		\LuaMethod{Engine,connect}

		Connects an event listener.

		\code
		function Engine:connect(type, func)
		\endcode

		\param type \LuaClassRef{EngineEvents} as the event to listen to.
		\param func Global callback function.
	*/
	void connect(int evt, gsFunction func);

	// connect self:function
	/**
		\sectionseperator{Overload:}

		\code
		function Engine:connect(type, table, func)
		\endcode


		\param type \LuaClassRef{EngineEvents} as the event to listen to.
		\param table    Pointer to a Lua table object, ie; self 
		\param function Pointer to a Lua function, method of self. 
	*/
	void connect(int evt, gsSelf self, gsFunction method);

	/**
		\LuaMethod{Engine,loadBlendFile}

		Returns the active scene in the .blend

		\code
		function Engine:loadBlendFile(name)
		\endcode

		\param name Path to the blend file.
		\returns \LuaClassRef{Scene}

		\note The returned scene is \b not loaded.
	*/
	gkScene* loadBlendFile(const gkString& name);
	/**
		\LuaMethod{Engine,getActiveScene}

		Returns the currently loaded scene.

		\code
		function Engine:getActiveScene()
		\endcode

		\returns \LuaClassRef{Scene}
	*/
	gkScene* getActiveScene(void);
	/**

	*/
	gkScene* getScene(const gkString& sceneName);

	/**

	*/
	gkScene* addOverlayScene(gsScene* scene);
	/**

	*/
	gkScene* addOverlayScene(const gkString& name);
	/**

	*/
	gkScene* addBackgroundScene(gsScene* scene);
	/**

	*/
	gkScene* addBackgroundScene(const gkString& name);
	/**

	*/
	void removeScene(gsScene* scene);
	/**

	*/
	void removeScene(const gkString& name);

#ifndef SWIG
	gkScene* addOverlayScene(gkScene* scene);
	gkScene* addBackgroundScene(gkScene* scene);
	void removeScene(gkScene* scene);
#endif
	/**
		\LuaMethod{Engine,unloadBlendFile}

		Unload the .blend file and scene.

		\code
		function Engine:unloadBlendFile(name)
		\endcode

		\param name Path to the blend file.
	*/
	void unloadBlendFile(const gkString& name);
	/**
		\LuaMethod{Engine,unloadAllBlendFiles}

		Unload all .blend files and scenes.

		\code
		function Engine:unloadAllBlendFiles()
		\endcode	
	*/
	void unloadAllBlendFiles();

	/**
		\LuaMethod{Engine,getUserDefs}

		Returns the user defined options class. 

		\code
		function Engine:getUserDefs()
		\endcode

		\returns \LuaClassRef{UserDefs}
	*/
	gsUserDefs& getUserDefs(void);

	// internal wrap
	OGRE_KIT_WRAP_CLASS(gkEngine, m_engine);
};




class gsObject
{
public:
	gsObject();

	/**
		\LuaMethod{Object,createInstance}

		Adds this object.

		\code
		function Object:createInstance()
		\endcode
	*/
	void createInstance(void);
	/**
		\LuaMethod{Object,destroyInstance}

		Removes this object.

		\code
		function Object:destroyInstance()
		\endcode
	*/
	void destroyInstance(void);
	/**
		\LuaMethod{Object,reinstance}

		Removes this object, then adds it back.

		\code
		function Object:reinstance()
		\endcode
	*/
	void reinstance(void);

	/**
		\LuaMethod{Object,getName}

		Returns the string identifier of this object.

		\code
		function Object:getName()
		\endcode

		\returns string
	*/
	gkString getName(void);

	// internal wrap
	OGRE_KIT_WRAP_CLASS_COPY_CTOR(gsObject, gkInstancedObject, m_object);
	OGRE_KIT_INTERNAL_CAST(m_object);
};

class gsHUD
{
public:
	gsHUD();
	/**
		\LuaMethod{HUD,show}

		Show or hide all HUDElements in the HUD.

		\code
		function HUD:show(v)
		\endcode

		\param v bool.
	*/
	void show(bool v);

	/**
		\LuaMethod{HUD,getChild}

		return a child \LuaClassRef{HUDElement} in the HUD.

		\code
		function HUD:getChild(name)
		\endcode

		\param name the string  identifier of child.

		\returns \LuaClassRef{HUDElement}
	*/
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
	/**
		\LuaMethod{HUDElement,show}

		Show or hide the HUDElement.

		\code
		function HUDElement:show(v)
		\endcode

		\param v bool
	*/
	void show(bool v);
	/**
		\LuaMethod{HUDElement,getValue}

		Gets the caption for this element, , if support. 

		\code
		function HUDElement:getValue()
		\endcode

		\returns the caption string.
	*/
	gkString getValue();
	/**
		\LuaMethod{HUDElement,setValue}

		Sets the caption for this element, if support. 

		\code
		function HUDElement:setValue(value)
		\endcode

		\param value the caption string
	*/
	void setValue(const gkString& value);
	/**
		\LuaMethod{HUDElement,setUvCoords}

		Sets the texture coordinates for the HUDElement. 

		\code
		function HUDElement:setUvCoords(u1,v1,u2,v2)
		\endcode

		\param u1 number
		\param v1 number 
		\param u2 number 
		\param v2 number 

		\returns 
	*/
	void setUvCoords(float u1, float v1, float u2, float v2);
	/**
		\LuaMethod{HUDElement,getParameter}

		Generic parameter retrieval method. 

		\code
		function HUDElement:getParameter(name)
		\endcode

		\param name The name of the parameter to get 

		\returns String value of parameter, blank if not found 
	*/
	gkString getParameter(const gkString& name);
	/**
		\LuaMethod{HUDElement,setParameter}

		Generic parameter setting method. 

		\code
		function HUDElement:setParameter(name,value)
		\endcode

		\param name The name of the parameter to set 
		\param value string value

		\returns 
	*/
	void setParameter(const gkString& name, const gkString& value);
	/**
		\LuaMethod{HUDElement,getMaterialName}

		\code
		function HUDElement:getMaterialName()
		\endcode

		\returns Gets the name of the material this element uses
	*/
	gkString getMaterialName();
	/**
		\LuaMethod{HUDElement,setMaterialName}

		Sets the name of the material this element will use. 

		\code
		function HUDElement:setMaterialName(materialName)
		\endcode

		\param materialName the name of the material 
	*/
	void setMaterialName(const gkString& material);
	/**
		\LuaMethod{HUDElement,getMaterialAlpha}

		Gets the alpha of the texture this element's material.

		\code
		function HUDElement:getMaterialAlpha()
		\endcode

		\returns a number in [0-1]
	*/
	float getMaterialAlpha();
	/**
		\LuaMethod{HUDElement,setMaterialAlpha}

		Sets the alpha of the texture this element's material.

		\code
		function HUDElement:setMaterialAlpha(alpha)
		\endcode

		\param alpha a number in [0-1]
	*/
	void setMaterialAlpha(float factor);
	/**
		\LuaMethod{HUDElement,getMaterialAlphaRejectValue}

		Gets the alpha reject value. 

		\code
		function HUDElement:getMaterialAlphaRejectValue()
		\endcode

		\returns a integer number in [0-255]
	*/
	int getMaterialAlphaRejectValue();
	/**
		\LuaMethod{HUDElement,setMaterialAlphaRejectValue}

		Sets the alpha reject value. 

		\code
		function HUDElement:setMaterialAlphaRejectValue(value, isGreater)
		\endcode

		\param value a integer number in [0-255]
		\param isGreater bool	
	*/
	void setMaterialAlphaRejectValue(int val, bool isGreater);
	/**

	*/
	gsVector3 getPosition();
	/**

	*/
	void setPosition(gsVector3 pos);
	/**

	*/
	void setPosition(float x,float y);

	// internal wrap
	OGRE_KIT_WRAP_CLASS_COPY_CTOR(gsHUDElement, gkHUDElement, m_object);
	OGRE_KIT_INTERNAL_CAST(m_object);
	//OGRE_KIT_TEMPLATE_NEW_INLINE(gsHUDElement, gkHUDElement);

private:
	gsVector3 mPosition;
};

/**
	\LuaClass{getHUD}

	Retrieves the named \LuaClassRef{HUD}

	\code
	function OgreKit.getHUD(name)
	\endcode

	\param name the \LuaClassRef{HUD} name.

	\returns the named \LuaClassRef{HUD}
*/
extern gkHUD* getHUD(const gkString& name);

class gsScene : public gsObject
{
private:
	gsArray<gsGameObject, gkGameObject> m_objectCache;
	gsRay* m_pickRay;
#ifdef OGREKIT_USE_PROCESSMANAGER
	gkProcessManager* m_processManager;
#endif

public:
	gsScene();
	~gsScene();

	/**
		\LuaMethod{Scene,hasObject}

		Returns true if the scene contains the specified object.

		\code
		function Scene:hasObject(name)
		\endcode

		\param name Find by the object's identifier.
		\return bool
	*/
	bool hasObject(const gkString& name);
	/**
		\LuaMethod{Scene,getObject}

		Looks for an generic game object in the scene.

		\code
		function Scene:getObject(name)
		\endcode

		\param name Find by the object's identifier.
		\returns nil if not found or, GameObject, Entity, Camera, Light, or Skeleton depending on the GameObjectTypes. 
	*/
	gkGameObject* getObject(const gkString& name);
	/**
		\LuaMethod{Scene,createEmpty}

		Creates an empty node.

		\code
		function Scene:createEmpty(name)
		\endcode

		\param name Unique name of the empty node.
		\return \LuaClassRef{GameObject}
	*/
	gkGameObject* createEmpty(const gkString& name);
	/**

	*/
	gkGameObject* createEntity(const gkString& name);
	/**
		\LuaMethod{Scene,cloneObject}

		Clone a object.

		\code
		function Scene:cloneObject(object,lifeSpan,instantiate)
		\endcode

		\param object The template object to be cloned.
		\param lifeSpan Life span of a cloned object. (0: permanet)
		\param instantiate Instantiate a cloned object.
		\return \LuaClassRef{GameObject}
	*/
    gkGameObject* cloneObject(gsGameObject* obj, int lifeSpan, bool instantiate);
	/**
		\LuaMethod{Scene,getObjectList}

		Returns a list of all loaded objects in the scene.

		\code
		function Scene:getObjectList()
		\endcode

		\returns \LuaClassRef{ObjectList}
	*/
	gsArray<gsGameObject, gkGameObject> &getObjectList(void);
	/**

	*/
	gkDynamicsWorld* getDynamicsWorld(void);
	/**

	*/
	gkCamera* getMainCamera(void);
	/**

	*/
	gsRay* getPickRay(float x=-1,float y=-1);
	/**

	*/
	void setUpdateFlags(unsigned int flags);
	/**

	*/
	void setLayer(int layer);
	/**

	*/
	int getLayer(void);

#ifdef OGREKIT_USE_PROCESSMANAGER
	gkProcessManager* getProcessManager(void);
#endif

	gkLogicManager* getLogicBrickManager(void);

	gkScene* getOwner();
	// internal
	OGRE_KIT_WRAP_BASE_COPY_CTOR(gsScene, gkInstancedObject);
};

extern gkScene* getActiveScene(void);
extern gkScene* getScene(const gkString& sceneName);
extern gkScene* addScene(gsScene* scene,int zorder);
class gsGameObjectInstance
{
public:
	gsGameObjectInstance( gkGameObjectInstance* inst );
	virtual ~gsGameObjectInstance();
	/**

	*/
	int getElementCount();
	/**

	*/
	gkGameObject* getElementAt(int pos);
	/**

	*/
	gkGameObject* getElementByName(gkString name);
	/**

	*/
	gkGameObject* getRoot(void);
	/**

	*/
	void destroyInstance();
	/**

	*/
	void createInstance();
	/**

	*/
	void reinstance();
	/**

	*/
	gkString getName();
private :
	gkGameObjectInstance* m_gobj;
};

extern gkGameObjectInstance* createGroupInstance(gkString name,gsVector3 loc=gsVector3(0,0,0),gsVector3 orientation=gsVector3(0,0,0),gsVector3 scale=gsVector3(1,1,1));
extern gkGameObjectInstance* createGroupInstance(gsScene* scene,gkString name,gsVector3 loc=gsVector3(0,0,0),gsVector3 orientation=gsVector3(0,0,0),gsVector3 scale=gsVector3(1,1,1));
extern gkGameObjectInstance* createGroupInstance(gkScene* scene,gkString name,gsVector3 loc=gsVector3(0,0,0),gsVector3 orientation=gsVector3(0,0,0),gsVector3 scale=gsVector3(1,1,1));

class gsGameObject : public gsObject
{
public:
	gsGameObject();
	~gsGameObject() {}

	/**
		\LuaMethod{GameObject,getPosition}

		Returns the local position if this object is part of a parent / child hierarchy.
		otherwise returns the world position. 

		\code
		function GameObject:getPosition()
		\endcode

		\returns \LuaClassRef{Vector3}
	*/
	gsVector3 getPosition(void);
	/**
		\LuaMethod{GameObject,getRotation}

		Returns the local rotation if this object is part of a parent / child hierarchy.
		otherwise returns the world rotation. 

		\code
		function GameObject:getRotation()
		\endcode

		\returns \LuaClassRef{Vector3} as Euler, pitch, yaw, roll 
	*/
	gsVector3 getRotation(void);
	/**
		\LuaMethod{GameObject,getOrientation}

		Returns the local orientation if this object is part of a parent / child hierarchy.
		otherwise returns the world orientation. 

		\code
		function GameObject:getOrientation()
		\endcode

		\returns \LuaClassRef{Quaternion}
	*/
	gsQuaternion getOrientation(void);
	/**
		\LuaMethod{GameObject,getScale}

		Returns the objects scale.

		\code
		function GameObject:getScale()
		\endcode

		\returns \LuaClassRef{Vector3}
	*/
	gsVector3 getScale(void);
	/**
		\LuaMethod{GameObject,getWorldPosition}

		Returns the combined position of the object within the parent / child hierarchy.

		\code
		function GameObject:getWorldPosition()
		\endcode

		\returns \LuaClassRef{Vector3}
	*/
	gsVector3 getWorldPosition(void);
	/**
		\LuaMethod{GameObject,getWorldRotation}

		Returns the combined rotation of the object within the parent / child hierarchy.

		\code
		function GameObject:getWorldRotation()
		\endcode

		\returns \LuaClassRef{Vector3} as Euler, pitch, yaw, roll 
	*/
	gsVector3 getWorldRotation(void);
	/**
		\LuaMethod{GameObject,getWorldOrientation}

		Returns the combined orientation of the object within the parent / child hierarchy.

		\code
		function GameObject:getWorldOrientation()
		\endcode

		\returns \LuaClassRef{Quaternion}
	*/
	gsQuaternion getWorldOrientation(void);
	/**
		\LuaMethod{GameObject,getLinearVelocity}

		Returns the linear velocity of the game object.

		\code
		function GameObject:getLinearVelocity()
		\endcode

		\returns \LuaClassRef{Vector3}

		
	*/
	gsVector3 getLinearVelocity(void);
	/**
		\LuaMethod{GameObject,getAngularVelocity}

		Returns the angular velocity of the game object.

		\code
		function GameObject:getAngularVelocity()
		\endcode

		\returns \LuaClassRef{Vector3}		
	*/
	gsVector3 getAngularVelocity(void);
	/**
		\LuaMethod{GameObject,setLinearVelocity}

		Sets the linear velocity of the game object.

		\code
		function GameObject:setLinearVelocity(vec)
		\endcode

		\param vec \LuaClassRef{Vector3} as the new velocity.
	*/
	void setLinearVelocity(const gsVector3& v, gsTransformSpace ts=TS_PARENT);
	/**
		\sectionseperator{Overload:}

		\code
		function GameObject:setLinearVelocity(x, y, z)
		\endcode

		\param x x component.
		\param y y component.
		\param z z component.
	*/
	void setLinearVelocity(float x, float y, float z, gsTransformSpace ts=TS_PARENT);
	/**
		\LuaMethod{GameObject,setAngularVelocity}

		Sets the angular velocity of the game object.

		\code
		function GameObject:setAngularVelocity(vec)
		\endcode

		\param vec \LuaClassRef{Vector3} as the new velocity.
	*/
	void setAngularVelocity(const gsVector3& v, gsTransformSpace ts=TS_PARENT);
	/**
		\sectionseperator{Overload:}

		\code
		function GameObject:setAngularVelocity(x, y, z)
		\endcode

		\param x x component.
		\param y y component.
		\param z z component.
	*/
	void setAngularVelocity(float x, float y, float z, gsTransformSpace ts=TS_PARENT);
	/**
		\LuaMethod{GameObject,setPosition}

		Sets the position of the game object.


		\code
		function GameObject:setPosition(pos)
		\endcode

		\param pos \LuaClassRef{Vector3} as the new position.
	*/
	void setPosition(const gsVector3& v);
	/**
		\sectionseperator{Overload:}

		\code
		function GameObject:setPosition(x, y, z)
		\endcode

		\param x x component.
		\param y y component.
		\param z z component.
	*/
	void setPosition(float x, float y, float z);
	/**
		\LuaMethod{GameObject,setRotation}

		Sets the rotation of the game object.


		\code
		function GameObject:setRotation(eul)
		\endcode

		\param eul \LuaClassRef{Vector3} as the new rotation in degrees.
	*/
	void setRotation(const gsVector3& v);
	/**
		\sectionseperator{Overload:}

		\code
		function GameObject:setRotation(pitch, yaw, roll)
		\endcode

		\param pitch x component in degrees.
		\param yaw y component in degrees.
		\param roll z component in degrees.
	*/
	void setRotation(float pitch, float yaw, float roll);
	/**

	*/
	void lookAt(const gsVector3& v);
	/**

	*/
	void lookAt(gsGameObject* lookAtObj);
	/**
		\LuaMethod{GameObject,setOrientation}

		Sets the orientation of the game object.


		\code
		function GameObject:setOrientation(quat)
		\endcode

		\param quat \LuaClassRef{Quaternion} as the new rotation.
	*/
	void setOrientation(const gsQuaternion& quat);
	/**
		\sectionseperator{Overload:}

		\code
		function GameObject:setOrientation(w, x, y, z)
		\endcode

		\param w w component.
		\param x x component.
		\param y y component.
		\param z z component.
	*/
	void setOrientation(float w, float x, float y, float z);
	/**

	*/
	void setScale(const gsVector3& v);
	/**

	*/
	void setScale(float x,float y,float z);
	/**
		\LuaMethod{GameObject,getType}

		Returns the sub class type.

		\code
		function GameObject:getType()
		\endcode

		\returns  \LuaClassRef{GameObjectTypes}
	*/
	gsGameObjectTypes getType(void);
	/**

	*/
	void rotate(float dx, float dy, float dz);
	/**

	*/
	void rotate(const gsVector3& v);
	/**

	*/
	void rotate(const gsQuaternion& v);
	/**
		\LuaMethod{GameObject,rotate}

		Rotates the game object.

		\code
		function GameObject:rotate(pitch, yaw, roll, ts=OgreKit.TS_PARENT)
		\endcode

		\param pitch x rotation in degrees.
		\param yaw y rotation in degrees.
		\param roll z rotation in degrees.
		\param ts \LuaClassRef{TransformSpace}
	*/
	void rotate(float dx, float dy, float dz, gsTransformSpace ts);
	/**
		\sectionseperator{Overload 1:}

		\code
		function GameObject:rotate(eul, ts=OgreKit.TS_PARENT)
		\endcode
		\param eul \LuaClassRef{Vector3} as pitch, yaw and roll in degrees.
		\param ts \LuaClassRef{TransformSpace}
	*/
	void rotate(const gsVector3& v, gsTransformSpace ts);
	/**
		\sectionseperator{Overload 2:}

		\code
		function GameObject:rotate(quat, ts=OgreKit.TS_PARENT)
		\endcode
		\param quat \LuaClassRef{Quaternion} 
		\param ts \LuaClassRef{TransformSpace}
	*/
	void rotate(const gsQuaternion& v, gsTransformSpace ts);

	/**
		\LuaMethod{GameObject,translate}

		Translate the game object.

		\code
		function GameObject:translate(val, ts=OgreKit.TS_PARENT)
		\endcode

		\param val \LuaClassRef{Vector3} as the delta translation
		\param ts \LuaClassRef{TransformSpace}

	*/
	void translate(float x, float y, float z);
	/**

	*/
	void translate(const gsVector3& v);
	/**
		\sectionseperator{Overload:}

		\code
		function GameObject:translate(x, y, z, ts=OgreKit.TS_PARENT)
		\endcode
		\param x x-axis component.
		\param y y-axis component.
		\param z z-axis component.
		\param ts \LuaClassRef{TransformSpace}

	*/
	void translate(float x, float y, float z, gsTransformSpace ts);
	/**

	*/
	void translate(const gsVector3& v, gsTransformSpace ts);

	/**
		\LuaMethod{GameObject,scale}

		Scale the game object.

		\code
		function GameObject:scale(val)
		\endcode

		\param val \LuaClassRef{Vector3} as scale
	*/
	void scale(const gsVector3& v);
	/**
		\sectionseperator{Overload:}

		\code
		function GameObject:scale(x, y, z)
		\endcode

		\param x x-axis component.
		\param y y-axis component.
		\param z z-axis component.		
	*/
	void scale(float x, float y, float z);

	/**

	*/
	void yaw(float deg);
	/**
		\LuaMethod{GameObject,yaw}

		Rotate object around the y axis.

		\code
		function GameObject:yaw(val, ts=OgreKit.TS_PARENT)
		\endcode

		\param val Rotation in degrees.
		\param ts \LuaClassRef{TransformSpace}		
	*/
	void yaw(float deg, gsTransformSpace ts);

	/**

	*/
	void pitch(float deg);
	/**

	*/
	void pitch(float deg, gsTransformSpace ts);

	/**

	*/
	void roll(float deg);
	/**
		\LuaMethod{GameObject,roll}

		Rotate object around the z axis.

		\code
		function GameObject:roll(val, ts=OgreKit.TS_PARENT)
		\endcode

		\param val Rotation in degrees.
		\param ts \LuaClassRef{TransformSpace}		
	*/
	void roll(float deg, gsTransformSpace ts);
	/**
		\LuaMethod{GameObject,getState}

		Returns the logic brick state.

		\code
		function GameObject:getState()
		\endcode

		\returns number	
	*/
	int getState(void);
	/**
		\LuaMethod{GameObject,changeState}

		Change the logic brick state.

		\code
		function GameObject:changeState(state)
		\endcode

		\param state as the new state.
	*/
	void changeState(int v);
	/**
		\LuaMethod{GameObject,hasParent}

		Returns true if the object is part of a parent/child hierarchy. 

		\code
		function GameObject:hasParent()
		\endcode

		\returns bool
	*/
	bool hasParent();
	/**
		\LuaMethod{GameObject,setParent}

		Sets the parent game object.

		\code
		function GameObject:setParent(object)
		\endcode

		\param object \LuaClassRef{GameObject} as the new parent.
	*/
	void setParent(gsGameObject* par);
	/**
		\LuaMethod{GameObject,setParentInPlace}

		Sets the parent game object, maintaining the object at the same global world transform.

		\code
		function GameObject:setParentInPlace(object)
		\endcode

		\param object \LuaClassRef{GameObject} as the new parent.
	*/
	void setParentInPlace(gsGameObject* par);
	/**
		\LuaMethod{GameObject,clearParent}

		Clear the parent game object.

		\code
		function GameObject:clearParent()
		\endcode
	*/
	void clearParent(void);
	/**
		\LuaMethod{GameObject,clearParentInPlace}

		Clear the parent game object, maintaining the object at the same global world transform.

		\code
		function GameObject:clearParentInPlae()
		\endcode
	*/
	void clearParentInPlace(void);
	/**

	*/
	void addChild(gsGameObject* chi);
	/**

	*/
	void removeChild(gsGameObject* chi);
	/**

	*/
	gkGameObject* getChildAt(int pos);
	/**

	*/
	int getChildCount(void);
	/**

	*/
	gkGameObject* getChildByName(const gkString& name);

	/**
		\LuaMethod{GameObject,getParent}

		Returns the parent game object.

		\code
		function GameObject:getParent()
		\endcode

		\returns nil if not found or, GameObject, Entity, Camera, Light, or Skeleton depending on the GameObjectTypes. 
	*/
	gkGameObject* getParent(void);
	/**

	*/
	void enableContacts(bool v);
	/**

	*/
	bool hasContacts();
	/**

	*/
	bool hasContact(const gkString& object);
	/**
		\LuaMethod{GameObject,getScene}

		Returns the owning scene

		\code
		function GameObject:getScene()
		\endcode

		\returns \LuaClassRef{Scene}	
	*/
	gkScene* getScene(void);
	/**
		\LuaMethod{GameObject,playAnimation}

		Plays an animation. 

		\code
		function GameObject:playAnimation(name, blend)
		\endcode

		\param name Identifier of the animation.
		\param blend The number of blend-in frames, if a previous action is playing.
	*/
	void playAnimation(const gkString& name, float blend, bool restart=false);

	// actually just calling not lua-enabled method!
	/**

	*/
	gsProperty	getProperty(const gkString& name)				{return __getitem__(name);}
	/**

	*/
	void        setProperty(const gkString& prop, bool  v)		{__setitem__(prop,v);}
	/**

	*/
	void		setProperty(const gkString& prop, float v)		{__setitem__(prop,v);}
	/**

	*/
	void        setProperty(const gkString& prop, gkString v)	{__setitem__(prop,v.c_str());}

	gsProperty  __getitem__(const gkString& prop);
	void        __setitem__(const gkString& prop, bool  v);
	void        __setitem__(const gkString& prop, float v);
	void        __setitem__(const gkString& prop, const char* v);


	/**

	*/
	gkGameObjectInstance* getGroupInstance();
	/**

	*/
	bool             isGroupInstance();

	// internal
	OGRE_KIT_WRAP_BASE_COPY_CTOR(gsGameObject, gkInstancedObject);
	OGRE_KIT_TEMPLATE_CAST(gkGameObject, m_object);
//	OGRE_KIT_TEMPLATE_NEW(gsGameObject, gkGameObject);
};

class gsCurve : public gsGameObject
{
public:
	gsCurve();
	~gsCurve() {}
	OGRE_KIT_WRAP_BASE_COPY_CTOR(gsCurve, gkInstancedObject);
	
	/**

	*/
	gsVector3 getPoint(int nr);
	/**

	*/
	int getPointCount();
	/**

	*/
	bool isCyclic();
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

	/**
		\LuaMethod{Camera,setClipping}

		Sets the cameras near and far clip distances.

		\code
		function Camera:setClipping(start, end)
		\endcode

		\param start Near clip distance.
		\param end Far clip distance.
	*/
	void setClipping(float start, float end);
	/**
		\LuaMethod{Camera,getClipStart}

		Gets the cameras near distance.

		\code
		function Camera:getClipStart()
		\endcode

		\returns number	
	*/
	float getClipStart();
	/**
		\LuaMethod{Camera,getClipEnd}

		Gets the cameras far distance.

		\code
		function Camera:getClipEnd()
		\endcode

		\returns number
	*/

	float getClipEnd();

	/**
		\LuaMethod{Camera,setFov}

		Sets the field of view.

		\code
		function Camera:setFov(fov)
		\endcode

		\param fov number value expressed in degrees.
	*/
	void setFov(float fov);
	/**
		\LuaMethod{Camera,getFov}

		Gets the field of view.

		\code
		function Camera:getFov()
		\endcode

		\returns number	
	*/
	float getFov();
	/**
		\LuaMethod{Camera,makeCurrent}

		Sets this camera as the viewing camera for the scene. 

		\code
		function Camera:makeCurrent()
		\endcode
	*/
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

	/**
		\LuaMethod{Entity,hasCharacter}

		Test the attached character is exists. 

		\code
		function Entity:hasCharacter()
		\endcode

		\return bool True if the character is attached to the entity.
	*/
	bool hasCharacter(void);
	/**
		\LuaMethod{Entity,getCharacter}

		Returns the attached character(or nil) of the entity. 

		\code
		function Entity:getCharacter()
		\endcode

		\return \LuaClassRef{Character}
	*/
	gkCharacter* getCharacter(void);
	/**

	*/
	void setMaterialName(const gkString& materialName);
	/**

	*/
	gkMesh* getMesh();

	// internal
	OGRE_KIT_WRAP_BASE_COPY_CTOR(gsEntity, gkInstancedObject);
};



class gsSkeleton : public gsGameObject
{
public:
	gsSkeleton();
	~gsSkeleton() {}

	/**

	*/
	void attachObjectToBone(const gkString& bone, gsGameObject* gsobj,gsVector3 loc=gsVector3(0,0,0),gsVector3 orientation=gsVector3(0,0,0),gsVector3 scale=gsVector3(1,1,1));
	/**

	*/
	void attachObjectToBoneInPlace(const gkString& bone, gsGameObject* gsobj);
	/**

	*/
	void setBoneManual(const gkString& boneName,bool manual);
	/**

	*/
	bool isBoneManual(const gkString& boneName);
	/**

	*/
	void applyBoneChannelTransform(const gkString& boneName, gsVector3 loc=gsVector3(0,0,0), gsVector3 orientation=gsVector3(0,0,0),gsVector3 scale=gsVector3(1,1,1), gkScalar pWeight=1.0F);

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

	/**
		\LuaMethod{Debugger,constructor}

		Creates a debugger for the active scene.

		\code
		function Debugger:constructor(scene)
		\endcode

		\param scene The active \LuaClassRef{Scene} 
	*/
	gsDebugger(gsScene* sc);
	~gsDebugger() {}

	/**
		\LuaMethod{Debugger,drawLine}

		Draws a line in the scene.

		\code
		function Debugger:drawLine(from, to, color)
		\endcode

		\param from \LuaClassRef{Vector3} as the starting position.
		\param to \LuaClassRef{Vector3} as the ending position.
		\param color \LuaClassRef{Vector3} as a color value ranging from [0..1]
	*/
	void drawLine(const gsVector3& from, const gsVector3& to, const gsVector3& color=gsVector3(1,1,1));
	/**
		\LuaMethod{Debugger,drawObjectAxis}

		Draws the local axis of the specified game object.

		\code
		function Debugger:drawLine(object, size)
		\endcode

		\param from \LuaClassRef{GameObject} as the object in question.
		\param size number representing the length of each axis.
	*/
	void drawObjectAxis(gsGameObject* ptr, float size);
	/**

	*/
	void drawCurve(gsCurve* path, const gsVector3& color=gsVector3(1,1,1));
	/**
		\LuaMethod{Debugger,clear}

		Removes all lines from the scene.

		\code
		function Debugger:clear()
		\endcode
	*/
	void clear(void);
};

class gsSubMesh {
	friend class gsMesh;
public:
	gsSubMesh();
	/**

	*/
	gsSubMesh(gkSubMesh* submesh);
	~gsSubMesh();

	/**

	*/
	gkVertex* getVertex(int nr){ return &(m_submesh->getVertexBuffer().at(nr));};
	/**

	*/
	int getVertexAmount(){ return m_submesh->getVertexBuffer().size();};
	/**

	*/
	const gkTriangle addTriangle(const gkVertex& v0,
            unsigned int i0,
            const gkVertex& v1,
            unsigned int i1,
            const gkVertex& v2,
            unsigned int i2);

	/**

	*/
	gkMaterialProperties& getMaterial(void)                 {return m_submesh->getMaterial();}
	/**

	*/
	void setUVCount(int count) { m_submesh->setTotalLayers(1);}
	/**

	*/
	int getUVCount() { return m_submesh->getUvLayerCount();}
	/**

	*/
	void setMaterialName(const gkString& materialName);

private:
	gkSubMesh* m_submesh;
	bool m_isMeshCreator;
};

class gsMesh
{
public:
	/**

	*/
	gsMesh(gkMesh* mesh);
	~gsMesh() {}
	/**

	*/
	int getSubMeshAmount() { return m_mesh->m_submeshes.size();}
	/**

	*/
	gkSubMesh* getSubMesh(int nr) { return m_mesh->m_submeshes.at(nr); }
	/**

	*/
	void addSubMesh(gsSubMesh* submesh);
	/**

	*/
	void reload() {m_mesh->reload();}
private:
	gkMesh* m_mesh;
};




/**

*/
extern void sendMessage(const char* from,const char* to,const char* subject,const char* body);

/**
	\LuaClass{DebugPrint}

	Prints message to the render window.

	\code
	function OgreKit.DebugPrint(string)
	\endcode

	\param string Debug message.
*/
extern void gsDebugPrint(const char* str);
/**
	\LuaClass{SetCompoisitorChain}

	Set compositor chain operation to the viewport. (build option: OGREKIT_USE_COMPOSITOR)

	\code
	function OgreKit.SetCompoisitorChain(op, name)
	\endcode


	\param op \LuaClassRef{CompositorOp}.
	\param name compositor name as string. 
	\remark Built-in compositor name is 'Bloom', 'B&W', 'Embossed', 'Sharpen Edges', 'Invert', 'Posterize', 'Laplace', 'Tiling', 'HDR', 'Gaussian Blur', 'Radial Blur', 'Halftone', 'Dither'.
	\remark Extra compositor is required compositor textures. Name is 'Glass', 'Old TV', 'Old Movie', 'ASCII', 'Night Vision', 'Motion Blur', 'Heat Vision'. (build option: OGREKIT_USE_COMPOSITOR_TEX)
*/
extern bool gsSetCompositorChain(gsCompositorOp op, const gkString& compositorName);
/**

*/
extern void import(const gkString& scriptName);
/**

*/
extern gkString getPlatform();
/**

*/
extern bool isSoundAvailable();
/**

*/
extern bool isMaterialInitialized(const gkString& matName);
/**

*/
extern void initMaterial(const gkString& matName);
/**

*/
extern void setMaterialParam(const gkString& matName, int shaderType,const gkString& paramName, float value);
/**

*/
extern void setMaterialParam(const gkString& matName, int shaderType,const gkString& paramName, gsVector3* vec3);
/**

*/
extern void setMaterialParam(const gkString& matName, int shaderType,const gkString& paramName, gsVector4* vec4);

/** @} */

#endif//_OgreKitApi_h_
