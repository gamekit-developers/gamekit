# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

project(Dependencies)
SET(Dependencies 1.0)

if (OGREKIT_COMPILE_ENET)
	subdirs(Source/enet/)
endif()

if (OGREKIT_CREATE_OGRE_DEPENDENCY_DIR)
	set(OGRE_DEPENDENCIES_DIR ${CMAKE_BINARY_DIR}/PrebuiltDependencies)
	if (OGREKIT_BUILD_ANDROID)
		set(OGRE_PREBUILT_DEPENDENCY_LIB_DIR ${OGRE_DEPENDENCIES_DIR}/lib/${ANDROID_ABI})
	else()
		set(OGRE_PREBUILT_DEPENDENCY_LIB_DIR ${OGRE_DEPENDENCIES_DIR}/lib)
	endif()
	set(OGRE_PREBUILT_DEPENDENCY_INCLUDE_DIR ${OGRE_DEPENDENCIES_DIR}/include)
	
	file(MAKE_DIRECTORY ${OGRE_PREBUILT_DEPENDENCY_LIB_DIR})
	file(MAKE_DIRECTORY ${OGRE_PREBUILT_DEPENDENCY_INCLUDE_DIR})
endif()


subdirs(Source/GameKit/Utils)
subdirs(Source/GameKit/AnimKit)

if (OGREKIT_USE_BPARSE)
    include_directories( GameKit/Utils )
	subdirs(Source/Blender25)
endif()

if (OGREKIT_HEADER_GENERATOR)
	subdirs(Source/Blender25/Headers)
endif()

include(NoWarn)
subdirs(Source/OIS)
subdirs(Source/FreeType)

if (OGREKIT_COMPILE_OPENSTEER)
	subdirs(Source/OpenSteer)
endif()

if (OGREKIT_COMPILE_RECAST)
	subdirs(Source/Recast)
	subdirs(Source/Detour)
endif()

if (OGREKIT_USE_STATIC_FREEIMAGE)
	subdirs(Source/FreeImage)
endif()

if (NOT OGREKIT_DISABLE_ZIP)
	subdirs(Source/ZZipLib)
endif()

if (OGREKIT_USE_LUA)
	subdirs(Source/Lua)
endif()

if (OGREKIT_OPENAL_SOUND)
	subdirs(Source/Codecs)
	
	if (WIN32)
		subdirs(Source/OpenAL)
	endif()
endif()

if (OGREKIT_COMPILE_TINYXML)
    subdirs(Source/TinyXml)
endif()

if (OGREKIT_COMPILE_LIBROCKET)
	subdirs(Source/libRocket/Build)
endif()
