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
#ifndef _gsBricks_h_
#define _gsBricks_h_


#include "gsCommon.h"
#include "gsMath.h"
#include "gsUtils.h"
#include "Script/Lua/gkLuaUtils.h"

/** \addtogroup Bricks
	@{
*/

#ifndef SWIG
#define BRICK_GET(METH, DEF) return (m_brick ? get()->METH : DEF);
#define BRICK_SET(METH) if (m_brick) get()->METH;
#endif


/**
	\LuaClass{BrickMode}
 
	The logic brick toggle state.

	\code
	OgreKit.BM_NONE,      No state response.
	OgreKit.BM_ON,        Brick is switched on
	OgreKit.BM_OFF,       Brick is switched off
	\endcode
*/
enum gsBrickMode
{
	GS_BM_NONE = BM_IDLE,
	GS_BM_OFF  = BM_OFF,
	GS_BM_ON   = BM_ON,
};


/**
	\LuaClass{PulseMode}

	The sensor pulse state.

	\code
	OgreKit.PM_NONE,      No state response.
	OgreKit.PM_ON,        True activation.
	OgreKit.PM_OFF,       False activation.
	\endcode
*/
enum gsPulseMode
{
	PM_NONE = gkLogicSensor::PM_IDLE,
	PM_OFF  = gkLogicSensor::PM_FALSE,
	PM_ON   = gkLogicSensor::PM_TRUE,
};


/**
	\LuaClass{MouseSensorType}

	Event type for mouse sensors.

	\code
	OgreKit.MST_LEFT
	OgreKit.MST_RIGHT
	OgreKit.MST_MIDDLE
	OgreKit.MST_MOTION
	OgreKit.MST_WHEEL_UP
	OgreKit.MST_WHEEL_DOWN
	OgreKit.MST_OVER
	OgreKit.MST_OVER_ANY
	\endcode
*/
enum gsMouseSensorType
{
	MST_LEFT        = gkMouseSensor::MOUSE_LEFT,
	MST_RIGHT       = gkMouseSensor::MOUSE_RIGHT,
	MST_MIDDLE      = gkMouseSensor::MOUSE_MIDDLE,
	MST_MOTION      = gkMouseSensor::MOUSE_MOTION,
	MST_WHEEL_UP    = gkMouseSensor::MOUSE_WHEEL_UP,
	MST_WHEEL_DOWN  = gkMouseSensor::MOUSE_WHEEL_DOWN,
	MST_OVER        = gkMouseSensor::MOUSE_MOUSE_OVER,
	MST_OVER_ANY    = gkMouseSensor::MOUSE_MOUSE_OVER_ANY,
};

/**
	\LuaClass{ListenerMode}

	Callback mode for controlling how the custom event is handled.

	\code
	OgreKit.LM_EVT_OVERIDE,   Skip builtin query, you must provide your own.
	OgreKit.LM_EVT_AND,       Peforms an AND operation, (yourCallback) AND (builtin test).
	\endcode
*/
enum gsListenerMode
{
	LM_EVT_OVERIDE,
	LM_EVT_AND,
};


/**
	\LuaClass{PropertySensorType}

	Event type for property sensors.

	\code
	OgreKit.PST_EQUAL
	OgreKit.PST_NEQUAL
	OgreKit.PST_INTERVAL
	OgreKit.PST_CHANGED
	\endcode
*/
enum gsPropertySensorType
{
	PST_EQUAL    = gkPropertySensor::PS_EQUAL,
	PST_NEQUAL   = gkPropertySensor::PS_NEQUAL,
	PST_INTERVAL = gkPropertySensor::PS_INTERVAL,
	PST_CHANGED  = gkPropertySensor::PS_CHANGED,
};


/**
	\LuaClass{RaySensorAxis}

	Axis for Ray and Radar sensor.

	\code
	OgreKit.RSA_XPOS
	OgreKit.RSA_YPOS
	OgreKit.RSA_ZPOS
	OgreKit.RSA_XNEG
	OgreKit.RSA_YNEG
	OgreKit.RSA_ZNEG
	\endcode
*/
enum gsRaySensorAxis
{
	RSA_XPOS = gkRaySensor::RA_XPOS,
	RSA_YPOS = gkRaySensor::RA_YPOS,
	RSA_ZPOS = gkRaySensor::RA_ZPOS,
	RSA_XNEG = gkRaySensor::RA_XNEG,
	RSA_YNEG = gkRaySensor::RA_YNEG,
	RSA_ZNEG = gkRaySensor::RA_ZNEG,
};


/**
	\LuaClass{LogicOp}

	Logical operation 

	\code
	OgreKit.LOP_AND
	OgreKit.LOP_OR
	OgreKit.LOP_XOR
	OgreKit.LOP_NAND
	OgreKit.LOP_NOR
	OgreKit.LOP_XNOR
	\endcode
*/
enum gsLogicOp
{
	LOP_NONE = gkLogicOpController::OP_NILL,
	LOP_AND  = gkLogicOpController::OP_AND,
	LOP_OR   = gkLogicOpController::OP_OR,
	LOP_XOR  = gkLogicOpController::OP_XOR,
	LOP_NAND = gkLogicOpController::OP_NAND,
	LOP_NOR  = gkLogicOpController::OP_NOR,
	LOP_XNOR = gkLogicOpController::OP_XNOR,
};




enum gsActionActuatorType
{
	AAT_PLAY       = gkActionActuator::AA_PLAY,
	AAT_PONG       = gkActionActuator::AA_PONG,
	AAT_FLIPPER    = gkActionActuator::AA_FLIPPER,
	AAT_LOOP_STOP  = gkActionActuator::AA_LOOP_STOP,
	AAT_LOOP_END   = gkActionActuator::AA_LOOP_END,
	AAT_PROPERTY   = gkActionActuator::AA_PROPERTY,
};



enum gsGameActuatorType
{
	GAT_QUIT        = gkGameActuator::GM_QUIT,
	GAT_RESTART     = gkGameActuator::GM_RESTART,
	GAT_START_NEW   = gkGameActuator::GM_START_NEW,
	GAT_SAVE        = gkGameActuator::GM_SAVE,
	GAT_LOAD        = gkGameActuator::GM_LOAD,
};



enum gsEditObjectActuatorType
{
	EOT_ADDOBJ        = gkEditObjectActuator::EO_ADDOBJ,
	EOT_ENDOBJ        = gkEditObjectActuator::EO_ENDOBJ,
	EOT_REPLACEMESH   = gkEditObjectActuator::EO_REPLACEMESH,
	EOT_TRACKTO_2D    = gkEditObjectActuator::EO_TRACKTO_2D,
	EOT_TRACKTO_3D    = gkEditObjectActuator::EO_TRACKTO_3D,
	EOT_DYNAMICS      = gkEditObjectActuator::EO_DYNAMICS,
};



enum gsEditObjectActuatorDynamics
{
	EOD_RESTORE        = gkEditObjectActuator::EOD_RESTORE,
	EOD_SUSPEND        = gkEditObjectActuator::EOD_SUSPEND,
	EOD_ENABLE_BODY    = gkEditObjectActuator::EOD_ENABLE_BODY,
	EOD_DISABLE_BODY   = gkEditObjectActuator::EOD_DISABLE_BODY,
	EOD_SETMASS        = gkEditObjectActuator::EOD_SETMASS,
};



enum gsMessageActuatorBodyType
{
	MBT_TEXT  = gkMessageActuator::BT_TEXT,
	MBT_PROP  = gkMessageActuator::BT_PROP,
};



enum gsParentActuatorMode
{
	PA_SET   = gkParentActuator::PA_SET,
	PA_CLEAR = gkParentActuator::PA_CLEAR,
};



enum gsPropertyActuatorType
{
	PA_ASSIGN = gkPropertyActuator::PA_ASSIGN,
	PA_ADD    = gkPropertyActuator::PA_ADD,
	PA_COPY   = gkPropertyActuator::PA_COPY,
	PA_TOGGLE = gkPropertyActuator::PA_TOGGLE,
};



enum gsRandomActuatorDistribution
{
	RA_BOOL_CONSTANT   = gkRandomActuator::RA_BOOL_CONSTANT,
	RA_BOOL_UNIFORM    = gkRandomActuator::RA_BOOL_UNIFORM,
	RA_BOOL_BERNOUILLI = gkRandomActuator::RA_BOOL_BERNOUILLI,
	RA_INT_CONSTANT    = gkRandomActuator::RA_INT_CONSTANT,
	RA_INT_UNIFORM     = gkRandomActuator::RA_INT_UNIFORM,
	RA_INT_POISSON     = gkRandomActuator::RA_INT_POISSON,
	RA_FLOAT_CONSTANT  = gkRandomActuator::RA_FLOAT_CONSTANT,
	RA_FLOAT_UNIFORM   = gkRandomActuator::RA_FLOAT_UNIFORM,
	RA_FLOAT_NORMAL    = gkRandomActuator::RA_FLOAT_NORMAL,
	RA_FLOAT_NEGEXP    = gkRandomActuator::RA_FLOAT_NEGEXP,
};



