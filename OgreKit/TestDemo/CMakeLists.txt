
set(SRC Main.cpp)

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
add_executable(BlendReader ${SRC})
