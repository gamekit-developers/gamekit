# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

project(OgreLite)
include(PrecompiledHeader)

set(OgreLite 1.0)

set(OGRE_THREAD_PROVIDER  0)
set(OGRE_LIB_TYPE         STATIC)
SET(OGRE_BINARY_DIR       ${OGREKIT_BINARY_DIR})
SET(OGRE_SOURCE_DIR       ${OgreLite_SOURCE_DIR})

set(OGRE_BUILD_PLUGIN_PFX        TRUE)
set(OGRE_CONFIG_ENABLE_FREEIMAGE TRUE)
set(OGRE_CONFIG_ENABLE_DDS       TRUE)

## Keep Zip support ? 
set(OGRE_CONFIG_ENABLE_ZIP       TRUE)



if (CMAKE_COMPILER_IS_GNUCXX)

	check_cxx_compiler_flag(-msse OGRE_GCC_HAS_SSE)

	if (OGRE_GCC_HAS_SSE AND NOT OGRE_BUILD_PLATFORM_IPHONE)
		add_definitions(-msse)
	endif()

	if (WIN32)

		# So Mingw w32api-3.14 can find 
		# MonitorFromPoint in the Render system
		add_definitions(-D_WIN32_WINNT=0x0500)

	endif(WIN32)

endif (CMAKE_COMPILER_IS_GNUCXX)



include_directories(${OGRELITE_SOURCE_DIR}/OgreMain/include)
include_directories(${OGRELITE_SOURCE_DIR}/Settings)

if (APPLE)
  if (OGRE_BUILD_PLATFORM_IPHONE)
	include_directories(${OGRELITE_SOURCE_DIR}/OgreMain/include/iPhone)
  else ()
	include_directories(${OGRELITE_SOURCE_DIR}/OgreMain/include/OSX)
  endif()
endif()

subdirs(OgreMain RenderSystems PlugIns)
