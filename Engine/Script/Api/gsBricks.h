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
#ifndef _gsBricks_h_
#define _gsBricks_h_


#include "gsCommon.h"
#include "gsMath.h"
#include "gsUtils.h"
#include "Script/Lua/gkLuaUtils.h"

#ifndef SWIG
#define BRICK_GET(METH, DEF) return (m_brick ? get()->METH : DEF);
#define BRICK_SET(METH) if (m_brick) get()->METH;
#endif


enum gsBrickMode
{
	GS_BM_NONE = BM_IDLE,
	GS_BM_OFF  = BM_OFF,
	GS_BM_ON   = BM_ON,
};



enum gsPulseMode
{
	PM_NONE = gkLogicSensor::PM_IDLE,
	PM_OFF  = gkLogicSensor::PM_FALSE,
	PM_ON   = gkLogicSensor::PM_TRUE,
};


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


enum gsListenerMode
{
	LM_EVT_OVERIDE,
	LM_EVT_AND,
};



enum gsPropertySensorType
{
	PST_EQUAL    = gkPropertySensor::PS_EQUAL,
	PST_NEQUAL   = gkPropertySensor::PS_NEQUAL,
	PST_INTERVAL = gkPropertySensor::PS_INTERVAL,
	PST_CHANGED  = gkPropertySensor::PS_CHANGED,
};



enum gsRaySensorAxis
{
	RSA_XPOS = gkRaySensor::RA_XPOS,
	RSA_YPOS = gkRaySensor::RA_YPOS,
	RSA_ZPOS = gkRaySensor::RA_ZPOS,
	RSA_XNEG = gkRaySensor::RA_XNEG,
	RSA_YNEG = gkRaySensor::RA_YNEG,
	RSA_ZNEG = gkRaySensor::RA_ZNEG,
};



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
	EOT_TRACKTO       = gkEditObjectActuator::EO_TRACKTO,
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
	gsLogicManager();
	~gsLogicManager();


	gkLogicLink* newObject(gsGameObject* obj);
	gkLogicLink* getObject(const gkString& name);
	gsArray<gsLogicObject, gkLogicLink> getObjectList();
};



class gsLogicObject
{
private:

	int m_incr;

public:
	gsLogicObject();
	~gsLogicObject();

	gkString             getName(void);
	void                 setDebug(bool v);
	bool                 isDebug(void);


	gkLogicSensor*      getSensor(const gkString& name);
	gkLogicController*  getController(const gkString& name);
	gkLogicActuator*    getActuator(const gkString& name);

	gsArray<gsSensor, gkLogicSensor>         getSensors();
	gsArray<gsController, gkLogicController> getControllers();
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

	gkString     getName(void);


	bool isOn(void)      {BRICK_GET(isPulseOn(), false); }
	bool isOff(void)     {BRICK_GET(isPulseOff(), false);}
	bool isActive(void)  {BRICK_GET(isActive(), false);}
	gkGameObject* getOwner(void);
	gkLogicLink* getLogicObject(void){ return get()->getLink();}


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

	void            setPulse(int mode)            { BRICK_SET(setMode(mode));  }
	int             getPulse(void)                { BRICK_GET(getMode(), PM_NONE); }
	void            setFrequency(float f)         { BRICK_SET(setFrequency(f)); }
	float           getFrequency(void)            { BRICK_GET(getFrequency(), 0.f); }
	void            setLevel(bool v)              { BRICK_SET(setDetector(v)); }
	bool            isLevel(void)                 { BRICK_GET(isDetector(), false);}
	void            setTap(bool v)                { BRICK_SET(setTap(v)); }
	bool            isTap(void)                   { BRICK_GET(isTap(), false); }
	void            setInvert(bool v)             { BRICK_SET(invert(v)); }
	bool            isInverse(void)               { BRICK_GET(isInverse(), false); }

	bool			isPositive(void) 			  { BRICK_GET(isPositive(),false); }

	OGRE_KIT_LOGIC_BRICK_BASE(Sensor);
//	OGRE_KIT_TEMPLATE_NEW(gsSensor, gkLogicSensor);
};




class gsActuatorSensor : public gsSensor
{
public:
	gsActuatorSensor();
	gsActuatorSensor(gsLogicObject* parent, const gkString& name = "");
	~gsActuatorSensor();

