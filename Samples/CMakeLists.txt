# Adding options only to speed up build/dev time

set(AK_DEMO_SRC
	akEntity.cpp
	akDemoBase.cpp
	akGlutDemo.cpp
)

set(AK_DEMO_HDR
	akEntity.h
	akDemoBase.h
	akGlutDemo.h
)

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
#	${FBT_SOURCE_DIR}/File
#	${FBT_SOURCE_DIR}/FileFormats/Blend
	)

link_libraries(${GLEW_LIBRARY} 
	${GLUT_LIBRARIES} 
	${OPENGL_LIBRARIES} 
	${ANIMKIT_ANIMKIT_TARGET} 
	${GAMEKIT_UTILS_TARGET}
#	fbtFile
#	bfBlend
	)

add_library(akDemoBase ${AK_DEMO_SRC} ${AK_DEMO_HDR})

set(AK_DEMOBASE_LIBRARY akDemoBase)
set(AK_BLOADER_LIBRARY akBLoader)

subdirs(BlenderLoader)

if(SAMPLES_ANIMKIT_GL)
	subdirs(AnimKitGL)
	subdirs(SimpleAnimKit)
endif()

