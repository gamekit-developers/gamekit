# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

PROJECT(AppAndroidTest)

include_directories(
	${OGREKIT_INCLUDE}
)

link_libraries(
	${OGREKIT_LIB}
	#log	
)


set(HiddenCMakeLists ../CMakeLists.txt)
source_group(ParentCMakeLists FILES ${HiddenCMakeLists})

set(SRC	
	Main.cpp
	AndroidLogListener.h
	AndroidLogListener.cpp
)

add_executable(${PROJECT_NAME} ${SRC} ${HiddenCMakeLists})
	
IF (NOT INTERNAL_CREATE_DISTRIBUTABLE_MSVC_PROJECTFILES)
    ADD_CUSTOM_COMMAND(
        TARGET ${PROJECT_NAME} 
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/../../IPhoneDemo/momo_ogre_i.blend ${CMAKE_CURRENT_BINARY_DIR}/momo_ogre_i.blend
    )
ENDIF ()