	void      setActuatorName(const gkString& str)  { BRICK_SET(setActuatorName(str));  }
	gkString  getActuatorName(void)                 { BRICK_GET(getActuatorName(), ""); }

	OGRE_KIT_LOGIC_BRICK(ActuatorSensor);
};



class gsAlwaysSensor : public gsSensor
{
public:
	gsAlwaysSensor();
	gsAlwaysSensor(gsLogicObject* parent, const gkString& name = "");
	~gsAlwaysSensor();


	OGRE_KIT_LOGIC_BRICK(AlwaysSensor);
};



class gsCollisionSensor : public gsSensor
{
public:
	gsCollisionSensor();
	gsCollisionSensor(gsLogicObject* parent, const gkString& name = "");
	~gsCollisionSensor();


	void      setMaterialName(const gkString& str)  { BRICK_SET(setMaterial(str));  }
	gkString  getMaterialName(void)                 { BRICK_GET(getMaterial(), ""); }

	void      setPropertyName(const gkString& str)  { BRICK_SET(setProperty(str));  }
	gkString  getPropertyName(void)                 { BRICK_GET(getProperty(), ""); }

	int           getHitObjectsCount()              { return get()->getHitObjectCount(); }
	gkGameObject* getHitObject(int nr)              { return get()->getHitObject(nr);    }

	OGRE_KIT_LOGIC_BRICK(CollisionSensor);
};



class gsTouchSensor : public gsSensor
{
public:
	gsTouchSensor();
	gsTouchSensor(gsLogicObject* parent, const gkString& name = "");
	~gsTouchSensor();


	void      setMaterialName(const gkString& str)  { BRICK_SET(setMaterial(str));  }
	gkString  getMaterialName(void)                 { BRICK_GET(getMaterial(), ""); }

	void      setPropertyName(const gkString& str)  { BRICK_SET(setProperty(str));  }
	gkString  getPropertyName(void)                 { BRICK_GET(getProperty(), ""); }

	int     getHitObjectsCount() {return get()->getHitObjectCount();}
	gkGameObject* getHitObject(int nr) { return get()->getHitObject(nr);}

	OGRE_KIT_LOGIC_BRICK(TouchSensor);
};



class gsDelaySensor : public gsSensor
{
public:
	gsDelaySensor();
	gsDelaySensor(gsLogicObject* parent, const gkString& name = "");
	~gsDelaySensor();



	void setDelay(int v)    { BRICK_SET(setDelay((unsigned int)gkAbs(v))); }
	void setDuration(int v) { BRICK_SET(setDuration((unsigned int)gkAbs(v))); }
	void setRepeat(bool v)  { BRICK_SET(setRepeat(v)); }
	int  getDelay(void)     { BRICK_GET(getDelay(), 0);}
	int  getDuration(void)  { BRICK_GET(getDuration(), 0);}
	bool getRepeat(void)    { BRICK_GET(getRepeat(), false);}

	OGRE_KIT_LOGIC_BRICK(DelaySensor);
};



class gsKeyboardSensor : public gsSensor
{
public:
	gsKeyboardSensor();
	gsKeyboardSensor(gsLogicObject* parent, const gkString& name = "");
	~gsKeyboardSensor();


	void setKey(int scanCode)        {BRICK_SET(setKey(scanCode));}
	void setModifier0(int scanCode)  {BRICK_SET(setMod0(scanCode));}
	void setModifier1(int scanCode)  {BRICK_SET(setMod1(scanCode));}
	void setAllKeys(bool v)          {BRICK_SET(setAllKeys(v));}
	int  getKey(void)                {BRICK_GET(getKey(), KC_NONE); }
	int  getModifier0(void)          {BRICK_GET(getMod0(), KC_NONE);}
	int  getModifier1(void)          {BRICK_GET(getMod1(), KC_NONE);}
	int  getAllKeys(void)            {BRICK_GET(getAllKeys(), false);}

	OGRE_KIT_LOGIC_BRICK(KeyboardSensor);
};




class gsMessageSensor : public gsSensor
{
public:
	gsMessageSensor();
	gsMessageSensor(gsLogicObject* parent, const gkString& name = "");
	~gsMessageSensor();


