
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
		${OGRE_STATIC_RESOURCE})

include_directories(
	${OGREKIT_OGRE_INCLUDE}
	${OGREKIT_OIS_INCLUDE}
	${GAMEKIT_SERIALIZE_COMMON}
	${GAMEKIT_SERIALIZE_BLENDER}
	${OGREKIT_BULLET_INCLUDE}
	${OGREKIT_INCLUDE}
	)

link_libraries(
	${GAMEKIT_SERIALIZE_COMMON_TARGET}
	${GAMEKIT_SERIALIZE_BLENDER_TARGET}
	${OGREKIT_OGRE_LIBS}
	${OGREKIT_BULLET_LIBS}
	OgreKit
	OIS
)

configure_rendersystem()
add_executable(Momo ${SRC} ${HDR} ${EXTRA})
