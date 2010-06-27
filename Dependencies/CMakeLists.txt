# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)



project(Dependencies)
SET(Dependencies 1.0)

subdirs(Source/FreeImage)
subdirs(Source/OIS)
subdirs(Source/ZZipLib)
subdirs(Source/FreeType)
subdirs(Source/Lua)
subdirs(Source/Recast)
subdirs(Source/Detour)
subdirs(Source/OpenSteer)

if (OGREKIT_OPENAL_SOUND)
    subdirs(Source/Codecs)
endif()
