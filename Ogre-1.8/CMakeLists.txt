# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6.2)
#set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
#cmake_policy(SET CMP0003 NEW)

# CMake 2.8.2 has a bug that creates unusable Xcode projects when using ARCHS_STANDARD_32_BIT
# to specify both armv6 and armv7.
if(OGRE_BUILD_PLATFORM_APPLE_IOS AND (CMAKE_VERSION VERSION_EQUAL 2.8.2) AND (CMAKE_GENERATOR STREQUAL "Xcode"))
	message(FATAL_ERROR "CMake 2.8.2 cannot create compatible Xcode projects for iOS, please download the latest version or an older release from http://www.cmake.org/files/")
endif()

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


if (OGREKIT_DISABLE_ZIP)
	set(OGRE_CONFIG_ENABLE_ZIP FALSE)
else()
	set(OGRE_CONFIG_ENABLE_ZIP TRUE)
endif()

if (OGREKIT_BUILD_ANDROID)
	add_definitions(-DNO_MALLINFO=1) #disable nedmalloc struct mallinfo declaration
endif()


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


# Configure settings
if (OGREKIT_DISABLE_ZIP)
	set(OGRE_NO_ZIP_ARCHIVE 1)
else()
	set(OGRE_NO_ZIP_ARCHIVE 0)
endif()
configure_file(${OgreLite_SOURCE_DIR}/Settings/OgreBuildSettings.h.in ${OGREKIT_BINARY_DIR}/Settings/OgreBuildSettings.h)

# determine Ogre version numbers
include(OgreGetVersion)
ogre_get_version(${OGRE_SOURCE_DIR}/OgreMain/include/OgrePrerequisites.h)
message(STATUS "Configuring OGRE ${OGRE_VERSION}")
# Configure version file for use by scripts
#configure_file("${OGRE_TEMPLATES_DIR}/version.txt.in" "${OGRE_BINARY_DIR}/version.txt" @ONLY)


include_directories(${OGRELITE_SOURCE_DIR}/OgreMain/include)
include_directories(${OGREKIT_BINARY_DIR}/Settings)

if (APPLE)
  if (OGRE_BUILD_PLATFORM_IPHONE)
	include_directories(${OGRELITE_SOURCE_DIR}/OgreMain/include/iPhone)
  else ()
	include_directories(${OGRELITE_SOURCE_DIR}/OgreMain/include/OSX)
  endif()
endif()

subdirs(OgreMain RenderSystems PlugIns Components)