	void      setSubject(const gkString& v) {BRICK_SET(setSubject(v));}
	gkString  getSubject(void)              {BRICK_GET(getSubject(), "");}

	OGRE_KIT_LOGIC_BRICK(MessageSensor);
};




class gsMouseSensor : public gsSensor
{
public:
	gsMouseSensor();
	gsMouseSensor(gsLogicObject* parent, const gkString& name = "");
	~gsMouseSensor();

	void setType(int v) {BRICK_SET(setType(v));}
	int  getType(int v) {BRICK_GET(getType(), MST_LEFT);}

	OGRE_KIT_LOGIC_BRICK(MouseSensor);
};



class gsNearSensor : public gsSensor
{
public:
	gsNearSensor();
	gsNearSensor(gsLogicObject* parent, const gkString& name = "");
	~gsNearSensor();


	void     setRange(float v)              {BRICK_SET(setRange(v));}
	void     setResetRange(float v)         {BRICK_SET(setResetRange(v));}
	void     setMaterial(const gkString& v) {BRICK_SET(setMaterial(v));}
	void     setProperty(const gkString& v) {BRICK_SET(setProperty(v));}
	float    getRange(void)                 {BRICK_GET(getRange(), 0.f);}
	float    getResetRange(void)            {BRICK_GET(getResetRange(), 0.f);}
	gkString getMaterial(void)              {BRICK_GET(getMaterial(), "");}
	gkString getProperty(void)              {BRICK_GET(getProperty(), "");}

	int     getNearObjectCount()                 {return get()->getNearObjectCount();}
	const   gkGameObject* getNearObject(int nr)  {return get()->getNearObject(nr);   }

	OGRE_KIT_LOGIC_BRICK(NearSensor);
};



class gsPropertySensor : public gsSensor
{
public:
	gsPropertySensor();
	gsPropertySensor(gsLogicObject* parent, const gkString& name = "");
	~gsPropertySensor();



	void      setType(int type)                  {BRICK_SET(setType(type));}
	void      setPropertyName(const gkString& v) {BRICK_SET(setProperty(v));}
	void      setValue(const gkString& v)        {BRICK_SET(setValue(v));}

	int       getType(void)                      {BRICK_GET(getType(), PST_EQUAL);}
	gkString  getPropertyName(void)              {BRICK_GET(getProperty(), "");}
	gkString  getValue(void)                     {BRICK_GET(getValue(), "");}

	OGRE_KIT_LOGIC_BRICK(PropertySensor);
};




class gsRaySensor : public gsSensor
{
public:
	gsRaySensor();
	gsRaySensor(gsLogicObject* parent, const gkString& name = "");
	~gsRaySensor();

	void      setRange(float v)               {BRICK_SET(setRange(v));}
	float     getRange(void)                  {BRICK_GET(getRange(), 0.f);}
	void      setAxis(int v)                  {BRICK_SET(setAxis(v));}
	int       getAxis(void)                   {BRICK_GET(getAxis(), RSA_XPOS);}


	void      setMaterial(const gkString& v)  {BRICK_SET(setMaterial(v));}
	gkString  getMaterial(void)               {BRICK_GET(getMaterial(), "");}
	void      setProperty(const gkString& v)  {BRICK_SET(setProperty(v));}
	gkString  getProperty(void)               {BRICK_GET(getProperty(), "");}

	OGRE_KIT_LOGIC_BRICK(RaySensor);
};



class gsRadarSensor : public gsSensor
{
public:
	gsRadarSensor();
	gsRadarSensor(gsLogicObject* parent, const gkString& name = "");
	~gsRadarSensor();

	void      setRange(float v)               {BRICK_SET(setRange(v));}
	float     getRange(void)                  {BRICK_GET(getRange(), 0.f);}
	void      setAxis(int v)                  {BRICK_SET(setAxis(v));}
	int       getAxis(void)                   {BRICK_GET(getAxis(), RSA_XPOS);}


	void      setMaterial(const gkString& v)  {BRICK_SET(setMaterial(v));}
	gkString  getMaterial(void)               {BRICK_GET(getMaterial(), "");}
	void      setProperty(const gkString& v)  {BRICK_SET(setProperty(v));}
	gkString  getProperty(void)               {BRICK_GET(getProperty(), "");}

