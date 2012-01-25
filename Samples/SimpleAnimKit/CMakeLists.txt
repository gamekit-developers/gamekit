# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

set(TargetName AppSimpleAnimKitGL)

set(AKGL_SRC
	akDemo.cpp
)

set(AKGL_HDR
	akDemo.h
)

set(ALL
	${AKGL_SRC}
	${AKGL_HDR}
	${DATA}
)

if(WIN32)
	source_group(Datafiles FILES ${DATA})
endif()


include_directories(${GLEW_INCLUDE_PATH}
	${GLUT_INCLUDE_DIR}
	${OPENGL_INCLUDE_DIR}
	${ANIMKIT_ANIMKIT_PATH}
	${ANIMKIT_VECTORMATH_PATH}
	${GAMEKIT_UTILS_PATH}
	)

link_libraries(${GLEW_LIBRARY} 
	${GLUT_LIBRARIES} 
	${OPENGL_LIBRARIES} 
	${ANIMKIT_ANIMKIT_TARGET} 
	${GAMEKIT_UTILS_TARGET}
	${AK_DEMOBASE_LIBRARY}
	)

add_executable(${TargetName} ${ALL})
	
	

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
