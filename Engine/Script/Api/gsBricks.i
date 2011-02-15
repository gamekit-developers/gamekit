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
%{
#include "gsBricks.h"
%}
%include "gsMacros.i"


%ignore OGRE_KIT_LOGIC_BRICK;
%ignore OGRE_KIT_LOGIC_BRICK_BASE;


%newobject getCurrentController;
%newobject gsLogicManager::getObject;
%newobject gsLogicManager::newObject;



GS_SCRIPT_NAME(LogicManager)
GS_SCRIPT_NAME(LogicObject)
GS_SCRIPT_NAME(Brick)
GS_SCRIPT_NAME(Sensor)
GS_SCRIPT_NAME(ActuatorSensor)
GS_SCRIPT_NAME(AlwaysSensor)
GS_SCRIPT_NAME(CollisionSensor)
GS_SCRIPT_NAME(DelaySensor)
GS_SCRIPT_NAME(KeyboardSensor)
GS_SCRIPT_NAME(MessageSensor)
GS_SCRIPT_NAME(MouseSensor)
GS_SCRIPT_NAME(NearSensor)
GS_SCRIPT_NAME(PropertySensor)
GS_SCRIPT_NAME(RaySensor)
GS_SCRIPT_NAME(RadarSensor)
GS_SCRIPT_NAME(RandomSensor)
GS_SCRIPT_NAME(Controller)
GS_SCRIPT_NAME(ExpressionController)
GS_SCRIPT_NAME(ScriptController)
GS_SCRIPT_NAME(LogicOpController)
GS_SCRIPT_NAME(Actuator)
GS_SCRIPT_NAME(ActionActuator)
GS_SCRIPT_NAME(EditObjectActuator)
GS_SCRIPT_NAME(GameActuator)
GS_SCRIPT_NAME(MessageActuator)
GS_SCRIPT_NAME(MotionActuator)
GS_SCRIPT_NAME(ParentActuator)
GS_SCRIPT_NAME(PropertyActuator)
GS_SCRIPT_NAME(RandomActuator)
GS_SCRIPT_NAME(SceneActuator)
GS_SCRIPT_NAME(SoundActuator)
GS_SCRIPT_NAME(StateActuator)
GS_SCRIPT_NAME(VisibilityActuator)
GS_SCRIPT_NAME(BrickMode)
GS_SCRIPT_NAME(PulseMode)
GS_SCRIPT_NAME(MouseSensorType)
GS_SCRIPT_NAME(ListenerMode)
GS_SCRIPT_NAME(PropertySensorType)
GS_SCRIPT_NAME(RaySensorAxis)
GS_SCRIPT_NAME(LogicOp)
GS_SCRIPT_NAME(ActionActuatorType)
GS_SCRIPT_NAME(GameActuatorType)
GS_SCRIPT_NAME(EditObjectActuatorType)
GS_SCRIPT_NAME(EditObjectActuatorDynamics)
GS_SCRIPT_NAME(MessageActuatorBodyType)
GS_SCRIPT_NAME(ParentActuatorMode)
GS_SCRIPT_NAME(PropertyActuatorType)
GS_SCRIPT_NAME(RandomActuatorDistribution)
GS_SCRIPT_NAME(SoundActuatorType)
GS_SCRIPT_NAME(SceneActuatorMode)
GS_SCRIPT_NAME(StateActuatorType)
GS_SCRIPT_NAME(VisibilityActuatorType)



GS_ENUM_NAME(BM_NONE)
GS_ENUM_NAME(BM_OFF)
GS_ENUM_NAME(BM_ON)



// Arrays
GS_ARRAY_WRAP(gsLogicObject,  gkLogicLink,        LogicList,    LogicIterator)
GS_ARRAY_WRAP(gsSensor,       gkLogicSensor,      Sensors,      SensorIterator)
GS_ARRAY_WRAP(gsController,   gkLogicController,  Controllers,  ControllerIterator)
GS_ARRAY_WRAP(gsActuator,     gkLogicActuator,    Actuators,    ActuatorIterator)


%include "gsBricks.h"
