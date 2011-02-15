/*! 

<!-- ============================================ BrickMode ============================================ -->
\LuaClass{BrickMode}
 

The logic brick toggle state.

\code
OgreKit.BM_NONE,      No state response.
OgreKit.BM_ON,        Brick is switched on
OgreKit.BM_OFF,       Brick is switched off
\endcode

\endpage

<!-- ============================================ PulseMode ============================================ -->
\LuaClass{PulseMode}


The sensor pulse state.

\code
OgreKit.PM_NONE,      No state response.
OgreKit.PM_ON,        True activation.
OgreKit.PM_OFF,       False activation.
\endcode

\endpage


<!-- ============================================ ListenerMode ============================================ -->
\LuaClass{ListenerMode}



Callback mode for controlling how the custom event is handled.


\code
OgreKit.LM_EVT_OVERIDE,   Skip builtin query, you must provide your own.
OgreKit.LM_EVT_AND,       Peforms an AND operation, (yourCallback) AND (builtin test).
\endcode


\endpage




<!-- ============================================ MouseSensorType ============================================ -->
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


\endpage


<!-- ============================================ PropertySensorType ============================================ -->
\LuaClass{PropertySensorType}



Event type for property sensors.

\code
OgreKit.PST_EQUAL
OgreKit.PST_NEQUAL
OgreKit.PST_INTERVAL
OgreKit.PST_CHANGED
\endcode


\endpage




<!-- ============================================ RaySensorAxis ============================================ -->
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

\endpage





<!-- ============================================ LogicOp ============================================ -->
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

\endpage




<!-- ============================================ Controllers ============================================ -->
\LuaClass{Controllers}
\LuaClassExtend{Array}

This is an array template implementation where T is equal to \LuaClassRef{Controller}


\LuaGlobalRef{CArrayIter}

\endpage


<!-- ============================================ Sensors ============================================ -->
\LuaClass{Sensors}
\LuaClassExtend{Array}

This is an array template implementation where T is equal to \LuaClassRef{Sensor}


\LuaGlobalRef{CArrayIter}

\endpage


<!-- ============================================ Actuators ============================================ -->
\LuaClass{Actuators}
\LuaClassExtend{Array}

This is an array template implementation where T is equal to \LuaClassRef{Actuator}


\LuaGlobalRef{CArrayIter}

\endpage


<!-- ============================================ LogicList ============================================ -->
\LuaClass{LogicList}
\LuaClassExtend{Array}

This is an array template implementation where T is equal to \LuaClassRef{LogicObject}


\LuaGlobalRef{CArrayIter}

\endpage


<!-- ============================================ LogicManager ============================================ -->
\LuaClass{LogicManager}

\beginmenu{Methods}
\LuaMethodMenu{LogicManager,getObject}
\LuaMethodMenu{LogicManager,getObjectList}
\endmenu


\endpage

<!-- ======================================== -->
\LuaMethod{LogicManager,getObject}
\LuaClassUp{LogicManager}


Looks for a logic object by name.



\code
function LogicManager:getObject(name)
\endcode

\param name Find by the object's identifier. It's the same name as game object it's attached to.
\returns \LuaClassRef{LogicObject} or nil if the object is not found.

\endpage



<!-- ======================================== -->
\LuaMethod{LogicManager,getObjectList}
\LuaClassUp{LogicManager}

Returns a list of all logic objects.


\code
function LogicManager:getObjectList()
\endcode

\returns \LuaClassRef{LogicList}

\endpage


<!-- ============================================ LogicObject ============================================ -->
\LuaClass{LogicObject}

\beginmenu{Methods}
\LuaMethodMenu{LogicObject,getName}
\LuaMethodMenu{LogicObject,getSensor}
\LuaMethodMenu{LogicObject,getSensors}
\LuaMethodMenu{LogicObject,getController}
\LuaMethodMenu{LogicObject,getControllers}
\LuaMethodMenu{LogicObject,getActuator}
\LuaMethodMenu{LogicObject,getActuators}
\endmenu

\endpage


<!-- ======================================== -->
\LuaMethod{LogicObject,getName}
\LuaClassUp{LogicObject}

Returns the string identifier of this object.

\code
function LogicObject:getName()
\endcode

\returns string

\endpage


<!-- ======================================== -->
\LuaMethod{LogicObject,getSensor}
\LuaClassUp{LogicObject}

Finds a sensor attached to this object.

\code
function LogicObject:getSensor(name, crossSearch=false)
\endcode


\param name The name given to the sensor in the blender UI.
\param crossSearch Includes cross linked sensors while trying to match name.


\returns \LuaClassRef{Sensor}

\endpage


<!-- ======================================== -->
\LuaMethod{LogicObject,getSensors}
\LuaClassUp{LogicObject}

Return a list of sensors attached to this object.

\code
function LogicObject:getSensors(includeCross=false)
\endcode


\param includeCross Includes cross linked sensors.

\returns \LuaClassRef{Sensors}

\endpage




<!-- ======================================== -->
\LuaMethod{LogicObject,getController}
\LuaClassUp{LogicObject}

Finds a controller attached to this object.

\code
function LogicObject:getController(name, crossSearch=false)
\endcode


\param name The name given to the controller in the blender UI.
\param crossSearch Includes cross linked controllers while trying to match name.


\returns \LuaClassRef{Controller}

\endpage


<!-- ======================================== -->
\LuaMethod{LogicObject,getControllers}
\LuaClassUp{LogicObject}

Return a list of controllers attached to this object.

\code
function LogicObject:getControllers()
\endcode

\returns \LuaClassRef{Controllers}

\endpage




<!-- ======================================== -->
\LuaMethod{LogicObject,getActuator}
\LuaClassUp{LogicObject}

Finds an actuator attached to this object.

\code
function LogicObject:getActuator(name, crossSearch=false)
\endcode


\param name The name given to the actuator in the blender UI.
\param crossSearch Includes cross linked actuator while trying to match name.


\returns \LuaClassRef{Actuator}

\endpage


<!-- ======================================== -->
\LuaMethod{LogicObject,getActuators}
\LuaClassUp{LogicObject}

Return a list of actuators attached to this object.

\code
function LogicObject:getActuators()
\endcode

\returns \LuaClassRef{Actuators}

\endpage

<!-- ============================================ Brick ============================================ -->
\LuaClass{Brick}


\beginmenu{Methods}
\LuaMethodMenu{Brick,connect}
\LuaMethodMenu{Brick,getName}
\LuaMethodMenu{Brick,isOn}
\LuaMethodMenu{Brick,isOff}
\LuaMethodMenu{Brick,isActive}
\endmenu

\endpage



<!-- ======================================== -->
\LuaMethod{Brick,connect}


Connects an event listener to this logic brick for custom callbacks.


\code
function Brick:connect(execMode, table, function)
\endcode


\param execMode \LuaClassRef{ListenerMode} The mode in which this will execute.
\param table    Pointer to a Lua table object, ie; self
\param function Pointer to a Lua function, method of self.

\LuaGlobalRef{CustomLogicBricks}


\endpage


<!-- ======================================== -->
\LuaMethod{Brick,getName}


Returns the identifier for this brick.

\code
function Brick:getName()
\endcode

\returns string

\endpage




<!-- ======================================== -->
\LuaMethod{Brick,isOn}


Returns true if this brick has an \b ON pulse.

\code
function Brick:isOn()
\endcode

\returns \LuaClassRef{BrickMode}

\endpage


<!-- ======================================== -->
\LuaMethod{Brick,isOff}


Returns true if this brick has an \b OFF pulse.

\code
function Brick:isOff()
\endcode

\returns \LuaClassRef{BrickMode}

\endpage



<!-- ======================================== -->
\LuaMethod{Brick,isActive}


Returns true if this brick is activly running in the logic manager.

\code
function Brick:isActive()
\endcode

\returns bool

\endpage



<!-- ============================================ Sensor ============================================ -->
\LuaClass{Sensor}
\LuaClassExtend{Brick}


\beginmenu{Methods}
\LuaMethodMenu{Sensor,getFrequency}
\LuaMethodMenu{Sensor,getPulse}
\LuaMethodMenu{Sensor,isLevel}
\LuaMethodMenu{Sensor,isTap}
\LuaMethodMenu{Sensor,isInverse}
\LuaMethodMenu{Sensor,setFrequency}
\LuaMethodMenu{Sensor,setInvert}
\LuaMethodMenu{Sensor,setLevel}
\LuaMethodMenu{Sensor,setPulse}
\LuaMethodMenu{Sensor,setTap}
\endmenu


\endpage



<!-- ======================================== -->
\LuaMethod{Sensor,getFrequency}


Returns the frequency.


\code
function Sensor:getFrequency()
\endcode

\returns number

\endpage

<!-- ======================================== -->
\LuaMethod{Sensor,getPulse}

Returns the pulse mode.

\code
function Sensor:getPulse()
\endcode

\returns \LuaClassRef{PulseMode}

\endpage

<!-- ======================================== -->
\LuaMethod{Sensor,isLevel}

Returns true if this sensor is a state transition detector.

\code
function Sensor:isLevel()
\endcode

\returns bool

\endpage


<!-- ======================================== -->
\LuaMethod{Sensor,isTap}

Returns true if this sensor is in tap / toggle mode.

\code
function Sensor:isTap()
\endcode

\returns bool

\endpage


<!-- ======================================== -->
\LuaMethod{Sensor,isInverse}

Returns true if this sensor is inverting events.

\code
function Sensor:isInverse()
\endcode

\returns bool

\endpage


<!-- ======================================== -->
\LuaMethod{Sensor,setFrequency}

Sets the update frequency in game ticks (1 / 60).

\code
function Sensor:setFrequency(number)
\endcode

\param number The update rate in game ticks.

\endpage


<!-- ======================================== -->
\LuaMethod{Sensor,setInvert}

Makes this sensor invert event queries.

\code
function Sensor:setInvert(val)
\endcode

\param val bool value.

\endpage

<!-- ======================================== -->
\LuaMethod{Sensor,setLevel}

Makes this sensor execute on state changes.

\code
function Sensor:setLevel(val)
\endcode

\param val bool value.

\endpage



<!-- ======================================== -->
\LuaMethod{Sensor,setPulse}

Sets the pulse mode for this sensor.

\code
function Sensor:setPulse(pulse)
\endcode

\param pulse \LuaClassRef{PulseMode}

\endpage


<!-- ======================================== -->
\LuaMethod{Sensor,setTap}

Enable tap / toggle mode.


\code
function Sensor:setTap(val)
\endcode

\param val Boolean value.

\endpage


<!-- ============================================ ActuatorSensor ============================================ -->
\LuaClass{ActuatorSensor}
\LuaClassExtend{Sensor}


\beginmenu{Methods}
\LuaMethodMenu{ActuatorSensor,constructor}
\LuaMethodMenu{ActuatorSensor,setActuatorName}
\LuaMethodMenu{ActuatorSensor,getActuatorName}
\endmenu


\endpage


<!-- ======================================== -->
\LuaMethod{ActuatorSensor,constructor}
\LuaClassUp{ActuatorSensor}


Default constructor does nothing.

\code
function ActuatorSensor:constructor()
\endcode


<!-- ============================== -->
\sectionseperator{Overload:}

Creates a new sensor.

\code
function ActuatorSensor:constructor(object, name="")
\endcode

\param object The parent \LuaClassRef{LogicObject}
\param name   Unique identifier for this sensor. If this is empty one will be generated automatically

\returns \LuaClassRef{ActuatorSensor}

\endpage



<!-- ======================================== -->
\LuaMethod{ActuatorSensor,setActuatorName}
\LuaClassUp{ActuatorSensor}

Sets the name of the actuator this sensor responds to.

\code
function ActuatorSensor:setActuatorName(name)
\endcode

\param name string

\endpage




<!-- ======================================== -->
\LuaMethod{ActuatorSensor,getActuatorName}
\LuaClassUp{ActuatorSensor}

Gets the name of the actuator this sensor responds to.

\code
function ActuatorSensor:getActuatorName()
\endcode

\returns string

\endpage




<!-- ============================================ AlwaysSensor ============================================ -->
\LuaClass{AlwaysSensor}
\LuaClassExtend{Sensor}


\beginmenu{Methods}
\LuaMethodMenu{AlwaysSensor,constructor}
\endmenu

\endpage



<!-- ======================================== -->

\LuaMethod{AlwaysSensor,constructor}
\LuaClassUp{AlwaysSensor}


Default constructor does nothing.

\code
function AlwaysSensor:constructor()
\endcode


<!-- ============================== -->
\sectionseperator{Overload:}

Creates a new sensor.

\code
function AlwaysSensor:constructor(object, name="")
\endcode

\param object The parent \LuaClassRef{LogicObject}
\param name   Unique identifier for this sensor. If this is empty one will be generated automatically

\returns \LuaClassRef{AlwaysSensor}

\endpage




<!-- ============================================ CollisionSensor ============================================ -->
\LuaClass{CollisionSensor}
\LuaClassExtend{Sensor}



\beginmenu{Methods}
\LuaMethodMenu{CollisionSensor,constructor}
\LuaMethodMenu{CollisionSensor,setMaterialName}
\LuaMethodMenu{CollisionSensor,getMaterialName}
\LuaMethodMenu{CollisionSensor,setPropertyName}
\LuaMethodMenu{CollisionSensor,getPropertyName}
\endmenu

\endpage



<!-- ======================================== -->

\LuaMethod{CollisionSensor,constructor}



Default constructor does nothing.

\code
function CollisionSensor:constructor()
\endcode


<!-- ============================== -->
\sectionseperator{Overload:}

Creates a new sensor.

\code
function CollisionSensor:constructor(object, name="")
\endcode

\param object The parent \LuaClassRef{LogicObject}
\param name   Unique identifier for this sensor. If this is empty one will be generated automatically

\returns \LuaClassRef{CollisionSensor}

\endpage





<!-- ======================================== -->
\LuaMethod{CollisionSensor,setMaterialName}


Sets the name of the material this sensor responds to.

\code
function CollisionSensor:setMaterialName(name)
\endcode

\param name string

\endpage




<!-- ======================================== -->
\LuaMethod{CollisionSensor,getMaterialName}


Gets the name of the material this sensor responds to.

\code
function CollisionSensor:getMaterialName()
\endcode

\returns string

\endpage




<!-- ======================================== -->
\LuaMethod{CollisionSensor,setPropertyName}


Sets the name of the property this sensor responds to.

\code
function CollisionSensor:setPropertyName(name)
\endcode

\param name string

\endpage




<!-- ======================================== -->
\LuaMethod{CollisionSensor,getPropertyName}


Gets the name of the property this sensor responds to.

\code
function CollisionSensor:getPropertyName()
\endcode

\returns string

\endpage





<!-- ============================================ DelaySensor ============================================ -->
\LuaClass{DelaySensor}
\LuaClassExtend{Sensor}



\beginmenu{Methods}
\LuaMethodMenu{DelaySensor,constructor}
\LuaMethodMenu{DelaySensor,setDelay}
\LuaMethodMenu{DelaySensor,getDelay}
\LuaMethodMenu{DelaySensor,setDuration}
\LuaMethodMenu{DelaySensor,getDuration}
\LuaMethodMenu{DelaySensor,setRepeat}
\LuaMethodMenu{DelaySensor,getRepeat}
\endmenu

\endpage



<!-- ======================================== -->

\LuaMethod{DelaySensor,constructor}
\LuaClassUp{DelaySensor}


Default constructor does nothing.

\code
function DelaySensor:constructor()
\endcode


<!-- ============================== -->
\sectionseperator{Overload:}

Creates a new sensor.

\code
function DelaySensor:constructor(object, name="")
\endcode

\param object The parent \LuaClassRef{LogicObject}
\param name   Unique identifier for this sensor. If this is empty one will be generated automatically

\returns \LuaClassRef{DelaySensor}

\endpage

<!-- ======================================== -->
\LuaMethod{DelaySensor,setDelay}
\LuaClassUp{DelaySensor}

Sets the delay in game ticks.

\code
function DelaySensor:setDelay(delay)
\endcode


\param delay number 

\endpage

<!-- ======================================== -->
\LuaMethod{DelaySensor,getDelay}
\LuaClassUp{DelaySensor}

Gets the delay in game ticks.

\code
function DelaySensor:getDelay()
\endcode


\returns number 

\endpage


<!-- ======================================== -->
\LuaMethod{DelaySensor,getDuration}
\LuaClassUp{DelaySensor}

Gets the execution time.

\code
function DelaySensor:getDuration()
\endcode


\returns number 

\endpage



<!-- ======================================== -->
\LuaMethod{DelaySensor,setDuration}
\LuaClassUp{DelaySensor}

Sets the execution time.

\code
function DelaySensor:setDuration(val)
\endcode


\param val number 

\endpage






<!-- ======================================== -->
\LuaMethod{DelaySensor,setRepeat}
\LuaClassUp{DelaySensor}


Forces a delay -> execute(span) loop


\code
function DelaySensor:setRepeat(val)
\endcode

\param val bool 

\endpage



<!-- ======================================== -->
\LuaMethod{DelaySensor,getRepeat}
\LuaClassUp{DelaySensor}

Returns true if the sensor is in loop mode.

\code
function DelaySensor:getRepeat()
\endcode

\returns bool 

\endpage






<!-- ============================================ KeyboardSensor ============================================ -->
\LuaClass{KeyboardSensor}
\LuaClassExtend{Sensor}



\beginmenu{Methods}
\LuaMethodMenu{KeyboardSensor,constructor}
\LuaMethodMenu{KeyboardSensor,setKey}
\LuaMethodMenu{KeyboardSensor,getKey}
\LuaMethodMenu{KeyboardSensor,setModifier0}
\LuaMethodMenu{KeyboardSensor,getModifier0}
\LuaMethodMenu{KeyboardSensor,setModifier1}
\LuaMethodMenu{KeyboardSensor,getModifier1}
\LuaMethodMenu{KeyboardSensor,setAllKeys}
\LuaMethodMenu{KeyboardSensor,getAllKeys}
\endmenu

\endpage


<!-- ======================================== -->

\LuaMethod{KeyboardSensor,constructor}
\LuaClassUp{KeyboardSensor}


Default constructor does nothing.

\code
function KeyboardSensor:constructor()
\endcode


<!-- ============================== -->
\sectionseperator{Overload:}

Creates a new sensor.

\code
function KeyboardSensor:constructor(object, name="")
\endcode

\param object The parent \LuaClassRef{LogicObject}
\param name   Unique identifier for this sensor. If this is empty one will be generated automatically

\returns \LuaClassRef{KeyboardSensor}

\endpage




<!-- ======================================== -->
\LuaMethod{KeyboardSensor,setKey}
\LuaClassUp{KeyboardSensor}

Sets the keyboard scan code.

\code
function KeyboardSensor:setKey(code)
\endcode

\param code \LuaClassRef{ScanCode}

\endpage


<!-- ======================================== -->
\LuaMethod{KeyboardSensor,getKey}
\LuaClassUp{KeyboardSensor}

Gets the keyboard scan code.

\code
function KeyboardSensor:getKey()
\endcode

\returns \LuaClassRef{ScanCode}

\endpage


<!-- ======================================== -->
\LuaMethod{KeyboardSensor,setModifier0}
\LuaClassUp{KeyboardSensor}

Sets the first modifier scan code.

\code
function KeyboardSensor:setModifier0(code)
\endcode

\param code \LuaClassRef{ScanCode}

\endpage


<!-- ======================================== -->
\LuaMethod{KeyboardSensor,getModifier0}
\LuaClassUp{KeyboardSensor}

Gets the first modifier scan code.

\code
function KeyboardSensor:getModifier0()
\endcode

\returns \LuaClassRef{ScanCode}

\endpage


<!-- ======================================== -->
\LuaMethod{KeyboardSensor,setModifier1}
\LuaClassUp{KeyboardSensor}

Sets the second modifier scan code.

\code
function KeyboardSensor:setModifier1(code)
\endcode

\param code \LuaClassRef{ScanCode}

\endpage


<!-- ======================================== -->
\LuaMethod{KeyboardSensor,getModifier1}
\LuaClassUp{KeyboardSensor}

Gets the second modifier scan code.

\code
function KeyboardSensor:getModifier1()
\endcode

\returns \LuaClassRef{ScanCode}

\endpage



<!-- ======================================== -->
\LuaMethod{KeyboardSensor,setAllKeys}
\LuaClassUp{KeyboardSensor}

Sets the sensor to react on any key press

\code
function KeyboardSensor:setAllKeys(val)
\endcode

\param val bool

\endpage


<!-- ======================================== -->
\LuaMethod{KeyboardSensor,getAllKeys}
\LuaClassUp{KeyboardSensor}

Returns true if this sensor reacts to any key press.

\code
function KeyboardSensor:getAllKeys()
\endcode

\returns bool

\endpage




<!-- ============================================ MessageSensor ============================================ -->
\LuaClass{MessageSensor}
\LuaClassExtend{Sensor}


\beginmenu{Methods}
\LuaMethodMenu{MessageSensor,constructor}
\LuaMethodMenu{MessageSensor,setSubject}
\LuaMethodMenu{MessageSensor,getSubject}
\endmenu

\endpage


<!-- ======================================== -->

\LuaMethod{MessageSensor,constructor}
\LuaClassUp{MessageSensor}


Default constructor does nothing.

\code
function MessageSensor:constructor()
\endcode


<!-- ============================== -->
\sectionseperator{Overload:}

Creates a new sensor.

\code
function MessageSensor:constructor(object, name="")
\endcode

\param object The parent \LuaClassRef{LogicObject}
\param name   Unique identifier for this sensor. If this is empty one will be generated automatically

\returns \LuaClassRef{MessageSensor}

\endpage



<!-- ======================================== -->
\LuaMethod{MessageSensor,setSubject}
\LuaClassUp{MessageSensor}

Sets the subject to listen for, empty for all messages.

\code
function MessageSensor:setSubject(val)
\endcode

\param val string 

\endpage


<!-- ======================================== -->
\LuaMethod{MessageSensor,getSubject}
\LuaClassUp{MessageSensor}

Gets the subject to listen for.

\code
function MessageSensor:getSubject()
\endcode

\returns string

\endpage






<!-- ============================================ MouseSensor ============================================ -->
\LuaClass{MouseSensor}
\LuaClassExtend{Sensor}


\beginmenu{Methods}
\LuaMethodMenu{MouseSensor,constructor}
\LuaMethodMenu{MouseSensor,setType}
\LuaMethodMenu{MouseSensor,getType}
\endmenu

\endpage


<!-- ======================================== -->

\LuaMethod{MouseSensor,constructor}


Default constructor does nothing.

\code
function MouseSensor:constructor()
\endcode


<!-- ============================== -->
\sectionseperator{Overload:}

Creates a new sensor.

\code
function MouseSensor:constructor(object, name="")
\endcode

\param object The parent \LuaClassRef{LogicObject}
\param name   Unique identifier for this sensor. If this is empty one will be generated automatically

\returns \LuaClassRef{MouseSensor}

\endpage




<!-- ======================================== -->
\LuaMethod{MouseSensor,setType}


Sets the type of event to listen for.

\code
function MouseSensor:setType(val)
\endcode

\param val \LuaClassRef{MouseSensorType}

\endpage


<!-- ======================================== -->
\LuaMethod{MouseSensor,getType}


Gets the type of event currently listening for.

\code
function MouseSensor:getType()
\endcode

\returns \LuaClassRef{MouseSensorType}

\endpage







<!-- ============================================ NearSensor ============================================ -->
\LuaClass{NearSensor}
\LuaClassExtend{Sensor}


\beginmenu{Methods}
\LuaMethodMenu{NearSensor,constructor}
\LuaMethodMenu{NearSensor,setRange}
\LuaMethodMenu{NearSensor,getRange}
\LuaMethodMenu{NearSensor,setResetRange}
\LuaMethodMenu{NearSensor,getResetRange}
\LuaMethodMenu{NearSensor,setMaterial}
\LuaMethodMenu{NearSensor,getMaterial}
\LuaMethodMenu{NearSensor,setProperty}
\LuaMethodMenu{NearSensor,getProperty}
\endmenu

\endpage


<!-- ======================================== -->

\LuaMethod{NearSensor,constructor}



Default constructor does nothing.

\code
function NearSensor:constructor()
\endcode


<!-- ============================== -->
\sectionseperator{Overload:}

Creates a new sensor.

\code
function NearSensor:constructor(object, name="")
\endcode

\param object The parent \LuaClassRef{LogicObject}
\param name   Unique identifier for this sensor. If this is empty one will be generated automatically

\returns \LuaClassRef{NearSensor}

\endpage




<!-- ======================================== -->
\LuaMethod{NearSensor,setRange}


Sets the trigger distance

\code
function NearSensor:setRange(val)
\endcode

\param val number

\endpage


<!-- ======================================== -->
\LuaMethod{NearSensor,getRange}


Gets the trigger distance

\code
function NearSensor:getRange()
\endcode

\returns number

\endpage




<!-- ======================================== -->
\LuaMethod{NearSensor,setResetRange}


Sets the trigger reset distance

\code
function NearSensor:setResetRange(val)
\endcode

\param val number

\endpage


<!-- ======================================== -->
\LuaMethod{NearSensor,getResetRange}


Gets the trigger reset distance

\code
function NearSensor:getResetRange()
\endcode

\returns number

\endpage





<!-- ======================================== -->
\LuaMethod{NearSensor,setMaterial}


Sets the material name to look for. Blank for any.

\code
function NearSensor:setMaterial(val)
\endcode

\param val string

\endpage


<!-- ======================================== -->
\LuaMethod{NearSensor,getMaterial}


Gets the material name to look for.

\code
function NearSensor:getMaterial()
\endcode

\returns string

\endpage


<!-- ======================================== -->
\LuaMethod{NearSensor,setProperty}


Sets the property name to look for. Blank for any.

\code
function NearSensor:setProperty(val)
\endcode

\param val string

\endpage


<!-- ======================================== -->
\LuaMethod{NearSensor,getProperty}


Gets the property name to look for.

\code
function NearSensor:getProperty()
\endcode

\returns string

\endpage






<!-- ============================================ PropertySensor ============================================ -->
\LuaClass{PropertySensor}
\LuaClassExtend{Sensor}


\beginmenu{Methods}
\LuaMethodMenu{PropertySensor,constructor}
\LuaMethodMenu{PropertySensor,setType}
\LuaMethodMenu{PropertySensor,getType}
\LuaMethodMenu{PropertySensor,setPropertyName}
\LuaMethodMenu{PropertySensor,getPropertyName}
\LuaMethodMenu{PropertySensor,setValue}
\LuaMethodMenu{PropertySensor,getValue}
\endmenu

\endpage


<!-- ======================================== -->

\LuaMethod{PropertySensor,constructor}



Default constructor does nothing.

\code
function PropertySensor:constructor()
\endcode


<!-- ============================== -->
\sectionseperator{Overload:}

Creates a new sensor.

\code
function PropertySensor:constructor(object, name="")
\endcode

\param object The parent \LuaClassRef{LogicObject}
\param name   Unique identifier for this sensor. If this is empty one will be generated automatically

\returns \LuaClassRef{PropertySensor}

\endpage


<!-- ======================================== -->
\LuaMethod{PropertySensor,setType}


Sets the type of reaction this sensor has.

\code
function PropertySensor:setType(val)
\endcode

\param val \LuaClassRef{PropertySensorType}

\endpage


<!-- ======================================== -->
\LuaMethod{PropertySensor,getType}


Gets the type of reaction this sensor has.

\code
function PropertySensor:getType()
\endcode

\returns  \LuaClassRef{PropertySensorType}

\endpage



<!-- ======================================== -->
\LuaMethod{PropertySensor,setPropertyName}


Sets the property name to look for.

\code
function PropertySensor:setPropertyName(val)
\endcode

\param val string

\endpage


<!-- ======================================== -->
\LuaMethod{PropertySensor,getPropertyName}


Gets the property name to look for.

\code
function PropertySensor:getPropertyName()
\endcode

\returns string

\endpage



<!-- ======================================== -->
\LuaMethod{PropertySensor,setValue}


Sets the value for. PST_EQUAL and PST_NEQUAL operations.


\code
function PropertySensor:setValue(val)
\endcode

\param val string

\endpage


<!-- ======================================== -->
\LuaMethod{PropertySensor,getValue}


Gets the value for. PST_EQUAL and PST_NEQUAL operations.

\code
function PropertySensor:getValue()
\endcode

\returns string

\endpage







<!-- ============================================ RaySensor ============================================ -->
\LuaClass{RaySensor}
\LuaClassExtend{Sensor}


\beginmenu{Methods}
\LuaMethodMenu{RaySensor,constructor}
\LuaMethodMenu{RaySensor,setRange}
\LuaMethodMenu{RaySensor,getRange}
\LuaMethodMenu{RaySensor,setAxis}
\LuaMethodMenu{RaySensor,getAxis}
\LuaMethodMenu{RaySensor,setMaterial}
\LuaMethodMenu{RaySensor,getMaterial}
\LuaMethodMenu{RaySensor,setProperty}
\LuaMethodMenu{RaySensor,getProperty}
\endmenu

\endpage



<!-- ======================================== -->

\LuaMethod{RaySensor,constructor}



Default constructor does nothing.

\code
function RaySensor:constructor()
\endcode


<!-- ============================== -->
\sectionseperator{Overload:}

Creates a new sensor.

\code
function RaySensor:constructor(object, name="")
\endcode

\param object The parent \LuaClassRef{LogicObject}
\param name   Unique identifier for this sensor. If this is empty one will be generated automatically

\returns \LuaClassRef{RaySensor}

\endpage


<!-- ======================================== -->
\LuaMethod{RaySensor,setRange}


Sets the distance of the ray.

\code
function RaySensor:setRange(val)
\endcode

\param val number

\endpage


<!-- ======================================== -->
\LuaMethod{RaySensor,getRange}


Gets the distance of the ray.

\code
function RaySensor:getRange()
\endcode

\returns number

\endpage




<!-- ======================================== -->
\LuaMethod{RaySensor,setAxis}


Sets the axis of the cast.

\code
function RaySensor:setAxis(val)
\endcode

\param val \LuaClassRef{RaySensorAxis}

\endpage


<!-- ======================================== -->
\LuaMethod{RaySensor,getAxis}


Gets the axis of the cast.

\code
function RaySensor:getAxis()
\endcode

\returns \LuaClassRef{RaySensorAxis}

\endpage


<!-- ======================================== -->
\LuaMethod{RaySensor,setMaterial}


Sets the material name to look for. Blank for any.

\code
function RaySensor:setMaterial(val)
\endcode

\param val string

\endpage


<!-- ======================================== -->
\LuaMethod{RaySensor,getMaterial}


Gets the material name to look for.

\code
function RaySensor:getMaterial()
\endcode

\returns string

\endpage


<!-- ======================================== -->
\LuaMethod{RaySensor,setProperty}


Sets the property name to look for. Blank for any.

\code
function RaySensor:setProperty(val)
\endcode

\param val string

\endpage


<!-- ======================================== -->
\LuaMethod{RaySensor,getProperty}


Gets the property name to look for.

\code
function RaySensor:getProperty()
\endcode

\returns string

\endpage






<!-- ============================================ RadarSensor ============================================ -->
\LuaClass{RadarSensor}
\LuaClassExtend{Sensor}


\beginmenu{Methods}
\LuaMethodMenu{RadarSensor,constructor}
\LuaMethodMenu{RadarSensor,setAngle}
\LuaMethodMenu{RadarSensor,getAngle}
\LuaMethodMenu{RadarSensor,setRange}
\LuaMethodMenu{RadarSensor,getRange}
\LuaMethodMenu{RadarSensor,setAxis}
\LuaMethodMenu{RadarSensor,getAxis}
\LuaMethodMenu{RadarSensor,setMaterial}
\LuaMethodMenu{RadarSensor,getMaterial}
\LuaMethodMenu{RadarSensor,setProperty}
\LuaMethodMenu{RadarSensor,getProperty}
\endmenu

\endpage



<!-- ======================================== -->

\LuaMethod{RadarSensor,constructor}



Default constructor does nothing.

\code
function RadarSensor:constructor()
\endcode


<!-- ============================== -->
\sectionseperator{Overload:}

Creates a new sensor.

\code
function RadarSensor:constructor(object, name="")
\endcode

\param object The parent \LuaClassRef{LogicObject}
\param name   Unique identifier for this sensor. If this is empty one will be generated automatically

\returns \LuaClassRef{RadarSensor}

\endpage


<!-- ======================================== -->
\LuaMethod{RadarSensor,setAngle}


Sets the opening angle of the cone.

\code
function RadarSensor:setAngle(val)
\endcode

\param val number

\endpage


<!-- ======================================== -->
\LuaMethod{RadarSensor,getAngle}


Gets the opening angle of the cone.

\code
function RadarSensor:getAngle()
\endcode

\returns number

\endpage

<!-- ======================================== -->
\LuaMethod{RadarSensor,setRange}


Sets the depth of the cone.

\code
function RadarSensor:setRange(val)
\endcode

\param val number

\endpage


<!-- ======================================== -->
\LuaMethod{RadarSensor,getRange}


Gets the depth of the cone.

\code
function RadarSensor:getRange()
\endcode

\returns number

\endpage




<!-- ======================================== -->
\LuaMethod{RadarSensor,setAxis}


Sets the axis of the cast.

\code
function RadarSensor:setAxis(val)
\endcode

\param val \LuaClassRef{RaySensorAxis}

\endpage


<!-- ======================================== -->
\LuaMethod{RadarSensor,getAxis}


Gets the axis of the cast.

\code
function RadarSensor:getAxis()
\endcode

\returns \LuaClassRef{RaySensorAxis}

\endpage


<!-- ======================================== -->
\LuaMethod{RadarSensor,setMaterial}


Sets the material name to look for. Blank for any.

\code
function RadarSensor:setMaterial(val)
\endcode

\param val string

\endpage


<!-- ======================================== -->
\LuaMethod{RadarSensor,getMaterial}


Gets the material name to look for.

\code
function RadarSensor:getMaterial()
\endcode

\returns string

\endpage


<!-- ======================================== -->
\LuaMethod{RadarSensor,setProperty}


Sets the property name to look for. Blank for any.

\code
function RadarSensor:setProperty(val)
\endcode

\param val string

\endpage


<!-- ======================================== -->
\LuaMethod{RadarSensor,getProperty}


Gets the property name to look for.

\code
function RadarSensor:getProperty()
\endcode

\returns string

\endpage






<!-- ============================================ RandomSensor ============================================ -->
\LuaClass{RandomSensor}
\LuaClassExtend{Sensor}


\beginmenu{Methods}
\LuaMethodMenu{RandomSensor,constructor}
\LuaMethodMenu{RandomSensor,setSeed}
\LuaMethodMenu{RandomSensor,getSeed}
\endmenu

\endpage



<!-- ======================================== -->

\LuaMethod{RandomSensor,constructor}



Default constructor does nothing.

\code
function RandomSensor:constructor()
\endcode


<!-- ============================== -->
\sectionseperator{Overload:}

Creates a new sensor.

\code
function RandomSensor:constructor(object, name="")
\endcode

\param object The parent \LuaClassRef{LogicObject}
\param name   Unique identifier for this sensor. If this is empty one will be generated automatically

\returns \LuaClassRef{RandomSensor}

\endpage


<!-- ======================================== -->
\LuaMethod{RandomSensor,setSeed}


Sets the inital seed of the random generator.

\code
function RandomSensor:setSeed(val)
\endcode

\param val number

\endpage


<!-- ======================================== -->
\LuaMethod{RandomSensor,getSeed}


Gets the inital seed of the random generator.

\code
function RandomSensor:getSeed()
\endcode

\returns number

\endpage









<!-- ============================================ TouchSensor ============================================ -->
\LuaClass{TouchSensor}
\LuaClassExtend{Sensor}



\beginmenu{Methods}
\LuaMethodMenu{TouchSensor,constructor}
\LuaMethodMenu{TouchSensor,setMaterialName}
\LuaMethodMenu{TouchSensor,getMaterialName}
\LuaMethodMenu{TouchSensor,setPropertyName}
\LuaMethodMenu{TouchSensor,getPropertyName}
\endmenu

\endpage



<!-- ======================================== -->

\LuaMethod{TouchSensor,constructor}



Default constructor does nothing.

\code
function TouchSensor:constructor()
\endcode


<!-- ============================== -->
\sectionseperator{Overload:}

Creates a new sensor.

\code
function TouchSensor:constructor(object, name="")
\endcode

\param object The parent \LuaClassRef{LogicObject}
\param name   Unique identifier for this sensor. If this is empty one will be generated automatically

\returns \LuaClassRef{TouchSensor}

\endpage





<!-- ======================================== -->
\LuaMethod{TouchSensor,setMaterialName}


Sets the name of the material this sensor responds to.

\code
function TouchSensor:setMaterialName(name)
\endcode

\param name string

\endpage




<!-- ======================================== -->
\LuaMethod{TouchSensor,getMaterialName}


Gets the name of the material this sensor responds to.

\code
function TouchSensor:getMaterialName()
\endcode

\returns string

\endpage




<!-- ======================================== -->
\LuaMethod{TouchSensor,setPropertyName}


Sets the name of the property this sensor responds to.

\code
function TouchSensor:setPropertyName(name)
\endcode

\param name string

\endpage




<!-- ======================================== -->
\LuaMethod{TouchSensor,getPropertyName}


Gets the name of the property this sensor responds to.

\code
function TouchSensor:getPropertyName()
\endcode

\returns string

\endpage




<!-- ============================================ Controller ============================================ -->
\LuaClass{Controller}
\LuaClassExtend{Brick}



\beginmenu{Methods}
\LuaMethodMenu{Controller,link}
\endmenu



\endpage




<!-- ======================================== -->
\LuaMethod{Controller,link}



This is a two stage link. \n

First the sensors output is linked to the controllers input.\n
Second the controllers input is linked to the sensors output. \n

\code
function Controller:link(sensor)
\endcode


<!-- ============================== -->
\sectionseperator{Overload:}

This is a one stage link. \n
The actuators input is linked to the controllers output.

\code
function Controller:link(actuator)
\endcode


\endpage



<!-- ============================================ LogicOpController ============================================ -->
\LuaClass{LogicOpController}
\LuaClassExtend{Controller}



\beginmenu{Methods}
\LuaMethodMenu{LogicOpController,constructor}
\LuaMethodMenu{LogicOpController,setOp}
\LuaMethodMenu{LogicOpController,getOp}
\endmenu

\endpage



<!-- ======================================== -->

\LuaMethod{LogicOpController,constructor}



Default constructor does nothing.

\code
function LogicOpController:constructor()
\endcode


<!-- ============================== -->
\sectionseperator{Overload:}

Creates a new controller.

\code
function LogicOpController:constructor(object, name="")
\endcode

\param object The parent \LuaClassRef{LogicObject}
\param name   Unique identifier for this controller. If this is empty one will be generated automatically

\returns \LuaClassRef{LogicOpController}

\endpage





<!-- ======================================== -->
\LuaMethod{LogicOpController,setOp}


Sets the operation method.

\code
function LogicOpController:setOp(op)
\endcode

\param op \LuaClassRef{LogicOp}

\endpage




<!-- ======================================== -->
\LuaMethod{LogicOpController,getOp}


Gets the operation method.

\code
function LogicOpController:getOp()
\endcode

\returns \LuaClassRef{LogicOp}

\endpage






<!-- ============================================ ScriptController ============================================ -->
\LuaClass{ScriptController}
\LuaClassExtend{Controller}



\beginmenu{Methods}
\LuaMethodMenu{ScriptController,constructor}
\LuaMethodMenu{ScriptController,setScript}
\endmenu

\endpage



<!-- ======================================== -->

\LuaMethod{ScriptController,constructor}



Default constructor does nothing.

\code
function ScriptController:constructor()
\endcode


<!-- ============================== -->
\sectionseperator{Overload:}

Creates a new controller.

\code
function ScriptController:constructor(object, name="")
\endcode

\param object The parent \LuaClassRef{LogicObject}
\param name   Unique identifier for this controller. If this is empty one will be generated automatically

\returns \LuaClassRef{ScriptController}

\endpage





<!-- ======================================== -->
\LuaMethod{ScriptController,setScript}


Sets the name of the script to call. This will be an internal Blender Text data block name.


\code
function ScriptController:setScript(name)
\endcode

\param name Text data block name.

\endpage



<!-- ============================================ ExpressionController ============================================ -->
\LuaClass{ExpressionController}
\LuaClassExtend{Controller}



\beginmenu{Methods}
\LuaMethodMenu{ExpressionController,constructor}
\LuaMethodMenu{ExpressionController,setExpression}
\endmenu

\endpage



<!-- ======================================== -->

\LuaMethod{ExpressionController,constructor}



Default constructor does nothing.

\code
function ExpressionController:constructor()
\endcode


<!-- ============================== -->
\sectionseperator{Overload:}

Creates a new controller.

\code
function ExpressionController:constructor(object, name="")
\endcode

\param object The parent \LuaClassRef{LogicObject}
\param name   Unique identifier for this controller. If this is empty one will be generated automatically

\returns \LuaClassRef{ExpressionController}

\endpage





<!-- ======================================== -->
\LuaMethod{ExpressionController,setExpression}


Sets the expression to call. This will be an Lua expression.


\code
function ScriptController:setExpression(expr)
\endcode

\param expr Lua expression.

\endpage


<!-- ============================================ Actuator ============================================ -->
\LuaClass{Actuator}
\LuaClassExtend{Brick}

\endpage


<!-- ============================================ ActionActuator ============================================ -->
\LuaClass{ActionActuator}
\LuaClassExtend{Actuator}



\endpage

<!-- ============================================ EditObjectActuator ============================================ -->
\LuaClass{EditObjectActuator}
\LuaClassExtend{Actuator}


\endpage


<!-- ============================================ GameActuator ============================================ -->
\LuaClass{GameActuator}
\LuaClassExtend{Actuator}


\endpage


<!-- ============================================ MessageActuator ============================================ -->
\LuaClass{MessageActuator}
\LuaClassExtend{Actuator}


\endpage

<!-- ============================================ MotionActuator ============================================ -->
\LuaClass{MotionActuator}
\LuaClassExtend{Actuator}


\endpage

<!-- ============================================ ParentActuator ============================================ -->
\LuaClass{ParentActuator}
\LuaClassExtend{Actuator}


\endpage

<!-- ============================================ PropertyActuator ============================================ -->
\LuaClass{PropertyActuator}
\LuaClassExtend{Actuator}


\endpage

<!-- ============================================ RandomActuator ============================================ -->
\LuaClass{RandomActuator}
\LuaClassExtend{Actuator}


\endpage

<!-- ============================================ SceneActuator ============================================ -->
\LuaClass{SceneActuator}
\LuaClassExtend{Actuator}


\endpage

<!-- ============================================ SoundActuator ============================================ -->
\LuaClass{SoundActuator}
\LuaClassExtend{Actuator}


\endpage

<!-- ============================================ StateActuator ============================================ -->
\LuaClass{StateActuator}
\LuaClassExtend{Actuator}


\endpage

<!-- ============================================ VisibilityActuator ============================================ -->
\LuaClass{VisibilityActuator}
\LuaClassExtend{Actuator}

\endpage


*/
