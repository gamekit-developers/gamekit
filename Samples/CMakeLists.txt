# Adding options only to speed up build/dev time

# skip pre-1.9mercurial-Android-stuff
if (NOT GAMEKIT_GRAPHIC_BACKEND EQUAL GAMEKIT_GRAPHIC_BACKEND_19_mercurial)
    if (OGREKIT_BUILD_ANDROID)
	    subdirs(AndroidDemo)
		subdirs(OgreDemo)
		return()
	elseif (OGREKIT_BUILD_NACL)
		subdirs(OgreDemo)
		return()
    elseif (OGREKIT_BUILD_IPHONE)
	    subdirs(IPhoneDemo)
		return()
    elseif (SAMPLES_ANDROIDTEST)
		subdirs(AndroidDemo/Test)
	endif()    
endif()


if (SAMPLES_RUNTIME)
    subdirs(Runtime)    
endif()

if (SAMPLES_LOGICDEMO)
	subdirs(LogicDemo)
endif()

if (SAMPLES_VEHICLEDEMO)
	subdirs(VehicleDemo)
endif()

if (SAMPLES_LUARUNTIME AND OGREKIT_USE_LUA)
	subdirs(LuaRuntime)
endif()

if (SAMPLES_CPPDEMO)    
	subdirs(CppDemo)    
endif()

if (SAMPLES_NODE_EDITOR)
	subdirs(NodeEditor)
endif()

if (SAMPLES_EMBEDDEMO)
	subdirs(EmbedDemo)
endif()

if (SAMPLES_INSPECTOR)
	subdirs(FileInspector)
endif()

if (SAMPLES_LUA_EDITOR)
    subdirs(LuaEditor)
endif()

if (SAMPLES_GUIDEMO)
    subdirs(GuiDemo)
endif()

if (SAMPLES_OGREDEMO)	
	subdirs(OgreDemo)
endif()
