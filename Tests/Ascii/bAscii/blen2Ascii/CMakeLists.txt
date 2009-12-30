

set(Blend2AsciiSRC Main.cpp Writer.cpp Writer.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../bAscii/bUtils 
					${CMAKE_CURRENT_SOURCE_DIR}/../bAscii/bApi
					${CMAKE_CURRENT_SOURCE_DIR}/../bAscii
					${BPARSE_EXTERN_DIR}
					)


link_libraries(bAscii ${BPARSE_EXTERN_LIBS})
add_executable(blend2Ascii ${Blend2AsciiSRC})
