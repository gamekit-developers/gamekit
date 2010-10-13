# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

project(UnitTestFbt)

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
	${GAMEKIT_UTILS_PATH} 
	${OGREKIT_INCLUDE}
	${GTEST_INCLUDE}
	${FBT_SOURCE_DIR}/File
	${FBT_SOURCE_DIR}/FileFormats/Blend
	${FBT_SOURCE_DIR}/FileFormats/Bullet
)

link_libraries(
	${GAMEKIT_UTILS_TARGET} 
	${OGREKIT_LIB}
	${GTEST_LIB}
	fbtFile
	bfBlend
	btBulletFile
)



set(HiddenCMakeLists ../CMakeLists.txt)
source_group(ParentCMakeLists FILES ${HiddenCMakeLists})

use_precompiled_header(${PROJECT_NAME} StdAfx.h StdAfx.cpp)


add_executable(${PROJECT_NAME} ${ALL} ${HiddenCMakeLists})	
	
set(CPCMD ${CMAKE_COMMAND} -E copy_directory    ${CMAKE_CURRENT_SOURCE_DIR}/${APP_DATA_DIR} ${CMAKE_CURRENT_BINARY_DIR}/${APP_DATA_DIR})
set(RMCMD ${CMAKE_COMMAND} -E remove_directory  ${CMAKE_CURRENT_BINARY_DIR}/${APP_DATA_DIR}/.svn)


IF (NOT INTERNAL_CREATE_DISTRIBUTABLE_MSVC_PROJECTFILES)
	ADD_CUSTOM_COMMAND(
		TARGET ${PROJECT_NAME} 
		POST_BUILD
		COMMAND ${CPCMD}
		COMMAND ${RMCMD}
	)
	
	if (MSVC)
		ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD COMMAND $(TargetPath))
	endif()
	
ENDIF ()
