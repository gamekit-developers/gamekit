# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

project(UnitTestOgreKit)

set(APP_DATA_DIR TestData)

file(GLOB_RECURSE APP_SRC TestCase/*.cpp TestCase/*.h)

list(APPEND APP_SRC	
	StdAfx.cpp	
)

set(APP_HDR
	StdAfx.h
)

set(ALL
	${APP_SRC}
	${APP_HDR}
	TestCase/testCaseName.cpp.txt
)

include_directories(
	.
	./TestCase
	${OGREKIT_INCLUDE}
	${GTEST_INCLUDE}
)

link_libraries(
	${OGREKIT_LIB}
	${GTEST_LIB}
)

set(HiddenCMakeLists ../CMakeLists.txt)
source_group(ParentCMakeLists FILES ${HiddenCMakeLists})

use_precompiled_header(${PROJECT_NAME} StdAfx.h StdAfx.cpp)


add_executable(${PROJECT_NAME} ${ALL} ${HiddenCMakeLists})	
	
IF (NOT INTERNAL_CREATE_DISTRIBUTABLE_MSVC_PROJECTFILES)

	get_target_property(TESTCMD ${PROJECT_NAME} LOCATION)	
	ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${TESTCMD} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
	
ENDIF ()
