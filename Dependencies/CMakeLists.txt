# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)



project(Dependencies)
SET(Dependencies 1.0)
subdirs(Source/Blender25)


include(NoWarn)
subdirs(Source/FreeImage)
subdirs(Source/OIS)
subdirs(Source/FreeType)
subdirs(Source/Recast)
subdirs(Source/Detour)
subdirs(Source/OpenSteer)



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
