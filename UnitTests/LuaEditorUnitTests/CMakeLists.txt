# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

project(UnitTestLuaEditor)


#set(OGREKIT_USE_FILETOOLS TRUE CACHE BOOL "Forcing FileTools" FORCE)


set(APP_DATA_DIR TestData)

file(GLOB_RECURSE APP_SRC TestCase/*.cpp TestCase/*.h)

list(APPEND APP_SRC	
	StdAfx.cpp	
)

set(APP_HDR
	StdAfx.h
)


file(GLOB_RECURSE LULIB_SRC 
	${OGREKIT_SAMPLES_DIR}/LuaEditor/Lib/*.cpp 
	${OGREKIT_SAMPLES_DIR}/LuaEditor/Lib/*.h
	${OGREKIT_SAMPLES_DIR}/LuaEditor/luFile.cpp
	${OGREKIT_SAMPLES_DIR}/LuaEditor/luFile.h
)

set(ALL
	${APP_SRC}
	${APP_HDR}
	TestCase/testCaseName.cpp.txt
	${LULIB_SRC}
)

include_directories(
	.
	./TestCase
	${OGREKIT_INCLUDE}
	${GTEST_INCLUDE}
	${OGREKIT_TINYXML_INCLUDE}
	${OGREKIT_SAMPLES_DIR}/LuaEditor
	${OGREKIT_SAMPLES_DIR}/LuaEditor/Lib
)

link_libraries(
	${OGREKIT_LIB}
	${OGREKIT_TINYXML_TARGET}
	${GTEST_LIB}	
)

if (UNIX)
	link_libraries(pthread)
endif()



set(HiddenCMakeLists ../CMakeLists.txt)
source_group(ParentCMakeLists FILES ${HiddenCMakeLists})
source_group(LuaEditor FILES ${LULIB_SRC})

use_precompiled_header(${PROJECT_NAME} StdAfx.h StdAfx.cpp)


add_executable(${PROJECT_NAME} ${ALL} ${HiddenCMakeLists})	
	
IF (NOT INTERNAL_CREATE_DISTRIBUTABLE_MSVC_PROJECTFILES)

	get_target_property(TESTCMD ${PROJECT_NAME} LOCATION)	
	ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${TESTCMD} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
		
ENDIF ()
