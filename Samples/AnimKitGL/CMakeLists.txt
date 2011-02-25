
set(AKGL_SRC
	Main.cpp
	akEntity.cpp
	akDemo.cpp
	akDemoBase.cpp
	
	BlenderLoader/akAnimationLoader.cpp
	BlenderLoader/akBLoader.cpp
)

set(AKGL_HDR
	akEntity.h
	akDemo.h
	akDemoBase.h
	
	BlenderLoader/akAnimationLoader.h
	BlenderLoader/akBlenderDefines.h
	BlenderLoader/akBLoader.h
)

set(DATA 
	Blu.blend
)

set(ALL
	${AKGL_SRC}
	${AKGL_HDR}
	${DATA}
)

include_directories(${GLUT_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${GAMEKIT_ANIMKIT_PATH} ${GAMEKIT_UTILS_PATH} ${FBT_SOURCE_DIR}/File ${FBT_SOURCE_DIR}/FileFormats/Blend)

link_libraries(${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} ${GAMEKIT_ANIMKIT_TARGET} fbtFile bfBlend)

add_executable(AppAnimKitGL ${ALL})

