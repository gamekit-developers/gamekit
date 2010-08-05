# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)



project(Dependencies)
SET(Dependencies 1.0)

subdirs(Source/FreeImage)
subdirs(Source/OIS)
subdirs(Source/ZZipLib)
subdirs(Source/FreeType)
subdirs(Source/Recast)
subdirs(Source/Detour)
subdirs(Source/OpenSteer)
subdirs(Source/Blender25)
subdirs(Source/Lua)


if (OGREKIT_BUILD_V8)
    subdirs(Source/V8)
endif()

if (OGREKIT_COMPLIE_SWIG)
    subdirs(Source/Swig)
endif()

if (OGREKIT_OPENAL_SOUND)
    subdirs(Source/Codecs)
    
    if (WIN32)
		subdirs(Source/OpenAL)
    endif()
endif()
