# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

set(TargetName AppAnimKitGL)

set(AKGL_SRC
	Main.cpp
	akEntity.cpp
	akDemo.cpp
	akDemoBase.cpp
	
	BlenderLoader/akAnimationLoader.cpp
	BlenderLoader/akBLoader.cpp
	BlenderLoader/akMeshLoader.cpp
)

set(AKGL_HDR
	akEntity.h
	akDemo.h
	akDemoBase.h
	
	BlenderLoader/akAnimationLoader.h
	BlenderLoader/akBlenderDefines.h
	BlenderLoader/akBLoader.h
	BlenderLoader/akMeshLoader.h
)

set(DATA 
	Blu.blend
)

set(ALL
	${AKGL_SRC}
	${AKGL_HDR}
	${DATA}
)

if(WIN32)
	source_group(Datafiles FILES ${DATA})
endif()

find_package(OpenGL)

find_package(GLUT)
if(NOT GLUT_FOUND)
	IF (MINGW)
		MESSAGE ("GLUT NOT FOUND, trying to use MINGW glut32")
		SET(GLUT_LIBRARIES glut32)
		#TODO add better GLUT detection for MinGW
		SET(GLUT_FOUND TRUE)
	ENDIF (MINGW)
	IF (MSVC)
		SET(GLUT_FOUND TRUE)
		IF (CMAKE_CL_64)
			message("Win64 using Glut/glut64.lib")
			SET(GLUT_LIBRARIES ${ANIMKIT_SAMPLES_DIR}/Glut/glut64.lib)
		ELSE(CMAKE_CL_64)
			message("Win32 using Glut/glut32.lib")
			SET(GLUT_LIBRARIES ${ANIMKIT_SAMPLES_DIR}/Glut/glut32.lib)
		ENDIF (CMAKE_CL_64)
		set(GLUT_INCLUDE_DIR ${ANIMKIT_SAMPLES_DIR}/Glut)
	endif()
endif()

find_package(GLEW)
if(NOT GLEW_FOUND)
	if(MINGW)
		message("GLEW NOT FOUND, trying to use MINGW glew32")
		set(GLEW_LIBRARY glew32)
		#TODO add better GLEW detection for MinGW
		set(GLEW_FOUND TRUE)
	endif(MINGW)
	if(MSVC)
		if(CMAKE_CL_64)
			message("Win64 using Glut/glew64.lib")
			set(GLEW_LIBRARY ${ANIMKIT_SAMPLES_DIR}/Glut/glew64.lib)
		else(CMAKE_CL_64)
			message("Win32 using Glut/glew32.lib")
			SET(GLEW_LIBRARY ${ANIMKIT_SAMPLES_DIR}/Glut/glew32.lib)
		endif()
		set(GLEW_INCLUDE_PATH ${ANIMKIT_SAMPLES_DIR}/Glut)
		set(GLEW_FOUND TRUE)
	endif()
endif()

include_directories(${GLEW_INCLUDE_PATH}
	${GLUT_INCLUDE_DIR}
	${OPENGL_INCLUDE_DIR}
	${ANIMKIT_ANIMKIT_PATH}
	${ANIMKIT_VECTORMATH_PATH}
	${GAMEKIT_UTILS_PATH}
	${FBT_SOURCE_DIR}/File
	${FBT_SOURCE_DIR}/FileFormats/Blend
	)

link_libraries(${GLEW_LIBRARY} 
	${GLUT_LIBRARIES} 
	${OPENGL_LIBRARIES} 
	${ANIMKIT_ANIMKIT_TARGET} 
	fbtFile
	bfBlend
	)

add_executable(${TargetName} ${ALL})
	
	
IF (NOT INTERNAL_CREATE_DISTRIBUTABLE_MSVC_PROJECTFILES)
ADD_CUSTOM_COMMAND(
	TARGET ${TargetName} 
	POST_BUILD
	COMMAND ${CMAKE_COMMAND}
	ARGS -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/Blu.blend ${CMAKE_CURRENT_BINARY_DIR}
	)
ENDIF ()



IF(WIN32)
IF (CMAKE_CL_64)
	IF (NOT INTERNAL_CREATE_DISTRIBUTABLE_MSVC_PROJECTFILES)
		ADD_CUSTOM_COMMAND(
			TARGET ${TargetName}
			POST_BUILD
			COMMAND ${CMAKE_COMMAND}
			ARGS -E copy_if_different ${ANIMKIT_SAMPLES_DIR}/glut64.dll ${CMAKE_CURRENT_BINARY_DIR}
		)
		ADD_CUSTOM_COMMAND(
			TARGET ${TargetName}
			POST_BUILD
			COMMAND ${CMAKE_COMMAND}
			ARGS -E copy_if_different ${ANIMKIT_SAMPLES_DIR}/glew64.DLL ${CMAKE_CURRENT_BINARY_DIR}
		)
    ENDIF()
ELSE(CMAKE_CL_64)
	IF (NOT INTERNAL_CREATE_DISTRIBUTABLE_MSVC_PROJECTFILES)
		ADD_CUSTOM_COMMAND(
			TARGET ${TargetName}
			POST_BUILD
			COMMAND ${CMAKE_COMMAND}
			ARGS -E copy_if_different ${ANIMKIT_SAMPLES_DIR}/glut32.dll ${CMAKE_CURRENT_BINARY_DIR}
		)
		ADD_CUSTOM_COMMAND(
			TARGET ${TargetName}
			POST_BUILD
			COMMAND ${CMAKE_COMMAND}
			ARGS -E copy_if_different ${ANIMKIT_SAMPLES_DIR}/glew32.dll ${CMAKE_CURRENT_BINARY_DIR}
		)
    ENDIF()
ENDIF(CMAKE_CL_64)
ENDIF(WIN32)
