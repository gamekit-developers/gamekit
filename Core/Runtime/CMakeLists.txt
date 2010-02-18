# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)



if (APPLE)
include_directories(${OGRELITE_SOURCE_DIR}/OgreMain/include/OSX)
        SET(EXETYPE MACOSX_BUNDLE)
        SET_SOURCE_FILES_PROPERTIES( MainMenu.nib  PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
 	SET_SOURCE_FILES_PROPERTIES(momo_ogre.blend PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
	SET (NibFile MainMenu.nib)
endif()

set(SRC momo_ogre.blend Main.cpp)
include_directories(
    ${OGREKIT_OGRE_INCLUDE}
    ${OGREKIT_BULLET_INCLUDE}
    ../Engine2/
    ../Engine2/Script/
    ../Engine2/Loaders/Blender/
    ../Engine2/Physics/
    ../Engine2/LogicBricks
    ../Engine2/Logic
    ../Engine2/Utils/Src
    ../Engine2/Animation
)


link_libraries(
    OgreKitCore
    Utils 
    ${OGREKIT_OGRE_LIBS}
    ${OGREKIT_BULLET_LIBS}
)

if (WIN32)
	if (CMAKE_COMPILER_IS_GNUCXX)
		# FreeImage LibRawLite dependency 
		link_libraries(ws2_32)
	endif (CMAKE_COMPILER_IS_GNUCXX)
endif(WIN32)

if (APPLE)
	add_executable(
	OgreKit 
	MACOSX_BUNDLE
	MainMenu.nib
	${SRC}
)
else ()
	add_executable(OgreKit ${SRC})
	install(TARGETS OgreKit RUNTIME DESTINATION ${OGREKIT_INSTALL_PREFIX})
endif()