enum gsSceneActuatorMode
{
	SC_RESTART    = gkSceneActuator::SC_RESTART,
	SC_SET_SCENE  = gkSceneActuator::SC_SET_SCENE,
	SC_SET_CAMERA = gkSceneActuator::SC_SET_CAMERA,
	SC_ADD_BACK   = gkSceneActuator::SC_ADD_BACK,
	SC_ADD_FRONT  = gkSceneActuator::SC_ADD_FRONT,
	SC_REMOVE     = gkSceneActuator::SC_REMOVE,
	SC_SUSPEND    = gkSceneActuator::SC_SUSPEND,
	SC_RESUME     = gkSceneActuator::SC_RESUME,
};



enum gsSoundActuatorType
{
	SA_PLAY_STOP   = gkSoundActuator::SA_PLAY_STOP,
	SA_PLAY_END    = gkSoundActuator::SA_PLAY_END,
	SA_LOOP_STOP   = gkSoundActuator::SA_LOOP_STOP,
	SA_LOOP_END    = gkSoundActuator::SA_LOOP_END,
};


enum gsStateActuatorType
{
	SOP_ADD = gkStateActuator::OP_ADD,
	SOP_SUB = gkStateActuator::OP_SUB,
	SOP_CPY = gkStateActuator::OP_CPY,
	SOP_INV = gkStateActuator::OP_INV,
};



enum gsVisibilityActuatorType
{
	VA_INVIS_FLAG  = gkVisibilityActuator::VA_INVIS_FLAG,
	VA_OCCLUDER    = gkVisibilityActuator::VA_OCCLUDER,
	VA_CHILDREN    = gkVisibilityActuator::VA_CHILDREN,
};





class gsLogicManager
{
public:
	// default to active-scene
	gsLogicManager(gsScene* scene=0);
	~gsLogicManager();


	gkLogicLink* newObject(gsGameObject* obj);

	/**
		\LuaMethod{LogicManager,getObject}

		Looks for a logic object by name.

		\code
		function LogicManager:getObject(name)
		\endcode

		\param name Find by the object's identifier. It's the same name as game object it's attached to.
		\returns \LuaClassRef{LogicObject} or nil if the object is not found.
	*/
	gkLogicLink* getObject(const gkString& name);

	/**
		\LuaMethod{LogicManager,getObjectList}

		Returns a list of all logic objects.

		\code
		function LogicManager:getObjectList()
		\endcode

		\returns \LuaClassRef{LogicList}
	*/
	gsArray<gsLogicObject, gkLogicLink> getObjectList();

private:
	gkScene* m_scene;
};



class gsLogicObject
{
private:

	int m_incr;

public:
	gsLogicObject();
	~gsLogicObject();

	/**
		\LuaMethod{LogicObject,getName}

		Returns the string identifier of this object.

		\code
		function LogicObject:getName()
		\endcode

		\returns string
	*/
	gkString             getName(void);
	void                 setDebug(bool v);
	bool                 isDebug(void);

	/**
		\LuaMethod{LogicObject,getSensor}

		Finds a sensor attached to this object.

		\code
		function LogicObject:getSensor(name, crossSearch=false)
		\endcode

		\param name The name given to the sensor in the blender UI.
		\param crossSearch Includes cross linked sensors while trying to match name.

		\returns \LuaClassRef{Sensor}		
	*/
	gkLogicSensor*      getSensor(const gkString& name);

	/**
		\LuaMethod{LogicObject,getController}

		Finds a controller attached to this object.

		\code
		function LogicObject:getController(name, crossSearch=false)
		\endcode

		\param name The name given to the controller in the blender UI.
		\param crossSearch Includes cross linked controllers while trying to match name.

		\returns \LuaClassRef{Controller}
	*/
	gkLogicController*  getController(const gkString& name);

	
	/**
		\LuaMethod{LogicObject,getActuator}

		Finds an actuator attached to this object.

		\code
		function LogicObject:getActuator(name, crossSearch=false)
		\endcode

		\param name The name given to the actuator in the blender UI.
		\param crossSearch Includes cross linked actuator while trying to match name.

		\returns \LuaClassRef{Actuator}
	*/
	gkLogicActuator*    getActuator(const gkString& name);

	/**
		\LuaMethod{LogicObject,getSensors}

		Return a list of sensors attached to this object.

		\code
		function LogicObject:getSensors(includeCross=false)
		\endcode

		\param includeCross Includes cross linked sensors.

		\returns \LuaClassRef{Sensors}
	*/
	gsArray<gsSensor, gkLogicSensor>         getSensors();

	/**
		\LuaMethod{LogicObject,getControllers}

		Return a list of controllers attached to this object.

		\code
		function LogicObject:getControllers()
		\endcode

		\returns \LuaClassRef{Controllers}
	*/
	gsArray<gsController, gkLogicController> getControllers();


	/**
		\LuaMethod{LogicObject,getActuators}

		Return a list of actuators attached to this object.

		\code
		function LogicObject:getActuators()
		\endcode

		\returns \LuaClassRef{Actuators}
	*/
	gsArray<gsActuator, gkLogicActuator>     getActuators();


	OGRE_KIT_WRAP_CLASS_COPY_CTOR(gsLogicObject, gkLogicLink, m_link);
	OGRE_KIT_TEMPLATE_CAST(gkLogicLink, m_link);
	OGRE_KIT_TEMPLATE_NEW_INLINE(gsLogicObject, gkLogicLink);


#ifndef SWIG
	bool hasBrick(gkLogicLink* link, const gkString& name);
	gkString getUniqueName(void);
	void makeOwner(bool v) {m_link->setExternalOwner(v);}
#endif


};



class gsBrick
{
protected:
	bool m_isLocal;


	template<typename T>
	T* create(gsLogicObject* parent, const gkString& name)
	{
		T* ret = 0;
		if (parent)
		{
			gkString cpy = name;
			if (cpy.empty())
				cpy = parent->getUniqueName();

			gkLogicLink* lnk = parent->get();
			if (lnk && !parent->hasBrick(lnk, cpy))
			{
				gkGameObject* ob = lnk->getObject();
				ret = new T(ob, lnk, cpy);
				ret->setMask(1);
				ret->setDebugMask(lnk->getDebug());
				lnk->push(ret, this);
			}
		}
		return ret;
	}

	class EventListener : public gkLogicBrick::Listener
	{
	private:
		gkLuaEvent* m_evt;
		gkLogicBrick* m_parent;

	public:
		EventListener(gsListenerMode mode, gkLogicBrick* par, gsSelf self, gsFunction func);
		virtual ~EventListener();

		virtual bool executeEvent(gkLogicBrick* brick);
	};

	EventListener* m_listener;


public:
	gsBrick();
	~gsBrick();

	/**
		\LuaMethod{Brick,getName}

		Returns the identifier for this brick.

		\code
		function Brick:getName()
		\endcode

		\returns string
	*/
	gkString     getName(void);


	/**
		\LuaMethod{Brick,isOn}

		Returns true if this brick has an \b ON pulse.

		\code
		function Brick:isOn()
		\endcode

		\returns \LuaClassRef{BrickMode}
	*/
	bool isOn(void)      {BRICK_GET(isPulseOn(), false); }
	
	/**
		\LuaMethod{Brick,isOff}

		Returns true if this brick has an \b OFF pulse.

		\code
		function Brick:isOff()
		\endcode

		\returns \LuaClassRef{BrickMode}
	*/
	bool isOff(void)     {BRICK_GET(isPulseOff(), false);}

	/**
		\LuaMethod{Brick,isActive}

		Returns true if this brick is activly running in the logic manager.

		\code
		function Brick:isActive()
		\endcode

		\returns bool
	*/
	bool isActive(void)  {BRICK_GET(isActive(), false);}
	gkGameObject* getOwner(void);
	gkLogicLink* getLogicObject(void){ return get()->getLink();}

	/**
		\LuaMethod{Brick,connect}

		Connects an event listener to this logic brick for custom callbacks.

		\code
		function Brick:connect(execMode, table, function)
		\endcode

		\param execMode \LuaClassRef{ListenerMode} The mode in which this will execute.
		\param table    Pointer to a Lua table object, ie; self
		\param function Pointer to a Lua function, method of self.

		\LuaGlobalRef{CustomLogicBricks}
	*/
	void connect(gsListenerMode mode, gsSelf self, gsFunction func)
	{
		if (!m_listener && get())
		{
			m_listener = new EventListener(mode, get(), self, func);
			get()->setListener(m_listener);
		}
	}

	OGRE_KIT_WRAP_CLASS_COPY_CTOR(gsBrick, gkLogicBrick, m_brick);
	OGRE_KIT_TEMPLATE_CAST(gkLogicBrick, m_brick);
};



class gsSensor : public gsBrick
{

public:
	gsSensor();
	~gsSensor();

	/**
		\LuaMethod{Sensor,setPulse}

		Sets the pulse mode for this sensor.

		\code
		function Sensor:setPulse(pulse)
		\endcode

		\param pulse \LuaClassRef{PulseMode}
	*/
	void            setPulse(int mode)            { BRICK_SET(setMode(mode));  }

