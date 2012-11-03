#------------------------------------------------------------------------------
#   This file is part of FBT (File Binary Tables).
#   http://gamekit.googlecode.com/
#
#   Copyright (c) 2010 Charlie C & Erwin Coumans.
#
#------------------------------------------------------------------------------
# This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.
#------------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.4)
if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project(FBT)

list(APPEND CMAKE_MODULE_PATH ${FBT_SOURCE_DIR}/CMake)


#set(FBT_USE_BLEND_TEST 0)
#set(FBT_USE_ZLIB 0)
#set(FBT_ZLIB_INCLUDE )
#set(FBT_ZLIB_LIBRARY )

if(OGREKIT_ZLIB_TARGET) #use ZLib in Ogrekit dependence libraries
	set(FBT_USE_ZLIB 1)
	set(FBT_ZLIB_INCLUDE ${OGREKIT_ZLIB_INCLUDE})
	set(FBT_ZLIB_LIBRARY ${OGREKIT_ZLIB_TARGET})	
else()
	message(STATUS "Can't found ZLib: ftbFile don't support compressed file.")
endif()

option(FBT_BLEND_STRIP "Compile .blend file stripper" OFF)

if (FBT_USE_ZLIB)
    add_definitions(-DFBT_USE_GZ_FILE=1)
endif()

#add_definitions(-DFFBT_USE_COMPILER_CHECKS=0)
#add_definitions(-DFBT_USE_DEBUG_ASSERTS=0)


subdirs(File)
subdirs(FileFormats/Blend)

if (NOT OGREKIT_BUILD_IPHONE AND NOT OGREKIT_BUILD_ANDROID)
subdirs(Compiler)
subdirs(FileFormats/Bullet)
endif()

if (FBT_USE_BLEND_TEST)
	subdirs(FileTest)
endif()

if (FBT_BLEND_STRIP)
	subdirs(Strip)
endif()

#if (OGREKIT_HEADER_GENERATOR)
#	subdirs(Headers)
#endif()