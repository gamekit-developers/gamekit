# ---------------------------------------------------------
include(ConfigApp)

set(SRC	
	Lua.cpp
)


set(CONTENT	Test1.lua Test1.blend)
add_ogrekit_executable(LuaRuntime ${SRC} CONTENT)