	/**
		\LuaMethod{Sensor,getPulse}

		Returns the pulse mode.

		\code
		function Sensor:getPulse()
		\endcode

		\returns \LuaClassRef{PulseMode}	
	*/
	int             getPulse(void)                { BRICK_GET(getMode(), PM_NONE); }
	/**
		\LuaMethod{Sensor,setFrequency}

		Sets the update frequency in game ticks (1 / 60).

		\code
		function Sensor:setFrequency(number)
		\endcode

		\param number The update rate in game ticks.
	*/
	void            setFrequency(float f)         { BRICK_SET(setFrequency(f)); }

	/**
		\LuaMethod{Sensor,getFrequency}

		Returns the frequency.

		\code
		function Sensor:getFrequency()
		\endcode

		\returns number
	*/
	float           getFrequency(void)            { BRICK_GET(getFrequency(), 0.f); }
	void            setLevel(bool v)              { BRICK_SET(setDetector(v)); }
	/**
		\LuaMethod{Sensor,isLevel}

		Returns true if this sensor is a state transition detector.

		\code
		function Sensor:isLevel()
		\endcode

		\returns bool
	*/
	bool            isLevel(void)                 { BRICK_GET(isDetector(), false);}
	/**
		\LuaMethod{Sensor,setTap}

		Enable tap / toggle mode.

		\code
		function Sensor:setTap(val)
		\endcode

		\param val Boolean value.
	*/
	void            setTap(bool v)                { BRICK_SET(setTap(v)); }
	/**
		\LuaMethod{Sensor,isTap}
		Returns true if this sensor is in tap / toggle mode.

		\code
		function Sensor:isTap()
		\endcode

		\returns bool
	*/
	bool            isTap(void)                   { BRICK_GET(isTap(), false); }
	/**
		\LuaMethod{Sensor,setInvert}

		Makes this sensor invert event queries.

		\code
		function Sensor:setInvert(val)
		\endcode

		\param val bool value.
	*/
	void            setInvert(bool v)             { BRICK_SET(invert(v)); }
	/**
		\LuaMethod{Sensor,isInverse}

		Returns true if this sensor is inverting events.

		\code
		function Sensor:isInverse()
		\endcode

		\returns bool
	*/
	bool            isInverse(void)               { BRICK_GET(isInverse(), false); }

	bool			isPositive(void) 			  { BRICK_GET(isPositive(),false); }

	OGRE_KIT_LOGIC_BRICK_BASE(Sensor);
//	OGRE_KIT_TEMPLATE_NEW(gsSensor, gkLogicSensor);
};




class gsActuatorSensor : public gsSensor
{
public:
	/**
		\LuaMethod{ActuatorSensor,constructor}

		Default constructor does nothing.

		\code
		function ActuatorSensor:constructor()
		\endcode
	*/
	gsActuatorSensor();
	/**
		\sectionseperator{Overload:}

		Creates a new sensor.

		\code
		function ActuatorSensor:constructor(object, name="")
		\endcode

		\param object The parent \LuaClassRef{LogicObject}
		\param name   Unique identifier for this sensor. If this is empty one will be generated automatically

		\returns \LuaClassRef{ActuatorSensor}		
	*/
	gsActuatorSensor(gsLogicObject* parent, const gkString& name = "");
	~gsActuatorSensor();

	/**
		\LuaMethod{ActuatorSensor,setActuatorName}

		Sets the name of the actuator this sensor responds to.

		\code
		function ActuatorSensor:setActuatorName(name)
		\endcode

		\param name string	
	*/
	void      setActuatorName(const gkString& str)  { BRICK_SET(setActuatorName(str));  }

	/**
		\LuaMethod{ActuatorSensor,getActuatorName}

		Gets the name of the actuator this sensor responds to.

		\code
		function ActuatorSensor:getActuatorName()
		\endcode

		\returns string	
	*/
	gkString  getActuatorName(void)                 { BRICK_GET(getActuatorName(), ""); }

	OGRE_KIT_LOGIC_BRICK(ActuatorSensor);
};



class gsAlwaysSensor : public gsSensor
{
public:
	/**
		\LuaMethod{AlwaysSensor,constructor}

		Default constructor does nothing.

		\code
		function AlwaysSensor:constructor()
		\endcode
	*/
	gsAlwaysSensor();
	/**
		\sectionseperator{Overload:}

		Creates a new sensor.

		\code
		function AlwaysSensor:constructor(object, name="")
		\endcode

		\param object The parent \LuaClassRef{LogicObject}
		\param name   Unique identifier for this sensor. If this is empty one will be generated automatically

		\returns \LuaClassRef{AlwaysSensor}
	*/
	gsAlwaysSensor(gsLogicObject* parent, const gkString& name = "");
	~gsAlwaysSensor();


	OGRE_KIT_LOGIC_BRICK(AlwaysSensor);
};



class gsCollisionSensor : public gsSensor
{
public:
	/**
		\LuaMethod{CollisionSensor,constructor}

		Default constructor does nothing.

		\code
		function CollisionSensor:constructor()
		\endcode
	*/
	gsCollisionSensor();
	/**
		\sectionseperator{Overload:}

		Creates a new sensor.

		\code
		function CollisionSensor:constructor(object, name="")
		\endcode

		\param object The parent \LuaClassRef{LogicObject}
		\param name   Unique identifier for this sensor. If this is empty one will be generated automatically

		\returns \LuaClassRef{CollisionSensor}	
	*/
	gsCollisionSensor(gsLogicObject* parent, const gkString& name = "");
	~gsCollisionSensor();

	/**
		\LuaMethod{CollisionSensor,setMaterialName}

		Sets the name of the material this sensor responds to.

		\code
		function CollisionSensor:setMaterialName(name)
		\endcode

		\param name string
	*/
	void      setMaterialName(const gkString& str)  { BRICK_SET(setMaterial(str));  }
	/**
		\LuaMethod{CollisionSensor,getMaterialName}

		Gets the name of the material this sensor responds to.

		\code
		function CollisionSensor:getMaterialName()
		\endcode

		\returns string
	*/
	gkString  getMaterialName(void)                 { BRICK_GET(getMaterial(), ""); }
	/**
		\LuaMethod{CollisionSensor,setPropertyName}

		Sets the name of the property this sensor responds to.

		\code
		function CollisionSensor:setPropertyName(name)
		\endcode

		\param name string
	*/
	void      setPropertyName(const gkString& str)  { BRICK_SET(setProperty(str));  }
	/**
		\LuaMethod{CollisionSensor,getPropertyName}

		Gets the name of the property this sensor responds to.

		\code
		function CollisionSensor:getPropertyName()
		\endcode

		\returns string
	*/
	gkString  getPropertyName(void)                 { BRICK_GET(getProperty(), ""); }
	/**
	*/
	int           getHitObjectsCount()              { return get()->getHitObjectCount(); }
	/**
	*/
	gkGameObject* getHitObject(int nr)              { return get()->getHitObject(nr);    }

	OGRE_KIT_LOGIC_BRICK(CollisionSensor);
};



class gsTouchSensor : public gsSensor
{
public:
	/**
		\LuaMethod{TouchSensor,constructor}

		Default constructor does nothing.

		\code
		function TouchSensor:constructor()
		\endcode
	*/
	gsTouchSensor();
	/**
		\sectionseperator{Overload:}

		Creates a new sensor.

		\code
		function TouchSensor:constructor(object, name="")
		\endcode

		\param object The parent \LuaClassRef{LogicObject}
		\param name   Unique identifier for this sensor. If this is empty one will be generated automatically

		\returns \LuaClassRef{TouchSensor}
	*/
	gsTouchSensor(gsLogicObject* parent, const gkString& name = "");
	~gsTouchSensor();

	/**
		\LuaMethod{TouchSensor,setMaterialName}

		Sets the name of the material this sensor responds to.

		\code
		function TouchSensor:setMaterialName(name)
		\endcode

		\param name string
	*/
	void      setMaterialName(const gkString& str)  { BRICK_SET(setMaterial(str));  }
	/**
		\LuaMethod{TouchSensor,getMaterialName}


		Gets the name of the material this sensor responds to.

		\code
		function TouchSensor:getMaterialName()
		\endcode

		\returns string
	*/
	gkString  getMaterialName(void)                 { BRICK_GET(getMaterial(), ""); }
	/**
		\LuaMethod{TouchSensor,setPropertyName}


		Sets the name of the property this sensor responds to.

		\code
		function TouchSensor:setPropertyName(name)
		\endcode

		\param name string
	*/
	void      setPropertyName(const gkString& str)  { BRICK_SET(setProperty(str));  }
	/**
		\LuaMethod{TouchSensor,getPropertyName}

		Gets the name of the property this sensor responds to.

		\code
		function TouchSensor:getPropertyName()
		\endcode

		\returns string
	*/
	gkString  getPropertyName(void)                 { BRICK_GET(getProperty(), ""); }

	/**

	*/
	int     getHitObjectsCount() {return get()->getHitObjectCount();}
	/**

	*/
	gkGameObject* getHitObject(int nr) { return get()->getHitObject(nr);}