	void      setAngle(float v)               {BRICK_SET(setAngle(v));}
	float     getAngle(void)                  {BRICK_GET(getAngle(), 0.f);}

	OGRE_KIT_LOGIC_BRICK(RadarSensor);
};




class gsRandomSensor : public gsSensor
{
public:
	gsRandomSensor();
	gsRandomSensor(gsLogicObject* parent, const gkString& name = "");
	~gsRandomSensor();

	void      setSeed(int v)               {BRICK_SET(setSeed((UTuint32)gkAbs(v)));}
	int       getSeed(void)                {BRICK_GET(getSeed(), 0);}

	OGRE_KIT_LOGIC_BRICK(RandomSensor);
};



class gsController : public gsBrick
{
public:
	typedef utList<gkLogicBrick*> BrickList;

	gsController();
	~gsController();


	void link(gsSensor* sens);
	void link(gsActuator* act);

	gkLogicSensor* getSensor(gkString sensorName);
	gkLogicActuator* getActuator(gkString actuatorName);


	OGRE_KIT_LOGIC_BRICK_BASE(Controller);
//	OGRE_KIT_TEMPLATE_NEW(gsController, gkLogicController);
};



class gsLogicOpController : public gsController
{
public:
	gsLogicOpController(gsLogicObject* parent, const gkString& name = "");


	gsLogicOpController();
	~gsLogicOpController();


	int         getOp(void)          {BRICK_GET(getOp(), LOP_NONE);}
	void        setOp(gsLogicOp op)  {BRICK_SET(setOp((int)op));  }

	OGRE_KIT_LOGIC_BRICK(LogicOpController);
};

class gsExpressionController : public gsController
{
public:
	gsExpressionController(gsLogicObject* parent, const gkString& name = "");
	gsExpressionController();
	~gsExpressionController();

	void setExpression(const gkString& expr)  { BRICK_SET( setExpression(expr) ); }


	OGRE_KIT_LOGIC_BRICK(ExpressionController);
};

class gsScriptController : public gsController
{
public:
	gsScriptController(gsLogicObject* parent, const gkString& name = "");
	gsScriptController();
	~gsScriptController();

	void setScript(const gkString& name)  { BRICK_SET( setScript(name) ); }


	OGRE_KIT_LOGIC_BRICK(ScriptController);
};

// global context
extern gsScriptController* getCurrentController(void);




class gsActuator : public gsBrick
{
public:
	gsActuator();
	~gsActuator();

	OGRE_KIT_LOGIC_BRICK_BASE(Actuator);
//	OGRE_KIT_TEMPLATE_NEW(gsActuator, gkLogicActuator);
};



class gsActionActuator : public gsActuator
{
public:
	gsActionActuator(gsLogicObject* parent, const gkString& name = "");
	gsActionActuator();
	~gsActionActuator();

	void  setStart(int v)                 {BRICK_SET( setStart(v) );}
	void  setEnd(int v)                   {BRICK_SET( setEnd(v) );}
	void  setBlend(int v)                 {BRICK_SET( setBlend(v) );}
	void  setMode(int v)                  {BRICK_SET( setMode(v) );}
	void  setPriority(int v)              {BRICK_SET( setPriority(v) );}
	void  setAnimation(const gkString& v) {BRICK_SET( setAnimation(v) );}
	void  setProperty(const gkString& v)  {BRICK_SET( setProperty(v) );}
	void  setReset(bool v)                {BRICK_SET( setReset(v) );}

	int       getStart(void)              {BRICK_GET( getStart(), 0 );}
	int       getEnd(void)                {BRICK_GET( getEnd(), 0 );}
	int       getBlend(void)              {BRICK_GET( getBlend(), 0 );}
	int       getMode(void)               {BRICK_GET( getMode(), 0 );}
	int       getPriority(void)           {BRICK_GET( getPriority(), 0 );}
	gkString  getAnimation(void)          {BRICK_GET( getAnimation(), "" );}
	gkString  getProperty(void)           {BRICK_GET( getProperty(), "" );}
	bool      getReset(void)              {BRICK_GET( getReset(), 0 );}

	OGRE_KIT_LOGIC_BRICK(ActionActuator);
};



