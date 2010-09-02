# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

project(AppVehicleDemo)
SET(${PROJECT_NAME} 1.0)

set(DATA 
	clubsilo_demo.blend 
	OgreKitStartup.cfg 
	OgreKitResource.cfg 
	ReadMe.txt
)

if (APPLE)
	include_directories(${OGRELITE_SOURCE_DIR}/OgreMain/include/OSX)
    SET(EXETYPE MACOSX_BUNDLE)
    SET_SOURCE_FILES_PROPERTIES( MainMenu.nib  PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
 	SET_SOURCE_FILES_PROPERTIES(${DATA} PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
	SET (NibFile MainMenu.nib)
endif()


set(SRC 
	${DATA} 
	Main.cpp 
	vdVehicle.cpp
	vdVehicle.h
	vdLogic.cpp
	vdLogic.h
	vdVehicleNode.cpp
	vdVehicleNode.h
)

include_directories(
    ${OGREKIT_OGRE_INCLUDE}
    ${OGREKIT_BULLET_INCLUDE}
    ${OGREKIT_OPENSTEER_INCLUDE}
    ../../Engine/
    ../../Engine/AI
    ../../Engine/Loaders/Blender/
)

link_libraries(
    ${OGREKIT_LIB}
)

if (WIN32)
	if (CMAKE_COMPILER_IS_GNUCXX)
		# FreeImage LibRawLite dependency 
		link_libraries(ws2_32)
	endif (CMAKE_COMPILER_IS_GNUCXX)
endif(WIN32)

set(HiddenCMakeLists ../CMakeLists.txt)
source_group(ParentCMakeLists FILES ${HiddenCMakeLists})


if (APPLE)
	add_executable(
	${PROJECT_NAME}
	MACOSX_BUNDLE
	MainMenu.nib
	${SRC} ${HiddenCMakeLists}
)
else ()
	add_executable(${PROJECT_NAME} ${SRC} ${HiddenCMakeLists})
	
	IF (NOT INTERNAL_CREATE_DISTRIBUTABLE_MSVC_PROJECTFILES)

		if(NOT (${CMAKE_CURRENT_BINARY_DIR} EQUAL ${CMAKE_SOURCE_DIR}))
			ADD_CUSTOM_COMMAND(
				TARGET ${PROJECT_NAME}
				POST_BUILD
				COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/clubsilo_demo.blend ${CMAKE_CURRENT_BINARY_DIR}
				COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/OgreKitResource.cfg ${CMAKE_CURRENT_BINARY_DIR}
				COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/OgreKitStartup.cfg ${CMAKE_CURRENT_BINARY_DIR})
		endif()
	endif ()
		
endif()

