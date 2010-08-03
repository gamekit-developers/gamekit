/*! \page Examples Examples and tutorials.


\beginmenu{LuaRuntime,For use in the LuaRuntime package}
\LuaGlobalMenu{Setup}
\LuaGlobalMenu{CDefs}
\LuaGlobalMenu{CArrayIter}
\endmenu


\endpage


<!-- ============================================ Setup ============================================ -->
\LuaGlobal{Setup,Engine Setup}


\LuaExtern{Blend:,Test0.blend}

An example showing how to set up the OgreKit Engine, and load a .blend. 
\include Test0.lua

\endpage



<!-- ============================================ CDefs ============================================ -->
\LuaGlobal{CDefs,Custom User defined variables.}


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
\LuaGlobal{CArrayIter,Using arrays and iterators.}

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


*/