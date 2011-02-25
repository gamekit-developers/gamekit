# Adding options only to speed up build/dev time

if (OGREKIT_BUILD_IPHONE)
	subdirs(IPhoneDemo)
else()

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

if (SAMPLES_ANIMKIT_GL)
	find_package(GLUT)
	#find_package(OpenGL)
	if(NOT GLUT_FOUND)
		IF (MINGW)
			MESSAGE ("GLUT NOT FOUND not found, trying to use MINGW glut32")
			SET(GLUT_LIBRARIES glut32)
			#TODO add better GLUT detection for MinGW
			SET(GLUT_FOUND TRUE)
		ENDIF (MINGW)
		IF (MSVC)
			SET(GLUT_FOUND TRUE)
			IF (CMAKE_CL_64)
				message("Win64 using Glut/glut64.lib")
				SET(GLUT_LIBRARIES ${OGREKIT_SAMPLES_DIR}/Glut/glut64.lib)
			ELSE(CMAKE_CL_64)
				message("Win32 using Glut/glut32.lib")
				SET(GLUT_LIBRARIES ${OGREKIT_SAMPLES_DIR}/Glut/glut32.lib)
			ENDIF (CMAKE_CL_64)
		 	set(GLUT_INCLUDE_DIR ${OGREKIT_SAMPLES_DIR}/Glut)
		endif()
	endif()
	
	subdirs(AnimKitGL)
	
endif()

if (SAMPLES_GUIDEMO)
    subdirs(GuiDemo)
endif()

endif()
