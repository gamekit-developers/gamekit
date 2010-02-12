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

printf(">> Running width debug info\n")
	


-- import internal modules
System.import("GameKit")
System.import("KeyCodes")


debugTable(_G,			"_G");
debugTable(System,		"System");
debugTable(GameKit,		"GameKit");
debugTable(KeyCodes,	"KeyCodes");


local exit_key = KeyCodes.KC_QKEY
local exit_str = "(press Q Key to exit)"

local engine		= GameKit.Engine()
engine.title		= "Lua Test " .. exit_str 
engine.blenmat		= false
engine.quiet		= true
engine.width		= 800
engine.height		= 600
engine.fullscreen	= false


debugTable(engine,	"Engine");

-- FIXME, needs to use video enumeration
--setWideFullScreen(engine)
--setFullScreen(engine)




-- Grab last command line option. eg; fname.lua fname.blend
local fname = System.argv[System.argc-1]

printf("--------------------------------------------\n");
printf("Loading Blend : %s\n", fname);
printf("--------------\n");
local blend = engine:loadBlend(fname)
debugTable(blend, "BlendFile");

local scene = blend:getScene()
printf("--------------------------------------------\n");
printf("Found default scene : '%s' in blend file %s\n", scene:getName(), fname)
debugTable(scene, "Scene")


testSceneReload(scene, 20)
printObjectInfo(scene)


-- load main scene
printf("--------------------------------------------\n");
printf("Starting main loop.\n");

scene:load()

if (scene:isLoaded()) then

	-- internal gamelogic (not setup in lua right now)

	-- scale applied to relitave mouse movement
	local rotfac = -0.3
	-- scale applied to movement vector (forward - reverse) * transfac
	-- (forward and reverse) being unit scale
	local tnsfac = 0.5

	printf("-------------------\n");
	printf("Creating demo mouse-look logic.\n")
	printf("  Rotation factor    = %f\n", rotfac)
	printf("  Translation factor = %f\n", tnsfac);

	scene:createDemoLogic(rotfac, tnsfac)
	scene:addExitHook(exit_key)

	printf("-------------------\n");
	printf("Set exit hook to key code %i\n", exit_key);
	printf("Calling main loop.\n")
	engine:run()

	printf("-------------------\n");
	printf("Main loop returned, exit...\n")
end	
