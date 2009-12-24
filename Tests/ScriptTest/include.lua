--[[
-------------------------------------------------------------------------------
	Lua 5.1 script tests.
	http://gamekit.googlecode.com/
-------------------------------------------------------------------------------
 External file import.
-------------------------------------------------------------------------------
--]]


-- C style printf
function printf(...)
	io.write(string.format(...))
end

-- print command line arguments to stdout
function printCommandLine(sys)

	printf("Compiled with %s\n", _VERSION)
	printf(">> ");
	for i=0, (sys.argc-1) do
		printf("%s ", sys.argv[i])
	end
	printf("\n")
end


-- reload a scene
function reload(scene)
	scene:unload()
	scene:load()
end

function setWideFullScreen(engine)
	
	engine.width		= 1360
	engine.height		= 768
	engine.fullscreen	= true

end


function setFullScreen(engine)
	
	engine.width		= 1024
	engine.height		= 768
	engine.fullscreen	= true

end

-- Python style dir for printing table methods
function dir(t)
	if (type(t) == "userdata") then
		local mtt = getmetatable(t)
		if (type(mtt) == "table") then
			return dir(mtt)
		end
	end
	
	local str = "["
	local cur = 0
	for k,v in pairs(t) do
		if (cur == 0) then
			str = str .. tostring(k)
		else
			str = str .. ", " .. tostring(k)
		end
		cur = 1
	end
	str = str .. "]"
	return str
end	


-- print table methods to stdout
function dump(t)

	if (type(t) == "userdata") then
		local mtt = getmetatable(t)
		if (type(mtt) == "table") then
			return dump(mtt)
		end
	end
	
	if (not type(t) == "table") then return end


	-- allign print-out
	local max = 0
	local tot = 0
	for k,v in pairs(t) do
		if max < tostring(k):len()  then
			max = tostring(k):len()
		end
		tot = tot + 1
	end
	
	local cur = 0
	for k,v in pairs(t) do
	
		local space = tostring(k):len()
		local newspace = (max - space)
		local spacestr = ""

		for i=0, newspace do spacestr = spacestr .. " " end
		printf("%s%s= %s\n", tostring(k), spacestr, tostring(v))
	end
end


function debugTable(t, name)
	printf("--------------------------------------------\n");
	printf("Table Definition : %s\n", name);
	printf("-----------------\n");
	dump(t)
end

-- Get Game object type string from enumeration
-- see: gkGameObjectTypes
function gameObjectTypeString(t) 
	if (t == 1) then return "Camera" end
	if (t == 2) then return "Light" end
	if (t == 3) then return "Entity" end
	if (t == 4) then return "Node" end
	return "Unknown"
end



function getRenderSystemString(rs, gk)
	if (rs == gk.OGRE_RS_D3D9) then return "Direct3D 9" end
	if (rs == gk.OGRE_RS_D3D10) then return "Direct3D 10" end
	if (rs == gk.OGRE_RS_GL) then return "OpenGL" end
	if (rs == gk.OGRE_RS_GLES) then return "OpenGL ES" end
	return "unknown render system";
end

function clamp(x, mi, ma)
	if (x < mi) then return mi end
	if (x > ma) then return ma end
	return x
end

-- Test scene reload 
function testSceneReload(scene, reload_nr)

	local maxReload = clamp(reload_nr, 1, 20)

	printf("--------------------------------------------\n");
	printf("Testing Scene reload functions\n");
	printf("Unloading/Loading %i time(s)\n", maxReload);
	printf("-----------------\n");

	for i=0, maxReload-1 do
		if (i == 0) then 
			-- Object creation. (the longest)
			printf("load:= 1, creating all objects from scratch.\n")
		elseif (i==1) then
			printf("load:= 2, using cached data from now on (untill scene is deleted).\n")
		else	

			-- Only RenderEngine creation
			-- build data is already present (fast)
			printf("load:= %i\n", i+1)
		end
		reload(scene)
	end
end


-- Test scene reload 
function printObjectInfo(scene)

	local obcnt = scene:getObjectCount()
	
	printf("--------------------------------------------\n");
	printf("Testing object functions\n");
	printf("Total number of objects %i.\n", obcnt)
	printf("-----------------\n");

	for i=0, obcnt-1 do
		local obj = scene:getObjectAt(i)
		printf("%i:= %s\n  type:= %s\n", i+1, obj:getName(), gameObjectTypeString(obj:getType()))
	end
end