	OGRE_KIT_LOGIC_BRICK(TouchSensor);
};



class gsDelaySensor : public gsSensor
{
public:
	/**	
		\LuaMethod{DelaySensor,constructor}

		Default constructor does nothing.

		\code
		function DelaySensor:constructor()
		\endcode
	*/
	gsDelaySensor();
	/**
		\sectionseperator{Overload:}

		Creates a new sensor.

		\code
		function DelaySensor:constructor(object, name="")
		\endcode

		\param object The parent \LuaClassRef{LogicObject}
		\param name   Unique identifier for this sensor. If this is empty one will be generated automatically

		\returns \LuaClassRef{DelaySensor}
	*/
	gsDelaySensor(gsLogicObject* parent, const gkString& name = "");
	~gsDelaySensor();


	/**
		\LuaMethod{DelaySensor,setDelay}

		Sets the delay in game ticks.

		\code
		function DelaySensor:setDelay(delay)
		\endcode

		\param delay number 
	*/
	void setDelay(int v)    { BRICK_SET(setDelay((unsigned int)gkAbs(v))); }
	/**
		\LuaMethod{DelaySensor,setDuration}

		Sets the execution time.

		\code
		function DelaySensor:setDuration(val)
		\endcode

		\param val number 
	*/
	void setDuration(int v) { BRICK_SET(setDuration((unsigned int)gkAbs(v))); }
	/**
		\LuaMethod{DelaySensor,setRepeat}

		Forces a delay -> execute(span) loop

		\code
		function DelaySensor:setRepeat(val)
		\endcode

		\param val bool 
	*/
	void setRepeat(bool v)  { BRICK_SET(setRepeat(v)); }
	/**
		\LuaMethod{DelaySensor,getDelay}

		Gets the delay in game ticks.

		\code
		function DelaySensor:getDelay()
		\endcode

		\returns number 		
	*/
	int  getDelay(void)     { BRICK_GET(getDelay(), 0);}
	/**
		\LuaMethod{DelaySensor,getDuration}

		Gets the execution time.

		\code
		function DelaySensor:getDuration()
		\endcode

		\returns number 
	*/
	int  getDuration(void)  { BRICK_GET(getDuration(), 0);}
	/**
		\LuaMethod{DelaySensor,getRepeat}

		Returns true if the sensor is in loop mode.

		\code
		function DelaySensor:getRepeat()
		\endcode

		\returns bool 		
	*/
	bool getRepeat(void)    { BRICK_GET(getRepeat(), false);}

	OGRE_KIT_LOGIC_BRICK(DelaySensor);
};



class gsKeyboardSensor : public gsSensor
{
public:
	/**
		\LuaMethod{KeyboardSensor,constructor}

		Default constructor does nothing.

		\code
		function KeyboardSensor:constructor()
		\endcode
	*/
	gsKeyboardSensor();
	/**
		\sectionseperator{Overload:}

		Creates a new sensor.

		\code
		function KeyboardSensor:constructor(object, name="")
		\endcode

		\param object The parent \LuaClassRef{LogicObject}
		\param name   Unique identifier for this sensor. If this is empty one will be generated automatically

		\returns \LuaClassRef{KeyboardSensor}
	*/
	gsKeyboardSensor(gsLogicObject* parent, const gkString& name = "");
	~gsKeyboardSensor();

	/**
		\LuaMethod{KeyboardSensor,setKey}

		Sets the keyboard scan code.

		\code
		function KeyboardSensor:setKey(code)
		\endcode

		\param code \LuaClassRef{ScanCode}
	*/
	void setKey(int scanCode)        {BRICK_SET(setKey(scanCode));}
	/**
		\LuaMethod{KeyboardSensor,setModifier0}

		Sets the first modifier scan code.

		\code
		function KeyboardSensor:setModifier0(code)
		\endcode

		\param code \LuaClassRef{ScanCode}		
	*/
	void setModifier0(int scanCode)  {BRICK_SET(setMod0(scanCode));}
	/**
		\LuaMethod{KeyboardSensor,setModifier1}

		Sets the second modifier scan code.

		\code
		function KeyboardSensor:setModifier1(code)
		\endcode

		\param code \LuaClassRef{ScanCode}
	*/
	void setModifier1(int scanCode)  {BRICK_SET(setMod1(scanCode));}
	/**
		\LuaMethod{KeyboardSensor,setAllKeys}

		Sets the sensor to react on any key press

		\code
		function KeyboardSensor:setAllKeys(val)
		\endcode

		\param val bool	
	*/
	void setAllKeys(bool v)          {BRICK_SET(setAllKeys(v));}
	/**
		\LuaMethod{KeyboardSensor,getKey}

		Gets the keyboard scan code.

		\code
		function KeyboardSensor:getKey()
		\endcode

		\returns \LuaClassRef{ScanCode}	
	*/
	int  getKey(void)                {BRICK_GET(getKey(), KC_NONE); }
	/**
		\LuaMethod{KeyboardSensor,getModifier0}

		Gets the first modifier scan code.

		\code
		function KeyboardSensor:getModifier0()
		\endcode

		\returns \LuaClassRef{ScanCode}
	*/
	int  getModifier0(void)          {BRICK_GET(getMod0(), KC_NONE);}
	/**
		\LuaMethod{KeyboardSensor,getModifier1}

		Gets the second modifier scan code.

		\code
		function KeyboardSensor:getModifier1()
		\endcode

		\returns \LuaClassRef{ScanCode}		
	*/
	int  getModifier1(void)          {BRICK_GET(getMod1(), KC_NONE);}
	/**
		\LuaMethod{KeyboardSensor,getAllKeys}

		Returns true if this sensor reacts to any key press.

		\code
		function KeyboardSensor:getAllKeys()
		\endcode

		\returns bool		
	*/
	int  getAllKeys(void)            {BRICK_GET(getAllKeys(), false);}

	OGRE_KIT_LOGIC_BRICK(KeyboardSensor);
};




class gsMessageSensor : public gsSensor
{
public:
	/**
		\LuaMethod{MessageSensor,constructor}

		Default constructor does nothing.

		\code
		function MessageSensor:constructor()
		\endcode
	*/
	gsMessageSensor();
	/**
		\sectionseperator{Overload:}

		Creates a new sensor.

		\code
		function MessageSensor:constructor(object, name="")
		\endcode

		\param object The parent \LuaClassRef{LogicObject}
		\param name   Unique identifier for this sensor. If this is empty one will be generated automatically

		\returns \LuaClassRef{MessageSensor}
	*/
	gsMessageSensor(gsLogicObject* parent, const gkString& name = "");
	~gsMessageSensor();

	/**
		\LuaMethod{MessageSensor,setSubject}

		Sets the subject to listen for, empty for all messages.

		\code
		function MessageSensor:setSubject(val)
		\endcode

		\param val string 
	*/
	void      setSubject(const gkString& v) {BRICK_SET(setSubject(v));}
	/**
		\LuaMethod{MessageSensor,getSubject}

		Gets the subject to listen for.

		\code
		function MessageSensor:getSubject()
		\endcode

		\returns string
	*/
	gkString  getSubject(void)              {BRICK_GET(getSubject(), "");}
	int	      getMessageCount(void) { BRICK_GET(getMessageCount(),0);}
	gkString  getMessageSubject(int msgNr) { return get()->getMessage(msgNr).m_subject;}
	gkString  getMessageBody(int msgNr) { return get()->getMessage(msgNr).m_body;}
	gkString  getMessageFrom(int msgNr) { return get()->getMessage(msgNr).m_from;}
	gkString  getMessageTo(int msgNr) { return get()->getMessage(msgNr).m_to;}
	OGRE_KIT_LOGIC_BRICK(MessageSensor);
};




class gsMouseSensor : public gsSensor
{
public:
	/**
		\LuaMethod{MouseSensor,constructor}

		Default constructor does nothing.

		\code
		function MouseSensor:constructor()
		\endcode
	*/
	gsMouseSensor();
	/**
		\sectionseperator{Overload:}

		Creates a new sensor.

		\code
		function MouseSensor:constructor(object, name="")
		\endcode

		\param object The parent \LuaClassRef{LogicObject}
		\param name   Unique identifier for this sensor. If this is empty one will be generated automatically

		\returns \LuaClassRef{MouseSensor}
	*/
	gsMouseSensor(gsLogicObject* parent, const gkString& name = "");
	~gsMouseSensor();

	/**
		\LuaMethod{MouseSensor,setType}

		Sets the type of event to listen for.

		\code
		function MouseSensor:setType(val)
		\endcode

		\param val \LuaClassRef{MouseSensorType}
	*/
	void setType(int v) {BRICK_SET(setType(v));}
	/**
		\LuaMethod{MouseSensor,getType}

		Gets the type of event currently listening for.

		\code
		function MouseSensor:getType()
		\endcode

		\returns \LuaClassRef{MouseSensorType}
	*/
	int  getType(int v) {BRICK_GET(getType(), MST_LEFT);}
	OGRE_KIT_LOGIC_BRICK(MouseSensor);
};



