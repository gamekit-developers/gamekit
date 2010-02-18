

if (APPLE)
    SET(EXETYPE MACOSX_BUNDLE)
    SET_SOURCE_FILES_PROPERTIES( MainMenu.nib  PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
 	SET_SOURCE_FILES_PROPERTIES( PhysicsAnimationBakingDemo.blend PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
	SET (NibFile MainMenu.nib)
endif()

set(SRC Main.cpp
		PhysicsAnimationBakingDemo.blend
		)

include_directories(
	${OGREKIT_OGRE_INCLUDE}
	${OGREKIT_OIS_INCLUDE}
	${OGREKIT_BULLET_INCLUDE}
	${OGREKIT_INCLUDE}
	${GAMEKIT_SERIALIZE_COMMON}
	${GAMEKIT_SERIALIZE_BLENDER}
	)

link_libraries(
	OgreKit
	OIS
	${OGREKIT_OGRE_LIBS}
	${OGREKIT_BULLET_LIBS}
	${GAMEKIT_SERIALIZE_COMMON_TARGET}
	${GAMEKIT_SERIALIZE_BLENDER_TARGET}
)

configure_rendersystem()

if (APPLE)
	add_executable( BlendReader MACOSX_BUNDLE MainMenu.nib ${SRC})
else ()
	add_executable(BlendReader ${SRC})
endif()
