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
)

set(AKGL_HDR
	akEntity.h
	akDemo.h
	akDemoBase.h
	
	BlenderLoader/akAnimationLoader.h
	BlenderLoader/akBlenderDefines.h
	BlenderLoader/akBLoader.h
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

include_directories(${GLUT_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${GAMEKIT_ANIMKIT_PATH} ${GAMEKIT_UTILS_PATH} ${FBT_SOURCE_DIR}/File ${FBT_SOURCE_DIR}/FileFormats/Blend)

link_libraries(${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} ${GAMEKIT_ANIMKIT_TARGET} fbtFile bfBlend)



if (APPLE)

	add_executable(
		${TargetName} 
		MACOSX_BUNDLE
		MainMenu.nib
		${ALL} 
	)

else ()

	add_executable(${TargetName} ${ALL})
	
	
	IF (NOT INTERNAL_CREATE_DISTRIBUTABLE_MSVC_PROJECTFILES)
		ADD_CUSTOM_COMMAND(
			TARGET ${TargetName} 
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/Blu.blend ${CMAKE_CURRENT_BINARY_DIR}/Blu.blend
		)
	ENDIF ()

endif()