class gsNearSensor : public gsSensor
{
public:
	/**
		\LuaMethod{NearSensor,constructor}

		Default constructor does nothing.

		\code
		function NearSensor:constructor()
		\endcode
	*/
	gsNearSensor();
	/**
		\sectionseperator{Overload:}

		Creates a new sensor.

		\code
		function NearSensor:constructor(object, name="")
		\endcode

		\param object The parent \LuaClassRef{LogicObject}
		\param name   Unique identifier for this sensor. If this is empty one will be generated automatically

		\returns \LuaClassRef{NearSensor}
	*/
	gsNearSensor(gsLogicObject* parent, const gkString& name = "");
	~gsNearSensor();


	/**
		\LuaMethod{NearSensor,setRange}

		Sets the trigger distance

		\code
		function NearSensor:setRange(val)
		\endcode

		\param val number
	*/
	void     setRange(float v)              {BRICK_SET(setRange(v));}
	/**
		\LuaMethod{NearSensor,setResetRange}

		Sets the trigger reset distance

		\code
		function NearSensor:setResetRange(val)
		\endcode

		\param val number
	*/
	void     setResetRange(float v)         {BRICK_SET(setResetRange(v));}
	/**
		\LuaMethod{NearSensor,setMaterial}

		Sets the material name to look for. Blank for any.

		\code
		function NearSensor:setMaterial(val)
		\endcode

		\param val string
	*/
	void     setMaterial(const gkString& v) {BRICK_SET(setMaterial(v));}
	/**
		\LuaMethod{NearSensor,setProperty}

		Sets the property name to look for. Blank for any.

		\code
		function NearSensor:setProperty(val)
		\endcode

		\param val string
	*/
	void     setProperty(const gkString& v) {BRICK_SET(setProperty(v));}
	/**
		\LuaMethod{NearSensor,getRange}

		Gets the trigger distance

		\code
		function NearSensor:getRange()
		\endcode

		\returns number
	*/
	float    getRange(void)                 {BRICK_GET(getRange(), 0.f);}
	/**
		\LuaMethod{NearSensor,getResetRange}

		Gets the trigger reset distance

		\code
		function NearSensor:getResetRange()
		\endcode

		\returns number
	*/
	float    getResetRange(void)            {BRICK_GET(getResetRange(), 0.f);}
	/**
		\LuaMethod{NearSensor,getMaterial}

		Gets the material name to look for.

		\code
		function NearSensor:getMaterial()
		\endcode

		\returns string
	*/
	gkString getMaterial(void)              {BRICK_GET(getMaterial(), "");}
	/**
		\LuaMethod{NearSensor,getProperty}

		Gets the property name to look for.

		\code
		function NearSensor:getProperty()
		\endcode

		\returns string
	*/
	gkString getProperty(void)              {BRICK_GET(getProperty(), "");}
	/**
	*/
	int     getNearObjectCount()                 {return get()->getNearObjectCount();}
	/**
	*/
	const   gkGameObject* getNearObject(int nr)  {return get()->getNearObject(nr);   }

	OGRE_KIT_LOGIC_BRICK(NearSensor);
};



class gsPropertySensor : public gsSensor
{
public:
	/**
		\LuaMethod{PropertySensor,constructor}

		Default constructor does nothing.

		\code
		function PropertySensor:constructor()
		\endcode
	*/
	gsPropertySensor();
	/**
		\sectionseperator{Overload:}

		Creates a new sensor.

		\code
		function PropertySensor:constructor(object, name="")
		\endcode

		\param object The parent \LuaClassRef{LogicObject}
		\param name   Unique identifier for this sensor. If this is empty one will be generated automatically

		\returns \LuaClassRef{PropertySensor}
	*/
	gsPropertySensor(gsLogicObject* parent, const gkString& name = "");
	~gsPropertySensor();


	/**
		\LuaMethod{PropertySensor,setType}

		Sets the type of reaction this sensor has.

		\code
		function PropertySensor:setType(val)
		\endcode

		\param val \LuaClassRef{PropertySensorType}
	*/
	void      setType(int type)                  {BRICK_SET(setType(type));}
	/**
		\LuaMethod{PropertySensor,setPropertyName}

		Sets the property name to look for.

		\code
		function PropertySensor:setPropertyName(val)
		\endcode

		\param val string
	*/
	void      setPropertyName(const gkString& v) {BRICK_SET(setProperty(v));}
	/**
		\LuaMethod{PropertySensor,setValue}

		Sets the value for. PST_EQUAL and PST_NEQUAL operations.

		\code
		function PropertySensor:setValue(val)
		\endcode

		\param val string
	*/
	void      setValue(const gkString& v)        {BRICK_SET(setValue(v));}

	/**	
		\LuaMethod{PropertySensor,getType}

		Gets the type of reaction this sensor has.

		\code
		function PropertySensor:getType()
		\endcode

		\returns  \LuaClassRef{PropertySensorType}
	*/
	int       getType(void)                      {BRICK_GET(getType(), PST_EQUAL);}
	/**
		\LuaMethod{PropertySensor,getPropertyName}

		Gets the property name to look for.

		\code
		function PropertySensor:getPropertyName()
		\endcode

		\returns string
	*/
	gkString  getPropertyName(void)              {BRICK_GET(getProperty(), "");}
	/**
		\LuaMethod{PropertySensor,getValue}

		Gets the value for. PST_EQUAL and PST_NEQUAL operations.

		\code
		function PropertySensor:getValue()
		\endcode

		\returns string
	*/
	gkString  getValue(void)                     {BRICK_GET(getValue(), "");}

	OGRE_KIT_LOGIC_BRICK(PropertySensor);
};




class gsRaySensor : public gsSensor
{
public:
	/**
		\LuaMethod{RaySensor,constructor}

		Default constructor does nothing.

		\code
		function RaySensor:constructor()
		\endcode
	*/
	gsRaySensor();
	/**
		\sectionseperator{Overload:}

		Creates a new sensor.

		\code
		function RaySensor:constructor(object, name="")
		\endcode

		\param object The parent \LuaClassRef{LogicObject}
		\param name   Unique identifier for this sensor. If this is empty one will be generated automatically

		\returns \LuaClassRef{RaySensor}
	*/
	gsRaySensor(gsLogicObject* parent, const gkString& name = "");
	~gsRaySensor();

	/**
		\LuaMethod{RaySensor,setRange}

		Sets the distance of the ray.

		\code
		function RaySensor:setRange(val)
		\endcode

		\param val number
	*/
	void      setRange(float v)               {BRICK_SET(setRange(v));}
	/**
		\LuaMethod{RaySensor,getRange}

		Gets the distance of the ray.

		\code
		function RaySensor:getRange()
		\endcode

		\returns number
	*/
	float     getRange(void)                  {BRICK_GET(getRange(), 0.f);}
	/**
		\LuaMethod{RaySensor,setAxis}

		Sets the axis of the cast.

		\code
		function RaySensor:setAxis(val)
		\endcode

		\param val \LuaClassRef{RaySensorAxis}
	*/
	void      setAxis(int v)                  {BRICK_SET(setAxis(v));}
	/**
		\LuaMethod{RaySensor,getAxis}

		Gets the axis of the cast.

		\code
		function RaySensor:getAxis()
		\endcode

		\returns \LuaClassRef{RaySensorAxis}
	*/
	int       getAxis(void)                   {BRICK_GET(getAxis(), RSA_XPOS);}

	/**
		\LuaMethod{RaySensor,setMaterial}

		Sets the material name to look for. Blank for any.

		\code
		function RaySensor:setMaterial(val)
		\endcode

		\param val string
	*/
	void      setMaterial(const gkString& v)  {BRICK_SET(setMaterial(v));}
	/**
		\LuaMethod{RaySensor,getMaterial}

		Gets the material name to look for.

		\code
		function RaySensor:getMaterial()
		\endcode

		\returns string
	*/
	gkString  getMaterial(void)               {BRICK_GET(getMaterial(), "");}
	/**
		\LuaMethod{RaySensor,setProperty}

		Sets the property name to look for. Blank for any.

		\code
		function RaySensor:setProperty(val)
		\endcode

		\param val string
	*/
	void      setProperty(const gkString& v)  {BRICK_SET(setProperty(v));}
	/**
		\LuaMethod{RaySensor,getProperty}

		Gets the property name to look for.

		\code
		function RaySensor:getProperty()
		\endcode

		\returns string
	*/
	gkString  getProperty(void)               {BRICK_GET(getProperty(), "");}

	OGRE_KIT_LOGIC_BRICK(RaySensor);
};



class gsRadarSensor : public gsSensor
{
public:
	/**
		\LuaMethod{RadarSensor,constructor}

		Default constructor does nothing.

		\code
		function RadarSensor:constructor()
		\endcode
	*/
	gsRadarSensor();
	/**
		\sectionseperator{Overload:}

		Creates a new sensor.

		\code
		function RadarSensor:constructor(object, name="")
		\endcode

		\param object The parent \LuaClassRef{LogicObject}
		\param name   Unique identifier for this sensor. If this is empty one will be generated automatically

		\returns \LuaClassRef{RadarSensor}
	*/
	gsRadarSensor(gsLogicObject* parent, const gkString& name = "");
	~gsRadarSensor();