class gsEditObjectActuator : public gsActuator
{
public:
	gsEditObjectActuator(gsLogicObject* parent, const gkString& name = "");
	gsEditObjectActuator();
	~gsEditObjectActuator();

	void  setType(int v)                                          {BRICK_SET( setMode(v) );}
	void  setDynMode(int v)                                       {BRICK_SET( setDynMode(v) );}
	void  setObject(const gkString& v)                            {BRICK_SET( setObject(v) );}
	void  setLinearVelocity(const gsVector3& v, bool isLocal)     {BRICK_SET( setLinV(v) ); BRICK_SET( setLinVL(isLocal) );}
	void  setAngularVelocity(const gsVector3& v, bool isLocal)    {BRICK_SET( setAngV(v) ); BRICK_SET( setAngVL(isLocal) );}
	void  setLifeSpan(int v)                                      {BRICK_SET( setLifeSpan(v) ); }

	int        getType(void)                  {BRICK_GET( getMode(),    EOT_ADDOBJ );}
	int        getDynMode(void)               {BRICK_GET( getDynMode(), EOD_RESTORE );}
	gkString   getObject(void)                {BRICK_GET( getObject(), "");}
	gsVector3  getLinearVelocity(void)        {BRICK_GET( getLinV(), gsVector3(0, 0, 0) );}
	gsVector3  getAngularVelocity(void)       {BRICK_GET( getAngV(), gsVector3(0, 0, 0) );}
	int        getLifeSpan(void)              {BRICK_GET( getLifeSpan(), 0 );}

	gkGameObject* getLastCreatedObject(void) {BRICK_GET( getLastCreatedObject(),0);}

	OGRE_KIT_LOGIC_BRICK(EditObjectActuator);
};



class gsGameActuator : public gsActuator
{
public:
	gsGameActuator(gsLogicObject* parent, const gkString& name = "");
	gsGameActuator();
	~gsGameActuator();

	void  setType(int v)   {BRICK_SET( setMode(v) );}
	int   getType(void)    {BRICK_GET( getMode(), GAT_QUIT );}


	OGRE_KIT_LOGIC_BRICK(GameActuator);
};



class gsMessageActuator : public gsActuator
{
public:
	gsMessageActuator(gsLogicObject* parent, const gkString& name = "");
	gsMessageActuator();
	~gsMessageActuator();

	void      setTo(const gkString& v)           {BRICK_SET( setTo(v) );}
	gkString  getTo(void)                        {BRICK_GET( getTo(), "" );}
	void      setSubject(const gkString& v)      {BRICK_SET( setSubject(v) );}
	gkString  getSubject(void)                   {BRICK_GET( getSubject(), "" );}
	void      setBodyText(const gkString& v)     {BRICK_SET( setBodyText(v) );}
	gkString  getBodyText(void)                  {BRICK_GET( getBodyText(), "" );}
	void      setBodyProperty(const gkString& v) {BRICK_SET( setBodyProperty(v) );}
	gkString  getBodyProperty(void)              {BRICK_GET( getBodyProperty(), "" );}
	void      setBodyType(int v)                 {BRICK_SET( setBodyType(v) );}
	int       getBodyType(void)                  {BRICK_GET( getBodyType(), MBT_TEXT );}

	OGRE_KIT_LOGIC_BRICK(MessageActuator);
};


class gsMotionActuator : public gsActuator
{
public:
	gsMotionActuator(gsLogicObject* parent, const gkString& name = "");
	gsMotionActuator();
	~gsMotionActuator();

	void setTranslation(float x, float y, float z, bool tsLocal = true)     { BRICK_SET( setTranslation(gkVector3(x, y, z), tsLocal) ); }
	void setTranslation(const gsVector3& vec, bool tsLocal = true)          { BRICK_SET( setTranslation(vec, tsLocal) ); }

	void setRotation(float x, float y, float z, bool tsLocal = true)        { BRICK_SET( setRotation(gkVector3(x, y, z), tsLocal) ); }
	void setRotation(const gsVector3& vec, bool tsLocal = true)             { BRICK_SET( setRotation(vec, tsLocal) ); }

