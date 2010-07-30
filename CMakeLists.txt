# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

#SET(CMAKE_SUPPRESS_REGENERATION  1)
#SET(CMAKE_USE_RELATIVE_PATHS 1)

project(OGREKIT)
SET(OGREKIT 1.0)

if(COMMAND cmake_policy)
   cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

if (MSVC)
	add_definitions( -D_CRT_SECURE_NO_WARNINGS )
	add_definitions( -D_CRT_SECURE_NO_DEPRECATE )
endif()

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

configure_gamekit(${CMAKE_SOURCE_DIR}/Dependencies/Source/Blender25)
configure_ogrekit(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/OgreLite)

set(OGREKIT_BULLET_LIBS BulletDynamics BulletCollision BulletSoftBody LinearMath)
set(OGREKIT_BULLET_INCLUDE ${CMAKE_SOURCE_DIR}/bullet/src)

# ---------------------------------------------------------
# Common base paths
set(OGREKIT_INCLUDE ${OGREKIT_OGRE_INCLUDE}
                    ${OGREKIT_SOURCE_DIR}/Engine
                    ${OGREKIT_SOURCE_DIR}/Engine/AI
                    ${OGREKIT_BULLET_INCLUDE}
                    ${OGREKIT_DEP_INCLUDE}
                    )
set(OGREKIT_LIB             OgreKitCore ${OGREKIT_LUA_TARGET})
set(OGREKIT_STD_LUA_LIB     OgreKitCore ${OGREKIT_LUA_STD_TARGET})


subdirs(bullet)
subdirs(Dependencies)
subdirs(OgreLite)
subdirs(Engine)
subdirs(Samples)
subdirs(Docs/LuaAPI)