	/**
		\LuaMethod{RadarSensor,setRange}

		Sets the depth of the cone.

		\code
		function RadarSensor:setRange(val)
		\endcode

		\param val number
	*/
	void      setRange(float v)               {BRICK_SET(setRange(v));}
	/**
		\LuaMethod{RadarSensor,getRange}

		Gets the depth of the cone.

		\code
		function RadarSensor:getRange()
		\endcode

		\returns number
	*/
	float     getRange(void)                  {BRICK_GET(getRange(), 0.f);}
	/**
		\LuaMethod{RadarSensor,setAxis}

		Sets the axis of the cast.

		\code
		function RadarSensor:setAxis(val)
		\endcode

		\param val \LuaClassRef{RaySensorAxis}
	*/
	void      setAxis(int v)                  {BRICK_SET(setAxis(v));}
	/**
		\LuaMethod{RadarSensor,getAxis}

		Gets the axis of the cast.

		\code
		function RadarSensor:getAxis()
		\endcode

		\returns \LuaClassRef{RaySensorAxis}
	*/
	int       getAxis(void)                   {BRICK_GET(getAxis(), RSA_XPOS);}
	

	/**
		\LuaMethod{RadarSensor,setMaterial}

		Sets the material name to look for. Blank for any.

		\code
		function RadarSensor:setMaterial(val)
		\endcode

		\param val string
	*/
	void      setMaterial(const gkString& v)  {BRICK_SET(setMaterial(v));}
	/**
		\LuaMethod{RadarSensor,getMaterial}

		Gets the material name to look for.

		\code
		function RadarSensor:getMaterial()
		\endcode

		\returns string
	*/
	gkString  getMaterial(void)               {BRICK_GET(getMaterial(), "");}
	/**
		\LuaMethod{RadarSensor,setProperty}

		Sets the property name to look for. Blank for any.

		\code
		function RadarSensor:setProperty(val)
		\endcode

		\param val string
	*/
	void      setProperty(const gkString& v)  {BRICK_SET(setProperty(v));}
	/**
		\LuaMethod{RadarSensor,getProperty}

		Gets the property name to look for.

		\code
		function RadarSensor:getProperty()
		\endcode

		\returns string
	*/
	gkString  getProperty(void)               {BRICK_GET(getProperty(), "");}
	/**
		\LuaMethod{RadarSensor,setAngle}

		Sets the opening angle of the cone.

		\code
		function RadarSensor:setAngle(val)
		\endcode

		\param val number
	*/
	void      setAngle(float v)               {BRICK_SET(setAngle(v));}
	/**
		\LuaMethod{RadarSensor,getAngle}

		Gets the opening angle of the cone.

		\code
		function RadarSensor:getAngle()
		\endcode

		\returns number
	*/
	float     getAngle(void)                  {BRICK_GET(getAngle(), 0.f);}

	OGRE_KIT_LOGIC_BRICK(RadarSensor);
};




class gsRandomSensor : public gsSensor
{
public:
	/**
		\LuaMethod{RandomSensor,constructor}

		Default constructor does nothing.

		\code
		function RandomSensor:constructor()
		\endcode
	*/
	gsRandomSensor();
	/**
		\sectionseperator{Overload:}

		Creates a new sensor.

		\code
		function RandomSensor:constructor(object, name="")
		\endcode

		\param object The parent \LuaClassRef{LogicObject}
		\param name   Unique identifier for this sensor. If this is empty one will be generated automatically

		\returns \LuaClassRef{RandomSensor}
	*/
	gsRandomSensor(gsLogicObject* parent, const gkString& name = "");
	~gsRandomSensor();

	/**
		\LuaMethod{RandomSensor,setSeed}

		Sets the inital seed of the random generator.

		\code
		function RandomSensor:setSeed(val)
		\endcode

		\param val number
	*/
	void      setSeed(int v)               {BRICK_SET(setSeed((UTuint32)gkAbs(v)));}
	/**
		\LuaMethod{RandomSensor,getSeed}

		Gets the inital seed of the random generator.

		\code
		function RandomSensor:getSeed()
		\endcode

		\returns number
	*/
	int       getSeed(void)                {BRICK_GET(getSeed(), 0);}

	OGRE_KIT_LOGIC_BRICK(RandomSensor);
};


class gsController : public gsBrick
{
public:
	typedef utList<gkLogicBrick*> BrickList;
	/**

	*/
	gsController();
	~gsController();

	/**
		\LuaMethod{Controller,link}

		This is a two stage link. \n

		First the sensors output is linked to the controllers input.\n
		Second the controllers input is linked to the sensors output. \n

		\code
		function Controller:link(sensor)
		\endcode
	*/
	void link(gsSensor* sens);
	/**
		\sectionseperator{Overload:}

		This is a one stage link. \n
		The actuators input is linked to the controllers output.

		\code
		function Controller:link(actuator)
		\endcode
	*/
	void link(gsActuator* act);

	/**

	*/
	gkLogicSensor* getSensor(gkString sensorName);
	/**

	*/
	gkLogicActuator* getActuator(gkString actuatorName);


	OGRE_KIT_LOGIC_BRICK_BASE(Controller);
//	OGRE_KIT_TEMPLATE_NEW(gsController, gkLogicController);
};



class gsLogicOpController : public gsController
{
public:
	/**
		\LuaMethod{LogicOpController,constructor}

		Default constructor does nothing.

		\code
		function LogicOpController:constructor()
		\endcode
	*/
	gsLogicOpController(gsLogicObject* parent, const gkString& name = "");

	/**
		\sectionseperator{Overload:}

		Creates a new controller.

		\code
		function LogicOpController:constructor(object, name="")
		\endcode

		\param object The parent \LuaClassRef{LogicObject}
		\param name   Unique identifier for this controller. If this is empty one will be generated automatically

		\returns \LuaClassRef{LogicOpController}
	*/
	gsLogicOpController();
	~gsLogicOpController();

	/**
		\LuaMethod{LogicOpController,getOp}

		Gets the operation method.

		\code
		function LogicOpController:getOp()
		\endcode

		\returns \LuaClassRef{LogicOp}
	*/
	int         getOp(void)          {BRICK_GET(getOp(), LOP_NONE);}
	/**
		\LuaMethod{LogicOpController,setOp}

		Sets the operation method.

		\code
		function LogicOpController:setOp(op)
		\endcode

		\param op \LuaClassRef{LogicOp}
	*/
	void        setOp(gsLogicOp op)  {BRICK_SET(setOp((int)op));  }

	OGRE_KIT_LOGIC_BRICK(LogicOpController);
};

class gsExpressionController : public gsController
{
public:
	/**
		\LuaMethod{ExpressionController,constructor}

		Default constructor does nothing.

		\code
		function ExpressionController:constructor()
		\endcode
	*/
	gsExpressionController(gsLogicObject* parent, const gkString& name = "");
	/**
		\sectionseperator{Overload:}

		Creates a new controller.

		\code
		function ExpressionController:constructor(object, name="")
		\endcode

		\param object The parent \LuaClassRef{LogicObject}
		\param name   Unique identifier for this controller. If this is empty one will be generated automatically

		\returns \LuaClassRef{ExpressionController}
	*/
	gsExpressionController();
	~gsExpressionController();

	/**
		\LuaMethod{ExpressionController,setExpression}

		Sets the expression to call. This will be an Lua expression.

		\code
		function ScriptController:setExpression(expr)
		\endcode

		\param expr Lua expression.
	*/
	void setExpression(const gkString& expr)  { BRICK_SET( setExpression(expr) ); }


	OGRE_KIT_LOGIC_BRICK(ExpressionController);
};

class gsScriptController : public gsController
{
public:
	/**
		\LuaMethod{ScriptController,constructor}

		Default constructor does nothing.

		\code
		function ScriptController:constructor()
		\endcode
	*/
	gsScriptController(gsLogicObject* parent, const gkString& name = "");
	/**
		\sectionseperator{Overload:}

		Creates a new controller.

		\code
		function ScriptController:constructor(object, name="")
		\endcode

		\param object The parent \LuaClassRef{LogicObject}
		\param name   Unique identifier for this controller. If this is empty one will be generated automatically

		\returns \LuaClassRef{ScriptController}	
	*/
	gsScriptController();
	~gsScriptController();

	/**
		\LuaMethod{ScriptController,setScript}

		Sets the name of the script to call. This will be an internal Blender Text data block name.

		\code
		function ScriptController:setScript(name)
		\endcode

		\param name Text data block name.
	*/
	void setScript(const gkString& name)  { BRICK_SET( setScript(name) ); }
	/**

	*/
	void setScriptByString(const gkString& script) { BRICK_SET(  setScriptByString(script) );}
	/**

	*/
	void setLuaScript(gsFunction function) { BRICK_SET( setLuaScript(function) );};
	/**

	*/
	void setLuaScript(gsSelf self, gsFunction function){ BRICK_SET( setLuaScript(self,function) ); };
	OGRE_KIT_LOGIC_BRICK(ScriptController);
};

