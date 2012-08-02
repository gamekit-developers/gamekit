cmake_minimum_required(VERSION 2.4)

PROJECT(BULLET_PHYSICS)
SET(BULLET_VERSION 2.75)

IF (NOT CMAKE_BUILD_TYPE)
# SET(CMAKE_BUILD_TYPE "Debug")
 SET(CMAKE_BUILD_TYPE "Release")
ENDIF (NOT CMAKE_BUILD_TYPE) 

   
IF(COMMAND cmake_policy)
   cmake_policy(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

if (OGREKIT_BUILD_IPHONE)    
	add_definitions(-DARM_NEON_GCC_COMPATIBILITY=1)
endif()

SUBDIRS(src/BulletCollision src/BulletDynamics src/LinearMath )

if (OGREKIT_COMPILE_SOFTBODY)
	SUBDIRS(src/BulletSoftBody)
endif()
