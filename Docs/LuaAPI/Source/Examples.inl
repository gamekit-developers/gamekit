/*! \page Examples Examples and tutorials.


\beginmenu
\LuaGlobalMenu{Setup}
\LuaGlobalMenu{OnInit}
\LuaGlobalMenu{CDefs}
\LuaGlobalMenu{CArrayIter}
\LuaGlobalMenu{CustomLogicBricks}
\endmenu


<!-- ============================================ Setup ============================================ -->
\LuaExample{Setup,Setup for a stand alone application}



An example showing how to set up the OgreKit Engine, and load a .blend.
\include Test0.lua

\endpage


<!-- ============================================ OnInit ============================================ -->
\LuaExample{OnInit,Setup for an embedded application}


Setup from inside blender takes place in a predefined text block named \b OnInit.lua \n
This script will execute every time the scene is loaded.\n

Scripts may still span multiple text files using the standard Blender game engine method.
By attaching to a Python controller.<i> \LuaClassRef{ScriptController} </i>

\include OnInit.lua


\endpage




<!-- ============================================ CDefs ============================================ -->
\LuaExample{CDefs,Custom User defined variables.}


This shows a simple way to add custom properties to \LuaClassRef{UserDefs}.
\code
local engine = OgreKit.Engine()
local prefs  = engine:getUserDefs()

local custom = OgreKit.Property("Custom", "Hello World")
prefs:addProperty(custom)

print(prefs.Custom)

\endcode
\endpage




<!-- ============================================ CArrayIter ============================================ -->
\LuaExample{CArrayIter,Using arrays and iterators.}

Example array and iterator usage

\code

local objects = OgreKit.ObjectList()
objects:push(scene:getObject("Cube"))


local iter = objects:iterator()
while (iter:hasMoreElements()) do
    local obj = iter:getNext()

    dPrintf("%s, %s", obj:getName(), tostring(obj:getWorldPosition()))
end
\endcode


\endpage




<!-- ============================================ CustomLogicBricks ============================================ -->
\LuaExample{CustomLogicBricks,Adding custom logic brick callbacks.}

This shows one way to add custom callbacks on logic bricks.
The example here shows usage for sensors, but the method applies to controllers and actuators.

\code

--- Get logic manager
logicManager = OgreKit.LogicManager()

player = scene:getObject("Cube")

logicPlayer = logicManager:newObject(player)



--- Sensor

delay = OgreKit.DelaySensor(logicPlayer)
delay:setDelay(20)


--- Controller

operator = OgreKit.LogicOpController(logicPlayer)
operator:setOp(OgreKit.LOP_AND)


--- Actuator

motion = OgreKit.MotionActuator(logicPlayer)
motion:setRotation(0.0, 0.0, 0.15, true)


--- Connect links

operator:link(delay)
operator:link(motion)



-- Add callbacks

DelayCallback = Class()
function DelayCallback:constructor(sensor)


    sensor:connect(OgreKit.LM_EVT_OVERIDE, self, DelayCallback.OnQuery)

    self.maxDelay = sensor:getDelay()
    self.incrment = 0
    self.len = self.maxDelay

end


function DelayCallback:OnQuery()

    -- do tests


    if (self.incrment > self.maxDelay) then
        if (self.incrment > self.len * 2) then
            self.incrment = 0
        else
            self.incrment = self.incrment + 1
        end

        return true
    end

    self.incrment = self.incrment + 1
    return false
end

callback = DelayCallback(delay)


\endcode

\endpage

*/