// global context
extern gsController* getCurrentController(void);




class gsActuator : public gsBrick
{
public:
	/**
	*/
	gsActuator();
	~gsActuator();

	OGRE_KIT_LOGIC_BRICK_BASE(Actuator);
//	OGRE_KIT_TEMPLATE_NEW(gsActuator, gkLogicActuator);
};



class gsActionActuator : public gsActuator
{
public:
	/**
	*/
	gsActionActuator(gsLogicObject* parent, const gkString& name = "");
	/**
	*/
	gsActionActuator();
	~gsActionActuator();
	/**
	*/
	void  setStart(int v)                 {BRICK_SET( setStart(v) );}
	/**
	*/
	void  setEnd(int v)                   {BRICK_SET( setEnd(v) );}
	/**
	*/
	void  setBlend(int v)                 {BRICK_SET( setBlend(v) );}
	/**
	*/
	void  setMode(int v)                  {BRICK_SET( setMode(v) );}
	/**
	*/
	void  setPriority(int v)              {BRICK_SET( setPriority(v) );}
	/**
	*/
	void  setAnimation(const gkString& v) {BRICK_SET( setAnimation(v) );}
	/**
	*/
	void  setProperty(const gkString& v)  {BRICK_SET( setProperty(v) );}
	/**
	*/
	void  setReset(bool v)                {BRICK_SET( setReset(v) );}
	/**
	*/
	void  reInit(void)					  {BRICK_SET( doInit() );}
	/**
	*/
	void  play(void);
	/**
	*/
	void  stop(void)					  {BRICK_SET( stopAction() );}
	/**
	*/
	int       getStart(void)              {BRICK_GET( getStart(), 0 );}
	/**
	*/
	int       getEnd(void)                {BRICK_GET( getEnd(), 0 );}
	/**
	*/
	int       getBlend(void)              {BRICK_GET( getBlend(), 0 );}
	/**
	*/
	int       getMode(void)               {BRICK_GET( getMode(), 0 );}
	/**
	*/
	int       getPriority(void)           {BRICK_GET( getPriority(), 0 );}
	/**
	*/
	gkString  getAnimation(void)          {BRICK_GET( getAnimation(), "" );}
	/**
	*/
	gkString  getProperty(void)           {BRICK_GET( getProperty(), "" );}
	/**
	*/
	bool      getReset(void)              {BRICK_GET( getReset(), 0 );}
	/**
	*/
	void 	  setAnimPosition(float time){BRICK_SET( setAnimationPosition(time));}
	/**
	*/
	float  getAnimPosition()			  {BRICK_GET( getAnimationPosition(), 0 );}
	/**
	*/
	float  getAnimLength(void)    {BRICK_GET( getAnimationLength(), 0 );}
	/**
	*/
	bool 	  isActionEnded(void)		  {BRICK_GET( isActionEnded(),false);}	
	OGRE_KIT_LOGIC_BRICK(ActionActuator);
};



class gsEditObjectActuator : public gsActuator
{
public:
	/**
	*/
	gsEditObjectActuator(gsLogicObject* parent, const gkString& name = "");
	/**
	*/
	gsEditObjectActuator();
	~gsEditObjectActuator();
	/**
	*/
	void  setType(int v)                                          {BRICK_SET( setMode(v) );}
	/**
	*/
	void  setDynMode(int v)                                       {BRICK_SET( setDynMode(v) );}
	/**
	*/
	void  setObject(const gkString& v)                            {BRICK_SET( setObject(v) );}
	/**
	*/
	void  setLinearVelocity(const gsVector3& v, bool isLocal)     {BRICK_SET( setLinV(v) ); BRICK_SET( setLinVL(isLocal) );}
	/**
	*/
	void  setAngularVelocity(const gsVector3& v, bool isLocal)    {BRICK_SET( setAngV(v) ); BRICK_SET( setAngVL(isLocal) );}
	/**
	*/
	void  setLifeSpan(int v)                                      {BRICK_SET( setLifeSpan(v) ); }
	/**
	*/
	int        getType(void)                  {BRICK_GET( getMode(),    EOT_ADDOBJ );}
	/**
	*/
	int        getDynMode(void)               {BRICK_GET( getDynMode(), EOD_RESTORE );}
	/**
	*/
	gkString   getObject(void)                {BRICK_GET( getObject(), "");}
	/**
	*/
	gsVector3  getLinearVelocity(void)        {BRICK_GET( getLinV(), gsVector3(0, 0, 0) );}
	/**
	*/
	gsVector3  getAngularVelocity(void)       {BRICK_GET( getAngV(), gsVector3(0, 0, 0) );}
	/**
	*/
	int        getLifeSpan(void)              {BRICK_GET( getLifeSpan(), 0 );}
	/**
	*/
	gkGameObject* getLastCreatedObject(void) {BRICK_GET( getLastCreatedObject(),0);}
	/**
	*/
	void refreshTrackTo()	{ get()->trackToObject(); }

	OGRE_KIT_LOGIC_BRICK(EditObjectActuator);
};



class gsGameActuator : public gsActuator
{
public:
	/**
	*/
	gsGameActuator(gsLogicObject* parent, const gkString& name = "");
	/**
	*/
	gsGameActuator();
	~gsGameActuator();
	/**
	*/
	void  setType(int v)   {BRICK_SET( setMode(v) );}
	/**
	*/
	int   getType(void)    {BRICK_GET( getMode(), GAT_QUIT );}


	OGRE_KIT_LOGIC_BRICK(GameActuator);
};



class gsMessageActuator : public gsActuator
{
public:
	/**
	*/
	gsMessageActuator(gsLogicObject* parent, const gkString& name = "");
	/**
	*/
	gsMessageActuator();
	~gsMessageActuator();
	/**
	*/
	void      setTo(const gkString& v)           {BRICK_SET( setTo(v) );}
	/**
	*/
	gkString  getTo(void)                        {BRICK_GET( getTo(), "" );}
	/**
	*/
	void      setSubject(const gkString& v)      {BRICK_SET( setSubject(v) );}
	/**
	*/
	gkString  getSubject(void)                   {BRICK_GET( getSubject(), "" );}
	/**
	*/
	void      setBodyText(const gkString& v)     {BRICK_SET( setBodyText(v) );}
	/**
	*/
	gkString  getBodyText(void)                  {BRICK_GET( getBodyText(), "" );}
	/**
	*/
	void      setBodyProperty(const gkString& v) {BRICK_SET( setBodyProperty(v) );}
	/**
	*/
	gkString  getBodyProperty(void)              {BRICK_GET( getBodyProperty(), "" );}
	/**
	*/
	void      setBodyType(int v)                 {BRICK_SET( setBodyType(v) );}
	/**
	*/
	int       getBodyType(void)                  {BRICK_GET( getBodyType(), MBT_TEXT );}
	
	OGRE_KIT_LOGIC_BRICK(MessageActuator);
};


class gsMotionActuator : public gsActuator
{
public:
	/**
	*/
	gsMotionActuator(gsLogicObject* parent, const gkString& name = "");
	/**
	*/
	gsMotionActuator();
	~gsMotionActuator();
	/**
	*/
	void setTranslation(float x, float y, float z, bool tsLocal = true)     { BRICK_SET( setTranslation(gkVector3(x, y, z), tsLocal) ); }
	/**
	*/
	void setTranslation(const gsVector3& vec, bool tsLocal = true)          { BRICK_SET( setTranslation(vec, tsLocal) ); }
	/**
	*/
	void setRotation(float x, float y, float z, bool tsLocal = true)        { BRICK_SET( setRotation(gkVector3(x, y, z), tsLocal) ); }
	/**
	*/
	void setRotation(const gsVector3& vec, bool tsLocal = true)             { BRICK_SET( setRotation(vec, tsLocal) ); }
	/**
	*/
	void setForce(float x, float y, float z, bool tsLocal = true)           { BRICK_SET( setForce(gkVector3(x, y, z), tsLocal) ); }
	/**
	*/
	void setForce(const gsVector3& vec, bool tsLocal = true)                { BRICK_SET( setForce(vec, tsLocal) ); }
	/**
	*/
	void setTorque(float x, float y, float z, bool tsLocal = true)          { BRICK_SET( setTorque(gkVector3(x, y, z), tsLocal) ); }
	/**
	*/
	void setTorque(const gsVector3& vec, bool tsLocal = true)               { BRICK_SET( setTorque(vec, tsLocal) ); }

	/**
	*/
	void setLinearVelocity(float x, float y, float z, bool tsLocal = true)  { BRICK_SET( setLinearVelocity(gkVector3(x, y, z), tsLocal) ); }
	/**
	*/
	void setLinearVelocity(const gsVector3& vec, bool tsLocal = true)       { BRICK_SET( setLinearVelocity(vec, tsLocal) ); }

