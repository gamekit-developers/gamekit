
if (APPLE)
    SET(EXETYPE MACOSX_BUNDLE)
    SET_SOURCE_FILES_PROPERTIES( MainMenu.nib  PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
 	SET_SOURCE_FILES_PROPERTIES( MomoAnimation.blend PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
	SET (NibFile MainMenu.nib)
endif()

set(HDR OgreAction.h
		OgreActionChannel.h
		OgreActionManager.h
		OgreBezierSpline.h
		OgreDebugBone.h
		OgreManualBone.h
		OgreManualSkeleton.h
		)
set(SRC Momo.cpp
		OgreAction.cpp
		OgreActionChannel.cpp
		OgreActionManager.cpp
		OgreBezierSpline.cpp
		OgreDebugBone.cpp
		OgreManualBone.cpp
		OgreManualSkeleton.cpp
		MomoAnimation.blend
		)

include_directories(
	${OGREKIT_OGRE_INCLUDE}
	${OGREKIT_OIS_INCLUDE}
	${GAMEKIT_SERIALIZE_COMMON}
	${GAMEKIT_SERIALIZE_BLENDER}
	${OGREKIT_BULLET_INCLUDE}
	${OGREKIT_INCLUDE}
	)

link_libraries(
	OIS
	OgreKit
	${GAMEKIT_SERIALIZE_COMMON_TARGET}
	${GAMEKIT_SERIALIZE_BLENDER_TARGET}
	${OGREKIT_OGRE_LIBS}
	${OGREKIT_BULLET_LIBS}
)

configure_rendersystem()


if (APPLE)
	add_executable( Momo MACOSX_BUNDLE MainMenu.nib ${SRC} ${HDR})
else ()
	add_executable(Momo ${SRC} ${HDR})
endif()
