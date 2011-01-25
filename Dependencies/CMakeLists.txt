# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)



project(Dependencies)
SET(Dependencies 1.0)
subdirs(Source/Blender25)
subdirs(Source/GameKit/Utils)
subdirs(Source/GameKit/AnimKit)


include(NoWarn)
subdirs(Source/OIS)
subdirs(Source/FreeType)
subdirs(Source/Recast)
subdirs(Source/Detour)
subdirs(Source/OpenSteer)
subdirs(Source/opts)

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

