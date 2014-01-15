# ------------OLD---------------------------------------------
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

find_package(Cg)

set(OgreLite 1.0)
set (OGRE_CONFIG_HEADER 1)
set (OGRE_SDK_BUILD 1)
set(OGRE_THREAD_PROVIDER  0)
set(OGRE_LIB_TYPE         STATIC)
#set(OGRE_LIB_TYPE         SHARED)
#set(OGRE_STATIC	FALSE)
SET(OGRE_BINARY_DIR       ${OGREKIT_BINARY_DIR})
SET(OGRE_SOURCE_DIR       ${OgreLite_SOURCE_DIR})
set(OGRE_BUILD_COMPONENT_OVERLAY TRUE)
if (OGREKIT_USE_PARTICLE)
	set(OGRE_BUILD_PLUGIN_PFX        TRUE)
else()
	set(OGRE_BUILD_PLUGIN_PFX        FALSE)
endif()
	
set(OGRE_CONFIG_ENABLE_FREEIMAGE TRUE)
set(OGRE_CONFIG_ENABLE_DDS       TRUE)
set(OGRE_SET_DISABLE_LIBCPP_SUPPORT 0)

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

set(OGRE_CONFIG_ENABLE_ETC TRUE)


if (Cg_FOUND AND NOT OGREKIT_BUILD_MOBILE) 
    option(OGRE_BUILD_PLUGIN_CG                 "Enable / Disable Ogre cgProgramManager" OFF)
else()
    set(OGRE_BUILD_PLUGIN_CG 0)
endif()

set(OGRE_SET_ASSERT_MODE 0) 
set(OGRE_SET_DOUBLE 0)
set(OGRE_SET_ALLOCATOR ${OGRE_CONFIG_ALLOCATOR})
set(OGRE_SET_CONTAINERS_USE_ALLOCATOR 0)
set(OGRE_SET_STRING_USE_ALLOCATOR 0)
set(OGRE_SET_MEMTRACK_DEBUG 0)
set(OGRE_SET_MEMTRACK_RELEASE 0)
set(OGRE_SET_THREADS 0)
set(OGRE_SET_ALLOCATOR 4)
set(OGRE_SET_THREAD_PROVIDER ${OGRE_THREAD_PROVIDER})
set(OGRE_SET_DISABLE_FREEIMAGE 0)
set(OGRE_SET_DISABLE_DDS 0)
set(OGRE_SET_DISABLE_PVRTC 0)
set(OGRE_SET_DISABLE_ETC 0)
set(OGRE_SET_DISABLE_ZIP 0)
set(OGRE_SET_DISABLE_VIEWPORT_ORIENTATIONMODE 1)
set(OGRE_SET_DISABLE_GLES2_CG_SUPPORT 0)
#todo test this
set(OGRE_SET_DISABLE_GLES2_VAO_SUPPORT 0)
set(OGRE_SET_DISABLE_GL_STATE_CACHE_SUPPORT 1)
set(OGRE_SET_DISABLE_GLES2_GLSL_OPTIMISER 0)
set(OGRE_SET_NEW_COMPILERS 0)
set(OGRE_STATIC_LIB 0)
set(OGRE_SET_USE_BOOST 0)
set(OGRE_SET_PROFILING 0)
if (OGRE_CONFIG_DOUBLE)
  set(OGRE_SET_DOUBLE 1)
endif()
if (OGRE_CONFIG_CONTAINERS_USE_CUSTOM_ALLOCATOR)
  set(OGRE_SET_CONTAINERS_USE_ALLOCATOR 1)
endif ()
if (OGRE_CONFIG_STRING_USE_CUSTOM_ALLOCATOR)
  set(OGRE_SET_STRING_USE_ALLOCATOR 1)
endif ()
if (OGRE_CONFIG_MEMTRACK_DEBUG)
  set(OGRE_SET_MEMTRACK_DEBUG 1)
endif()
if (OGRE_CONFIG_MEMTRACK_RELEASE)
  set(OGRE_SET_MEMTRACK_RELEASE 1)
