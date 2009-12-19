--[[
-------------------------------------------------------------------------------
	Lua 5.1 script tests.
	http://gamekit.googlecode.com/
-------------------------------------------------------------------------------
	Example GameKit loader.
-------------------------------------------------------------------------------
--]]
-- include files
System.import("include.lua")

function test_usage(sys)

	local usage = "Usage : file.lua file.blend"

	local fname = sys.argv[sys.argc-1]
	if (fname == nil) then print(usage) return false end
	if (string.find(fname, ".blend") == nil) then print(usage) return false end	

	printCommandLine(sys)
	return true
end

if (test_usage(System) == false) then return end

-- import internal modules
System.import("GameKit")
System.import("KeyCodes")


local exit_key = KeyCodes.KC_QKEY
local exit_str = "(press Q Key to exit)"

local engine		= GameKit.Engine()
engine.title		= "Lua Test " .. exit_str 
engine.blenmat		= false
engine.quiet		= true
engine.width		= 800
engine.height		= 600
engine.fullscreen	= false

-- FIXME, needs to use video enumeration
--setWideFullScreen(engine)
--setFullScreen(engine)

-- Grab last command line option. eg; fname.lua fname.blend
local fname = System.argv[System.argc-1]
local blend = engine:loadBlend(fname)
local scene = blend:getScene()

-- load main scene
scene:load()

if (scene:isLoaded()) then

	-- internal gamelogic (not setup in lua right now)

	-- scale applied to relitave mouse movement
	local rotfac = -0.3
	-- scale applied to movement vector (forward - reverse) * transfac
	-- (forward and reverse) being unit scale
	local tnsfac = 0.5

	scene:createDemoLogic(rotfac, tnsfac)
	scene:addExitHook(exit_key)

	engine:run()
end	
