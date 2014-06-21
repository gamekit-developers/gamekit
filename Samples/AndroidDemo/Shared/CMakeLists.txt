# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

PROJECT(OgreKit)

include_directories(
	${OGREKIT_INCLUDE}
	${OGREKIT_GLES2RS_INCLUDE}
	${OGREKIT_GLES2RS_INCLUDE}/EGL
	${ANDROID_NDK}/sources/android/native_app_glue
)

link_libraries(
	${OGREKIT_LIB}	
	log
	GLESv2	
	android
)

set(HiddenCMakeLists ../CMakeLists.txt)
source_group(ParentCMakeLists FILES ${HiddenCMakeLists})

ADD_LIBRARY(${PROJECT_NAME}
	SHARED
	Main.cpp
	AndroidLogListener.h
	AndroidLogListener.cpp
	${ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c
	${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
)


if (1) #cp && strip

SET(ANT_EXECUTABLE "ant")
if(${ANDROID_NATIVE_API_LEVEL} LESS 14)
	MATH(EXPR ANDROID_SDK_API_LEVEL "${ANDROID_NATIVE_API_LEVEL}+1")
else()
	SET(ANDROID_SDK_API_LEVEL "${ANDROID_NATIVE_API_LEVEL}")
	SET(SCREEN_SIZE "|screenSize")
endif()

# if(OGRE_CONFIG_ENABLE_GLES2_GLSL_OPTIMISER)
	# SET(GLES_OPTIMISER "-lglsl_optimizer -lmesa -lglcpp-library")
# endif()

SET(ANDROID_TARGET "android-${ANDROID_SDK_API_LEVEL}")


SET(NDKOUT "${CMAKE_BINARY_DIR}/AndroidDemo")
file(MAKE_DIRECTORY "${NDKOUT}")
file(MAKE_DIRECTORY "${NDKOUT}/jni")
file(MAKE_DIRECTORY "${NDKOUT}/assets")	
file(MAKE_DIRECTORY "${NDKOUT}/res")
file(MAKE_DIRECTORY "${NDKOUT}/libs")
file(MAKE_DIRECTORY "${NDKOUT}/libs/${ANDROID_ABI}")
file(WRITE "${NDKOUT}/default.properties" "target=${ANDROID_TARGET}")

set(DEMO_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../Demo)

set(ANDROID_MOD_NAME OgreKit)
configure_file("${OGRE_TEMPLATES_DIR}/eclipse.project.in" "${NDKOUT}/.project" @ONLY)
configure_file("${DEMO_SRC}/AndroidManifest.xml.in" "${NDKOUT}/AndroidManifest.xml" @ONLY)
file(COPY "${DEMO_SRC}/assets" DESTINATION "${NDKOUT}")
file(COPY "${DEMO_SRC}/res" DESTINATION "${NDKOUT}")
FILE(COPY "${DEMO_SRC}/src" DESTINATION "${NDKOUT}")
FILE(COPY "${DEMO_SRC}/.classpath" DESTINATION "${NDKOUT}")
file(COPY "${OGREKIT_OPENAL_LIBRARY}" DESTINATION "${NDKOUT}/libs/${ANDROID_ABI}")
#file(COPY "${DEMO_SRC}/.project" DESTINATION "${NDKOUT}")
	
set (OUTPUT_NAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PROJECT_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}) #lib<name>.so
set (STRIP_TOOL arm-linux-androideabi-strip)

#message(STATUS ${PROJECT_NAME} "-------" ${OUTPUT_NAME} "-------" ${STRIP_TOOL})

set_target_properties(${PROJECT_NAME} PROPERTIES    
		LIBRARY_OUTPUT_DIRECTORY ${NDKOUT}/libs/${ANDROID_ABI}
		RUNTIME_OUTPUT_DIRECTORY ${NDKOUT}/libs/${ANDROID_ABI}
		)


# add_custom_command(	TARGET ${PROJECT_NAME}
	# POST_BUILD
	# COMMAND ${CMAKE_COMMAND} 
	# ARGS -E copy_if_different 
		# ${OUTPUT_NAME}	
		# ${CMAKE_CURRENT_SOURCE_DIR}/../Demo/libs/${ANDROID_ABI}/${OUTPUT_NAME}
# )		
#		${CMAKE_CURRENT_SOURCE_DIR}/../Demo/libs/armeabi/${OUTPUT_NAME}

add_custom_command(
	TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND ${CMAKE_STRIP}
	ARGS ${NDKOUT}/libs/${ANDROID_ABI}/${OUTPUT_NAME})	
	
	
endif()

message (STATUS "android-outputdir  ${NDKOUT}")