	/**
	*/
	void setAngularVelocity(float x, float y, float z, bool tsLocal = true) { BRICK_SET( setAngularVelocity(gkVector3(x, y, z), tsLocal) ); }
	/**
	*/
	void setAngularVelocity(const gsVector3& vec, bool tsLocal = true)      { BRICK_SET( setAngularVelocity(vec, tsLocal) ); }

	/**
	*/
	void  setIncrementalTargetVelocity(float v)   { BRICK_SET( setDamping(v) ); }
	/**
	*/
	void  setIncrementalVelocity(bool v)          { BRICK_SET( setIncrementalVelocity(v) ); }
	/**
	*/
	float getIncrementalTargetVelocity(void)      { BRICK_GET( getDamping() , 0.f ); }
	/**
	*/
	bool  getIncrementalVelocity(void)            { BRICK_GET( getIncrementalVelocity(), false ); }

	/**
	*/
	gsVector3 getTranslation(void)        {BRICK_GET(getTranslation(), gkVector3(0, 0, 0))}
	/**
	*/
	gsVector3 getRotation(void)           {BRICK_GET(getRotation(), gkVector3(0, 0, 0))}
	/**
	*/
	gsVector3 getForce(void)              {BRICK_GET(getForce(), gkVector3(0, 0, 0))}
	/**
	*/
	gsVector3 getTorque(void)             {BRICK_GET(getTorque(), gkVector3(0, 0, 0))}
	/**
	*/
	gsVector3 getLinearVelocity(void)     {BRICK_GET(getLinearVelocity(), gkVector3(0, 0, 0))}
	/**
	*/
	gsVector3 getAngularVelocity(void)    {BRICK_GET(getAngularVelocity(), gkVector3(0, 0, 0))}

	OGRE_KIT_LOGIC_BRICK(MotionActuator);
};


class gsParentActuator : public gsActuator
{
public:
	/**
	*/
	gsParentActuator(gsLogicObject* parent, const gkString& name = "");
	/**
	*/
	gsParentActuator();
	~gsParentActuator();
	/**
	*/
	void      setMode(int v)               {BRICK_SET( setMode(v) );}
	/**
	*/
	int       getMode(void)                {BRICK_GET( getMode(), PA_SET );}
	/**
	*/
	void      setParent(const gkString& v) {BRICK_SET( setParent(v) );}
	/**
	*/
	gkString  getParent(void)              {BRICK_GET( getParent(), "" );}
	/**
	*/
	void      setCompound(bool v)          {BRICK_SET( setCompound(v) );}
	/**
	*/
	bool      getCompound(void)            {BRICK_GET( getCompound(), false );}
	/**
	*/
	void      setGhost(bool v)             {BRICK_SET( setGhost(v) );}
	/**
	*/
	bool      getGhost(void)               {BRICK_GET( getGhost(), false );}


	OGRE_KIT_LOGIC_BRICK(ParentActuator);
};



class gsPropertyActuator : public gsActuator
{
public:
	/**
	*/
	gsPropertyActuator(gsLogicObject* parent, const gkString& name = "");
	/**
	*/
	gsPropertyActuator();
	~gsPropertyActuator();
	/**
	*/
	void      setType(int v)                  {BRICK_SET( setType(v) );}
	/**
	*/
	int       getType(void)                   {BRICK_GET( getType(), PA_ASSIGN );}
	/**
	*/
	void      setProperty(const gkString& v)  {BRICK_SET( setProperty(v) );}
	/**
	*/
	gkString  getProperty(void)               {BRICK_GET( getProperty(), "" );}
	/**
	*/
	void      setValue(const gkString& v)     {BRICK_SET( setValue(v) );}
	/**
	*/
	gkString  getValue(void)                  {BRICK_GET( getValue(), "" );}
	/**
	*/
	void      setObject(const gkString& v)    {BRICK_SET( setObject(v) );}
	/**
	*/
	gkString  getObject(void)                 {BRICK_GET( getObject(), "" );}


	OGRE_KIT_LOGIC_BRICK(PropertyActuator);
};


class gsRandomActuator : public gsActuator
{
public:
	/**
	*/
	gsRandomActuator(gsLogicObject* parent, const gkString& name = "");
	/**
	*/
	gsRandomActuator();
	~gsRandomActuator();

	/**
	*/
	void     setSeed(int v)                 {BRICK_SET( setSeed(v) );}
	/**
	*/
	int      getSeed(void)                  {BRICK_GET( getSeed(), 0 );}
	/**
	*/
	void     setDistribution(int v)         {BRICK_SET( setDistribution(v) );}
	/**
	*/
	int      getDistribution(void)          {BRICK_GET( getDistribution(), RA_BOOL_CONSTANT );}
	/**
	*/
	void     setProperty(const gkString& v) {BRICK_SET( setProperty(v) );}
	/**
	*/
	gkString getProperty(void)              {BRICK_GET( getProperty(), "" );}
	/**
	*/
	void     setMin(float v)                {BRICK_SET( setMin(v) );}
	/**
	*/
	float    getMin(void)                   {BRICK_GET( getMin(), 0 );}
	/**
	*/
	void     setMax(float v)                {BRICK_SET( setMax(v) );}
	/**
	*/
	float    getMax(void)                   {BRICK_GET( getMax(), 0 );}
	/**
	*/
	void     setConstant(float v)           {BRICK_SET( setConstant(v) );}
	/**
	*/
	float    getConstant(void)              {BRICK_GET( getConstant(), 0 );}
	/**
	*/
	void     setMean(float v)               {BRICK_SET( setMean(v) );}
	/**
	*/
	float    getMean(void)                  {BRICK_GET( getMean(), 0 );}
	/**
	*/
	void     setDeviation(float v)          {BRICK_SET( setDeviation(v) );}
	/**
	*/
	float    getDeviation(void)             {BRICK_GET( getDeviation(), 0 );}
	/**
	*/
	void     setHalfLife(float v)           {BRICK_SET( setHalfLife(v) );}
	/**
	*/
	float    getHalfLife(void)              {BRICK_GET( getHalfLife(), 0 );}


	OGRE_KIT_LOGIC_BRICK(RandomActuator);
};


class gsSceneActuator : public gsActuator
{
public:
	/**
	*/
	gsSceneActuator(gsLogicObject* parent, const gkString& name = "");
	/**
	*/
	gsSceneActuator();
	~gsSceneActuator();

	/**
	*/
	void     setMode(int v)                  {BRICK_SET( setMode(v) );}
	/**
	*/
	int      getMode(void)                   {BRICK_GET( getMode(), SC_RESTART );}
	/**
	*/
	void     setScene(const gkString& v)     {BRICK_SET( setScene(v) );}
	/**
	*/
	gkString getScene(void)                  {BRICK_GET( getScene(), "" );}
	/**
	*/
	void     setCamera(const gkString& v)    {BRICK_SET( setCamera(v) );}
	/**
	*/
	gkString getCamera(void)                 {BRICK_GET( getCamera(), "" );}


	OGRE_KIT_LOGIC_BRICK(SceneActuator);
};


class gsSoundActuator : public gsActuator
{
public:
	/**
	*/
	gsSoundActuator(gsLogicObject* parent, const gkString& name = "");
	/**
	*/
	gsSoundActuator();
	~gsSoundActuator();
	/**
	*/
	void     setType(int v)                      {BRICK_SET( setMode(v) );}
	/**
	*/
	int      getType(void)                       {BRICK_GET( getMode(), SA_PLAY_STOP );}
	/**
	*/
	void     setSoundFile(const gkString& v)     {BRICK_SET( setSoundFile(v) );}
	/**
	*/
	gkString getSoundFile(void)                  {BRICK_GET( getSoundFile(), "" );}
	/**
	*/
	void setVolume(float volume);
	/**
	*/
	float getVolume(void);
	/**
	*/
	void setPitch(float pitch);
	/**
	*/
	float getPitch(void);

	OGRE_KIT_LOGIC_BRICK(SoundActuator);
};



class gsStateActuator : public gsActuator
{
public:
	/**
	*/
	gsStateActuator(gsLogicObject* parent, const gkString& name = "");
	/**
	*/
	gsStateActuator();
	~gsStateActuator();
	/**
	*/
	void     setMask(int v) {BRICK_SET( setMask(v) );}
	int      getMask(void)  {BRICK_GET( getMask(), 0 );}
	/**
	*/
	void     setOp(int v)   {BRICK_SET( setOp(v) );}
	/**
	*/
	int      getOp(void)    {BRICK_GET( getOp(), SOP_ADD );}

	OGRE_KIT_LOGIC_BRICK(StateActuator);
};



class gsVisibilityActuator : public gsActuator
{
public:
	/**
	*/
	gsVisibilityActuator(gsLogicObject* parent, const gkString& name = "");
	/**
	*/
	gsVisibilityActuator();
	~gsVisibilityActuator();

	/**
	*/
	void     setFlag(int v) {BRICK_SET( setFlag(v) );}
	/**
	*/
	int      getFlag(void)  {BRICK_GET( getFlag(), 0 );}

	OGRE_KIT_LOGIC_BRICK(VisibilityActuator);
};

/** @} */

#endif//_gsBricks_h_
