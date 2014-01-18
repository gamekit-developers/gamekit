#-------------------------------------------------------------------
# This file is part of the CMake build system for OGRE
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http://www.ogre3d.org/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

# Configure Browser demo build

file(GLOB HEADER_FILES 
	include/*.h
	Common/include/*.h
	BezierPatch/include/*.h
	ShaderSystem/include/*.h
	)

set(SOURCE_FILES src/SampleBrowser.cpp)
# Get the list of configured samples
get_property(OGRE_SAMPLES_LIST GLOBAL PROPERTY "OGRE_SAMPLES_LIST")

if (OGRE_BUILD_PLATFORM_APPLE_IOS)
  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/SampleBrowser.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
endif ()

if (APPLE AND OGRE_BUILD_PLATFORM_APPLE_IOS)
  set(HEADER_FILES ${HEADER_FILES}
    include/SampleBrowser_iOS.h
  )
elseif (OGRE_BUILD_PLATFORM_NACL)
  set(HEADER_FILES ${HEADER_FILES}
    include/SampleBrowser_NaCl.h
  )
else ()
  set(HEADER_FILES ${HEADER_FILES}
    include/SampleBrowser_OSX.h
	Common/misc/SampleBrowser_OSX.icns
  )
endif (APPLE AND OGRE_BUILD_PLATFORM_APPLE_IOS)

if(ANDROID)
  set(HEADER_FILES ${HEADER_FILES} include/SampleBrowser_Android.h)
endif(ANDROID)

include_directories(
	#${OGREKIT_INCLUDE}
	${OGREKIT_OGRE_INCLUDE}
	${OGRELITE_SOURCE_DIR}/PlugIns/ParticleFX/include
	${OGREKIT_DEP_INCLUDE}
)

message(STATUS "ogre:"${OGREKIT_OGRE_INCLUDE})

link_libraries(${OGREKIT_OGRE_LIBS} Plugin_ParticleFX)

link_directories(${OGRE_LIBDIR})
#include_directories(${OGREKIT_BULLET_INCLUDE})

configure_rendersystem()


include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/Common/include
	${CMAKE_CURRENT_SOURCE_DIR}/BezierPatch/include
	${CMAKE_CURRENT_SOURCE_DIR}/ShaderSystem/include
)

if(ANDROID)
	
	include_directories(
		${ANDROID_NDK}/sources/android/native_app_glue
		${OGRE_SOURCE_DIR}/RenderSystems/GLES2/include/EGL
		${OGRE_SAMPLE_DIR}/include		
	)
	
	#message(STATUS "android:"${OGREKIT_OGRE_INCLUDE}/Android)
	#message(STATUS "include:"${OGRE_SOURCE_DIR}/OgreMain/include)
	

    ADD_LIBRARY(SampleBrowser MODULE #${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp	
		${SOURCE_FILES}
		${ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c
		${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
	)
    target_link_libraries(SampleBrowser ${SAMPLE_LIBRARIES}
		log
		GLESv2	
		android
	)
    #add_dependencies(SampleBrowser ${OGRE_SAMPLES_LIST})
	#add_dependencies(SampleBrowser Sample_BezierPatch)
	
	
	#file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp "int x = 0;")

    if(APPLE OR WIN32)
      SET(ANDROID_EXECUTABLE "android")
      SET(NDK_BUILD_EXECUTABLE "ndk-build")
    else()
      SET(ANDROID_EXECUTABLE "$ENV{ANDROID_SDK}/tools/android")
      SET(NDK_BUILD_EXECUTABLE "$ENV{ANDROID_NDK}/ndk-build")
    endif()

    SET(ANT_EXECUTABLE "ant")
	if(${ANDROID_NATIVE_API_LEVEL} LESS 14)
		MATH(EXPR ANDROID_SDK_API_LEVEL "${ANDROID_NATIVE_API_LEVEL}+1")
	else()
		SET(ANDROID_SDK_API_LEVEL "${ANDROID_NATIVE_API_LEVEL}")
		SET(SCREEN_SIZE "|screenSize")
	endif()
	
	if(OGRE_CONFIG_ENABLE_GLES2_GLSL_OPTIMISER)
		SET(GLES_OPTIMISER "-lglsl_optimizer -lmesa -lglcpp-library")
	endif()

    SET(ANDROID_TARGET "android-${ANDROID_SDK_API_LEVEL}")
    
    SET(NDKOUT "${CMAKE_BINARY_DIR}/SampleBrowserNDK")
    file(MAKE_DIRECTORY "${NDKOUT}")
    file(MAKE_DIRECTORY "${NDKOUT}/jni")
    file(MAKE_DIRECTORY "${NDKOUT}/assets")	
    file(MAKE_DIRECTORY "${NDKOUT}/res")
	file(MAKE_DIRECTORY "${NDKOUT}/libs")
	file(MAKE_DIRECTORY "${NDKOUT}/libs/${ANDROID_ABI}")
    file(WRITE "${NDKOUT}/default.properties" "target=${ANDROID_TARGET}")
	
	
    #file(WRITE "${NDKOUT}/jni/Application.mk" "APP_ABI := ${ANDROID_ABI}\nAPP_STL := gnustl_static ")
    configure_file("${OGRE_TEMPLATES_DIR}/AndroidManifest.xml.in" "${NDKOUT}/AndroidManifest.xml" @ONLY)	

	SET(ANDROID_MOD_NAME "OgreSampleBrowser")
	configure_file("${OGRE_TEMPLATES_DIR}/eclipse.project.in" "${NDKOUT}/.project" @ONLY)
	
	#SET(JNI_SRC "LOCAL_PATH := @CMAKE_SOURCE_DIR@/Samples/Browser/src\n")
	SET(JNI_SRC "LOCAL_PATH := @CMAKE_CURRENT_SOURCE_DIR@/src\n")
    SET(JNI_SRC "${JNI_SRC}\tLOCAL_SRC_FILES := SampleBrowser.cpp\n")
	
	if (0)
    if(NOT ANDROID_GLES_ONLY)
	 SET(SAMPLE_LDLIBS "LOCAL_LDLIBS += -lSample_BezierPatch\n")
	 configure_file("${OGRE_TEMPLATES_DIR}/Android.mk.in" "${NDKOUT}/jni/Android.mk" @ONLY)
    else()
     SET(SAMPLE_LDLIBS "LOCAL_LDLIBS += -lSample_BezierPatch\n")
     configure_file("${OGRE_TEMPLATES_DIR}/AndroidGLES1.mk.in" "${NDKOUT}/jni/Android.mk" @ONLY)
    endif()
	endif()
	

    configure_file("${OGRE_TEMPLATES_DIR}/Android_resources.cfg.in" "${NDKOUT}/assets/resources.cfg" @ONLY)
    configure_file("${OGRE_TEMPLATES_DIR}/samples.cfg.in" "${NDKOUT}/assets/samples.cfg" @ONLY)
    
    file(COPY "${OGRE_MEDIA_DIR}/models" DESTINATION "${NDKOUT}/assets")
    file(COPY "${OGRE_MEDIA_DIR}/particle" DESTINATION "${NDKOUT}/assets")
    file(COPY "${OGRE_MEDIA_DIR}/RTShaderLib" DESTINATION "${NDKOUT}/assets")
    file(COPY "${OGRE_MEDIA_DIR}/thumbnails" DESTINATION "${NDKOUT}/assets")
    file(COPY "${OGRE_MEDIA_DIR}/packs" DESTINATION "${NDKOUT}/assets")
    file(COPY "${OGRE_MEDIA_DIR}/materials" DESTINATION "${NDKOUT}/assets")
    
	file(COPY "${OGRE_SDK_DIR}/Android/drawable-hdpi" DESTINATION "${NDKOUT}/res")
	file(COPY "${OGRE_SDK_DIR}/Android/drawable-ldpi" DESTINATION "${NDKOUT}/res")
	file(COPY "${OGRE_SDK_DIR}/Android/drawable-mdpi" DESTINATION "${NDKOUT}/res")
	file(COPY "${OGRE_SDK_DIR}/Android/drawable-xhdpi" DESTINATION "${NDKOUT}/res")
		
	set_target_properties(SampleBrowser PROPERTIES    
		LIBRARY_OUTPUT_DIRECTORY ${NDKOUT}/libs/${ANDROID_ABI}
		RUNTIME_OUTPUT_DIRECTORY ${NDKOUT}/libs/${ANDROID_ABI}
		)	

else(ANDROID)

if (WIN32)
	set(RESOURCE_FILES
		${OGRE_SOURCE_DIR}/OgreMain/src/WIN32/resource.h
		${OGRE_SOURCE_DIR}/OgreMain/src/WIN32/OgreWin32Resources.rc
		${OGRE_SOURCE_DIR}/OgreMain/src/WIN32/winres.h
	)
	source_group(Resources FILES ${RESOURCE_FILES})
endif()	

#MESSAGE(STATUS "res:" ${RESOURCE_FILES} " static:" ${OGRE_STATIC} "ogre: " ${OGRE_SOURCE_DIR})

add_executable(SampleBrowser WIN32 ${HEADER_FILES} ${SOURCE_FILES} ${RESOURCE_FILES})

target_link_libraries(SampleBrowser ${OGRE_LIBRARIES} ${OGRE_PLUGIN_LIBRARIES} ${OIS_LIBRARIES} ${SAMPLE_LIBRARIES})

if (OGRE_BUILD_PLATFORM_NACL)
	SET_TARGET_PROPERTIES(SampleBrowser PROPERTIES OUTPUT_NAME "SampleBrowser_${OGREKIT_EXE_POSTFIX}")
endif()


# Add samples as dependencies
#add_dependencies(SampleBrowser ${OGRE_SAMPLES_LIST})

if (OGRE_STATIC)
	# Link to samples too
	target_link_libraries(SampleBrowser ${OGRE_SAMPLES_LIST})
endif()

if (SAMPLE_DEPENDENCIES)
  add_dependencies(SampleBrowser ${SAMPLE_DEPENDENCIES})
endif ()

# create resources.cfg
if (APPLE)
	configure_file(${OGRE_TEMPLATES_DIR}/resources.cfg.in ${OGRE_BINARY_DIR}/bin/debug/resources.cfg)
else()
	configure_file(${OGRE_TEMPLATES_DIR}/resources_d.cfg.in ${OGRE_BINARY_DIR}/bin/debug/resources_d.cfg)
endif()
configure_file(${OGRE_TEMPLATES_DIR}/resources.cfg.in ${OGRE_BINARY_DIR}/bin/release/resources.cfg)
configure_file(${OGRE_TEMPLATES_DIR}/resources.cfg.in ${OGRE_BINARY_DIR}/bin/relwithdebinfo/resources.cfg)
configure_file(${OGRE_TEMPLATES_DIR}/resources.cfg.in ${OGRE_BINARY_DIR}/bin/minsizerel/resources.cfg)
# create samples.cfg
if (APPLE)
	configure_file(${OGRE_TEMPLATES_DIR}/samples.cfg.in ${OGRE_BINARY_DIR}/bin/debug/samples.cfg)
else()
	configure_file(${OGRE_TEMPLATES_DIR}/samples_d.cfg.in ${OGRE_BINARY_DIR}/bin/debug/samples_d.cfg)
endif()
configure_file(${OGRE_TEMPLATES_DIR}/samples.cfg.in ${OGRE_BINARY_DIR}/bin/release/samples.cfg)
configure_file(${OGRE_TEMPLATES_DIR}/samples.cfg.in ${OGRE_BINARY_DIR}/bin/relwithdebinfo/samples.cfg)
configure_file(${OGRE_TEMPLATES_DIR}/samples.cfg.in ${OGRE_BINARY_DIR}/bin/minsizerel/samples.cfg)


# configure SampleBrowser build

ogre_config_common(SampleBrowser)


# append _d for debug builds
#if (NOT APPLE)
	#set_property(TARGET SampleBrowser APPEND PROPERTY DEBUG_POSTFIX "_d")
#endif ()

# set install RPATH for Unix systems
if (UNIX AND OGRE_FULL_RPATH)
	set_property(TARGET SampleBrowser APPEND PROPERTY
		INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/${OGRE_LIB_DIRECTORY})
	set_property(TARGET SampleBrowser PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
endif ()

if (APPLE)
	set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/SampleBrowser.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")

	# On OS X, create .app bundle
	set_property(TARGET SampleBrowser PROPERTY MACOSX_BUNDLE TRUE)
	set_property(TARGET SampleBrowser PROPERTY MACOSX_BUNDLE_ICON_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Common/misc/SampleBrowser_OSX.icns)
	set_property(TARGET SampleBrowser PROPERTY MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Common/misc/Info.plist)
	set(MACOSX_BUNDLE_GUI_IDENTIFIER "org.ogre3d.\${PRODUCT_NAME:rfc1034identifier}")
    set_target_properties(SampleBrowser PROPERTIES XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH "NO")

  if (OGRE_BUILD_PLATFORM_APPLE_IOS)
    set_target_properties(SampleBrowser PROPERTIES XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2")
    set(OGRE_SAMPLE_CONTENTS_PATH ${OGRE_BINARY_DIR}/bin/$(CONFIGURATION)/SampleBrowser.app)
    add_custom_command(TARGET SampleBrowser POST_BUILD
      COMMAND ditto ${CMAKE_CURRENT_SOURCE_DIR}/Common/misc/*.png ${OGRE_SAMPLE_CONTENTS_PATH}/
      COMMAND ditto ${OGRE_BINARY_DIR}/bin/*.cfg ${OGRE_SAMPLE_CONTENTS_PATH}/
    )
    if(OGRE_SDK_BUILD)
	    add_custom_command(TARGET SampleBrowser POST_BUILD
          COMMAND mkdir ARGS -p ${OGRE_BINARY_DIR}/lib/$(CONFIGURATION)/
	      COMMAND ditto ${OGRE_BINARY_DIR}/Media ${OGRE_SAMPLE_CONTENTS_PATH}/Media
	    )
    else()
        add_custom_command(TARGET SampleBrowser POST_BUILD
          COMMAND mkdir ARGS -p ${OGRE_BINARY_DIR}/lib/$(CONFIGURATION)/
          COMMAND ditto ${OGRE_MEDIA_DIR} ${OGRE_SAMPLE_CONTENTS_PATH}/Media
        )
    endif(OGRE_SDK_BUILD)
    
  else()

    include_directories(
        ${OGRE_SOURCE_DIR}/RenderSystems/GL/include
        ${OGRE_SOURCE_DIR}/RenderSystems/GL/include/OSX
        )

	target_link_libraries(SampleBrowser ${OPENGL_LIBRARIES} ${CoreVideo_LIBRARIES})

   set_target_properties(SampleBrowser PROPERTIES
        LINK_FLAGS "-framework IOKit -framework Cocoa -framework Carbon -framework CoreVideo -framework ApplicationServices")

    # Add the path where the Ogre framework was found, if it has been found somewhere
    if(NOT ${OGRE_FRAMEWORK_PATH} STREQUAL "" AND OGRE_SDK_BUILD)
      set_target_properties(SampleBrowser PROPERTIES
        COMPILE_FLAGS "-F${OGRE_FRAMEWORK_PATH}"
        LINK_FLAGS "-F${OGRE_FRAMEWORK_PATH} -framework IOKit -framework Cocoa -framework Carbon -framework OpenGL -framework CoreVideo"
      )
    endif()
	# also, symlink frameworks so .app is standalone
	# NOTE: $(CONFIGURATION) is not resolvable at CMake run time, it's only 
	# valid at build time (hence parenthesis rather than braces)
	
    set(OGRE_OSX_BUILD_CONFIGURATION "$(PLATFORM_NAME)/$(CONFIGURATION)")
    set(OGRE_BUILT_FRAMEWORK "$(PLATFORM_NAME)/$(CONFIGURATION)")
	set (OGRE_SAMPLE_CONTENTS_PATH 
		${OGRE_BINARY_DIR}/bin/$(CONFIGURATION)/SampleBrowser.app/Contents)
	add_custom_command(TARGET SampleBrowser POST_BUILD
		COMMAND mkdir ARGS -p ${OGRE_SAMPLE_CONTENTS_PATH}/Frameworks
		COMMAND ln ARGS -s -f ${OGRE_BINARY_DIR}/lib/${OGRE_BUILT_FRAMEWORK}/Ogre.framework 
		${OGRE_SAMPLE_CONTENTS_PATH}/Frameworks/
		COMMAND ln ARGS -s -f ${Cg_LIBRARY_REL}
		${OGRE_SAMPLE_CONTENTS_PATH}/Frameworks/
		)
	# now cfg files
	if(OGRE_SDK_BUILD)
	add_custom_command(TARGET SampleBrowser POST_BUILD
		COMMAND mkdir ARGS -p ${OGRE_SAMPLE_CONTENTS_PATH}/Resources
		COMMAND ln ARGS -s -f ${OGRE_BINARY_DIR}/bin/macosx/plugins.cfg 
		${OGRE_SAMPLE_CONTENTS_PATH}/Resources/
		COMMAND ln ARGS -s -f ${OGRE_BINARY_DIR}/bin/macosx/resources.cfg 
		${OGRE_SAMPLE_CONTENTS_PATH}/Resources/
		COMMAND ln ARGS -s -f ${OGRE_BINARY_DIR}/bin/macosx/samples.cfg 
		${OGRE_SAMPLE_CONTENTS_PATH}/Resources/
		COMMAND ln ARGS -s -f ${OGRE_BINARY_DIR}/bin/macosx/quakemap.cfg 
		${OGRE_SAMPLE_CONTENTS_PATH}/Resources/
		COMMAND ln ARGS -s -f ${OGRE_SOURCE_DIR}/Media
		${OGRE_SAMPLE_CONTENTS_PATH}/Resources/
        COMMAND ditto 
		${OGRE_SOURCE_DIR}/Samples/Common/misc/SampleBrowser_OSX.icns ${OGRE_SAMPLE_CONTENTS_PATH}/Resources
		)
	else()
	add_custom_command(TARGET SampleBrowser POST_BUILD
		COMMAND mkdir ARGS -p ${OGRE_SAMPLE_CONTENTS_PATH}/Resources
		COMMAND ln ARGS -s -f ${OGRE_BINARY_DIR}/bin/plugins.cfg 
		${OGRE_SAMPLE_CONTENTS_PATH}/Resources/
		COMMAND ln ARGS -s -f ${OGRE_BINARY_DIR}/bin/resources.cfg 
		${OGRE_SAMPLE_CONTENTS_PATH}/Resources/
		COMMAND ln ARGS -s -f ${OGRE_BINARY_DIR}/bin/samples.cfg 
		${OGRE_SAMPLE_CONTENTS_PATH}/Resources/
		COMMAND ln ARGS -s -f ${OGRE_BINARY_DIR}/bin/quakemap.cfg 
		${OGRE_SAMPLE_CONTENTS_PATH}/Resources/
		COMMAND ln ARGS -s -f ${CMAKE_CURRENT_SOURCE_DIR}/Media
		${OGRE_SAMPLE_CONTENTS_PATH}/Resources/
        COMMAND ditto 
		${CMAKE_CURRENT_SOURCE_DIR}/Common/misc/SampleBrowser_OSX.icns ${OGRE_SAMPLE_CONTENTS_PATH}/Resources
		)
	endif()

	# now plugins
	add_custom_command(TARGET SampleBrowser POST_BUILD
		COMMAND mkdir ARGS -p ${OGRE_SAMPLE_CONTENTS_PATH}/Frameworks
		COMMAND mkdir ARGS -p ${OGRE_SAMPLE_CONTENTS_PATH}/Plugins)

    set(FRAMEWORKS OgreOverlay
        OgrePaging
        OgreProperty
        OgreRTShaderSystem
        OgreTerrain
        OgreVolume
        Plugin_BSPSceneManager
        Plugin_CgProgramManager
        Plugin_OctreeSceneManager
        Plugin_OctreeZone
        Plugin_ParticleFX
        Plugin_PCZSceneManager
        RenderSystem_GL
        RenderSystem_GL3Plus
    )

    foreach(FWK ${FRAMEWORKS})
	add_custom_command(TARGET SampleBrowser POST_BUILD
        COMMAND ARGS if [ -d ${OGRE_BINARY_DIR}/lib/${OGRE_OSX_BUILD_CONFIGURATION}/${FWK}.framework ]\; then ln -s -f ${OGRE_BINARY_DIR}/lib/${OGRE_OSX_BUILD_CONFIGURATION}/${FWK}.framework ${OGRE_SAMPLE_CONTENTS_PATH}/Frameworks/${FWK}.framework\; fi
		)
    endforeach(FWK)

	# Add links for the sample plugins
    foreach(SAMPLE_PLUGIN ${OGRE_SAMPLES_LIST})
	add_custom_command(TARGET SampleBrowser POST_BUILD
		COMMAND ln ARGS -s -f ${OGRE_BINARY_DIR}/lib/${OGRE_OSX_BUILD_CONFIGURATION}/${SAMPLE_PLUGIN}.dylib
		${OGRE_SAMPLE_CONTENTS_PATH}/Plugins/
		)
    endforeach(SAMPLE_PLUGIN)

	if (OGRE_BUILD_TESTS)
	  add_custom_command(TARGET SampleBrowser POST_BUILD
		COMMAND ln ARGS -s -f ${OGRE_BINARY_DIR}/lib/${OGRE_OSX_BUILD_CONFIGURATION}/PlayPen.dylib 
		${OGRE_SAMPLE_CONTENTS_PATH}/Plugins/
		)
	endif()
  endif()
endif (APPLE)
	
endif(ANDROID)