	void setForce(float x, float y, float z, bool tsLocal = true)           { BRICK_SET( setForce(gkVector3(x, y, z), tsLocal) ); }
	void setForce(const gsVector3& vec, bool tsLocal = true)                { BRICK_SET( setForce(vec, tsLocal) ); }

	void setTorque(float x, float y, float z, bool tsLocal = true)          { BRICK_SET( setTorque(gkVector3(x, y, z), tsLocal) ); }
	void setTorque(const gsVector3& vec, bool tsLocal = true)               { BRICK_SET( setTorque(vec, tsLocal) ); }

	void setLinearVelocity(float x, float y, float z, bool tsLocal = true)  { BRICK_SET( setLinearVelocity(gkVector3(x, y, z), tsLocal) ); }
	void setLinearVelocity(const gsVector3& vec, bool tsLocal = true)       { BRICK_SET( setLinearVelocity(vec, tsLocal) ); }

	void setAngularVelocity(float x, float y, float z, bool tsLocal = true) { BRICK_SET( setAngularVelocity(gkVector3(x, y, z), tsLocal) ); }
	void setAngularVelocity(const gsVector3& vec, bool tsLocal = true)      { BRICK_SET( setAngularVelocity(vec, tsLocal) ); }

	void  setIncrementalTargetVelocity(float v)   { BRICK_SET( setDamping(v) ); }
	void  setIncrementalVelocity(bool v)          { BRICK_SET( setIncrementalVelocity(v) ); }
	float getIncrementalTargetVelocity(void)      { BRICK_GET( getDamping() , 0.f ); }
	bool  getIncrementalVelocity(void)            { BRICK_GET( getIncrementalVelocity(), false ); }


	gsVector3 getTranslation(void)        {BRICK_GET(getTranslation(), gkVector3(0, 0, 0))}
	gsVector3 getRotation(void)           {BRICK_GET(getRotation(), gkVector3(0, 0, 0))}
	gsVector3 getForce(void)              {BRICK_GET(getForce(), gkVector3(0, 0, 0))}
	gsVector3 getTorque(void)             {BRICK_GET(getTorque(), gkVector3(0, 0, 0))}
	gsVector3 getLinearVelocity(void)     {BRICK_GET(getLinearVelocity(), gkVector3(0, 0, 0))}
	gsVector3 getAngularVelocity(void)    {BRICK_GET(getAngularVelocity(), gkVector3(0, 0, 0))}

	OGRE_KIT_LOGIC_BRICK(MotionActuator);
};


class gsParentActuator : public gsActuator
{
public:
	gsParentActuator(gsLogicObject* parent, const gkString& name = "");
	gsParentActuator();
	~gsParentActuator();

	void      setMode(int v)               {BRICK_SET( setMode(v) );}
	int       getMode(void)                {BRICK_GET( getMode(), PA_SET );}
	void      setParent(const gkString& v) {BRICK_SET( setParent(v) );}
	gkString  getParent(void)              {BRICK_GET( getParent(), "" );}
	void      setCompound(bool v)          {BRICK_SET( setCompound(v) );}
	bool      getCompound(void)            {BRICK_GET( getCompound(), false );}
	void      setGhost(bool v)             {BRICK_SET( setGhost(v) );}
	bool      getGhost(void)               {BRICK_GET( getGhost(), false );}


	OGRE_KIT_LOGIC_BRICK(ParentActuator);
};



class gsPropertyActuator : public gsActuator
{
public:
	gsPropertyActuator(gsLogicObject* parent, const gkString& name = "");
	gsPropertyActuator();
	~gsPropertyActuator();

	void      setType(int v)                  {BRICK_SET( setType(v) );}
	int       getType(void)                   {BRICK_GET( getType(), PA_ASSIGN );}
	void      setProperty(const gkString& v)  {BRICK_SET( setProperty(v) );}
	gkString  getProperty(void)               {BRICK_GET( getProperty(), "" );}
	void      setValue(const gkString& v)     {BRICK_SET( setValue(v) );}
	gkString  getValue(void)                  {BRICK_GET( getValue(), "" );}
	void      setObject(const gkString& v)    {BRICK_SET( setObject(v) );}
	gkString  getObject(void)                 {BRICK_GET( getObject(), "" );}


	OGRE_KIT_LOGIC_BRICK(PropertyActuator);
};


