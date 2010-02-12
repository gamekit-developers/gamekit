

set(Blend2AsciiSRC Main.cpp Writer.cpp Writer.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../bAscii/bUtils 
					${CMAKE_CURRENT_SOURCE_DIR}/../bAscii/bApi
					${CMAKE_CURRENT_SOURCE_DIR}/../bAscii
					${GAMEKIT_SERIALIZE_COMMON}
					${GAMEKIT_SERIALIZE_BLENDER}
					${OGREKIT_BULLET_INCLUDE}
					)


link_libraries(bAscii 
	${GAMEKIT_SERIALIZE_COMMON_TARGET}
	${GAMEKIT_SERIALIZE_BLENDER_TARGET}
	${OGREKIT_BULLET_LIBS}
	)
add_executable(blend2Ascii ${Blend2AsciiSRC})
