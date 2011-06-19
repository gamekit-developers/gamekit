
set(AK_BLOADER_SRC
	akAnimationLoader.cpp
	akBLoader.cpp
	akMeshLoader.cpp
	akTextureLoader.cpp
)

set(AK_BLOADER_HDR
	akAnimationLoader.h
	akBlenderDefines.h
	akBLoader.h
	akMeshLoader.h
	akTextureLoader.h
)


include_directories(${GLEW_INCLUDE_PATH}
	${GLUT_INCLUDE_DIR}
	${OPENGL_INCLUDE_DIR}
	${ANIMKIT_ANIMKIT_PATH}
	${ANIMKIT_VECTORMATH_PATH}
	${GAMEKIT_UTILS_PATH}
	${FBT_SOURCE_DIR}/File
	${FBT_SOURCE_DIR}/FileFormats/Blend
	)

link_libraries(${GLEW_LIBRARY} 
	${GLUT_LIBRARIES} 
	${OPENGL_LIBRARIES} 
	${ANIMKIT_ANIMKIT_TARGET} 
	${GAMEKIT_UTILS_TARGET}
	fbtFile
	bfBlend
	)

add_library(${AK_BLOADER_LIBRARY} ${AK_BLOADER_SRC} ${AK_BLOADER_HDR})

