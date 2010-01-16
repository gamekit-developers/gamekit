# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

#SET(CMAKE_SUPPRESS_REGENERATION  1)
#SET(CMAKE_USE_RELATIVE_PATHS 1)

project(OGREKIT)
SET(OGREKIT 1.0)

if(COMMAND cmake_policy)
   cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)


# ---------------------------------------------------------
set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/CMake
    ${CMAKE_SOURCE_DIR}/CMake/Utils
    ${CMAKE_SOURCE_DIR}/CMake/Packages
    ${CMAKE_SOURCE_DIR}/CMake/Templates
    )
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)
include(ConfigOgreKit)
include(ConfigGameKit)


configure_gamekit(${CMAKE_SOURCE_DIR}/GameKit)
configure_ogrekit(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/OgreLite)

set(OGREKIT_BULLET_LIBS BulletDynamics BulletCollision BulletSoftBody LinearMath)
set(OGREKIT_BULLET_INCLUDE ${CMAKE_SOURCE_DIR}/bullet/src)

subdirs(GameKit)
subdirs(bullet)

subdirs(Dependencies)
subdirs(OgreLite)
subdirs(OgreKit)


# Depreciated 
# subdirs(Core)

# Todo Implement a loader
# subdirs(Tests/Ascii/bAscii)
# subdirs(Tests/Ascii)

# Todo, update for new demo system
# subdirs(Tests/ScriptTest)
