# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

PROJECT(dummy_ogrekit)

include_directories(
	${OGREKIT_INCLUDE}
)

link_libraries(
	#${OGREKIT_LIB}
	OIS
	log
)


set(HiddenCMakeLists ../CMakeLists.txt)
source_group(ParentCMakeLists FILES ${HiddenCMakeLists})

ADD_LIBRARY(${PROJECT_NAME}
	SHARED
	Main.cpp
	#AndroidLogListener.h
	#AndroidLogListener.cpp
)

#message(STATUS ${CMAKE_CURRENT_BINARY_DIR})

if (1) #cp && strip

set (OUTPUT_NAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PROJECT_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}) #lib<name>.so
set (STRIP_TOOL arm-linux-androideabi-strip)

#message(STATUS ${PROJECT_NAME} "-------" ${OUTPUT_NAME} "-------" ${STRIP_TOOL})

add_custom_command(
	TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} 
	ARGS -E copy_if_different 
		${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_NAME}	
		${CMAKE_CURRENT_SOURCE_DIR}/../Demo/libs/armeabi/${OUTPUT_NAME}
)
	
add_custom_command(
	TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND ${STRIP_TOOL}
	ARGS ${CMAKE_CURRENT_SOURCE_DIR}/../Demo/libs/armeabi/${OUTPUT_NAME})	
	
endif()
