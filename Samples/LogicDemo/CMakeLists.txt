# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

project(LogicDemo)
SET(${PROJECT_NAME} 1.0)

#IF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
#MESSAGE(FATAL_ERROR "CMake generation for LogicDemo is not allowed within the source directory!")
#ENDIF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

set(DATA 
	./data/momo_ogre_plus.blend 
	./data/OgreKitStartup.cfg 
	./data/OgreKitResource.cfg 
	ReadMe.txt
)

if (APPLE)
include_directories(${OGRELITE_SOURCE_DIR}/OgreMain/include/OSX)
        SET(EXETYPE MACOSX_BUNDLE)
        SET_SOURCE_FILES_PROPERTIES( MainMenu.nib  PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
 	SET_SOURCE_FILES_PROPERTIES(${DATA} PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
	SET (NibFile MainMenu.nib)
endif()


set(SRC ${DATA} Main.cpp Common.h RatLogic.h RatLogic.cpp MomoLogic.h MomoLogic.cpp SceneLogic.h SceneLogic.cpp)

include_directories(
    ${OGREKIT_OGRE_INCLUDE}
    ${OGREKIT_BULLET_INCLUDE}
    ../../Engine/
    ../../Engine/Script/
    ../../Engine/Loaders/Blender/
    ../../Engine/Physics/
    ../../Engine/LogicBricks
    ../../Engine/Logic
    ../../Engine/Utils
    ../../Engine/Animation
)

link_libraries(
    OgreKitCore
    ${OGREKIT_OGRE_LIBS}
    ${OGREKIT_BULLET_LIBS}
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
endif()

if(NOT (${CMAKE_CURRENT_BINARY_DIR} EQUAL ${CMAKE_SOURCE_DIR}))

	ADD_CUSTOM_COMMAND(
		TARGET ${PROJECT_NAME}
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/data
		COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/data/momo_ogre_plus.blend ${CMAKE_CURRENT_BINARY_DIR}/data
		COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/data/OgreKitResource.cfg ${CMAKE_CURRENT_BINARY_DIR}/data
		COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/data/OgreKitStartup.cfg ${CMAKE_CURRENT_BINARY_DIR}/data
	)

endif()