class gsRandomActuator : public gsActuator
{
public:
	gsRandomActuator(gsLogicObject* parent, const gkString& name = "");
	gsRandomActuator();
	~gsRandomActuator();

	void     setSeed(int v)                 {BRICK_SET( setSeed(v) );}
	int      getSeed(void)                  {BRICK_GET( getSeed(), 0 );}
	void     setDistribution(int v)         {BRICK_SET( setDistribution(v) );}
	int      getDistribution(void)          {BRICK_GET( getDistribution(), RA_BOOL_CONSTANT );}
	void     setProperty(const gkString& v) {BRICK_SET( setProperty(v) );}
	gkString getProperty(void)              {BRICK_GET( getProperty(), "" );}
	void     setMin(float v)                {BRICK_SET( setMin(v) );}
	float    getMin(void)                   {BRICK_GET( getMin(), 0 );}
	void     setMax(float v)                {BRICK_SET( setMax(v) );}
	float    getMax(void)                   {BRICK_GET( getMax(), 0 );}
	void     setConstant(float v)           {BRICK_SET( setConstant(v) );}
	float    getConstant(void)              {BRICK_GET( getConstant(), 0 );}
	void     setMean(float v)               {BRICK_SET( setMean(v) );}
	float    getMean(void)                  {BRICK_GET( getMean(), 0 );}
	void     setDeviation(float v)          {BRICK_SET( setDeviation(v) );}
	float    getDeviation(void)             {BRICK_GET( getDeviation(), 0 );}
	void     setHalfLife(float v)           {BRICK_SET( setHalfLife(v) );}
	float    getHalfLife(void)              {BRICK_GET( getHalfLife(), 0 );}


	OGRE_KIT_LOGIC_BRICK(RandomActuator);
};


class gsSceneActuator : public gsActuator
{
public:
	gsSceneActuator(gsLogicObject* parent, const gkString& name = "");
	gsSceneActuator();
	~gsSceneActuator();

	void     setMode(int v)                  {BRICK_SET( setMode(v) );}
	int      getMode(void)                   {BRICK_GET( getMode(), SC_RESTART );}
	void     setScene(const gkString& v)     {BRICK_SET( setScene(v) );}
	gkString getScene(void)                  {BRICK_GET( getScene(), "" );}
	void     setCamera(const gkString& v)    {BRICK_SET( setCamera(v) );}
	gkString getCamera(void)                 {BRICK_GET( getCamera(), "" );}


	OGRE_KIT_LOGIC_BRICK(SceneActuator);
};


class gsSoundActuator : public gsActuator
{
public:
	gsSoundActuator(gsLogicObject* parent, const gkString& name = "");
	gsSoundActuator();
	~gsSoundActuator();

	void     setType(int v)                      {BRICK_SET( setMode(v) );}
	int      getType(void)                       {BRICK_GET( getMode(), SA_PLAY_STOP );}
	void     setSoundFile(const gkString& v)     {BRICK_SET( setSoundFile(v) );}
	gkString getSoundFile(void)                  {BRICK_GET( getSoundFile(), "" );}


	OGRE_KIT_LOGIC_BRICK(SoundActuator);
};



class gsStateActuator : public gsActuator
{
public:
	gsStateActuator(gsLogicObject* parent, const gkString& name = "");
	gsStateActuator();
	~gsStateActuator();

	void     setMask(int v) {BRICK_SET( setMask(v) );}
	int      getMask(void)  {BRICK_GET( getMask(), 0 );}
	void     setOp(int v)   {BRICK_SET( setOp(v) );}
	int      getOp(void)    {BRICK_GET( getOp(), SOP_ADD );}

	OGRE_KIT_LOGIC_BRICK(StateActuator);
};



class gsVisibilityActuator : public gsActuator
{
public:
	gsVisibilityActuator(gsLogicObject* parent, const gkString& name = "");
	gsVisibilityActuator();
	~gsVisibilityActuator();


	void     setFlag(int v) {BRICK_SET( setFlag(v) );}
	int      getFlag(void)  {BRICK_GET( getFlag(), 0 );}

	OGRE_KIT_LOGIC_BRICK(VisibilityActuator);
};



#endif//_gsBricks_h_