endif()
if (NOT OGRE_CONFIG_ENABLE_FREEIMAGE)
  set(OGRE_SET_DISABLE_FREEIMAGE 1)
endif()
if (NOT OGRE_CONFIG_ENABLE_DDS)
  set(OGRE_SET_DISABLE_DDS 1)
endif()
if (NOT OGRE_CONFIG_ENABLE_PVRTC)
  set(OGRE_SET_DISABLE_PVRTC 1)
endif()
if (NOT OGRE_CONFIG_ENABLE_ETC)
  set(OGRE_SET_DISABLE_ETC 1)
endif()
if (NOT OGRE_CONFIG_ENABLE_ZIP)
  set(OGRE_SET_DISABLE_ZIP 1)
endif() 
if (OGRE_CONFIG_ENABLE_VIEWPORT_ORIENTATIONMODE)
  set(OGRE_SET_DISABLE_VIEWPORT_ORIENTATIONMODE 0)
endif()
if (NOT OGRE_CONFIG_ENABLE_GLES2_CG_SUPPORT)
  set(OGRE_SET_DISABLE_GLES2_CG_SUPPORT 1)
endif()
if (NOT OGRE_CONFIG_ENABLE_GLES2_GLSL_OPTIMISER)
  set(OGRE_SET_DISABLE_GLES2_GLSL_OPTIMISER 1)
endif()
if(OGRE_CONFIG_NEW_COMPILERS)
  set(OGRE_SET_NEW_COMPILERS 1)
endif()
if(NOT OGRE_CONFIG_ENABLE_GLES2_VAO_SUPPORT)
  set(OGRE_SET_DISABLE_GLES2_VAO_SUPPORT 1)
endif()
if (NOT OGRE_CONFIG_ENABLE_GL_STATE_CACHE_SUPPORT)
  set(OGRE_SET_DISABLE_GL_STATE_CACHE_SUPPORT 1)
endif()
if (OGRE_STATIC)
  set(OGRE_STATIC_LIB 1)
endif()
if (OGRE_USE_BOOST)
  set(OGRE_SET_USE_BOOST 1)
endif()
if (OGRE_PROFILING)
  set(OGRE_SET_PROFILING 1)
endif()

if (OGRE_TEST_BIG_ENDIAN)
  set(OGRE_CONFIG_BIG_ENDIAN 1)
else ()
  set(OGRE_CONFIG_LITTLE_ENDIAN 1)
endif ()

set(OGRE_SET_DISABLE_GLES3_SUPPORT 1)
#set(OGRE_NO_GLES3_SUPPORT TRUE CACHE BOOL "" FORCE)


# determine Ogre version numbers
include(OgreGetVersion)
ogre_get_version(${OGRE_SOURCE_DIR}/OgreMain/include/OgrePrerequisites.h)
message(STATUS "Configuring OGRE ${OGRE_VERSION}")

configure_file(${OGRE_TEMPLATES_DIR}/OgreBuildSettings.h.in ${OGREKIT_BINARY_DIR}/include/OgreBuildSettings.h)

# Configure version file for use by scripts
#configure_file("${OGRE_TEMPLATES_DIR}/version.txt.in" "${OGRE_BINARY_DIR}/version.txt" @ONLY)


include_directories(${OGRELITE_SOURCE_DIR}/OgreMain/include)
include_directories(${OGREKIT_BINARY_DIR}/include)
include_directories(${OGREKIT_FREETYPE_INCLUDE} ${OGREKIT_FREEIMAGE_INCLUDE} ${OGREKIT_ZZIP_INCLUDE} ${OGREKIT_ZLIB_INCLUDE} )

if (APPLE)
  if (OGRE_BUILD_PLATFORM_IPHONE)
	include_directories(${OGRELITE_SOURCE_DIR}/OgreMain/include/iPhone)
	include_directories(${OGRELITE_SOURCE_DIR}/OgreMain/include/OSX)
  else ()
	include_directories(${OGRELITE_SOURCE_DIR}/OgreMain/include/OSX)
  endif()
endif()

subdirs(OgreMain RenderSystems PlugIns Components)
